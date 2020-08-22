#include "resource.h"
#include "tools.h"

#define SetBuffThreadNum 2
#define GetBuffThreadNum 4


TCHAR ptStr[256] = { 0 };		//�ַ���
PTCHAR ptStrPoint = NULL;		//�ַ���ָ��
TCHAR ptBuff[SetBuffThreadNum][2] = { 0 };		//����2���������ַ�
TCHAR ptTexts[GetBuffThreadNum][128] = { 0 };		//�̳߳Ե����ַ�
CRITICAL_SECTION cs = { 0 };	//�ٽ���
HANDLE ghSignal;				//ȡ�������ַ��ź���
HANDLE ghSetbuffSignal;				//���û������ַ��ź���

HWND hMain;			//�����ھ��
WORD pwDlgItemIdsOfSetBuff[SetBuffThreadNum] = { IDC_EDIT_BUFF1 ,IDC_EDIT_BUFF2};	//�������༭�򴰿�id
WORD pwDlgItemIdsOfGetBuff[GetBuffThreadNum] = { IDC_EDIT_THREAD1 ,IDC_EDIT_THREAD2,IDC_EDIT_THREAD3,IDC_EDIT_THREAD4 };
HANDLE hSetBuffThreads[SetBuffThreadNum] = { 0 };		//���û������߳�����
HANDLE hGetBuffThreads[GetBuffThreadNum] = { 0 };		//��ȡ�������ַ��߳�����

//���û������ַ����༭���߳�
DWORD WINAPI threadOfSetBuff(LPVOID lParameter) {
	DWORD dwStrLen;
	while (1)
	{

		WaitForSingleObject(ghSetbuffSignal, -1);

		//�����ٽ���
		EnterCriticalSection(&cs);

		//��ȡ�ַ�����,�ж��Ƿ����ַ�
		dwStrLen = StrLen(ptStrPoint);
 		if (dwStrLen == 0)
			break;

		//ȡ�׸��ַ���������
		ptBuff[(DWORD)lParameter][0] = *ptStrPoint;
		ptStrPoint++;

		//�����ַ����������༭��
		SetDlgItemText(hMain, pwDlgItemIdsOfSetBuff[(DWORD)lParameter], ptBuff[(DWORD)lParameter]);

		Sleep(200);


		//�ź�����1,����һ���߳�����,�ź�����1���ͷ��ٽ���ǰ����Ϊ�˱���׼���׵���,��ȡ�������ַ��߳�ȴ����ִ��
		ReleaseSemaphore(ghSignal, 1, NULL);	
		
		//�ͷ��ٽ���
		LeaveCriticalSection(&cs);

	}
	return 0;
}

//ȡ�������ַ��߳�
DWORD WINAPI threadOfGetBuff(LPVOID lParameter) {
	while (1)
	{
		//�����ź���
		WaitForSingleObject(ghSignal, -1);

		//�����ٽ���
		EnterCriticalSection(&cs);

		//ȡ�������ַ�,�ŵ�����������
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

		//���û������ź�����1,�û��������Եõ����
		ReleaseSemaphore(ghSetbuffSignal, 1, NULL);

		//�ͷ��ٽ���
		LeaveCriticalSection(&cs);
	}
	return 0;
}


//�߳̿��ƺ���
DWORD WINAPI threadCtrl(LPVOID lParameter) 
{
	DWORD dwGetBuffThreadNum = 0;
	DWORD dwSetBuffThreadNum = 0;

	//�������������ַ��߳�
	while (dwGetBuffThreadNum < GetBuffThreadNum)
	{
		hGetBuffThreads[dwGetBuffThreadNum] = CreateThread(NULL, 0, threadOfGetBuff, (LPVOID)(dwGetBuffThreadNum), 0, NULL);
		dwGetBuffThreadNum++;
	}

	//���������ַ��������߳�
	while (dwSetBuffThreadNum < SetBuffThreadNum)
	{
		hSetBuffThreads[dwSetBuffThreadNum] = CreateThread(NULL, 0, threadOfSetBuff, (LPVOID)(dwSetBuffThreadNum), 0, NULL);
		dwSetBuffThreadNum++;
	}

	
	//�������û������߳�
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