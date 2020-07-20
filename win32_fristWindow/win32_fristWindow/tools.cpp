#include "tools.h"

void __cdecl OutputDebugStringF(const char* format, ...)
{
	va_list vlArgs;
	char* strBuffer = (char*)GlobalAlloc(GPTR, 4096);

	va_start(vlArgs, format);
	_vsnprintf_s(strBuffer, 4096 - 1,1, format, vlArgs);
	va_end(vlArgs);
	strcat_s(strBuffer,1, "\n");
	OutputDebugStringA(strBuffer);
	GlobalFree(strBuffer);
	return;
}
