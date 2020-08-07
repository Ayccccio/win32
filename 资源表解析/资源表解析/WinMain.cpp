#include "const.h"
#include "resource.h"
#include "tools.h"

INT_PTR CALLBACK WindowProc(HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam) {

	HICON hIcon;			//图标文件句柄

	OPENFILENAME ofn = { 0 };				//文件选择对话框结构
	TCHAR ptFileNameBuff[MAX_PATH] = { 0 };	//文件路径缓冲区

	DWORD dwReadFileSize;		//PE文件加载大小
	

	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		hIcon = LoadIcon(hAPPInterface, MAKEINTRESOURCE(IDI_ICON_LOAD));
		SendMessage((HWND)hAPPInterface, WM_SETICON, ICON_BIG, (DWORD)hIcon);
		SendMessage((HWND)hAPPInterface, WM_SETICON, ICON_SMALL, (DWORD)hIcon);
		return TRUE;
	}
	case WM_CLOSE:
	{
		//EndDialog(hwnd, 0);
		PostQuitMessage(0);
		return TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		
		case IDC_BUTTON_OPEN:
		{
			//初始化OPENFILENAME结构
			ofn.lStructSize = sizeof OPENFILENAME;		//结构大小
			ofn.hwndOwner = NULL;						//所属窗口的句柄
			ofn.lpstrFilter = TEXT("PE文件\0*.exe");	//文件选择过滤器
			ofn.nFilterIndex = 1;						      //默认过滤器索引
			ofn.lpstrFile = ptFileNameBuff;				//文件路径存放缓冲区
			ofn.nMaxFile = sizeof ptFileNameBuff;		//文件路径存放缓冲区最大大小
			ofn.lpstrInitialDir = NULL;					//文件选择对话框默认打开路径
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;	//文件,路径必须存在,隐藏只读文件

			GetOpenFileName(&ofn);	//打开文件对话框
			if (*ptFileNameBuff != TEXT('\0'))
			{
				SetDlgItemText(hwnd, IDC_EDIT_FILENAME, ptFileNameBuff);		//设置编辑框内容
			}
			return TRUE;
		}
		case IDC_BUTTON_LOAD:
		{
			
			GetDlgItemText(hwnd, IDC_EDIT_FILENAME, ptFileNameBuff, sizeof ptFileNameBuff);	//获取编辑框内容
			if (*ptFileNameBuff == TEXT('\0'))
			{
				SetDlgItemText(hwnd, IDC_STATIC_STATE, TEXT("路径为空"));
				break;
			}
			else {
				dwReadFileSize = loadPEFile(ptFileNameBuff, pFileBuff);		//加载PE文件
				if (pFileBuff)
				{
					SetDlgItemText(hwnd, IDC_STATIC_STATE, TEXT("加载成功"));
				}
				else {
					SetDlgItemText(hwnd, IDC_STATIC_STATE, TEXT("加载失败"));
				}
			}
			return TRUE;
		}
		case IDC_BUTTON_PARSE:
		{
			
		}
		}
	}
	return FALSE;
}


int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
) {
	hAPPInterface = hInstance;		//将模块句柄给全局变量

	DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, WindowProc);	//打开对话框

	
}