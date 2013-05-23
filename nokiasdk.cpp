// nokiasdk.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "nokiasdk.h"
#include <string>
#include <wchar.h>
#include <winioctl.h>
#include <ctime> 
//#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
using namespace std;
/*
�޸���־:
  2011-8-1 �޸�Lua��װ�Ĳ�������BUG 
  2011-8-24 ������־����Ĺ���,�̶������c:\nokiasms.log,û������,�ļ��᲻�ϵ�����
*/
//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CNokiasdkApp

BEGIN_MESSAGE_MAP(CNokiasdkApp, CWinApp)
	//{{AFX_MSG_MAP(CNokiasdkApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void*hMutex;
char*strMutex = "Junction-vqap-ats";



void Mutex_Init()
{
    hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, strMutex);
    if(hMutex == NULL)
    {
		Sys_lua_addlog("Mutex δ����");
        bool bExists = false;
        CreateMutex(NULL, FALSE, strMutex);
        DWORD dwRet = GetLastError();
        if(dwRet == 0)
        {
			Sys_lua_addlog("Mutex �����ɹ�");
            bExists = true;
        }
        else if(dwRet == ERROR_ALREADY_EXISTS)
        {
            bExists = true; 
        }
        if(bExists)
        {
            hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, strMutex);
        }else
			hMutex = NULL;
    }
	if(hMutex)
		Sys_lua_addlog("��Mutex�ɹ�");
	else
		Sys_lua_addlog("��Mutexʧ��");
}

void Mutex_Wait()
{
	Sys_lua_addlog("�ȴ��С�����");
	WaitForSingleObject(hMutex,INFINITE);
	Sys_lua_addlog("�ɹ��ȵ�");
}

void Mutex_Free()
{
	ReleaseMutex(hMutex);
	Sys_lua_addlog("����һ��");
}


char sLogBuffer[1024];


void CNokiasdkApp::_ShowMsg(char*msg)
{
	//::MessageBox(0, msg, "Hint", 0);
	Sys_addlog(msg);
}

DWORD __stdcall Sys_lua_addlog(const char* log)
{
	//::MessageBox(0, "log", "Hint", 0);
	try
	{
		FILE *fp;
		char sfilename[100];
		strcpy(sfilename, "c:\\nokiasms.log");
		if((fp=fopen(sfilename, "a"))!=NULL){
			CTime tm;
			int yy = tm.GetCurrentTime().GetYear();  
			int mm = tm.GetCurrentTime().GetMonth();  
			int dd = tm.GetCurrentTime().GetDay();  	
			int hh = tm.GetCurrentTime().GetHour();
			int mi = tm.GetCurrentTime().GetMinute();
			int se = tm.GetCurrentTime().GetSecond();
			CString str;
			str.Format("%02d-%02d-%02dT%02d:%02d:%02d\t", yy,mm,dd,hh,mi,se);
			fputs(str, fp);
			fputs(log, fp);
			fputs("\n", fp);
			fclose(fp);
		}//else
		//	::MessageBox(0, "file open error", "Hint", 0);
	}catch(...)
	{
		//::MessageBox(0, "log error", "Hint", 0);
	}
	return 0;
}

DWORD  CNokiasdkApp::Sys_addlog(const char* log)
{
	try
	{
		FILE *fp;
		char sfilename[100];
		strcpy(sfilename, "c:\\nokiasms.log");
		if((fp=fopen(sfilename, "a"))!=NULL){
			CTime tm;
			int yy = tm.GetCurrentTime().GetYear();  
			int mm = tm.GetCurrentTime().GetMonth();  
			int dd = tm.GetCurrentTime().GetDay();  	
			int hh = tm.GetCurrentTime().GetHour();
			int mi = tm.GetCurrentTime().GetMinute();
			int se = tm.GetCurrentTime().GetSecond();
			CString str;
			str.Format("%02d-%02d-%02dT%02d:%02d:%02d\t%s\t", yy,mm,dd,hh,mi,se,str_imei);
			fputs(str, fp);
			fputs(log, fp);
			fputs("\n", fp);
			fclose(fp);
		} 
	}catch(...)
	{
		 
	}
	return 0;
}

DWORD COperationNotify::OnCAOperationNotify(DWORD dwOperation,
											DWORD dwCurrent,
											DWORD dwTotalAmount,
											DWORD dwStatus,
											CA_ITEM_ID* pItemID)
{
	/*LPWSTR pstrUid = NULL;
	ItemUidToString(pItemID, pstrUid);
	SYSTEMTIME sTime;
	::GetLocalTime(&sTime);
	wchar_t szDate[128] = {0};
	GetFormattedDate(&sTime, szDate, 128);
	CString strStatus = L"";
	strStatus.Format(L"%s OnCAOperationNotify: Operation(%d), progress(%d), total(%d), status(%d), item(%s)",
		szDate, dwOperation, dwCurrent, dwTotalAmount, dwStatus, pstrUid);
	if(pstrUid)
	{
		delete [] pstrUid;
	}
	if(g_NotificationsDlg)
	{
		g_NotificationsDlg->InsertNotification(strStatus);
	}
	strStatus += L"\n";
	TRACE(strStatus);*/
	return CONA_OK;
}


WCHAR* Ascii2Wchar(char*src, WCHAR*dst)
{
	const   char   *pSN   =  src;
    int   nLen   =   strlen(pSN)   +   1;
    int   nwLen   =   MultiByteToWideChar(CP_ACP,   0,   pSN,   nLen,   NULL,   0);
    MultiByteToWideChar(CP_ACP,   0,   pSN,   nLen,   dst,   nwLen); 
	return dst;
}

/////////////////////////////////////////////////////////////////////////////
// CNokiasdkApp construction

CNokiasdkApp::CNokiasdkApp()
{
	_ShowMsg("DLL ��ʼ��");
	m_hDM = NULL;
	m_hSMS = NULL;
	//m_hMMS = NULL;
	hFS = NULL;
}

