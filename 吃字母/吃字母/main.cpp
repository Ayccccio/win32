#include "resource.h"
#include "tools.h"

#define SetBuffThreadNum 2
#define GetBuffThreadNum 4


TCHAR ptStr[256] = { 0 };		//字符串
PTCHAR ptStrPoint = NULL;		//字符串指针
TCHAR ptBuff[SetBuffThreadNum][2] = { 0 };		//保存2个缓冲区字符
TCHAR ptTexts[GetBuffThreadNum][128] = { 0 };		//线程吃掉的字符
CRITICAL_SECTION cs = { 0 };	//临界区
HANDLE ghSignal;				//取缓冲区字符信号量
HANDLE ghSetbuffSignal;				//设置缓冲区字符信号量

HWND hMain;			//主窗口句柄
WORD pwDlgItemIdsOfSetBuff[SetBuffThreadNum] = { IDC_EDIT_BUFF1 ,IDC_EDIT_BUFF2};	//缓冲区编辑框窗口id
WORD pwDlgItemIdsOfGetBuff[GetBuffThreadNum] = { IDC_EDIT_THREAD1 ,IDC_EDIT_THREAD2,IDC_EDIT_THREAD3,IDC_EDIT_THREAD4 };
HANDLE hSetBuffThreads[SetBuffThreadNum] = { 0 };		//设置缓冲区线程数组
HANDLE hGetBuffThreads[GetBuffThreadNum] = { 0 };		//获取缓冲区字符线程数组

//设置缓冲区字符到编辑框线程
DWORD WINAPI threadOfSetBuff(LPVOID lParameter) {
	DWORD dwStrLen;
	while (1)
	{

		WaitForSingleObject(ghSetbuffSignal, -1);

		//进入临界区
		EnterCriticalSection(&cs);

		//获取字符长度,判断是否还有字符
		dwStrLen = StrLen(ptStrPoint);
 		if (dwStrLen == 0)
			break;

		//取首个字符到缓冲区
		ptBuff[(DWORD)lParameter][0] = *ptStrPoint;
		ptStrPoint++;

		//设置字符到缓冲区编辑框
		SetDlgItemText(hMain, pwDlgItemIdsOfSetBuff[(DWORD)lParameter], ptBuff[(DWORD)lParameter]);

		Sleep(200);


		//信号量加1,增加一个线程运行,信号量加1放释放临界区前面是为了避免准备妥当了,获取缓冲区字符线程却还不执行
		ReleaseSemaphore(ghSignal, 1, NULL);	
		
		//释放临界区
		LeaveCriticalSection(&cs);

	}
	return 0;
}

//取缓冲区字符线程
DWORD WINAPI threadOfGetBuff(LPVOID lParameter) {
	while (1)
	{
		//拦截信号量
		WaitForSingleObject(ghSignal, -1);

		//进入临界区
		EnterCriticalSection(&cs);

		//取缓冲区字符,放到自身缓冲区中
		if (ptBuff[0][0] != TEXT('\0'))
		{
			StrCat(ptTexts[(DWORD)lParameter], ptBuff[0]);
			ptBuff[0][0] = TEXT('\0');
			SetDlgItemText(hMain, pwDlgItemIdsOfSetBuff[0], TEXT(""));
		}
		else if (ptBuff[1][0] != TEXT('\0'))
		{
			StrCat(ptTexts[(DWORD)lParameter], ptBuff[1]);
			ptBuff[1][0] = TEXT('\0');
			SetDlgItemText(hMain, pwDlgItemIdsOfSetBuff[1], TEXT(""));
		}
		else {
			if (WaitForMultipleObjects(SetBuffThreadNum, hSetBuffThreads, TRUE, 1000) == WAIT_OBJECT_0)
			{
				break;
			}
			else {
				LeaveCriticalSection(&cs);
				continue;
			}
		}
		SetDlgItemText(hMain, pwDlgItemIdsOfGetBuff[(DWORD)lParameter], ptTexts[(DWORD)lParameter]);
		Sleep(200);

		//设置缓冲区信号量加1,让缓冲区可以得到填充
		ReleaseSemaphore(ghSetbuffSignal, 1, NULL);

		//释放临界区
		LeaveCriticalSection(&cs);
	}
	return 0;
}


//线程控制函数
DWORD WINAPI threadCtrl(LPVOID lParameter) 
{
	DWORD dwGetBuffThreadNum = 0;
	DWORD dwSetBuffThreadNum = 0;

	//创建抢缓冲区字符线程
	while (dwGetBuffThreadNum < GetBuffThreadNum)
	{
		hGetBuffThreads[dwGetBuffThreadNum] = CreateThread(NULL, 0, threadOfGetBuff, (LPVOID)(dwGetBuffThreadNum), 0, NULL);
		dwGetBuffThreadNum++;
	}

	//创建设置字符缓冲区线程
	while (dwSetBuffThreadNum < SetBuffThreadNum)
	{
		hSetBuffThreads[dwSetBuffThreadNum] = CreateThread(NULL, 0, threadOfSetBuff, (LPVOID)(dwSetBuffThreadNum), 0, NULL);
		dwSetBuffThreadNum++;
	}

	
	//阻塞设置缓冲区线程
	WaitForMultipleObjects(SetBuffThreadNum, hSetBuffThreads, TRUE, -1);
	dwSetBuffThreadNum = 0;
	while (dwSetBuffThreadNum < SetBuffThreadNum)
	{
		CloseHandle(hSetBuffThreads[dwSetBuffThreadNum]);
	}

	WaitForMultipleObjects(GetBuffThreadNum, hGetBuffThreads, TRUE, -1);
	dwGetBuffThreadNum = 0;
	while (dwGetBuffThreadNum < GetBuffThreadNum)
	{
		CloseHandle(hGetBuffThreads[dwGetBuffThreadNum]);
	}

	

	EnableWindow(GetDlgItem(hMain, IDC_BUTTON_START), TRUE);

	return 0;
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
		CloseHandle(ghSignal);
		EndDialog(hwnd, 0);
		return TRUE;
	}
	case WM_INITDIALOG:
	{
		SetDlgItemText(hwnd, IDC_EDIT_STR, TEXT("AB"));
		hMain = hwnd;
		ghSignal = CreateSemaphore(NULL, 0, 4, NULL);
		ghSetbuffSignal = CreateSemaphore(NULL, 2, 2, NULL);
		return TRUE;
	}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case IDC_BUTTON_START:
		{
			EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_START),FALSE);
			GetDlgItemText(hwnd, IDC_EDIT_STR, ptStr, sizeof ptStr);
			ptStrPoint = ptStr;
			if (StrLen(ptStr))
			{
				CloseHandle(CreateThread(NULL, 0, threadCtrl, NULL, 0, NULL));
			}
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
	InitializeCriticalSection(&cs);
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, winProcOfMain);
}