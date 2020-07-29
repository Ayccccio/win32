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

