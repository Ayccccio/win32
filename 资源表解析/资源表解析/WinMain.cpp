#include "const.h"
#include "resource.h"

BOOL CALLBACK WindowProc(HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam) {
	HICON hIcon;
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		hIcon = LoadIcon(hAPPInterface, MAKEINTRESOURCE(IDI_ICON_LOAD));
		SendMessage((HWND)hAPPInterface, WM_SETICON, ICON_BIG, (DWORD)hIcon);
		SendMessage((HWND)hAPPInterface, WM_SETICON, ICON_SMALL, (DWORD)hIcon);
		return TRUE;
	}
	case WM_DESTROY:
	{
		EndDialog(hwnd, 0);
		return TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		
		case IDC_BUTTON_OPEN:
		{
			//定义OPENFILENAME结构
			OPENFILENAME ofn = { 0 };
			TCHAR pFileNameBuff[MAX_PATH] = { 0 };
			ofn.lStructSize = sizeof OPENFILENAME;
			ofn.hwndOwner = NULL;
			ofn.lpstrFilter = TEXT("PE文件\0*.exe");
			ofn.nFilterIndex = 1;
			ofn.lpstrFile = pFileNameBuff;
			ofn.nMaxFile = sizeof pFileNameBuff;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

			GetOpenFileName(&ofn);	//打开文件对话框
		}
		}
	}
	return FALSE;
}

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
) {
	hAPPInterface = hInstance;

	DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, WindowProc);
}