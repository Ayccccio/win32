#pragma once
#include <windows.h>

#define ProcessListControlColumNumber 7		//����ͨ�ÿؼ������� 
#define MoudelListControlColumNumber 5		//ģ��ͨ�ÿؼ������� 


extern HINSTANCE hAPPInterface;		//������ڵ�ַ
extern TCHAR ptText[MAX_PATH];		//�ı�������

extern BOOL bFlag;					//��Ϊ����,�����ж�һЩ�����Ƿ�ִ��

extern PVOID pFileBuff;				//�ļ�������
extern PIMAGE_FILE_HEADER pFileHeader;			//��׼PEͷ
extern PIMAGE_OPTIONAL_HEADER pOptionalHeader;	//��ѡPEͷ