int CNokiasdkApp::Nokia_Init()
{
	DWORD dwRet	= CONA_OK;
	dwRet = DMAPI_Initialize(DMAPI_VERSION_32, NULL);
	if (dwRet != CONA_OK)
	{
		return 1;
	}
		// Initialize Content Access API
	dwRet = CAAPI_Initialize(CAAPI_VERSION_31, NULL);
	DWORD dwRet1 = FSAPI_Initialize(FSAPI_VERSION_32, NULL);
	if (dwRet != CONA_OK || dwRet1 != CONA_OK)
	{
		DMAPI_Terminate(NULL);
		return 2;
	}

	// Initialize Common Functions API
	dwRet = CFAPI_Initialize(CFAPI_VERSION_10, NULL);
	if (dwRet != CONA_OK)
	{
		DMAPI_Terminate(NULL);
		CAAPI_Terminate(NULL);
		FSAPI_Terminate(NULL);
		return 3;
	}
	return CONA_OK;
}

void CNokiasdkApp::Nokia_Uninit()
{
	Sys_lua_addlog("UnInit");
	if(m_hDM)
	{
		CONACloseDM(m_hDM);
		m_hDM = NULL;
	}
	if(m_hSMS)
		CloseConnection(m_hSMS);
	//if(m_hMMS)
	//	CloseConnection(m_hMMS);
	// Terminate Common Functions API
	CFAPI_Terminate(NULL);
	// Terminate Content Access API
	FSAPI_Terminate(NULL);
	CAAPI_Terminate(NULL);
	// Terminate Device Management API
	DMAPI_Terminate(NULL);
}

DWORD CNokiasdkApp::CloseConnection(CAHANDLE& hCA)
{
	DWORD dwRet = CONA_OK;
	if(hCA)
	{
		// Close CA connection
		dwRet = DACloseCA(hCA);
		if(dwRet != CONA_OK)
		{
			//ErrorMessageDlg(_T("DACloseCA"), dwRet);
		}
		wcscpy(m_strOldSrc, _L(""));
		hCA = NULL;
	}
	return dwRet;
}

DWORD CNokiasdkApp::CheckSMSConnection(WCHAR* SN)
{
	DWORD dwRet = CONA_OK;
	int iCmp = wcscmp(SN, m_strOldSrc);
	if(iCmp != 0)
		CloseConnection(m_hSMS);

	_ShowMsg("Enter CheckSMSConnection");
	if(!m_hSMS)
	{	// No CA connection, open it
		DWORD dwMedia = API_MEDIA_ALL;
		dwRet = DAOpenCA(SN, &dwMedia, CA_TARGET_SMS_MESSAGES, &m_hSMS);
		if(dwRet == CONA_OK)
		{
			wcscpy(m_strOldSrc, SN);
			_ShowMsg("SMS DAOpenCA Ok");
		}
		else
			_ShowMsg("SMS DAOpenCA Error");
	}
 
	return dwRet;
}

void CNokiasdkApp::GetFloderInfo(CA_FOLDER_INFO *Info)
{
	memcpy(&m_fiInBox, Info, sizeof(CA_FOLDER_INFO));
}


//��ȡ��ͨ�����ռ����Ŀ¼
DWORD CNokiasdkApp::GetSMSFolders(WCHAR*SN, DWORD dwFolderId)
{
		// Check CA connection to SMS folders and open it if needed
	_ShowMsg("Enter GetSMSFloders");
	DWORD dwRet = CheckSMSConnection(SN);
	if(dwRet == CONA_OK)
	{	// Get SMS folder target paths
		CA_FOLDER_INFO pFolderInfo = {0};
		pFolderInfo.dwSize = sizeof(pFolderInfo);
		dwRet = CAGetFolderInfo(m_hSMS, &pFolderInfo);
		if(dwRet == CONA_OK)
		{
			if(pFolderInfo.dwFolderId == dwFolderId)
				GetFloderInfo(&pFolderInfo);
			else if(pFolderInfo.dwSubFolderCount>0)
			{
				_ShowMsg("Ŀ¼����>0");
				char tmpBuffer[20];
				for(DWORD i = 0; i < pFolderInfo.dwSubFolderCount; i++)
				{
					sprintf(tmpBuffer, "Item[%d]:%d, Obj:%d", i, pFolderInfo.pSubFolders[i].dwFolderId,dwFolderId);
					_ShowMsg(tmpBuffer);
					if(pFolderInfo.pSubFolders[i].dwFolderId == dwFolderId)
					{
						GetFloderInfo(&pFolderInfo.pSubFolders[i]);
						break;
					}
				}
			}
		}
	}else
	{
		_ShowMsg("���SMS����ʧ��");
	}
	return dwRet;
}

char* WChar2Ascii(WCHAR*Data, DWORD dwLen, char*buffer)
{
		//WCharת����Char
    char buf[200];
    int   nLen   =   wcslen(Data)+1;
    WideCharToMultiByte(CP_ACP,   0,   Data,   nLen,   buf,   2*nLen,   NULL,   NULL);
	strcpy(buffer, buf);
	return buffer;

}

char* CNokiasdkApp::Unicode2Ascii(BYTE*Data, DWORD dwLen, char*buffer)
{
	//���ڴ��Byte�������WCHAR���
	WCHAR wStr[500]={0};
    if(dwLen > 500)
        dwLen = 500;
    unsigned short latter;
    int index=0;
    for(DWORD i=0; i<dwLen; i+=2)
    {
        if(i<dwLen && (i+1)<dwLen)
        {
            latter = Data[i+1] <<8 | Data[i];
            wStr[index++] = latter;
        }
    }
	//WCharת����Char
    char buf[200];
    int   nLen   =   wcslen(wStr)+1;
    WideCharToMultiByte(CP_ACP,   0,   wStr,   nLen,   buf,   2*nLen,   NULL,   NULL);
	strcpy(buffer, buf);
	return buffer;
}

BYTE* CNokiasdkApp::Ascii2Unicode(BYTE*Data, DWORD dwLen, char*buffer)
{
	//���ڴ��Byte�������WCHAR���

    unsigned char latter,l1;
    int index=0;
    for(DWORD i=0; i<strlen(buffer); i++)
    {
		latter = buffer[i];
		if(latter>0 && latter <0x80)
		{
			Data[index++] = latter;
			Data[index++] = 0x00;
		}else
		{
			l1 = buffer[i+1];
			Data[index + 1] = latter;
			Data[index] = l1;
			index += 2;
			i++;
		}
    }
	return Data;
}

