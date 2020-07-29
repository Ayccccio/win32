#include <windows.h>
#include "tools.h"


//�¼����̺���
LRESULT CALLBACK  WindowProc(IN HWND hwnd,		//��Ϣ��Ӧ�Ĵ��ھ��
	IN UINT uMsg,		//��Ϣ����
	IN WPARAM wParam,	//��Ϣ���͸�����Ϣ
	IN LPARAM lParam)	//��Ϣ���͸�����Ϣ
{
	switch (uMsg) {
	case WM_DESTROY: {
			DbgPrintf("WM_DESTROY %d %d\n", wParam, lParam);
			PostQuitMessage(0);
			return 0;
		}

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);		//������Ϣ����window����
}


//win32���������
int CALLBACK WinMain(_In_ HINSTANCE hInstance,		//����ʵ�����,ImageBase
	_In_ HINSTANCE hPrevInstance,					//��һ�����ڵ�ʵ�����,��:����ͬһ����ͬ�ĳ���,��һ��ֵ����NULL,�ڶ����ͻ��ǵ�һ����hInstance
	_In_ LPSTR lpCmdLine,							//cmd�����в���
	_In_ int nCmdShow)								//cmd��������ʾ״̬
{
	TCHAR className[] = TEXT("My First Window");	//���ô�������

	//����������Ķ���
	WNDCLASS wndClass = { 0 };
	wndClass.hbrBackground = (HBRUSH)COLOR_MENU;
	wndClass.lpfnWndProc = WindowProc;
	wndClass.lpszClassName = className;
	wndClass.hInstance = hInstance;


	//ע�ᴰ����
	RegisterClass(&wndClass);

	//��������
	HWND hwnd = CreateWindow(
		className,					//����
		TEXT("my frist window"),	//���ڱ���
		WS_OVERLAPPEDWINDOW,		//���������ʽ
		10,							//����ڸ����ڵ�X����
		10,							//����ڸ����ڵ�Y����
		600,						//���ڵĿ��
		300,						//���ڵĸ߶�
		NULL,						//�����ھ��,ΪNULL
		NULL,						//�˵����,ΪNULL
		hInstance,					//��ǰӦ�ó���ľ��
		NULL);						//��������һ��ΪNULL


	DWORD errorCode = GetLastError();	//��ȡ�������
	DbgPrintf("%d", errorCode);			//��ӡ�������


	if (hwnd == NULL)				//�ж��Ƿ񴴽��ɹ�
	{
		return 0;
	}

	//��ʾ����
	ShowWindow(hwnd, SW_SHOW);

	//��Ϣѭ��
	MSG msg;
	while (GetMessage(&msg,NULL,0,0))	//��ȡMSG
	{
		TranslateMessage(&msg);		//ת��MSG
		DispatchMessage(&msg);		//ת�ظ�windows������Ϣ������
	}

	return 0;
}