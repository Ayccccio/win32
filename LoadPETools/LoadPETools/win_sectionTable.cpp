#include "win_sectionTable.h"

INT_PTR CALLBACK WinProcOfSectionTable(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam) 
{
	WORD wColumnWidths[SectionListControlColumNumber] = { 100,100,100,100,100,100 };
	HWND hListControl = NULL;

	switch (uMsg)
	{
	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
		break;
	}
	case WM_INITDIALOG: 
	{
		//��ȡ�б�ͨ�ÿؼ����
		hListControl = GetDlgItem(hwnd, IDC_LIST_SECTION);

		//���õ���б�ͨ�öԻ���ѡ������
		SendMessage(hListControl, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

		//��ʼ���б�ͨ�ÿؼ���ͷ
		initListControlHeader(hListControl, SectionListControlColumNumber, (PTCHAR)TEXT("����\0RVA\0VSize\0FOA\0FSize\0����"), wColumnWidths);

		//�����б�ͨ�ÿؼ�����
		addListControlOfSection(hListControl);
		break;
	}
	default:
		return FALSE;
	}
	return TRUE;
}


DWORD addListControlOfSection(HWND hwListControl) {
	PIMAGE_SECTION_HEADER pImageSecitonHead = NULL;
	int i = 1;
	size_t num = 0;
	LVITEM lv = {0};
	PTCHAR ptTemp = ptText;
	lv.mask = LVIF_TEXT;


	if (getPESectionHeader(pFileBuff, pImageSecitonHead))
	{
		while (i <= pImageFileHeader->NumberOfSections)
		{
			lv.iItem = i - 1;
			//1.����
			memset(ptText, 0, sizeof ptText);
			getSectionName(pImageSecitonHead, (PTCHAR*)&ptTemp, sizeof ptText);
			//wcsprintf(ptText, TEXT("%s"), pImageSecitonHead->Name);
			lv.pszText = ptText;
			lv.iSubItem = 0;
			ListView_InsertItem(hwListControl,&lv);

			//2.RVA
			wcsprintf(ptText, TEXT("%08X"), pImageSecitonHead->VirtualAddress);
			lv.pszText = ptText;
			lv.iSubItem = 1;
			ListView_SetItem(hwListControl, &lv);

			//3.Rva size
			wcsprintf(ptText, TEXT("%08X"), pImageSecitonHead->Misc.VirtualSize);
			lv.pszText = ptText;
			lv.iSubItem = 2;
			ListView_SetItem(hwListControl, &lv);

			//4.Foa
			wcsprintf(ptText, TEXT("%08X"), pImageSecitonHead->PointerToRawData);
			lv.pszText = ptText;
			lv.iSubItem = 3;
			ListView_SetItem(hwListControl, &lv);

			//5.Foa size
			wcsprintf(ptText, TEXT("%08X"), pImageSecitonHead->SizeOfRawData);
			lv.pszText = ptText;
			lv.iSubItem = 4;
			ListView_SetItem(hwListControl, &lv);

			//6.characteristics
			wcsprintf(ptText, TEXT("%08X"), pImageSecitonHead->Characteristics);
			lv.pszText = ptText;
			lv.iSubItem = 5;
			ListView_SetItem(hwListControl, &lv);

			pImageSecitonHead = (PIMAGE_SECTION_HEADER)((ADWORD)pImageSecitonHead +  IMAGE_SIZEOF_SECTION_HEADER);
			i++;
		}
	}
	return i - 1;
}