/*	
CA_DATA_ADDRESS * GetSMSAddressBuffer(char*strNumber)
{
    CA_DATA_ADDRESS *dataAddress;
	dataAddress = new CA_DATA_ADDRESS();
	dataAddress->dwSize = sizeof(dataAddress); 
	dataAddress->dwAddressInfo = CA_MSG_ADDRESS_TYPE_NUMBER;
	Ascii2Wchar(strNumber, dataAddress->pstrAddress);
    return dataAddress;
}*/

void CNokiasdkApp::GetCurrentPIMDate(CA_DATA_DATE *pimDate)
{
	CTime t1;
	t1 = CTime::GetCurrentTime();
	pimDate->dwSize = sizeof(CA_DATA_DATE);
	pimDate->wYear = (unsigned short)t1.GetYear();
	pimDate->bMonth = (BYTE)t1.GetMonth();
	pimDate->bDay = (BYTE)t1.GetDay();
	pimDate->bHour = (BYTE)t1.GetHour();
	pimDate->bMinute = (BYTE)t1.GetMinute();
	pimDate->bSecond = (BYTE)t1.GetSecond();
}

DWORD CNokiasdkApp::WriteItem(DWORD dwFolderId, LPVOID pItemData)
{
	CAOPERATIONHANDLE hOperHandle = NULL;
	DWORD dwRet = CABeginOperation(m_hSMS, 0, &hOperHandle);
	DWORD dwResult = 1;
	if(dwRet != CONA_OK)
	{
		//ErrorMessageDlg(_T("CABeginOperation"), dwRet);
		return 1;
	}
	if(hOperHandle == 0)
	{
		_ShowMsg("OperHandle is null");
		return 2;
	}

	COperationNotify* pOpNotify = new COperationNotify;
	dwRet = CARegisterOperationNotifyIF(hOperHandle, API_REGISTER, pOpNotify);
	if(dwRet != CONA_OK)
	{
		_ShowMsg(_T("CARegisterOperationNotifyIF"));
	}

	CA_ITEM_ID itemUid = {0};
	itemUid.dwSize = sizeof(itemUid);
	itemUid.dwFolderId = dwFolderId;
	itemUid.dwTemporaryID = 0;
	itemUid.dwStatus = 0;
	itemUid.dwUidLen = 0;
	itemUid.pbUid = NULL;
	_ShowMsg("begin write");
	dwRet = CAWriteItem(hOperHandle, &itemUid, 0, CA_DATA_FORMAT_STRUCT, pItemData);
	if(dwRet == CONA_OK)
	{
		_ShowMsg("Write Ok");
		dwRet = CACommitOperations(hOperHandle, NULL);	
		if(dwRet != CONA_OK)
		{
			
			_ShowMsg("Commit Error");
			//ErrorMessageDlg(_T("CACommitOperations"), dwRet);
		}else
			dwResult = 0;
	}
	else
	{
		if(dwRet == ECONA_INVALID_HANDLE)
			_ShowMsg("ECONA_INVALID_HANDLE");
		else if(dwRet == ECONA_INVALID_PARAMETER)
			_ShowMsg("ECONA_INVALID_PARAMETER");
		else if(dwRet == ECONA_INVALID_POINTER)
			_ShowMsg("ECONA_INVALID_POINTER");
		else if(dwRet == ECONA_CONNECTION_LOST)
			_ShowMsg("ECONA_CONNECTION_LOST");
		else
		 _ShowMsg("Write ECONA_UNKNOWN_ERROR");
		//ErrorMessageDlg(_T("DAWriteItem"), dwRet);
	}
	// Free memory allocated by CA API
	/*try
	{
		//_ShowMsg("Free Write Item");
		dwRet = CAFreeItemData(m_hSMS, CA_DATA_FORMAT_STRUCT, pItemData);
		if(dwRet != CONA_OK)
		{
			_ShowMsg("Free Write Item Error");
			//ErrorMessageDlg(_T("CAFreeItemData"), dwRet);
		}//else
		//	_ShowMsg("Free Write Item Ok");
	}catch(...){}*/
	_ShowMsg("Un Reg Write ");
	dwRet = CARegisterOperationNotifyIF(hOperHandle, API_UNREGISTER, pOpNotify);
	if(dwRet != CONA_OK)
	{
		_ShowMsg(_T("CARegisterOperationNotifyIF"));
	}else
		_ShowMsg("Un Reg Write Ok");
	delete pOpNotify;

	_ShowMsg("End Opera");
	dwRet = CAEndOperation(hOperHandle);
	if(dwRet != CONA_OK)
	{
		//ErrorMessageDlg(_T("CAEndOperation"), dwRet);
		_ShowMsg("End Opera Error");
	}else
		_ShowMsg("End Opera Ok");
	return dwResult;
}

