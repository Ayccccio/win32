#pragma once
#include <windows.h>
#include <stdio.h>
#include <commctrl.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <ShlWapi.h>
#include "ay_pe.h"
#include "ay_io.h"
#include "const.h"
#include "resource.h"
#pragma comment(lib, "shlwapi")
#pragma comment(lib,"comctl32.lib")	


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


//************************************
// Method:    showPEMagicWin
// FullName:  ��ʾPE��Ǵ���
// Access:    public 
// Returns:   VOID
// Qualifier:
// Parameter: HWND hwnd ��ָ���ĸ����ھ��
//************************************
VOID showPEMagicWin(HWND hwnd);