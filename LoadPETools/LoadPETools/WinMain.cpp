#include "const.h"
#include "tools.h"
#include "resource.h"
	


INT_PTR CALLBACK WindowsProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	HICON hIcon = NULL;
	HWND hListCtrl = NULL;
	TCHAR ptColumNames[1024] = TEXT("������\0PID\0�����ַ\0�����С");

	switch (uMsg)
	{
	case WM_CLOSE:
	{
		EndDialog(hwnd,0);
		return 0;
	}
	case WM_INITDIALOG:
	{
		//����ͼ��
		hIcon = LoadIcon(hAPPInterface, MAKEINTRESOURCE(IDI_ICON1));
		SendMessage((HWND)hAPPInterface, WM_SETICON, ICON_BIG, (DWORD)hIcon);
		SendMessage((HWND)hAPPInterface, WM_SETICON, ICON_SMALL, (DWORD)hIcon);

		//�б�ͨ�öԻ����ʼ����ͷ
		hListCtrl = GetDlgItem(hwnd, IDC_LIST_PROCESS);	//��ȡ�б�ͨ�ÿؼ����
		initListControlHeader(hListCtrl, 4, ptColumNames);

		return TRUE;
	}
	case WM_COMMAND: 
	{
		switch (wParam)
		{
		case IDC_BUTTON_LOGOUT:
		{
			EndDialog(hwnd, 0);
			break;
		}
		default:
			break;
		}
		return TRUE;
	}
	case WM_NOTIFY:
	{
		
	}
	}
	return FALSE;
}


INT_PTR CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd) 
{
	hAPPInterface = hInstance;

	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icex);

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, WindowsProc);
}