DWORD CNokiasdkApp::WriteSMS(WCHAR*SN, CA_FOLDER_INFO *pTargetFloder, char* MobNo, char*txt)
{
	_ShowMsg("Enter WriteSMS");
	DWORD dwRet = CheckSMSConnection(SN);
	DWORD dwWrite = 1;
	if(dwRet == CONA_OK)
	{

		//����Ŀ�����,����ֻ֧��һ������
		CA_DATA_MSG dataMsg  ={0};
		dataMsg.dwSize = sizeof(CA_DATA_MSG);
		CA_SET_DATA_FORMAT(dataMsg.dwInfoField, CA_DATA_FORMAT_UNICODE);
		CA_SET_DATA_CODING(dataMsg.dwInfoField, CA_DATA_CODING_UNICODE);
		CA_SET_MESSAGE_STATUS(dataMsg.dwInfoField, CA_MESSAGE_STATUS_DRAFT);
		CA_SET_MESSAGE_TYPE(dataMsg.dwInfoField, CA_SMS_SUBMIT);

		//�ֻ����������ݵĲ���
		char*strNumber = MobNo;//"13858061334";
		int iLength = strlen(strNumber);
		WCHAR *pNum=NULL;
		BYTE*bData;
		//�����ֻ����������
		pNum = (LPWSTR)CoTaskMemAlloc((iLength+1)*sizeof(WCHAR));
		memset(pNum, 0, (iLength+1)*sizeof(WCHAR));
		//��ʼ���ֻ�����
		CA_DATA_ADDRESS* pDataAddress = (CA_DATA_ADDRESS*)CoTaskMemAlloc(sizeof(CA_DATA_ADDRESS));
		pDataAddress->dwSize = sizeof(CA_DATA_ADDRESS);
		pDataAddress->dwAddressInfo = CA_MSG_ADDRESS_TYPE_NUMBER;
		//ת���ֻ�����
		bData = Ascii2Unicode((BYTE*)pNum, (iLength)*sizeof(WCHAR), strNumber);
		pDataAddress->pstrAddress = (WCHAR*)pNum;
		dataMsg.bAddressCount = 1;
		dataMsg.pAddress = pDataAddress;
		/*char ttmp[300],ttmp2[300];
		memset(ttmp, 0, 300);
		for(DWORD i=0;i<(iLength)*sizeof(WCHAR);i++)
		{
			sprintf(ttmp2, "%02x", bData[i]);
			sprintf(ttmp, "%s,%s", ttmp, ttmp2);
		}
		_ShowMsg(ttmp);*/

		/////Message body text
		//iLength = 1;//strlen(txt);
		//dataMsg.dwDataLength = iLength * 2;
		//pStr = (LPWSTR)CoTaskMemAlloc((iLength) * sizeof(WCHAR));

		//���ｫ�����ַ�����ת��
		char*strMsg = txt;
		//2011-9-21 23:32 by hupo
		//ת����wchar_t,Ȼ����ת��Ϊunicode
		wchar_t lpszMsg[1024]={0};
		int nLen = strlen(strMsg) + 1;
		int nwLen = MultiByteToWideChar(CP_ACP,   0,   strMsg,   nLen,   NULL,   0);

	    MultiByteToWideChar(CP_ACP,   0,   strMsg,   nLen,   lpszMsg,   nwLen);
		int iwLen = wcslen(lpszMsg);//ת��Ϊwchar_t���,���Ҽ������յ��ַ�������,������1������.
		//ת��Ϊunicode
typedef union {
    unsigned char ichar[2];
    unsigned short iint;
} TINT2;
		//���䷢�����ݵ��ڴ�
		BYTE* pbMsg = (BYTE*)CoTaskMemAlloc((iwLen) * 2);
		memset(pbMsg, 0, (iwLen) * 2);
		int pbMsgIdx=0;
		TINT2 int2;
		for(int iwc=0; iwc<iwLen; iwc++)
		{
			int2.iint = lpszMsg[iwc];
			pbMsg[pbMsgIdx++] = int2.ichar[0];
			pbMsg[pbMsgIdx++] = int2.ichar[1];
		}


		dataMsg.dwDataLength = (iwLen) * 2;//sizeof(WCHAR);
		dataMsg.pbData = pbMsg;//(BYTE*)pStr;//bData;

		GetCurrentPIMDate(&dataMsg.messageDate);

		_ShowMsg("write");
		dwWrite = WriteItem(CA_MESSAGE_STATUS_SENT, &dataMsg);
		//�ͷ��ڴ�
		CoTaskMemFree(pNum);
		CoTaskMemFree(pDataAddress);
		CoTaskMemFree(pbMsg);
	}
	if(dwWrite == 0)
		_ShowMsg("ok");
	else
		_ShowMsg("error");
	return dwWrite;
}


//ɾ����ͨ���ŵ�����
DWORD CNokiasdkApp::DelSMSMessages(WCHAR*SN, CA_FOLDER_INFO *pTargetFloder)
{
	_ShowMsg("Enter DelSMSMessages");
	DWORD dwRet = CheckSMSConnection(SN);
	if(dwRet == CONA_OK)
	{

		if(!pTargetFloder->dwFolderId || pTargetFloder->dwFolderId == CA_MESSAGE_FOLDER_USER_FOLDERS)
		{
			return 1;
		}
		// Read all the SMS item UIDs from the connected device 
		CA_ID_LIST caIDList = {0};
		caIDList.dwSize = sizeof(caIDList);
		dwRet = CAGetIDList(m_hSMS, pTargetFloder->dwFolderId, 0, &caIDList); 
		if(dwRet != CONA_OK)
		{
			_ShowMsg("GetIDList Fauild");
			return 1;
		}
		_ShowMsg("GetIDList Ok");
		CAOPERATIONHANDLE hOperHandle = NULL;
		dwRet = CABeginOperation(m_hSMS, 0, &hOperHandle);
		if(dwRet != CONA_OK)
		{
			_ShowMsg("Opera error");
			return 1;
		}
		char snum[10]={0};
		itoa(caIDList.dwUIDCount, snum, 10);
		_ShowMsg("Opera Ok");
		_ShowMsg(snum);
		DWORD dwCount = 0;
		if(caIDList.dwUIDCount>0)
		{
			for(DWORD k = caIDList.dwUIDCount - 1 ; k >= 0; k--)
			{	// Delete SMS item from the connected device
				_ShowMsg("Delete...");
				dwRet = CADeleteItem(hOperHandle, &caIDList.pUIDs[k], 0);
				dwCount ++;
				if(dwRet == CONA_OK)
				{
					
					_ShowMsg("Delete Ok.");
					
				}else
					_ShowMsg("Delete Error.");
				if(dwCount >= caIDList.dwUIDCount) break;
			}//end of "for ..."
		}
		dwRet = CACommitOperations(hOperHandle, &caIDList);
		if(dwRet == 0)
			_ShowMsg("Commit Ok");
		dwRet = CAEndOperation(hOperHandle);
		if(dwRet != CONA_OK)
		{
			return 1;
		}

	}

	return CONA_OK;
}


