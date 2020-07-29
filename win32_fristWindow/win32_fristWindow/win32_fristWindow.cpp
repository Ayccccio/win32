#include <windows.h>
#include "tools.h"


//事件过程函数
LRESULT CALLBACK  WindowProc(IN HWND hwnd,		//消息对应的窗口句柄
	IN UINT uMsg,		//消息类型
	IN WPARAM wParam,	//消息类型附加信息
	IN LPARAM lParam)	//消息类型附加信息
{
	switch (uMsg) {
	case WM_DESTROY: {
			DbgPrintf("WM_DESTROY %d %d\n", wParam, lParam);
			PostQuitMessage(0);
			return 0;
		}

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);		//其他消息丢给window处理
}


//win32主程序入口
int CALLBACK WinMain(_In_ HINSTANCE hInstance,		//窗口实例句柄,ImageBase
	_In_ HINSTANCE hPrevInstance,					//上一个窗口的实例句柄,例:运行同一个相同的程序,第一个值就是NULL,第二个就会是第一个的hInstance
	_In_ LPSTR lpCmdLine,							//cmd命令行参数
	_In_ int nCmdShow)								//cmd命令行显示状态
{
	TCHAR className[] = TEXT("My First Window");	//设置窗口类名

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
		TEXT("my frist window"),	//窗口标题
		WS_OVERLAPPEDWINDOW,		//窗口外观样式
		10,							//相对于父窗口的X坐标
		10,							//相对于父窗口的Y坐标
		600,						//窗口的宽度
		300,						//窗口的高度
		NULL,						//父窗口句柄,为NULL
		NULL,						//菜单句柄,为NULL
		hInstance,					//当前应用程序的句柄
		NULL);						//附加数据一般为NULL


	DWORD errorCode = GetLastError();	//获取错误代码
	DbgPrintf("%d", errorCode);			//打印错误代码


	if (hwnd == NULL)				//判断是否创建成功
	{
		return 0;
	}

	//显示窗口
	ShowWindow(hwnd, SW_SHOW);

	//消息循环
	MSG msg;
	while (GetMessage(&msg,NULL,0,0))	//获取MSG
	{
		TranslateMessage(&msg);		//转换MSG
		DispatchMessage(&msg);		//转回给windows调用消息处理函数
	}

	return 0;
}