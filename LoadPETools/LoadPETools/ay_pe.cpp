#include "ay_pe.h"


DWORD loadPEFile(IN PTCHAR ptFileName, OUT PVOID* pFileBuff) {
	FILE* pfFile;
	DWORD dwFileSize;
	DWORD dwReadSize;
	PVOID ptemp;

	freePeFileBuff(*pFileBuff);	//释放缓冲区,避免重复申请内存空间
	fileOpen(&pfFile, ptFileName, TEXT("rb+"));		//打开文件
	if (!pfFile)
	{
		DbgPrintf(TEXT("file open fail"));
		return 0;
	}

	//获取文件大小
	fseek(pfFile, 0, SEEK_END);
	dwFileSize = ftell(pfFile);
	fseek(pfFile, 0, SEEK_SET);

	//申请缓冲区内存空间
	ptemp = malloc(dwFileSize);

	//判断是否申请成功
	if (!ptemp)
	{
		fclose(pfFile);
		DbgPrintf(TEXT("fileBuff malloc fail"));
		return 0;
	}

	//读取文件二进制数据到缓冲区
	dwReadSize = fread_s(ptemp, dwFileSize, dwFileSize, 1, pfFile) * dwFileSize;
	fclose(pfFile);
	*pFileBuff = ptemp;
	return dwReadSize;
}

void freePeFileBuff(PVOID& pFileBuff)
{
	if (pFileBuff != NULL)
	{
		free(pFileBuff);
		pFileBuff = NULL;
	}
}


DWORD getPEHeader(IN VOID* pFileBuff,
	OUT PIMAGE_DOS_HEADER* pImageDosHead,
	OUT PIMAGE_FILE_HEADER* pImageFileHead,
	OUT PIMAGE_OPTIONAL_HEADER* pImageOptionalHead) {

	PIMAGE_DOS_HEADER pDosH = (PIMAGE_DOS_HEADER)pFileBuff;		//获取DOS头
	DWORD ret = 0;

	if (pDosH->e_magic != IMAGE_DOS_SIGNATURE)	//判断MZ标记
	{
		DbgPrintf(TEXT("PE fileBuff error"));
		return 0;
	}
	if (*(LONG*)((ADWORD)pFileBuff + pDosH->e_lfanew) != IMAGE_NT_SIGNATURE)	//[判断PE标记
	{
		DbgPrintf(TEXT("PE fileBuff error"));
		return 0;
	}
	if (pImageDosHead != NULL)		
	{
		*pImageDosHead = pDosH;
		ret++;
	}
	if (pImageFileHead != NULL)
	{
		*pImageFileHead = (PIMAGE_FILE_HEADER)((ADWORD)pFileBuff + pDosH->e_lfanew + 4);
		ret++;
	}
	if (pImageOptionalHead != NULL)
	{
		*pImageOptionalHead = (PIMAGE_OPTIONAL_HEADER)((ADWORD)pFileBuff + pDosH->e_lfanew + 4 + IMAGE_SIZEOF_FILE_HEADER);
		ret++;
	}
	return ret;
}


BOOL getPESectionHeader(IN VOID* pFileBuff, OUT PIMAGE_SECTION_HEADER& pImageSectionHead) {
	PIMAGE_FILE_HEADER pImageFileHead = NULL;
	PIMAGE_OPTIONAL_HEADER pImageOptionalHead = NULL;
	if (getPEHeader(pFileBuff,0,&pImageFileHead,&pImageOptionalHead))		//获取PE头
	{
		pImageSectionHead = (PIMAGE_SECTION_HEADER)((ADWORD)pImageOptionalHead + pImageFileHead->SizeOfOptionalHeader);		//计算节表指针
		return TRUE;
	}
	return FALSE;
}


BOOL getPELastSectionHeader(IN VOID* pFileBuff, OUT PIMAGE_SECTION_HEADER& pImageSectionHead) {
	PIMAGE_FILE_HEADER pImageFileHead = NULL;
	PIMAGE_OPTIONAL_HEADER pImageOptionalHead = NULL;
	if (getPEHeader(pFileBuff, 0, &pImageFileHead, &pImageOptionalHead))		//获取PE头
	{
		pImageSectionHead = (PIMAGE_SECTION_HEADER)((ADWORD)pImageOptionalHead +
			pImageFileHead->SizeOfOptionalHeader + 
			(pImageFileHead->NumberOfSections - 1) * 
			IMAGE_SIZEOF_SECTION_HEADER);		//计算节表指针
		return TRUE;
	}
	return FALSE;
}