//��ȡ��������,
//����:
//SN:IMEI��
//pTargetFloder:�ļ���,Ĭ��Ϊ�ռ���,����Ϊ�ѷ�����
//MobNo,txt:�ֻ��ż�����,�����Ϊ��,�򷵻ش���ָ��������
DWORD CNokiasdkApp::GetSMSMessages(WCHAR*SN, CA_FOLDER_INFO *pTargetFloder, char*MobNo, char*txt)
{

	// Check CA connection to SMS folders and open it if needed
	_ShowMsg("Eneter GetSMS");
	DWORD dwRet = CheckSMSConnection(SN);
	if(dwRet == CONA_OK)
	{

		if(!pTargetFloder->dwFolderId || pTargetFloder->dwFolderId == CA_MESSAGE_FOLDER_USER_FOLDERS)
		{
			return 1;
		}
		// Read all the SMS item UIDs from the connected device 
		CA_ID_LIST caIDList = {0};
		caIDList.dwSize = sizeof(caIDList);
		dwRet = CAGetIDList(m_hSMS, pTargetFloder->dwFolderId, 0, &caIDList); 
		if(dwRet != CONA_OK)
		{
			return 1;
		}
		CAOPERATIONHANDLE hOperHandle = NULL;
		dwRet = CABeginOperation(m_hSMS, 0, &hOperHandle);
		if(dwRet != CONA_OK)
		{
			return 1;
		}
		
		char TextBuffer[512], TextNo[50];
		DWORD dwSMSLen = 1024*caIDList.dwUIDCount;
		
		if(dwSMSLen > 0)
		{
			_ShowMsg("SMS Count>0");

			int iIndex=0;
			char strDate[20];
			for(DWORD k = 0; k < caIDList.dwUIDCount; k++)
			{	// Read SMS item from the connected device
				CA_DATA_MSG dataMsg = {0};
				dataMsg.dwSize = sizeof(dataMsg);
				dwRet = CAReadItem(hOperHandle, &caIDList.pUIDs[k], 0, CA_DATA_FORMAT_STRUCT, (LPVOID*)&dataMsg);
				if(dwRet == CONA_OK)
				{
					if(dataMsg.dwDataLength > 0)
					{
						_ShowMsg("������");
						if((CA_GET_DATA_FORMAT(dataMsg.dwInfoField)) == CA_DATA_FORMAT_UNICODE)
						{
							if(dataMsg.pbData)
							{	// Insert SMS message item in tree view
								memset(strDate, 0, 20);
								//dataMsg.messageDate.
								sprintf(strDate, "%.04d%.02d%.02d%.02d%.02d%.02d",  
									dataMsg.messageDate.wYear,
									dataMsg.messageDate.bMonth,
									dataMsg.messageDate.bDay,
									dataMsg.messageDate.bHour + 8,//�����й�ʱ��
									dataMsg.messageDate.bMinute,
									dataMsg.messageDate.bSecond);
								_ShowMsg(strDate);

								Unicode2Ascii(dataMsg.pbData, dataMsg.dwDataLength, TextBuffer);
								_ShowMsg(TextBuffer);
								WChar2Ascii(dataMsg.pAddress[0].pstrAddress, dataMsg.pAddress[0].dwSize, TextNo);
								_ShowMsg(TextNo);

								//if(iIndex > 0)
								if(strlen(m_SMSContent)>0)
									strcat(m_SMSContent, "^");
								iIndex++;
								bool bAdd = true;
								if(strlen(MobNo)>0 && strlen(txt)>0)
								{
									if(!(strstr(TextNo, MobNo) && strstr(TextBuffer, txt)))
										bAdd = false;
								}
								if(bAdd)
								{//���Ѿ���ȡ�����Ķ������ݼ��뵽�ڴ��
									strcat(m_SMSContent, TextNo);
									strcat(m_SMSContent, ",");
									strcat(m_SMSContent, strDate);
									strcat(m_SMSContent, TextBuffer);
								}
							}
							else
							{
								// No data, GMS or other kind of message?
								_ShowMsg("����CA_DATA_FORMAT_UNICODE��ʽ������");
							}
						}
						else
						{
							// Message in data format
						}
					}
					// Free memory allocated by CA API
					dwRet = CAFreeItemData(m_hSMS, CA_DATA_FORMAT_STRUCT, (LPVOID)&dataMsg);
					if(dwRet != CONA_OK)
					{
						//return;
					}
				}
				else
				{
					//return;
				}
			}//end of "for ..."
		}
		dwRet = CAEndOperation(hOperHandle);
		if(dwRet != CONA_OK)
		{
			return 1;
		}
	}
	return 0;
}

DWORD CNokiasdkApp::Nokia_FileSystem_OpenW(WCHAR*IMEI)
{
	return 0;
}

DWORD CNokiasdkApp::Nokia_FileSystem_CloseW(WCHAR*IMEI)
{
	return 0;
}

//0:����,1,OpenDMʧ��,2OpenFSʧ��,3,Copyʧ��
DWORD CNokiasdkApp::Nokia_GetFileW(WCHAR*IMEI, WCHAR*FolderPath, WCHAR*FileName, WCHAR*DestPath)
{
	DWORD dwMedia = API_MEDIA_ALL;
	
	DWORD dwDeviceID = 0;       
	DWORD dwResult = CONA_OK;
	if(m_hDM == NULL)
		dwResult = CONAOpenDM(&m_hDM);
	if(dwResult == CONA_OK)
	{
		_ShowMsg("Open DM Ok");
		if(hFS == NULL)
			dwResult = CONAOpenFS(IMEI, &dwMedia, &hFS, &dwDeviceID);
		else
			dwResult = CONA_OK;
		if(dwResult == CONA_OK)
		{
			_ShowMsg("Open FS Ok");
			dwResult = CONACopyFile(hFS, 
				CONA_DIRECT_PHONE_TO_PC | CONA_OVERWRITE, 
				FileName, 
				FolderPath, 
				DestPath);
			if(dwResult == CONA_OK)
				_ShowMsg("Copy File Ok");
			else
			{
				dwResult = 2;
				_ShowMsg("Copy File Error");
			}
			//_ShowMsg("Close DM");
			//CONACloseFS(hFS);
		} else
		{
			if(dwResult ==ECONA_DEVICE_NOT_FOUND)
				dwResult = 3;
			else if(dwResult ==ECONA_NO_CONNECTION_VIA_MEDIA)
				dwResult = 4;
			else if(dwResult ==ECONA_NOT_SUPPORTED_DEVICE)
				dwResult = 5;
			else if(dwResult ==ECONA_CONNECTION_FAILED)
				dwResult = 6;
			else if(dwResult ==ECONA_CONNECTION_BUSY)
				dwResult = 7;
			else if(dwResult ==ECONA_CONNECTION_LOST)
				dwResult = 8;
			else if(dwResult ==ECONA_FAILED_TIMEOUT)
				dwResult = 9;
			else if(dwResult ==ECONA_INVALID_POINTER)
				dwResult = 10;
			else if(dwResult ==ECONA_NOT_INITIALIZED)
				dwResult = 11;
			else if(dwResult ==ECONA_NOT_SUPPORTED_MANUFACTURER)
				dwResult = 12;
			else if(dwResult ==ECONA_NOT_ENOUGH_MEMORY)
				dwResult = 13;
			else if(dwResult ==ECONA_UNKNOWN_ERROR)
				dwResult = 14;
			else
				dwResult = 15;
			_ShowMsg("Open FS Error");
		}
		//CONACloseDM(m_hDM);
	}else
	{
		dwResult = 1;
		_ShowMsg("Open DM Error");
	}
	return dwResult;
}

