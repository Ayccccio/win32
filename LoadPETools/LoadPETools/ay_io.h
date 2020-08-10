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
// Parameter: T var ֵ
// Parameter: DWORD index ����
//************************************
template <class T>
DWORD getBitOfIndex(T var, DWORD index);

template <class T>
DWORD getBitOfIndex(T var, DWORD index) {
	DWORD dwNum = sizeof var * 8;
	if (index > dwNum || index == 0)
	{
		return -1;
	}

	if (index == 1)
	{
		return var & 1;
	}
	else
		return var & (T)pow(2, index - 1);
}
