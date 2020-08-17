#include "tools.h"


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
				CloseHandle(handle);
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

BOOL openFileName(PTCHAR ptText,DWORD dwBuffSize) {
	OPENFILENAME ofn = { 0 };					//�ļ�ѡ��Ի���ṹ

	//��ʼ��OPENFILENAME�ṹ
	ofn.lStructSize = sizeof OPENFILENAME;		//�ṹ��С
	ofn.hwndOwner = NULL;						//�������ڵľ��
	ofn.lpstrFilter = TEXT("PE�ļ�\0*.exe");	//�ļ�ѡ�������
	ofn.nFilterIndex = 1;						//Ĭ�Ϲ���������
	ofn.lpstrFile = ptText;						//�ļ�·����Ż�����
	ofn.nMaxFile = dwBuffSize;					//�ļ�·����Ż���������С
	ofn.lpstrInitialDir = NULL;					//�ļ�ѡ��Ի���Ĭ�ϴ�·��
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;	//�ļ�,·���������,����ֻ���ļ�

	return GetOpenFileName(&ofn);	//���ļ��Ի���
}

