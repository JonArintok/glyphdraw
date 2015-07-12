

#include <iostream>
#include <vector>
#include <string>
#include <CL/cl.h>
#include <SDL2/SDL.h>
using namespace std;
typedef uint32_t uint;
typedef uint8_t  uchar;
#include "cl_getErrorString.h"
#include "foundation.hpp"
#include "init.hpp"
#include "someroots.hpp"


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
	gsi.glyphW = 10;
	gsi.glyphH = 23;
	gsi.colCount = 64;
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
			&status                //cl_int                *errcode_ret
		);
		if (status != CL_SUCCESS) {
			cout << "failedto create 'glyphSheet'" << endl;
			exit(__LINE__);
		}
	}
	{
		size_t origin[] = {0,0,0};
		size_t region[] = {(size_t)gss->w, (size_t)gss->h, 1};
		status = clEnqueueWriteImage(
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
		if (status != CL_SUCCESS) {
			cout << "failed: clEnqueueWriteImage" << endl;
			return __LINE__;
		}
	}
	
	
	
	buildsomeroots();
	
	
	size_t UItextSize = sizeof(uint) * UItextBlock.w * UItextBlock.h;
	cl_mem UItext_clmem = clCreateBuffer(
		context, 
		CL_MEM_READ_ONLY, 
		UItextSize, 
		NULL,
		&status
	);
	if (status != CL_SUCCESS) {
		cout << "failed: clCreateBuffer" << endl;
		return __LINE__;
	}
	status = clEnqueueWriteBuffer (
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
	if (status != CL_SUCCESS) {
		cout << "failed: clEnqueueWriteBuffer" << endl;
		return __LINE__;
	}
	
	
	cl_mem gsi_clmem = clCreateBuffer(
		context, 
		CL_MEM_READ_ONLY, 
		UItextSize, 
		NULL,
		&status
	);
	if (status != CL_SUCCESS) {
		cout << "failed: clCreateBuffer" << endl;
		return __LINE__;
	}
	status = clEnqueueWriteBuffer (
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
	if (status != CL_SUCCESS) {
		cout << "failed: clEnqueueWriteBuffer" << endl;
		return __LINE__;
	}
	
	
	
	
	status = clSetKernelArg(kernel, 0, sizeof(uint), (void*)&UItextBlock.w);
	if (status != CL_SUCCESS) {
		cout << "failed to set kernel arg 0" << endl;
		return __LINE__;
	}
	status = clSetKernelArg(kernel, 1, sizeof(uint), (void*)&UItextBlock.h);
	if (status != CL_SUCCESS) {
		cout << "failed to set kernel arg 1" << endl;
		return __LINE__;
	}
	
	status = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&UItext_clmem);
	if (status != CL_SUCCESS) {
		cout << "failed to set kernel arg 2" << endl;
		return __LINE__;
	}
	status = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void*)&gsi_clmem);
	if (status != CL_SUCCESS) {
		cout << "failed to set kernel arg 3" << endl;
		return __LINE__;
	}
	
	status = clSetKernelArg(kernel, 4, sizeof(cl_mem), (void*)&glyphSheet);
	if (status != CL_SUCCESS) {
		cout << "failed to set kernel arg 4" << endl;
		return __LINE__;
	}
	status = clSetKernelArg(kernel, 5, sizeof(cl_mem), (void*)&outputImage);
	if (status != CL_SUCCESS) {
		cout << "failed to set kernel arg 5" << endl;
		return __LINE__;
	}
	
	uint32_t *videoOut = new uint32_t[videoSize];
	initVideo(videoWidth, videoHeight, __FILE__);
	SDL_SetRenderTarget(renderer, texture);
	SDL_RenderClear(renderer);
	
	
	
	
	bool  running      = true;
	bool  shouldRedraw = true;
	float curFrame = 1;
	while (running) {
		
		if (shouldRedraw) {
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
			
			shouldRedraw = false;
		}
		
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
	delete[] UItextBlock.text;
	SDL_FreeSurface(gss);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	exit(0);
}
