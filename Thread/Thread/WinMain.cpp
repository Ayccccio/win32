#include <windows.h>
#include <stdio.h>
#include "resource.h"
#include "tools.h"

DWORD dwNum = 0;
HWND hDialog = 0;
BOOL bQuit = FALSE;

DWORD WINAPI threadProc1(LPVOID lParameter) {
	TCHAR ptText[10] = { 0 };
	int i = 0;
	while (i <= 2000)
	{
		if (bQuit)
		{
			break;
		}
		wcsprintf(ptText, TEXT("%d"), *(DWORD*)lParameter);
		SetDlgItemText(hDialog, IDC_EDIT_NUM, ptText);
		Sleep(20);
		*(DWORD*)lParameter = *(DWORD*)lParameter + 1;
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
	HANDLE hThread1 = NULL;
	HANDLE hThread2 = NULL;
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
		return TRUE;
	}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case IDC_BUTTON_START1:
		{
			hThread1 = CreateThread(NULL, 0, threadProc1, &dwNum, 0, NULL);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_START1),)
			return TRUE;
		}
		case IDC_BUTTON_START2:
		{
			hThread2 = CreateThread(NULL, 0, threadProc1, &dwNum, 0, NULL);
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
			TerminateThread(hThread1,1);
			WaitForSingleObject(hThread1, INFINITE);

			TerminateThread(hThread2, 1);
			WaitForSingleObject(hThread2, INFINITE);
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
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_THREAD), NULL, winProcOfThread);
}

