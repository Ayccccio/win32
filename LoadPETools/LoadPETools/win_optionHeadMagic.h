#pragma once
#include "tools.h"

//可选PE头Magic标志窗口消息回调函数
INT_PTR CALLBACK winProcPEMagic(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam);


//************************************
// Method:    showWinOfOptionHeadMagic
// FullName:  显示PE标记窗口
// Access:    public 
// Returns:   VOID
// Qualifier:
// Parameter: HWND hwnd 需指定的父窗口句柄
//************************************
VOID showWinOfOptionHeadMagic(HWND hwnd);