DWORD CNokiasdkApp::GetDeivceCount(DMHANDLE hnd, DWORD &dwCount)
{
	DWORD dwDeviceCount = 0;
	DWORD dwRet;
	int iIndex = 0;
	while(iIndex < 10)
	{
		dwRet = CONAGetDeviceCount(hnd, &dwDeviceCount);
		if(dwRet == CONA_OK)
		{
			if(dwCount > 0)
				break;
		}
		_ShowMsg("���Ի�ȡ�豸����");
		::Sleep(100);
		iIndex ++;
	}
	dwCount = dwDeviceCount;
	return dwRet;
}

DWORD CNokiasdkApp::OperaSMS(WCHAR*SN, DWORD dwType, char*mobNo, char*SMSInfo)
{
	//��ʼPC API �׼�
	DWORD dwRet = CONA_OK;
	m_SMSContent = NULL;
	//���豸��������
	if(m_hDM == NULL)
		dwRet = CONAOpenDM(&m_hDM);
	if(dwRet == CONA_OK)
	{
		_ShowMsg("Open DM Ok");
		//�ر�SMS������
		CloseConnection(m_hSMS);
		//��ȡ��ǰ�ж��ٸ�Nokia�豸������
		DWORD dwDeviceCount;
		dwRet = GetDeivceCount(m_hDM, dwDeviceCount);//CONAGetDeviceCount(m_hDM, &dwDeviceCount);
		if(dwRet == CONA_OK)
		{
			if(dwDeviceCount >0)
			{
				_ShowMsg("DeviceCount>0");
				//��ȡ�����豸���豸��Ϣ
				//WCHAR*		pstrSerialNumber;   IMEI��, 3519....456
				//WCHAR*		pstrFriendlyName;   �ֻ�����,��Nokia 6120 classic
				//WCHAR*		pstrModel;			�ֻ��̼��İ汾,��RM-243,��*#0000#�����ֻ��ϲ鿴
				//WCHAR*		pstrManufacturer;	��������,һ��ΪNokia
				//DWORD		dwNumberOfItems;
				CONAPI_DEVICE* pDevices = new CONAPI_DEVICE[dwDeviceCount];
				dwRet = CONAGetDevices(m_hDM, &dwDeviceCount, pDevices);
				CString strObjSN = SN;
				CString strDstSN,strShow;
				for(DWORD dwDeviceIndex = 0; dwDeviceIndex < dwDeviceCount; dwDeviceIndex++)
				{	// Insert phone item in tree view
					//��ȡ��ǰ��IMEI
					strDstSN = pDevices[dwDeviceIndex].pstrSerialNumber;
					strShow = "��ǰ��IMEI:" + strDstSN+", Ŀ��IMEI:" + strObjSN;
					_ShowMsg(strShow.GetBuffer(0));
					//�Ƚ�IMEI
					if(strDstSN == strObjSN)
					{
						//��ȡָ��IMEI���ռ���
						//CA_MESSAGE_FOLDER_INBOX 
						_ShowMsg("Ŀ���ֻ����ҵ���");
						if(GetSMSFolders(pDevices[dwDeviceIndex].pstrSerialNumber, CA_MESSAGE_FOLDER_INBOX) == CONA_OK)
						{
							_ShowMsg("Find Device Ok");
							if(dwType == NOKIA_OPERATION_GET)
							{
								m_SMSContent = (char*)malloc(300 * 100);//ÿ����Ϣ���200���ַ�
								if(m_SMSContent)
								{
									memset(m_SMSContent, 0, 300 * 100);
									//�õ����ռ����µ�������Ϣ
									if(GetSMSMessages(pDevices[dwDeviceIndex].pstrSerialNumber, &m_fiInBox, "", "") == CONA_OK)
									{
										_ShowMsg("Get OK");
										if(m_SMSContent)
										{
											strcpy(SMSInfo, m_SMSContent);
										}
									}
									free(m_SMSContent);
									m_SMSContent = NULL;
								}
							}else if(dwType == NOKIA_OPERATION_DEL)
							{
								//ɾ���ռ������Ϣ
								dwRet = DelSMSMessages(pDevices[dwDeviceIndex].pstrSerialNumber, &m_fiInBox);
								//ɾ�����������Ϣ
								_ShowMsg("delete out box");
								if(GetSMSFolders(pDevices[dwDeviceIndex].pstrSerialNumber, CA_MESSAGE_FOLDER_SENT) == CONA_OK)
									dwRet = dwRet | DelSMSMessages(pDevices[dwDeviceIndex].pstrSerialNumber, &m_fiInBox);
								//ɾ������
								//if(GetMMSFolders(pDevices[dwDeviceIndex].pstrSerialNumber, CA_MESSAGE_FOLDER_INBOX) == CONA_OK)
								//	dwRet = dwRet | DelMMSMessages(pDevices[dwDeviceIndex].pstrSerialNumber, &m_fiInBox);
							}else if(dwType == NOKIA_OPERATION_SEND)
							{

								//WCHAR*SN, CA_FOLDER_INFO *pTargetFloder, WCHAR* MobNo, WCHAR*txt
								_ShowMsg("write out box");
								if(GetSMSFolders(pDevices[dwDeviceIndex].pstrSerialNumber, CA_MESSAGE_FOLDER_OUTBOX) == CONA_OK)
									dwRet = WriteSMS(pDevices[dwDeviceIndex].pstrSerialNumber, &m_fiInBox, mobNo, SMSInfo);
							}else if(dwType == NOKIA_OPERATION_SENDSTATUS)
							{
								
							}
							break;
						}else
							_ShowMsg("��ȡ�ռ���Ŀ¼ʧ��");
					}
				}
				// Free memory allocated by CONAGetDevices
				CONAFreeDeviceStructure(dwDeviceCount, pDevices);
			}else
			{
				dwRet = 1;
				_ShowMsg("Device Count = 0");
			}
		}else
		{
			dwRet = 1;
			_ShowMsg("CONAGetDeviceCount Error");
		}
	}else
	{
		_ShowMsg("Open DM Error");
		dwRet = 1;
	}
	//CONACloseDM(m_hDM);
	return dwRet;
}



