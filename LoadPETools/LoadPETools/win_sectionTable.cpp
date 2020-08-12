#include "win_sectionTable.h"

INT_PTR CALLBACK WinProcOfSectionTable(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam) 
{
	WORD wColumnWidths[SectionListControlColumNumber] = { 100,100,100,100,100,100 };

	switch (uMsg)
	{
	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
		break;
	}
	case WM_INITDIALOG: 
	{
		//初始化列表通用控件表头
		initListControlHeader(GetDlgItem(hwnd, IDC_LIST_SECTION), SectionListControlColumNumber, (PTCHAR)TEXT("名称\0RVA\0VSize\0FOA\0FSize\0特征"), wColumnWidths);

		//添加列表通用控件内容
		addListControlOfSection(GetDlgItem(hwnd, IDC_LIST_SECTION));
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
	lv.mask = LVIF_TEXT;


	if (getPESectionHeader(pFileBuff, pImageSecitonHead))
	{
		while (i <= pImageFileHeader->NumberOfSections)
		{
			//1.名称
			memset(ptText, 0, sizeof ptText);
			getSectionName(pImageSecitonHead, (PTCHAR*)&ptText[0], sizeof ptText);
			wcsprintf(ptText, TEXT("%s"), pImageSecitonHead->Name);
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

			pImageSecitonHead = (PIMAGE_SECTION_HEADER)((ADWORD)pImageFileHeader + i * IMAGE_SIZEOF_SECTION_HEADER);
			i++;
		}
	}
	return i - 1;
}