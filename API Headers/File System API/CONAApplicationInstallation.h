/*
==============================================================================
* File System Application Installation API 3.0
*
Filename    : CONAApplicationInstallation.h
Description : Application Installation functions
Version     : 3.0

Copyright (c) 2005, 2006 Nokia Corporation.
This software, including but not limited to documentation and any related 
computer programs ("Software"), is protected by intellectual property rights 
of Nokia Corporation and/or its licensors. All rights are reserved. By using 
the Software you agree to the terms and conditions hereunder. If you do not 
agree you must cease using the software immediately.
Reproducing, disclosing, modifying, translating, or distributing any or all 
of the Software requires the prior written consent of Nokia Corporation. 
Nokia Corporation retains the right to make changes to the Software at any 
time without notice.

A copyright license is hereby granted to use of the Software to make, publish, 
distribute, sub-license and/or sell new Software utilizing this Software. 
The Software may not constitute the primary value of any new software utilizing 
this software. No other license to any other intellectual property rights of 
Nokia or a third party is granted. 

THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS 
OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF NON-INFRINGEMENT, 
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
NOKIA CORPORATION BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL, 
OR CONSEQUENTIAL LOSS OR DAMAGES, INCLUDING BUT NOT LIMITED TO, LOST PROFITS 
OR REVENUE, LOSS OF USE, COST OF SUBSTITUTE PROGRAM, OR LOSS OF DATA OR EQUIPMENT 
ARISING OUT OF THE USE OR INABILITY TO USE THE MATERIAL, EVEN IF 
NOKIA CORPORATION HAS BEEN ADVISED OF THE LIKELIHOOD OF SUCH DAMAGES OCCURRING. 
==============================================================================
*/
#ifndef _CONAAPPLICATIONINSTALLATION_H_
#define _CONAAPPLICATIONINSTALLATION_H_

///////////////////////////////////////////////////////////
// PC Connectivity API
///////////////////////////////////////////////////////////
	#ifndef PCCS_FSAPI
		#define PCCS_FSAPI DWORD STDAPICALLTYPE
		#define PCCS_FSAPI_EX(type) type STDAPICALLTYPE
	#endif
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// File System API definitions
///////////////////////////////////////////////////////////
	#include "FSAPIDefinitions.h"
