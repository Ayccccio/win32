#include "win_fileHeadCharacter.h"

INT_PTR CALLBACK selectCharacterOfFileHeadWinProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	//复选框ID
	WORD pwIndex[] = { IDC_CHECK_RELOCS ,IDC_CHECK_IMAGE ,IDC_CHECK_LINE ,IDC_CHECK_LOCAL ,
						IDC_CHECK_WS ,IDC_CHECK_LARGE ,0,IDC_CHECK_BYTES ,
						IDC_CHECK_32BIT, IDC_CHECK_DEBUG ,IDC_CHECK_REMOVABLE ,IDC_CHECK_NET,
						IDC_CHECK_SYSTEM ,IDC_CHECK_DLL, IDC_CHECK_ONLY_SYSTEM ,IDC_CHECK_REVERSEN_HI };
	pwCheckBoxIndexOfFileCharacter = pwIndex;

	int i = 1;
	DWORD dwCheck = 0;

	switch (uMsg)
	{
	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
		break;
	}
	case WM_INITDIALOG:
	{
		bFlag = 0;

		//设置静态标签当前值
		SetDlgItemText(hwnd, IDC_STATIC_CHARACTER, ptText);

		swcscanf_s(ptText, TEXT("%x"), &dwCharacterOfFileHead);


		//遍历wCharacter取每个二进制位的值
		while (i < 17)
		{
			if (i == 7)
			{
				i++;
				continue;
			}
			if (getBitOfIndex(dwCharacterOfFileHead, i))
			{
				SendMessage(GetDlgItem(hwnd, pwIndex[i - 1]), BM_SETCHECK, BST_CHECKED, NULL);
			}
			i++;
		}
		break;
	}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case IDC_BUTTON_CHARSAVE:
		{
			bFlag = 1;
			EndDialog(hwnd, 0);
			break;
		}
		case IDC_CHECK_RELOCS:
		{
			setCharacterOfFileHead(hwnd, 1, &dwCharacterOfFileHead);
			break;
		}
		case IDC_CHECK_IMAGE:
		{
			setCharacterOfFileHead(hwnd, 2, &dwCharacterOfFileHead);
			break;
		}
		case IDC_CHECK_LINE:
		{
			setCharacterOfFileHead(hwnd, 3, &dwCharacterOfFileHead);
			break;
		}
		case IDC_CHECK_LOCAL:
		{
			setCharacterOfFileHead(hwnd, 4, &dwCharacterOfFileHead);
			break;
		}
		case IDC_CHECK_WS:
		{
			setCharacterOfFileHead(hwnd, 5, &dwCharacterOfFileHead);
			break;
		}
		case IDC_CHECK_LARGE:
		{
			setCharacterOfFileHead(hwnd, 6, &dwCharacterOfFileHead);
			break;
		}
		case IDC_CHECK_BYTES:
		{
			setCharacterOfFileHead(hwnd, 8, &dwCharacterOfFileHead);
			break;
		}
		case IDC_CHECK_32BIT:
		{
			setCharacterOfFileHead(hwnd, 9, &dwCharacterOfFileHead);
			break;
		}
		case IDC_CHECK_DEBUG:
		{
			setCharacterOfFileHead(hwnd, 10, &dwCharacterOfFileHead);
			break;
		}
		case IDC_CHECK_REMOVABLE:
		{
			setCharacterOfFileHead(hwnd, 11, &dwCharacterOfFileHead);
			break;
		}
		case IDC_CHECK_NET:
		{
			setCharacterOfFileHead(hwnd, 12, &dwCharacterOfFileHead);
			break;
		}
		case IDC_CHECK_SYSTEM:
		{
			setCharacterOfFileHead(hwnd, 13, &dwCharacterOfFileHead);
			break;
		}
		case IDC_CHECK_DLL:
		{
			setCharacterOfFileHead(hwnd, 14, &dwCharacterOfFileHead);
			break;
		}
		case IDC_CHECK_ONLY_SYSTEM:
		{
			setCharacterOfFileHead(hwnd, 15, &dwCharacterOfFileHead);
			break;
		}
		case IDC_CHECK_REVERSEN_HI:
		{
			setCharacterOfFileHead(hwnd, 16, &dwCharacterOfFileHead);
			break;
		}
		default:
			return FALSE;
			return TRUE;
		}
		break;
	}
	default:
		return FALSE;
	}
	return TRUE;
}


BOOL openSelCharacterWinOfFileHead(HWND hwnd)
{
	GetDlgItemText(hwnd, IDC_EDIT_CHARACTER, ptText, sizeof ptText);
	DialogBox(hAPPInterface, MAKEINTRESOURCE(IDD_DIALOG_CHARACTER), hwnd, selectCharacterOfFileHeadWinProc);
	return bFlag;
}

DWORD setCharacterOfFileHead(HWND hwnd, int index, PDWORD pdCharacterValue) {
	if (Button_GetCheck(GetDlgItem(hwnd, *(pwCheckBoxIndexOfFileCharacter + index - 1))))
	{
		setBitOfIndex(&dwCharacterOfFileHead, index, 1);
	}
	else {
		setBitOfIndex(&dwCharacterOfFileHead, index, 0);
	}
	wcsprintf(ptText, TEXT("%x"), dwCharacterOfFileHead);
	DbgPrintf(TEXT("%s"), ptText);
	SetDlgItemText(hwnd, IDC_STATIC_CHARACTER, ptText);
	return dwCharacterOfFileHead;
}

