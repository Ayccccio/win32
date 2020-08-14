#include "win_peImport.h"

INT_PTR CALLBACK winProcOfImport(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	WORD pwDllWidths[ImportDllListControlColumNumber] = {50, 100,80,80,80,80,80,80 };
	WORD pwFunWidths[ImportFunListControlColumNumber] = {50, 80,80,80,50,200 };
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
		//��ȡͨ�ÿؼ����
		hDllListControl = GetDlgItem(hwnd, IDC_LIST_IMPORT_DLL);
		hFunListControl = GetDlgItem(hwnd, IDC_LIST_IMPORT_FUN);

		//�����б�ؼ�����ѡ��
		SendMessage(hDllListControl, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
		SendMessage(hFunListControl, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

		//��ʼ���б�ͨ�ÿؼ���ͷ
		initListControlHeader(hDllListControl, ImportDllListControlColumNumber, (PTCHAR)TEXT("���\0DLL����\0INT\0IAT\0ʱ���\0DLL����\0����"), pwDllWidths);
		initListControlHeader(hFunListControl, ImportFunListControlColumNumber, (PTCHAR)TEXT("���\0RVA\0FOA\0VALUE\0HINT\0������"), pwFunWidths);

		//���Dll�б�����
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

	//��ȡ��һ�������
	pImageImportDirectory = (PIMAGE_IMPORT_DESCRIPTOR)(rvaToFoa(pFileBuff, pImageOptionalHeader->DataDirectory[1].VirtualAddress) + (ADWORD)pFileBuff);

	//���������
	while (pImageImportDirectory->Name)
	{
		lv.iItem = i;

		//���
		wcsprintf(ptText, TEXT("%d"),i);
		lv.pszText = ptText;
		lv.iSubItem = 0;
		ListView_InsertItem(hwnd, &lv);

		//0.����
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

		//3.ʱ���
		wcsprintf(ptText, TEXT("%08X"), pImageImportDirectory->TimeDateStamp);
		lv.iSubItem = 4;
		ListView_SetItem(hwnd, &lv);

		//4.DLL��������
		wcsprintf(ptText, TEXT("%08X"), pImageImportDirectory->ForwarderChain);
		lv.iSubItem = 5;
		ListView_SetItem(hwnd, &lv);

		//5.����
		wcsprintf(ptText, TEXT("%08X"), pImageImportDirectory->Characteristics);
		lv.iSubItem = 6;
		ListView_SetItem(hwnd, &lv);

		i++;
		pImageImportDirectory++;
	}
	return i;
}


DWORD addFunListControlContent(HWND hwnd) {
	
}