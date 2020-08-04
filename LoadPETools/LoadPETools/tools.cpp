#define  _CRT_SECURE_NO_WARNINGS

#include "tools.h"



void __cdecl OutputDebugStringF(const TCHAR* format, ...)
{
	va_list vlArgs;
	PTCHAR strBuffer = (PTCHAR)GlobalAlloc(GPTR, 4096);

	va_start(vlArgs, format);
	_vsntprintf(strBuffer, 4096 - 1, format, vlArgs);
	va_end(vlArgs);
	StrCat(strBuffer, TEXT("\n"));
	OutputDebugString(strBuffer);
	GlobalFree(strBuffer);
	return;
}


DWORD initListControlHeader(HWND hListControl,DWORD dwLenth,PTCHAR ptColumNames) {
	LVCOLUMN lvCol = { 0 };
	int i = 0;
	PTCHAR ptTemp = ptColumNames;

	lvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

	while (i < dwLenth)
	{
		lvCol.pszText = ptTemp;
		if (i == 0)
			lvCol.cx = 200;
		else
			lvCol.cx = 100;

		lvCol.iSubItem = i;
		ListView_InsertColumn(hListControl,i, &lvCol);
		ptTemp += StrLen(ptTemp) + 1;
		i++;
	}
	return i;
}


BOOL privilegeUp(HANDLE processHandle,LPCWSTR privilege) {
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

DWORD addProcessListControlRow(HWND hListControl) {

	HANDLE hProcess;
	PROCESSENTRY32 processInfo = {0};
	BOOL bGetPro;
	PROCESS_MEMORY_COUNTERS pmc = {0};

	LV_ITEM lv = {0};
	lv.mask = LVIF_TEXT;
	int i = 0;
	TCHAR pszText[1024] = {0};

	BOOL j;

	//提权
	if (privilegeUp(GetCurrentProcess(), SE_DEBUG_NAME))
	{
		hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
		if (hProcess)
		{
			//初始化PROCESSENTRY32
			processInfo.dwSize = sizeof PROCESSENTRY32;

			bGetPro = Process32First(hProcess, &processInfo);
			while (bGetPro)
			{
				//1
				lv.iItem = i;
				lv.iSubItem = 0;
				lv.pszText = processInfo.szExeFile;
				ListView_InsertItem(hListControl, &lv);
				//SendMessage(hListControl, LVM_INSERTITEM, 0, (DWORD)&lv);

				//2
				lv.iItem = i;
				lv.iSubItem = 1;
				lv.pszText = pszText;
				wcsprintf(pszText, TEXT("%d"), processInfo.th32ProcessID);
				ListView_SetItem(hListControl, &lv);

				//3
				lv.iItem = i;
				lv.iSubItem = 2;
				lv.pszText = pszText;
				wcsprintf(pszText, TEXT("%d"), processInfo.th32ModuleID);
				ListView_SetItem(hListControl, &lv);

				//4
				lv.iItem = i;
				lv.iSubItem = 3;
				GetProcessMemoryInfo(hProcess, &pmc, sizeof pmc);
				wcsprintf(lv.pszText, TEXT("%d"), pmc.WorkingSetSize);
				ListView_SetItem(hListControl, &lv);

				i++;
				bGetPro = Process32Next(hProcess, &processInfo);
			}
		}
		CloseHandle(hProcess);
	}
	return i;
}