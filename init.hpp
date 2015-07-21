

void initOpenCL(
	cl_device_id     *devices, 
	const cl_uint     maxDevices, 
	cl_context       &context, 
	cl_command_queue &commandQueue
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
	vector<const char*> &paths, 
	cl_program          &program, 
	cl_context          &context, 
	cl_device_id        *devices
) {
	string      sources     [paths.size()];
	const char *sourcesData [paths.size()];
	size_t      sourceSizes [paths.size()];
	cout << "building program from files:" << endl;
	for (uint32_t i = 0; i < paths.size(); i++) {
		cout << '\t' << paths[i] << endl;
		ifstream sourceFile(paths[i]);
		stringstream sourceStream;
		sourceStream << sourceFile.rdbuf();
		sources[i] = sourceStream.str();
		sourcesData[i] = sources[i].c_str();
		sourceSizes[i] = sources[i].size();
	}
	program = clCreateProgramWithSource(
		context, paths.size(), sourcesData, sourceSizes, &CLstatus
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
