#pragma once
#include "stdafx.h"
#include "RuntimeUtils.h"



class CoreClrDefinitions
{
public:
	coreclr_initialize_ptr initializeCoreClr;
	coreclr_create_delegate_ptr createManagedDelegate;
	coreclr_shutdown_ptr shutdownCoreClr;

	RuntimeEnvironment environment;

	template <typename T>
	ManagedDelegateResult<T> CreateManagedDelegate();



	CoreClrDefinitions();
	~CoreClrDefinitions();

	bool Shutdown();
	int Init(RuntimeEnvironment env);

private:
	bool initialized = false;
	
	void* clrHostHandle;
	unsigned int clrDomainId;

	int InitHostingFunctions();
	int InitCoreClr();
};
