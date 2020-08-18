#include <windows.h>
#include <stdio.h>
#include "resource.h"
#include "tools.h"

DWORD dwNum = 0;			//ȫ�ֱ���,�̺߳�������
HWND hDialog = 0;			//�����ھ��
HWND hStartBtn1 = NULL;		//��ʼ1��ť���
HWND hStartBtn2 = NULL;		//��ʼ2��ť���
BOOL bEndThread1 = 0;		//�߳�1����
BOOL bEndThread2 = 0;		//�߳�2����
CRITICAL_SECTION cs;		//�ٽ���

HANDLE hThread1 = NULL;
HANDLE hThread2 = NULL;

DWORD WINAPI threadProc1(LPVOID lParameter) {
	TCHAR ptText[10] = { 0 };
	int i = 0;
	while (i < 200)
	{
		if (*(PBOOL)lParameter)
		{
			break;
		}
		EnterCriticalSection(&cs);
		dwNum++;
		wcsprintf(ptText, TEXT("%d"), dwNum);
		LeaveCriticalSection(&cs);

		SetDlgItemText(hDialog, IDC_EDIT_NUM, ptText);
		Sleep(20);
		i++;
	}
	return 0;
}

INT_PTR CALLBACK winProcOfThread(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam) 
{
	DWORD dwExitCode = 0;
	switch (uMsg)
	{
	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
		return TRUE;
	}
	case WM_INITDIALOG:
	{
		hDialog = hwnd;
		hStartBtn1 = GetDlgItem(hwnd, IDC_BUTTON_START1);
		hStartBtn2 = GetDlgItem(hwnd, IDC_BUTTON_START2);
		return TRUE;
	}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case IDC_BUTTON_START1:
		{
			bEndThread1 = 0;
			GetExitCodeThread(hThread1, &dwExitCode);
			hThread1 = CreateThread(NULL, 0, threadProc1, &bEndThread1, 0, NULL);
			EnableWindow(hStartBtn1, FALSE);
			return TRUE;
		}
		case IDC_BUTTON_START2:
		{
			bEndThread2 = 0;
			hThread2 = CreateThread(NULL, 0, threadProc1, &bEndThread2, 0, NULL);
			EnableWindow(hStartBtn2, FALSE);
			return TRUE;
		}
		case IDC_BUTTON_PAUSE:
		{
			SuspendThread(hThread1);
			SuspendThread(hThread2);
			return TRUE;
		}
		case IDC_BUTTON_REGAIN:
		{
			ResumeThread(hThread1);
			ResumeThread(hThread2);
			return TRUE;
		}
		case IDC_BUTTON_QUIT:
		{
			/*TerminateThread(hThread1,1);
			WaitForSingleObject(hThread1, INFINITE);

			TerminateThread(hThread2, 1);
			WaitForSingleObject(hThread2, INFINITE);*/

			bEndThread1 = 1;
			bEndThread2 = 1;
			EnableWindow(hStartBtn1, TRUE);
			EnableWindow(hStartBtn2, TRUE);
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
	//��ʼ���ٽ���
	InitializeCriticalSection(&cs);
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_THREAD), NULL, winProcOfThread);
}

