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
		//��ȡͨ�ÿؼ����
		hDllListControl = GetDlgItem(hwnd, IDC_LIST_IMPORT_DLL);
		hFunListControl = GetDlgItem(hwnd, IDC_LIST_IMPORT_FUN);

		//�����б�ؼ�����ѡ��
		SendMessage(hDllListControl, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
		SendMessage(hFunListControl, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

		//��ʼ���б�ͨ�ÿؼ���ͷ
		initListControlHeader(hDllListControl, ImportDllListControlColumNumber, (PTCHAR)TEXT("DLL����\0INT\0IAT\0ʱ���\0DLL����\0����"), pwDllWidths);
		initListControlHeader(hDllListControl, ImportFunListControlColumNumber, (PTCHAR)TEXT("RVA\0FOA\0VALUE\0HINT\0������"), pwFunWidths);

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
	pImageImport = (PIMAGE_IMPORT_DESCRIPTOR)(rvaToFoa(pFileBuff, pImageOptionalHeader->DataDirectory[1].VirtualAddress) + (ADWORD)pFileBuff);

	//���������
	while (pImageImport->Name)
	{
		lv.iItem = i;

		//0.����
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

		//3.ʱ���
		wcsprintf(ptText, TEXT("%08X"), pImageImport->TimeDateStamp);
		lv.iSubItem = 3;
		ListView_SetItem(hwnd, &lv);

		//4.DLL��������
		wcsprintf(ptText, TEXT("%08X"), pImageImport->ForwarderChain);
		lv.iSubItem = 4;
		ListView_SetItem(hwnd, &lv);

		//5.����
		wcsprintf(ptText, TEXT("%08X"), pImageImport->Characteristics);
		lv.iSubItem = 5;
		ListView_SetItem(hwnd, &lv);

		i++;
		pImageImport++;
	}
	return i;
}

