#pragma once
#include "tools.h"
#include "win_fileHeadCharacter.h"
#include "win_subSystem.h"
#include "win_optionHeadMagic.h"
#include "win_sectionTable.h"
#include "win_peDirectory.h"


//PE�༭������Ϣ����ص�����
INT_PTR CALLBACK winProcPEEdit(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam);

//************************************
// Method:    addPEEditWinContent
// FullName:  ���PE�༭������
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Parameter: HWND hwnd PE�༭���ھ��
// Parameter: PTCHAR ptFileName �ļ���ָ��,���ڼ���PE�ļ���������
//************************************
BOOL addPEEditWinContent(HWND hwnd, PTCHAR ptFileName);

