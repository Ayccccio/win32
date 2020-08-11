#pragma once
#include "tools.h"

//选择特征属性窗口消息回调函数
INT_PTR CALLBACK selectCharacterOfFileHeadWinProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam);

//************************************
// Method:    openSelCharacterWinOfFileHead
// FullName:  打开标准PE头选择特征窗口
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Parameter: HWND hwnd 父窗口句柄
//************************************
BOOL openSelCharacterWinOfFileHead(HWND hwnd);



//************************************
// Method:    setCharacterOfFileHead
// FullName:  设置标准PE头的特征值
// Access:    public 
// Returns:   DWORD
// Qualifier:
// Parameter: HWND hwnd 特征值选择窗口句柄
// Parameter: int index 复选框ID数组索引
// Parameter: PDWORD pdCharacterValue 特征值指针
//************************************
DWORD setCharacterOfFileHead(HWND hwnd, int index, PDWORD pdCharacterValue);