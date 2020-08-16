#include "win_peImport.h"

INT_PTR CALLBACK winProcOfImport(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	WORD pwDllWidths[ImportDllListControlColumNumber] = {50, 100,80,80,80,80,80,80 };
	WORD pwFunWidths[ImportFunListControlColumNumber] = {50, 80,80,50,200 };
	HWND hDllListControl;
	HWND hFunListControl;
	NMHDR* pNmhdr;

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
		initListControlHeader(hDllListControl, ImportDllListControlColumNumber, (PTCHAR)TEXT("序号\0DLL名称\0INT\0IAT\0时间戳\0DLL依赖\0特征"), pwDllWidths);
		initListControlHeader(hFunListControl, ImportFunListControlColumNumber, (PTCHAR)TEXT("序号\0Thunk Rva\0Thunk value\0HINT\0函数名"), pwFunWidths);

		//添加Dll列表内容
		addDllListControlContent(hDllListControl);
		return TRUE;
	}
	case WM_NOTIFY:
	{
		pNmhdr = (NMHDR*)lParam;
		switch (wParam)
		{
		case IDC_LIST_IMPORT_DLL:
		{
			//获取通用控件句柄
			hDllListControl = GetDlgItem(hwnd, IDC_LIST_IMPORT_DLL);
			hFunListControl = GetDlgItem(hwnd, IDC_LIST_IMPORT_FUN);

			if (pNmhdr->code == NM_CLICK)	//dll列表控件被点击
			{
				//清楚funList内容
				ListView_DeleteAllItems(hFunListControl);

				//添加函数列表控件内容
				addFunListControlContent(hDllListControl, hFunListControl);
				return TRUE;
			}
		}
		}
	}
	}
	return FALSE;
}