///////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// File System API Application Installation functions
/////////////////////////////////////////////////////////////////////

	//=========================================================
	// CONAInstallApplication 
	//
	// Description:
	//	CONAInstallApplication installs the given application on 
	//	the device. FS's CONACancel function can be used to cancel 
	//	this function. The application file name(s) must be given 
	//	in the CONAPI_APPLICATION_SIS, CONAPI_APPLICATION_JAVA or
	//	CONAPI_APPLICATION_FILE structure. 
	//	
	//	The function also checks the amount of free memory from 
	//	the phone before the installation. If there is not enough 
	//	memory left in the target memory drive, the function fails 
	//	with the error code ECONA_MEMORY_FULL.
	//	
	//	If the Application file type is sis or .sisx, the CONAPI_APPLICATION_SIS 
	//	structure must be used. The dwAppicationType parameter's 
	//	value must be CONA_APPLICATION_TYPE_SIS.
	//	NOTE 1: The file type of sis or sisx is NOT allowed for Series 40 device. 
	//	NOTE 2: The file type of sisx is allowed only for Series 60 3nd edition device.
	//	
	//	If the Application file type is jar, the CONAPI_APPLICATION_JAVA 
	//	structure must be used. The dwAppicationType parameter's 
	//	value must be CONA_APPLICATION_TYPE_JAVA.
	//
	//	If the Application file type is nth, the CONAPI_APPLICATION_FILE 
	//	structure must be used. he dwAppicationType parameter's 
	//	value must be CONA_APPLICATION_TYPE_THEMES.
	//	NOTE 1: This is supported only with Connectivity API 2.1 or newer.
	//	NOTE 2: The file type of nth is allowed only for Series 40 device.
	//
	//
	// Parameters:
	//	hFSHandle			[in] File System handle
	//	dwApplicationType	[in] Used struct type: CONA_APPLICATION_TYPE_SIS,  
	//						   CONA_APPLICATION_TYPE_JAVA or CONA_APPLICATION_TYPE_THEMES.
	//	pApplicationStruct	[in] Pointer to CONAPI_APPLICATION_SIS, 
	//							 CONAPI_APPLICATION_JAVA or CONAPI_APPLICATION_FILE struct.
	//	dwOptions			[in] Options: 
	//		CONA_DEFAULT_FOLDER: Copies the application to the device's default 
	//		application folder automatically and starts the device's installer 
	//		(if required to do so).Target path or current folder is not used.  
	//
	//		CONA_OVERWRITE:	Overwrites the application file if it is already 
	//		exists in the target folder.
	//
	//		CONA_INFORM_IF_USER_ACTION_NEEDED: Function returns a CONA_OK_BUT_USER_ACTION_NEEDED
	//		result code if the user have to finish the installation from device side.
	//
	//		CONA_WAIT_THAT_USER_ACTION_IS_DONE: If the user have to finish the installation from
	//		device side, function returns when this is done. A maximum waiting time is 15 minutes
	//		for this user action. Whole this waiting time ConnAPI sends File System callback 
	//		nofications and dwState value is CONA_OK_BUT_USER_ACTION_NEEDED.	
	//		NOTE: If device does not support this feature, function returns the 
	//		CONA_OK_BUT_USER_ACTION_NEEDED result code. Same result code is returned if  
	//		user has not to finish the installation on the maximum waiting time.
	//
	//	pstrSourcePath		[in] Source folder path on the PC. The value must be NULL, 
	//					 if path is given with file name(s) in Application structure.
	//	pstrTargetPath   	[in] Target folder.If NULL, current folder is used.
	//					 If CONA_DEFAULT_FOLDER is used,Application will 
	//					 be installed to device's default application folder. 
	//
	// Return values:
	// CONA_OK
	// CONA_OK_BUT_USER_ACTION_NEEDED
	// CONA_WAIT_CONNECTION_IS_BUSY
	// ECONA_INVALID_HANDLE
	// ECONA_INVALID_PARAMETER
	// ECONA_INVALID_POINTER
	// ECONA_CONNECTION_BUSY
	// ECONA_CONNECTION_LOST
	// ECONA_INVALID_DATA_DEVICE
	// ECONA_MEMORY_FULL
	// ECONA_CURRENT_FOLDER_NOT_FOUND
	// ECONA_FOLDER_PATH_TOO_LONG
	// ECONA_FOLDER_NAME_INVALID
	// ECONA_FOLDER_NOT_FOUND
	// ECONA_FOLDER_NO_PERMISSION_ON_PC
	// ECONA_FOLDER_NO_PERMISSION
	// ECONA_FILE_TOO_BIG_DEVICE
	// ECONA_FILE_NAME_INVALID
	// ECONA_FILE_NAME_TOO_LONG
	// ECONA_FILE_TYPE_NOT_SUPPORTED
	// ECONA_FILE_NOT_FOUND
	// ECONA_FILE_ALREADY_EXIST
	// ECONA_FILE_NO_PERMISSION
	// ECONA_FILE_NO_PERMISSION_ON_PC
	// ECONA_FILE_BUSY
	// ECONA_DEVICE_INSTALLER_BUSY
	// ECONA_CANCELLED
	// ECONA_FAILED_TIMEOUT
	// ECONA_NOT_SUPPORTED_DEVICE
	// ECONA_UNKNOWN_ERROR_DEVICE
	// ECONA_NOT_INITIALIZED
	// ECONA_NOT_SUPPORTED_MANUFACTURER
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_FSAPI CONAInstallApplication( FSHANDLE hFSHandle,
										 DWORD	dwApplicationType,
										 LPVOID  pApplicationStruct,
										 DWORD	dwOptions,
										 const WCHAR* pstrSourcePath,
										 const WCHAR* pstrTargetPath);
	//=========================================================

	//=========================================================
	// CONAListApplications 
	//
	// Description:
	//	CONAListApplications function list all installed applications on 
	//	the device. FS's CONACancel function can be used to cancel 
	//	this function. 
	//	NOTE 1: Only new Series 60 3nd edition devices are supported at the moment. 
	//	        If target device is not supported function fails with error code ECONA_NOT_SUPPORTED_DEVICE.
	//	NOTE 2: If target device is supported, CONAGetDeviceInfo function returns value CONAPI_FS_LIST_APPLICATIONS.
	//	
	//
	//
	// Parameters:
	//	hFSHandle			[in] File System handle
	//	dwOptions			[in] Options: 
	//			CONA_LIST_ALL_APPICATIONS: List all installed applications. This value must be used.
	//	pdwNumberOfAppInfoStructures [out] Number of CONAPI_APPLICATION_INFO structures. 
	//	ppAppInfoStructures			 [out] Pointer to CONAPI_APPLICATION_INFO structure(s). 
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_HANDLE
	// ECONA_INVALID_PARAMETER
	// ECONA_INVALID_POINTER
	// ECONA_CONNECTION_BUSY
	// ECONA_CONNECTION_LOST
	// ECONA_INVALID_DATA_DEVICE
	// ECONA_MEMORY_FULL
	// ECONA_CANCELLED
	// ECONA_FAILED_TIMEOUT
	// ECONA_NOT_SUPPORTED_DEVICE
	// ECONA_UNKNOWN_ERROR_DEVICE
	// ECONA_NOT_INITIALIZED
	// ECONA_NOT_SUPPORTED_MANUFACTURER
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_FSAPI CONAListApplications( FSHANDLE hFSHandle,
									   DWORD	  dwOptions,
									   DWORD*	  pdwNumberOfAppInfoStructures,
									   CONAPI_APPLICATION_INFO**	ppAppInfoStructures);
	//=========================================================

	//=========================================================
	// CONAUninstallApplication 
	//
	// Description:
	//	CONAUninstallApplication function uninstalls application
	//	from device. FS's CONACancel function can be used to cancel 
	//	this function. 
	//	NOTE 1: Only new Series 60 3nd edition devices are supported at the moment. 
	//	        If target device is not supported, function fails with error code 
	//			ECONA_NOT_SUPPORTED_DEVICE. And if target device is supported, 
	//			CONAGetDeviceInfo function returns value CONAPI_FS_UNINSTALL_APPLICATIONS.
	//	
	// Parameters:
	//	hFSHandle			[in] File System handle
	//	dwOptions			[in] Options: 
	//		CONA_DEFAULT_UNINSTALLATION: Default uninstallation, the user may have to finish the uninstallation from device side.
	//		If user action is needed, a maximum waiting time is 15 minutes for this user action. Whole this waiting time ConnAPI 
	//		sends File System callback nofications and dwState value is CONA_OK_BUT_USER_ACTION_NEEDED.
	//		CONA_SILENT_UNINSTALLATION: Silent uninstallation, no need user action from device side.
	//	pstrName			[in] Target application Name, see CONAPI_APPLICATION_INFO structure. This value must be set.
	//	pstrUID				[in] Target application UID, see CONAPI_APPLICATION_INFO structure. This value must be set.
	//
	// Return values:
	// CONA_OK
	// CONA_OK_BUT_USER_ACTION_NEEDED
	// ECONA_INVALID_HANDLE
	// ECONA_INVALID_PARAMETER
	// ECONA_INVALID_POINTER
	// ECONA_CONNECTION_BUSY
	// ECONA_CONNECTION_LOST
	// ECONA_INVALID_DATA_DEVICE
	// ECONA_FILE_NO_PERMISSION
	// ECONA_FILE_NOT_FOUND
	// ECONA_FILE_BUSY
	// ECONA_MEMORY_FULL
	// ECONA_CANCELLED
	// ECONA_FAILED_TIMEOUT
	// ECONA_NOT_SUPPORTED_DEVICE
	// ECONA_UNKNOWN_ERROR_DEVICE
	// ECONA_NOT_INITIALIZED
	// ECONA_NOT_SUPPORTED_MANUFACTURER
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_FSAPI CONAUninstallApplication( FSHANDLE hFSHandle,
										   DWORD	  dwOptions,
										   const WCHAR* pstrAppName,
										   const WCHAR* pstrAppUID);
	//=========================================================

	//=========================================================
	// CONAFreeApplicationInfoStructures
	//
	// Description:
	//	CONAFreeApplicationInfoStructures releases the CONAPI_APPLICATION_INFO structs, 
	//	which CONAListApplications function is allocated.
	//
	// Parameters:
	//	dwNumberOfAppInfoStructures	[in] Number of CONAPI_APPLICATION_INFO structures.
	//	ppAppInfoStructures	[in] Pointer to CONAPI_APPLICATION_INFO structure(s).
	//
	// Return values:
	//	CONA_OK
	//	ECONA_INVALID_POINTER
	//	ECONA_INVALID_PARAMETER
	//	ECONA_UNKNOWN_ERROR
	//
		PCCS_FSAPI CONAFreeApplicationInfoStructures( 
												DWORD dwNumberOfAppInfoStructures, 
												CONAPI_APPLICATION_INFO** ppAppInfoStructures );
	//=========================================================

#endif
