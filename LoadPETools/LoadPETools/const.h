#pragma once
#include <windows.h>

#define ProcessListControlColumNumber 7		//����ͨ�ÿؼ������� 
#define MoudelListControlColumNumber 5		//ģ��ͨ�ÿؼ������� 
#define SectionListControlColumNumber 6		//�ڿ�ͨ�ÿؼ������� 
#define ExportListControlColumNumber 4		//�ڿ�ͨ�ÿؼ������� 
#define ImportDllListControlColumNumber 8		//�����DLL�б�ͨ�ÿؼ������� 
#define ImportFunListControlColumNumber 6		//��������б�ͨ�ÿؼ������� 



extern HINSTANCE hAPPInterface;		//������ڵ�ַ
extern TCHAR ptText[MAX_PATH];		//�ı�������

extern BOOL bFlag;					//��Ϊ����,�����ж�һЩ�����Ƿ�ִ��

extern PVOID pFileBuff;				//�ļ�������
extern PIMAGE_FILE_HEADER pImageFileHeader;			//��׼PEͷ
extern PIMAGE_OPTIONAL_HEADER32 pImageOptionalHeader32;	//��ѡPEͷ
extern PIMAGE_OPTIONAL_HEADER64 pImageOptionalHeader64;	//��ѡPEͷ
extern PIMAGE_EXPORT_DIRECTORY pImageExportDirectory;	//������
extern PIMAGE_IMPORT_DESCRIPTOR pImageImportDirectory;	//�����

extern PWORD pwCheckBoxIndexOfFileCharacter;		//��׼PEͷ������ѡ������ָ��
extern DWORD dwCharacterOfFileHead;					//��׼PEͷ����ֵ