/////////////////////////////////////////////////////////////////////////////
// The one and only CNokiasdkApp object

CNokiasdkApp theApp;

DWORD __stdcall Nokia_Open()
{
	Mutex_Init();
	Mutex_Wait();

	strcpy(theApp.str_imei, "");
	Sys_lua_addlog("��DM");
	Mutex_Free();
	return theApp.Nokia_Init();
}

void __stdcall Nokia_Close()
{
	strcpy(theApp.str_imei, "");
	Sys_lua_addlog("�ر�DM");
	//theApp.Nokia_Uninit();
}


DWORD __stdcall Nokia_GetSMS(char*IMEI, char*SMSInfo)
{
	Sys_lua_addlog("start dll get sms");
	Mutex_Init();
	Mutex_Wait();
	strcpy(theApp.str_imei, IMEI);
	DWORD dwRet=0;
	try
	{
		//Sys_lua_addlog(IMEI);
		WCHAR   lpSN[256];
		Ascii2Wchar(IMEI, lpSN);

		char mobNo[100];
		strcpy(SMSInfo, "");
		CString strSN = lpSN;
		Sys_lua_addlog(strSN.GetBuffer(0));
		dwRet = theApp.OperaSMS(lpSN, NOKIA_OPERATION_GET, mobNo, SMSInfo);
		char sRet[20]={0};
		itoa(dwRet, sRet, 10);
		strcpy(sLogBuffer, "Result=");
		strcat(sLogBuffer, sRet);
		Sys_lua_addlog(sLogBuffer);
		Sys_lua_addlog(SMSInfo);
	}catch(...)
	{
		dwRet = 1;
		Sys_lua_addlog("dll get sms exception");
	}
	Sys_lua_addlog("end dll get sms");
	Mutex_Free();
	return dwRet;
}

DWORD __stdcall Nokia_DelSMS(char*IMEI)
{
	Sys_lua_addlog("start dll delete sms");
	Mutex_Init();
	Mutex_Wait();
	strcpy(theApp.str_imei, IMEI);
	DWORD dwRet=0;
	try
	{
		WCHAR   lpSN[256];
		Ascii2Wchar(IMEI, lpSN);
		char mobNo[100], SMSInfo[100];
		dwRet= theApp.OperaSMS(lpSN, NOKIA_OPERATION_DEL, mobNo, SMSInfo);
		char sRet[20]={0};
		itoa(dwRet, sRet, 10);
		strcpy(sLogBuffer, "Result=");
		strcat(sLogBuffer, sRet);
		Sys_lua_addlog(sLogBuffer);
	}catch(...)
	{
		Sys_lua_addlog("error");
		dwRet = 0;
	}
	Sys_lua_addlog("end dll delete sms");
	Mutex_Free();
	return dwRet;
}

//�������Ƿ��ͳɹ�
//0:�ɹ�
DWORD __stdcall Nokia_SMSSendStatus(char*IMEI, char*mobNo, char*text)
{
	Mutex_Init();
	Mutex_Wait();
	DWORD dwRet = 1;
	strcpy(theApp.str_imei, IMEI);
	try
	{
		WCHAR   lpSN[256];
		Ascii2Wchar(IMEI, lpSN);
		//WCHAR	lpMobNo[256], lpText[500];
		//Ascii2Wchar(mobNo, lpMobNo);
		//Ascii2Wchar(text, lpText);
		dwRet = theApp.OperaSMS(lpSN, NOKIA_OPERATION_SENDSTATUS, mobNo, text);
	}catch(...)
	{
		Sys_lua_addlog("send msg error.");
	}
	Mutex_Free();
	return dwRet;
}

DWORD __stdcall Nokia_SendSMS(char*IMEI, char*mobNo, char*text)
{
	Mutex_Init();
	Mutex_Wait();
	DWORD dwRet = 1;
	strcpy(theApp.str_imei, IMEI);
	
	try
	{
		WCHAR   lpSN[256];
		Ascii2Wchar(IMEI, lpSN); 
		dwRet = theApp.OperaSMS(lpSN, NOKIA_OPERATION_SEND, mobNo, text);
	}catch(...)
	{
		Sys_lua_addlog("send msg error.");
	}
	Mutex_Free();
	return dwRet;
}

/*
����:���ֻ������ļ���PC��
����:
	IMEI:����IMEI��ʶ��ͬ���ֻ�
	FolderPath:�ֻ���Ŀ¼��
	FileName:�ֻ��е��ļ�
	DestPath:PC�˵�Ŀ¼��
*/
DWORD __stdcall Nokia_GetFile(char*IMEI, char*FolderPath, char*FileName, char*DestPath)
{
	Mutex_Init();
	Mutex_Wait();
	strcpy(theApp.str_imei, IMEI);
	WCHAR   lpSN[256], lpSrcFolder[256], lpSrcFile[256], lpDestPath[256];
	Ascii2Wchar(IMEI, lpSN);
	Ascii2Wchar(FolderPath, lpSrcFolder);
	Ascii2Wchar(FileName, lpSrcFile);
	Ascii2Wchar(DestPath, lpDestPath);
	DWORD dwRet = theApp.Nokia_GetFileW(lpSN, lpSrcFolder, lpSrcFile, lpDestPath);
	Mutex_Free();
	return dwRet;
}
///-------------------------------------------------------------------------
//			LUA ��װ		
///-------------------------------------------------------------------------
///-------------------------------------------------------------------------
extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

