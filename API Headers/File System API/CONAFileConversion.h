/*
==============================================================================
* File System Conversion API 3.0
*
Filename    : CONAFileConversion.h
Description : File conversion functions
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
#ifndef _CONAFILECONVERSION_H_
#define _CONAFILECONVERSION_H_

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
// File System API File Conversion functions
/////////////////////////////////////////////////////////////////////

	//=========================================================
	// CONAConvertFile 
	//
	// Description:
	//
	//  CONAConvertFile converts a file on Device's target folder 
	//  from Symbian format to Windows format. Function returns a 
	//  converted new file name.
	//
	//  The supported file extensions and MIME types, which can 
	//  be converted on device, can get CONAGetConvertFileTypes function.
	//
	// Parameters:
	//	hFSHandle			[in]  File System handle
	//	pstrFileName		[in]  Target file name.
	//	ppstrNewFileName	[out] Converted new file name.
	//	pstrTargetPath   	[in]  Target path. If the path is NULL, the current folder is used.
	//
	// Return values:
	// CONA_OK
	// ECONA_INVALID_HANDLE
	// ECONA_INVALID_PARAMETER
	// ECONA_INVALID_POINTER
	// ECONA_CONNECTION_BUSY
	// ECONA_CONNECTION_LOST
	// ECONA_MEMORY_FULL
	// ECONA_CURRENT_FOLDER_NOT_FOUND
	// ECONA_FOLDER_PATH_TOO_LONG
	// ECONA_FOLDER_NAME_INVALID
	// ECONA_FILE_NAME_TOO_LONG
	// ECONA_FILE_NOT_FOUND
	// ECONA_FILE_NO_PERMISSION
	// ECONA_FILE_BUSY
	// ECONA_FILE_TYPE_NOT_SUPPORTED
	// ECONA_NOT_SUPPORTED_DEVICE
	// ECONA_CANCELLED
	// ECONA_FAILED_TIMEOUT
	// ECONA_UNKNOWN_ERROR_DEVICE
	// ECONA_NOT_INITIALIZED
	// ECONA_UNKNOWN_ERROR
	//
		PCCS_FSAPI CONAConvertFile(
							FSHANDLE		hFSHandle,
							const WCHAR*	pstrFileName,
							WCHAR**			ppstrNewFileName,
							const WCHAR*	pstrTargetPath );
	//=========================================================

	//=========================================================
	// CONAGetConvertFileTypes 
	//
	// Description:
	//
	//  CONAGetConvertFileTypes returns CONAPI_CONVERT_FILE_TYPE structures, 
	//  which include all possible file formats that can be converted on device. 
	//  Structure includes also output file format. If device does not support 
	//  conversion, function returns an error code ECONA_NOT_SUPPORTED_DEVICE.
	//
	//  The returned CONAPI_CONVERT_FILE_TYPE structures must be release by 
	//  CONAFreeConvertFileTypes function. 
	// Parameters:
	//	hFSHandle				[in]  File System handle
	//	pdwNumberOfStructures	[out] Number of CONAPI_CONVERT_FILE_TYPE structures.
	//	ppFileTypeStructures	[out] Pointer to CONAPI_CONVERT_FILE_TYPE structure(s).
	//
	// Return values:
	//	CONA_OK
	//	ECONA_INVALID_HANDLE
	//	ECONA_INVALID_POINTER
	//	ECONA_NOT_SUPPORTED_DEVICE
	//	ECONA_NOT_ENOUGH_MEMORY
	//	ECONA_NOT_INITIALIZED
	//	ECONA_UNKNOWN_ERROR
	//
		PCCS_FSAPI CONAGetConvertFileTypes(	
							FSHANDLE hFSHandle,
							DWORD*	 pdwNumberOfStructures,
							CONAPI_CONVERT_FILE_TYPE**	ppFileTypeStructures );
	//=========================================================

	//=========================================================
	// CONAFreeConvertFileTypes 
	//
	// Description:
	//	
	//  CONAFreeConvertFileTypes releases the CONAPI_CONVERT_FILE_TYPE structures, 
	//	which the CONAGetConvertFileTypes function allocates.
	//
	// Parameters:
	//	dwNumberOfStructures	[in] Number of the used CONAPI_CONVERT_FILE_TYPE structures. 
	//      The value must be same number value which CONAGetConvertFileTypes function is returned.
	//	ppFileTypeStructures	[in] Pointer to CONAPI_CONVERT_FILE_TYPE structure(s).  
	//
	// Return values:
	//	CONA_OK
	//	ECONA_INVALID_POINTER
	//  ECONA_INVALID_PARAMETER
	//	ECONA_UNKNOWN_ERROR
	//
		PCCS_FSAPI CONAFreeConvertFileTypes(
							DWORD dwNumberOfStructures, 
							CONAPI_CONVERT_FILE_TYPE** ppFileTypeStructures );
	//=========================================================

#endif



