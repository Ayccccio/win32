#include "resource.h"
#include <windows.h>
#include <stdio.h>

#define ThreadNum 3

HANDLE ghEvent;			//�¼�����
DWORD dwNum = 0;		//����ֵ
HWND hMain;				//�����ھ��
BYTE pbIndex[ThreadNum] = { 0,1,2 };	//�߳�����,�����̻߳ص���������,���Ʊ༭����
DWORD pdDlgItemId[ThreadNum] = { IDC_EDIT_NUM1, IDC_EDIT_NUM2,IDC_EDIT_NUM3 };	//�༭��ID,
HANDLE pThreadHandles[ThreadNum] = { 0 };	//�߳̾�����

//�߳̿�ʼ�ص�����
DWORD WINAPI threadProcOfStart(LPVOID lParameter) {
	TCHAR ptBuff[128] = { 0 };

	//�����ں˶���֪ͨ״̬
	WaitForSingleObject(ghEvent, -1);

	//��ȡ�����༭���ֵ,�����Ӧ�༭����
	wsprintf(ptBuff, TEXT("%d"), dwNum);
	SetDlgItemText(hMain, pdDlgItemId[*(PBYTE)lParameter], ptBuff);

	Sleep(200);
	//���ں˶���֪ͨ״̬��Ϊ��֪ͨ
	SetEvent(ghEvent);
	return 0;
}

//�߳̿��ƻص�����
DWORD WINAPI threadProcOfStartCtrl(LPVOID lParameter) {
	TCHAR ptBuff[128] = { 0 };
	int i = 0;
	//�����ں��¼�����,Ĭ��δ֪ͨ״̬,wait��Ĭ�ϸ�δ֪ͨ״̬���ֶ���״̬
	

	//��ʼ���߳̾������
	memset(pThreadHandles, 0, sizeof pThreadHandles);

	//ѭ�������߳�
	while (i < ThreadNum)
	{
		pThreadHandles[i] = CreateThread(NULL, 0, threadProcOfStart, &pbIndex[i], 0, NULL);
		i++;
	}

	//�༭��1ѭ����
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

	//�ȴ��߳�ȫ��ִ�����
	WaitForMultipleObjects(ThreadNum, pThreadHandles, TRUE, -1);
	i = 0;
	while (i < ThreadNum)	//�ͷ��߳̾��
	{
		CloseHandle(pThreadHandles[i]);
		i++;
	}

	//�����¼�����
	ResetEvent(ghEvent);
	EnableWindow(GetDlgItem(hMain, IDC_BUTTON_START), TRUE);	//���ð�ť
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
		CloseHandle(ghEvent);		//�ͷ��¼�����
		return TRUE;
	}
	case WM_INITDIALOG:
	{
		ghEvent = CreateEvent(NULL, FALSE, FALSE, NULL);		//�����¼�����
		hMain = hwnd;
		return TRUE;
	}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case IDC_BUTTON_START:
		{
			//�����ֵ�༭�������
			while (i < ThreadNum)
			{
				SetDlgItemText(hwnd, pdDlgItemId[i], TEXT(""));
				i++;
			}

			//���ÿ�ʼ��ť
			EnableWindow(GetDlgItem(hwnd, IDC_BUTTON_START),FALSE);

			CloseHandle(CreateThread(NULL, 0, threadProcOfStartCtrl, NULL, 0, NULL));		//�����߳̿����߳�
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