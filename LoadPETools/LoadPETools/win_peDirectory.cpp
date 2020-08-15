#include "win_peDirectory.h"

INT_PTR CALLBACK winProcOfDirectory(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam) {
	switch (uMsg)
	{
	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
		return TRUE;
	}
	case WM_INITDIALOG:
	{
		addDirectory(hwnd);
		return TRUE;
	}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case IDC_BUTTON_EXPROT:
		{
			DialogBox(hAPPInterface, MAKEINTRESOURCE(IDD_DIALOG_EXPORT), hwnd, winProcOfExport);
			return TRUE;
		}
		case IDC_BUTTON_IMPORT:
		{
			DialogBox(hAPPInterface, MAKEINTRESOURCE(IDD_DIALOG_IMPORT), hwnd, winProcOfImport);
			return TRUE;
		}
		}
	}
	}
	return FALSE;
}


DWORD addDirectory(HWND hwnd) {
	WORD wItem[16][3] = {
		{IDC_EDIT_EXPROT_RVA,IDC_EDIT_EXPROT_FOA,IDC_EDIT_EXPROT_SIZE},
		{IDC_EDIT_IMPORT_RVA,IDC_EDIT_IMPORT_FOA,IDC_EDIT_IMPORT_SIZE},
		{IDC_EDIT_RESOURCE_RVA,IDC_EDIT_RESOURCE_FOA,IDC_EDIT_RESOURCE_SIZE},
		{IDC_EDIT_EXCEPTION_RVA,IDC_EDIT_EXCEPTION_FOA,IDC_EDIT_EXCEPTION_SIZE},
		{IDC_EDIT_SECURITY_RVA,IDC_EDIT_SECURITY_FOA,IDC_EDIT_SECURITY_SIZE},
		{IDC_EDIT_BASERELOC_RVA,IDC_EDIT_BASERELOC_FOA,IDC_EDIT_BASERELOC_SIZE},
		{IDC_EDIT_DEBUG_RVA,IDC_EDIT_DEBUG_FOA,IDC_EDIT_DEBUG_SIZE},
		{IDC_EDIT_COPYRIGHT_RVA,IDC_EDIT_COPYRIGHT_FOA,IDC_EDIT_COPYRIGHT_SIZE},
		{IDC_EDIT_GLOBALPTR_RVA,IDC_EDIT_GLOBALPTR_FOA,IDC_EDIT_GLOBALPTR_SIZE},
		{IDC_EDIT_TLS_RVA,IDC_EDIT_TLS_FOA,IDC_EDIT_TLS_SIZE},
		{IDC_EDIT_CONFIG_RVA,IDC_EDIT_CONFIG_FOA,IDC_EDIT_CONFIG_SIZE},
		{IDC_EDIT_BOUND_RVA,IDC_EDIT_BOUND_FOA,IDC_EDIT_BOUND_SIZE},
		{IDC_EDIT_IAT_RVA,IDC_EDIT_IAT_FOA,IDC_EDIT_IAT_SIZE},
		{IDC_EDIT_DELAY_RVA,IDC_EDIT_DELAY_FOA,IDC_EDIT_DELAY_SIZE},
		{IDC_EDIT_COM_RVA,IDC_EDIT_COM_FOA,IDC_EDIT_COM_SIZE},
		{IDC_EDIT_RETAIN_RVA,IDC_EDIT_RETAIN_FOA,IDC_EDIT_RETAIN_SIZE}
	};
	int i = 0;
	if (pImageOptionalHeader32->Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC)
	{
		while (i < pImageOptionalHeader32->NumberOfRvaAndSizes)
		{
			//RVA
			wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader32->DataDirectory[i].VirtualAddress);
			SetDlgItemText(hwnd, wItem[i][0], ptText);

			//FOA
			wcsprintf(ptText, TEXT("%08X"), rvaToFoa(pFileBuff, pImageOptionalHeader32->DataDirectory[i].VirtualAddress));
			SetDlgItemText(hwnd, wItem[i][1], ptText);

			//size
			wcsprintf(ptText, TEXT("%08X"), rvaToFoa(pFileBuff, pImageOptionalHeader32->DataDirectory[i].Size));
			SetDlgItemText(hwnd, wItem[i][2], ptText);
			i++;
		}
	}
	else {
		while (i < pImageOptionalHeader64->NumberOfRvaAndSizes)
		{
			//RVA
			wcsprintf(ptText, TEXT("%08X"), pImageOptionalHeader64->DataDirectory[i].VirtualAddress);
			SetDlgItemText(hwnd, wItem[i][0], ptText);

			//FOA
			wcsprintf(ptText, TEXT("%08X"), rvaToFoa(pFileBuff, pImageOptionalHeader64->DataDirectory[i].VirtualAddress));
			SetDlgItemText(hwnd, wItem[i][1], ptText);

			//size
			wcsprintf(ptText, TEXT("%08X"), rvaToFoa(pFileBuff, pImageOptionalHeader64->DataDirectory[i].Size));
			SetDlgItemText(hwnd, wItem[i][2], ptText);
			i++;
		}
	}


	return i * 3;
}