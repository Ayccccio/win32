#pragma once
#include <stdio.h>
#include <ShlWapi.h>
#include <windowsx.h>
#include <windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <commctrl.h>
#pragma comment(lib,"comctl32.lib")	
#pragma comment(lib, "shlwapi")



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

#ifdef UNICODE
#define swcscanf_s swscanf_s
#else
#define swcscanf_s sscanf_s
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
// Method:    openFileName
// FullName:  ���ļ��Ի���
// Access:    public 
// Returns:   BOOL
// Qualifier:
// Parameter: PTCHAR ptText �ı�������,�����洢�ļ��Ի��򷵻ص��ļ�·��
// Parameter: DWORD dwBuffSize �ı���������С
//************************************
BOOL openFileName(PTCHAR ptText, DWORD dwBuffSize);

