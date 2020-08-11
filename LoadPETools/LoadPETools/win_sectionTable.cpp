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

		break;
	}
	default:
		return FALSE;
	}
	return TRUE;
}


DWORD addListControlOfSection(HWND hwnd) {
	PIMAGE_SECTION_HEADER pImageSecitonHead = NULL;
	int i = 1;

	if (getPESectionHeader(pFileBuff, pImageSecitonHead))
	{
		while (i <= pImageFileHeader->NumberOfSections)
		{
			//1.名称
			wcsprintf(ptText, TEXT("%s"), pImageSecitonHead->Name);
			pImageSecitonHead = (PIMAGE_SECTION_HEADER)((ADWORD)pImageFileHeader + i * IMAGE_SIZEOF_SECTION_HEADER);
			i++;
		}
	}
	
}