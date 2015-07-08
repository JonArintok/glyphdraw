

#include <iostream>
#include <vector>
#include <CL/cl.h>
#include <SDL2/SDL.h>
using namespace std;
typedef unsigned int  uint;
typedef unsigned char uchar;
#include "cl_getErrorString.h"
#include "foundation.hpp"
#include "init.hpp"

bool  running  = false;
float curFrame = 1;

void handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) running = false;
	}
}

int main(int argc, char* argv[]) {
	
	const float    videoWidth  = 1280;
	const float    videoHeight =  720;
	const uint32_t videoSize   = videoWidth * videoHeight;
	
	cl_int status = CL_SUCCESS;
	const cl_uint maxDevices = 8;
	cl_device_id     computeDevices[maxDevices];
	cl_context       context = NULL;
	cl_command_queue commandQueue = NULL;
	initOpenCL(computeDevices, maxDevices, context, commandQueue, status);
	if (status != CL_SUCCESS) {
		cout << "failed: initOpenCL" << endl;
		exit(__LINE__);
	}
	cl_program program;
	initClProgram("UIshader.cl", program, context, computeDevices, status);
	if (status != CL_SUCCESS) {
		cout << "failed: initClProgram" << endl;
		exit(__LINE__);
	}
	cl_kernel kernel = clCreateKernel(program, "helloPixel", &status);
	if (status != CL_SUCCESS) {
		cout << "failed to create kernel" << endl;
		exit(__LINE__);
	}
	
	cl_mem outputImage;
	{
		cl_image_format outputImageFormat = {CL_ARGB, CL_UNORM_INT8};
		cl_image_desc outputImageDesc;
		memset(&outputImageDesc, '\0', sizeof(cl_image_desc));
		outputImageDesc.image_type   = CL_MEM_OBJECT_IMAGE2D;
		outputImageDesc.image_width  = videoWidth;
		outputImageDesc.image_height = videoHeight;
		outputImage = clCreateImage(
			context,               //cl_context             context,
			CL_MEM_WRITE_ONLY,     //cl_mem_flags           flags,
			&outputImageFormat,    //const cl_image_format *image_format,
			&outputImageDesc,      //const cl_image_desc   *image_desc,
			NULL,                  //void                  *host_ptr,
			&status                //cl_int                *errcode_ret
		);
		if (status != CL_SUCCESS) {
			cout << "failedto create 'outputImage'" << endl;
			exit(__LINE__);
		}
	}
	
	
	
	const char *bmp_path = "GS_000032-000127_064x003_010x023_hermit_16.bmp";
	gs_glyphW = 10;
	gs_glyphH = 23;
	gs_glyphColCount = 64;
	gs_glyphRowCount =  3;
	gs_unicodefirst =  32;
	gs_unicodeLast  = 127;
	gs_surface = SDL_LoadBMP(bmp_path);
	{
		cl_image_format glyphSheetFormat = {CL_LUMINANCE, CL_UNORM_INT8};
		cl_image_desc glyphSheetDesc;
		memset(&glyphSheetDesc, '\0', sizeof(cl_image_desc));
		glyphSheetDesc.image_type   = CL_MEM_OBJECT_IMAGE2D;
		glyphSheetDesc.image_width  = gs_surface->w;
		glyphSheetDesc.image_height = gs_surface->h;
		glyphSheet = clCreateImage(
			context,               //cl_context             context,
			CL_MEM_READ_ONLY,      //cl_mem_flags           flags,
			&glyphSheetFormat,     //const cl_image_format *image_format,
			&glyphSheetDesc,       //const cl_image_desc   *image_desc,
			NULL,                  //void                  *host_ptr,
			&status                //cl_int                *errcode_ret
		);
		if (status != CL_SUCCESS) {
			cout << "failedto create 'glyphSheet'" << endl;
			exit(__LINE__);
		}
	}
	{
		size_t origin[] = {0,0,0};
		size_t region[] = {(size_t)gs_surface->w, (size_t)gs_surface->h, 1};
		clEnqueueWriteImage(
			commandQueue,         //cl_command_queue  command_queue,
			glyphSheet,           //cl_mem            image,
			CL_TRUE,              //cl_bool           blocking_write,
			&origin[0],           //const size_t     *origin,
			&region[0],           //const size_t     *region,
			gs_surface->w,        //size_t            input_row_pitch,
			0,                    //size_t            input_slice_pitch,
			gs_surface->pixels,   //const void       *ptr,
			0,                    //cl_uint           num_events_in_wait_list,
			NULL,                 //const cl_event   *event_wait_list,
			NULL                  //cl_event         *event
		);
	}
	
	
	
	
	
	
	
	
	
	
	
	
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&glyphSheet);
	if (status != CL_SUCCESS) {
		cout << "failed to set kernel arg 1" << endl;
		return __LINE__;
	}
	status = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&outputImage);
	if (status != CL_SUCCESS) {
		cout << "failed to set kernel arg 2" << endl;
		return __LINE__;
	}
	
	uint32_t *videoOut = new uint32_t[videoSize];
	initVideo(videoWidth, videoHeight, __FILE__);
	SDL_SetRenderTarget(renderer, texture);
	SDL_RenderClear(renderer);
	
	running = true;
	while (running) {
		
		status = clSetKernelArg(kernel, 0, sizeof(float),  (void*)&curFrame);
		if (status != CL_SUCCESS) {
			cout << "failed to set kernel arg 0" << endl;
			return __LINE__;
		}
		
		//run the kernel
		size_t globalWorkSize[] = {(uint32_t)videoWidth, (uint32_t)videoHeight};
		status = clEnqueueNDRangeKernel(
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
		if (status != CL_SUCCESS) {
			cout << "failed: clEnqueueNDRangeKernel:" << endl 
			<< cl_getErrorString(status) << endl;
			exit(__LINE__);
		}
		
		//read the outputBuffer modified by our kernel back to host memory
		size_t origin[] = {0, 0, 0};
		size_t region[] = {(uint32_t)videoWidth, (uint32_t)videoHeight, 1};
		status = clEnqueueReadImage(
			commandQueue,      //cl_command_queue command_queue,
			outputImage,       //cl_mem           image,
			CL_TRUE,           //cl_bool          blocking_read,
			origin,            //const            size_t origin[3],
			region,            //const            size_t region[3],
			0,                 //size_t           row_pitch,
			0,                 //size_t           slice_pitch,
			videoOut,          //void            *ptr,
			0,                 //cl_uint          num_events_in_wait_list,
			NULL,              //const cl_event  *event_wait_list,
			NULL               //cl_event        *event
		);
		if (status != CL_SUCCESS) {
			cout << "failed: clEnqueueReadImage" << endl;
			exit(__LINE__);
		}
		
		//render image on screen
		SDL_UpdateTexture(texture, NULL, videoOut, videoWidth*sizeof(uint32_t));
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
		
		handleEvents();
		curFrame++;
		SDL_Delay(10);
	}
	
	status = clReleaseKernel(kernel);
	status = clReleaseProgram(program);
	status = clReleaseMemObject(outputImage);
	status = clReleaseCommandQueue(commandQueue);
	status = clReleaseContext(context);
	if (status != CL_SUCCESS) {
		cout << "failed to release some CL objects" << endl;
		return __LINE__;
	}
	delete[] videoOut;
	SDL_FreeSurface(gs_surface);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	exit(0);
}
