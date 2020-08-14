#include "win_peImport.h"

INT_PTR CALLBACK winProcOfImport(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	WORD pwDllWidths[ImportDllListControlColumNumber] = { 200,100,100,100,100,100,100 };
	WORD pwFunWidths[ImportFunListControlColumNumber] = { 100,100,100,100,200 };
	HWND hDllListControl;
	HWND hFunListControl;

	switch (uMsg)
	{
	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
		return TRUE;
	}
	case WM_INITDIALOG:
	{
		//获取通用控件句柄
		hDllListControl = GetDlgItem(hwnd, IDC_LIST_IMPORT_DLL);
		hFunListControl = GetDlgItem(hwnd, IDC_LIST_IMPORT_FUN);

		//设置列表控件整行选中
		SendMessage(hDllListControl, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
		SendMessage(hFunListControl, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

		//初始化列表通用控件表头
		initListControlHeader(hDllListControl, ImportDllListControlColumNumber, (PTCHAR)TEXT("DLL名称\0INT\0IAT\0时间戳\0DLL依赖\0特征"), pwDllWidths);
		initListControlHeader(hDllListControl, ImportFunListControlColumNumber, (PTCHAR)TEXT("RVA\0FOA\0VALUE\0HINT\0函数名"), pwFunWidths);

		//添加Dll列表内容
		addDllListControlContent(hDllListControl);
	}
	default:
		break;
	}
	return FALSE;
}


DWORD addDllListControlContent(HWND hwnd) {
	LVITEM lv = { 0 };
	lv.mask = LVIF_TEXT;
	int i = 0;

	if (pImageOptionalHeader == 0)
	{
		return 0;
	}

	//获取第一个导入表
	pImageImport = (PIMAGE_IMPORT_DESCRIPTOR)(rvaToFoa(pFileBuff, pImageOptionalHeader->DataDirectory[1].VirtualAddress) + (ADWORD)pFileBuff);

	//遍历导入表
	while (pImageImport->Name)
	{
		lv.iItem = i;

		//0.名称
		wcsprintf(ptText, TEXT("%s"), rvaToFoa(pFileBuff, pImageImport->Name) + (ADWORD)pFileBuff);
		lv.pszText = ptText;
		lv.iSubItem = 0;
		ListView_InsertItem(hwnd, &lv);

		//1.INT
		wcsprintf(ptText, TEXT("%08X"), pImageImport->OriginalFirstThunk);
		lv.iSubItem = 1;
		ListView_SetItem(hwnd, &lv);

		//2.IAT
		wcsprintf(ptText, TEXT("%08X"), pImageImport->FirstThunk);
		lv.iSubItem = 2;
		ListView_SetItem(hwnd, &lv);

		//3.时间戳
		wcsprintf(ptText, TEXT("%08X"), pImageImport->TimeDateStamp);
		lv.iSubItem = 3;
		ListView_SetItem(hwnd, &lv);

		//4.DLL依赖个数
		wcsprintf(ptText, TEXT("%08X"), pImageImport->ForwarderChain);
		lv.iSubItem = 4;
		ListView_SetItem(hwnd, &lv);

		//5.特征
		wcsprintf(ptText, TEXT("%08X"), pImageImport->Characteristics);
		lv.iSubItem = 5;
		ListView_SetItem(hwnd, &lv);

		i++;
		pImageImport++;
	}
	return i;
}

