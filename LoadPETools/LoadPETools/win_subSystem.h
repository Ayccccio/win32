#pragma once
#include "tools.h"

//ѡ����ϵͳ������Ϣ�ص�����
INT_PTR CALLBACK selectSubSystemWinProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam);

//************************************
// Method:    openSelectSubSystem
// FullName:  ѡ����ϵͳ��־ֵ��ȫ�ֱ����е�ptText��
// Access:    public 
// Returns:   PTCHAR
// Qualifier:
// Parameter: HWND hwnd ����ָ�������ھ��
//************************************
BOOL openSelectSubSystem(HWND hwnd);