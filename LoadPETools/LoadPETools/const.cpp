#include "const.h"


HINSTANCE hAPPInterface;	//������ڵ�ַ
TCHAR ptText[MAX_PATH];		//�ı�������

BOOL bFlag = FALSE;				//��Ϊ����,�����ж�һЩ�����Ƿ�ִ��

PVOID pFileBuff = NULL;			//�ļ�������
PIMAGE_FILE_HEADER pImageFileHeader = NULL;			//��׼PEͷ
PIMAGE_OPTIONAL_HEADER32 pImageOptionalHeader32 = NULL;	//��ѡPEͷ
PIMAGE_OPTIONAL_HEADER64 pImageOptionalHeader64 = NULL;	//��ѡPEͷ
PIMAGE_EXPORT_DIRECTORY pImageExportDirectory = NULL;	//������
PIMAGE_IMPORT_DESCRIPTOR pImageImportDirectory = NULL;

PWORD pwCheckBoxIndexOfFileCharacter = NULL;
DWORD dwCharacterOfFileHead = 0;