/*
Filename    : PhoneListBox.h
Part of     : File Browser
Description : Declaration of File Browser's phone list box
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

#ifndef __NOKIA_PCCS_FILEBROWSER_EXAMPLE__PHONELISTBOX_H__
#define __NOKIA_PCCS_FILEBROWSER_EXAMPLE__PHONELISTBOX_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	PHONELIST_STATE_PHONELIST	 1
#define	PHONELIST_STATE_PHONECONTENT 2

//===================================================================
// CPhoneListBox window

class CPhoneListBox : public CListBox, IDMAPIDeviceNotify
{
// Construction
public:
	CPhoneListBox();

// Attributes
public:

// Operations
public:
    void Init(CStatic* pLabel);
    void ListAllPhones();    
    void ResetContent();
    CString GetSelectedFolder();
    CString GetCurrentFolder();
    CString GetCurrentFile();
    CString GetCurrentSN();
    DWORD GetState();
    void ShowPhoneFolder(CString strFolder);
    DMHANDLE GetDMHandle(){ return m_hDM; };
    FSHANDLE GetFSHandle(){ return m_hFS; };

    // IDMAPIDeviceNotify methods
	void OnDeviceListUpdated();
    void OnDeviceAdded(const WCHAR* pstrSN, DWORD dwStatus);
    void OnDeviceRemoved(const WCHAR* pstrSN, DWORD dwStatus);
    void OnDeviceUpdated(const WCHAR* pstrSN, DWORD dwStatus);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhoneListBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPhoneListBox();

private:
	FSHANDLE OpenFS(CString strSN);
	void CloseFS(FSHANDLE hFS);
    int AddPhone(CONAPI_DEVICE* pDevice);
    void PhoneItemDblClicked();
    void ShowFolders(FINDHANDLE hFindHandle);
    void ShowFiles(FINDHANDLE hFindHandle);    
    CString GetFreeMemoryString(CONAPI_DEVICE* pDevice);

	// Generated message map functions
protected:
    DMHANDLE m_hDM; // Device management connection handle
    FSHANDLE m_hFS; // File system handle
    WORD m_wState; // state of the phonelist
    CString m_strCurrentSN; // serial number of currently selected phone
    CString m_strCurrentFolder; // current folder that is shown in list box
    CStatic* m_pLabel; // static control in the main window

	//{{AFX_MSG(CPhoneListBox)
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
};

//===================================================================

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__NOKIA_PCCS_FILEBROWSER_EXAMPLE__PHONELISTBOX_H__)
