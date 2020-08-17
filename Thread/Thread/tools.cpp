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
				CloseHandle(handle);
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

BOOL openFileName(PTCHAR ptText,DWORD dwBuffSize) {
	OPENFILENAME ofn = { 0 };					//文件选择对话框结构

	//初始化OPENFILENAME结构
	ofn.lStructSize = sizeof OPENFILENAME;		//结构大小
	ofn.hwndOwner = NULL;						//所属窗口的句柄
	ofn.lpstrFilter = TEXT("PE文件\0*.exe");	//文件选择过滤器
	ofn.nFilterIndex = 1;						//默认过滤器索引
	ofn.lpstrFile = ptText;						//文件路径存放缓冲区
	ofn.nMaxFile = dwBuffSize;					//文件路径存放缓冲区最大大小
	ofn.lpstrInitialDir = NULL;					//文件选择对话框默认打开路径
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;	//文件,路径必须存在,隐藏只读文件

	return GetOpenFileName(&ofn);	//打开文件对话框
}

