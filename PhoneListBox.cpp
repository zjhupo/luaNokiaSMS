/*
Filename    : PhoneListBox.cpp
Part of     : File Browser
Description : Implementation of File Browser's phone list box
Version     : 3.2

This example is only to be used with PC Connectivity API version 3.2.
Compability ("as is") with future versions is not quaranteed.

Copyright (c) 2007 Nokia Corporation.
 
This material, including but not limited to documentation and any related 
computer programs, is protected by intellectual property rights of Nokia 
Corporation and/or its licensors.
All rights are reserved. Reproducing, modifying, translating, or 
distributing any or all of this material requires the prior written consent 
of Nokia Corporation. Nokia Corporation retains the right to make changes 
to this material at any time without notice. A copyright license is hereby 
granted to download and print a copy of this material for personal use only.
No other license to any other intellectual property rights is granted. The 
material is provided "as is" without warranty of any kind, either express or 
implied, including without limitation, any warranty of non-infringement, 
merchantability and fitness for a particular purpose. In no event shall 
Nokia Corporation be liable for any direct, indirect, special, incidental, 
or consequential loss or damages, including but not limited to, lost profits 
or revenue,loss of use, cost of substitute program, or loss of data or 
equipment arising out of the use or inability to use the material, even if 
Nokia Corporation has been advised of the likelihood of such damages occurring.
*/ 

#include "stdafx.h"
#include "FileBrowser.h"
#include "FileBrowserDlg.h"
#include "PhoneListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const CString g_strPhoneRoot = L"\\\\"; // '\\' means phone root

//===================================================================
// CPhoneListBox

//===================================================================
// Constructor
//
//===================================================================
CPhoneListBox::CPhoneListBox():CListBox()
{
    m_wState = PHONELIST_STATE_PHONELIST;
	m_hFS = NULL;
    m_strCurrentSN = L"";
    m_strCurrentFolder = L"";
    m_pLabel = NULL;
	TRACE(L"CPhoneListBox::CPhoneListBox(): Calling CONAOpenDM...");
    DWORD dwResult = CONAOpenDM(&m_hDM);
    if(dwResult != CONA_OK)
    {
        TRACE(L"CPhoneListBox::CPhoneListBox(): CONAOpenDM failed\n");
        ErrorMessageDlg(L"CPhoneListBox::CPhoneListBox(): CONAOpenDM failed", dwResult);
    }
    else
    {
        TRACE(L"CPhoneListBox::CPhoneListBox(): CONAOpenDM succeeded\n");
    }
	dwResult = CONARegisterDMNotifyIF(m_hDM, API_REGISTER, this);
    if(dwResult != CONA_OK)
    {
        TRACE(L"CPhoneListBox::CPhoneListBox(): CONARegisterDMNotifyIF() failed\n");
        ErrorMessageDlg(L"CPhoneListBox::CPhoneListBox(): CONARegisterDMNotifyIF() failed", dwResult);
    }
    else
    {
        TRACE(L"CPhoneListBox::CPhoneListBox(): CONARegisterDMNotifyIF() succeeded\n");
    }
}

//===================================================================
// Destructor
//
//===================================================================
CPhoneListBox::~CPhoneListBox()
{  
    TRACE(L"CPhoneListBox::~CPhoneListBox()\n");
	DWORD dwResult = CONARegisterDMNotifyIF(m_hDM, API_UNREGISTER, this);
    if(dwResult != CONA_OK)
    {
        TRACE(L"CPhoneListBox::~CPhoneListBox(): CONARegisterDMNotifyIF() failed\n");
        ErrorMessageDlg(L"CPhoneListBox::~CPhoneListBox(): CONARegisterDMNotifyIF() failed", dwResult);
    }
    else
    {
        TRACE(L"CPhoneListBox::~CPhoneListBox(): CONARegisterDMNotifyIF() succeeded\n");
    }
}

BEGIN_MESSAGE_MAP(CPhoneListBox, CListBox)
	//{{AFX_MSG_MAP(CPhoneListBox)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//===================================================================
// Init
// 
// Sets member variable m_pLabel
// 
//===================================================================
void CPhoneListBox::Init(CStatic* pLabel)
{
    // Set static control to write status information
    m_pLabel = pLabel;
}


