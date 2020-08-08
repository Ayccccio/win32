#pragma once
#include <windows.h>

#define ProcessListControlColumNumber 7		//进程通用控件列数量 
#define MoudelListControlColumNumber 5		//模块通用控件列数量 


extern HINSTANCE hAPPInterface;		//程序入口地址
extern TCHAR ptText[MAX_PATH];		//文本缓冲区

extern BOOL bFlag;					//作为开关,用于判断一些程序是否执行

extern PVOID pFileBuff;				//文件缓冲区
extern PIMAGE_FILE_HEADER pFileHeader;			//标准PE头
extern PIMAGE_OPTIONAL_HEADER pOptionalHeader;	//可选PE头