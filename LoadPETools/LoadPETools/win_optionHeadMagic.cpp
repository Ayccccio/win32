#include "win_optionHeadMagic.h"

INT_PTR CALLBACK peMagicWinProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam) {
	switch (uMsg)
	{
	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
		return TRUE;
	}

	}
	return FALSE;
}

VOID showWinOfOptionHeadMagic(HWND hwnd) {
	DialogBox(hAPPInterface, MAKEINTRESOURCE(IDD_DIALOG_PEMAGIC), hwnd, peMagicWinProc);
}


