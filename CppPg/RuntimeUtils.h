#pragma once
#include "stdafx.h"



class RuntimeUtils
{
public:
    static RuntimeEnvironment GetEnvironment(char* argv[]);
    static const RuntimeEnvironment& GetEnvironmentRef(char* argv[]);
    static RuntimeEnvironment* GetEnvironmentPtr(char* argv[]);
    static std::string BuildTpaList(const char* directory, const char* extension);
private:
    static std::string GetRuntimePath(char** argv);
};


