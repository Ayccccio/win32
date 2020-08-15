#include "win_peEdit.h"


INT_PTR CALLBACK winProcPEEdit(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	TCHAR ptTitle[512] = { 0 };
	HWND hPEDialog;

	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		wcsprintf(ptTitle, TEXT("[ PE�༭�� ] - %s"), ptText);

		SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)ptTitle);

		if (!addPEEditWinContent(hwnd, ptText))
		{
			SendMessage(hwnd, WM_CLOSE, 0, 0);
		}

		return TRUE;
	}
	case WM_CLOSE:
	{
		freePeFileBuff(pFileBuff);
		EndDialog(hwnd, 0);
		return TRUE;
	}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case IDC_BUTTON_PEMAGIC: {		//PE��־��ť����
			showWinOfOptionHeadMagic(hwnd);
			return TRUE;
		}
		case IDC_BUTTON_SUBSYS:			//��ϵͳ��ť����
		{
			if (openSelectSubSystem(hwnd))
			{
				SetDlgItemText(hwnd, IDC_EDIT_SUBSYS, ptText);
			}
			return TRUE;
		}
		case IDC_BUTTON_CHARACTER:		//��Ϣ��־��ť����
		{
			if (openSelCharacterWinOfFileHead(hwnd))
			{
				wcsprintf(ptText, TEXT("%x"), dwCharacterOfFileHead);
				SetDlgItemText(hwnd, IDC_EDIT_CHARACTER, ptText);
			}
			return TRUE;
		}
		case IDC_BUTTON_SECTION:		//���ΰ�ť����
		{
			DialogBox(hAPPInterface, MAKEINTRESOURCE(IDD_DIALOG_SECTION), hwnd, winProcOfSectionTable);
			return TRUE;
		}
		case IDC_BUTTON_DIRECTORY:
		{
			DialogBox(hAPPInterface, MAKEINTRESOURCE(IDD_DIALOG_DIRECTORY), hwnd, winProcOfDirectory);
			return TRUE;
		}
		}
	}
	}
	return FALSE;
}

BOOL addPEEditWinContent(HWND hwnd, PTCHAR ptFileName) {
	DWORD dwFileSize;

	dwFileSize = loadPEFile(ptFileName, &pFileBuff);		//����PE�ļ���������
	if (pFileBuff == NULL)
		return FALSE;


	if (getPEHeader(pFileBuff, 0, &pImageFileHeader, (PIMAGE_OPTIONAL_HEADER*)&pImageOptionalHeader32))	//��ȡPEͷ
	{
		pImageOptionalHeader64 = (PIMAGE_OPTIONAL_HEADER64)pImageOptionalHeader32;

		if (pImageOptionalHeader32->Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
		{
			//��ڵ�
			wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader32->AddressOfEntryPoint);
			SetDlgItemText(hwnd, IDC_EDIT_ENTRY, ptText);

			//��ַ
			wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader32->ImageBase);
			SetDlgItemText(hwnd, IDC_EDIT_BASE, ptText);

			//�ļ���С
			wcsprintf(ptText, TEXT("%04X"), dwFileSize);
			SetDlgItemText(hwnd, IDC_EDIT_FILESIZE, ptText);

			//�ڴ�ڿ����
			wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader32->SectionAlignment);
			SetDlgItemText(hwnd, IDC_EDIT_MENALIG, ptText);

			//�ļ��ڿ����
			wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader32->FileAlignment);
			SetDlgItemText(hwnd, IDC_EDIT_FILEALIG, ptText);

			//PE��־
			wcsprintf(ptText, TEXT("%04X"), pImageOptionalHeader32->Magic);
			SetDlgItemText(hwnd, IDC_EDIT_PEMAGIC, ptText);

			//��ϵͳ
			wcsprintf(ptText, TEXT("%04X"), pImageOptionalHeader32->Subsystem);
			SetDlgItemText(hwnd, IDC_EDIT_SUBSYS, ptText);

			//�ײ����ڱ��С
			wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader32->SizeOfHeaders);
			SetDlgItemText(hwnd, IDC_EDIT_HEADSIZE, ptText);

			//��ѡPEͷ��С
			wcsprintf(ptText, TEXT("%04X"), sizeof IMAGE_OPTIONAL_HEADER32);
			SetDlgItemText(hwnd, IDC_EDIT_OPTIONSIZE, ptText);

		}
		else {
			//��ڵ�
			wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader64->AddressOfEntryPoint);
			SetDlgItemText(hwnd, IDC_EDIT_ENTRY, ptText);

			//��ַ
			wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader64->ImageBase);
			SetDlgItemText(hwnd, IDC_EDIT_BASE, ptText);

			//�ļ���С
			wcsprintf(ptText, TEXT("%04X"), dwFileSize);
			SetDlgItemText(hwnd, IDC_EDIT_FILESIZE, ptText);

			//�ڴ�ڿ����
			wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader64->SectionAlignment);
			SetDlgItemText(hwnd, IDC_EDIT_MENALIG, ptText);

			//�ļ��ڿ����
			wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader64->FileAlignment);
			SetDlgItemText(hwnd, IDC_EDIT_FILEALIG, ptText);

			//PE��־
			wcsprintf(ptText, TEXT("%04X"), pImageOptionalHeader64->Magic);
			SetDlgItemText(hwnd, IDC_EDIT_PEMAGIC, ptText);

			//��ϵͳ
			wcsprintf(ptText, TEXT("%04X"), pImageOptionalHeader64->Subsystem);
			SetDlgItemText(hwnd, IDC_EDIT_SUBSYS, ptText);

			//�ײ����ڱ��С
			wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader64->SizeOfHeaders);
			SetDlgItemText(hwnd, IDC_EDIT_HEADSIZE, ptText);

			//��ѡPEͷ��С
			wcsprintf(ptText, TEXT("%04X"), sizeof IMAGE_OPTIONAL_HEADER32);
			SetDlgItemText(hwnd, IDC_EDIT_OPTIONSIZE, ptText);
		}

		//������Ŀ
		wcsprintf(ptText, TEXT("%04X"), pImageFileHeader->NumberOfSections);
		SetDlgItemText(hwnd, IDC_EDIT_SECTIONNUMBER, ptText);

		//�ļ�ʱ��
		wcsprintf(ptText, TEXT("%08X"), pImageFileHeader->TimeDateStamp);
		SetDlgItemText(hwnd, IDC_EDIT_FILETIME, ptText);

		//��Ϣ��־
		wcsprintf(ptText, TEXT("%04X"), pImageFileHeader->Characteristics);
		SetDlgItemText(hwnd, IDC_EDIT_CHARACTER, ptText);
	}
	return TRUE;
}

