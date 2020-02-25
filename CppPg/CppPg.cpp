#include "stdafx.h"
#include "RuntimeUtils.h"
#include "CoreClrDefinitions.h"

#include "ManagedDelegateResult.h"

// Function pointer types for the managed call and callback

void BuildTpaList(const char* directory, const char* extension, std::string& tpaList);
int ReportProgressCallback(int progress);

char* GetRuntimePath(char** argv)
{
	char runtimePath[MAX_PATH];

#if WINDOWS
	GetFullPathNameA(argv[0], MAX_PATH, runtimePath, NULL);
#elif LINUX
	realpath(argv[0], runtimePath);
#endif
	char* last_slash = strrchr(runtimePath, FS_SEPARATOR[0]);
	if (last_slash != NULL)
		*last_slash = 0;

	return runtimePath;
}

int dostuff(char** argv) {
	// do something
	auto env = RuntimeUtils::GetEnvironmentPtr(argv);
	auto& refToARunTimeEnvironment = RuntimeUtils::GetEnvironmentRef(argv);

	// cast away const
	RuntimeEnvironment& re = const_cast<RuntimeEnvironment&>(refToARunTimeEnvironment);

	// the first element of an array is a pointer
	// strings are arrays of chars
	// cpppg.exe aa bb cc dd ee

	delete env;
	return 69;
}

