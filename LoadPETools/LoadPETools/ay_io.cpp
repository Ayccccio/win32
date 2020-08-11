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
	DWORD dwNum = sizeof var * 8;
	if (index > dwNum || index == 0)
	{
		return -1;
	}

	if (index == 1)
	{
		return var & 1;
	}
	else
		return var & (T)pow(2, index - 1);
}


DWORD setBitOfIndex(DWORD* pdwData, DWORD dwIndex, BOOL var) {
	DWORD dwTemp = *pdwData;
	if (dwIndex == 1)
	{
		if (var == 1)
		{
			dwTemp |= 1;
		}
		else {
			dwTemp &= 0xfffffffe;
		}
	}
}