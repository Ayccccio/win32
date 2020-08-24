#include "resource.h"
#include "tools.h"

#define ProducterThreadNum 2
#define CustomerThreadNum 4


TCHAR ptStr[256] = { 0 };		//�ַ���
PTCHAR ptStrPoint = NULL;		//�ַ���ָ��

TCHAR ptProducter[ProducterThreadNum][2] = { 0 };			//�������ַ�
TCHAR ptCustomer[CustomerThreadNum][128] = { 0 };		//�������ַ�
CRITICAL_SECTION cs = { 0 };			//�ٽ���
HANDLE ghSignalOfProducter;				//�������ź���
HANDLE ghSignalOfCustomer;				//�������ź���

HWND hEditOfStr;	//�ַ����༭����
HWND hBtnOfStart;	//��ʼ��ť���
HWND hEditOfProducter[ProducterThreadNum] = { 0 };		//�����߱༭�򴰿ھ��
HWND hEditOfCustomer[CustomerThreadNum] = { 0 };		//�����߱༭�򴰿ھ��
HANDLE hProducterThreads[ProducterThreadNum] = { 0 };	//�������߳�����
HANDLE hCustomerThreads[CustomerThreadNum] = { 0 };		//���������߳�����

BOOL bCustomerThreadQuit = FALSE;

//�������߳�
DWORD WINAPI threadProOfProducter(LPVOID lParameter) {
	//�ַ���ȡ���߳̽���
	while (1)
	{
		//���ַ���ȡ�ַ����õ������߱༭��ͻ�����

		WaitForSingleObject(ghSignalOfProducter, -1);

		EnterCriticalSection(&cs);
		//�ж��ַ����Ƿ�ȡ��,�߳̽�������
		if (*ptStrPoint == '\0')
		{
			bCustomerThreadQuit = TRUE;
			LeaveCriticalSection(&cs);

			//�����������̻߳�ȡ�����ź�������
			ReleaseSemaphore(ghSignalOfCustomer, 4, NULL);
			break;
		}
		//�жϵ�ǰ�������ַ����Ƿ����ַ�
		if (ptProducter[(DWORD)lParameter][0] == '\0')
		{
			//ȡ�ַ�
			ptProducter[(DWORD)lParameter][0] = *ptStrPoint;

			//ȡ�����ַ�,�ַ�������
			ptStrPoint++;

			//�����ַ����༭��
			SetWindowText(hEditOfProducter[(DWORD)lParameter], ptProducter[(DWORD)lParameter]);

			ReleaseSemaphore(ghSignalOfCustomer, 1, NULL);
		}
		LeaveCriticalSection(&cs);
	}
	return 0;
}


//�������߳�
DWORD WINAPI threadProOfCustomer(LPVOID lParameter) {
	//ȡ�������ַ�,�������ַ���.�������ַ���0

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
		//�����������ַ�
		while (dwI < ProducterThreadNum)
		{
			if (ptProducter[dwI][0] != '\0')
			{
				//ȡ�ַ�
				StrCat(ptCustomer[(DWORD)lParameter], ptProducter[dwI]);

				//�����ַ����༭��
				SetWindowText(hEditOfCustomer[(DWORD)lParameter], ptCustomer[(DWORD)lParameter]);

				//�������ַ���0
				ptProducter[dwI][0] = '\0';
				//�����߱༭���ÿ�
				SetWindowText(hEditOfProducter[dwI], TEXT(""));
				break;
			}
			dwI++;
		}

		//�������ź�����1,��������
		ReleaseSemaphore(ghSignalOfProducter, 1, NULL);
		LeaveCriticalSection(&cs);

		//�ж��ַ����Ƿ�ȡ��,ȡ���������߳��ź���������1�����������̻߳�ȡ�����źſ���
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


//�������߳�
DWORD WINAPI threadProOfControl(LPVOID lParameter) {
	DWORD dwThreadNum = 0;

	ghSignalOfProducter = CreateSemaphore(NULL, 2, 2, NULL);
	ghSignalOfCustomer = CreateSemaphore(NULL, 0, 4, NULL);

	InitializeCriticalSection(&cs);


	//�����������߳�
	while (dwThreadNum < ProducterThreadNum)
	{
		hProducterThreads[dwThreadNum] = CreateThread(NULL, 0, threadProOfProducter, (LPVOID)dwThreadNum, 0, NULL);
		dwThreadNum++;
	}

	//�����������߳�
	dwThreadNum = 0;
	while (dwThreadNum < CustomerThreadNum)
	{
		hCustomerThreads[dwThreadNum] = CreateThread(NULL, 0, threadProOfCustomer, (LPVOID)dwThreadNum, 0, NULL);
		dwThreadNum++;
	}

	//�ͷ��������߳̾��
	WaitForMultipleObjects(CustomerThreadNum, hCustomerThreads, TRUE, -1);
	dwThreadNum = 0;
	while (dwThreadNum < CustomerThreadNum)
	{
		CloseHandle(hCustomerThreads[dwThreadNum]);
		dwThreadNum++;
	}

	//�������źż�2,��ֹ�ò����ͺ����̲߳�����
	//ReleaseSemaphore(ghSignalOfProducter, 2, NULL);

	//�ж��߳��Ƿ����
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


//��������Ϣ�ص�����
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

			//��ȡ�ַ����ı������ݵ�ptStr
			GetDlgItemText(hwnd, IDC_EDIT_STR, ptStr, sizeof ptStr);
			//�ַ���ָ��
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