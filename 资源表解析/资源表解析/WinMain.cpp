#include "const.h"
#include "resource.h"
#include "tools.h"

INT_PTR CALLBACK WindowProc(HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam) {

	HICON hIcon;			//ͼ���ļ����

	OPENFILENAME ofn = { 0 };				//�ļ�ѡ��Ի���ṹ
	TCHAR ptFileNameBuff[MAX_PATH] = { 0 };	//�ļ�·��������

	DWORD dwReadFileSize;		//PE�ļ����ش�С
	

	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		hIcon = LoadIcon(hAPPInterface, MAKEINTRESOURCE(IDI_ICON_LOAD));
		SendMessage((HWND)hAPPInterface, WM_SETICON, ICON_BIG, (DWORD)hIcon);
		SendMessage((HWND)hAPPInterface, WM_SETICON, ICON_SMALL, (DWORD)hIcon);
		return TRUE;
	}
	case WM_CLOSE:
	{
		//EndDialog(hwnd, 0);
		PostQuitMessage(0);
		return TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		
		case IDC_BUTTON_OPEN:
		{
			//��ʼ��OPENFILENAME�ṹ
			ofn.lStructSize = sizeof OPENFILENAME;		//�ṹ��С
			ofn.hwndOwner = NULL;						//�������ڵľ��
			ofn.lpstrFilter = TEXT("PE�ļ�\0*.exe");	//�ļ�ѡ�������
			ofn.nFilterIndex = 1;						      //Ĭ�Ϲ���������
			ofn.lpstrFile = ptFileNameBuff;				//�ļ�·����Ż�����
			ofn.nMaxFile = sizeof ptFileNameBuff;		//�ļ�·����Ż���������С
			ofn.lpstrInitialDir = NULL;					//�ļ�ѡ��Ի���Ĭ�ϴ�·��
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;	//�ļ�,·���������,����ֻ���ļ�

			GetOpenFileName(&ofn);	//���ļ��Ի���
			if (*ptFileNameBuff != TEXT('\0'))
			{
				SetDlgItemText(hwnd, IDC_EDIT_FILENAME, ptFileNameBuff);		//���ñ༭������
			}
			return TRUE;
		}
		case IDC_BUTTON_LOAD:
		{
			
			GetDlgItemText(hwnd, IDC_EDIT_FILENAME, ptFileNameBuff, sizeof ptFileNameBuff);	//��ȡ�༭������
			if (*ptFileNameBuff == TEXT('\0'))
			{
				SetDlgItemText(hwnd, IDC_STATIC_STATE, TEXT("·��Ϊ��"));
				break;
			}
			else {
				dwReadFileSize = loadPEFile(ptFileNameBuff, pFileBuff);		//����PE�ļ�
				if (pFileBuff)
				{
					SetDlgItemText(hwnd, IDC_STATIC_STATE, TEXT("���سɹ�"));
				}
				else {
					SetDlgItemText(hwnd, IDC_STATIC_STATE, TEXT("����ʧ��"));
				}
			}
			return TRUE;
		}
		case IDC_BUTTON_PARSE:
		{
			
		}
		}
	}
	return FALSE;
}


int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
) {
	hAPPInterface = hInstance;		//��ģ������ȫ�ֱ���

	DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, WindowProc);	//�򿪶Ի���

	
}