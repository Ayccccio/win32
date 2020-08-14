#define  _CRT_SECURE_NO_WARNINGS

#pragma once
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <math.h>

void __cdecl OutputDebugStringF(const TCHAR* format, ...);

#ifdef _DEBUG  
#define DbgPrintf   OutputDebugStringF  
#else  
#define DbgPrintf  
#endif 


//************************************
// Method:    getBitOfIndex
// FullName:  ��ȡ���ݶ�������ָ��λ��ֵ
// Access:    public 
// Returns:   DWORD ����������Χ����-1,�ɹ�����ָ������λ�ö�����ֵ
// Qualifier:
// Parameter: DWORD data ֵ
// Parameter: DWORD index ����
//************************************
DWORD getBitOfIndex(DWORD data, DWORD dwIndex);



//************************************
// Method:    setBitOfIndex
// FullName:  �������ݶ�������ָ��λ��ֵ
// Access:    public 
// Returns:   DWORD ����������Χ����-1,�ɹ������������ú��ֵ
// Qualifier:
// Parameter: DWORD * pdwData ֵ
// Parameter: DWORD dwIndex ����
// Parameter: BOOL bVar ������ֵ
//************************************
DWORD setBitOfIndex(DWORD* pdwData, DWORD dwIndex, BOOL bVar);



DWORD multByteToWideChar(PCHAR pcResource, int reSize, PWCHAR* pwBuff, int buffSize);