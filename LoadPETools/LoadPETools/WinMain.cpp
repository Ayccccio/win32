#pragma once
#include "const.h"
#include "tools.h"
#include "win_main.h"
#include "resource.h"


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
			DialogBox(hAPPInterface, MAKEINTRESOURCE(IDD_DIALOG_PEEDIT), GetDlgItem((HWND)hAPPInterface, IDD_DIALOG_MAIN), peEditWinProc);
		//}
		bFlag = FALSE;		//开关置0
	}
	//DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, WindowsProc);

	
}