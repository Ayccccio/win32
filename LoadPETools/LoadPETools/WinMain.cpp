#include "tools.h"
#include "resource.h"
#include "const.h"
#include <CommCtrl.h>
#pragma comment(lib,"commctrl.h")


INT_PTR CALLBACK WindowProc(HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam) 
{
	HICON hIcon = NULL;

	switch (uMsg)
	{
	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
		return TRUE;
	}
	case WM_INITDIALOG:
	{
		hIcon = LoadIcon(hAPPInstance, MAKEINTRESOURCE(IDI_ICON1));
		SendMessage((HWND)hAPPInstance, WM_SETICON, ICON_BIG, (DWORD)hIcon);
		SendMessage((HWND)hAPPInstance, WM_SETICON, ICON_SMALL, (DWORD)hIcon);
		return TRUE;
	}

	}
}


int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	hAPPInstance = hInstance;

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, WindowProc);
}