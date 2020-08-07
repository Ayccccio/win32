#pragma once
#include "const.h"
#include "tools.h"
#include "resource.h"

INT_PTR CALLBACK PEDialogProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	TCHAR ptTitle[512] = { 0 };
	HWND hPEDialog;

	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		wcsprintf(ptTitle, TEXT("[ PE�༭�� ] - %s"), ptText);
		
		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)ptTitle);

		addPEWindowContent(hwnd,ptText);

		return TRUE;
	}
	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
		return TRUE;
	}
	}
	return FALSE;
}


INT_PTR CALLBACK WindowsProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	HICON hIcon = NULL;				//ͼ����
	HWND hProcessListCtrl = NULL;	//�����б�ͨ�ÿؼ����
	HWND hMoudelListCtrl = NULL;	//ģ���б�ͨ�ÿؼ����
	HWND hMainDlg = NULL;			//��Dialog���

	WORD pdProListCtrlColWidth[ProcessListControlColumNumber] = { 0,200,50,80,50,100,100 };		//�����б�ͨ�ÿؼ��еĿ��
	WORD pdMudListCtrlColWidth[MoudelListControlColumNumber] = {50, 200,100,100,100 };			//ģ���б�ͨ�ÿؼ��еĿ��

	NMHDR* pNmhdr;			//WM_NOTIFY ��Ϣ�ṹָ��
	

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
		initListControlHeader(hProcessListCtrl, ProcessListControlColumNumber, (PTCHAR)TEXT("���\0������\0PID\0����PID\0�߳���\0�����ַ\0�����С"), pdProListCtrlColWidth);	//��ʼ�������б�ͨ�ÿؼ���ͷ
		initListControlHeader(hMoudelListCtrl, MoudelListControlColumNumber, (PTCHAR)TEXT("���\0ģ������\0ģ���ַ\0ģ���С\0ģ�����"),pdMudListCtrlColWidth);	//��ʼ�������б�ͨ�ÿؼ���ͷ



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
		case IDC_BUTTON_PEEDIT:
		{
			if (openFileName(ptText, sizeof ptText))	//�ļ��Ի����ȡ�ļ�Ŀ¼
			{
				hMainDlg = GetDlgItem((HWND)hAPPInterface, IDD_DIALOG_MAIN);
				DialogBox(hAPPInterface, MAKEINTRESOURCE(IDD_DIALOG_PEEDIT), hMainDlg, PEDialogProc);
			}
		}
		}
		return TRUE;
	}
	case WM_NOTIFY:
	{
		pNmhdr = (NMHDR*)lParam;
		switch (wParam)
		{
		case IDC_LIST_PROCESS:
		{
			hProcessListCtrl = GetDlgItem(hwnd, IDC_LIST_PROCESS);
			if (pNmhdr->code == NM_CLICK)
			{
				hMoudelListCtrl = GetDlgItem(hwnd, IDC_LIST_MOUDEL);
				ListView_DeleteAllItems(hMoudelListCtrl);
				addMoudelListControlRow(hProcessListCtrl,hMoudelListCtrl);
			}
		}
		}
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