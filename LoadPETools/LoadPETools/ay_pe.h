#pragma once
#include <windows.h>
#include <stdio.h>
#include "ay_io.h"

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


//************************************
// Method:    getPEHeader
// FullName:  ��PE�ļ��������л�ȡDOS,FILE,OPTIONALͷ
// Access:    public 
// Returns:   DWORD ������������PE��ʽ����0,�ɹ����ػ�ȡ����PEͷ����
// Qualifier:
// Parameter: IN VOID * pFileBuff ������ָ��
// Parameter: OUT PIMAGE_DOS_HEADER & pImageDosHead	DOSͷָ��
// Parameter: OUT PIMAGE_FILE_HEADER & pImageFileHead ��׼PEͷָ��
// Parameter: OUT PIMAGE_OPTIONAL_HEADER & pImageOptionalHead ��ѡPEͷָ��
//************************************
DWORD getPEHeader(IN VOID* pFileBuff,
	OUT PIMAGE_DOS_HEADER* pImageDosHead,
	OUT PIMAGE_FILE_HEADER* pImageFileHead,
	OUT PIMAGE_OPTIONAL_HEADER* pImageOptionalHead);



//************************************
// Method:    getPESectionHeader
// FullName:  ͨ��PE�ļ�������ָ���ȡ��һ���ڱ�
// Access:    public 
// Returns:   BOOL �ɹ�����TRUE,ʧ�ܷ���FALSE
// Qualifier:
// Parameter: IN VOID * pFileBuff ������ָ��
// Parameter: OUT PIMAGE_SECTION_HEADER & pImageSectionHead �ڱ�ָ������
//************************************
BOOL getPESectionHeader(IN VOID* pFileBuff, OUT PIMAGE_SECTION_HEADER& pImageSectionHead);


//************************************
// Method:    getPESectionHeader
// FullName:  ͨ��PE�ļ�������ָ���ȡ�����ڱ�
// Access:    public 
// Returns:   BOOL �ɹ�����TRUE,ʧ�ܷ���FALSE
// Qualifier:
// Parameter: IN VOID * pFileBuff ������ָ��
// Parameter: OUT PIMAGE_SECTION_HEADER & pImageSectionHead �ڱ�ָ������
//************************************
BOOL getPELastSectionHeader(IN VOID* pFileBuff, OUT PIMAGE_SECTION_HEADER& pImageSectionHead);



//************************************
// Method:    foaToRva
// FullName:  �ļ�ƫ�Ƶ�ַת�����ڴ�ƫ�Ƶ�ַ
// Access:    public 
// Returns:   DWORD �ɹ����������ڴ�ƫ�Ƶ�ַ,ʧ�ܷ���0
// Qualifier:
// Parameter: IN VOID * pFileBuff PE�ļ�������
// Parameter: DWORD dwFoa �ļ�ƫ�Ƶ�ַ
//************************************
DWORD foaToRva(IN VOID* pFileBuff, IN DWORD dwFoa);



//************************************
// Method:    rvaToFoa
// FullName:  �����ڴ�ƫ��ת�ļ�ƫ��
// Access:    public 
// Returns:   DWORD
// Qualifier:
// Parameter: IN VOID * pFileBuff ������ָ��
// Parameter: IN DWORD dwRva �����ڴ�ƫ��
//************************************
DWORD rvaToFoa(IN VOID* pFileBuff, IN DWORD dwRva);