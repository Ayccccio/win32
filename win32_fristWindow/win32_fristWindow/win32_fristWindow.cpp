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

	//创建窗口类的对象
	WNDCLASS wndClass = { 0 };
	wndClass.hbrBackground = (HBRUSH)COLOR_MENU;
	wndClass.lpfnWndProc = WindowProc;
	wndClass.lpszClassName = className;
	wndClass.hInstance = hInstance;

	//注册窗口类
	RegisterClass(&wndClass);

	//创建窗口
	HWND hwnd = CreateWindow(
		className,					//类名
		className,					//窗口标题
		WS_OVERLAPPEDWINDOW,		//窗口外观样式
		10,							//相对于父窗口的X坐标
		10,							//相对于父窗口的Y坐标
		600,						//窗口的宽度
		300,						//窗口的高度
		NULL,						//父窗口句柄,为NULL
		NULL,						//菜单句柄,为NULL
		hInstance,					//当前应用程序的句柄
		NULL);						//附加数据一般为NULL

	if (hwnd == NULL)				//判断是否创建成功
	{
		return 0;
	}

	//显示窗口
	ShowWindow(hwnd, SW_SHOW);

	//消息循环
	MSG msg;
	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}