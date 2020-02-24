#pragma once
#include "stdafx.h"
#include <vector>
#include "RuntimeUtils.h"


class CoreClrDefinitions
{
public:
	static coreclr_initialize_ptr initializeCoreClr;
	static coreclr_create_delegate_ptr createManagedDelegate;
	static coreclr_shutdown_ptr shutdownCoreClr;

	CoreClrDefinitions();
	int Init(RuntimeEnvironment env);

	~CoreClrDefinitions();

private:
	bool initiated = false;
};


