#include "const.h"
#include "tools.h"
#include "resource.h"
	

	HWND hProcessListCtrl = NULL;

INT_PTR CALLBACK WindowsProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	HICON hIcon = NULL;
	HWND hMoudelListCtrl = NULL;
	WORD pdProListCtrlColWidth[ProcessListControlColumNumber] = { 50,200,100,100,100 };
	WORD pdMudListCtrlColWidth[MoudelListControlColumNumber] = { 200,200};

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

		hProcessListCtrl = GetDlgItem(hwnd, IDC_LIST_PROCESS);	//��ȡ�����б�ͨ�ÿؼ����
		hMoudelListCtrl = GetDlgItem(hwnd, IDC_LIST_MOUDEL);	//��ȡģ���б�ͨ�ÿؼ����

		//���ý����б�ͨ�öԻ���ѡ��
		SendMessage(hProcessListCtrl, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

		//�б�ͨ�öԻ����ʼ����ͷ
		initListControlHeader(hProcessListCtrl, ProcessListControlColumNumber, (PTCHAR)TEXT("���\0������\0PID\0�����ַ\0�����С"), pdProListCtrlColWidth);	//��ʼ�������б�ͨ�ÿؼ���ͷ
		initListControlHeader(hMoudelListCtrl, MoudelListControlColumNumber, (PTCHAR)TEXT("ģ������\0ģ���ַ"),pdMudListCtrlColWidth);	//��ʼ�������б�ͨ�ÿؼ���ͷ

		/*
		LV_ITEM vitem;

		//��ʼ��						
		memset(&vitem, 0, sizeof(LV_ITEM));
		vitem.mask = LVIF_TEXT;

		vitem.pszText = (LPWSTR)TEXT("csrss.exe");
		vitem.iItem = 0;
		vitem.iSubItem = 0;
		ListView_InsertItem(hProcessListCtrl, &vitem);
		//SendMessage(hProcessListCtrl, LVM_INSERTITEM, 0, (DWORD)&vitem);

		vitem.pszText = (LPWSTR)TEXT("448");
		vitem.iItem = 0;
		vitem.iSubItem = 1;
		ListView_SetItem(hProcessListCtrl, &vitem);

		vitem.pszText = (LPWSTR)TEXT("56590000");
		vitem.iItem = 0;
		vitem.iSubItem = 2;
		ListView_SetItem(hProcessListCtrl, &vitem);

		vitem.pszText = (LPWSTR)TEXT("000F0000");
		vitem.iItem = 0;
		vitem.iSubItem = 3;
		ListView_SetItem(hProcessListCtrl, &vitem);

		vitem.pszText = (LPWSTR)TEXT("winlogon.exe");
		vitem.iItem = 1;
		vitem.iSubItem = 0;
		ListView_InsertItem(hProcessListCtrl, &vitem);
		//SendMessage(hProcessListCtrl, LVM_INSERTITEM, 0, (DWORD)&vitem);

		vitem.pszText = (LPWSTR)TEXT("456");
		vitem.iSubItem = 1;
		ListView_SetItem(hProcessListCtrl, &vitem);

		vitem.pszText = (LPWSTR)TEXT("10000000");
		vitem.iSubItem = 2;
		ListView_SetItem(hProcessListCtrl, &vitem);

		vitem.pszText = (LPWSTR)TEXT("000045800");
		vitem.iSubItem = 3;
		ListView_SetItem(hProcessListCtrl, &vitem);
		*/


		addProcessListControlRow(hProcessListCtrl);
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
		case IDC_BUTTON_ABOUT:
		{
			
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