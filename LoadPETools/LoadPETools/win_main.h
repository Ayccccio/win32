#pragma once
#include "tools.h"
#include "win_peEdit.h"

//主窗口消息处理回调函数
INT_PTR CALLBACK WindowsProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam);

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