ADWORD foaToRva(IN VOID* pFileBuff,IN ADWORD dwFoa) {
	PIMAGE_FILE_HEADER pImageFileHead = NULL;
	PIMAGE_OPTIONAL_HEADER pImageOptionalHead = NULL;
	PIMAGE_SECTION_HEADER pImageSectionHead = NULL;
	int i = 0;
	ADWORD dwRva;

	if (getPEHeader(pFileBuff, 0, &pImageFileHead, &pImageOptionalHead) != 2)	//获取PE头
		return 0;

	if (dwFoa < pImageOptionalHead->SizeOfHeaders)		//判断foa是否在头部
	{
		return dwFoa;
	}
	else {
		if (!getPESectionHeader(pFileBuff, pImageSectionHead))		//判断节表是否获取成功
			return 0;
		pImageSectionHead = (PIMAGE_SECTION_HEADER)((ADWORD)pImageSectionHead +		//获取最后一个节表
			(pImageFileHead->NumberOfSections - 1) * IMAGE_SIZEOF_SECTION_HEADER);

		if (dwFoa >= pImageSectionHead->PointerToRawData + pImageSectionHead->SizeOfRawData)	//判断foa是否超出范围
			return 0;
		else {
			i = pImageFileHead->NumberOfSections - 1;

			do		//循环遍历节表
			{
				if (dwFoa >= pImageSectionHead->PointerToRawData)	//判断foa是否在当前节块
				{
					dwRva = dwFoa - pImageSectionHead->PointerToRawData + pImageSectionHead->VirtualAddress;
					break;
				}
				pImageSectionHead--;
				i--;
			} while (i >= 0);
		}
	}
	return dwRva;
}


ADWORD rvaToFoa(IN VOID* pFileBuff, IN ADWORD dwRva) {
	PIMAGE_FILE_HEADER pImageFileHead = NULL;
	PIMAGE_OPTIONAL_HEADER pImageOptionalHead = NULL;
	PIMAGE_SECTION_HEADER pImageSectionHead = NULL;
	int i = 0;
	ADWORD dwFoa = 0;

	if (getPEHeader(pFileBuff, 0, &pImageFileHead, &pImageOptionalHead) != 2)	//获取PE头
		return 0;

	if (dwRva < pImageOptionalHead->SizeOfHeaders)		//判断rva是否在头部
	{
		return dwRva;
	}
	else if(dwRva >= pImageOptionalHead->SizeOfImage){	//判断rva是否超出image大小
		return 0;
	}
	else {
		if (!getPELastSectionHeader(pFileBuff, pImageSectionHead))		//判断节表是否获取成功
			return 0;

		i = pImageFileHead->NumberOfSections;
		while (i > 0)	//循环节表
		{
			if (dwRva >= pImageSectionHead->VirtualAddress)		//判断rva是否在当前节块
			{
				dwFoa = dwRva - pImageSectionHead->VirtualAddress + pImageSectionHead->PointerToRawData;
				break;
			}
			pImageSectionHead--;
			i--;
		}
	}
	return dwFoa;
}

DWORD64 rvaToFoa64(IN DWORD64* pFileBuff, IN DWORD64 dwRva) {
	PIMAGE_FILE_HEADER pImageFileHead = NULL;
	PIMAGE_OPTIONAL_HEADER pImageOptionalHead = NULL;
	PIMAGE_SECTION_HEADER pImageSectionHead = NULL;
	int i = 0;
	DWORD64 dwFoa = 0;

	if (getPEHeader(pFileBuff, 0, &pImageFileHead, &pImageOptionalHead) != 2)	//获取PE头
		return 0;

	if (dwRva < pImageOptionalHead->SizeOfHeaders)		//判断rva是否在头部
	{
		return dwRva;
	}
	else if (dwRva >= pImageOptionalHead->SizeOfImage) {	//判断rva是否超出image大小
		return 0;
	}
	else {
		if (!getPELastSectionHeader(pFileBuff, pImageSectionHead))		//判断节表是否获取成功
			return 0;

		i = pImageFileHead->NumberOfSections;
		while (i > 0)	//循环节表
		{
			if (dwRva >= pImageSectionHead->VirtualAddress)		//判断rva是否在当前节块
			{
				dwFoa = dwRva - pImageSectionHead->VirtualAddress + pImageSectionHead->PointerToRawData;
				break;
			}
			pImageSectionHead--;
			i--;
		}
	}
	return dwFoa;
}



DWORD getSectionName(PIMAGE_SECTION_HEADER pImageSectionHead, PTCHAR* ptBuff, DWORD dwBuffSize) {
	if (ptBuff == NULL)
	{
		return -1;
	}
	char* name = (char*)pImageSectionHead->Name;
	PTCHAR ptText = *ptBuff;
	int i = 0;
	while (i < 8)
	{
		if (*name == '\0')
		{
			break;
		}
		MultiByteToWideChar(CP_UTF8, 0, name, 1, ptText, dwBuffSize);
#ifdef UNICODE
		ptText = (PTCHAR)((ADWORD)ptText + 2);
		dwBuffSize -= 2;
#else
		ptText = (PTCHAR)((ADWORD)ptText + 1);
		dwBuffSize -= 1;
#endif
		name += 1;
		i++;
	}
	return i;
}