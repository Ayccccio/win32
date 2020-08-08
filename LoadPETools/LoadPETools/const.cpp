#include "const.h"


HINSTANCE hAPPInterface;	//程序入口地址
TCHAR ptText[MAX_PATH];		//文本缓冲区

BOOL bOpenFileNameFlag = FALSE; //文件对话框是否打开,避免重复打开

PVOID pFileBuff = NULL;			//文件缓冲区
PIMAGE_FILE_HEADER pFileHeader = NULL;			//标准PE头
PIMAGE_OPTIONAL_HEADER pOptionalHeader = NULL;	//可选PE头

