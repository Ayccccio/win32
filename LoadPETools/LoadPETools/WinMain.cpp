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
		wcsprintf(ptTitle, TEXT("[ PE编辑器 ] - %s"), ptText);
		
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
	HICON hIcon = NULL;				//图标句柄
	HWND hProcessListCtrl = NULL;	//进程列表通用控件句柄
	HWND hMoudelListCtrl = NULL;	//模块列表通用控件句柄
	HWND hMainDlg = NULL;			//主Dialog句柄

	WORD pdProListCtrlColWidth[ProcessListControlColumNumber] = { 0,200,50,80,50,100,100 };		//进程列表通用控件列的宽度
	WORD pdMudListCtrlColWidth[MoudelListControlColumNumber] = {50, 200,100,100,100 };			//模块列表通用控件列的宽度

	NMHDR* pNmhdr;			//WM_NOTIFY 消息结构指针
	

	switch (uMsg)
	{
	case WM_CLOSE:
	{
		EndDialog(hwnd,0);
		return 0;
	}
	case WM_INITDIALOG:
	{
		//设置图标
		hIcon = LoadIcon(hAPPInterface, MAKEINTRESOURCE(IDI_ICON1));
		SendMessage((HWND)hAPPInterface, WM_SETICON, ICON_BIG, (DWORD)hIcon);
		SendMessage((HWND)hAPPInterface, WM_SETICON, ICON_SMALL, (DWORD)hIcon);

		hProcessListCtrl = GetDlgItem(hwnd, IDC_LIST_PROCESS);	//获取进程列表通用控件句柄
		hMoudelListCtrl = GetDlgItem(hwnd, IDC_LIST_MOUDEL);	//获取模块列表通用控件句柄

		//设置进程列表通用对话框选中
		SendMessage(hProcessListCtrl, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

		//列表通用对话框初始化表头
		initListControlHeader(hProcessListCtrl, ProcessListControlColumNumber, (PTCHAR)TEXT("序号\0进程名\0PID\0父级PID\0线程数\0镜像地址\0镜像大小"), pdProListCtrlColWidth);	//初始化进程列表通用控件表头
		initListControlHeader(hMoudelListCtrl, MoudelListControlColumNumber, (PTCHAR)TEXT("序号\0模块名称\0模块地址\0模块大小\0模块入口"),pdMudListCtrlColWidth);	//初始化进程列表通用控件表头



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
			if (openFileName(ptText, sizeof ptText))	//文件对话框获取文件目录
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