//===================================================================
// PostNcDestroy
// 
// Overridden for cleanup
// 
//===================================================================
void CPhoneListBox::PostNcDestroy()
{
	// Close phone file system handle
	CloseFS(m_hFS);
	// Close device management handle
    DWORD dwResult = CONACloseDM(m_hDM);
    if(dwResult != CONA_OK)
    {
        TRACE(L"CPhoneListBox::~CPhoneListBox(): CONACloseDM failed\n");
        ErrorMessageDlg(L"CPhoneListBox::~CPhoneListBox(): CONACloseDM failed", dwResult);
    }
    else
    {
        TRACE(L"CPhoneListBox::~CPhoneListBox(): CONACloseDM succeeded\n");
    }

	__super::PostNcDestroy();
}

//===================================================================
// PhoneItemDblClicked
// 
// Called, when user has doubleclicked phone item
// If item is [..], shows parent folder
// If item is [foldername], shows subfolder
// 
//===================================================================
void CPhoneListBox::PhoneItemDblClicked()
{
    CString strSelectedTxt, strCurrentFolder;
    int index = GetCurSel();
    if(index != LB_ERR)
    {
        GetText(index, strSelectedTxt);
        if(strSelectedTxt == L"[..]")
        { // go up in folder tree
            if((strCurrentFolder = GetSelectedFolder()) == g_strPhoneRoot)
            { // root folder --> show phone list
                ResetContent();
                m_wState = PHONELIST_STATE_PHONELIST;            
                ListAllPhones();
            }
            else
            {
                // getting parent folder of strCurrentFolder:
                if(strCurrentFolder != g_strPhoneRoot)
                {
                    strCurrentFolder = strCurrentFolder.Left(strCurrentFolder.ReverseFind('\\'));
                    if(strCurrentFolder == L"\\") 
                    { // move to root folder
                        strCurrentFolder = g_strPhoneRoot;
                    }
                }
                ShowPhoneFolder(strCurrentFolder);
            }
        }
        else if(strSelectedTxt[0] == '[')
        { // selected item is folder
            ShowPhoneFolder(GetSelectedFolder());
        }
        else
        { // selected item is file
            TRACE(L"CPhoneListBox::PhoneItemDblClicked(): Double clicked file %s --> ignoring...\n", strSelectedTxt);
        }
    }
    else
    {
        TRACE(L"CPhoneListBox::PhoneItemDblClicked(): No list box item selected.\n");
    }
}

//===================================================================
// ShowFolders
//
// Adds all found folders to listbox by using CONAFindNextFolder.
// 
//===================================================================
void CPhoneListBox::ShowFolders(FINDHANDLE hFindHandle)
{
    CONAPI_FOLDER_INFO FolderInfo = {0};
    DWORD dwResult = 0;
    while((dwResult = CONAFindNextFolder(hFindHandle, &FolderInfo)) == CONA_OK)
    {  
        CTime time = CTime(FolderInfo.tFolderTime);
        TRACE(L"Folder: %s\t%s\tModified: %s\tLabel: %s\tMemory type: %s\n", 
            FolderInfo.pstrName,
            Permissions2String(FolderInfo.dwAttributes),
            time.Format(L"%d.%m.%Y %H:%M:%S"), 
            FolderInfo.pstrLabel,
            FolderInfo.pstrMemoryType);
        int index = AddString(L"[" + CString(FolderInfo.pstrName) + L"]");
        
        // Setting folder name as itemdata
        CString* pItemData = new CString(FolderInfo.pstrName);        
        SetItemDataPtr(index, pItemData);
        dwResult = CONAFreeFolderInfoStructure(&FolderInfo);
        if(dwResult != CONA_OK)
        {            
            ErrorMessageDlg(L"CPhoneListBox::ShowFolders(): CONAFreeFolderinfoStructure failed", dwResult);
        }
    }    
    if(dwResult != ECONA_ALL_LISTED)
    {
        ErrorMessageDlg(L"CPhoneListBox::ShowFolders(): CONAFindNextFolder failed!", dwResult);
    }
}

