#include "resource.h"
#include "tools.h"
#include <windows.h>

TCHAR ptStr[256] = { 0 };		//�ַ���
PTCHAR ptStrPoint = NULL;		//�ַ���ָ��
TCHAR ptBuff[4] = { 0 };		//����2���������ַ�
TCHAR ptTexts[4][128] = { {0},{0}, {0}, {0} };		//�̳߳Ե����ַ�
CRITICAL_SECTION cs = { 0 };	//�ٽ���
HANDLE ghSignal;				//�ź���

HWND hMain;
WORD pwDlgItemIdsOfBuff[2] = { IDC_EDIT_BUFF1 ,IDC_EDIT_BUFF2};

//ȡ��ĸ���������߳�
DWORD WINAPI threadOfBuff(LPVOID lParameter) {
	DWORD dwStrLen = 0;
	while (1)
	{

		//�����ٽ���
		EnterCriticalSection(&cs);

		//��ȡ�ַ�����,�ж��Ƿ����ַ�
		dwStrLen = StrLen(ptStrPoint);
		if (dwStrLen == 0)
			break;

		//ȡ�׸��ַ���������
		ptBuff[(DWORD)lParameter * 2] = *ptStrPoint;

		//�����ַ����������༭��
		SetDlgItemText(hMain, pwDlgItemIdsOfBuff[(DWORD)lParameter], ptBuff + (DWORD)lParameter * 2);

		//
		ptStrPoint++;
		ReleaseSemaphore(ghSignal, 1, NULL);	//�ź�����1,����һ���߳�

		//�ͷ��ٽ���
		LeaveCriticalSection(&cs);
	}

}

//ȡ�������ַ��߳�
DWORD WINAPI threadOfGetBuff(LPVOID lParameter) {
	while (1)
	{
		//�����ź���
		WaitForSingleObject(ghSignal, -1);

		//�����ٽ���
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


//�߳̿��ƺ���
DWORD WINAPI threadCtrl(LPVOID lParameter) 
{

}


//��������Ϣ�ص�����
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