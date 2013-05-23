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
修改日志:
  2011-8-1 修改Lua包装的参数返回BUG 
  2011-8-24 增加日志输出的功能,固定输出在c:\nokiasms.log,没有配置,文件会不断的增长
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
		Sys_lua_addlog("Mutex 未存在");
        bool bExists = false;
        CreateMutex(NULL, FALSE, strMutex);
        DWORD dwRet = GetLastError();
        if(dwRet == 0)
        {
			Sys_lua_addlog("Mutex 创建成功");
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
		Sys_lua_addlog("打开Mutex成功");
	else
		Sys_lua_addlog("打开Mutex失败");
}

void Mutex_Wait()
{
	Sys_lua_addlog("等待中。。。");
	WaitForSingleObject(hMutex,INFINITE);
	Sys_lua_addlog("成功等到");
}

void Mutex_Free()
{
	ReleaseMutex(hMutex);
	Sys_lua_addlog("给下一个");
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
	_ShowMsg("DLL 初始化");
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


//获取普通短信收件箱的目录
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
				_ShowMsg("目录个数>0");
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
		_ShowMsg("检查SMS连接失败");
	}
	return dwRet;
}

char* WChar2Ascii(WCHAR*Data, DWORD dwLen, char*buffer)
{
		//WChar转换成Char
    char buf[200];
    int   nLen   =   wcslen(Data)+1;
    WideCharToMultiByte(CP_ACP,   0,   Data,   nLen,   buf,   2*nLen,   NULL,   NULL);
	strcpy(buffer, buf);
	return buffer;

}

char* CNokiasdkApp::Unicode2Ascii(BYTE*Data, DWORD dwLen, char*buffer)
{
	//将内存的Byte数组进行WCHAR组合
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
	//WChar转换成Char
    char buf[200];
    int   nLen   =   wcslen(wStr)+1;
    WideCharToMultiByte(CP_ACP,   0,   wStr,   nLen,   buf,   2*nLen,   NULL,   NULL);
	strcpy(buffer, buf);
	return buffer;
}

BYTE* CNokiasdkApp::Ascii2Unicode(BYTE*Data, DWORD dwLen, char*buffer)
{
	//将内存的Byte数组进行WCHAR组合

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

		//定义目标号码,这里只支持一个号码
		CA_DATA_MSG dataMsg  ={0};
		dataMsg.dwSize = sizeof(CA_DATA_MSG);
		CA_SET_DATA_FORMAT(dataMsg.dwInfoField, CA_DATA_FORMAT_UNICODE);
		CA_SET_DATA_CODING(dataMsg.dwInfoField, CA_DATA_CODING_UNICODE);
		CA_SET_MESSAGE_STATUS(dataMsg.dwInfoField, CA_MESSAGE_STATUS_DRAFT);
		CA_SET_MESSAGE_TYPE(dataMsg.dwInfoField, CA_SMS_SUBMIT);

		//手机号码与内容的操作
		char*strNumber = MobNo;//"13858061334";
		int iLength = strlen(strNumber);
		WCHAR *pNum=NULL;
		BYTE*bData;
		//分配手机号码的内容
		pNum = (LPWSTR)CoTaskMemAlloc((iLength+1)*sizeof(WCHAR));
		memset(pNum, 0, (iLength+1)*sizeof(WCHAR));
		//初始化手机号码
		CA_DATA_ADDRESS* pDataAddress = (CA_DATA_ADDRESS*)CoTaskMemAlloc(sizeof(CA_DATA_ADDRESS));
		pDataAddress->dwSize = sizeof(CA_DATA_ADDRESS);
		pDataAddress->dwAddressInfo = CA_MSG_ADDRESS_TYPE_NUMBER;
		//转换手机号码
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

		//这里将发送字符进行转换
		char*strMsg = txt;
		//2011-9-21 23:32 by hupo
		//转换成wchar_t,然后再转换为unicode
		wchar_t lpszMsg[1024]={0};
		int nLen = strlen(strMsg) + 1;
		int nwLen = MultiByteToWideChar(CP_ACP,   0,   strMsg,   nLen,   NULL,   0);

	    MultiByteToWideChar(CP_ACP,   0,   strMsg,   nLen,   lpszMsg,   nwLen);
		int iwLen = wcslen(lpszMsg);//转换为wchar_t完毕,并且计算最终的字符串长度,中文算1个长度.
		//转换为unicode
typedef union {
    unsigned char ichar[2];
    unsigned short iint;
} TINT2;
		//分配发送内容的内存
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
		//释放内存
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


//删除普通短信的内容
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


//获取短信内容,
//参数:
//SN:IMEI号
//pTargetFloder:文件夹,默认为收件箱,可以为已发信箱
//MobNo,txt:手机号及内容,如果不为空,则返回存在指定的内容
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
						_ShowMsg("有内容");
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
									dataMsg.messageDate.bHour + 8,//加入中国时差
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
								{//将已经读取出来的短信内容加入到内存表
									strcat(m_SMSContent, TextNo);
									strcat(m_SMSContent, ",");
									strcat(m_SMSContent, strDate);
									strcat(m_SMSContent, TextBuffer);
								}
							}
							else
							{
								// No data, GMS or other kind of message?
								_ShowMsg("不是CA_DATA_FORMAT_UNICODE格式的内容");
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

//0:正常,1,OpenDM失败,2OpenFS失败,3,Copy失败
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
		_ShowMsg("重试获取设备数量");
		::Sleep(100);
		iIndex ++;
	}
	dwCount = dwDeviceCount;
	return dwRet;
}

