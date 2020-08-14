#pragma once
#include "tools.h"


INT_PTR CALLBACK winProcOfImport(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam);


DWORD addDllListControlContent(HWND hwnd);

