#pragma once
#include "tools.h"

//��ѡPEͷMagic��־������Ϣ�ص�����
INT_PTR CALLBACK winProcPEMagic(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam);


//************************************
// Method:    showWinOfOptionHeadMagic
// FullName:  ��ʾPE��Ǵ���
// Access:    public 
// Returns:   VOID
// Qualifier:
// Parameter: HWND hwnd ��ָ���ĸ����ھ��
//************************************
VOID showWinOfOptionHeadMagic(HWND hwnd);
