#include "win_subSystem.h"

INT_PTR CALLBACK selectSubSystemWinProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		bFlag = 0;
		SetDlgItemText(hwnd, IDC_STATIC_SUBSYS, ptText);
		break;
	}
	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
		break;
	}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case IDC_BUTTON_UNKNOWN:
		{
			wcsprintf(ptText, TEXT("%04X"), IMAGE_SUBSYSTEM_UNKNOWN);
			SetDlgItemText(hwnd, IDC_STATIC_SUBSYS, ptText);
			break;
		}
		case IDC_BUTTON_NATIVE:
		{
			wcsprintf(ptText, TEXT("%04X"), IMAGE_SUBSYSTEM_NATIVE);
			SetDlgItemText(hwnd, IDC_STATIC_SUBSYS, ptText);
			break;
		}
		case IDC_BUTTON_GUI:
		{
			wcsprintf(ptText, TEXT("%04X"), IMAGE_SUBSYSTEM_WINDOWS_GUI);
			SetDlgItemText(hwnd, IDC_STATIC_SUBSYS, ptText);
			break;
		}
		case IDC_BUTTON_CUI:
		{
			wcsprintf(ptText, TEXT("%04X"), IMAGE_SUBSYSTEM_WINDOWS_CUI);
			SetDlgItemText(hwnd, IDC_STATIC_SUBSYS, ptText);
			break;
		}
		case IDC_BUTTON_OS2CUI:
		{
			wcsprintf(ptText, TEXT("%04X"), IMAGE_SUBSYSTEM_OS2_CUI);
			SetDlgItemText(hwnd, IDC_STATIC_SUBSYS, ptText);
			break;
		}
		case IDC_BUTTON_POSIXCUI:
		{
			wcsprintf(ptText, TEXT("%04X"), IMAGE_SUBSYSTEM_POSIX_CUI);
			SetDlgItemText(hwnd, IDC_STATIC_SUBSYS, ptText);
			break;
		}
		case IDC_BUTTON_WIN9X:
		{
			wcsprintf(ptText, TEXT("%04X"), IMAGE_SUBSYSTEM_NATIVE_WINDOWS);
			SetDlgItemText(hwnd, IDC_STATIC_SUBSYS, ptText);
			break;
		}
		case IDC_BUTTON_WINCE:
		{
			wcsprintf(ptText, TEXT("%04X"), IMAGE_SUBSYSTEM_WINDOWS_CE_GUI);
			SetDlgItemText(hwnd, IDC_STATIC_SUBSYS, ptText);
			break;
		}
		case IDC_BUTTON_EFIAPP:
		{
			wcsprintf(ptText, TEXT("%04X"), IMAGE_SUBSYSTEM_EFI_APPLICATION);
			SetDlgItemText(hwnd, IDC_STATIC_SUBSYS, ptText);
			break;
		}
		case IDC_BUTTON_EFI_BOOT:
		{
			wcsprintf(ptText, TEXT("%04X"), IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER);
			SetDlgItemText(hwnd, IDC_STATIC_SUBSYS, ptText);
			break;
		}
		case IDC_BUTTON_EFI_RUNTIME:
		{
			wcsprintf(ptText, TEXT("%04X"), IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER);
			SetDlgItemText(hwnd, IDC_STATIC_SUBSYS, ptText);
			break;
		}
		case IDC_BUTTON_EFI_ROM:
		{
			wcsprintf(ptText, TEXT("%04X"), IMAGE_SUBSYSTEM_EFI_ROM);
			SetDlgItemText(hwnd, IDC_STATIC_SUBSYS, ptText);
			break;
		}
		case IDC_BUTTON_XBOX:
		{
			wcsprintf(ptText, TEXT("%04X"), IMAGE_SUBSYSTEM_XBOX);
			SetDlgItemText(hwnd, IDC_STATIC_SUBSYS, ptText);
			break;
		}
		case IDC_BUTTON_WIN_BOOT:
		{
			wcsprintf(ptText, TEXT("%04X"), IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION);
			SetDlgItemText(hwnd, IDC_STATIC_SUBSYS, ptText);
			break;
		}
		case IDC_BUTTON_XBOX_CODE:
		{
			wcsprintf(ptText, TEXT("%04X"), IMAGE_SUBSYSTEM_XBOX_CODE_CATALOG);
			SetDlgItemText(hwnd, IDC_STATIC_SUBSYS, ptText);
			break;
		}
		case IDC_BUTTON_SUBSYSSAVE:
		{
			bFlag = 1;		//借用一下BOLL变量,用于决定是否修改
			EndDialog(hwnd, 0);
			break;
		}
		default:
		{
			return FALSE;
		}
		return TRUE;
		}
		break;
	}
	default:
		return FALSE;
		return TRUE;
	}
}


BOOL openSelectSubSystem(HWND hwnd) {
	GetDlgItemText(hwnd, IDC_EDIT_SUBSYS, ptText, sizeof ptText);
	DialogBox(hAPPInterface, MAKEINTRESOURCE(IDD_DIALOG_SUBSYS), hwnd, selectSubSystemWinProc);
	return bFlag;
}

