#pragma once
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <ShlWapi.h>
#include <commctrl.h>			
#pragma comment(lib, "shlwapi")
#pragma comment(lib,"comctl32.lib")		

void __cdecl OutputDebugStringF(const TCHAR* format, ...);

#ifdef _DEBUG  
#define DbgPrintf   OutputDebugStringF  
#else  
#define DbgPrintf  
#endif 

#ifdef UNICODE
#define StrLen wcslen
#else
#define StrLen strlen
#endif

//************************************
// Method:    initListControlHeader
// FullName:  初始化列表通用控件表头
// Access:    public 
// Returns:   DWORD
// Qualifier:
// Parameter: HWND hListControl 通用控件句柄
// Parameter: DWORD dwLenth 列数
// Parameter: PTCHAR & ptColumNames 列名缓冲区,列名必须连续存储,以结束符隔开
//************************************
DWORD initListControlHeader(HWND hListControl, DWORD dwLenth, PTCHAR ptColumNames);