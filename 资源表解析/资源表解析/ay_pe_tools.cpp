#include "ay_pe_tools.h"


DWORD loadPEFile(IN PTCHAR ptFileName, OUT VOID*& pFileBuff) {
	FILE* pfFile;
	DWORD dwFileSize;
	DWORD dwReadSize;

	freePeFileBuff(pFileBuff);	//�ͷŻ�����,�����ظ������ڴ�ռ�
	fileOpen(&pfFile, ptFileName, TEXT("rb+"));		//���ļ�
	if (!pfFile)
	{
		DbgPrintf(TEXT("file open fail"));
		return 0;
	}

	//��ȡ�ļ���С
	fseek(pfFile, 0, SEEK_END);
	dwFileSize = ftell(pfFile);
	fseek(pfFile, 0, SEEK_SET);

	//���뻺�����ڴ�ռ�
	pFileBuff = malloc(dwFileSize);

	//�ж��Ƿ�����ɹ�
	if (!pFileBuff)
	{
		fclose(pfFile);
		DbgPrintf(TEXT("fileBuff malloc fail"));
		return 0;
	}

	//��ȡ�ļ����������ݵ�������
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

	PIMAGE_DOS_HEADER pDosH = (PIMAGE_DOS_HEADER)pFileBuff;		//��ȡDOSͷ
	DWORD ret = 0;

	if (pDosH->e_magic != IMAGE_DOS_SIGNATURE)	//�ж�MZ���
	{
		DbgPrintf(TEXT("PE fileBuff error"));
		return 0;
	}
	if (*(DWORD*)((DWORD)pFileBuff + pDosH->e_lfanew) != IMAGE_NT_SIGNATURE)	//[�ж�PE���
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
	if (getPEHeader(pFileBuff,0,&pImageFileHead,&pImageOptionalHead))		//��ȡPEͷ
	{
		pImageSectionHead = (PIMAGE_SECTION_HEADER)((DWORD)pImageOptionalHead + pImageFileHead->SizeOfOptionalHeader);		//����ڱ�ָ��
		return TRUE;
	}
	return FALSE;
}