//����Ҫ����
extern "C" int lua_Nokia_Open(lua_State* L)
{
	double dRet = Nokia_Open();
	lua_pushnumber(L, dRet);
	return 1;
}
//����Ҫ����
extern "C" int lua_Nokia_Close(lua_State* L)
{
	Nokia_Close();
	return 0;
}
//��Ҫ2������
extern "C" int lua_Nokia_GetSMS(lua_State* L)
{
	Sys_lua_addlog("start lua get sms");
	try
	{
		if (!(lua_tostring(L,1)))
		{
			Sys_lua_addlog("Invalid argument");
			Sys_lua_addlog("end lua get sms");
			lua_pushnumber(L,-1001);
			return 1;//ȱ�ٲ���
		}
		char SMSInfo[8149]={0};
		char sImei[50]={0};
		std::string IMEI=(LPTSTR)lua_tostring(L,1);
		strcpy(sImei, IMEI.c_str());
		memset(sLogBuffer, 0, sizeof(sLogBuffer));
		strcpy(sLogBuffer, sImei);
		Sys_lua_addlog(sLogBuffer);

		DWORD dwRet = Nokia_GetSMS(sImei, SMSInfo);
		lua_pushnumber(L, dwRet);
		if(dwRet == 0)
		{
			if(strlen(SMSInfo)==0)
				strcpy(SMSInfo, "_");
			lua_pushstring(L, SMSInfo);
		}
		else
			lua_pushstring(L, "_");

		itoa(dwRet, sImei, 10);
		strcpy(sLogBuffer, "Result=");
		strcat(sLogBuffer, sImei);
		Sys_lua_addlog(sLogBuffer);
		Sys_lua_addlog(SMSInfo);
		Sys_lua_addlog("end lua get sms");
	}catch(std::exception& e)
	{
		Sys_lua_addlog(e.what());	
	}catch(...)
	{
		Sys_lua_addlog("other error.");
	}
	return 2;
}
//��Ҫһ������
extern "C" int lua_Nokia_DelSMS(lua_State* L)
{
	Sys_lua_addlog("start lua delete sms");
	if (!(lua_tostring(L,1)))
	{
		Sys_lua_addlog("Invalid argument");
		Sys_lua_addlog("end lua delete sms");
		lua_pushnumber(L,-1001);
		return 1;//ȱ�ٲ���
	}
	std::string IMEI=(LPTSTR)lua_tostring(L,1);
	char sImei[50]={0};
	strcpy(sImei, IMEI.c_str());
	Sys_lua_addlog(sImei);
	DWORD dwRet = Nokia_DelSMS(sImei);
	itoa(dwRet, sImei, 10);
	strcpy(sLogBuffer, "Result=");
	strcat(sLogBuffer, sImei);
	Sys_lua_addlog(sLogBuffer);
	lua_pushnumber(L, dwRet);
	Sys_lua_addlog("end lua delete sms");
	return 1;
}
//��Ҫ��������
extern "C" int lua_Nokia_SendSMS(lua_State* L)
{
	Sys_lua_addlog("start lua send sms");
	if (!(lua_tostring(L,1)&&lua_tostring(L,2)&&lua_tostring(L,3)))
	{
		Sys_lua_addlog("Invalid argument");
		Sys_lua_addlog("end lua send sms");
		lua_pushnumber(L,-1001);
		return 1;//ȱ�ٲ���
	}
	std::string IMEI=(LPTSTR)lua_tostring(L,1);
	std::string mobNo=(LPTSTR)lua_tostring(L,2);
	std::string text=(LPTSTR)lua_tostring(L,3);

	char sImei[50], sMobNo[50], sText[200];
	strcpy(sImei, IMEI.c_str());
	strcpy(sMobNo, mobNo.c_str());
	strcpy(sText, text.c_str());
	memset(sLogBuffer, 0, sizeof(sLogBuffer));
	strcpy(sLogBuffer, sImei);
	strcat(sLogBuffer, "->");
	strcat(sLogBuffer, sMobNo);
	strcat(sLogBuffer, "->");
	strcat(sLogBuffer, sText);
	Sys_lua_addlog(sLogBuffer);
	
	DWORD dwRet = Nokia_SendSMS(sImei, sMobNo, sText);
	itoa(dwRet, sText, 10);
	strcpy(sLogBuffer, "Result=");
	strcat(sLogBuffer, sText);
	Sys_lua_addlog(sLogBuffer);
	Sys_lua_addlog("end lua send sms");
	lua_pushnumber(L, dwRet);
	return 1;
}

 struct luaL_reg lrLibs[] =   
 {  
    { "lua_Nokia_Open", lua_Nokia_Open },  
	{ "lua_Nokia_Close", lua_Nokia_Close }, 
	{ "lua_Nokia_GetSMS", lua_Nokia_GetSMS }, 
	{ "lua_Nokia_DelSMS", lua_Nokia_DelSMS }, 
	{ "lua_Nokia_SendSMS", lua_Nokia_SendSMS }, 
    { NULL, NULL }  /* sentinel */  
 }; 

 extern "C" __declspec(dllexport) int  luaopen_luaNokiaSMS(lua_State* L)
{
    
	luaL_register( L, "luaNokiaSMS", lrLibs );  
    return 0;
}


 BOOL WINAPI DllMain ( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved )
{
    // �ֱ������֪ͨ
    switch( fdwReason ) 
    { 
    case DLL_PROCESS_ATTACH:
			theApp.Nokia_Init();
            // Ϊ�½��̳�ʼ����ֻ�ܽ���1��
            // �������ʧ���򷵻�FALSE
            //MessageBox(0, TEXT("DLL_PROCESS_ATTACH"), TEXT(""), MB_OK);
            break;
        case DLL_THREAD_ATTACH:
			//::MessageBox(0, TEXT("DLL_THREAD_ATTACH"), TEXT(""), MB_OK);
            // ִ���̳߳�ʼ������
            break;
        case DLL_THREAD_DETACH:
			//::MessageBox(0, TEXT("DLL_THREAD_DETACH"), TEXT(""), MB_OK);
            // ִ���߳��������
            break;
        case DLL_PROCESS_DETACH:
            // ִ���������
			//theApp.Nokia_Uninit();
			//::MessageBox(0, TEXT("DLL_PROCESS_DETACH"), TEXT(""), MB_OK);
           break;
    }
    return TRUE;
}
 