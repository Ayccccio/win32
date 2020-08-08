#include "tools.h"

//PE标志窗口消息回调函数
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

//选择子系统窗口消息回调函数
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
	//提权
	HANDLE handle;
	LUID lUid;
	TOKEN_PRIVILEGES tkp;
	if (OpenProcessToken(processHandle, TOKEN_ALL_ACCESS, &handle))		//获取进程的令牌句柄
	{
		if (LookupPrivilegeValue(NULL, privilege, &lUid))		//获取权限LUID
		{
			//设置权限存储结构体
			tkp.PrivilegeCount = 1;
			tkp.Privileges[0].Luid = lUid;
			tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			if (AdjustTokenPrivileges(handle, 0, &tkp, sizeof tkp, NULL, NULL))	//按权限结构体修改令牌权限
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

DWORD addListControlRow(HWND hListControl,DWORD dwRow, DWORD dwCol, PTCHAR ptText) {
	DWORD ret;	//返回值
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

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);	//获取进程快照
	if (hProcessSnap)
	{
		//初始化PROCESSENTRY32
		processInfo.dwSize = sizeof PROCESSENTRY32;

		bGetPro = Process32First(hProcessSnap, &processInfo);	//获取第一个进程
		while (bGetPro)
		{
			//0
			wcsprintf(ptText, TEXT("%03d"), i);
			//lv.pszText = ptText;
			lv.pszText = (LPWSTR)TEXT("");			//经测试第一列放变量会出现其他列不显示的情况
			lv.iItem = 0;							//经测试这里改成行索引会出现显示不全
			lv.iSubItem = 0;
			ListView_InsertItem(hListControl, &lv);	//经测试只能在第一列设置空文本

			//1.进程名
			lv.pszText = processInfo.szExeFile;
			lv.iSubItem = 1;
			ListView_SetItem(hListControl, &lv);

			//2.PID
			wcsprintf(ptText, TEXT("%05d"), processInfo.th32ProcessID);
			lv.pszText = ptText;
			lv.iSubItem = 2;
			ListView_SetItem(hListControl, &lv);

			//3.父进程PID
			wcsprintf(ptText, TEXT("%05d"), processInfo.th32ParentProcessID);
			lv.pszText = ptText;
			lv.iSubItem = 3;
			ListView_SetItem(hListControl, &lv);

			//4.线程数
			wcsprintf(ptText, TEXT("%03d"), processInfo.cntThreads);
			lv.pszText = ptText;
			lv.iSubItem = 4;
			ListView_SetItem(hListControl, &lv);

			//5.模块地址
			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processInfo.th32ProcessID);
			EnumProcessModules(hProcess, &hModule, sizeof hModule, &dwModuleCount);
			wcsprintf(ptText, TEXT("%08X"), hModule);
			lv.pszText = ptText;
			lv.iSubItem = 5;
			ListView_SetItem(hListControl, &lv);

			//6.模块大小
			GetProcessMemoryInfo(hProcess, &pmc, sizeof pmc);
			wcsprintf(ptText, TEXT("%08X"), pmc.WorkingSetSize);
			lv.pszText = ptText;
			lv.iSubItem = 6;
			ListView_SetItem(hListControl, &lv);

			CloseHandle(hProcess);
			i++;
			bGetPro = Process32Next(hProcessSnap, &processInfo);	//获取下一个进程
		}
	}
	CloseHandle(hProcessSnap);
	return i;
}

