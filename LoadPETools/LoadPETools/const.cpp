#include "const.h"


HINSTANCE hAPPInterface;	//������ڵ�ַ
TCHAR ptText[MAX_PATH];		//�ı�������

BOOL bOpenFileNameFlag = FALSE; //�ļ��Ի����Ƿ��,�����ظ���

PVOID pFileBuff = NULL;			//�ļ�������
PIMAGE_FILE_HEADER pFileHeader = NULL;			//��׼PEͷ
PIMAGE_OPTIONAL_HEADER pOptionalHeader = NULL;	//��ѡPEͷ

