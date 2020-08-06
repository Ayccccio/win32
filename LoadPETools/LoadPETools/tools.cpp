#define  _CRT_SECURE_NO_WARNINGS

#include "tools.h"



void __cdecl OutputDebugStringF(const TCHAR* format, ...)
{
	va_list vlArgs;
	PTCHAR strBuffer = (PTCHAR)GlobalAlloc(GPTR, 4096);

	va_start(vlArgs, format);
	_vsntprintf(strBuffer, 4096 - 1, format, vlArgs);
	va_end(vlArgs);
	//StrCat(strBuffer, TEXT("\n"));
	OutputDebugString(strBuffer);
	GlobalFree(strBuffer);
	return;
}


DWORD initListControlHeader(HWND hListControl,DWORD dwLenth,PTCHAR ptColumNames,PWORD pdColWidths) {
	LVCOLUMN lvCol = { 0 };
	int i = 0;
	PTCHAR ptTemp = ptColumNames;

	lvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCFMT_RIGHT;

	while (i < dwLenth)
	{
		lvCol.pszText = ptTemp;
		lvCol.cx = pdColWidths[i];
		lvCol.iSubItem = i;
		ListView_InsertColumn(hListControl,i, &lvCol);
		ptTemp += StrLen(ptTemp) + 1;
		i++;
	}
	return i;
}


BOOL processTokenUp(HANDLE processHandle,LPCWSTR privilege) {
	//提权
	HANDLE handle;
	LUID lUid;
	TOKEN_PRIVILEGES tkp;
	if (OpenProcessToken(processHandle, TOKEN_ALL_ACCESS, &handle))		//获取进程的令牌句柄
	{
		if (LookupPrivilegeValue(NULL, privilege, &lUid))		//获取权限LUID
		{
			//设置权限存储结构体
			tkp.PrivilegeCount = 1;
			tkp.Privileges[0].Luid = lUid;
			tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			if (AdjustTokenPrivileges(handle, 0, &tkp, sizeof tkp, NULL, NULL))	//按权限结构体修改令牌权限
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}


DWORD addListControlRow(HWND hListControl,DWORD dwRow, DWORD dwCol, PTCHAR ptText) {
	DWORD ret;	//返回值
	LV_ITEM lv = { 0 };
	lv.mask = LVIF_TEXT;

	lv.iItem = dwRow;
	lv.iSubItem = dwCol;
	lv.pszText = ptText;

	if (dwCol == 0)
		ret = ListView_InsertItem(hListControl, &lv);
	else
		ret = ListView_SetItem(hListControl, &lv);
	return ret;
}

DWORD addProcessListControlRow(HWND hListControl) {

	HANDLE hProcessSnap;
	HANDLE hProcess;
	HMODULE hModule = 0;
	PROCESSENTRY32 processInfo = {0};
	BOOL bGetPro;
	PROCESS_MEMORY_COUNTERS pmc = {0};

	DWORD dwModuleCount;

	LVITEM lv = { 0 };
	lv.mask = LVFIF_TEXT;
	
	int i = 0;
	TCHAR ptText[259] = {0};

	//提权
	if (processTokenUp(GetCurrentProcess(), SE_DEBUG_NAME))
	{
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
	}
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
	ListView_GetItemText(hProcessListCtrl,dwRow, 2, ptText, sizeof ptText);
	dwPid = StrToLong(ptText);

	//3.打开指定PID的进程
	if (!processTokenUp(GetCurrentProcess(), SE_DEBUG_NAME))	//提权
		return 0;

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPid);
	EnumProcessModules(hProcess, phMoudles, sizeof phMoudles, &dwMoudleCount);

	if (dwMoudleCount > sizeof phMoudles)
		MessageBox(0, TEXT("缓冲区设置过小,部分DLL没显示出来"),TEXT("提示"),0);

	dwMoudleCount /= sizeof HMODULE;

	if (dwMoudleCount > sizeof phMoudles / sizeof HMODULE)
		dwMoudleCount = sizeof phMoudles / sizeof HMODULE;


	//4.遍历进程模块,添加模块信息到列表通用控件
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