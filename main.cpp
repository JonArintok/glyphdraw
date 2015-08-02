
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <CL/cl.h>
#include <SDL2/SDL.h>
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::ifstream;
using std::stringstream;
#include "01_error.hpp"
#include "02_CLtypes.hpp"
#include "03_sharedWithCL.h"
#include "04_initCL.hpp"
#include "05_foundation.hpp"
#include "06_testRoots.hpp"

/*
class scrollable {
  float accel;
  //float vertScrollRailThresh = 0.8;
  float2 boundary;
  float2 overBounds;
  float2 pOverBounds;
  float2 vel;
  float2 pos;
  float2 pPos;
  float2 boundaryBRC = scrollBoundary;
public:
  scrollable(const float2 b, const float a) : boundary(b), accel(a) {
    boundaryBRC = scrollBoundary;
  }
}*/

float overBound(const float tl, const float br, const float winSize) {
  return tl > 0 ? tl : (br < winSize ? br - winSize : 0);
}
float ternaryReduc(const float in) {return in ? (in > 0 ? 1 : -1) : 0;}
bool crossedZero(const float a, const float b) {
  return (a > 0 && b <= 0) || (a < 0 && b >= 0);
}
float closeEnough(const float in) {
  return in < 0.1 && in > -0.1 ? 0 : in;
}
float rebound(
  const float tl,
  const float br,
  const float winSize,
  const float vel,
  const float accel
) {
  const float overBounds = overBound(tl, br, winSize);
  if (!overBounds) return vel;
  const float reVel = vel + ternaryReduc(overBounds)*accel*-1;
  const float nOverBounds = overBound(tl+reVel, br+reVel, winSize);
  const float out = crossedZero(overBounds, nOverBounds) ?
    0-overBounds : reVel;
  cout << "tl: " << tl << endl;
  cout << "br: " << br << endl;
  cout << "overBounds: " << overBounds << endl;
  cout << "reVel: " << reVel << endl;
  cout << "nOverBounds: " << nOverBounds << endl;
  cout << "out: " << out << endl;
  return closeEnough(out);
}

