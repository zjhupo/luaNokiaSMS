# Microsoft Developer Studio Project File - Name="nokiasdk" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=nokiasdk - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "nokiasdk.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "nokiasdk.mak" CFG="nokiasdk - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "nokiasdk - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "nokiasdk - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "nokiasdk - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "D:\Program Files\Lua\5.1\include" /I "D:\Program Files\Lua\5.1\lib" /I "API Headers" /I "API Headers\Common Functions API" /I "e:\Program Files\Lua\5.1\lib" /I "API Headers\Content Access API" /I "API Headers\Device Management API" /I "e:\Program Files\Lua\5.1\include" /I "API Headers\File System API" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_WINDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"Release/luaNokiaSMS.dll"

!ELSEIF  "$(CFG)" == "nokiasdk - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "API Headers" /I "API Headers\Common Functions API" /I "API Headers\Content Access API" /I "API Headers\Device Management API" /I "API Headers\File System API" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "nokiasdk - Win32 Release"
# Name "nokiasdk - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\nokiasdk.cpp
# End Source File
# Begin Source File

SOURCE=.\nokiasdk.def
# End Source File
# Begin Source File

SOURCE=.\nokiasdk.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=".\API Headers\Content Access API\CADataDefinitions.h"
# End Source File
# Begin Source File

SOURCE=".\API Headers\Common Functions API\CommonFunctionsAPI.h"
# End Source File
# Begin Source File

SOURCE=".\API Headers\File System API\CONAApplicationInstallation.h"
# End Source File
# Begin Source File

SOURCE=".\API Headers\Device Management API\CONADeviceManagement.h"
# End Source File
# Begin Source File

SOURCE=".\API Headers\File System API\CONAFileConversion.h"
# End Source File
# Begin Source File

SOURCE=".\API Headers\File System API\CONAFileSystem.h"
# End Source File
# Begin Source File

SOURCE=".\API Headers\Content Access API\DAContentAccess.h"
# End Source File
# Begin Source File

SOURCE=".\API Headers\Content Access API\DAContentAccessDefinitions.h"
# End Source File
# Begin Source File

SOURCE=".\API Headers\Device Management API\DMAPIDefinitions.h"
# End Source File
# Begin Source File

SOURCE=".\API Headers\File System API\FSAPIDefinitions.h"
# End Source File
# Begin Source File

SOURCE=.\nokiasdk.h
# End Source File
# Begin Source File

SOURCE=".\API Headers\PCCSErrors.h"
# End Source File
# Begin Source File

SOURCE=".\API Headers\PCCSTypeDefinitions.h"
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\nokiasdk.rc2
# End Source File
# End Group
# Begin Group "NokiaSDK"

# PROP Default_Filter "*.lib"
# Begin Source File

SOURCE=".\API Headers\Common Functions API\CommonFunctionsAPI.lib"
# End Source File
# Begin Source File

SOURCE=".\API Headers\Content Access API\ContentAccessAPI.lib"
# End Source File
# Begin Source File

SOURCE=".\API Headers\Device Management API\DeviceManagementAPI.lib"
# End Source File
# Begin Source File

SOURCE=".\API Headers\File System API\FileSystemAPI.lib"
# End Source File
# Begin Source File

SOURCE="d:\Program Files\Lua\5.1\lib\lua51.lib"
# End Source File
# End Group
# End Target
# End Project