int main(int argc, char* argv[])
{
	//dostuff(argv);
	//CppPg.obj : error LNK2019: unresolved external symbol 
	//"public: struct ManagedDelegateResult<char * (__cdecl*)(char const *,int,int,double *,int (__cdecl*)(int))> __cdecl CoreClrDefinitions::CreateManagedDelegate<char * (__cdecl*)(char const *,int,int,double *,int (__cdecl*)(int))>(void)" (??$CreateManagedDelegate@P6APEADPEBDHHPEANP6AHH@Z@Z@CoreClrDefinitions@@QEAA?AU?$ManagedDelegateResult@P6APEADPEBDHHPEANP6AHH@Z@Z@@XZ) referenced in function main
	//C:\Users\Weirdo\source\repos\CppPg\x64\Debug\CppPg.exe : fatal error LNK1120 : 1 unresolved externals
	auto env = RuntimeUtils::GetEnvironment(argv);
	auto def = new CoreClrDefinitions();
	if (def->Init(env) == -1) {
		return -1;
	};

	//	//std::cout << "Initialized: " << def->initialized << "\n";
	//
	//#pragma region Done
	//
	//	//
	//	// STEP 1: Load CoreCLR (coreclr.dll/libcoreclr.so)
	//	//
	//#if WINDOWS
	//	// <Snippet1>
	//	HMODULE coreClr = LoadLibraryExA(env.coreClrPath.c_str(), NULL, 0);
	//	// </Snippet1>
	//#elif LINUX
	//	void* coreClr = dlopen(coreClrPath.c_str(), RTLD_NOW | RTLD_LOCAL);
	//#endif
	//	if (coreClr == NULL)
	//	{
	//		printf("ERROR: Failed to load CoreCLR from %s\n", env.coreClrPath.c_str());
	//		return -1;
	//	}
	//	else
	//	{
	//		printf("Loaded CoreCLR from %s\n", env.coreClrPath.c_str());
	//	}
	//
	//	//
	//	// STEP 2: Get CoreCLR hosting functions
	//	//
	//#if WINDOWS
	//	// <Snippet2>
	//	coreclr_initialize_ptr initializeCoreClr = (coreclr_initialize_ptr)GetProcAddress(coreClr, "coreclr_initialize");
	//	coreclr_create_delegate_ptr createManagedDelegate = (coreclr_create_delegate_ptr)GetProcAddress(coreClr, "coreclr_create_delegate");
	//	coreclr_shutdown_ptr shutdownCoreClr = (coreclr_shutdown_ptr)GetProcAddress(coreClr, "coreclr_shutdown");
	//	// </Snippet2>
	//#elif LINUX
	//	coreclr_initialize_ptr initializeCoreClr = (coreclr_initialize_ptr)dlsym(coreClr, "coreclr_initialize");
	//	coreclr_create_delegate_ptr createManagedDelegate = (coreclr_create_delegate_ptr)dlsym(coreClr, "coreclr_create_delegate");
	//	coreclr_shutdown_ptr shutdownCoreClr = (coreclr_shutdown_ptr)dlsym(coreClr, "coreclr_shutdown");
	//#endif
	//
	//	if (initializeCoreClr == NULL)
	//	{
	//		printf("coreclr_initialize not found");
	//		return -1;
	//	}
	//
	//	if (createManagedDelegate == NULL)
	//	{
	//		printf("coreclr_create_delegate not found");
	//		return -1;
	//	}
	//
	//	if (shutdownCoreClr == NULL)
	//	{
	//		printf("coreclr_shutdown not found");
	//		return -1;
	//	}
	//#pragma endregion
	//
	//	// <Snippet3>
	//	// Define CoreCLR properties
	//	// Other properties related to assembly loading are common here,
	//	// but for this simple sample, TRUSTED_PLATFORM_ASSEMBLIES is all
	//	// that is needed. Check hosting documentation for other common properties.
	//	const char* propertyKeys[] = {
	//		"TRUSTED_PLATFORM_ASSEMBLIES"      // Trusted assemblies
	//	};
	//
	//	const char* propertyValues[] = {
	//		env.tpaList.c_str()
	//	};
	//	// </Snippet3>
	//
	//	//
	//	// STEP 4: Start the CoreCLR runtime
	//	//
	//
	//	// <Snippet4>
	//	void* hostHandle;
	//	unsigned int domainId;
	//
	//	// This function both starts the .NET Core runtime and creates
	//	// the default (and only) AppDomain
	//	int hr = initializeCoreClr(
	//		env.runtimePath.c_str(),        // App base path
	//		"SampleHost",       // AppDomain friendly name
	//		sizeof(propertyKeys) / sizeof(char*),   // Property count
	//		propertyKeys,       // Property names
	//		propertyValues,     // Property values
	//		&hostHandle,        // Host handle
	//		&domainId);         // AppDomain ID
	//	// </Snippet4>
	//
	//	if (hr >= 0)
	//	{
	//		printf("CoreCLR started\n");
	//	}
	//	else
	//	{
	//		printf("coreclr_initialize failed - status: 0x%08x\n", hr);
	//		return -1;
	//	}
	//
	//	//
	//	// STEP 5: Create delegate to managed code and invoke it
	//	//
	//
	//	// <Snippet5>
	//	doWork_ptr managedDelegate;
	//
	//	// The assembly name passed in the third parameter is a managed assembly name
	//	// as described at https://docs.microsoft.com/dotnet/framework/app-domains/assembly-names
	//	hr = createManagedDelegate
	//	(
	//		hostHandle,
	//		domainId,
	//		"ManagedLibrary, Version=1.0.0.0",
	//		"ManagedLibrary.ManagedWorker",
	//		"DoWork",
	//		(void**)&managedDelegate
	//	);
	//	// </Snippet5>
	//
	//	if (hr >= 0)
	//	{
	//		printf("Managed delegate created\n");
	//	}
	//	else
	//	{
	//		printf("coreclr_create_delegate failed - status: 0x%08x\n", hr);
	//		return -1;
	//	}

	auto delegateResult = def->CreateManagedDelegate<doWork_ptr>();
	if (!delegateResult.success)
	{
		printf("coreclr_create_delegate failed - status: 0x%08x\n", delegateResult.failureCode);
		return -1;
	}

	auto managedDelegate = delegateResult.Delegate;

	// Create sample data for the double[] argument of the managed method to be called
	double data[4];
	data[0] = 0;
	data[1] = 0.25;
	data[2] = 0.5;
	data[3] = 0.75;

	// Invoke the managed delegate and write the returned string to the console
	char* ret = managedDelegate("Test job", 5, sizeof(data) / sizeof(double), data, ReportProgressCallback);

	printf("Managed code returned: %s\n", ret);

	// Strings returned to native code must be freed by the native code
#if WINDOWS
	CoTaskMemFree(ret);
#elif LINUX
	free(ret);
#endif


	//
	// STEP 6: Shutdown CoreCLR
	//
	def->Shutdown();

	// <Snippet6>
	//hr = shutdownCoreClr(hostHandle, domainId);
	//// </Snippet6>

	//if (hr >= 0)
	//{
	//	printf("CoreCLR successfully shutdown\n");
	//}
	//else
	//{
	//	printf("coreclr_shutdown failed - status: 0x%08x\n", hr);
	//}

	return 0;
}

// Callback function passed to managed code to facilitate calling back into native code with status
int ReportProgressCallback(int progress)
{
	// Just print the progress parameter to the console and return -progress
	printf("Received status from managed code: %d\n", progress);
	return -progress;
}