int main(int argc, char* argv[]) {

  const int2 videoSize = int2(1280, 720);

  const cl_uint    maxDevices = 8;
  cl_device_id     computeDevices[maxDevices];
  cl_context       context = NULL;
  cl_command_queue commandQueue = NULL;
  initOpenCL(computeDevices, maxDevices, context, commandQueue);
  checkCLerror(__LINE__, __FILE__);

  cl_program program;
  vector<const char*> paths = {"03_sharedWithCL.h", "UIshader.cl"};
  initClProgram(paths, program, context, computeDevices);
  checkCLerror(__LINE__, __FILE__);
  cl_kernel kernel = clCreateKernel(program, "UIshader", &CLstatus);
  checkCLerror(__LINE__, __FILE__);

  cl_mem outputImage;
  {
    cl_image_format outputImageFormat = {CL_RGBA, CL_UNORM_INT8};
    cl_image_desc outputImageDesc;
    memset(&outputImageDesc, '\0', sizeof(cl_image_desc));
    outputImageDesc.image_type   = CL_MEM_OBJECT_IMAGE2D;
    outputImageDesc.image_width  = videoSize.x;
    outputImageDesc.image_height = videoSize.y;
    outputImage = clCreateImage(
      context,               //cl_context             context,
      CL_MEM_WRITE_ONLY,     //cl_mem_flags           flags,
      &outputImageFormat,    //const cl_image_format *image_format,
      &outputImageDesc,      //const cl_image_desc   *image_desc,
      NULL,                  //void                  *host_ptr,
      &CLstatus              //cl_int                *errcode_ret
    );
    checkCLerror(__LINE__, __FILE__);
  }


  initGlyphSheet("glyphSheets/hermit_010_023");
  {
    cl_image_format glyphSheetFormat = {CL_LUMINANCE, CL_UNORM_INT8};
    cl_image_desc glyphSheetDesc;
    memset(&glyphSheetDesc, '\0', sizeof(cl_image_desc));
    glyphSheetDesc.image_type   = CL_MEM_OBJECT_IMAGE2D;
    glyphSheetDesc.image_width  = gss->w;
    glyphSheetDesc.image_height = gss->h;
    glyphSheet = clCreateImage(
      context,               //cl_context             context,
      CL_MEM_READ_ONLY,      //cl_mem_flags           flags,
      &glyphSheetFormat,     //const cl_image_format *image_format,
      &glyphSheetDesc,       //const cl_image_desc   *image_desc,
      NULL,                  //void                  *host_ptr,
      &CLstatus              //cl_int                *errcode_ret
    );
    checkCLerror(__LINE__, __FILE__);
  }
  {
    size_t origin[] = {0,0,0};
    size_t region[] = {(size_t)gss->w, (size_t)gss->h, 1};
    CLstatus = clEnqueueWriteImage(
      commandQueue,         //cl_command_queue  command_queue,
      glyphSheet,           //cl_mem            image,
      CL_TRUE,              //cl_bool           blocking_write,
      &origin[0],           //const size_t     *origin,
      &region[0],           //const size_t     *region,
      gss->w,               //size_t            input_row_pitch,
      0,                    //size_t            input_slice_pitch,
      gss->pixels,          //const void       *ptr,
      0,                    //cl_uint           num_events_in_wait_list,
      NULL,                 //const cl_event   *event_wait_list,
      NULL                  //cl_event         *event
    );
    checkCLerror(__LINE__, __FILE__);
  }

  buildsomeroots();

  size_t UItextSize = sizeof(int) * UItextBlock.size.pro();
  cl_mem UItext_clmem = clCreateBuffer(
    context,
    CL_MEM_READ_ONLY,
    UItextSize,
    NULL,
    &CLstatus
  );
  checkCLerror(__LINE__, __FILE__);
  CLstatus = clEnqueueWriteBuffer (
    commandQueue,                    //cl_command_queue command_queue,
    UItext_clmem,                    //cl_mem           buffer,
    CL_TRUE,                         //cl_bool          blocking_write,
    0,                               //size_t           offset,
    UItextSize,                      //size_t           cb,
    (void*)UItextBlock.text.data(),  //const void      *ptr,
    0,                               //cl_uint          num_events_in_wait_list,
    NULL,                            //const cl_event  *event_wait_list,
    NULL                             //cl_event        *event
  );
  checkCLerror(__LINE__, __FILE__);

  cl_mem gsi_clmem = clCreateBuffer(
    context,
    CL_MEM_READ_ONLY,
    UItextSize,
    NULL,
    &CLstatus
  );
  checkCLerror(__LINE__, __FILE__);
  CLstatus = clEnqueueWriteBuffer (
    commandQueue,             //cl_command_queue command_queue,
    gsi_clmem,                //cl_mem           buffer,
    CL_TRUE,                  //cl_bool          blocking_write,
    0,                        //size_t           offset,
    sizeof(glyphSheetInfo),   //size_t           cb,
    (void*)&gsi,              //const void      *ptr,
    0,                        //cl_uint          num_events_in_wait_list,
    NULL,                     //const cl_event  *event_wait_list,
    NULL                      //cl_event        *event
  );
  checkCLerror(__LINE__, __FILE__);

#if kernelInspectArgIndex
  vector<int> kernelInspect(videoSize.Pro());
  for (int i = 0; i < videoSize.Pro(); i++) kernelInspect[i] = 1234;
  cl_mem kernelInspect_clmem = clCreateBuffer(
    context,
    CL_MEM_WRITE_ONLY,
    sizeof(int)*videoSize.Pro(),
    NULL,
    &CLstatus
  );
  checkCLerror(__LINE__, __FILE__);
  CLstatus = clEnqueueWriteBuffer (
    commandQueue,                  //cl_command_queue command_queue,
    kernelInspect_clmem,           //cl_mem           buffer,
    CL_TRUE,                       //cl_bool          blocking_write,
    0,                             //size_t           offset,
    sizeof(int)*videoSize.Pro(),   //size_t           cb,
    (void*)kernelInspect.data(),   //const void      *ptr,
    0,                             //cl_uint          num_events_in_wait_list,
    NULL,                          //const cl_event  *event_wait_list,
    NULL                           //cl_event        *event
  );
  checkCLerror(__LINE__, __FILE__);
#endif

  CLstatus = clSetKernelArg(kernel, 1, sizeof(int2), (void*)&UItextBlock.size);
  checkCLerror(__LINE__, __FILE__);

  CLstatus = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&UItext_clmem);
  checkCLerror(__LINE__, __FILE__);
  CLstatus = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void*)&gsi_clmem);
  checkCLerror(__LINE__, __FILE__);

  CLstatus = clSetKernelArg(kernel, 4, sizeof(cl_mem), (void*)&glyphSheet);
  checkCLerror(__LINE__, __FILE__);
  CLstatus = clSetKernelArg(kernel, 5, sizeof(cl_mem), (void*)&outputImage);
  checkCLerror(__LINE__, __FILE__);

