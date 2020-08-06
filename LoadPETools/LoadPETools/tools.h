#pragma once
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <ShlWapi.h>
#include <commctrl.h>
#include <TlHelp32.h>
#include <Psapi.h>
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

#ifdef UNICODE
#define wcsprintf wsprintf
#else
#define wcsprintf sprintf
#endif

//************************************
// Method:    initListControlHeader
// FullName:  初始化列表通用控件表头
// Access:    public 
// Returns:   DWORD
// Qualifier:
// Parameter: HWND hListControl 通用控件句柄
// Parameter: DWORD dwLenth 列数
// Parameter: PTCHAR ptColumNames 列名缓冲区,列名必须连续存储,以结束符隔开
// Parameter: PWORD pdColWidths 所有列宽
//************************************
DWORD initListControlHeader(HWND hListControl, DWORD dwLenth, PTCHAR ptColumNames, PWORD pdColWidths);


//************************************
// Method:    processTokenUp
// FullName:  提升进程令牌权限
// Access:    public 
// Returns:   BOOL 成功返回真,失败返回假
// Qualifier:
// Parameter: HANDLE processHandle 进程句柄
// Parameter: LPCWSTR privilege 权限类型
//************************************
BOOL processTokenUp(HANDLE processHandle, LPCWSTR privilege);



//************************************
// Method:    initProcessListControlRow
// FullName:  初始化进程列表通用控件内容
// Access:    public 
// Returns:   DWORD 成功返回添加行数,失败返回0
// Qualifier:
// Parameter: HWND hListControl 列表通用控件句柄
//************************************
DWORD addProcessListControlRow(HWND hListControl);




DWORD addMoudelListControlRow(HWND& hProcessListCtrl, HWND& hMoudelListCtrl);