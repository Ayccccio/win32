#include "resource.h"
#include <windows.h>
#include <stdio.h>

#define ThreadNum 3

HANDLE ghEvent;			//事件对象
DWORD dwNum = 0;		//共享值
HWND hMain;				//主窗口句柄
BYTE pbIndex[ThreadNum] = { 0,1,2 };	//线程索引,用于线程回调函数传参,控制编辑框用
DWORD pdDlgItemId[ThreadNum] = { IDC_EDIT_NUM1, IDC_EDIT_NUM2,IDC_EDIT_NUM3 };	//编辑框ID,
HANDLE pThreadHandles[ThreadNum] = { 0 };	//线程句柄存放

//线程开始回调函数
DWORD WINAPI threadProcOfStart(LPVOID lParameter) {
	TCHAR ptBuff[128] = { 0 };

	//拦截内核对象通知状态
	WaitForSingleObject(ghEvent, -1);

	//获取总数编辑框的值,放入对应编辑框中
	wsprintf(ptBuff, TEXT("%d"), dwNum);
	SetDlgItemText(hMain, pdDlgItemId[*(PBYTE)lParameter], ptBuff);

	Sleep(200);
	//将内核对象通知状态改为已通知
	SetEvent(ghEvent);
	return 0;
}

//线程控制回调函数
DWORD WINAPI threadProcOfStartCtrl(LPVOID lParameter) {
	TCHAR ptBuff[128] = { 0 };
	int i = 0;
	//创建内核事件对象,默认未通知状态,wait后默认改未通知状态需手动改状态
	

	//初始化线程句柄集合
	memset(pThreadHandles, 0, sizeof pThreadHandles);

	//循环创建线程
	while (i < ThreadNum)
	{
		pThreadHandles[i] = CreateThread(NULL, 0, threadProcOfStart, &pbIndex[i], 0, NULL);
		i++;
	}

	//编辑框1循环加
	i = 0;
	while (i < 100)
	{
		dwNum++;
		wsprintf(ptBuff, TEXT("%d"), dwNum);
		SetDlgItemText(hMain, IDC_EDIT_COUNT, ptBuff);
		i++;
		Sleep(20);
	}
	SetEvent(ghEvent);

	//等待线程全部执行完毕
	WaitForMultipleObjects(ThreadNum, pThreadHandles, TRUE, -1);
	i = 0;
	while (i < ThreadNum)	//释放线程句柄
	{
		CloseHandle(pThreadHandles[i]);
		i++;
	}

	//重置事件对象
	ResetEvent(ghEvent);
	EnableWindow(GetDlgItem(hMain, IDC_BUTTON_START), TRUE);	//启用按钮
	return 0;
}

int CALLBACK winProcOfMain(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	int i = 0;
	switch (uMsg)
	{
	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
		CloseHandle(ghEvent);		//释放事件对象
		return TRUE;
	}
	case WM_INITDIALOG:
	{
		ghEvent = CreateEvent(NULL, FALSE, FALSE, NULL);		//创建事件对象
		hMain = hwnd;
		return TRUE;
	}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case IDC_BUTTON_START:
		{
			//清空数值编辑框的内容
			while (i < ThreadNum)
			{
				SetDlgItemText(hwnd, pdDlgItemId[i], TEXT(""));
				i++;
			}

			//禁用开始按钮
			EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_START),FALSE);

			CloseHandle(CreateThread(NULL, 0, threadProcOfStartCtrl, NULL, 0, NULL));		//创建线程控制线程
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
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, winProcOfMain);
}