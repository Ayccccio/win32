#include "resource.h"
#include "tools.h"

#define ProducterThreadNum 2
#define CustomerThreadNum 4


TCHAR ptStr[256] = { 0 };		//字符串
PTCHAR ptStrPoint = NULL;		//字符串指针

TCHAR ptProducter[ProducterThreadNum][2] = { 0 };			//生产者字符
TCHAR ptCustomer[CustomerThreadNum][128] = { 0 };		//消费者字符
CRITICAL_SECTION cs = { 0 };			//临界区
HANDLE ghSignalOfProducter;				//生产者信号量
HANDLE ghSignalOfCustomer;				//消费者信号量

HWND hEditOfStr;	//字符串编辑框句柄
HWND hBtnOfStart;	//开始按钮句柄
HWND hEditOfProducter[ProducterThreadNum] = { 0 };		//生产者编辑框窗口句柄
HWND hEditOfCustomer[CustomerThreadNum] = { 0 };		//消费者编辑框窗口句柄
HANDLE hProducterThreads[ProducterThreadNum] = { 0 };	//生产者线程数组
HANDLE hCustomerThreads[CustomerThreadNum] = { 0 };		//获消费者线程数组

BOOL bCustomerThreadQuit = FALSE;

//生产者线程
DWORD WINAPI threadProOfProducter(LPVOID lParameter) {
	//字符串取完线程结束
	while (1)
	{
		//从字符串取字符设置到生产者编辑框和缓冲区

		WaitForSingleObject(ghSignalOfProducter, -1);

		EnterCriticalSection(&cs);
		//判断字符串是否取完,线程结束条件
		if (*ptStrPoint == '\0')
		{
			bCustomerThreadQuit = TRUE;
			LeaveCriticalSection(&cs);

			//避免消费者线程获取不到信号量卡死
			ReleaseSemaphore(ghSignalOfCustomer, 4, NULL);
			break;
		}
		//判断当前生产者字符中是否有字符
		if (ptProducter[(DWORD)lParameter][0] == '\0')
		{
			//取字符
			ptProducter[(DWORD)lParameter][0] = *ptStrPoint;

			//取到了字符,字符串右移
			ptStrPoint++;

			//设置字符到编辑框
			SetWindowText(hEditOfProducter[(DWORD)lParameter], ptProducter[(DWORD)lParameter]);

			ReleaseSemaphore(ghSignalOfCustomer, 1, NULL);
		}
		LeaveCriticalSection(&cs);
	}
	return 0;
}


//消费者线程
DWORD WINAPI threadProOfCustomer(LPVOID lParameter) {
	//取生产者字符,放自身字符里.生产者字符置0

	DWORD dwI = 0;
	while (1)
	{
		/*if (WaitForSingleObject(ghSignalOfCustomer, 2000) == WAIT_TIMEOUT)
		{
			break;
		}*/
		WaitForSingleObject(ghSignalOfCustomer, -1);
		
		EnterCriticalSection(&cs);

		Sleep(50);
		//遍历生产者字符
		while (dwI < ProducterThreadNum)
		{
			if (ptProducter[dwI][0] != '\0')
			{
				//取字符
				StrCat(ptCustomer[(DWORD)lParameter], ptProducter[dwI]);

				//设置字符到编辑框
				SetWindowText(hEditOfCustomer[(DWORD)lParameter], ptCustomer[(DWORD)lParameter]);

				//生产者字符置0
				ptProducter[dwI][0] = '\0';
				//生产者编辑框置空
				SetWindowText(hEditOfProducter[dwI], TEXT(""));
				break;
			}
			dwI++;
		}

		//生产者信号量加1,继续生产
		ReleaseSemaphore(ghSignalOfProducter, 1, NULL);
		LeaveCriticalSection(&cs);

		//判断字符串是否取完,取完生产者线程信号量继续加1避免生产者线程获取不到信号卡死
		if (*ptStrPoint == '\0')
		{
			ReleaseSemaphore(ghSignalOfProducter, 1, NULL);
			break;
		}


		/*if (bCustomerThreadQuit)
		{
		}*/
	}
	return 0;
}


