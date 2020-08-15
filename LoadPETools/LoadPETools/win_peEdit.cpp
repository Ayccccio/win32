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
		wcsprintf(ptTitle, TEXT("[ PE编辑器 ] - %s"), ptText);

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
		case IDC_BUTTON_PEMAGIC: {		//PE标志按钮单击
			showWinOfOptionHeadMagic(hwnd);
			return TRUE;
		}
		case IDC_BUTTON_SUBSYS:			//子系统按钮单击
		{
			if (openSelectSubSystem(hwnd))
			{
				SetDlgItemText(hwnd, IDC_EDIT_SUBSYS, ptText);
			}
			return TRUE;
		}
		case IDC_BUTTON_CHARACTER:		//信息标志按钮单击
		{
			if (openSelCharacterWinOfFileHead(hwnd))
			{
				wcsprintf(ptText, TEXT("%x"), dwCharacterOfFileHead);
				SetDlgItemText(hwnd, IDC_EDIT_CHARACTER, ptText);
			}
			return TRUE;
		}
		case IDC_BUTTON_SECTION:		//区段按钮单击
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

	dwFileSize = loadPEFile(ptFileName, &pFileBuff);		//加载PE文件到缓冲区
	if (pFileBuff == NULL)
		return FALSE;


	if (getPEHeader(pFileBuff, 0, &pImageFileHeader, (PIMAGE_OPTIONAL_HEADER*)&pImageOptionalHeader32))	//获取PE头
	{
		pImageOptionalHeader64 = (PIMAGE_OPTIONAL_HEADER64)pImageOptionalHeader32;

		if (pImageOptionalHeader32->Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
		{
			//入口点
			wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader32->AddressOfEntryPoint);
			SetDlgItemText(hwnd, IDC_EDIT_ENTRY, ptText);

			//基址
			wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader32->ImageBase);
			SetDlgItemText(hwnd, IDC_EDIT_BASE, ptText);

			//文件大小
			wcsprintf(ptText, TEXT("%04X"), dwFileSize);
			SetDlgItemText(hwnd, IDC_EDIT_FILESIZE, ptText);

			//内存节块对齐
			wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader32->SectionAlignment);
			SetDlgItemText(hwnd, IDC_EDIT_MENALIG, ptText);

			//文件节块对齐
			wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader32->FileAlignment);
			SetDlgItemText(hwnd, IDC_EDIT_FILEALIG, ptText);

			//PE标志
			wcsprintf(ptText, TEXT("%04X"), pImageOptionalHeader32->Magic);
			SetDlgItemText(hwnd, IDC_EDIT_PEMAGIC, ptText);

			//子系统
			wcsprintf(ptText, TEXT("%04X"), pImageOptionalHeader32->Subsystem);
			SetDlgItemText(hwnd, IDC_EDIT_SUBSYS, ptText);

			//首部及节表大小
			wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader32->SizeOfHeaders);
			SetDlgItemText(hwnd, IDC_EDIT_HEADSIZE, ptText);

			//可选PE头大小
			wcsprintf(ptText, TEXT("%04X"), sizeof IMAGE_OPTIONAL_HEADER32);
			SetDlgItemText(hwnd, IDC_EDIT_OPTIONSIZE, ptText);

		}
		else {
			//入口点
			wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader64->AddressOfEntryPoint);
			SetDlgItemText(hwnd, IDC_EDIT_ENTRY, ptText);

			//基址
			wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader64->ImageBase);
			SetDlgItemText(hwnd, IDC_EDIT_BASE, ptText);

			//文件大小
			wcsprintf(ptText, TEXT("%04X"), dwFileSize);
			SetDlgItemText(hwnd, IDC_EDIT_FILESIZE, ptText);

			//内存节块对齐
			wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader64->SectionAlignment);
			SetDlgItemText(hwnd, IDC_EDIT_MENALIG, ptText);

			//文件节块对齐
			wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader64->FileAlignment);
			SetDlgItemText(hwnd, IDC_EDIT_FILEALIG, ptText);

			//PE标志
			wcsprintf(ptText, TEXT("%04X"), pImageOptionalHeader64->Magic);
			SetDlgItemText(hwnd, IDC_EDIT_PEMAGIC, ptText);

			//子系统
			wcsprintf(ptText, TEXT("%04X"), pImageOptionalHeader64->Subsystem);
			SetDlgItemText(hwnd, IDC_EDIT_SUBSYS, ptText);

			//首部及节表大小
			wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader64->SizeOfHeaders);
			SetDlgItemText(hwnd, IDC_EDIT_HEADSIZE, ptText);

			//可选PE头大小
			wcsprintf(ptText, TEXT("%04X"), sizeof IMAGE_OPTIONAL_HEADER32);
			SetDlgItemText(hwnd, IDC_EDIT_OPTIONSIZE, ptText);
		}

		//区段数目
		wcsprintf(ptText, TEXT("%04X"), pImageFileHeader->NumberOfSections);
		SetDlgItemText(hwnd, IDC_EDIT_SECTIONNUMBER, ptText);

		//文件时间
		wcsprintf(ptText, TEXT("%08X"), pImageFileHeader->TimeDateStamp);
		SetDlgItemText(hwnd, IDC_EDIT_FILETIME, ptText);

		//信息标志
		wcsprintf(ptText, TEXT("%04X"), pImageFileHeader->Characteristics);
		SetDlgItemText(hwnd, IDC_EDIT_CHARACTER, ptText);
	}
	return TRUE;
}

