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

	// STEP 3: Construct properties used when starting the runtime

	// Construct the trusted platform assemblies (TPA) list
	// This is the list of assemblies that .NET Core can load as
	// trusted system assemblies.
	// For this host (as with most), assemblies next to CoreCLR will
	// be included in the TPA list
	env.tpaList = RuntimeUtils::BuildTpaList(env.runtimePath.c_str(), ".dll");

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



#if WINDOWS
// Win32 directory search for .dll files
// <Snippet7>
std::string RuntimeUtils::BuildTpaList(const char* directory, const char* extension)
{
	std::string tpaList;
	// This will add all files with a .dll extension to the TPA list.
	// This will include unmanaged assemblies (coreclr.dll, for example) that don't
	// belong on the TPA list. In a real host, only managed assemblies that the host
	// expects to load should be included. Having extra unmanaged assemblies doesn't
	// cause anything to fail, though, so this function just enumerates all dll's in
	// order to keep this sample concise.
	std::string searchPath(directory);
	searchPath.append(FS_SEPARATOR);
	searchPath.append("*");
	searchPath.append(extension);

	WIN32_FIND_DATAA findData;
	HANDLE fileHandle = FindFirstFileA(searchPath.c_str(), &findData);

	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		do
		{
			// Append the assembly to the list
			tpaList.append(directory);
			tpaList.append(FS_SEPARATOR);
			tpaList.append(findData.cFileName);
			tpaList.append(PATH_DELIMITER);

			// Note that the CLR does not guarantee which assembly will be loaded if an assembly
			// is in the TPA list multiple times (perhaps from different paths or perhaps with different NI/NI.dll
			// extensions. Therefore, a real host should probably add items to the list in priority order and only
			// add a file if it's not already present on the list.
			//
			// For this simple sample, though, and because we're only loading TPA assemblies from a single path,
			// and have no native images, we can ignore that complication.
		} while (FindNextFileA(fileHandle, &findData));
		FindClose(fileHandle);
	}

	return tpaList;
}
// </Snippet7>
#elif LINUX
// POSIX directory search for .dll files
std::string RuntimeUtils::BuildTpaList(const char* directory, const char* extension)
{
	std::string tpaList;
	DIR* dir = opendir(directory);
	struct dirent* entry;
	int extLength = strlen(extension);

	while ((entry = readdir(dir)) != NULL)
	{
		// This simple sample doesn't check for symlinks
		std::string filename(entry->d_name);

		// Check if the file has the right extension
		int extPos = filename.length() - extLength;
		if (extPos <= 0 || filename.compare(extPos, extLength, extension) != 0)
		{
			continue;
		}

		// Append the assembly to the list
		tpaList.append(directory);
		tpaList.append(FS_SEPARATOR);
		tpaList.append(filename);
		tpaList.append(PATH_DELIMITER);

		// Note that the CLR does not guarantee which assembly will be loaded if an assembly
		// is in the TPA list multiple times (perhaps from different paths or perhaps with different NI/NI.dll
		// extensions. Therefore, a real host should probably add items to the list in priority order and only
		// add a file if it's not already present on the list.
		//
		// For this simple sample, though, and because we're only loading TPA assemblies from a single path,
		// and have no native images, we can ignore that complication.
	}
	return tpaList;
}
#endif



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