//===================================================================
// ShowFolders
//
// Adds all found files to listbox by using CONAFindNextFile.
// 
//===================================================================
void CPhoneListBox::ShowFiles(FINDHANDLE hFindHandle)
{
    CONAPI_FILE_INFO FileInfo = {0};
    DWORD dwResult = 0;
    while((dwResult = CONAFindNextFile(hFindHandle, &FileInfo)) == CONA_OK)
    {  
        CTime time = CTime(FileInfo.tFileTime);
        CString strAttributes = L"";
        TRACE(L"File: %s\t%s\tSize: %d\tModified: %s\tMIMEType: %s\n", 
            FileInfo.pstrName,
            Permissions2String(FileInfo.dwAttributes),
            FileInfo.dwFileSize, 
            time.Format(L"%d.%m.%Y %H:%M:%S"), 
            CString(FileInfo.pstrMIMEType));
        int index = AddString(FileInfo.pstrName);
        // Setting file name as itemdata
        CString* pItemData = new CString(FileInfo.pstrName);
        SetItemDataPtr(index, pItemData);
        dwResult = CONAFreeFileInfoStructure(&FileInfo);
        if(dwResult != CONA_OK)
        {
            ErrorMessageDlg(L"CPhoneListBox::ShowFiles(): CONAFreeFileInfoStructure failed!", dwResult);
        }
    }    
    if(dwResult != ECONA_ALL_LISTED)
    {
        ErrorMessageDlg(L"CPhoneListBox::ShowFiles(): CONAFindNextFile failed!", dwResult);
    }
}

//===================================================================
// ShowPhoneFolder
//
// Adds all files and folders to listbox by using
// functions CONAFindBegin and CONAFindEnd.
// 
//===================================================================
void CPhoneListBox::ShowPhoneFolder(CString strFolder)
{       
    TRACE(L"CPhoneListBox::ShowPhoneFolder(): Begin\n\tstrFolder = %s\n", strFolder);
    FINDHANDLE hFind = NULL;
    DWORD dwMedia = API_MEDIA_ALL;
    DWORD dwDeviceID = 0;
	DWORD dwFindOptions = 0;
	if(((CFileBrowserDlg*)GetParent())->GetCacheOption())
	{
		dwFindOptions |= CONA_FIND_USE_CACHE;
	}
    DWORD dwResult = CONAFindBegin(m_hFS, dwFindOptions, &hFind, strFolder);
    if(dwResult == CONA_OK)
    {            
        ResetContent();
        AddString(L"[..]");            
        ShowFolders(hFind);
        ShowFiles(hFind);
        m_strCurrentFolder = strFolder;
        dwResult = CONAFindEnd(hFind);                
        if(dwResult != CONA_OK)
        {
            ErrorMessageDlg(L"CPhoneListBox::ShowPhoneFolder(): CONAFindEnd failed!", dwResult);
        }
    }
    else
    {
        CString strErrortxt;
        strErrortxt.Format(L"CPhoneListBox::ShowPhoneFolder(): CONAFindBegin(%s) failed!\nm_hFS = 0x%X, hFind = 0x%X", strFolder, m_hFS, hFind);
        ErrorMessageDlg(strErrortxt, dwResult);
        ListAllPhones();
    }

    if(m_pLabel)
    {
        CONAPI_DEVICE Device = {0};
        dwResult = CONAGetDevice(m_hDM, m_strCurrentSN, &Device);
        if(dwResult == CONA_OK)
        {
            CString mFN = Device.pstrFriendlyName;
            m_pLabel->SetWindowText(mFN + L":" + strFolder);
            dwResult = CONAFreeDeviceStructure(1, &Device);
            if(dwResult != CONA_OK)
            {
                ErrorMessageDlg(L"CPhoneListBox::ShowPhoneFolder(): CONAFreeDeviceStructure failed!", dwResult);
            }
        }            
    }
    TRACE(L"CPhoneListBox::ShowPhoneFolder(): End\n");
}

//===================================================================
// GetCurrentSN
//
// Returns serial number of currently selected phone
// 
//===================================================================
CString CPhoneListBox::GetCurrentSN()
{
    CString strRetVal = L"";
    if(m_wState == PHONELIST_STATE_PHONELIST)
    { // phone list is shown
        int index = GetCurSel(); 
        if(index != LB_ERR)
        { // there is a selected item
            CONAPI_DEVICE *pDevice = (CONAPI_DEVICE*)GetItemDataPtr(index);
            if(pDevice)
            { // copying serial number to strRetVal
                strRetVal = CString(pDevice->pstrSerialNumber);
            }
        }
    }
    else
    { // user is browsing phone content
        strRetVal = m_strCurrentSN;
    }
    return strRetVal;
}

