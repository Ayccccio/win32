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

	lvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

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


DWORD addListControlRow(HWND hListControl,DWORD dwRow, DWORD dwCol, PTCHAR ptText) {
	DWORD ret;	//返回值
	LV_ITEM lv = { 0 };
	lv.mask = LVIF_TEXT;

	lv.iItem = dwRow;
	lv.iSubItem = dwCol;
	lv.pszText = ptText;
	DbgPrintf(TEXT("%s  "), lv.pszText);
	if (dwCol == 4)
	{
		DbgPrintf(TEXT("\n"));
	}
	//SendMessage(hListControl, LVM_INSERTITEM, 0, (DWORD)&lv);

	if (dwCol == 0)
		ret = ListView_InsertItem(hListControl, &lv);
	else
		ret = ListView_SetItem(hListControl, &lv);
	return ret;
}

DWORD addProcessListControlRow(HWND hProcessListCtrl) {

	HANDLE hProcess;
	PROCESSENTRY32 processInfo = {0};
	BOOL bGetPro;
	PROCESS_MEMORY_COUNTERS pmc = {0};

	LV_ITEM lv = { 0 };
	lv.mask = LVIF_TEXT;
	
	int i = 0;
	TCHAR ptText[1024] = {0};

	BOOL j;

	//提权
	if (privilegeUp(GetCurrentProcess(), SE_DEBUG_NAME))
	{
		hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);	//获取进程快照
		if (hProcess)
		{
			//初始化PROCESSENTRY32
			processInfo.dwSize = sizeof PROCESSENTRY32;

			bGetPro = Process32First(hProcess, &processInfo);	//获取第一个进程
			while (bGetPro)
			{
				//0
				//wcsprintf(ptText, TEXT("%d"), i);
				//addListControlRow(hListControl, i, 0, ptText);

				////1
				//addListControlRow(hListControl, i, 1, processInfo.szExeFile);

				////2
				//wcsprintf(ptText, TEXT("%d"), processInfo.th32ProcessID);
				//addListControlRow(hListControl, i, 2, ptText);

				////3
				//wcsprintf(ptText, TEXT("%d"), processInfo.th32ModuleID);
				//addListControlRow(hListControl, i, 3, ptText);

				////4
				//wcsprintf(ptText, TEXT("%d"), pmc.WorkingSetSize);
				//addListControlRow(hListControl, i, 4, ptText);

				LV_ITEM vitem;

				//初始化						
				memset(&vitem, 0, sizeof(LV_ITEM));
				vitem.mask = LVIF_TEXT;

				vitem.pszText = (LPWSTR)TEXT("csrss.exe");
				vitem.iItem = 0;
				vitem.iSubItem = 0;
				ListView_InsertItem(hProcessListCtrl, &vitem);
				//SendMessage(hProcessListCtrl, LVM_INSERTITEM, 0, (DWORD)&vitem);

				vitem.pszText = (LPWSTR)TEXT("448");
				vitem.iItem = 0;
				vitem.iSubItem = 1;
				ListView_SetItem(hProcessListCtrl, &vitem);

				vitem.pszText = (LPWSTR)TEXT("56590000");
				vitem.iItem = 0;
				vitem.iSubItem = 2;
				ListView_SetItem(hProcessListCtrl, &vitem);

				vitem.pszText = (LPWSTR)TEXT("000F0000");
				vitem.iItem = 0;
				vitem.iSubItem = 3;
				ListView_SetItem(hProcessListCtrl, &vitem);

				

				i++;
				bGetPro = Process32Next(hProcess, &processInfo);	//获取下一个进程
			}
		}
		CloseHandle(hProcess);
	}
	return i;
}