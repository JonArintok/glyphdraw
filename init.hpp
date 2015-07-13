
SDL_Window    *window   = NULL;
SDL_Renderer  *renderer = NULL;
SDL_Texture   *texture  = NULL;
void initVideo(uint videoWidth, uint videoHeight, const char* windowTitle) {
	if ( SDL_Init(SDL_INIT_VIDEO) ) {
		checkSDLerror(__LINE__, __FILE__);
	}
	window = SDL_CreateWindow(
		windowTitle,               //const char* title,
		SDL_WINDOWPOS_UNDEFINED,   //int         x,
		SDL_WINDOWPOS_UNDEFINED,   //int         y,
		videoWidth,                //int         w,
		videoHeight,               //int         h,
		SDL_WINDOW_OPENGL          //Uint32      flags
	);
	if (window == NULL) {
		checkSDLerror(__LINE__, __FILE__);
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);
	if (renderer == NULL) {
		checkSDLerror(__LINE__, __FILE__);
	}
	texture = SDL_CreateTexture(
		renderer,                     //SDL_Renderer* renderer,
		SDL_PIXELFORMAT_RGBA8888,     //Uint32        format,
		SDL_TEXTUREACCESS_STREAMING,  //int           access, 
		videoWidth,                   //int           w,
		videoHeight                   //int           h
	);
	if (texture == NULL) {
		checkSDLerror(__LINE__, __FILE__);
	}
}

void initOpenCL(
	cl_device_id     *devices, 
	const cl_uint     maxDevices, 
	cl_context       &context, 
	cl_command_queue &commandQueue, 
	cl_int           &CLstatus
) {
	cl_uint platformCount;
	cl_platform_id platform = NULL;
	const int maxPlatforms = 8;
	cl_platform_id platforms[maxPlatforms];
	CLstatus = clGetPlatformIDs(maxPlatforms, platforms, &platformCount);
	checkCLerror(__LINE__, __FILE__);
	if (platformCount < 1) {
		cout << "failed to find any OpenCL platforms" << endl;
		return;
	}
	platform = platforms[0];
	cl_uint deviceCount = 0;
	CLstatus = clGetDeviceIDs(
		platform, CL_DEVICE_TYPE_GPU, maxDevices, devices, &deviceCount
	);
	checkCLerror(__LINE__, __FILE__);
	if (!deviceCount) {
		CLstatus = clGetDeviceIDs(
			platform, CL_DEVICE_TYPE_CPU, maxDevices, devices, &deviceCount
		);
		if (!deviceCount) {
			cout << "failed to find any GPU or CPU devices" << endl;
			return;
		}
		cout << "no GPU devices found, using CPU instead" << endl;
	}	
	context = clCreateContext(NULL,1, devices, NULL,NULL, &CLstatus);
	checkCLerror(__LINE__, __FILE__);
	commandQueue = clCreateCommandQueueWithProperties(
		context, devices[0], 0, &CLstatus
	);
	checkCLerror(__LINE__, __FILE__);
}


#include <sstream>
#include <fstream>
void initClProgram(
	const char   *filename, 
	cl_program   &program, 
	cl_context   &context, 
	cl_device_id *devices, 
	cl_int       &CLstatus
) {
	cout << "building program from " << filename << endl;
	ifstream sourceFile(filename);
	stringstream sourceStream;
	sourceStream << sourceFile.rdbuf();
	string source = sourceStream.str();
	const char *sources[] = {source.c_str()};
	size_t  sourceSizes[] = {strlen(sources[0])};
	
	program = clCreateProgramWithSource(
		context, 1, sources, sourceSizes, &CLstatus
	);
	checkCLerror(__LINE__, __FILE__);
	CLstatus = clBuildProgram(program, 1, devices, NULL,NULL,NULL);
	{
		size_t logLen;
		clGetProgramBuildInfo(
			program, 
			devices[0], 
			CL_PROGRAM_BUILD_LOG, 
			0, 
			NULL, 
			&logLen
		);
		char log[logLen];
		clGetProgramBuildInfo(
			program, 
			devices[0], 
			CL_PROGRAM_BUILD_LOG, 
			logLen, 
			log, 
			NULL
		);
		cout << log << endl;
	}
	return;
}
