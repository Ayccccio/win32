#pragma once
#include <windows.h>
												 
#define ProcessListControlColumNumber 7		//�����б�ͨ�ÿؼ������� 
#define MoudelListControlColumNumber 5		//ģ���б�ͨ�ÿؼ������� 
#define SectionListControlColumNumber 6		//�ڿ��б�ͨ�ÿؼ������� 
#define ImportDllListControlColumNumber 7		//�����DLL�б�ͨ�ÿؼ������� 
#define ImportFunListControlColumNumber 5		//��������б�ͨ�ÿؼ������� 


extern HINSTANCE hAPPInterface;		//������ڵ�ַ
extern TCHAR ptText[MAX_PATH];		//�ı�������

extern BOOL bFlag;					//��Ϊ����,�����ж�һЩ�����Ƿ�ִ��

extern PVOID pFileBuff;				//�ļ�������
extern PIMAGE_FILE_HEADER pImageFileHeader;			//��׼PEͷ
extern PIMAGE_OPTIONAL_HEADER pImageOptionalHeader;	//��ѡPEͷ
extern PIMAGE_IMPORT_DESCRIPTOR pImageImport;		//�����


extern PWORD pwCheckBoxIndexOfFileCharacter;			//��׼PEͷ������ѡ������ָ��
extern DWORD dwCharacterOfFileHead;					//��׼PEͷ����ֵ