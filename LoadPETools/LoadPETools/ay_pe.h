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


//************************************
// Method:    getPEHeader
// FullName:  从PE文件缓冲区中获取DOS,FILE,OPTIONAL头
// Access:    public 
// Returns:   DWORD 缓冲区不符合PE格式返回0,成功返回获取到的PE头个数
// Qualifier:
// Parameter: IN VOID * pFileBuff 缓冲区指针
// Parameter: OUT PIMAGE_DOS_HEADER & pImageDosHead	DOS头指针
// Parameter: OUT PIMAGE_FILE_HEADER & pImageFileHead 标准PE头指针
// Parameter: OUT PIMAGE_OPTIONAL_HEADER & pImageOptionalHead 可选PE头指针
//************************************
DWORD getPEHeader(IN VOID* pFileBuff,
	OUT PIMAGE_DOS_HEADER* pImageDosHead,
	OUT PIMAGE_FILE_HEADER* pImageFileHead,
	OUT PIMAGE_OPTIONAL_HEADER* pImageOptionalHead);



//************************************
// Method:    getPESectionHeader
// FullName:  通过PE文件缓冲区指针获取第一个节表
// Access:    public 
// Returns:   BOOL 成功返回TRUE,失败返回FALSE
// Qualifier:
// Parameter: IN VOID * pFileBuff 缓冲区指针
// Parameter: OUT PIMAGE_SECTION_HEADER & pImageSectionHead 节表指针引用
//************************************
BOOL getPESectionHeader(IN VOID* pFileBuff, OUT PIMAGE_SECTION_HEADER& pImageSectionHead);


//************************************
// Method:    getPESectionHeader
// FullName:  通过PE文件缓冲区指针获取最后个节表
// Access:    public 
// Returns:   BOOL 成功返回TRUE,失败返回FALSE
// Qualifier:
// Parameter: IN VOID * pFileBuff 缓冲区指针
// Parameter: OUT PIMAGE_SECTION_HEADER & pImageSectionHead 节表指针引用
//************************************
BOOL getPELastSectionHeader(IN VOID* pFileBuff, OUT PIMAGE_SECTION_HEADER& pImageSectionHead);



//************************************
// Method:    foaToRva
// FullName:  文件偏移地址转虚拟内存偏移地址
// Access:    public 
// Returns:   DWORD 成功返回虚拟内存偏移地址,失败返回0
// Qualifier:
// Parameter: IN VOID * pFileBuff PE文件缓冲区
// Parameter: DWORD dwFoa 文件偏移地址
//************************************
DWORD foaToRva(IN VOID* pFileBuff, IN DWORD dwFoa);



//************************************
// Method:    rvaToFoa
// FullName:  虚拟内存偏移转文件偏移
// Access:    public 
// Returns:   DWORD
// Qualifier:
// Parameter: IN VOID * pFileBuff 缓冲区指针
// Parameter: IN DWORD dwRva 虚拟内存偏移
//************************************
DWORD rvaToFoa(IN VOID* pFileBuff, IN DWORD dwRva);