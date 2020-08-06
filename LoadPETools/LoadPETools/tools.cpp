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

	//��Ȩ
	if (processTokenUp(GetCurrentProcess(), SE_DEBUG_NAME))
	{
		hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);	//��ȡ���̿���
		if (hProcessSnap)
		{
			//��ʼ��PROCESSENTRY32
			processInfo.dwSize = sizeof PROCESSENTRY32;

			bGetPro = Process32First(hProcessSnap, &processInfo);	//��ȡ��һ������
			while (bGetPro)
			{
				//0
				wcsprintf(ptText, TEXT("%03d"), i);
				//lv.pszText = ptText;
				lv.pszText = (LPWSTR)TEXT("");			//�����Ե�һ�зű�������������в���ʾ�����
				lv.iItem = 0;							//����������ĳ��������������ʾ��ȫ
				lv.iSubItem = 0;
				ListView_InsertItem(hListControl, &lv);	//������ֻ���ڵ�һ�����ÿ��ı�

				//1.������
				lv.pszText = processInfo.szExeFile;
				lv.iSubItem = 1;
				ListView_SetItem(hListControl, &lv);

				//2.PID
				wcsprintf(ptText, TEXT("%05d"), processInfo.th32ProcessID);
				lv.pszText = ptText;
				lv.iSubItem = 2;
				ListView_SetItem(hListControl, &lv);

				//3.������PID
				wcsprintf(ptText, TEXT("%05d"), processInfo.th32ParentProcessID);
				lv.pszText = ptText;
				lv.iSubItem = 3;
				ListView_SetItem(hListControl, &lv);

				//4.�߳���
				wcsprintf(ptText, TEXT("%03d"), processInfo.cntThreads);
				lv.pszText = ptText;
				lv.iSubItem = 4;
				ListView_SetItem(hListControl, &lv);

				//5.ģ���ַ
				hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processInfo.th32ProcessID);
				EnumProcessModules(hProcess, &hModule, sizeof hModule, &dwModuleCount);
				wcsprintf(ptText, TEXT("%08X"), hModule);
				lv.pszText = ptText;
				lv.iSubItem = 5;
				ListView_SetItem(hListControl, &lv);

				//6.ģ���С
				GetProcessMemoryInfo(hProcess, &pmc, sizeof pmc);
				wcsprintf(ptText, TEXT("%08X"), pmc.WorkingSetSize);
				lv.pszText = ptText;
				lv.iSubItem = 6;
				ListView_SetItem(hListControl, &lv);

				CloseHandle(hProcess);
				i++;
				bGetPro = Process32Next(hProcessSnap, &processInfo);	//��ȡ��һ������
			}
		}
		CloseHandle(hProcessSnap);
	}
	return i;
}

DWORD addMoudelListControlRow(HWND& hProcessListCtrl, HWND& hMoudelListCtrl) {

	DWORD dwPid;		//����PID
	DWORD dwRow;		//�����б�ͨ�ÿؼ����������

	HANDLE hProcess;				//���̾��
	HMODULE phMoudles[512] = { 0 };	//ģ��������
	DWORD dwMoudleCount = 0;		//ģ������ø���
	int i = 0;		//ѭ������

	LVITEM lv = { 0 };				//���ģ���б�ͨ�ÿؼ�����
	lv.mask = LVIF_TEXT;
	TCHAR ptText[259] = { 0 };		//�ı�������

	MODULEINFO moudleInfo = { 0 };	//ģ����Ϣ

	//1.��ȡ�к�
	dwRow = SendMessage(hProcessListCtrl, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

	//2.��ȡPID
	ListView_GetItemText(hProcessListCtrl,dwRow, 2, ptText, sizeof ptText);
	dwPid = StrToLong(ptText);

	//3.��ָ��PID�Ľ���
	if (!processTokenUp(GetCurrentProcess(), SE_DEBUG_NAME))	//��Ȩ
		return 0;

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPid);
	EnumProcessModules(hProcess, phMoudles, sizeof phMoudles, &dwMoudleCount);

	if (dwMoudleCount > sizeof phMoudles)
		MessageBox(0, TEXT("���������ù�С,����DLLû��ʾ����"),TEXT("��ʾ"),0);

	dwMoudleCount /= sizeof HMODULE;

	if (dwMoudleCount > sizeof phMoudles / sizeof HMODULE)
		dwMoudleCount = sizeof phMoudles / sizeof HMODULE;


	//4.��������ģ��,���ģ����Ϣ���б�ͨ�ÿؼ�
	while (i < dwMoudleCount)
	{
		//���
		wcsprintf(ptText, TEXT("%03d"), i);
		lv.pszText = ptText;
		lv.iSubItem = 0;
		ListView_InsertItem(hMoudelListCtrl, &lv);

		//0.ģ����
		GetModuleBaseName(hProcess, phMoudles[i], ptText, sizeof ptText);
		lv.pszText = ptText;
		lv.iSubItem = 1;
		ListView_SetItem(hMoudelListCtrl, &lv);
		//ListView_InsertItem(hMoudelListCtrl, &lv);

		GetModuleInformation(hProcess, phMoudles[i], &moudleInfo, sizeof moudleInfo);
		//1.ģ���ַ
		wcsprintf(ptText, TEXT("%08X"), (DWORD)moudleInfo.lpBaseOfDll);
		lv.pszText = ptText;
		lv.iSubItem = 2;
		ListView_SetItem(hMoudelListCtrl, &lv);

		//2.ģ���С
		wcsprintf(ptText, TEXT("%08X"), moudleInfo.SizeOfImage);
		lv.pszText = ptText;
		lv.iSubItem = 3;
		ListView_SetItem(hMoudelListCtrl, &lv);

		//3.ģ�����
		wcsprintf(ptText, TEXT("%08X"), (DWORD)moudleInfo.EntryPoint);
		lv.pszText = ptText;
		lv.iSubItem = 4;
		ListView_SetItem(hMoudelListCtrl, &lv);

		i++;
	}


	CloseHandle(hProcess);
	return i;

	

}