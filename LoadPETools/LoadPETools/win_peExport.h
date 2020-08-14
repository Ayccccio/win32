#pragma once
#include "tools.h"

INT_PTR CALLBACK winProcOfExport(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam);


DWORD addExportEditContent(HWND hwnd);


DWORD addExportListContent(HWND hwnd);