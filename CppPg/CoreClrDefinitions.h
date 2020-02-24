#pragma once
#include "stdafx.h"
#include <vector>
#include "RuntimeUtils.h"


class CoreClrDefinitions
{
public:
	coreclr_initialize_ptr initializeCoreClr;
	coreclr_create_delegate_ptr createManagedDelegate;
	coreclr_shutdown_ptr shutdownCoreClr;

	CoreClrDefinitions();
	int Init(RuntimeEnvironment env);

	~CoreClrDefinitions();

private:
	bool initiated = false;
};


