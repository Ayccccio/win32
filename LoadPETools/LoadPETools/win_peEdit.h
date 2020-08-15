#pragma once
#include "tools.h"
#include "win_fileHeadCharacter.h"
#include "win_subSystem.h"
#include "win_optionHeadMagic.h"
#include "win_sectionTable.h"
#include "win_peDirectory.h"


//PE编辑窗口消息处理回调函数
INT_PTR CALLBACK winProcPEEdit(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam);

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