//===================================================================
// GetState
//
// Returns current state of listbox 
// (PHONELIST_STATE_PHONELIST or PHONELIST_STATE_PHONECONTENT)
// 
//===================================================================
DWORD CPhoneListBox::GetState()
{
    return m_wState;
}

//===================================================================
// GetFreeMemoryString
//
// Returns string containing free memory of "DEV"
// This function demonstrates use of functions
// CONARefreshDeviceMemoryValues, CONAGetMemoryTypes and
// CONAGetMemoryValues
// 
//===================================================================
CString CPhoneListBox::GetFreeMemoryString(CONAPI_DEVICE* pDevice)
{  
    TRACE(L"CPhoneListBox::GetFreeMemoryString(): Begin\n");
    CString strRetVal = L"";
	CString strTmp = L"";
    WCHAR* pstrMemoryTypes = NULL;
    DWORD dwMedia = pDevice->pItems->dwMedia;
    DWORD dwDeviceID = 0;
    __int64 dwFreeMem = -1, dwTotalMem = -1, dwUsedMem = -1;

	FSHANDLE hFS = OpenFS(pDevice->pstrSerialNumber);
	if(!hFS)
	{
		return strRetVal;
	}
    // refreshing memory values
    DWORD dwResult = CONARefreshDeviceMemoryValues(hFS);
    if(dwResult != CONA_OK)
    {
        ErrorMessageDlg(L"CPhoneListBox::GetFreeMemoryString(): CONARefreshDeviceMemoryValues failed!", dwResult);
    }
    
    // Getting memory types of connected device
    dwResult = CONAGetMemoryTypes(hFS, &pstrMemoryTypes);
    if(dwResult == CONA_OK)
    {
        // go through memory type list:
        WCHAR* separator = L",";
		wchar_t* next_token;
		bool bFirst = true;
        WCHAR* token = wcstok_s(pstrMemoryTypes, separator, &next_token);
        while(token != NULL)
        {
            dwResult = CONAGetMemoryValues(hFS, token, &dwFreeMem, &dwTotalMem, &dwUsedMem);
            if(dwResult == CONA_OK)
            {
                TRACE(L"CPhoneListBox::GetFreeMemoryString(): %s: free %0.2f MB used %0.2f/%0.2f MB\n",
                    token, (double)dwFreeMem/1024/1024, (double)dwUsedMem/1024/1024, (double)dwTotalMem/1024/1024);

				// let's show memory values of the memory type
				if(bFirst)
				{
					bFirst = false;
					strTmp.Format(L"%s: ", token);
				}
				else
				{
					strTmp.Format(L", %s: ", token);
				}
                if(dwFreeMem != -1)
                    strTmp.Format(strTmp + L"Free %0.2f MB", (double)dwFreeMem/1024/1024);
                if(dwTotalMem != -1 && dwUsedMem != -1)
                    strTmp.Format(strTmp + L", used %0.2f/%0.2f MB", 
						(double)dwUsedMem/1024/1024, (double)dwTotalMem/1024/1024);
				strRetVal += strTmp;
            }
            else
            { // CONAGetMemoryValues failed
                ErrorMessageDlg(L"CPhoneListBox::GetFreeMemoryString(): CONAGetMemoryValues failed!", dwResult);
            } 
            token = wcstok_s(NULL, separator, &next_token);
        }
        // pstrMemoryTypes was allocated by CONAGetMemoryTypes
        // It must be de-allocated
		dwResult = CONAFreeString(pstrMemoryTypes);
        if(dwResult != CONA_OK)
        {
            ErrorMessageDlg(L"CPhoneListBox::GetFreeMemoryString(): CONAFreeString failed!", dwResult);
        }
    }
    else
    { // CONAGetMemoryTypes failed
        ErrorMessageDlg(L"CPhoneListBox::GetFreeMemoryString(): CONAGetMemoryTypes failed!", dwResult);
    }
	if(pDevice->pstrSerialNumber == m_strCurrentSN)
	{
		m_hFS = hFS;
	}
	else
	{
		CloseFS(hFS);
	}
	TRACE(L"CPhoneListBox::GetFreeMemoryString(): End\n");
    return strRetVal;
}

