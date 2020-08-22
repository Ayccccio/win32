#pragma once
#include <stdio.h>
#include <ShlWapi.h>
#include <windowsx.h>
#include <windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <commctrl.h>
#pragma comment(lib,"comctl32.lib")	
#pragma comment(lib, "shlwapi")



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

#ifdef UNICODE
#define swcscanf_s swscanf_s
#else
#define swcscanf_s sscanf_s
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
// Method:    openFileName
// FullName:  打开文件对话框
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Parameter: PTCHAR ptText 文本缓冲区,用来存储文件对话框返回的文件路径
// Parameter: DWORD dwBuffSize 文本缓冲区大小
//************************************
BOOL openFileName(PTCHAR ptText, DWORD dwBuffSize);