#if kernelInspectArgIndex
  CLstatus = clSetKernelArg(
    kernel,
    kernelInspectArgIndex,
    sizeof(cl_mem),
    (void*)&kernelInspect_clmem
  );
  checkCLerror(__LINE__, __FILE__);
#endif


  SDL_Init(SDL_INIT_VIDEO);
  if (!strcmp(SDL_GetError(), "Unknown touch device")) SDL_ClearError();//??
  checkSDLerror(__LINE__, __FILE__);
  SDL_Window *window = SDL_CreateWindow(
    "ShaderPunk",              //const char* title,
    SDL_WINDOWPOS_UNDEFINED,   //int         x,
    SDL_WINDOWPOS_UNDEFINED,   //int         y,
    videoSize.x,               //int         w,
    videoSize.y,               //int         h,
    0                          //Uint32      flags
  );
  checkSDLerror(__LINE__, __FILE__);
  SDL_Surface *windowSrfc = SDL_GetWindowSurface(window);
  if (!strcmp(SDL_GetError(), "Invalid renderer")) SDL_ClearError();//??
  checkSDLerror(__LINE__, __FILE__);

  {
    float  cursPress  = 0;
    float  pCursPress = 0;
    float2 cursPos;
    float2 pCursPos;
    float2 UItextBlockPixCount = UItextBlock.size * gsi.glyphSize;





    const float scrollAccel = 1.2;
    //const float vertScrollRailThresh = 0.8;
    float2 scrollBoundary = float2(
      UItextBlockPixCount.x < videoSize.x ? videoSize.x : UItextBlockPixCount.x,
      UItextBlockPixCount.y < videoSize.y ? videoSize.y : UItextBlockPixCount.y
    );
    float2 overBounds;
    float2 pOverBounds;
    float2 scrollVel;
    float2 scrollPos;
    float2 pScrollPos;
    float2 scrollBoundaryBRC = scrollBoundary;



    int curFrame = 0;
    bool running = true;
    while (running) {
      pCursPress = cursPress;
      pCursPos   = cursPos;
      SDL_Event event;
      while (SDL_PollEvent(&event)) {
        switch (event.type) {
          case SDL_QUIT: running = false; break;
          case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_EXPOSED) {
              SDL_UpdateWindowSurface(window);
              checkSDLerror(__LINE__, __FILE__);
            }
            break;
          case SDL_MOUSEMOTION:
            cursPos = float2(event.motion.x, event.motion.y);
            break;
          case SDL_MOUSEBUTTONDOWN:
            switch (event.button.button) {
              case SDL_BUTTON_LEFT: cursPress = 1; break;
            }
            break;
          case SDL_MOUSEBUTTONUP:
            switch (event.button.button) {
              case SDL_BUTTON_LEFT: cursPress = 0; break;
            }
            break;
        }
      }
      pOverBounds = overBounds;
      overBounds = distrib(overBound, scrollPos, scrollBoundaryBRC, videoSize);
      scrollVel = cursPress ?
        (pCursPress ? cursPos - pCursPos : float2()) :
        (overBounds.x || overBounds.y ?
          distrib(
            rebound,
            scrollPos,
            scrollBoundaryBRC,
            videoSize,
            scrollVel,
            float2(scrollAccel)
          ) :
          scrollVel
        )
      ;
      pScrollPos = scrollPos;
      scrollPos = distrib(closeEnough, scrollVel + scrollPos);
      scrollBoundaryBRC = scrollPos + scrollBoundary;
      cout << endl;
      if (scrollPos != pScrollPos || !curFrame) {

        int2 offset = int2(scrollPos.x, scrollPos.y);
        CLstatus = clSetKernelArg(kernel, 0, sizeof(int2), (void*)&offset);
        checkCLerror(__LINE__, __FILE__);

        size_t globalWorkSize[] = {(size_t)videoSize.x, (size_t)videoSize.y};
        CLstatus = clEnqueueNDRangeKernel(
          commandQueue,       //cl_command_queue command_queue,
          kernel,             //cl_kernel        kernel,
          2,                  //cl_uint          work_dim,
          NULL,               //const size_t    *global_work_offset,
          globalWorkSize,     //const size_t    *global_work_size,
          NULL,               //const size_t    *local_work_size,
          0,                  //cl_uint          num_events_in_wait_list,
          NULL,               //const cl_event  *event_wait_list,
          NULL                //cl_event        *event
        );
        checkCLerror(__LINE__, __FILE__);

        size_t origin[] = {0, 0, 0};
        size_t region[] = {(size_t)videoSize.x, (size_t)videoSize.y, 1};
        CLstatus = clEnqueueReadImage(
          commandQueue,       //cl_command_queue command_queue,
          outputImage,        //cl_mem           image,
          CL_TRUE,            //cl_bool          blocking_read,
          origin,             //const            size_t origin[3],
          region,             //const            size_t region[3],
          0,                  //size_t           row_pitch,
          0,                  //size_t           slice_pitch,
          windowSrfc->pixels, //void            *ptr,
          0,                  //cl_uint          num_events_in_wait_list,
          NULL,               //const cl_event  *event_wait_list,
          NULL                //cl_event        *event
        );
        checkCLerror(__LINE__, __FILE__);


        SDL_UpdateWindowSurface(window);
        checkSDLerror(__LINE__, __FILE__);

#if kernelInspectArgIndex
        CLstatus = clEnqueueReadBuffer(
          commandQueue,                 //cl_command_queue command_queue,
          kernelInspect_clmem,          //cl_mem           buffer,
          CL_TRUE,                      //cl_bool          blocking_read,
          0,                            //size_t           offset,
          sizeof(int)*videoSize.Pro(),  //size_t           cb,
          kernelInspect.data(),         //void            *ptr,
          0,                            //cl_uint          num_events_in_wait_list,
          NULL,                         //const cl_event  *event_wait_list,
          NULL                          //cl_event        *event
        );
        checkCLerror(__LINE__, __FILE__);
        cout << endl << endl << "glyphSheetPos.y" << endl << endl;
        for (int row = 0; row < 3; row++) {
          cout << endl << endl << "row: " << row << endl;
          for (
            int i = videoSize.x * gsi.glyphSize.y * row;
            i < videoSize.x * gsi.glyphSize.y * row + gss->w;
            i += gsi.glyphSize.x
          ) {
            cout << "kernelInspect[" << i << "]: " << kernelInspect[i] << endl;
          }
        }
#endif
      }
      SDL_Delay(10);
      curFrame++;
    }
  }

  CLstatus = clReleaseKernel(kernel);
  checkCLerror(__LINE__, __FILE__);
  CLstatus = clReleaseProgram(program);
  checkCLerror(__LINE__, __FILE__);
  CLstatus = clReleaseMemObject(UItext_clmem);
  checkCLerror(__LINE__, __FILE__);
  CLstatus = clReleaseMemObject(gsi_clmem);
  checkCLerror(__LINE__, __FILE__);
  CLstatus = clReleaseMemObject(glyphSheet);
  checkCLerror(__LINE__, __FILE__);
  CLstatus = clReleaseMemObject(outputImage);
  checkCLerror(__LINE__, __FILE__);
#if kernelInspectArgIndex
  CLstatus = clReleaseMemObject(kernelInspect_clmem);
  checkCLerror(__LINE__, __FILE__);
#endif
  CLstatus = clReleaseCommandQueue(commandQueue);
  checkCLerror(__LINE__, __FILE__);
  CLstatus = clReleaseContext(context);
  checkCLerror(__LINE__, __FILE__);

  SDL_FreeSurface(gss);
  checkSDLerror(__LINE__, __FILE__);
  SDL_FreeSurface(windowSrfc);
  checkSDLerror(__LINE__, __FILE__);
  SDL_DestroyWindow(window);
  checkSDLerror(__LINE__, __FILE__);
  SDL_Quit();

  cout << "exited normally" << endl;
  exit(0);
}
