#include "ay_pe_tools.h"


DWORD loadPEFile(IN PTCHAR ptFileName, OUT VOID*& pFileBuff) {
	FILE* pfFile;
	DWORD dwFileSize;
	DWORD dwReadSize;

	freePeFileBuff(pFileBuff);	//释放缓冲区,避免重复申请内存空间
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
	pFileBuff = malloc(dwFileSize);

	//判断是否申请成功
	if (!pFileBuff)
	{
		fclose(pfFile);
		DbgPrintf(TEXT("fileBuff malloc fail"));
		return 0;
	}

	//读取文件二进制数据到缓冲区
	dwReadSize = fread_s(pFileBuff, dwFileSize, dwFileSize, 1, pfFile) * dwFileSize;
	fclose(pfFile);
	return dwReadSize;
}

void freePeFileBuff(VOID* pFileBuff)
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
	if (*(DWORD*)((DWORD)pFileBuff + pDosH->e_lfanew) != IMAGE_NT_SIGNATURE)	//[判断PE标记
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
		*pImageFileHead = (PIMAGE_FILE_HEADER)((DWORD)pFileBuff + pDosH->e_lfanew + 4);
		ret++;
	}
	if (pImageOptionalHead != NULL)
	{
		*pImageFileHead = (PIMAGE_FILE_HEADER)((DWORD)pFileBuff + pDosH->e_lfanew + 4 + IMAGE_SIZEOF_FILE_HEADER);
		ret++;
	}
}


BOOL getPESectionHeader(IN VOID* pFileBuff, OUT PIMAGE_SECTION_HEADER& pImageSectionHead) {
	PIMAGE_FILE_HEADER pImageFileHead = NULL;
	PIMAGE_OPTIONAL_HEADER pImageOptionalHead = NULL;
	if (getPEHeader(pFileBuff,0,&pImageFileHead,&pImageOptionalHead))		//获取PE头
	{
		pImageSectionHead = (PIMAGE_SECTION_HEADER)((DWORD)pImageOptionalHead + pImageFileHead->SizeOfOptionalHeader);		//计算节表指针
		return TRUE;
	}
	return FALSE;
}