#include "win_peExport.h"

INT_PTR CALLBACK winProcOfExport(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam) 
{
	WORD pwListWidths[ExportListControlColumNumber] = { 80,100,100,300 };
	HWND hListControl;

	switch (uMsg)
	{
	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
		return TRUE;
	}
	case WM_INITDIALOG:
	{
		//获取列表通用控件句柄
		hListControl = GetDlgItem(hwnd, IDC_LIST_EXPROT);

		//设置列表控件整行选中
		SendMessage(hListControl, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

		//初始化列表通用控件头部
		initListControlHeader(hListControl, ExportListControlColumNumber, (PTCHAR)TEXT("函数序号\0RVA\0FOA\0函数名"), pwListWidths);

		//添加导出表编辑框内容
		addExportEditContent(hwnd);

		//添加列表通用控件内容
		addExportListContent(hListControl);
		return TRUE;
	}
	case WM_COMMAND:
	{
		
	}
	default:
		return FALSE;
	}
	return FALSE;
}


DWORD addExportEditContent(HWND hwnd) {
	if (pFileBuff == NULL)
	{
		return 0;
	}

	//获取导出表
	pImageExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((ADWORD)pFileBuff + rvaToFoa(pFileBuff, pImageOptionalHeader->DataDirectory[0].VirtualAddress));

	//特征
	wcsprintf(ptText, TEXT("%08X"), pImageExportDirectory->Characteristics);
	SetDlgItemText(hwnd, IDC_EDIT_EXPORT_CHARACTER,ptText);

	//时间戳
	wcsprintf(ptText, TEXT("%08X"), pImageExportDirectory->TimeDateStamp);
	SetDlgItemText(hwnd, IDC_EDIT_EXPORT_TIME, ptText);

	//基本序号
	wcsprintf(ptText, TEXT("%08X"), pImageExportDirectory->Base);
	SetDlgItemText(hwnd, IDC_EDIT_EXPORT_BASE, ptText);

	//名称地址
	wcsprintf(ptText, TEXT("%08X"), pImageExportDirectory->Name);
	SetDlgItemText(hwnd, IDC_EDIT_EXPORT_NAME_ADDR, ptText);

	//程序名称
	wcsprintf(ptText, TEXT("%S"), rvaToFoa(pFileBuff,pImageExportDirectory->Name) + (ADWORD)pFileBuff);
	SetDlgItemText(hwnd, IDC_EDIT_EXPORT_NAME, ptText);

	//函数数量
	wcsprintf(ptText, TEXT("%08X"), pImageExportDirectory->NumberOfFunctions);
	SetDlgItemText(hwnd, IDC_EDIT_EXPORT_FNNUM, ptText);

	//函数名数量
	wcsprintf(ptText, TEXT("%08X"), pImageExportDirectory->NumberOfNames);
	SetDlgItemText(hwnd, IDC_EDIT_EXPORT_FNNAMENUM, ptText);

	//函数地址
	wcsprintf(ptText, TEXT("%08X"), pImageExportDirectory->AddressOfFunctions);
	SetDlgItemText(hwnd, IDC_EDIT_EXPORT_FN_ADDR, ptText);

	//函数名地址
	wcsprintf(ptText, TEXT("%08X"), pImageExportDirectory->AddressOfNames);
	SetDlgItemText(hwnd, IDC_EDIT_EXPORT_FNNAME_ADDR, ptText);

	//函数名序号地址
	wcsprintf(ptText, TEXT("%08X"), pImageExportDirectory->AddressOfNameOrdinals);
	SetDlgItemText(hwnd, IDC_EDIT_EXPORT_FNNAMEOR_ADDR, ptText);

	return 1;
}


DWORD addExportListContent(HWND hwnd) {
	LVITEM lv = { 0 };
	lv.mask = LVIF_TEXT;
	int i = 0;
	int j = 0;

	PDWORD pdwFun = NULL;
	PDWORD pdwFunName = NULL;
	PWORD pdwFunNameOrd = NULL;
	PCHAR pFunName = NULL;
	PTCHAR ptTemp = ptText;

	if (pImageExportDirectory == NULL)
	{
		return 0;
	}

	pdwFun = (PDWORD)((ADWORD)pFileBuff + rvaToFoa(pFileBuff, pImageExportDirectory->AddressOfFunctions));
	pdwFunName = (PDWORD)((ADWORD)pFileBuff + rvaToFoa(pFileBuff, pImageExportDirectory->AddressOfNames));
	pdwFunNameOrd = (PWORD)((ADWORD)pFileBuff + rvaToFoa(pFileBuff, pImageExportDirectory->AddressOfNameOrdinals));


	//遍历函数地址
	while (i < pImageExportDirectory->NumberOfFunctions)
	{
		pFunName = NULL;
		j = 0;

		//遍历函数序号表
		while (j < pImageExportDirectory->NumberOfNames)
		{
			//判断序号表中是否有当前函数序号
			if (*(pdwFunNameOrd + j) == i)
			{
				pFunName = (PCHAR) (rvaToFoa(pFileBuff, *(pdwFunName + i)) + (ADWORD)pFileBuff);
				break;
			}
			j++;
		}

		lv.iItem = i;
		//1.函数序号
		wcsprintf(ptText, TEXT("%d"), i + pImageExportDirectory->Base);
		lv.pszText = ptText;
		lv.iSubItem = 0;
		ListView_InsertItem(hwnd, &lv);

		//2.rva
		wcsprintf(ptText, TEXT("%08X"), *(pdwFun + i));
		lv.pszText = ptText;
		lv.iSubItem = 1;
		ListView_SetItem(hwnd, &lv);

		//3.foa
		wcsprintf(ptText, TEXT("%08X"), rvaToFoa(pFileBuff, *(pdwFun + i)));
		lv.pszText = ptText;
		lv.iSubItem = 2;
		ListView_SetItem(hwnd, &lv);

		if (pFunName)
		{
			//4.函数名
			MultiByteToWideChar(CP_UTF8, 0, pFunName, -1, ptText, sizeof ptText);
			lv.pszText = ptText;
			lv.iSubItem = 3;
			ListView_SetItem(hwnd, &lv);
		}
		i++;
	}

}