DWORD CNokiasdkApp::OperaSMS(WCHAR*SN, DWORD dwType, char*mobNo, char*SMSInfo)
{
	//初始PC API 套件
	DWORD dwRet = CONA_OK;
	m_SMSContent = NULL;
	//打开设备管理中心
	if(m_hDM == NULL)
		dwRet = CONAOpenDM(&m_hDM);
	if(dwRet == CONA_OK)
	{
		_ShowMsg("Open DM Ok");
		//关闭SMS的连接
		CloseConnection(m_hSMS);
		//获取当前有多少个Nokia设备的连接
		DWORD dwDeviceCount;
		dwRet = GetDeivceCount(m_hDM, dwDeviceCount);//CONAGetDeviceCount(m_hDM, &dwDeviceCount);
		if(dwRet == CONA_OK)
		{
			if(dwDeviceCount >0)
			{
				_ShowMsg("DeviceCount>0");
				//获取所有设备的设备信息
				//WCHAR*		pstrSerialNumber;   IMEI号, 3519....456
				//WCHAR*		pstrFriendlyName;   手机名称,如Nokia 6120 classic
				//WCHAR*		pstrModel;			手机固件的版本,如RM-243,按*#0000#可在手机上查看
				//WCHAR*		pstrManufacturer;	厂家名称,一般为Nokia
				//DWORD		dwNumberOfItems;
				CONAPI_DEVICE* pDevices = new CONAPI_DEVICE[dwDeviceCount];
				dwRet = CONAGetDevices(m_hDM, &dwDeviceCount, pDevices);
				CString strObjSN = SN;
				CString strDstSN,strShow;
				for(DWORD dwDeviceIndex = 0; dwDeviceIndex < dwDeviceCount; dwDeviceIndex++)
				{	// Insert phone item in tree view
					//获取当前的IMEI
					strDstSN = pDevices[dwDeviceIndex].pstrSerialNumber;
					strShow = "当前的IMEI:" + strDstSN+", 目标IMEI:" + strObjSN;
					_ShowMsg(strShow.GetBuffer(0));
					//比较IMEI
					if(strDstSN == strObjSN)
					{
						//获取指定IMEI的收件箱
						//CA_MESSAGE_FOLDER_INBOX 
						_ShowMsg("目标手机已找到。");
						if(GetSMSFolders(pDevices[dwDeviceIndex].pstrSerialNumber, CA_MESSAGE_FOLDER_INBOX) == CONA_OK)
						{
							_ShowMsg("Find Device Ok");
							if(dwType == NOKIA_OPERATION_GET)
							{
								m_SMSContent = (char*)malloc(300 * 100);//每条信息最多200个字符
								if(m_SMSContent)
								{
									memset(m_SMSContent, 0, 300 * 100);
									//得到该收件箱下的所有信息
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
								//删除收件箱的信息
								dwRet = DelSMSMessages(pDevices[dwDeviceIndex].pstrSerialNumber, &m_fiInBox);
								//删除发件箱的信息
								_ShowMsg("delete out box");
								if(GetSMSFolders(pDevices[dwDeviceIndex].pstrSerialNumber, CA_MESSAGE_FOLDER_SENT) == CONA_OK)
									dwRet = dwRet | DelSMSMessages(pDevices[dwDeviceIndex].pstrSerialNumber, &m_fiInBox);
								//删除彩信
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
							_ShowMsg("获取收件箱目录失败");
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
	Sys_lua_addlog("打开DM");
	Mutex_Free();
	return theApp.Nokia_Init();
}

void __stdcall Nokia_Close()
{
	strcpy(theApp.str_imei, "");
	Sys_lua_addlog("关闭DM");
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

//检测短信是否发送成功
//0:成功
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
功能:从手机复制文件到PC端
参数:
	IMEI:根据IMEI来识别不同的手机
	FolderPath:手机的目录名
	FileName:手机中的文件
	DestPath:PC端的目录名
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
//			LUA 包装		
///-------------------------------------------------------------------------
///-------------------------------------------------------------------------
extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

//不需要参数
extern "C" int lua_Nokia_Open(lua_State* L)
{
	double dRet = Nokia_Open();
	lua_pushnumber(L, dRet);
	return 1;
}
//不需要参数
extern "C" int lua_Nokia_Close(lua_State* L)
{
	Nokia_Close();
	return 0;
}
//需要2个参数
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
			return 1;//缺少参数
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
//需要一个参数
extern "C" int lua_Nokia_DelSMS(lua_State* L)
{
	Sys_lua_addlog("start lua delete sms");
	if (!(lua_tostring(L,1)))
	{
		Sys_lua_addlog("Invalid argument");
		Sys_lua_addlog("end lua delete sms");
		lua_pushnumber(L,-1001);
		return 1;//缺少参数
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
//需要三个参数
extern "C" int lua_Nokia_SendSMS(lua_State* L)
{
	Sys_lua_addlog("start lua send sms");
	if (!(lua_tostring(L,1)&&lua_tostring(L,2)&&lua_tostring(L,3)))
	{
		Sys_lua_addlog("Invalid argument");
		Sys_lua_addlog("end lua send sms");
		lua_pushnumber(L,-1001);
		return 1;//缺少参数
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
    // 分别处理各种通知
    switch( fdwReason ) 
    { 
    case DLL_PROCESS_ATTACH:
			theApp.Nokia_Init();
            // 为新进程初始化，只能进行1次
            // 如果加载失败则返回FALSE
            //MessageBox(0, TEXT("DLL_PROCESS_ATTACH"), TEXT(""), MB_OK);
            break;
        case DLL_THREAD_ATTACH:
			//::MessageBox(0, TEXT("DLL_THREAD_ATTACH"), TEXT(""), MB_OK);
            // 执行线程初始化操作
            break;
        case DLL_THREAD_DETACH:
			//::MessageBox(0, TEXT("DLL_THREAD_DETACH"), TEXT(""), MB_OK);
            // 执行线程清理操作
            break;
        case DLL_PROCESS_DETACH:
            // 执行清理操作
			//theApp.Nokia_Uninit();
			//::MessageBox(0, TEXT("DLL_PROCESS_DETACH"), TEXT(""), MB_OK);
           break;
    }
    return TRUE;
}
 