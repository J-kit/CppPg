#include "RuntimeUtils.h"

#include "stdafx.h"
RuntimeEnvironment RuntimeUtils::GetEnvironment(char* argv[])
{
	RuntimeEnvironment env;

	// Get the current executable's directory
	// This sample assumes that both CoreCLR and the
	// managed assembly to be loaded are next to this host
	// so we need to get the current path in order to locate those.
    env.runtimePath = RuntimeUtils::GetRuntimePath(argv);

	// Construct the CoreCLR path
	// For this sample, we know CoreCLR's path. For other hosts,
	// it may be necessary to probe for coreclr.dll/libcoreclr.so
	std::string coreClrPath(env.runtimePath);
	coreClrPath.append(FS_SEPARATOR);
	coreClrPath.append(CORECLR_FILE_NAME);

	// Construct the managed library path
	std::string managedLibraryPath(env.runtimePath);
	managedLibraryPath.append(FS_SEPARATOR);
	managedLibraryPath.append(MANAGED_ASSEMBLY);

	env.coreClrPath = coreClrPath;
	env.managedLibraryPath = managedLibraryPath;

	return env;
}

const RuntimeEnvironment& RuntimeUtils::GetEnvironmentRef(char* argv[])
{
	return RuntimeUtils::GetEnvironment(argv);
}


RuntimeEnvironment* RuntimeUtils::GetEnvironmentPtr(char* argv[])
{
	auto* env = new RuntimeEnvironment();

	// Get the current executable's directory
	// This sample assumes that both CoreCLR and the
	// managed assembly to be loaded are next to this host
	// so we need to get the current path in order to locate those.
	env->runtimePath = RuntimeUtils::GetRuntimePath(argv);

	// Construct the CoreCLR path
	// For this sample, we know CoreCLR's path. For other hosts,
	// it may be necessary to probe for coreclr.dll/libcoreclr.so
	std::string coreClrPath(env->runtimePath);
	coreClrPath.append(FS_SEPARATOR);
	coreClrPath.append(CORECLR_FILE_NAME);

	// Construct the managed library path
	std::string managedLibraryPath(env->runtimePath);
	managedLibraryPath.append(FS_SEPARATOR);
	managedLibraryPath.append(MANAGED_ASSEMBLY);

	env->coreClrPath = coreClrPath;
	env->managedLibraryPath = managedLibraryPath;

	return env;
}


std::string RuntimeUtils::GetRuntimePath(char** argv)
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

    return std::string(runtimePath);
}


