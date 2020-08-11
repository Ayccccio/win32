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
		//��ʼ���б�ͨ�ÿؼ���ͷ
		initListControlHeader(GetDlgItem(hwnd, IDC_LIST_SECTION), SectionListControlColumNumber, (PTCHAR)TEXT("����\0RVA\0VSize\0FOA\0FSize\0����"), wColumnWidths);

		//����б�ͨ�ÿؼ�����

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
			//1.����
			wcsprintf(ptText, TEXT("%s"), pImageSecitonHead->Name);
			pImageSecitonHead = (PIMAGE_SECTION_HEADER)((ADWORD)pImageFileHeader + i * IMAGE_SIZEOF_SECTION_HEADER);
			i++;
		}
	}
	
}