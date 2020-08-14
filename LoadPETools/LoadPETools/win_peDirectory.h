#pragma once
#include "tools.h"
#include "win_peExport.h"

#include "win_peImport.h"

INT_PTR CALLBACK winProcOfDirectory(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam);


//************************************
// Method:    addDirectory
// FullName:  ���Ŀ¼�༭������
// Access:    public 
// Returns:   DWORD 
// Qualifier:
// Parameter: HWND hwnd
//************************************
DWORD addDirectory(HWND hwnd);