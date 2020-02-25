#pragma once

template <typename T>
struct ManagedDelegateResult {
	bool success;
	int failureCode;

	T Delegate;
};

