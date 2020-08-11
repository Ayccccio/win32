#pragma once
#include "tools.h"

//选择子系统窗口消息回调函数
INT_PTR CALLBACK selectSubSystemWinProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam);

//************************************
// Method:    openSelectSubSystem
// FullName:  选择子系统标志值放全局变量中的ptText中
// Access:    public 
// Returns:   PTCHAR
// Qualifier:
// Parameter: HWND hwnd 用于指定父窗口句柄
//************************************
BOOL openSelectSubSystem(HWND hwnd);