//主控制线程
DWORD WINAPI threadProOfControl(LPVOID lParameter) {
	DWORD dwThreadNum = 0;

	ghSignalOfProducter = CreateSemaphore(NULL, 2, 2, NULL);
	ghSignalOfCustomer = CreateSemaphore(NULL, 0, 4, NULL);

	InitializeCriticalSection(&cs);


	//创建生产者线程
	while (dwThreadNum < ProducterThreadNum)
	{
		hProducterThreads[dwThreadNum] = CreateThread(NULL, 0, threadProOfProducter, (LPVOID)dwThreadNum, 0, NULL);
		dwThreadNum++;
	}

	//创建消费者线程
	dwThreadNum = 0;
	while (dwThreadNum < CustomerThreadNum)
	{
		hCustomerThreads[dwThreadNum] = CreateThread(NULL, 0, threadProOfCustomer, (LPVOID)dwThreadNum, 0, NULL);
		dwThreadNum++;
	}

	//释放消费者线程句柄
	WaitForMultipleObjects(CustomerThreadNum, hCustomerThreads, TRUE, -1);
	dwThreadNum = 0;
	while (dwThreadNum < CustomerThreadNum)
	{
		CloseHandle(hCustomerThreads[dwThreadNum]);
		dwThreadNum++;
	}

	//生产者信号加2,防止得不到型号量线程不结束
	//ReleaseSemaphore(ghSignalOfProducter, 2, NULL);

	//判断线程是否结束
	WaitForMultipleObjects(ProducterThreadNum, hProducterThreads, TRUE, -1);
	dwThreadNum = 0;
	while (dwThreadNum < ProducterThreadNum)
	{
		CloseHandle(hProducterThreads[dwThreadNum]);
		dwThreadNum++;
	}

	CloseHandle(ghSignalOfCustomer);
	CloseHandle(ghSignalOfProducter);
	DeleteCriticalSection(&cs);

	EnableWindow(hBtnOfStart, TRUE);
	return 0;
}


//主窗口消息回调函数
INT_PTR CALLBACK winProcOfMain(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam) 
{
	DWORD dwI = 0;
	switch (uMsg)
	{
	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
		return TRUE;
	}
	case WM_INITDIALOG:
	{

		hEditOfProducter[0] = GetDlgItem(hwnd, IDC_EDIT_BUFF1);
		hEditOfProducter[1] = GetDlgItem(hwnd, IDC_EDIT_BUFF2);

		hEditOfCustomer[0] = GetDlgItem(hwnd, IDC_EDIT_THREAD1);
		hEditOfCustomer[1] = GetDlgItem(hwnd, IDC_EDIT_THREAD2);
		hEditOfCustomer[2] = GetDlgItem(hwnd, IDC_EDIT_THREAD3);
		hEditOfCustomer[3] = GetDlgItem(hwnd, IDC_EDIT_THREAD4);

		hEditOfStr = GetDlgItem(hwnd, IDC_EDIT_STR);
		hBtnOfStart = GetDlgItem(hwnd, IDC_BUTTON1);

		SetDlgItemText(hwnd, IDC_EDIT_STR, TEXT("ABCDEF"));
		return TRUE;
	}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case IDC_BUTTON_START:
		{
			EnableWindow(hBtnOfStart,FALSE);

			//获取字符串文本框内容到ptStr
			GetDlgItemText(hwnd, IDC_EDIT_STR, ptStr, sizeof ptStr);
			//字符串指针
			ptStrPoint = ptStr;

			if (StrLen(ptStr))
			{
				CloseHandle(CreateThread(NULL, 0, threadProOfControl, NULL, 0, NULL));
			}
			return TRUE;
		}
		case IDC_BUTTON_RESET: 
		{
			while (dwI < CustomerThreadNum)
			{
				SetWindowText(hEditOfCustomer[dwI], TEXT(""));
				dwI++;
			}
			memset(ptCustomer, 0, sizeof ptCustomer);
			memset(ptProducter, 0, sizeof ptProducter);
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
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, winProcOfMain);
}