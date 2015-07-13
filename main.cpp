

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
	
	const cl_uint maxDevices = 8;
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
			&CLstatus                //cl_int                *errcode_ret
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
	
	
	
	
	
	
	uint32_t *videoOut = new uint32_t[videoSize];
	initVideo(videoWidth, videoHeight, __FILE__);
	SDL_RenderClear(renderer);
	//checkSDLerror(__LINE__, __FILE__);
	SDL_SetRenderTarget(renderer, texture);
	//checkSDLerror(__LINE__, __FILE__);
	
	
	
	
	bool  running      = true;
	bool  shouldRedraw = true;
	float curFrame = 1;
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
			
			size_t globalWorkSize[] = {(uint32_t)videoWidth, (uint32_t)videoHeight};
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
			size_t region[] = {(uint32_t)videoWidth, (uint32_t)videoHeight, 1};
			CLstatus = clEnqueueReadImage(
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
			checkCLerror(__LINE__, __FILE__);
			
			SDL_UpdateTexture(texture, NULL, videoOut, videoWidth*sizeof(uint32_t));
			//checkSDLerror(__LINE__, __FILE__);
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			//checkSDLerror(__LINE__, __FILE__);
			SDL_RenderPresent(renderer);
			//checkSDLerror(__LINE__, __FILE__);
			
			
			
			
			
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
			/*
			cout << endl << endl << "glyphSheetPos.y" << endl << endl;
			for (uint row = 0; row < 3; row++) {
				cout << endl << endl << "row: " << row << endl;
				for (
					uint i = videoWidth * gsi.glyphH * row; 
					i < videoWidth * gsi.glyphH * row + gss->w;
					i += gsi.glyphW
				) {
					cout << "kernelInspect[" << i << "]: " << kernelInspect[i] << endl;
				}
			}
			*/
			
			
			
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
	
	delete[] videoOut;
	delete[] UItextBlock.text;
	delete[] kernelInspect;
	
	SDL_FreeSurface(gss);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	exit(0);
}
