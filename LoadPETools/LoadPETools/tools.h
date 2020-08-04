#pragma once
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <ShlWapi.h>
#include <commctrl.h>			
#pragma comment(lib, "shlwapi")
#pragma comment(lib,"comctl32.lib")		

void __cdecl OutputDebugStringF(const TCHAR* format, ...);

#ifdef _DEBUG  
#define DbgPrintf   OutputDebugStringF  
#else  
#define DbgPrintf  
#endif 

#ifdef UNICODE
#define StrLen wcslen
#else
#define StrLen strlen
#endif

//************************************
// Method:    initListControlHeader
// FullName:  ��ʼ���б�ͨ�ÿؼ���ͷ
// Access:    public 
// Returns:   DWORD
// Qualifier:
// Parameter: HWND hListControl ͨ�ÿؼ����
// Parameter: DWORD dwLenth ����
// Parameter: PTCHAR & ptColumNames ����������,�������������洢,�Խ���������
//************************************
DWORD initListControlHeader(HWND hListControl, DWORD dwLenth, PTCHAR ptColumNames);