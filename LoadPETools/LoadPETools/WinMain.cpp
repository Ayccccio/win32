#pragma once
#include "const.h"
#include "tools.h"
#include "resource.h"



//主窗口消息处理回调函数
INT_PTR CALLBACK WindowsProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	HICON hIcon = NULL;				//图标句柄
	HWND hProcessListCtrl = NULL;	//进程列表通用控件句柄
	HWND hMoudelListCtrl = NULL;	//模块列表通用控件句柄

	WORD pdProListCtrlColWidth[ProcessListControlColumNumber] = { 0,200,50,80,50,100,100 };		//进程列表通用控件列的宽度
	WORD pdMudListCtrlColWidth[MoudelListControlColumNumber] = {50, 200,100,100,100 };			//模块列表通用控件列的宽度

	NMHDR* pNmhdr;			//WM_NOTIFY 消息结构指针
	

	switch (uMsg)
	{
	case WM_CLOSE:
	{
		EndDialog(hwnd,0);
		return TRUE;
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

		//添加进程通用控件内容
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
			if (!bFlag)	//判断文件对话框是否打开
			{
				if (openFileName(ptText, sizeof ptText))	//文件对话框获取文件目录
				{
					DialogBox(hAPPInterface, MAKEINTRESOURCE(IDD_DIALOG_PEEDIT), hwnd, PEDialogProc);
				}
				bFlag = FALSE;		//开关置0
			}
			break;
		}
		default:
			return FALSE;
		return TRUE;
		}
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
		return TRUE;
		}
	}
	default:
		return FALSE;
	return TRUE;
	}
	
		
	return FALSE;
}


int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd) 
{ 
	hAPPInterface = hInstance;

	//初始化通用控件
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icex);

	//提权
	processTokenUp(GetCurrentProcess(), SE_DEBUG_NAME);

	if (!bFlag)	//判断文件对话框是否打开
	{
		//if (openFileName(ptText, sizeof ptText))	//文件对话框获取文件目录
		//{
			wcsprintf(ptText, TEXT("C:\\Users\\\Ay\\Desktop\\1.exe"));
			DialogBox(hAPPInterface, MAKEINTRESOURCE(IDD_DIALOG_PEEDIT), GetDlgItem((HWND)hAPPInterface, IDD_DIALOG_MAIN), PEDialogProc);
		//}
		bFlag = FALSE;		//开关置0
	}
	//DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, WindowsProc);

	
}