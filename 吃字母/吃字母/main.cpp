#include "resource.h"
#include "tools.h"
#include <windows.h>

TCHAR ptStr[256] = { 0 };		//字符串
PTCHAR ptStrPoint = NULL;		//字符串指针
TCHAR ptBuff[4] = { 0 };		//保存2个缓冲区字符
TCHAR ptTexts[4][128] = { {0},{0}, {0}, {0} };		//线程吃掉的字符
CRITICAL_SECTION cs = { 0 };	//临界区
HANDLE ghSignal;				//信号量

HWND hMain;
WORD pwDlgItemIdsOfBuff[2] = { IDC_EDIT_BUFF1 ,IDC_EDIT_BUFF2};

//取字母到缓冲区线程
DWORD WINAPI threadOfBuff(LPVOID lParameter) {
	DWORD dwStrLen = 0;
	while (1)
	{

		//进入临界区
		EnterCriticalSection(&cs);

		//获取字符长度,判断是否还有字符
		dwStrLen = StrLen(ptStrPoint);
		if (dwStrLen == 0)
			break;

		//取首个字符到缓冲区
		ptBuff[(DWORD)lParameter * 2] = *ptStrPoint;

		//设置字符到缓冲区编辑框
		SetDlgItemText(hMain, pwDlgItemIdsOfBuff[(DWORD)lParameter], ptBuff + (DWORD)lParameter * 2);

		//
		ptStrPoint++;
		ReleaseSemaphore(ghSignal, 1, NULL);	//信号量加1,增加一个线程

		//释放临界区
		LeaveCriticalSection(&cs);
	}

}

//取缓冲区字符线程
DWORD WINAPI threadOfGetBuff(LPVOID lParameter) {
	while (1)
	{
		//拦截信号量
		WaitForSingleObject(ghSignal, -1);

		//进入临界区
		EnterCriticalSection(&cs);
		if (*ptBuff != TEXT('\0'))
		{
			StrCat(ptTexts[(DWORD)lParameter], ptBuff);
		}
		else if (*(ptBuff + 1) != TEXT('\0'))
		{
			StrCat(ptTexts[(DWORD)lParameter], ptBuff + 1);
		}
		else
			continue;

	}
	

}


//线程控制函数
DWORD WINAPI threadCtrl(LPVOID lParameter) 
{

}


//主窗口消息回调函数
INT_PTR CALLBACK winProcOfMain(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam) 
{
	switch (uMsg)
	{
	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
		return TRUE;
	}
	case WM_INITDIALOG:
	{

		return TRUE;
	}
	case WM_COMMAND:
	{

		return TRUE;
	}
	}
}

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd)
{
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, winProcOfMain);
}