#pragma once
#include "tools.h"
#include "win_peExport.h"

INT_PTR CALLBACK winProcOfDirectory(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam);


//************************************
// Method:    addDirectory
// FullName:  Ìí¼ÓÄ¿Â¼±à¼­¿òÄÚÈÝ
// Access:    public 
// Returns:   DWORD 
// Qualifier:
// Parameter: HWND hwnd
//************************************
DWORD addDirectory(HWND hwnd);