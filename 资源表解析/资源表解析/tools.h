#pragma once
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include <ShlWapi.h>
#pragma comment(lib, "shlwapi");

void __cdecl OutputDebugStringF(const TCHAR* format, ...);

#ifdef _DEBUG  
#define DbgPrintf   OutputDebugStringF  
#else  
#define DbgPrintf  
#endif 


#ifdef UNICODE
#define fileOpen _wfopen_s
#else
#define fileOpen fopen_s
#endif


#ifdef UNICODE
#define strLen wcslen
#else
#define strLen strlen
#endif


//************************************
// Method:    loadPEFile
// FullName:  ����PE�ļ���������
// Access:    public 
// Returns:   DWORD
// Qualifier:
// Parameter: IN PTCHAR ptFileName �ļ�·��
// Parameter: OUT VOID * & pFileBuff ������ָ��,ͨ��malloc,��free�ͷ�
//************************************
DWORD loadPEFile(IN PTCHAR ptFileName, OUT VOID*& pFileBuff);


//************************************
// Method:    freePeFileBuff
// FullName:  �ͷ�PE�ļ����صĻ�����
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: VOID * pFileBuff ������ָ��
//************************************
void freePeFileBuff(VOID* pFileBuff);