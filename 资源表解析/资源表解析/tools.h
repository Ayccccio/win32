#pragma once
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include <ShlWapi.h>
#include "ay_pe_tools.h"
#pragma comment(lib, "shlwapi");

void __cdecl OutputDebugStringF(const TCHAR* format, ...);

#ifdef _DEBUG  
#define DbgPrintf   OutputDebugStringF  
#else  
#define DbgPrintf  
#endif 


#ifdef UNICODE
#define fileOpen _wfopen_s
#else
#define fileOpen fopen_s
#endif


#ifdef UNICODE
#define strLen wcslen
#else
#define strLen strlen
#endif

