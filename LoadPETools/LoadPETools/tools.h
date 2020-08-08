#pragma once
#include <windows.h>
#include <stdio.h>
#include <commctrl.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <ShlWapi.h>
#include "ay_pe.h"
#include "ay_io.h"
#include "const.h"
#include "resource.h"
#pragma comment(lib, "shlwapi")
#pragma comment(lib,"comctl32.lib")	


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



//************************************
// Method:    addMoudelListControlRow
// FullName:  添加模块列表通用空间内容
// Access:    public 
// Returns:   DWORD
// Qualifier:
// Parameter: HWND & hProcessListCtrl 进程列表通用空间句柄
// Parameter: HWND & hMoudelListCtrl 模块列表通用空间句柄
//************************************
DWORD addMoudelListControlRow(HWND& hProcessListCtrl, HWND& hMoudelListCtrl);



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



//************************************
// Method:    addPEEditWinContent
// FullName:  添加PE编辑框内容
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Parameter: HWND hwnd PE编辑窗口句柄
// Parameter: PTCHAR ptFileName 文件名指针,用于加载PE文件到缓冲区
//************************************
BOOL addPEEditWinContent(HWND hwnd, PTCHAR ptFileName);


//************************************
// Method:    showPEMagicWin
// FullName:  显示PE标记窗口
// Access:    public 
// Returns:   VOID
// Qualifier:
// Parameter: HWND hwnd 需指定的父窗口句柄
//************************************
VOID showPEMagicWin(HWND hwnd);