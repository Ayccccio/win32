#include "const.h"


HINSTANCE hAPPInterface;	//������ڵ�ַ
TCHAR ptText[MAX_PATH];		//�ı�������

BOOL bFlag = FALSE;				//��Ϊ����,�����ж�һЩ�����Ƿ�ִ��

PVOID pFileBuff = NULL;			//�ļ�������
PIMAGE_FILE_HEADER pImageFileHeader = NULL;			//��׼PEͷ
PIMAGE_OPTIONAL_HEADER pImageOptionalHeader = NULL;	//��ѡPEͷ
PIMAGE_IMPORT_DESCRIPTOR pImageImport = NULL;

PWORD pwCheckBoxIndexOfFileCharacter = NULL;
DWORD dwCharacterOfFileHead = 0;