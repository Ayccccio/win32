#pragma once
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <ShlWapi.h>
#pragma comment(lib, "shlwapi")

void __cdecl OutputDebugStringF(const TCHAR* format, ...);

#ifdef _DEBUG  
#define DbgPrintf   OutputDebugStringF  
#else  
#define DbgPrintf  
#endif 


