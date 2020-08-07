#define  _CRT_SECURE_NO_WARNINGS

#pragma once
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

void __cdecl OutputDebugStringF(const TCHAR* format, ...);

#ifdef _DEBUG  
#define DbgPrintf   OutputDebugStringF  
#else  
#define DbgPrintf  
#endif 