DWORD addMoudelListControlRow(HWND& hProcessListCtrl, HWND& hMoudelListCtrl) {

	DWORD dwPid;		//进程PID
	DWORD dwRow;		//进程列表通用控件点击的行数

	HANDLE hProcess;				//进程句柄
	HMODULE phMoudles[512] = { 0 };	//模块句柄集合
	DWORD dwMoudleCount = 0;		//模块句柄获得个数
	int i = 0;		//循环变量

	LVITEM lv = { 0 };				//添加模块列表通用控件内容
	lv.mask = LVIF_TEXT;
	TCHAR ptText[259] = { 0 };		//文本缓冲区

	MODULEINFO moudleInfo = { 0 };	//模块信息

	//1.获取行号
	dwRow = SendMessage(hProcessListCtrl, LVM_GETNEXTITEM, -1, LVNI_SELECTED);

	//2.获取PID
	ListView_GetItemText(hProcessListCtrl,dwRow, 2, ptText, sizeof ptText);
	dwPid = StrToLong(ptText);

	//3.打开指定PID的进程
	if (!processTokenUp(GetCurrentProcess(), SE_DEBUG_NAME))	//提权
		return 0;

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPid);
	EnumProcessModules(hProcess, phMoudles, sizeof phMoudles, &dwMoudleCount);

	if (dwMoudleCount > sizeof phMoudles)
		MessageBox(0, TEXT("缓冲区设置过小,部分DLL没显示出来"),TEXT("提示"),0);

	dwMoudleCount /= sizeof HMODULE;

	if (dwMoudleCount > sizeof phMoudles / sizeof HMODULE)
		dwMoudleCount = sizeof phMoudles / sizeof HMODULE;


	//4.遍历进程模块,添加模块信息到列表通用控件
	while (i < dwMoudleCount)
	{
		//序号
		wcsprintf(ptText, TEXT("%03d"), i);
		lv.pszText = ptText;
		lv.iSubItem = 0;
		ListView_InsertItem(hMoudelListCtrl, &lv);

		//0.模块名
		GetModuleBaseName(hProcess, phMoudles[i], ptText, sizeof ptText);
		lv.pszText = ptText;
		lv.iSubItem = 1;
		ListView_SetItem(hMoudelListCtrl, &lv);
		//ListView_InsertItem(hMoudelListCtrl, &lv);

		GetModuleInformation(hProcess, phMoudles[i], &moudleInfo, sizeof moudleInfo);
		//1.模块基址
		wcsprintf(ptText, TEXT("%08X"), (DWORD)moudleInfo.lpBaseOfDll);
		lv.pszText = ptText;
		lv.iSubItem = 2;
		ListView_SetItem(hMoudelListCtrl, &lv);

		//2.模块大小
		wcsprintf(ptText, TEXT("%08X"), moudleInfo.SizeOfImage);
		lv.pszText = ptText;
		lv.iSubItem = 3;
		ListView_SetItem(hMoudelListCtrl, &lv);

		//3.模块入口
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
	OPENFILENAME ofn = { 0 };					//文件选择对话框结构

	//初始化OPENFILENAME结构
	ofn.lStructSize = sizeof OPENFILENAME;		//结构大小
	ofn.hwndOwner = NULL;						//所属窗口的句柄
	ofn.lpstrFilter = TEXT("PE文件\0*.exe");	//文件选择过滤器
	ofn.nFilterIndex = 1;						//默认过滤器索引
	ofn.lpstrFile = ptText;						//文件路径存放缓冲区
	ofn.nMaxFile = dwBuffSize;					//文件路径存放缓冲区最大大小
	ofn.lpstrInitialDir = NULL;					//文件选择对话框默认打开路径
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;	//文件,路径必须存在,隐藏只读文件

	return GetOpenFileName(&ofn);	//打开文件对话框
}


BOOL addPEEditWinContent(HWND hwnd,PTCHAR ptFileName) {
	DWORD dwFileSize;

	dwFileSize = loadPEFile(ptFileName, &pFileBuff);		//加载PE文件到缓冲区
	if (pFileBuff == NULL)
		return FALSE;

	if (getPEHeader(pFileBuff, 0, &pFileHeader, &pOptionalHeader))	//获取PE头
	{
		//入口点
		wcsprintf(ptText, TEXT("%08X"), pOptionalHeader->AddressOfEntryPoint);
		SetDlgItemText(hwnd, IDC_EDIT_ENTRY, ptText);

		//基址
		wcsprintf(ptText, TEXT("%08X"), pOptionalHeader->ImageBase);
		SetDlgItemText(hwnd, IDC_EDIT_BASE, ptText);

		//文件大小
		wcsprintf(ptText, TEXT("%04X"), dwFileSize);
		SetDlgItemText(hwnd, IDC_EDIT_FILESIZE, ptText);

		//内存节块对齐
		wcsprintf(ptText, TEXT("%08X"), pOptionalHeader->SectionAlignment);
		SetDlgItemText(hwnd, IDC_EDIT_MENALIG, ptText);

		//文件节块对齐
		wcsprintf(ptText, TEXT("%08X"), pOptionalHeader->FileAlignment);
		SetDlgItemText(hwnd, IDC_EDIT_FILEALIG, ptText);

		//PE标志
		wcsprintf(ptText, TEXT("%04X"), pOptionalHeader->Magic);
		SetDlgItemText(hwnd, IDC_EDIT_PEMAGIC, ptText);

		//子系统
		wcsprintf(ptText, TEXT("%04X"), pOptionalHeader->Subsystem);
		SetDlgItemText(hwnd, IDC_EDIT_SUBSYS, ptText);

		//区段数目
		wcsprintf(ptText, TEXT("%04X"), pFileHeader->NumberOfSections);
		SetDlgItemText(hwnd, IDC_EDIT_SECTIONNUMBER, ptText);

		//文件时间
		wcsprintf(ptText, TEXT("%08X"), pFileHeader->TimeDateStamp);
		SetDlgItemText(hwnd, IDC_EDIT_FILETIME, ptText);

		//首部及节表大小
		wcsprintf(ptText, TEXT("%08X"), pOptionalHeader->SizeOfHeaders);
		SetDlgItemText(hwnd, IDC_EDIT_HEADSIZE, ptText);

		//信息标志
		wcsprintf(ptText, TEXT("%04X"), pFileHeader->Characteristics);
		SetDlgItemText(hwnd, IDC_EDIT_CHARACTER, ptText);

		//可选PE头大小
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