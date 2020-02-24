#pragma once
#include "stdafx.h"

struct RuntimeEnvironment {
    std::string runtimePath;
    std::string coreClrPath;
    std::string managedLibraryPath;
};


class RuntimeUtils
{
public:
    static RuntimeEnvironment GetEnvironment(char* argv[]);
    static const RuntimeEnvironment& GetEnvironmentRef(char* argv[]);
    static RuntimeEnvironment* GetEnvironmentPtr(char* argv[]);
private:
    static std::string GetRuntimePath(char** argv);
};


