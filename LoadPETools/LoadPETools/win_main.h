#pragma once
#include "tools.h"
#include "win_peEdit.h"

//��������Ϣ����ص�����
INT_PTR CALLBACK WindowsProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam);

//************************************
// Method:    initProcessListControlRow
// FullName:  ��ʼ�������б�ͨ�ÿؼ�����
// Access:    public 
// Returns:   DWORD �ɹ������������,ʧ�ܷ���0
// Qualifier:
// Parameter: HWND hListControl �б�ͨ�ÿؼ����
//************************************
DWORD addProcessListControlRow(HWND hListControl);



//************************************
// Method:    addMoudelListControlRow
// FullName:  ���ģ���б�ͨ�ÿռ�����
// Access:    public 
// Returns:   DWORD
// Qualifier:
// Parameter: HWND & hProcessListCtrl �����б�ͨ�ÿռ���
// Parameter: HWND & hMoudelListCtrl ģ���б�ͨ�ÿռ���
//************************************
DWORD addMoudelListControlRow(HWND& hProcessListCtrl, HWND& hMoudelListCtrl);