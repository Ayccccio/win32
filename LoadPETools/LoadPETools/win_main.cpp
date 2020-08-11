#include "win_main.h"

//��������Ϣ����ص�����
INT_PTR CALLBACK WindowsProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	HICON hIcon = NULL;				//ͼ����
	HWND hProcessListCtrl = NULL;	//�����б�ͨ�ÿؼ����
	HWND hMoudelListCtrl = NULL;	//ģ���б�ͨ�ÿؼ����

	WORD pdProListCtrlColWidth[ProcessListControlColumNumber] = { 0,200,50,80,50,100,100 };		//�����б�ͨ�ÿؼ��еĿ��
	WORD pdMudListCtrlColWidth[MoudelListControlColumNumber] = { 50, 200,100,100,100 };			//ģ���б�ͨ�ÿؼ��еĿ��

	NMHDR* pNmhdr;			//WM_NOTIFY ��Ϣ�ṹָ��


	switch (uMsg)
	{
	case WM_CLOSE:
	{
		EndDialog(hwnd, 0);
		return TRUE;
	}
	case WM_INITDIALOG:
	{
		//����ͼ��
		hIcon = LoadIcon(hAPPInterface, MAKEINTRESOURCE(IDI_ICON1));
		SendMessage((HWND)hAPPInterface, WM_SETICON, ICON_BIG, (DWORD)hIcon);
		SendMessage((HWND)hAPPInterface, WM_SETICON, ICON_SMALL, (DWORD)hIcon);

		hProcessListCtrl = GetDlgItem(hwnd, IDC_LIST_PROCESS);	//��ȡ�����б�ͨ�ÿؼ����
		hMoudelListCtrl = GetDlgItem(hwnd, IDC_LIST_MOUDEL);	//��ȡģ���б�ͨ�ÿؼ����

		//���ý����б�ͨ�öԻ���ѡ��
		SendMessage(hProcessListCtrl, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

		//�б�ͨ�öԻ����ʼ����ͷ
		initListControlHeader(hProcessListCtrl, ProcessListControlColumNumber, (PTCHAR)TEXT("���\0������\0PID\0����PID\0�߳���\0�����ַ\0�����С"), pdProListCtrlColWidth);	//��ʼ�������б�ͨ�ÿؼ���ͷ
		initListControlHeader(hMoudelListCtrl, MoudelListControlColumNumber, (PTCHAR)TEXT("���\0ģ������\0ģ���ַ\0ģ���С\0ģ�����"), pdMudListCtrlColWidth);	//��ʼ�������б�ͨ�ÿؼ���ͷ

		//��ӽ���ͨ�ÿؼ�����
		addProcessListControlRow(hProcessListCtrl);
		return TRUE;
	}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case IDC_BUTTON_LOGOUT:
		{
			EndDialog(hwnd, 0);
			break;
		}
		case IDC_BUTTON_ABOUT:
		{

			break;
		}
		case IDC_BUTTON_PEEDIT:
		{
			if (!bFlag)	//�ж��ļ��Ի����Ƿ��
			{
				if (openFileName(ptText, sizeof ptText))	//�ļ��Ի����ȡ�ļ�Ŀ¼
				{
					DialogBox(hAPPInterface, MAKEINTRESOURCE(IDD_DIALOG_PEEDIT), hwnd, peEditWinProc);
				}
				bFlag = FALSE;		//������0
			}
			break;
		}
		default:
			return FALSE;
			return TRUE;
		}
	}
	case WM_NOTIFY:
	{
		pNmhdr = (NMHDR*)lParam;
		switch (wParam)
		{
		case IDC_LIST_PROCESS:
		{
			hProcessListCtrl = GetDlgItem(hwnd, IDC_LIST_PROCESS);
			if (pNmhdr->code == NM_CLICK)
			{
				hMoudelListCtrl = GetDlgItem(hwnd, IDC_LIST_MOUDEL);
				ListView_DeleteAllItems(hMoudelListCtrl);
				addMoudelListControlRow(hProcessListCtrl, hMoudelListCtrl);
			}
		}
		return TRUE;
		}
	}
	default:
		return FALSE;
		return TRUE;
	}


	return FALSE;
}

DWORD addProcessListControlRow(HWND hListControl) {

	HANDLE hProcessSnap;
	HANDLE hProcess;
	HMODULE hModule = 0;
	PROCESSENTRY32 processInfo = { 0 };
	BOOL bGetPro;
	PROCESS_MEMORY_COUNTERS pmc = { 0 };

	DWORD dwModuleCount;

	LVITEM lv = { 0 };
	lv.mask = LVFIF_TEXT;

	int i = 0;
	TCHAR ptText[259] = { 0 };

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
	ListView_GetItemText(hProcessListCtrl, dwRow, 2, ptText, sizeof ptText);
	dwPid = StrToLong(ptText);

	//3.��ָ��PID�Ľ���
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPid);

	//4.ö�ٽ���ģ��
	EnumProcessModulesEx(hProcess, phMoudles, sizeof phMoudles, &dwMoudleCount, LIST_MODULES_ALL);

	if (dwMoudleCount > sizeof phMoudles)
		MessageBox(0, TEXT("���������ù�С,����DLLû��ʾ����"), TEXT("��ʾ"), 0);

	dwMoudleCount /= sizeof HMODULE;

	if (dwMoudleCount > sizeof phMoudles / sizeof HMODULE)
		dwMoudleCount = sizeof phMoudles / sizeof HMODULE;


	//5.��������ģ��,���ģ����Ϣ���б�ͨ�ÿؼ�
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
