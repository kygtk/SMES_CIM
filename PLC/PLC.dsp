# Microsoft Developer Studio Project File - Name="PLC" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=PLC - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PLC.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PLC.mak" CFG="PLC - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PLC - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 D:\FPDCIM\Bin\SysLib.lib D:\FPDCIM\Bin\mdFunc32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /stack:0xa00000 /subsystem:windows /debug /machine:I386 /out:"D:\FPDCIM\Down\CIMPLC.EXE" /pdbtype:sept
# Begin Target

# Name "PLC - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BaseModule.cpp
# End Source File
# Begin Source File

SOURCE=.\MainPlc.cpp
# End Source File
# Begin Source File

SOURCE=.\Melsec.cpp
# End Source File
# Begin Source File

SOURCE=.\MyLog.Cpp
# End Source File
# Begin Source File

SOURCE=.\PLC_Version.cpp
# End Source File
# Begin Source File

SOURCE=.\Thread.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeCheck.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BaseModule.h
# End Source File
# Begin Source File

SOURCE=..\INCLUDE\EventSignal.h
# End Source File
# Begin Source File

SOURCE=.\MainPlc.h
# End Source File
# Begin Source File

SOURCE=.\Map_Common.h
# End Source File
# Begin Source File

SOURCE=.\Map_PhotoInline.h
# End Source File
# Begin Source File

SOURCE=.\Melsec.h
# End Source File
# Begin Source File

SOURCE=.\MyLog.h
# End Source File
# Begin Source File

SOURCE=.\PLC_Version.h
# End Source File
# Begin Source File

SOURCE=.\Thread.h
# End Source File
# Begin Source File

SOURCE=.\TimeCheck.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\HisTory.txt
# End Source File
# End Target
# End Project