//===================================================================
// AddPhone
//
// Adds phone defined in CONAPI_DEVICE struct
// to list box
// 
//===================================================================
int CPhoneListBox::AddPhone(CONAPI_DEVICE* pDevice)
{
    TRACE(L"CPhoneListBox::AddPhone(): Begin\n");
    
    // Write content of pDevice to output window:
    TRACE(L"CPhoneListBox::AddPhone(): CONAPI_DEVICE\n");
    TRACE(L"\tSerial number: %s\n", pDevice->pstrSerialNumber);
    TRACE(L"\tFriendly name: %s\n", pDevice->pstrFriendlyName);
    TRACE(L"\tModel: %s\n", pDevice->pstrModel);
    TRACE(L"\tManufacturer: %s\n", pDevice->pstrManufacturer);
    TRACE(L"\tNumberOfItems: %d\n", pDevice->dwNumberOfItems);
    for(DWORD i = 0; i < pDevice->dwNumberOfItems; i++)
    {
        TRACE(L"\t\tConnection %d, DeviceID: %d\n", i + 1, pDevice->pItems[i].dwDeviceID);
        TRACE(L"\t\tConnection %d, DeviceName: %s\n", i + 1, pDevice->pItems[i].pstrDeviceName);
        if(pDevice->pItems[i].dwMedia == API_MEDIA_BLUETOOTH)
            TRACE(L"\t\tConnection %d, DeviceName : %s\n", i + 1, pDevice->pItems[i].pstrDeviceName);
        TRACE(L"\t\tConnection %d, State: %d\n", i + 1, pDevice->pItems[i].dwState);
    }
    CString strPhoneinfotxt;
    CString strFreeMemoryStr = GetFreeMemoryString(pDevice);

    strPhoneinfotxt.Format(L"%s (%s)", 
        pDevice->pstrFriendlyName,
        strFreeMemoryStr);
    int index = AddString(strPhoneinfotxt);
	
    // Copy pDevice to item data of listbox item:
    CONAPI_DEVICE *pDeviceCopy = new CONAPI_DEVICE;
	pDeviceCopy->pstrSerialNumber = new WCHAR[MAX_PATH];
	pDeviceCopy->pstrFriendlyName = new WCHAR[MAX_PATH];
	pDeviceCopy->pstrModel = new WCHAR[MAX_PATH];
	pDeviceCopy->pstrManufacturer = new WCHAR[MAX_PATH];
	
    pDeviceCopy->dwNumberOfItems = pDevice->dwNumberOfItems;
    pDeviceCopy->pItems = 0;    
    wcsncpy_s(pDeviceCopy->pstrSerialNumber, MAX_PATH, pDevice->pstrSerialNumber, MAX_PATH);
    wcsncpy_s(pDeviceCopy->pstrFriendlyName, MAX_PATH, pDevice->pstrFriendlyName, MAX_PATH);
    wcsncpy_s(pDeviceCopy->pstrModel, MAX_PATH, pDevice->pstrModel, MAX_PATH);
    wcsncpy_s(pDeviceCopy->pstrManufacturer, MAX_PATH, pDevice->pstrManufacturer, MAX_PATH);

    SetItemDataPtr(index, pDeviceCopy);
       
    TRACE(L"CPhoneListBox::AddPhone(): End\n");
    return index;
}

