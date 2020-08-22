#include "resource.h"
#include <windows.h>
#include <stdio.h>

#define ThreadNum 3

HWND hwndOfMain;						//�����ھ��
HANDLE ghMutex = NULL;					//������
HANDLE hThreads[ThreadNum] = { 0 };		//�̼߳���
int dwInitNum = 1000;					//�ܴ�С��ʼֵ
DWORD dwValues[ThreadNum] = { 0 };		//�����߳̿��Ƶ�ֵ
DWORD dwDlgItem[ThreadNum] = { IDC_EDIT_NUM1, IDC_EDIT_NUM2, IDC_EDIT_NUM3 };	//�༭��ID����
TCHAR ptBuff[128] = { 0 };				//�ı�������
DWORD dwIndex[ThreadNum] = { 0,1,2 };	//�߳�����,�̻߳ص�����������ֵ

//��ʼ�߳�
DWORD WINAPI threadProcOfStart(LPVOID lParameter) {

	while (1)		//�����Ƿ���������
	{

		WaitForSingleObject(ghMutex, -1);	//�жϻ�����
		if (dwInitNum < 50)
		{
			break;
		}

		dwInitNum -= 50;		//������50

		dwValues[*(DWORD*)lParameter] += 50;	//�߳�ֵ������50

		//�༭������ֵ
		wsprintf(ptBuff, TEXT("%d"), dwValues[*(DWORD*)lParameter]);
		SetDlgItemText(hwndOfMain, dwDlgItem[*(DWORD*)lParameter], ptBuff);


		ReleaseMutex(ghMutex);	//�ͷŻ�����
		Sleep(50);
	}
	return 0;
}

//��ʼ�߳̿���
DWORD WINAPI threadProcOfStartCtrl(LPVOID lParameter) {
	DWORD i = 0;
	while (i < ThreadNum)		//�����߳�
	{
		hThreads[i] = ::CreateThread(NULL, 0, threadProcOfStart, &dwIndex[i], 0, NULL);
		i++;
	}

	WaitForMultipleObjects(ThreadNum, hThreads, TRUE, -1);	//�ȴ�ȫ���߳̽���
	i = 0;
	while (i < ThreadNum)		//�ͷ��߳̾��
	{
		::CloseHandle(hThreads[i]);
		i++;
	}
	//::CloseHandle(ghMutex);		//�ͷŻ�����
	return 0;
}

//��Ϣ�ص�
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
			//ֵ���
			memset(dwValues, 0, sizeof dwValues);		
			//�༭�����
			SetDlgItemText(hwnd, IDC_EDIT_NUM1, TEXT(""));
			SetDlgItemText(hwnd, IDC_EDIT_NUM2, TEXT(""));
			SetDlgItemText(hwnd, IDC_EDIT_NUM3, TEXT(""));

			//�ӱ༭���ȡ��ֵ
			GetDlgItemText(hwnd, IDC_EDIT_COUNT, ptBuff, sizeof ptBuff);
			swscanf_s(ptBuff, TEXT("%d"), &dwInitNum);

			//���������߳�
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
	ghMutex = CreateMutex(NULL, FALSE, NULL);		//����������
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, winProcOfWinMain);
}