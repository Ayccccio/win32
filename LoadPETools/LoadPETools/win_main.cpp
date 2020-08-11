#include "win_main.h"

//主窗口消息处理回调函数
INT_PTR CALLBACK WindowsProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	HICON hIcon = NULL;				//图标句柄
	HWND hProcessListCtrl = NULL;	//进程列表通用控件句柄
	HWND hMoudelListCtrl = NULL;	//模块列表通用控件句柄

	WORD pdProListCtrlColWidth[ProcessListControlColumNumber] = { 0,200,50,80,50,100,100 };		//进程列表通用控件列的宽度
	WORD pdMudListCtrlColWidth[MoudelListControlColumNumber] = { 50, 200,100,100,100 };			//模块列表通用控件列的宽度

	NMHDR* pNmhdr;			//WM_NOTIFY 消息结构指针


	switch (uMsg)
	{
	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
		return TRUE;
	}
	case WM_INITDIALOG:
	{
		//设置图标
		hIcon = LoadIcon(hAPPInterface, MAKEINTRESOURCE(IDI_ICON1));
		SendMessage((HWND)hAPPInterface, WM_SETICON, ICON_BIG, (DWORD)hIcon);
		SendMessage((HWND)hAPPInterface, WM_SETICON, ICON_SMALL, (DWORD)hIcon);

		hProcessListCtrl = GetDlgItem(hwnd, IDC_LIST_PROCESS);	//获取进程列表通用控件句柄
		hMoudelListCtrl = GetDlgItem(hwnd, IDC_LIST_MOUDEL);	//获取模块列表通用控件句柄

		//设置进程列表通用对话框选中
		SendMessage(hProcessListCtrl, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

		//列表通用对话框初始化表头
		initListControlHeader(hProcessListCtrl, ProcessListControlColumNumber, (PTCHAR)TEXT("序号\0进程名\0PID\0父级PID\0线程数\0镜像地址\0镜像大小"), pdProListCtrlColWidth);	//初始化进程列表通用控件表头
		initListControlHeader(hMoudelListCtrl, MoudelListControlColumNumber, (PTCHAR)TEXT("序号\0模块名称\0模块地址\0模块大小\0模块入口"), pdMudListCtrlColWidth);	//初始化进程列表通用控件表头

		//添加进程通用控件内容
		addProcessListControlRow(hProcessListCtrl);
		return TRUE;
	}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case IDC_BUTTON_LOGOUT:
		{
			EndDialog(hwnd, 0);
			break;
		}
		case IDC_BUTTON_ABOUT:
		{

			break;
		}
		case IDC_BUTTON_PEEDIT:
		{
			if (!bFlag)	//判断文件对话框是否打开
			{
				if (openFileName(ptText, sizeof ptText))	//文件对话框获取文件目录
				{
					DialogBox(hAPPInterface, MAKEINTRESOURCE(IDD_DIALOG_PEEDIT), hwnd, peEditWinProc);
				}
				bFlag = FALSE;		//开关置0
			}
			break;
		}
		default:
			return FALSE;
			return TRUE;
		}
	}
	case WM_NOTIFY:
	{
		pNmhdr = (NMHDR*)lParam;
		switch (wParam)
		{
		case IDC_LIST_PROCESS:
		{
			hProcessListCtrl = GetDlgItem(hwnd, IDC_LIST_PROCESS);
			if (pNmhdr->code == NM_CLICK)
			{
				hMoudelListCtrl = GetDlgItem(hwnd, IDC_LIST_MOUDEL);
				ListView_DeleteAllItems(hMoudelListCtrl);
				addMoudelListControlRow(hProcessListCtrl, hMoudelListCtrl);
			}
		}
		return TRUE;
		}
	}
	default:
		return FALSE;
		return TRUE;
	}


	return FALSE;
}

