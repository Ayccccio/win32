#include "const.h"


HINSTANCE hAPPInterface;	//程序入口地址
TCHAR ptText[MAX_PATH];		//文本缓冲区

BOOL bFlag = FALSE;				//作为开关,用于判断一些程序是否执行

PVOID pFileBuff = NULL;			//文件缓冲区
PIMAGE_FILE_HEADER pImageFileHeader = NULL;			//标准PE头
PIMAGE_OPTIONAL_HEADER pImageOptionalHeader = NULL;	//可选PE头

PWORD pwCheckBoxIndexOfFileCharacter = NULL;
DWORD dwCharacterOfFileHead = 0;