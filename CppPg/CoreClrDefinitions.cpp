#include "CoreClrDefinitions.h"
#include "stdafx.h"

CoreClrDefinitions::CoreClrDefinitions()
{
}

typedef int (*report_callback_ptr)(int progress);
typedef char* (*doWork_ptr)(const char* jobName, int iterations, int dataSize, double* data, report_callback_ptr callbackFunction);

template <typename T>
ManagedDelegateResult<T> CoreClrDefinitions::CreateManagedDelegate()
{
	ManagedDelegateResult<T> result;

	T managedDelegate;

	auto hr = createManagedDelegate
	(
		clrHostHandle,
		clrDomainId,
		"ManagedLibrary, Version=1.0.0.0",
		"ManagedLibrary.ManagedWorker",
		"DoWork",
		(void**)&managedDelegate
	);

	if (hr >= 0)
	{
		printf("Managed delegate created\n");
		result.success = true;
		result.Delegate = managedDelegate;
	}
	else
	{
		printf("coreclr_create_delegate failed - status: 0x%08x\n", hr);
		result.success = false;
		result.failureCode = hr;
	}

	return result;
}

bool CoreClrDefinitions::Shutdown() {
	//
	// STEP 6: Shutdown CoreCLR
	//

	return shutdownCoreClr(clrHostHandle, clrDomainId) >= 0;

	//if (hr >= 0)
	//{
	//	printf("CoreCLR successfully shutdown\n");
	//
	//}
	//else
	//{
	//	printf("coreclr_shutdown failed - status: 0x%08x\n", hr);
	//}
}

int CoreClrDefinitions::Init(RuntimeEnvironment env) {
	//
	// STEP 1: Load CoreCLR (coreclr.dll/libcoreclr.so)
	//
	if (initialized) {
		return 1;
	}

	environment = env;

	if (InitHostingFunctions() == -1) {
		return -1;
	}

	if (InitCoreClr() == -1) {
		return -1;
	}

	initialized = true;
	return 1;
}

int CoreClrDefinitions::InitHostingFunctions() {
#if WINDOWS
	// <Snippet1>
	HMODULE coreClr = LoadLibraryExA(environment.coreClrPath.c_str(), NULL, 0);
	// </Snippet1>
#elif LINUX
	void* coreClr = dlopen(coreClrPath.c_str(), RTLD_NOW | RTLD_LOCAL);
#endif
	if (coreClr == NULL)
	{
		printf("ERROR: Failed to load CoreCLR from %s\n", environment.coreClrPath.c_str());
		return -1;
	}
	else
	{
		printf("Loaded CoreCLR from %s\n", environment.coreClrPath.c_str());
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
	return 1;
}

int CoreClrDefinitions::InitCoreClr() {
	const char* propertyKeys[] = {
		"TRUSTED_PLATFORM_ASSEMBLIES"      // Trusted assemblies
	};

	const char* propertyValues[] = {
		environment.tpaList.c_str()
	};

	// <Snippet4>
	//void* hostHandle;
	//unsigned int domainId;

	// This function both starts the .NET Core runtime and creates
	// the default (and only) AppDomain
	int hr = initializeCoreClr
	(
		environment.runtimePath.c_str(),        // App base path
		"SampleHost",       // AppDomain friendly name
		sizeof(propertyKeys) / sizeof(char*),   // Property count
		propertyKeys,       // Property names
		propertyValues,     // Property values
		&clrHostHandle,		// Host handle
		&clrDomainId		// AppDomain ID
	);
	// </Snippet4>

	if (hr >= 0)
	{
		printf("CoreCLR started\n");
		return 1;
	}
	else
	{
		printf("coreclr_initialize failed - status: 0x%08x\n", hr);
		return -1;
	}
}

CoreClrDefinitions::~CoreClrDefinitions()
{
	if (!initialized)
	{
		return;
	}
	initialized = false;
}