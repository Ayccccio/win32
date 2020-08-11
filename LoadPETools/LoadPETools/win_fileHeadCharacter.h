#pragma once
#include "tools.h"

//ѡ���������Դ�����Ϣ�ص�����
INT_PTR CALLBACK selectCharacterOfFileHeadWinProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam);

//************************************
// Method:    openSelCharacterWinOfFileHead
// FullName:  �򿪱�׼PEͷѡ����������
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Parameter: HWND hwnd �����ھ��
//************************************
BOOL openSelCharacterWinOfFileHead(HWND hwnd);



//************************************
// Method:    setCharacterOfFileHead
// FullName:  ���ñ�׼PEͷ������ֵ
// Access:    public 
// Returns:   DWORD
// Qualifier:
// Parameter: HWND hwnd ����ֵѡ�񴰿ھ��
// Parameter: int index ��ѡ��ID��������
// Parameter: PDWORD pdCharacterValue ����ֵָ��
//************************************
DWORD setCharacterOfFileHead(HWND hwnd, int index, PDWORD pdCharacterValue);