#include "CoreClrDefinitions.h"

CoreClrDefinitions::CoreClrDefinitions()
{

}


int CoreClrDefinitions::Init(RuntimeEnvironment env) {
//
// STEP 1: Load CoreCLR (coreclr.dll/libcoreclr.so)
//
	if (initiated) {
		return 1;
	}


#if WINDOWS
// <Snippet1>
	HMODULE coreClr = LoadLibraryExA(env.coreClrPath.c_str(), NULL, 0);
// </Snippet1>
#elif LINUX
	void* coreClr = dlopen(coreClrPath.c_str(), RTLD_NOW | RTLD_LOCAL);
#endif
	if (coreClr == NULL)
	{
		printf("ERROR: Failed to load CoreCLR from %s\n", env.coreClrPath.c_str());
		return -1;
	}
	else
	{
		printf("Loaded CoreCLR from %s\n", env.coreClrPath.c_str());
	}

	//
	// STEP 2: Get CoreCLR hosting functions
	//
#if WINDOWS
	// <Snippet2>
	initializeCoreClr = (coreclr_initialize_ptr)GetProcAddress(coreClr, "coreclr_initialize");
	createManagedDelegate = (coreclr_create_delegate_ptr)GetProcAddress(coreClr, "coreclr_create_delegate");
	shutdownCoreClr = (coreclr_shutdown_ptr)GetProcAddress(coreClr, "coreclr_shutdown");
	// </Snippet2>
#elif LINUX
	initializeCoreClr = (coreclr_initialize_ptr)dlsym(coreClr, "coreclr_initialize");
	createManagedDelegate = (coreclr_create_delegate_ptr)dlsym(coreClr, "coreclr_create_delegate");
	shutdownCoreClr = (coreclr_shutdown_ptr)dlsym(coreClr, "coreclr_shutdown");
#endif

	if (initializeCoreClr == NULL)
	{
		printf("coreclr_initialize not found");
		return -1;
	}

	if (createManagedDelegate == NULL)
	{
		printf("coreclr_create_delegate not found");
		return -1;
	}

	if (shutdownCoreClr == NULL)
	{
		printf("coreclr_shutdown not found");
		return -1;
	}
	initiated = true;
	return 1;
}

CoreClrDefinitions::~CoreClrDefinitions()
{
}