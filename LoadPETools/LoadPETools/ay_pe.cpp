#include "ay_pe.h"


DWORD loadPEFile(IN PTCHAR ptFileName, OUT PVOID* pFileBuff) {
	FILE* pfFile;
	DWORD dwFileSize;
	DWORD dwReadSize;
	PVOID ptemp;

	freePeFileBuff(*pFileBuff);	//�ͷŻ�����,�����ظ������ڴ�ռ�
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
	ptemp = malloc(dwFileSize);

	//�ж��Ƿ�����ɹ�
	if (!ptemp)
	{
		fclose(pfFile);
		DbgPrintf(TEXT("fileBuff malloc fail"));
		return 0;
	}

	//��ȡ�ļ����������ݵ�������
	dwReadSize = fread_s(ptemp, dwFileSize, dwFileSize, 1, pfFile) * dwFileSize;
	fclose(pfFile);
	*pFileBuff = ptemp;
	return dwReadSize;
}

void freePeFileBuff(PVOID pFileBuff)
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
		*pImageOptionalHead = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileBuff + pDosH->e_lfanew + 4 + IMAGE_SIZEOF_FILE_HEADER);
		ret++;
	}
	return ret;
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


BOOL getPELastSectionHeader(IN VOID* pFileBuff, OUT PIMAGE_SECTION_HEADER& pImageSectionHead) {
	PIMAGE_FILE_HEADER pImageFileHead = NULL;
	PIMAGE_OPTIONAL_HEADER pImageOptionalHead = NULL;
	if (getPEHeader(pFileBuff, 0, &pImageFileHead, &pImageOptionalHead))		//��ȡPEͷ
	{
		pImageSectionHead = (PIMAGE_SECTION_HEADER)((DWORD)pImageOptionalHead + 
			pImageFileHead->SizeOfOptionalHeader + 
			(pImageFileHead->NumberOfSections - 1) * 
			IMAGE_SIZEOF_SECTION_HEADER);		//����ڱ�ָ��
		return TRUE;
	}
	return FALSE;
}


DWORD foaToRva(IN VOID* pFileBuff,IN DWORD dwFoa) {
	PIMAGE_FILE_HEADER pImageFileHead = NULL;
	PIMAGE_OPTIONAL_HEADER pImageOptionalHead = NULL;
	PIMAGE_SECTION_HEADER pImageSectionHead = NULL;
	int i = 0;
	DWORD dwRva;

	if (getPEHeader(pFileBuff, 0, &pImageFileHead, &pImageOptionalHead) != 2)	//��ȡPEͷ
		return 0;

	if (dwFoa < pImageOptionalHead->SizeOfHeaders)		//�ж�foa�Ƿ���ͷ��
	{
		return dwFoa;
	}
	else {
		if (!getPESectionHeader(pFileBuff, pImageSectionHead))		//�жϽڱ��Ƿ��ȡ�ɹ�
			return 0;
		pImageSectionHead = (PIMAGE_SECTION_HEADER)((DWORD)pImageSectionHead +		//��ȡ���һ���ڱ�
			(pImageFileHead->NumberOfSections - 1) * IMAGE_SIZEOF_SECTION_HEADER);

		if (dwFoa >= pImageSectionHead->PointerToRawData + pImageSectionHead->SizeOfRawData)	//�ж�foa�Ƿ񳬳���Χ
			return 0;
		else {
			i = pImageFileHead->NumberOfSections - 1;

			do		//ѭ�������ڱ�
			{
				if (dwFoa >= pImageSectionHead->SizeOfRawData)	//�ж�foa�Ƿ��ڵ�ǰ�ڿ�
				{
					dwRva = pImageSectionHead->VirtualAddress + dwFoa - pImageSectionHead->SizeOfRawData;
				}
				pImageSectionHead--;
				i--;
			} while (i >= 0);
		}
	}
	return dwRva;
}


DWORD rvaToFoa(IN VOID* pFileBuff, IN DWORD dwRva) {
	PIMAGE_FILE_HEADER pImageFileHead = NULL;
	PIMAGE_OPTIONAL_HEADER pImageOptionalHead = NULL;
	PIMAGE_SECTION_HEADER pImageSectionHead = NULL;
	int i = 0;
	DWORD dwFoa;

	if (getPEHeader(pFileBuff, 0, &pImageFileHead, &pImageOptionalHead) != 2)	//��ȡPEͷ
		return 0;

	if (dwRva < pImageOptionalHead->SizeOfHeaders)		//�ж�rva�Ƿ���ͷ��
	{
		return dwRva;
	}
	else if(dwRva >= pImageOptionalHead->SizeOfImage){	//�ж�rva�Ƿ񳬳�image��С
		return 0;
	}
	else {
		if (!getPELastSectionHeader(pFileBuff, pImageSectionHead))		//�жϽڱ��Ƿ��ȡ�ɹ�
			return 0;

		i = pImageFileHead->NumberOfSections;
		while (i > 0)	//ѭ���ڱ�
		{
			if (dwRva >= pImageSectionHead->VirtualAddress)		//�ж�rva�Ƿ��ڵ�ǰ�ڿ�
			{
				dwFoa = dwRva - pImageSectionHead->VirtualAddress + pImageSectionHead->PointerToRawData;
			}
			pImageSectionHead--;
			i--;
		}
	}
	return dwFoa;
}


DWORD resourceParse(IN PIMAGE_RESOURCE_DIRECTORY pImageResourceDirectory, OUT PTCHAR ptStrBuff, DWORD dwBuffSize) {
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pImageResourceDirectoryEntry = NULL;
	DWORD dwDirNum = 0;
	int i = 0;
	if (pImageResourceDirectory == NULL)	//�ж���Դ��ͷ�Ƿ���Ч
		return 0;

	//��ȡĿ¼��ڸ���
	dwDirNum =	pImageResourceDirectory->NumberOfIdEntries + pImageResourceDirectory->NumberOfNamedEntries;
	if (!dwDirNum)
		return 0;
	while (i < dwDirNum)
	{
		pImageResourceDirectoryEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)((DWORD)pImageResourceDirectory + 16 + i * 8);

	}

}