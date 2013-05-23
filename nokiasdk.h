// nokiasdk.h : main header file for the NOKIASDK DLL
//

#if !defined(AFX_NOKIASDK_H__42BEADEB_8221_4C9E_A358_9CFF072E5C54__INCLUDED_)
#define AFX_NOKIASDK_H__42BEADEB_8221_4C9E_A358_9CFF072E5C54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include <PCCSErrors.h>
#include <CONADeviceManagement.h>
#include <CADataDefinitions.h>
#include <CommonFunctionsAPI.h>
#include <CONAApplicationInstallation.h>
#include <CONAFileConversion.h>
#include <CONAFileSystem.h>
#include <DAContentAccess.h>
#include <DMAPIDefinitions.h>
#include <FSAPIDefinitions.h>
#include <PCCSTypeDefinitions.h>
/////////////////////////////////////////////////////////////////////////////
// CNokiasdkApp
// See nokiasdk.cpp for the implementation of this class
//
#define NOKIA_OPERATION_GET 0
#define NOKIA_OPERATION_DEL 1
#define NOKIA_OPERATION_SEND 2
#define NOKIA_OPERATION_SENDSTATUS 3

class COperationNotify : public ICAOperationNotify
{
	// ICAOperationNotify
	virtual DWORD OnCAOperationNotify(DWORD dwOperation, DWORD dwCurrent, DWORD dwTotalAmount, DWORD dwStatus, CA_ITEM_ID * pItemID);
};

class CNokiasdkApp : public CWinApp
{

private:
	CAHANDLE	m_hSMS;//, m_hMMS;
	DMHANDLE	m_hDM;
	FSHANDLE	m_hFS;//文件句柄
	CA_FOLDER_INFO	m_fiInBox;//收件箱
	char* m_SMSContent;
	WCHAR m_strOldSrc[50];
	DWORD CloseConnection(CAHANDLE& hCA);
	DWORD CheckSMSConnection(WCHAR*SN);
	DWORD GetSMSFolders(WCHAR*SN, DWORD dwFolderId);
	DWORD GetSMSMessages(WCHAR*SN, CA_FOLDER_INFO *pTargetFloder, char*MobNo, char*txt);
	DWORD DelSMSMessages(WCHAR*SN, CA_FOLDER_INFO *pTargetFloder);
	DWORD WriteSMS(WCHAR*SN, CA_FOLDER_INFO *pTargetFloder, char* MobNo, char*txt);

	//DWORD GetMMSFolders(WCHAR*SN, DWORD dwFolderId);
	//DWORD DelMMSMessages(WCHAR*SN, CA_FOLDER_INFO *pTargetFloder);
	//DWORD GetMMSMessages(WCHAR*SN, CA_FOLDER_INFO *pTargetFloder, char*MobNo, char*txt);

	void GetFloderInfo(CA_FOLDER_INFO *Info);
	char* Unicode2Ascii(BYTE*Data, DWORD dwLen, char*buffer);
	//CA_DATA_ADDRESS * GetSMSAddressBuffer(char*strNumber); 
	BYTE* Ascii2Unicode(BYTE*Data, DWORD dwLen, char*buffer);
	void GetCurrentPIMDate(CA_DATA_DATE *pimDate);
	DWORD WriteItem(DWORD dwFolderId, LPVOID pItemData);
	FSHANDLE hFS;
	DWORD Sys_addlog(const char* log);
	void _ShowMsg(char*msg);
	DWORD GetDeivceCount(DMHANDLE hnd, DWORD &dwCount);
public:
	CNokiasdkApp();
	
	char str_imei[50];
	//0:ok,
	int Nokia_Init();
	void Nokia_Uninit();
	//dwType:0获取信息,1:删除信息,2:发送信息
	DWORD OperaSMS(WCHAR*SN, DWORD dwType, char*mobNo, char*SMSInfo);
	DWORD Nokia_GetFileW(WCHAR*IMEI, WCHAR*FolderPath, WCHAR*FileName, WCHAR*DestPath);
	DWORD Nokia_FileSystem_OpenW(WCHAR*IMEI);
	DWORD Nokia_FileSystem_CloseW(WCHAR*IMEI);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNokiasdkApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CNokiasdkApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

DWORD __stdcall Nokia_Open();
void __stdcall Nokia_Close();
DWORD __stdcall Nokia_GetSMS(char*IMEI, char*SMSInfo);
DWORD __stdcall Nokoa_DelSMS(char*IMEI);
DWORD __stdcall Nokia_SMSSendStatus(char*IMEI, char*mobNo, char*text);
DWORD __stdcall Nokia_SendSMS(char*IMEI, char*mobNo, char*text);
DWORD __stdcall Nokia_GetFile(char*IMEI, char*FolderPath, char*FileName, char*DestPath);
DWORD __stdcall Sys_lua_addlog(const char* log);
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOKIASDK_H__42BEADEB_8221_4C9E_A358_9CFF072E5C54__INCLUDED_)
