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


BOOL privilegeUp(HANDLE processHandle,LPCWSTR privilege) {
	//��Ȩ
	HANDLE handle;
	LUID lUid;
	TOKEN_PRIVILEGES tkp;
	if (OpenProcessToken(processHandle, TOKEN_ALL_ACCESS, &handle))		//��ȡ���̵����ƾ��
	{
		if (LookupPrivilegeValue(NULL, privilege, &lUid))		//��ȡȨ��LUID
		{
			//����Ȩ�޴洢�ṹ��
			tkp.PrivilegeCount = 1;
			tkp.Privileges[0].Luid = lUid;
			tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			if (AdjustTokenPrivileges(handle, 0, &tkp, sizeof tkp, NULL, NULL))	//��Ȩ�޽ṹ���޸�����Ȩ��
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}


DWORD addListControlRow(HWND hListControl,DWORD dwRow, DWORD dwCol, PTCHAR ptText) {
	DWORD ret;	//����ֵ
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

DWORD addProcessListControlRow(HWND hListControl) {

	HANDLE hProcess;
	PROCESSENTRY32 processInfo = {0};
	BOOL bGetPro;
	PROCESS_MEMORY_COUNTERS pmc = {0};

	LV_ITEM lv = { 0 };
	lv.mask = LVIF_TEXT;
	
	int i = 0;
	TCHAR ptText[259] = {0};

	//��Ȩ
	if (privilegeUp(GetCurrentProcess(), SE_DEBUG_NAME))
	{
		hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);	//��ȡ���̿���
		if (hProcess)
		{
			//��ʼ��PROCESSENTRY32
			processInfo.dwSize = sizeof PROCESSENTRY32;

			bGetPro = Process32First(hProcess, &processInfo);	//��ȡ��һ������
			while (bGetPro)
			{
				//0
				//wcsprintf(ptText, TEXT("%d"), i);
				//wcsprintf(ptText, TEXT("%d"), processInfo.th32ProcessID);
				//addListControlRow(hListControl, 0, 0, ptText);
				addListControlRow(hListControl, 0, 0, (PTCHAR)TEXT(""));

				//1.������
				addListControlRow(hListControl, 0, 1, processInfo.szExeFile);

				//2.PID
				wcsprintf(ptText, TEXT("%d"), processInfo.th32ProcessID);
				addListControlRow(hListControl, 0, 2, ptText);

				//3.������PID
				wcsprintf(ptText, TEXT("%d"), processInfo.th32ParentProcessID);
				addListControlRow(hListControl, 0, 3, ptText);

				//4.�߳���
				wcsprintf(ptText, TEXT("%d"), processInfo.cntThreads);
				addListControlRow(hListControl, 0, 4, ptText);

				//5.ģ���ַ

				//6.ģ���С

				i++;
				bGetPro = Process32Next(hProcess, &processInfo);	//��ȡ��һ������
			}
		}
		CloseHandle(hProcess);
	}
	return i;
}