//===================================================================
// ListAllPhones
//
// Adds all connected phones to list box
// 
//===================================================================
void CPhoneListBox::ListAllPhones()
{
    TRACE(L"CPhoneListBox::ListAllPhones(): Begin\n");
    DWORD dwDeviceCount = 0;
    DWORD dwIndex = 0;
    CONAPI_DEVICE* pDevices = NULL;
	CString strText = L"";
	CSize maxSize(0,0), textSize(0,0);

    ResetContent();
    m_wState = PHONELIST_STATE_PHONELIST;

    // Querying count of connected devices
    DWORD dwResult = CONAGetDeviceCount(m_hDM, &dwDeviceCount);
    if(dwResult == CONA_OK)
    {
        if(dwDeviceCount > 0)
        {
		    // Get list of currently connected devices
		    pDevices = new CONAPI_DEVICE[dwDeviceCount];
		    dwResult = CONAGetDevices(m_hDM, &dwDeviceCount, pDevices);
		    if(dwResult == CONA_OK)
		    {
			    // Add each device to the phone list box
			    while ( dwIndex < dwDeviceCount )
			    {
					CString strSN = pDevices[dwIndex].pstrSerialNumber;
					if(dwIndex == 0)
					{
						m_strCurrentSN = strSN;
					}
                    int idx = AddPhone(&pDevices[dwIndex]);
					
					// Update horizontal scrollbar width
					GetText(idx, strText);
					textSize = GetDC()->GetOutputTextExtent(strText);
					if (maxSize.cx < textSize.cx)
					{
						SetHorizontalExtent(textSize.cx);
						maxSize = textSize;
					}

				    dwIndex++;
			    }

			    // CONAGetDevices allocates memory for the member variables in
			    // CONAPI_DEVICE and it is callers responsibility to free it...
			    dwResult = CONAFreeDeviceStructure(dwDeviceCount, pDevices);
                if(dwResult != CONA_OK)
                {
                    ErrorMessageDlg(L"CPhoneListBox::ListAllPhones(): CONAFreeDeviceStructure failed!", dwResult);
                }
		    }
            else
            { // CONAGetDevices failed
                ErrorMessageDlg(L"CPhoneListBox::ListAllPhones(): CONAGetDevices failed!", dwResult);
            }

            // freeing allocated memory
            delete[] pDevices; pDevices = NULL;
        }
    }
    else
    { // CONAGetDeviceCount failed
        ErrorMessageDlg(L"CPhoneListBox::ListAllPhones(): CONAGetDeviceCount failed!", dwResult);
    }
	if(GetCount() > 0)
	{
		SetCurSel(0);
	}
    if(m_pLabel)
    {
        m_pLabel->SetWindowText(L"Connected phones:");
    }
    TRACE(L"CPhoneListBox::ListAllPhones(): End\n");
}

//===================================================================
// ResetContent
//
// Freeing allocated memory
// 
//===================================================================
void CPhoneListBox::ResetContent()
{
    for(int i = 0; i < GetCount(); i++)
    { 
        if(m_wState == PHONELIST_STATE_PHONELIST)
        { // contains phones --> let's delete CONAPI_DEVICE struct
            CONAPI_DEVICE* pDevice = (CONAPI_DEVICE*)GetItemDataPtr(i);
            if(pDevice)
            {
	            delete[] pDevice->pstrSerialNumber;
	            delete[] pDevice->pstrFriendlyName;
	            delete[] pDevice->pstrModel;
	            delete[] pDevice->pstrManufacturer;
                delete pDevice;
                pDevice = NULL;
            }
        }
        else if(m_wState == PHONELIST_STATE_PHONECONTENT)
        { // contains phone content --> let's delete file/folder name
            CString* pstrName = (CString*)GetItemDataPtr(i);
            if(pstrName)
            {
	            delete pstrName;
                pstrName = NULL;
            }
        }
    }
    CListBox::ResetContent();
}

//===================================================================
// GetSelectedFolder
//
// Returns currently selected folder
// 
//===================================================================
CString CPhoneListBox::GetSelectedFolder()
{
    CString strRetVal = L"";
    // Let's check if there is phone folder selected
    int index = GetCurSel();
    if(m_wState == PHONELIST_STATE_PHONECONTENT && index != LB_ERR)
    { // there is a selected item
        strRetVal = m_strCurrentFolder;
        CString strSelectedTxt;
        GetText(index, strSelectedTxt);
        if(strSelectedTxt[0] == '[' && strSelectedTxt != L"[..]")
        { // folder [directoryname]
            if(strRetVal.Right(1) != L"\\")
                strRetVal += L"\\";
            strRetVal += *((CString*)GetItemDataPtr(index));
        }            
    }
    return strRetVal;
}

//===================================================================
// GetCurrentFolder
//
// Returns folder, the content of which is currently shown 
// 
//===================================================================
CString CPhoneListBox::GetCurrentFolder()
{
    return m_strCurrentFolder;
}

//===================================================================
// GetCurrentFile
//
// Returns selected file. Return empty string if no file is selected.
// 
//===================================================================
CString CPhoneListBox::GetCurrentFile()
{
    CString strRetVal = L"";
    int index = GetCurSel();
    if(m_wState == PHONELIST_STATE_PHONECONTENT && index != LB_ERR)
    { // there is a selected item
        CString strSelectedTxt;
        GetText(index, strSelectedTxt);
        if(strSelectedTxt[0] != '[')
        { // selected item is file
            strRetVal = *((CString*)GetItemDataPtr(index));
        }            
    }
    return strRetVal;
}

