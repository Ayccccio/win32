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
		//��ȡ�б�ͨ�ÿؼ����
		hListControl = GetDlgItem(hwnd, IDC_LIST_EXPROT);

		//�����б�ؼ�����ѡ��
		SendMessage(hListControl, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

		//��ʼ���б�ͨ�ÿؼ�ͷ��
		initListControlHeader(hListControl, ExportListControlColumNumber, (PTCHAR)TEXT("�������\0RVA\0FOA\0������"), pwListWidths);

		//��ӵ�����༭������
		addExportEditContent(hwnd);

		//����б�ͨ�ÿؼ�����
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

	//��ȡ������
	pImageExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((ADWORD)pFileBuff + rvaToFoa(pFileBuff, pImageOptionalHeader->DataDirectory[0].VirtualAddress));

	//����
	wcsprintf(ptText, TEXT("%08X"), pImageExportDirectory->Characteristics);
	SetDlgItemText(hwnd, IDC_EDIT_EXPORT_CHARACTER,ptText);

	//ʱ���
	wcsprintf(ptText, TEXT("%08X"), pImageExportDirectory->TimeDateStamp);
	SetDlgItemText(hwnd, IDC_EDIT_EXPORT_TIME, ptText);

	//�������
	wcsprintf(ptText, TEXT("%08X"), pImageExportDirectory->Base);
	SetDlgItemText(hwnd, IDC_EDIT_EXPORT_BASE, ptText);

	//���Ƶ�ַ
	wcsprintf(ptText, TEXT("%08X"), pImageExportDirectory->Name);
	SetDlgItemText(hwnd, IDC_EDIT_EXPORT_NAME_ADDR, ptText);

	//��������
	wcsprintf(ptText, TEXT("%S"), rvaToFoa(pFileBuff,pImageExportDirectory->Name) + (ADWORD)pFileBuff);
	SetDlgItemText(hwnd, IDC_EDIT_EXPORT_NAME, ptText);

	//��������
	wcsprintf(ptText, TEXT("%08X"), pImageExportDirectory->NumberOfFunctions);
	SetDlgItemText(hwnd, IDC_EDIT_EXPORT_FNNUM, ptText);

	//����������
	wcsprintf(ptText, TEXT("%08X"), pImageExportDirectory->NumberOfNames);
	SetDlgItemText(hwnd, IDC_EDIT_EXPORT_FNNAMENUM, ptText);

	//������ַ
	wcsprintf(ptText, TEXT("%08X"), pImageExportDirectory->AddressOfFunctions);
	SetDlgItemText(hwnd, IDC_EDIT_EXPORT_FN_ADDR, ptText);

	//��������ַ
	wcsprintf(ptText, TEXT("%08X"), pImageExportDirectory->AddressOfNames);
	SetDlgItemText(hwnd, IDC_EDIT_EXPORT_FNNAME_ADDR, ptText);

	//��������ŵ�ַ
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


	//����������ַ
	while (i < pImageExportDirectory->NumberOfFunctions)
	{
		pFunName = NULL;
		j = 0;

		//����������ű�
		while (j < pImageExportDirectory->NumberOfNames)
		{
			//�ж���ű����Ƿ��е�ǰ�������
			if (*(pdwFunNameOrd + j) == i)
			{
				pFunName = (PCHAR) (rvaToFoa(pFileBuff, *(pdwFunName + i)) + (ADWORD)pFileBuff);
				break;
			}
			j++;
		}

		lv.iItem = i;
		//1.�������
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
			//4.������
			MultiByteToWideChar(CP_UTF8, 0, pFunName, -1, ptText, sizeof ptText);
			lv.pszText = ptText;
			lv.iSubItem = 3;
			ListView_SetItem(hwnd, &lv);
		}
		i++;
	}

}