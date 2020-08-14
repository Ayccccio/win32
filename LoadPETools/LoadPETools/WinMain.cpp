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

	//��ʼ��ͨ�ÿؼ�
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icex);

	//��Ȩ
	processTokenUp(GetCurrentProcess(), SE_DEBUG_NAME);

	if (!bFlag)	//�ж��ļ��Ի����Ƿ��
	{
		//if (openFileName(ptText, sizeof ptText))	//�ļ��Ի����ȡ�ļ�Ŀ¼
		//{
			wcsprintf(ptText, TEXT("C:\\Users\\\Ay\\Desktop\\1.exe"));
			DialogBox(hAPPInterface, MAKEINTRESOURCE(IDD_DIALOG_PEEDIT), GetDlgItem((HWND)hAPPInterface, IDD_DIALOG_MAIN), peEditWinProc);
		//}
		bFlag = FALSE;		//������0
	}
	//DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, WindowsProc);

	
}