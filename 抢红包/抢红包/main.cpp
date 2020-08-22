#include "resource.h"
#include <windows.h>
#include <stdio.h>

#define ThreadNum 3

HWND hwndOfMain;						//主窗口句柄
HANDLE ghMutex = NULL;					//互斥体
HANDLE hThreads[ThreadNum] = { 0 };		//线程集合
int dwInitNum = 1000;					//总大小初始值
DWORD dwValues[ThreadNum] = { 0 };		//各个线程控制的值
DWORD dwDlgItem[ThreadNum] = { IDC_EDIT_NUM1, IDC_EDIT_NUM2, IDC_EDIT_NUM3 };	//编辑框ID集合
TCHAR ptBuff[128] = { 0 };				//文本缓冲区
DWORD dwIndex[ThreadNum] = { 0,1,2 };	//线程索引,线程回调函数参数传值

//开始线程
DWORD WINAPI threadProcOfStart(LPVOID lParameter) {

	while (1)		//总数是否满足条件
	{

		WaitForSingleObject(ghMutex, -1);	//判断互斥体
		if (dwInitNum < 50)
		{
			break;
		}

		dwInitNum -= 50;		//总数减50

		dwValues[*(DWORD*)lParameter] += 50;	//线程值变量加50

		//编辑框设置值
		wsprintf(ptBuff, TEXT("%d"), dwValues[*(DWORD*)lParameter]);
		SetDlgItemText(hwndOfMain, dwDlgItem[*(DWORD*)lParameter], ptBuff);


		ReleaseMutex(ghMutex);	//释放互斥体
		Sleep(50);
	}
	return 0;
}

//开始线程控制
DWORD WINAPI threadProcOfStartCtrl(LPVOID lParameter) {
	DWORD i = 0;
	while (i < ThreadNum)		//创建线程
	{
		hThreads[i] = ::CreateThread(NULL, 0, threadProcOfStart, &dwIndex[i], 0, NULL);
		i++;
	}

	WaitForMultipleObjects(ThreadNum, hThreads, TRUE, -1);	//等待全部线程结束
	i = 0;
	while (i < ThreadNum)		//释放线程句柄
	{
		::CloseHandle(hThreads[i]);
		i++;
	}
	//::CloseHandle(ghMutex);		//释放互斥体
	return 0;
}

//消息回调
INT_PTR CALLBACK winProcOfWinMain(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam) 
{
	TCHAR ptBuff[128] = { 0 };
	switch (uMsg)
	{
	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
		return TRUE;
	}
	case WM_INITDIALOG:
	{
		hwndOfMain = hwnd;
		wsprintf(ptBuff, TEXT("%d"), dwInitNum);
		SetDlgItemText(hwnd, IDC_EDIT_COUNT, ptBuff);
		return TRUE;
	}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case IDC_BUTTON_START:
		{
			//值清空
			memset(dwValues, 0, sizeof dwValues);		
			//编辑框清空
			SetDlgItemText(hwnd, IDC_EDIT_NUM1, TEXT(""));
			SetDlgItemText(hwnd, IDC_EDIT_NUM2, TEXT(""));
			SetDlgItemText(hwnd, IDC_EDIT_NUM3, TEXT(""));

			//从编辑框获取总值
			GetDlgItemText(hwnd, IDC_EDIT_COUNT, ptBuff, sizeof ptBuff);
			swscanf_s(ptBuff, TEXT("%d"), &dwInitNum);

			//创建控制线程
			::CreateThread(NULL, 0, threadProcOfStartCtrl, NULL, 0, NULL);

			return TRUE;
		}
		}
	}
	}
	return FALSE;
}

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd)
{
	ghMutex = CreateMutex(NULL, FALSE, NULL);		//创建互斥体
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, winProcOfWinMain);
}