DWORD addProcessListControlRow(HWND hListControl) {

	HANDLE hProcessSnap;
	HANDLE hProcess;
	HMODULE hModule = 0;
	PROCESSENTRY32 processInfo = { 0 };
	BOOL bGetPro;
	PROCESS_MEMORY_COUNTERS pmc = { 0 };

	DWORD dwModuleCount;

	LVITEM lv = { 0 };
	lv.mask = LVFIF_TEXT;

	int i = 0;
	TCHAR ptText[259] = { 0 };

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);	//获取进程快照
	if (hProcessSnap)
	{
		//初始化PROCESSENTRY32
		processInfo.dwSize = sizeof PROCESSENTRY32;

		bGetPro = Process32First(hProcessSnap, &processInfo);	//获取第一个进程
		while (bGetPro)
		{
			//0
			wcsprintf(ptText, TEXT("%03d"), i);
			//lv.pszText = ptText;
			lv.pszText = (LPWSTR)TEXT("");			//经测试第一列放变量会出现其他列不显示的情况
			lv.iItem = 0;							//经测试这里改成行索引会出现显示不全
			lv.iSubItem = 0;
			ListView_InsertItem(hListControl, &lv);	//经测试只能在第一列设置空文本

			//1.进程名
			lv.pszText = processInfo.szExeFile;
			lv.iSubItem = 1;
			ListView_SetItem(hListControl, &lv);

			//2.PID
			wcsprintf(ptText, TEXT("%05d"), processInfo.th32ProcessID);
			lv.pszText = ptText;
			lv.iSubItem = 2;
			ListView_SetItem(hListControl, &lv);

			//3.父进程PID
			wcsprintf(ptText, TEXT("%05d"), processInfo.th32ParentProcessID);
			lv.pszText = ptText;
			lv.iSubItem = 3;
			ListView_SetItem(hListControl, &lv);

			//4.线程数
			wcsprintf(ptText, TEXT("%03d"), processInfo.cntThreads);
			lv.pszText = ptText;
			lv.iSubItem = 4;
			ListView_SetItem(hListControl, &lv);

			//5.模块地址
			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processInfo.th32ProcessID);
			EnumProcessModules(hProcess, &hModule, sizeof hModule, &dwModuleCount);
			wcsprintf(ptText, TEXT("%08X"), hModule);
			lv.pszText = ptText;
			lv.iSubItem = 5;
			ListView_SetItem(hListControl, &lv);

			//6.模块大小
			GetProcessMemoryInfo(hProcess, &pmc, sizeof pmc);
			wcsprintf(ptText, TEXT("%08X"), pmc.WorkingSetSize);
			lv.pszText = ptText;
			lv.iSubItem = 6;
			ListView_SetItem(hListControl, &lv);

			CloseHandle(hProcess);
			i++;
			bGetPro = Process32Next(hProcessSnap, &processInfo);	//获取下一个进程
		}
	}
	CloseHandle(hProcessSnap);
	return i;
}

DWORD addMoudelListControlRow(HWND& hProcessListCtrl, HWND& hMoudelListCtrl) {

	DWORD dwPid;		//进程PID
	DWORD dwRow;		//进程列表通用控件点击的行数

	HANDLE hProcess;				//进程句柄
	HMODULE phMoudles[512] = { 0 };	//模块句柄集合
	DWORD dwMoudleCount = 0;		//模块句柄获得个数
	int i = 0;		//循环变量

	LVITEM lv = { 0 };				//添加模块列表通用控件内容
	lv.mask = LVIF_TEXT;
	TCHAR ptText[259] = { 0 };		//文本缓冲区

	MODULEINFO moudleInfo = { 0 };	//模块信息

	//1.获取行号
	dwRow = SendMessage(hProcessListCtrl, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

	//2.获取PID
	ListView_GetItemText(hProcessListCtrl, dwRow, 2, ptText, sizeof ptText);
	dwPid = StrToLong(ptText);

	//3.打开指定PID的进程
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPid);

	//4.枚举进程模块
	EnumProcessModulesEx(hProcess, phMoudles, sizeof phMoudles, &dwMoudleCount, LIST_MODULES_ALL);

	if (dwMoudleCount > sizeof phMoudles)
		MessageBox(0, TEXT("缓冲区设置过小,部分DLL没显示出来"), TEXT("提示"), 0);

	dwMoudleCount /= sizeof HMODULE;

	if (dwMoudleCount > sizeof phMoudles / sizeof HMODULE)
		dwMoudleCount = sizeof phMoudles / sizeof HMODULE;


	//5.遍历进程模块,添加模块信息到列表通用控件
	while (i < dwMoudleCount)
	{
		//序号
		wcsprintf(ptText, TEXT("%03d"), i);
		lv.pszText = ptText;
		lv.iSubItem = 0;
		ListView_InsertItem(hMoudelListCtrl, &lv);

		//0.模块名
		GetModuleBaseName(hProcess, phMoudles[i], ptText, sizeof ptText);
		lv.pszText = ptText;
		lv.iSubItem = 1;
		ListView_SetItem(hMoudelListCtrl, &lv);
		//ListView_InsertItem(hMoudelListCtrl, &lv);

		GetModuleInformation(hProcess, phMoudles[i], &moudleInfo, sizeof moudleInfo);
		//1.模块基址
		wcsprintf(ptText, TEXT("%08X"), (DWORD)moudleInfo.lpBaseOfDll);
		lv.pszText = ptText;
		lv.iSubItem = 2;
		ListView_SetItem(hMoudelListCtrl, &lv);

		//2.模块大小
		wcsprintf(ptText, TEXT("%08X"), moudleInfo.SizeOfImage);
		lv.pszText = ptText;
		lv.iSubItem = 3;
		ListView_SetItem(hMoudelListCtrl, &lv);

		//3.模块入口
		wcsprintf(ptText, TEXT("%08X"), (DWORD)moudleInfo.EntryPoint);
		lv.pszText = ptText;
		lv.iSubItem = 4;
		ListView_SetItem(hMoudelListCtrl, &lv);

		i++;
	}
	CloseHandle(hProcess);
	return i;
}
