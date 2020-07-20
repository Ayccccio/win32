#include <windows.h>
#include "tools.h"



LRESULT CALLBACK  WindowProc(IN HWND hwnd,
	IN UINT uMsg,
	IN WPARAM wParam,
	IN LPARAM lParam) 
{
	switch (uMsg) {
	case WM_CREATE: {
			DbgPrintf("WM_CREATE %d %d\n", wParam, lParam);
			CREATESTRUCT* createTest = (CREATESTRUCT*)lParam;
			DbgPrintf("CREATESTRUCT %s\n", createTest->lpszClass);
			return 0;
		}
	case WM_DESTROY: {
			DbgPrintf("WM_DESTROY %d %d\n", wParam, lParam);
			PostQuitMessage(0);
			return 0;
		}

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


int CALLBACK WinMain1(_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) 
{
	TCHAR className[] = TEXT("My First Window");

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
		className,					//���ڱ���
		WS_OVERLAPPEDWINDOW,		//���������ʽ
		10,							//����ڸ����ڵ�X����
		10,							//����ڸ����ڵ�Y����
		600,						//���ڵĿ��
		300,						//���ڵĸ߶�
		NULL,						//�����ھ��,ΪNULL
		NULL,						//�˵����,ΪNULL
		hInstance,					//��ǰӦ�ó���ľ��
		NULL);						//��������һ��ΪNULL

	if (hwnd == NULL)				//�ж��Ƿ񴴽��ɹ�
	{
		return 0;
	}

	//��ʾ����
	ShowWindow(hwnd, SW_SHOW);

	//��Ϣѭ��
	MSG msg;
	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}