DWORD addDllListControlContent(HWND hwnd) {
	LVITEM lv = { 0 };
	lv.mask = LVIF_TEXT;
	int i = 0;

	if (pImageOptionalHeader32 == 0)
	{
		return 0;
	}

	//获取第一个导入表
	if (pImageOptionalHeader32->Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
	{
		pImageImportDirectory = (PIMAGE_IMPORT_DESCRIPTOR)(rvaToFoa(pFileBuff, pImageOptionalHeader32->DataDirectory[1].VirtualAddress) + (ADWORD)pFileBuff);
	}
	else {
		pImageImportDirectory = (PIMAGE_IMPORT_DESCRIPTOR)(rvaToFoa(pFileBuff, pImageOptionalHeader64->DataDirectory[1].VirtualAddress) + (ADWORD)pFileBuff);
	}

	

	//遍历导入表
	while (pImageImportDirectory->Name)
	{
		lv.iItem = i;

		//序号
		wcsprintf(ptText, TEXT("%d"),i + 1);
		lv.pszText = ptText;
		lv.iSubItem = 0;
		ListView_InsertItem(hwnd, &lv);

		//0.名称
		MultiByteToWideChar(CP_UTF8, 0, (LPCCH)(rvaToFoa(pFileBuff, pImageImportDirectory->Name) + (ADWORD)pFileBuff), -1, ptText, sizeof ptText);
		lv.pszText = ptText;
		lv.iSubItem = 1;
		ListView_SetItem(hwnd, &lv);

		//1.INT
		wcsprintf(ptText, TEXT("%08X"), pImageImportDirectory->OriginalFirstThunk);
		lv.iSubItem = 2;
		ListView_SetItem(hwnd, &lv);

		//2.IAT
		wcsprintf(ptText, TEXT("%08X"), pImageImportDirectory->FirstThunk);
		lv.iSubItem = 3;
		ListView_SetItem(hwnd, &lv);

		//3.时间戳
		wcsprintf(ptText, TEXT("%08X"), pImageImportDirectory->TimeDateStamp);
		lv.iSubItem = 4;
		ListView_SetItem(hwnd, &lv);

		//4.DLL依赖个数
		wcsprintf(ptText, TEXT("%08X"), pImageImportDirectory->ForwarderChain);
		lv.iSubItem = 5;
		ListView_SetItem(hwnd, &lv);

		//5.特征
		wcsprintf(ptText, TEXT("%08X"), pImageImportDirectory->Characteristics);
		lv.iSubItem = 6;
		ListView_SetItem(hwnd, &lv);

		i++;
		pImageImportDirectory++;
	}
	return i;
}


DWORD addFunListControlContent(HWND hDllList,HWND hFunList) {
	PIMAGE_THUNK_DATA32 pImageThunk32 = NULL;
	PIMAGE_THUNK_DATA64 pImageThunk64 = NULL;
	PIMAGE_IMPORT_BY_NAME pImageImportByName = NULL;
	DWORD dwRowNum = 0;
	DWORD dwIATRva = 0;
	DWORD dwThunkRva = 0;

	WORD wIndex = 0;
	LVITEM lv = { 0 };
	lv.mask = LVIF_TEXT;

	//获取行号
	dwRowNum = SendMessage(hDllList, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

	if (dwRowNum == -1)
	{
		return 0;
	}

	//获取IAT指针
	ListView_GetItemText(hDllList, dwRowNum, 3, ptText, sizeof ptText);
	swcscanf_s(ptText, TEXT("%08X"), &dwIATRva);

	if (pImageOptionalHeader32->Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
	{
		pImageThunk32 = (PIMAGE_THUNK_DATA32)(rvaToFoa(pFileBuff, dwIATRva) + (ADWORD)pFileBuff);
		//遍历IAT表
		while (pImageThunk32->u1.AddressOfData)
		{
			lv.iItem = wIndex;
			//0.序号
			wcsprintf(ptText, TEXT("%d"), wIndex + 1);
			lv.pszText = ptText;
			lv.iSubItem = 0;
			ListView_InsertItem(hFunList, &lv);

			//1.Thunk Rva
			dwThunkRva = foaToRva(pFileBuff, (ADWORD)pImageThunk32 - (ADWORD)pFileBuff);
			wcsprintf(ptText, TEXT("%08X"), dwThunkRva);
			lv.pszText = ptText;
			lv.iSubItem = 1;
			ListView_SetItem(hFunList, &lv);

			
			//2.Thunk value
			wcsprintf(ptText, TEXT("%08X"), pImageThunk32->u1.AddressOfData);
			lv.pszText = ptText;
			lv.iSubItem = 2;
			ListView_SetItem(hFunList, &lv);

			if (!(pImageThunk32->u1.AddressOfData & 0x80000000))
			{
				pImageImportByName = (PIMAGE_IMPORT_BY_NAME)(rvaToFoa(pFileBuff, pImageThunk32->u1.AddressOfData) + (ADWORD)pFileBuff);

				//3.Thunk HINT
				wcsprintf(ptText, TEXT("%04X"), pImageImportByName->Hint);
				lv.pszText = ptText;
				lv.iSubItem = 3;
				ListView_SetItem(hFunList, &lv);

				//4.Function Name
				MultiByteToWideChar(CP_UTF8, 0, pImageImportByName->Name, -1, ptText, sizeof ptText);
				lv.pszText = ptText;
				lv.iSubItem = 4;
				ListView_SetItem(hFunList, &lv);
			}
			pImageThunk32++;
			wIndex++;
		}
	}
	else {
		pImageThunk64 = (PIMAGE_THUNK_DATA64)(rvaToFoa(pFileBuff, dwIATRva) + (ADWORD)pFileBuff);
		//遍历IAT表
		while (pImageThunk64->u1.AddressOfData)
		{
			lv.iItem = wIndex;
			//0.序号
			wcsprintf(ptText, TEXT("%d"), wIndex + 1);
			lv.pszText = ptText;
			lv.iSubItem = 0;
			ListView_InsertItem(hFunList, &lv);

			//1.Thunk Rva
			dwThunkRva = foaToRva(pFileBuff, (ADWORD)pImageThunk64 - (ADWORD)pFileBuff);
			wcsprintf(ptText, TEXT("%08X"), dwThunkRva);
			lv.pszText = ptText;
			lv.iSubItem = 1;
			ListView_SetItem(hFunList, &lv);


			//2.Thunk value
			wcsprintf(ptText, TEXT("%08X"), pImageThunk64->u1.AddressOfData);
			lv.pszText = ptText;
			lv.iSubItem = 2;
			ListView_SetItem(hFunList, &lv);

			if (!(pImageThunk64->u1.AddressOfData & 0x8000000000000000))
			{
				pImageImportByName = (PIMAGE_IMPORT_BY_NAME)(rvaToFoa(pFileBuff, pImageThunk64->u1.AddressOfData) + (ADWORD)pFileBuff);

				//3.Thunk HINT
				wcsprintf(ptText, TEXT("%04X"), pImageImportByName->Hint);
				lv.pszText = ptText;
				lv.iSubItem = 3;
				ListView_SetItem(hFunList, &lv);

				//4.Function Name
				MultiByteToWideChar(CP_UTF8, 0, pImageImportByName->Name, -1, ptText, sizeof ptText);
				lv.pszText = ptText;
				lv.iSubItem = 4;
				ListView_SetItem(hFunList, &lv);
			}
			pImageThunk64++;
			wIndex++;
		}
	}

	return wIndex;
}