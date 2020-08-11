#include "win_peEdit.h"


INT_PTR CALLBACK peEditWinProc(
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

		addPEEditWinContent(hwnd, ptText);

		break;
	}
	case WM_CLOSE:
	{
		freePeFileBuff(pFileBuff);
		EndDialog(hwnd, 0);
		break;
	}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case IDC_BUTTON_PEMAGIC: {		//PE��־��ť����
			showWinOfOptionHeadMagic(hwnd);
			break;
		}
		case IDC_BUTTON_SUBSYS:			//��ϵͳ��ť����
		{
			if (openSelectSubSystem(hwnd))
			{
				SetDlgItemText(hwnd, IDC_EDIT_SUBSYS, ptText);
			}
			break;
		}
		case IDC_BUTTON_CHARACTER:		//��Ϣ��־��ť����
		{
			if (openSelCharacterWinOfFileHead(hwnd))
			{
				wcsprintf(ptText, TEXT("%x"), dwCharacterOfFileHead);
				SetDlgItemText(hwnd, IDC_EDIT_CHARACTER, ptText);
			}
			break;
		}
		case IDC_BUTTON_SECTION:
		{
			DialogBox(hAPPInterface, MAKEINTRESOURCE(IDD_DIALOG_SECTION), hwnd, WinProcOfSectionTable);
		}
		default:
			return FALSE;
			return TRUE;
		}
		break;
	}
	default:
		return FALSE;
		return TRUE;
	}
	return FALSE;
}

BOOL addPEEditWinContent(HWND hwnd, PTCHAR ptFileName) {
	DWORD dwFileSize;

	dwFileSize = loadPEFile(ptFileName, &pFileBuff);		//����PE�ļ���������
	if (pFileBuff == NULL)
		return FALSE;

	if (getPEHeader(pFileBuff, 0, &pImageFileHeader, &pImageOptionalHeader))	//��ȡPEͷ
	{
		//��ڵ�
		wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader->AddressOfEntryPoint);
		SetDlgItemText(hwnd, IDC_EDIT_ENTRY, ptText);

		//��ַ
		wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader->ImageBase);
		SetDlgItemText(hwnd, IDC_EDIT_BASE, ptText);

		//�ļ���С
		wcsprintf(ptText, TEXT("%04X"), dwFileSize);
		SetDlgItemText(hwnd, IDC_EDIT_FILESIZE, ptText);

		//�ڴ�ڿ����
		wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader->SectionAlignment);
		SetDlgItemText(hwnd, IDC_EDIT_MENALIG, ptText);

		//�ļ��ڿ����
		wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader->FileAlignment);
		SetDlgItemText(hwnd, IDC_EDIT_FILEALIG, ptText);

		//PE��־
		wcsprintf(ptText, TEXT("%04X"), pImageOptionalHeader->Magic);
		SetDlgItemText(hwnd, IDC_EDIT_PEMAGIC, ptText);

		//��ϵͳ
		wcsprintf(ptText, TEXT("%04X"), pImageOptionalHeader->Subsystem);
		SetDlgItemText(hwnd, IDC_EDIT_SUBSYS, ptText);

		//������Ŀ
		wcsprintf(ptText, TEXT("%04X"), pImageFileHeader->NumberOfSections);
		SetDlgItemText(hwnd, IDC_EDIT_SECTIONNUMBER, ptText);

		//�ļ�ʱ��
		wcsprintf(ptText, TEXT("%08X"), pImageFileHeader->TimeDateStamp);
		SetDlgItemText(hwnd, IDC_EDIT_FILETIME, ptText);

		//�ײ����ڱ��С
		wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader->SizeOfHeaders);
		SetDlgItemText(hwnd, IDC_EDIT_HEADSIZE, ptText);

		//��Ϣ��־
		wcsprintf(ptText, TEXT("%04X"), pImageFileHeader->Characteristics);
		SetDlgItemText(hwnd, IDC_EDIT_CHARACTER, ptText);

		//��ѡPEͷ��С
		wcsprintf(ptText, TEXT("%04X"), sizeof IMAGE_OPTIONAL_HEADER);
		SetDlgItemText(hwnd, IDC_EDIT_OPTIONSIZE, ptText);

	}
	return TRUE;
}

