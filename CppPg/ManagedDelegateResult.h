#pragma once
#include "stdafx.h"

template <typename T>
struct ManagedDelegateResult {
	bool success;
	int failureCode;

	T Delegate;
};