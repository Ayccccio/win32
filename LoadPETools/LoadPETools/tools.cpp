#include "tools.h"

//PE��־������Ϣ�ص�����
INT_PTR CALLBACK peMagicWinProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam) {
	switch (uMsg)
	{
	case WM_CLOSE:
	{
		EndDialog(hwnd,0);
		return TRUE;
	}
		
	}
	return FALSE;
}

//ѡ����ϵͳ������Ϣ�ص�����
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
			bFlag = 1;		//����һ��BOLL����,���ھ����Ƿ��޸�
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


DWORD initListControlHeader(HWND hListControl,DWORD dwLenth,PTCHAR ptColumNames,PWORD pdColWidths) {
	LVCOLUMN lvCol = { 0 };
	int i = 0;
	PTCHAR ptTemp = ptColumNames;

	lvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCFMT_RIGHT;

	while (i < dwLenth)
	{
		lvCol.pszText = ptTemp;
		lvCol.cx = pdColWidths[i];
		lvCol.iSubItem = i;
		ListView_InsertColumn(hListControl,i, &lvCol);
		ptTemp += StrLen(ptTemp) + 1;
		i++;
	}
	return i;
}

BOOL processTokenUp(HANDLE processHandle,LPCWSTR privilege) {
	//��Ȩ
	HANDLE handle;
	LUID lUid;
	TOKEN_PRIVILEGES tkp;
	if (OpenProcessToken(processHandle, TOKEN_ALL_ACCESS, &handle))		//��ȡ���̵����ƾ��
	{
		if (LookupPrivilegeValue(NULL, privilege, &lUid))		//��ȡȨ��LUID
		{
			//����Ȩ�޴洢�ṹ��
			tkp.PrivilegeCount = 1;
			tkp.Privileges[0].Luid = lUid;
			tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			if (AdjustTokenPrivileges(handle, 0, &tkp, sizeof tkp, NULL, NULL))	//��Ȩ�޽ṹ���޸�����Ȩ��
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

DWORD addListControlRow(HWND hListControl,DWORD dwRow, DWORD dwCol, PTCHAR ptText) {
	DWORD ret;	//����ֵ
	LV_ITEM lv = { 0 };
	lv.mask = LVIF_TEXT;

	lv.iItem = dwRow;
	lv.iSubItem = dwCol;
	lv.pszText = ptText;

	if (dwCol == 0)
		ret = ListView_InsertItem(hListControl, &lv);
	else
		ret = ListView_SetItem(hListControl, &lv);
	return ret;
}

DWORD addProcessListControlRow(HWND hListControl) {

	HANDLE hProcessSnap;
	HANDLE hProcess;
	HMODULE hModule = 0;
	PROCESSENTRY32 processInfo = {0};
	BOOL bGetPro;
	PROCESS_MEMORY_COUNTERS pmc = {0};

	DWORD dwModuleCount;

	LVITEM lv = { 0 };
	lv.mask = LVFIF_TEXT;
	
	int i = 0;
	TCHAR ptText[259] = {0};

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);	//��ȡ���̿���
	if (hProcessSnap)
	{
		//��ʼ��PROCESSENTRY32
		processInfo.dwSize = sizeof PROCESSENTRY32;

		bGetPro = Process32First(hProcessSnap, &processInfo);	//��ȡ��һ������
		while (bGetPro)
		{
			//0
			wcsprintf(ptText, TEXT("%03d"), i);
			//lv.pszText = ptText;
			lv.pszText = (LPWSTR)TEXT("");			//�����Ե�һ�зű�������������в���ʾ�����
			lv.iItem = 0;							//����������ĳ��������������ʾ��ȫ
			lv.iSubItem = 0;
			ListView_InsertItem(hListControl, &lv);	//������ֻ���ڵ�һ�����ÿ��ı�

			//1.������
			lv.pszText = processInfo.szExeFile;
			lv.iSubItem = 1;
			ListView_SetItem(hListControl, &lv);

			//2.PID
			wcsprintf(ptText, TEXT("%05d"), processInfo.th32ProcessID);
			lv.pszText = ptText;
			lv.iSubItem = 2;
			ListView_SetItem(hListControl, &lv);

			//3.������PID
			wcsprintf(ptText, TEXT("%05d"), processInfo.th32ParentProcessID);
			lv.pszText = ptText;
			lv.iSubItem = 3;
			ListView_SetItem(hListControl, &lv);

			//4.�߳���
			wcsprintf(ptText, TEXT("%03d"), processInfo.cntThreads);
			lv.pszText = ptText;
			lv.iSubItem = 4;
			ListView_SetItem(hListControl, &lv);

			//5.ģ���ַ
			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processInfo.th32ProcessID);
			EnumProcessModules(hProcess, &hModule, sizeof hModule, &dwModuleCount);
			wcsprintf(ptText, TEXT("%08X"), hModule);
			lv.pszText = ptText;
			lv.iSubItem = 5;
			ListView_SetItem(hListControl, &lv);

			//6.ģ���С
			GetProcessMemoryInfo(hProcess, &pmc, sizeof pmc);
			wcsprintf(ptText, TEXT("%08X"), pmc.WorkingSetSize);
			lv.pszText = ptText;
			lv.iSubItem = 6;
			ListView_SetItem(hListControl, &lv);

			CloseHandle(hProcess);
			i++;
			bGetPro = Process32Next(hProcessSnap, &processInfo);	//��ȡ��һ������
		}
	}
	CloseHandle(hProcessSnap);
	return i;
}

DWORD addMoudelListControlRow(HWND& hProcessListCtrl, HWND& hMoudelListCtrl) {

	DWORD dwPid;		//����PID
	DWORD dwRow;		//�����б�ͨ�ÿؼ����������

	HANDLE hProcess;				//���̾��
	HMODULE phMoudles[512] = { 0 };	//ģ��������
	DWORD dwMoudleCount = 0;		//ģ������ø���
	int i = 0;		//ѭ������

	LVITEM lv = { 0 };				//���ģ���б�ͨ�ÿؼ�����
	lv.mask = LVIF_TEXT;
	TCHAR ptText[259] = { 0 };		//�ı�������

	MODULEINFO moudleInfo = { 0 };	//ģ����Ϣ

	//1.��ȡ�к�
	dwRow = SendMessage(hProcessListCtrl, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

	//2.��ȡPID
	ListView_GetItemText(hProcessListCtrl,dwRow, 2, ptText, sizeof ptText);
	dwPid = StrToLong(ptText);

	//3.��ָ��PID�Ľ���
	if (!processTokenUp(GetCurrentProcess(), SE_DEBUG_NAME))	//��Ȩ
		return 0;

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPid);
	EnumProcessModules(hProcess, phMoudles, sizeof phMoudles, &dwMoudleCount);

	if (dwMoudleCount > sizeof phMoudles)
		MessageBox(0, TEXT("���������ù�С,����DLLû��ʾ����"),TEXT("��ʾ"),0);

	dwMoudleCount /= sizeof HMODULE;

	if (dwMoudleCount > sizeof phMoudles / sizeof HMODULE)
		dwMoudleCount = sizeof phMoudles / sizeof HMODULE;


	//4.��������ģ��,���ģ����Ϣ���б�ͨ�ÿؼ�
	while (i < dwMoudleCount)
	{
		//���
		wcsprintf(ptText, TEXT("%03d"), i);
		lv.pszText = ptText;
		lv.iSubItem = 0;
		ListView_InsertItem(hMoudelListCtrl, &lv);

		//0.ģ����
		GetModuleBaseName(hProcess, phMoudles[i], ptText, sizeof ptText);
		lv.pszText = ptText;
		lv.iSubItem = 1;
		ListView_SetItem(hMoudelListCtrl, &lv);
		//ListView_InsertItem(hMoudelListCtrl, &lv);

		GetModuleInformation(hProcess, phMoudles[i], &moudleInfo, sizeof moudleInfo);
		//1.ģ���ַ
		wcsprintf(ptText, TEXT("%08X"), (DWORD)moudleInfo.lpBaseOfDll);
		lv.pszText = ptText;
		lv.iSubItem = 2;
		ListView_SetItem(hMoudelListCtrl, &lv);

		//2.ģ���С
		wcsprintf(ptText, TEXT("%08X"), moudleInfo.SizeOfImage);
		lv.pszText = ptText;
		lv.iSubItem = 3;
		ListView_SetItem(hMoudelListCtrl, &lv);

		//3.ģ�����
		wcsprintf(ptText, TEXT("%08X"), (DWORD)moudleInfo.EntryPoint);
		lv.pszText = ptText;
		lv.iSubItem = 4;
		ListView_SetItem(hMoudelListCtrl, &lv);

		i++;
	}
	CloseHandle(hProcess);
	return i;
}

BOOL openFileName(PTCHAR ptText,DWORD dwBuffSize) {
	bFlag = TRUE;
	OPENFILENAME ofn = { 0 };					//�ļ�ѡ��Ի���ṹ

	//��ʼ��OPENFILENAME�ṹ
	ofn.lStructSize = sizeof OPENFILENAME;		//�ṹ��С
	ofn.hwndOwner = NULL;						//�������ڵľ��
	ofn.lpstrFilter = TEXT("PE�ļ�\0*.exe");	//�ļ�ѡ�������
	ofn.nFilterIndex = 1;						//Ĭ�Ϲ���������
	ofn.lpstrFile = ptText;						//�ļ�·����Ż�����
	ofn.nMaxFile = dwBuffSize;					//�ļ�·����Ż���������С
	ofn.lpstrInitialDir = NULL;					//�ļ�ѡ��Ի���Ĭ�ϴ�·��
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;	//�ļ�,·���������,����ֻ���ļ�

	return GetOpenFileName(&ofn);	//���ļ��Ի���
}


BOOL addPEEditWinContent(HWND hwnd,PTCHAR ptFileName) {
	DWORD dwFileSize;

	dwFileSize = loadPEFile(ptFileName, &pFileBuff);		//����PE�ļ���������
	if (pFileBuff == NULL)
		return FALSE;

	if (getPEHeader(pFileBuff, 0, &pFileHeader, &pOptionalHeader))	//��ȡPEͷ
	{
		//��ڵ�
		wcsprintf(ptText, TEXT("%08X"), pOptionalHeader->AddressOfEntryPoint);
		SetDlgItemText(hwnd, IDC_EDIT_ENTRY, ptText);

		//��ַ
		wcsprintf(ptText, TEXT("%08X"), pOptionalHeader->ImageBase);
		SetDlgItemText(hwnd, IDC_EDIT_BASE, ptText);

		//�ļ���С
		wcsprintf(ptText, TEXT("%04X"), dwFileSize);
		SetDlgItemText(hwnd, IDC_EDIT_FILESIZE, ptText);

		//�ڴ�ڿ����
		wcsprintf(ptText, TEXT("%08X"), pOptionalHeader->SectionAlignment);
		SetDlgItemText(hwnd, IDC_EDIT_MENALIG, ptText);

		//�ļ��ڿ����
		wcsprintf(ptText, TEXT("%08X"), pOptionalHeader->FileAlignment);
		SetDlgItemText(hwnd, IDC_EDIT_FILEALIG, ptText);

		//PE��־
		wcsprintf(ptText, TEXT("%04X"), pOptionalHeader->Magic);
		SetDlgItemText(hwnd, IDC_EDIT_PEMAGIC, ptText);

		//��ϵͳ
		wcsprintf(ptText, TEXT("%04X"), pOptionalHeader->Subsystem);
		SetDlgItemText(hwnd, IDC_EDIT_SUBSYS, ptText);

		//������Ŀ
		wcsprintf(ptText, TEXT("%04X"), pFileHeader->NumberOfSections);
		SetDlgItemText(hwnd, IDC_EDIT_SECTIONNUMBER, ptText);

		//�ļ�ʱ��
		wcsprintf(ptText, TEXT("%08X"), pFileHeader->TimeDateStamp);
		SetDlgItemText(hwnd, IDC_EDIT_FILETIME, ptText);

		//�ײ����ڱ��С
		wcsprintf(ptText, TEXT("%08X"), pOptionalHeader->SizeOfHeaders);
		SetDlgItemText(hwnd, IDC_EDIT_HEADSIZE, ptText);

		//��Ϣ��־
		wcsprintf(ptText, TEXT("%04X"), pFileHeader->Characteristics);
		SetDlgItemText(hwnd, IDC_EDIT_CHARACTER, ptText);

		//��ѡPEͷ��С
		wcsprintf(ptText, TEXT("%04X"), sizeof IMAGE_OPTIONAL_HEADER);
		SetDlgItemText(hwnd, IDC_EDIT_OPTIONSIZE, ptText);

	}
	return TRUE;
}

VOID showPEMagicWin(HWND hwnd) {
	DialogBox(hAPPInterface, MAKEINTRESOURCE(IDD_DIALOG_PEMAGIC), hwnd, peMagicWinProc);
}


BOOL selectSubSystem(HWND hwnd) {
	GetDlgItemText(hwnd, IDC_EDIT_SUBSYS, ptText, sizeof ptText);
	DialogBox(hAPPInterface, MAKEINTRESOURCE(IDD_DIALOG_SUBSYS), hwnd, selectSubSystemWinProc);
	return bFlag;
}