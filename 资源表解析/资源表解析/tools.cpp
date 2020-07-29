#define  _CRT_SECURE_NO_WARNINGS

#include "tools.h"


void __cdecl OutputDebugStringF(const TCHAR* format, ...)
{
	va_list vlArgs;
	PTCHAR strBuffer = (PTCHAR)GlobalAlloc(GPTR, 4096);

	va_start(vlArgs, format);
	_vsntprintf(strBuffer, 4096 - 1, format, vlArgs);
	va_end(vlArgs);
	StrCat(strBuffer, TEXT("\n"));
	OutputDebugString(strBuffer);
	GlobalFree(strBuffer);
	return;
}


DWORD strrpl(PTCHAR resource, PTCHAR oldVal, PTCHAR newVal) {
	DWORD  oldLen = strLen(oldVal);
	DWORD  newLen = strLen(newVal);
	DWORD  reLen = strLen(resource);
	DWORD count = 0;
	if (oldLen != newLen)
	{
		return -1;
	}
	else {
		for (int i = 0; i < reLen; i++)
		{
			for (int j = 0; j < oldLen; j++)
			{
				if (*(WORD*)((DWORD)resource + i * 2 + j * 2) != *(WORD*)((DWORD)oldVal + j * 2))
				{
					break;
				}
				if (j == oldLen - 1)
				{
					*(WORD*)((DWORD)resource + i * 2 + j * 2) = *(WORD*)((DWORD)newVal + j * 2);
					count++;
				}
			}
		}
		
	}
}

DWORD loadPEFile(IN PTCHAR ptFileName, OUT VOID* &pFileBuff) {
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