//===================================================================
// OpenFS
//
// Opens phone file system handle.
// 
//===================================================================
FSHANDLE CPhoneListBox::OpenFS(CString strSN)
{
	DWORD dwMedia = API_MEDIA_ALL;
	FSHANDLE hFS = NULL;
	DWORD dwDeviceID = 0;        
	DWORD dwResult = CONAOpenFS(strSN, &dwMedia, &hFS, &dwDeviceID);
	if(dwResult != CONA_OK)
	{
		ErrorMessageDlg(L"CPhoneListBox::OpenFS(): CONAOpenFS failed!", dwResult);
		return NULL;
	}    
	return hFS;
}

//===================================================================
// CloseFS
//
// Closes phone file system handle.
// 
//===================================================================
void CPhoneListBox::CloseFS(FSHANDLE hFS)
{
	if(hFS)
	{
		DWORD dwResult = CONACloseFS(hFS);
		if(dwResult != CONA_OK)
		{
			ErrorMessageDlg(L"CPhoneListBox::CloseFS(): CONACloseFS failed!", dwResult);
		}
	}
}

//===================================================================
// CPhoneListBox message handlers

//===================================================================
// OnDestroy
//
// Called before destruction
// 
//===================================================================
void CPhoneListBox::OnDestroy() 
{
	ResetContent();	
    CListBox::OnDestroy();		
}

//===================================================================
// OnLButtonDblClk
//
// Called when user doubleclicks list item
// 
//===================================================================
void CPhoneListBox::OnLButtonDblClk(UINT nFlags, CPoint point) 
{    
    switch(m_wState)
    {
    case PHONELIST_STATE_PHONELIST:
		{
			CString strCurrentSN = GetCurrentSN();
			if(!strCurrentSN.IsEmpty())
			{
				ResetContent(); // removing phones and cleaning memory
				if(strCurrentSN != m_strCurrentSN)
				{
					m_strCurrentSN = strCurrentSN;
					CloseFS(m_hFS);
					m_hFS = OpenFS(m_strCurrentSN);
				}
				if(m_hFS)
				{
					m_wState = PHONELIST_STATE_PHONECONTENT;
					ShowPhoneFolder(g_strPhoneRoot);
				}
			}
        }
        break;
    case PHONELIST_STATE_PHONECONTENT:
        PhoneItemDblClicked();
        break;
    }
	
	CListBox::OnLButtonDblClk(nFlags, point);
}

//===================================================================
// CCONADeviceNotify implementation

void CPhoneListBox::OnDeviceListUpdated()
{
    TRACE(L"CPhoneListBox::DeviceListUpdated()\n");
    if(m_wState == PHONELIST_STATE_PHONELIST)
    {
        ListAllPhones();
    }
}

void CPhoneListBox::OnDeviceAdded(const WCHAR* pstrSN, DWORD dwStatus)
{
    DWORD event = GET_CONAPI_CB_STATUS(dwStatus);
    DWORD info = GET_CONAPI_CB_INFO(dwStatus);
    DWORD infodata = GET_CONAPI_CB_INFO_DATA(dwStatus);
    
    VERIFY(event == CONAPI_DEVICE_ADDED);    
    VERIFY(info == CONAPI_CONNECTION_ADDED);
    
    TRACE(L"CPhoneListBox::DeviceAdded(%s, %i)\n", pstrSN, dwStatus);
    if(m_wState == PHONELIST_STATE_PHONELIST)
    {
        ListAllPhones();
    }
}

void CPhoneListBox::OnDeviceRemoved(const WCHAR* pstrSN, DWORD dwStatus)
{
    TRACE(L"CPhoneListBox::DeviceRemoved(%s, %i)\n", pstrSN, dwStatus);
    if(m_wState == PHONELIST_STATE_PHONELIST)
    {
        ListAllPhones();
    }
    else
    {
        if(CString(pstrSN) == GetCurrentSN())
        {
            AfxMessageBox(L"Connection to device lost!");
            ResetContent();
            m_wState = PHONELIST_STATE_PHONELIST;            
            ListAllPhones();
        }
    }
}

void CPhoneListBox::OnDeviceUpdated(const WCHAR* pstrSN, DWORD dwStatus)
{
    TRACE(L"CPhoneListBox::DeviceUpdated(%s, %i)\n", pstrSN, dwStatus);
    if(m_wState == PHONELIST_STATE_PHONELIST)
    {
        ListAllPhones();
    }
}
