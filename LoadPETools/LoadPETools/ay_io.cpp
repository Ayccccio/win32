#include "ay_io.h"


void __cdecl OutputDebugStringF(const TCHAR* format, ...)
{
	va_list vlArgs;
	PTCHAR strBuffer = (PTCHAR)GlobalAlloc(GPTR, 4096);

	va_start(vlArgs, format);
	_vsntprintf(strBuffer, 4096 - 1, format, vlArgs);
	va_end(vlArgs);
	//StrCat(strBuffer, TEXT("\n"));
	OutputDebugString(strBuffer);
	GlobalFree(strBuffer);
	return;
}


DWORD getBitOfIndex(DWORD dwData, DWORD dwIndex) {
	DWORD dwNum = sizeof dwData * 8;
	if (dwIndex > dwNum || dwIndex == 0)
	{
		return -1;
	}

	if (dwIndex == 1)
	{
		return dwData & 1;
	}
	else
		return dwData & (DWORD)pow(2, dwIndex - 1);
}


DWORD setBitOfIndex(DWORD* pdwData, DWORD dwIndex, BOOL bVar) {
	DWORD dwTemp = *pdwData;
	if (dwIndex > 4 * 8 )
	{
		return -1;
	}
	else if (dwIndex == 1)
	{
		if (bVar == 1)
		{
			dwTemp |= 1;
		}
		else {
			dwTemp &= 0xfffffffe;
		}
	}
	else {
		if (bVar == 1)
		{
			dwTemp |= (DWORD)pow(2, dwIndex - 1);
		}
		else {
			dwTemp = dwTemp & (0xffffffff - (DWORD)pow(2, dwIndex - 1));
		}
	}
	*pdwData = dwTemp;
	return dwTemp;
}



DWORD multByteToWideChar(PCHAR pcResource, int reSize, PWCHAR* pwBuff, int buffSize) {
	PWCHAR pwTemp = *pwBuff;

	int i = 0;
	while (i < buffSize)
	{
		if (reSize == 0)		//
		{
			if (*pcResource == '\0')
				break;
		}
		else
		{
			if (i >= reSize)
				break;
		}
		
		MultiByteToWideChar(CP_UTF8, 0, (LPCCH)pcResource, reSize, pwTemp, buffSize);
		pcResource++;
		pwTemp++;
		i++;
	}
	wcscat_s(pwTemp, 1,L"\0");
	return i;
}