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
// FullName:  加载PE文件到缓冲区
// Access:    public 
// Returns:   DWORD
// Qualifier:
// Parameter: IN PTCHAR ptFileName 文件路径
// Parameter: OUT VOID * & pFileBuff 缓冲区指针,通过malloc,需free释放
//************************************
DWORD loadPEFile(IN PTCHAR ptFileName, OUT VOID*& pFileBuff);


//************************************
// Method:    freePeFileBuff
// FullName:  释放PE文件加载的缓冲区
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: VOID * pFileBuff 缓冲区指针
//************************************
void freePeFileBuff(VOID* pFileBuff);