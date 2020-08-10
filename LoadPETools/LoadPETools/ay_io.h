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
// FullName:  获取数据二进制中指定位的值
// Access:    public 
// Returns:   DWORD 索引超出范围返回-1,成功返回指定索引位置二进制值
// Qualifier:
// Parameter: T var 值
// Parameter: DWORD index 索引
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
