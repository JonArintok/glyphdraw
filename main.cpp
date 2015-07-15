
#include <iostream>
#include <vector>
#include <string>
#include <CL/cl.h>
#include <SDL2/SDL.h>
using namespace std;
typedef uint32_t uint;
typedef uint8_t  uchar;
#include "error.hpp"
#include "foundation.hpp"
#include "init.hpp"
#include "testRoots.hpp"


int main(int argc, char* argv[]) {
	
	const uint videoW    = 1280;
	const uint videoH    =  720;
	const uint videoSize = videoW * videoH;
	
	const cl_uint    maxDevices = 8;
	cl_device_id     computeDevices[maxDevices];
	cl_context       context = NULL;
	cl_command_queue commandQueue = NULL;
	initOpenCL(computeDevices, maxDevices, context, commandQueue, CLstatus);
	checkCLerror(__LINE__, __FILE__);
	cl_program program;
	initClProgram("UIshader.cl", program, context, computeDevices, CLstatus);
	checkCLerror(__LINE__, __FILE__);
	cl_kernel kernel = clCreateKernel(program, "helloPixel", &CLstatus);
	checkCLerror(__LINE__, __FILE__);
	
	cl_mem outputImage;
	{
		cl_image_format outputImageFormat = {CL_RGBA, CL_UNORM_INT8};
		cl_image_desc outputImageDesc;
		memset(&outputImageDesc, '\0', sizeof(cl_image_desc));
		outputImageDesc.image_type   = CL_MEM_OBJECT_IMAGE2D;
		outputImageDesc.image_width  = videoW;
		outputImageDesc.image_height = videoH;
		outputImage = clCreateImage(
			context,               //cl_context             context,
			CL_MEM_WRITE_ONLY,     //cl_mem_flags           flags,
			&outputImageFormat,    //const cl_image_format *image_format,
			&outputImageDesc,      //const cl_image_desc   *image_desc,
			NULL,                  //void                  *host_ptr,
			&CLstatus                //cl_int                *errcode_ret
		);
		checkCLerror(__LINE__, __FILE__);
	}
	
	const char *bmp_path = "GS_0000032-0000127_032x003_010x023_hermit.bmp";
	gsi.glyphW = 10;
	gsi.glyphH = 23;
	gsi.colCount = 32;
	gsi.rowCount =  3;
	gsi.unicodeFirst =  32;
	gsi.unicodeLast  = 127;
	gss = SDL_LoadBMP(bmp_path);
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
	
	size_t UItextSize = sizeof(uint) * UItextBlock.w * UItextBlock.h;
	cl_mem UItext_clmem = clCreateBuffer(
		context, 
		CL_MEM_READ_ONLY, 
		UItextSize, 
		NULL,
		&CLstatus
	);
	checkCLerror(__LINE__, __FILE__);
	CLstatus = clEnqueueWriteBuffer (
		commandQueue,             //cl_command_queue command_queue,
		UItext_clmem,                   //cl_mem           buffer,
		CL_TRUE,                  //cl_bool          blocking_write,
		0,                        //size_t           offset,
		UItextSize,               //size_t           cb,
		(void*)UItextBlock.text,  //const void      *ptr,
		0,                        //cl_uint          num_events_in_wait_list,
		NULL,                     //const cl_event  *event_wait_list,
		NULL                      //cl_event        *event
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
	
	int *kernelInspect = new int[videoSize];
	for (uint i = 0; i < videoSize; i++) kernelInspect[i] = 1234;
	cl_mem kernelInspect_clmem = clCreateBuffer(
		context, 
		CL_MEM_WRITE_ONLY, 
		sizeof(int)*videoSize, 
		NULL,
		&CLstatus
	);
	checkCLerror(__LINE__, __FILE__);
	CLstatus = clEnqueueWriteBuffer (
		commandQueue,             //cl_command_queue command_queue,
		kernelInspect_clmem,      //cl_mem           buffer,
		CL_TRUE,                  //cl_bool          blocking_write,
		0,                        //size_t           offset,
		sizeof(int)*videoSize,    //size_t           cb,
		(void*)kernelInspect,     //const void      *ptr,
		0,                        //cl_uint          num_events_in_wait_list,
		NULL,                     //const cl_event  *event_wait_list,
		NULL                      //cl_event        *event
	);
	checkCLerror(__LINE__, __FILE__);
	
	
	CLstatus = clSetKernelArg(kernel, 0, sizeof(uint), (void*)&UItextBlock.w);
	checkCLerror(__LINE__, __FILE__);
	CLstatus = clSetKernelArg(kernel, 1, sizeof(uint), (void*)&UItextBlock.h);
	checkCLerror(__LINE__, __FILE__);
	
	CLstatus = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&UItext_clmem);
	checkCLerror(__LINE__, __FILE__);
	CLstatus = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void*)&gsi_clmem);
	checkCLerror(__LINE__, __FILE__);
	
	CLstatus = clSetKernelArg(kernel, 4, sizeof(cl_mem), (void*)&glyphSheet);
	checkCLerror(__LINE__, __FILE__);
	CLstatus = clSetKernelArg(kernel, 5, sizeof(cl_mem), (void*)&outputImage);
	checkCLerror(__LINE__, __FILE__);
	
	CLstatus = clSetKernelArg(
		kernel, 
		6, 
		sizeof(cl_mem),
		(void*)&kernelInspect_clmem
	);
	checkCLerror(__LINE__, __FILE__);
	
	
	SDL_Init(SDL_INIT_VIDEO);
	checkSDLerror(__LINE__, __FILE__);
	SDL_Window *window = SDL_CreateWindow(
		"ShaderPunk",              //const char* title,
		SDL_WINDOWPOS_UNDEFINED,   //int         x,
		SDL_WINDOWPOS_UNDEFINED,   //int         y,
		videoW,                    //int         w,
		videoH,                    //int         h,
		0                          //Uint32      flags
	);
	checkSDLerror(__LINE__, __FILE__);
	SDL_Surface *windowSrfc = SDL_GetWindowSurface(window);
	{ //This must be a bug in SDL; I'm not even USING a renderer...
		if (!strcmp(SDL_GetError(), "Invalid renderer")) SDL_ClearError();
	}
	checkSDLerror(__LINE__, __FILE__);
	
	
	bool  running      = true;
	bool  shouldRedraw = true;
	uint curFrame = 1;
	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: running = false; break;
				case SDL_WINDOWEVENT:
					if (event.window.event == SDL_WINDOWEVENT_EXPOSED) {
						shouldRedraw = true;
					}
					break;
			}
		}
		
		if (shouldRedraw) {
			shouldRedraw = false;
			
			size_t globalWorkSize[] = {videoW, videoH};
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
			size_t region[] = {videoW, videoH, 1};
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
			
			
			CLstatus = clEnqueueReadBuffer(
				commandQueue,                 //cl_command_queue command_queue,
				kernelInspect_clmem,          //cl_mem           buffer,
				CL_TRUE,                      //cl_bool          blocking_read,
				0,                            //size_t           offset,
				videoSize*sizeof(uint32_t),   //size_t           cb,
				kernelInspect,                //void            *ptr,
				0,                            //cl_uint          num_events_in_wait_list,
				NULL,                         //const cl_event  *event_wait_list,
				NULL                          //cl_event        *event
			);
			checkCLerror(__LINE__, __FILE__);
			//cout << endl << endl << "glyphSheetPos.y" << endl << endl;
			//for (uint row = 0; row < 3; row++) {
			//	cout << endl << endl << "row: " << row << endl;
			//	for (
			//		uint i = videoW * gsi.glyphH * row; 
			//		i < videoW * gsi.glyphH * row + gss->w;
			//		i += gsi.glyphW
			//	) {
			//		cout << "kernelInspect[" << i << "]: " << kernelInspect[i] << endl;
			//	}
			//}
		}
		
		SDL_Delay(10);
		curFrame++;
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
	CLstatus = clReleaseCommandQueue(commandQueue);
	checkCLerror(__LINE__, __FILE__);
	CLstatus = clReleaseContext(context);
	checkCLerror(__LINE__, __FILE__);
	
	delete[] UItextBlock.text;
	delete[] kernelInspect;
	
	SDL_FreeSurface(gss);
	checkSDLerror(__LINE__, __FILE__);
	SDL_FreeSurface(windowSrfc);
	checkSDLerror(__LINE__, __FILE__);
	SDL_DestroyWindow(window);
	checkSDLerror(__LINE__, __FILE__);
	SDL_Quit();
	
	exit(0);
}
