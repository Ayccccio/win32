#pragma once
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <ShlWapi.h>
#include <commctrl.h>
#include <TlHelp32.h>
#include <Psapi.h>
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

#ifdef UNICODE
#define wcsprintf wsprintf
#else
#define wcsprintf sprintf
#endif

//************************************
// Method:    initListControlHeader
// FullName:  ��ʼ���б�ͨ�ÿؼ���ͷ
// Access:    public 
// Returns:   DWORD
// Qualifier:
// Parameter: HWND hListControl ͨ�ÿؼ����
// Parameter: DWORD dwLenth ����
// Parameter: PTCHAR ptColumNames ����������,�������������洢,�Խ���������
// Parameter: PWORD pdColWidths �����п�
//************************************
DWORD initListControlHeader(HWND hListControl, DWORD dwLenth, PTCHAR ptColumNames, PWORD pdColWidths);


//************************************
// Method:    processTokenUp
// FullName:  ������������Ȩ��
// Access:    public 
// Returns:   BOOL �ɹ�������,ʧ�ܷ��ؼ�
// Qualifier:
// Parameter: HANDLE processHandle ���̾��
// Parameter: LPCWSTR privilege Ȩ������
//************************************
BOOL processTokenUp(HANDLE processHandle, LPCWSTR privilege);



//************************************
// Method:    initProcessListControlRow
// FullName:  ��ʼ�������б�ͨ�ÿؼ�����
// Access:    public 
// Returns:   DWORD �ɹ������������,ʧ�ܷ���0
// Qualifier:
// Parameter: HWND hListControl �б�ͨ�ÿؼ����
//************************************
DWORD addProcessListControlRow(HWND hListControl);




DWORD addMoudelListControlRow(HWND& hProcessListCtrl, HWND& hMoudelListCtrl);