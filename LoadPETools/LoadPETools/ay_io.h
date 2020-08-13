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
// Parameter: DWORD data 值
// Parameter: DWORD index 索引
//************************************
DWORD getBitOfIndex(DWORD data, DWORD dwIndex);



//************************************
// Method:    setBitOfIndex
// FullName:  设置数据二进制中指定位的值
// Access:    public 
// Returns:   DWORD 索引超出范围返回-1,成功返回数据设置后的值
// Qualifier:
// Parameter: DWORD * pdwData 值
// Parameter: DWORD dwIndex 索引
// Parameter: BOOL bVar 二进制值
//************************************
DWORD setBitOfIndex(DWORD* pdwData, DWORD dwIndex, BOOL bVar);



DWORD multByteToWideChar(PCHAR pcResource, int reSize, PWCHAR* pwBuff, int buffSize);