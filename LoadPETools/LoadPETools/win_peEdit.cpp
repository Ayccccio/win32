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
		wcsprintf(ptTitle, TEXT("[ PE编辑器 ] - %s"), ptText);

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
		case IDC_BUTTON_PEMAGIC: {		//PE标志按钮单击
			showWinOfOptionHeadMagic(hwnd);
			break;
		}
		case IDC_BUTTON_SUBSYS:			//子系统按钮单击
		{
			if (openSelectSubSystem(hwnd))
			{
				SetDlgItemText(hwnd, IDC_EDIT_SUBSYS, ptText);
			}
			break;
		}
		case IDC_BUTTON_CHARACTER:		//信息标志按钮单击
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

	dwFileSize = loadPEFile(ptFileName, &pFileBuff);		//加载PE文件到缓冲区
	if (pFileBuff == NULL)
		return FALSE;

	if (getPEHeader(pFileBuff, 0, &pImageFileHeader, &pImageOptionalHeader))	//获取PE头
	{
		//入口点
		wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader->AddressOfEntryPoint);
		SetDlgItemText(hwnd, IDC_EDIT_ENTRY, ptText);

		//基址
		wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader->ImageBase);
		SetDlgItemText(hwnd, IDC_EDIT_BASE, ptText);

		//文件大小
		wcsprintf(ptText, TEXT("%04X"), dwFileSize);
		SetDlgItemText(hwnd, IDC_EDIT_FILESIZE, ptText);

		//内存节块对齐
		wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader->SectionAlignment);
		SetDlgItemText(hwnd, IDC_EDIT_MENALIG, ptText);

		//文件节块对齐
		wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader->FileAlignment);
		SetDlgItemText(hwnd, IDC_EDIT_FILEALIG, ptText);

		//PE标志
		wcsprintf(ptText, TEXT("%04X"), pImageOptionalHeader->Magic);
		SetDlgItemText(hwnd, IDC_EDIT_PEMAGIC, ptText);

		//子系统
		wcsprintf(ptText, TEXT("%04X"), pImageOptionalHeader->Subsystem);
		SetDlgItemText(hwnd, IDC_EDIT_SUBSYS, ptText);

		//区段数目
		wcsprintf(ptText, TEXT("%04X"), pImageFileHeader->NumberOfSections);
		SetDlgItemText(hwnd, IDC_EDIT_SECTIONNUMBER, ptText);

		//文件时间
		wcsprintf(ptText, TEXT("%08X"), pImageFileHeader->TimeDateStamp);
		SetDlgItemText(hwnd, IDC_EDIT_FILETIME, ptText);

		//首部及节表大小
		wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader->SizeOfHeaders);
		SetDlgItemText(hwnd, IDC_EDIT_HEADSIZE, ptText);

		//信息标志
		wcsprintf(ptText, TEXT("%04X"), pImageFileHeader->Characteristics);
		SetDlgItemText(hwnd, IDC_EDIT_CHARACTER, ptText);

		//可选PE头大小
		wcsprintf(ptText, TEXT("%04X"), sizeof IMAGE_OPTIONAL_HEADER);
		SetDlgItemText(hwnd, IDC_EDIT_OPTIONSIZE, ptText);

	}
	return TRUE;
}

