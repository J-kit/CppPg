#pragma once
#include "stdafx.h"
#include "RuntimeUtils.h"
#include "ManagedDelegateResult.h"


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