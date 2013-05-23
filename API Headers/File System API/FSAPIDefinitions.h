/*
==============================================================================
* File System API 3.2
*
Filename    : FSAPIDefinitions.h
Description : File System API definitions 
Version     : 3.2

Copyright (c) 2005-2007 Nokia Corporation.
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
#ifndef _FSAPIDEFINITIONS_H__
#define _FSAPIDEFINITIONS_H__

//=========================================================
// Common data type definitions used in Connectivity API
//
	#include "PCCSTypeDefinitions.h"
//=========================================================

/////////////////////////////////////////////////////////////////////
// File System API definitions
/////////////////////////////////////////////////////////////////////

	// File System API handle types	
	typedef APIHANDLE	FSHANDLE;			// File system handle
	typedef APIHANDLE	FINDHANDLE;			// FSAPI find handle

	//Used for changing current folder:
	#define GO_TO_ROOT_FOLDER				L"\\\\"        
	#define GO_TO_PARENT_FOLDER				L"..\\"
	#define FOLDER_SEPARATOR				L"\\"

	//Options for CONADeleteFolder:
	#define CONA_DELETE_FOLDER_EMPTY		0x00000000	
	#define CONA_DELETE_FOLDER_WITH_FILES	0x00000001

	//Direction options for CONACopyFile and CONAMoveFile:
	#define CONA_DIRECT_PHONE_TO_PC			0x00000002
	#define CONA_DIRECT_PC_TO_PHONE			0x00000004
	#define CONA_DIRECT_PHONE_TO_PHONE		0x00000008	// Not used at the moment.

	//Other options for CONACopyFile and CONAMoveFile:
	#define CONA_OVERWRITE					0x00000010	
	#define CONA_RENAME						0x00000020	// Used only with CONACopyFile
	#define CONA_TRANSFER_ALL				0x00000040	// Not used at the moment.

	//Options for CONAFindBegin:
	#define CONA_FIND_USE_CACHE				0x00000080

	//Attribute defines for CONAPI_FOLDER_INFO,CONAPI_FOLDER_INFO2 and CONAPI_FILE_INFO structures:
	#define CONA_FPERM_READ					0x00000100	// All structures
	#define CONA_FPERM_WRITE				0x00000200	// All structures
	#define CONA_FPERM_DELETE				0x00000400	// All structures
	#define CONA_FPERM_FOLDER				0x00000800	// CONAPI_FOLDER_INFO and CONAPI_FOLDER_INFO2
	#define CONA_FPERM_DRIVE				0x00001000	// CONAPI_FOLDER_INFO and CONAPI_FOLDER_INFO2
	#define CONA_FPERM_HIDDEN				0x00002000	// Only for CONAPI_FOLDER_INFO2
	#define CONA_FPERM_ROOT					0x00004000	// Only for CONAPI_FOLDER_INFO2

	//Options for CONAGetFolderInfo
	#define	CONA_GET_FOLDER_INFO					0x00000001	// Gets target folder info
	#define	CONA_GET_FOLDER_CONTENT					0x00000002	// Gets target folder info and contents
	#define	CONA_GET_FOLDER_AND_SUB_FOLDERS_CONTENT	0x00000004	// Gets target folder info, content and sub folder(s) contents also
	#define	CONA_COMPARE_AND_UPDATE_IF_NEEDED		0x00000100  // Compare exist folder content. If change has happened, updates content
																// and returns CONA_OK_UPDATED. If no change, returns CONA_OK.

	#define CONA_DEFAULT_FOLDER					0x00010000  //Used only with CONAInstallApplication
	#define CONA_INFORM_IF_USER_ACTION_NEEDED	0x00020000  //Used only with CONAInstallApplication
	#define CONA_WAIT_THAT_USER_ACTION_IS_DONE	0x00040000  //Used only with CONAInstallApplication

	#define CONA_USE_IF_NOTICATION 				0x01000000  //Used only with CONAReadFileInBlocks and CONAWriteFileInBlocks
	#define CONA_USE_CB_NOTICATION 				0x02000000  //Used only with CONAReadFileInBlocks and CONAWriteFileInBlocks
	#define CONA_NOT_SET_FILE_DETAILS 			0x04000000  //Used only with CONAReadFileInBlocks
	#define CONA_ALL_DATA_SENT 					0x08000000  //Used only with IFSAPIBlockNotify and CONABlockDataCallbackFunction
	#define	CONA_IS_ALL_DATA_RECEIVED(dwState)((dwState >> 27) & 0x01) //Used only with IFSAPIBlockNotify and CONABlockDataCallbackFunction

	// ----------------------------------------------------
	// Folder info structure
	typedef struct 
	{
		WCHAR*		pstrName;		// Folder or Drive name
		DWORD		dwAttributes;	// Folder or Drive type and permission 
		FILETIME	tFolderTime;	// Folder time
		WCHAR*		pstrLabel;		// Drive lable name 
		WCHAR*		pstrMemoryType;	// Folder or Drive memory type
	} CONAPI_FOLDER_INFO;

	// File info structure
	typedef struct 
	{
		WCHAR*		pstrName;		// File name
		DWORD		dwAttributes;	// File permission
		FILETIME	tFileTime;		// File modified time
		DWORD		dwFileSize;		// File size
		WCHAR*		pstrMIMEType;	// File MIME type
	} CONAPI_FILE_INFO;

	// Folder info structure
	typedef struct 
	{
		DWORD		dwSize;			  // Size of struct
		WCHAR*		pstrName;		  // Folder or Drive name
		WCHAR*		pstrLocation;	  // Absolute location path to folder or drive
		DWORD		dwAttributes;	  // Folder or Drive type and permission 
		FILETIME	tFolderTime;	  // Folder time
		WCHAR*		pstrLabel;		  // Drive lable name 
		WCHAR*		pstrMemoryType;	  // Folder or Drive memory type
		WCHAR*		pstrID;			  // Identification ID
		__int64		dlFreeMemory;	  // Free memory in drive
		__int64		dlTotalMemory;	  // Total memory in drive
		__int64		dlUsedMemory;	  // Used memory in drive
		DWORD		dwContainFiles;	  // Number of files in target folder or drive
		DWORD		dwContainFolders; // Number of folders in target folder or drive
		__int64		dlTotalSize;	  // Size of folder content (including content of subfolders)
		WCHAR*		pstrValue;		  // Reserved for future
	} CONAPI_FOLDER_INFO2;

	// Folder content structure
	typedef struct CONAPI_FOLDER_CONTENT
	{
		DWORD					dwSize;						// Size of struct
		CONAPI_FOLDER_INFO2*	pFolderInfo;				// Folder info struct
		WCHAR*					pstrPath;					// Absolute path of sub files and sub folders
		DWORD					dwNumberOfFileInfo;			// Number of file structs
		CONAPI_FILE_INFO*		pFileInfo;					// File structs
		DWORD					dwNumberOfSubFolderContent; // Number of file structs
		CONAPI_FOLDER_CONTENT*  pSubFolderContent;			// File structs
		CONAPI_FOLDER_CONTENT*	pParentFolder;				// Pointer to the parent folder content struct
		WCHAR*					pstrValue;					// Reserved for future	
	} CONAPI_FOLDER_CONTENT;

	typedef CONAPI_FOLDER_INFO* LPCONAPI_FOLDER_INFO;
	typedef CONAPI_FILE_INFO*   LPCONAPI_FILE_INFO;
	typedef CONAPI_FOLDER_INFO2* LPCONAPI_FOLDER_INFO2;
	typedef CONAPI_FOLDER_CONTENT* LPCONAPI_FOLDER_CONTENT;

	// ----------------------------------------------------
	#ifdef __cplusplus
		// ----------------------------------------------------
		//	IFSAPINotify
		//
		class IFSAPINotify
		{
		public:
			virtual DWORD OnFSOperationState(	DWORD dwFSFunction,
												DWORD dwState,
												DWORD dwTransferredBytes,
												DWORD dwAllBytes) = 0;
			virtual DWORD OnFSConnectionLost(DWORD dwError) = 0;
			// For future use. Following notification is not currently supported by API.
			virtual DWORD OnFSUpdated(DWORD, const WCHAR*, const WCHAR*, void*){return 0;}
		};
		// ----------------------------------------------------

		// ----------------------------------------------------
		//	IFSAPIBlockNotify
		//
		// Description
		//	IFSAPIBlockNotify interface is used with CONAWriteFileInBlocks and 
		//	CONAReadFileInBlocks functions. Connectivity API calls  
		//	OnFSOperationBlockData function when a new data block is needed or received.
		// 
		//	With CONAWriteFileInBlocks function: 
		//	Connectivity API request more data calling this notification functions 
		//	when more data is needed. 
		//	Connectivity API-user must fill file data buffer and set the actual data length. 
		//	Connectivity API-user doesn't need to fill whole data buffer. 
		//	When there is no more data left, Connectivity API-user must set 
		//	CONA_ALL_DATA_SENT value to pdwState parameter. Otherwise value must be zero.
		//	
		//	With CONAReadFileInBlocks function: 
		//	Connectivity API delivers file data blocks to Connectivity API-user calling this 
		//	notification function. Connectivity API-user MUST copy actual file data from buffer.
		//	Otherwise buffer will be overwrited with next data block.
		//	When there is no more data left, pdwState parameter includes CONA_ALL_DATA_SENT value.
		//	Connectivity API-user must use CONA_IS_ALL_DATA_RECEIVED macro for check is all data 
		//	received (if true, macro returns 1).
		//	Connectivity API can add more values to pdwState afterwards so Connectivity API-user should 
		//	always use CONA_IS_ALL_DATA_RECEIVED macro to check pdwState value!
		//
		//	NOTE: Connectivity API-user MUST NOT free data block buffer in the notification.
		//
		// Parameters:
		//	dwFSFunction				[in]		Ongoing function (CONAReadFileInBlocksNtf or CONAWriteFileInBlocksNtf)
		//	pdwState					[in,out]	Pointer to function state. Used to inform when all file data is sent/received.
		//	dwSizeOfFileDataBlockBuffer	[in]		The size of file data block buffer.
		//	pdwFileDataBlockLenght		[in, out]	CONAReadFileInBlocks:	In: Actual file data lenght in buffer. 
		//											CONAWriteFileInBlocks:	Out: Actual file data length in buffer.
		//	pFileDataBlock				[in,out]	Pointer to file data block buffer.
		//		
		// Return values
		// The Connectivity API-user must return the CONA_OK value. If the callback 
		// function returns the error code ECONA_CANCELLED to the Connectivity API, 
		// the function will be cancelled with the error code ECONA_CANCELLED.
		//
		// Type definition: 
		class IFSAPIBlockNotify
		{
		public:
			virtual DWORD OnFSOperationBlockData(	DWORD dwFSFunction,
													DWORD *pdwState,
													const DWORD dwSizeOfFileDataBlockBuffer,
													DWORD *pdwFileDataBlockLenght,
													unsigned char* pFileDataBlock) = 0;
		};
		// ----------------------------------------------------
	#endif
	// ----------------------------------------------------

	// ----------------------------------------------------
	// FileOperationCallbackFunction function:
	//
	// Callback function prototype:
	//	DWORD CALLBACK FileOperationCallbackFunction(
	//							   DWORD dwFSFunction, 
	//							   DWORD dwState,
	//							   DWORD dwTransferredBytes,
	//							   DWORD dwAllBytes);
	//
		typedef DWORD (CALLBACK *PFN_CONA_FS_CALLBACK)(
									DWORD dwFSFunction,
									DWORD dwState,
									DWORD dwTransferredBytes,
									DWORD dwAllBytes );
	// ----------------------------------------------------

	// ----------------------------------------------------
	// CONAGetFolderInfoCallbackFunction function:
	//
	// Callback function prototype:
	//	DWORD CALLBACK CONAGetFolderInfoCallbackFunction(
	//							   LPCONAPI_FOLDER_CONTENT pFolderContent);
	//
		typedef DWORD (CALLBACK *PFN_CONA_FS_FOLDERINFO_CALLBACK)(
										LPCONAPI_FOLDER_INFO2 pFolderInfo);
	// ----------------------------------------------------

	// ----------------------------------------------------
	// CONABlockDataCallbackFunction function:
	//
	// Callback function prototype:
	//	DWORD CALLBACK CONABlockDataCallbackFunction(
	//							   DWORD dwFSFunction,
	//							   DWORD *pdwState,
	//							   const DWORD dwSizeOfFileDataBlockBuffer,
	//							   DWORD *pdwFileDataBlockLenght,
	//							   unsigned char* pFileDataBlock);
	//
	// Description
	//	See IFSAPIBlockNotify interface description for more information about parameters.
	//
		typedef DWORD (CALLBACK *PFN_CONA_FS_BLOCKDATA_CALLBACK)(
									DWORD dwFSFunction,
									DWORD *pdwState,
									const DWORD dwSizeOfFileDataBlockBuffer,
									DWORD *pdwFileDataBlockLenght,
									unsigned char* pFileDataBlock);
	// ----------------------------------------------------

	// FSFunction values:
	#define CONARefreshDeviceMemoryValuesNtf	0x00000001
	#define CONASetCurrentFolderNtf				0x00000002
	#define CONAFindBeginNtf					0x00000004
	#define CONACreateFolderNtf					0x00000008
	#define CONADeleteFolderNtf					0x00000010
	#define CONARenameFolderNtf					0x00000020
	#define CONAGetFileInfoNtf					0x00000040
	#define CONADeleteFileNtf					0x00000080
	#define CONAMoveFileNtf						0x00000100
	#define CONACopyFileNtf						0x00000200
	#define CONARenameFileNtf					0x00000400
	#define CONAReadFileNtf						0x00000800
	#define CONAWriteFileNtf					0x00001000
	#define CONAConnectionLostNtf 				0x00002000
	#define CONAInstallApplicationNtf			0x00004000
	#define CONAConvertFileNtf					0x00008000
	#define CONAGetFolderInfoNtf				0x00010000
	#define CONAListApplicationNtf				0x00020000
	#define CONAUninstallApplicationNtf			0x00040000
	#define CONAReadFileInBlocksNtf				0x00080000
	#define CONAWriteFileInBlocksNtf			0x00100000
	#define CONAMoveFolderNtf					0x00200000
	#define CONACopyFolderNtf					0x00400000
	#define CONAGetFileMetadataNtf				0x00800000
		
	// The next function do not send notifications:
	//	CONAOpenFS					
	//	CONACloseFS				
	//	CONARegisterFSNotifyCallback
	//	CONAGetMemoryTypes 			
	//	CONAGetMemoryValues			
	//	CONAGetCurrentFolder	
	//	CONAFindNextFolder		
	//	CONAFindNextFile		
	//	CONAFindEnd					
	//	CONACancel				

	// Possible error codes value in dwStatus parameter when 
	// FSFunction value is CONAConnectionLost:
	//	ECONA_CONNECTION_LOST
	//  ECONA_CONNECTION_REMOVED
	//  ECONA_CONNECTION_FAILED
	//  ECONA_SUSPEND
	// ----------------------------------------------------

//=========================================================

/////////////////////////////////////////////////////////////////////
// File System API Application Installation definitions
/////////////////////////////////////////////////////////////////////

	//The next define values used to define which type of struct is used:
	#define CONA_APPLICATION_TYPE_SIS		0x00000001 // Use when struct type is CONAPI_APPLICATION_SIS
	#define CONA_APPLICATION_TYPE_JAVA		0x00000002 // Use when struct type is CONAPI_APPLICATION_JAVA
	#define CONA_APPLICATION_TYPE_THEMES	0x00000004 // Use when struct type is CONAPI_APPLICATION_THEMES
	#define CONA_APPLICATION_TYPE_UNKNOWN	0x00000008 // Use only in CONAPI_APPLICATION_INFO struct, unknown application type.
	#define CONA_DEFAULT_UNINSTALLATION		0x00000100 // Default uninstallation, used with CONAUninstallApplication function.
	#define CONA_SILENT_UNINSTALLATION		0x00000200 // Silent uninstallation, used with CONAUninstallApplication function.
	#define CONA_LIST_ALL_APPICATIONS		0x00001000 // List all installed applications, used with CONAListApplications function
	#define CONA_LIST_JAVA_APPICATIONS		0x00002000 // List all installed java applications, used with CONAListApplications function
	#define CONA_LIST_THEMES_APPICATIONS	0x00004000 // List all installed themes, used with CONAListApplications function

	//The struct for sis applications
	typedef struct
	{
		WCHAR*	pstrFileNameSis; // sis application File name, must be set.
								 // The value can also include the file path in PC.
	} CONAPI_APPLICATION_SIS;

	//The struct for java applications
	typedef struct
	{
		WCHAR*	pstrFileNameJad;	// File name of the jad file. The value can also include the file path in PC. 
									// If jad file is not available, the value must be NULL.
		WCHAR*	pstrFileNameJar;	// File name of the jar file, must be set. 
									// The value can also include the file path in PC. 
	} CONAPI_APPLICATION_JAVA;

	//The struct for themes file
	typedef struct
	{
		WCHAR*	pstrFileName;		// File name of the nth file, must be set
									// The value can also include the file path in PC.
		DWORD	dwOptions;			// Reserved for future use, the value must be 0.
	} CONAPI_APPLICATION_FILE;

	//The struct for application info, used with CONAListApplications function
	typedef struct
	{
		DWORD   dwSize;				// size of struct in bytes.
		WCHAR*	pstrName;			// Application name. Always exist.
		WCHAR*	pstrDescription;	// Application description. If not available, pointer is NULL.
		WCHAR*  pstrVendor;			// Application vendor. If not available, pointer is NULL.
		WCHAR*	pstrVersion;		// Application version. If not available, pointer is NULL.
		WCHAR*	pstrParentAppNam;	// Parent application name. This is available if application is augmentation 
									// for some other application. 
		DWORD	dwApplicationSize;	// Application size in bytes. If not available, value is -1 (0xFFFFFFFF).
		DWORD	dwApplicationType;	// Application type possible values:
									//		CONA_APPLICATION_TYPE_SIS		:	Sis application
									//		CONA_APPLICATION_TYPE_JAVA		:	Java application
									//		CONA_APPLICATION_TYPE_THEMES	:	Themes application	
									//		CONA_APPLICATION_TYPE_UNKNOWN	:	Application type is not available
		WCHAR*	pstrUID;			// Application UID string, used with CONAUninstallApplication function.
		DWORD   dwOptions;			// Reserved for future use. Value is zero.
		WCHAR*  pstrValue;			// Reserved for future use. Pointer is NULL.
	} CONAPI_APPLICATION_INFO;
	
/////////////////////////////////////////////////////////////////////
// File System API File Conversion definitions
/////////////////////////////////////////////////////////////////////

	//	The CONAPI_CONVERT_FILE_TYPE structure
	typedef struct
	{
		WCHAR*	pstrFileExtension;			// Input file extension, which can be converted on device. E.g "ewd"
		WCHAR*	pstrFileMIMEType;			// Input file MIME type, which can be converted on device. E.g "x-epoc/x-app268450404"
		WCHAR*	pstrOutputFileExtension;	// Output file extension. E.g "doc"
	} CONAPI_CONVERT_FILE_TYPE;

#endif
