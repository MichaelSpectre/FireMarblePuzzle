#pragma once

#include <sdkddkver.h>
#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Just a quick method to help release
// COM objects which are still valid
template <typename T>
inline void SAFE_RELEASE(T *&p)
{
	if (nullptr != p)
	{
		p->Release();
		p = nullptr;
	}
}