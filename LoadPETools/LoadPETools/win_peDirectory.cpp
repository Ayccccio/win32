#include "win_peDirectory.h"

INT_PTR winProcOfDirectory(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam) {
	switch (uMsg)
	{
	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
		break;
	}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case IDC_BUTTON_IMPORT:
		{
			DialogBox(hAPPInterface, MAKEINTRESOURCE(IDD_DIALOG_IMPORT), hwnd, winProcOfImport);
		}
		}
	}
	default:
		return FALSE;
	}
	return TRUE;
}