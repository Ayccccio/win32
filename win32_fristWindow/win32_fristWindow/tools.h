#pragma once
#include <windows.h>
#include <stdio.h>

void __cdecl OutputDebugStringF(const char* format, ...);

#ifdef _DEBUG  
#define DbgPrintf   OutputDebugStringF  
#else  
#define DbgPrintf  
#endif 
