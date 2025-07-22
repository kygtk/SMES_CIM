# Microsoft Developer Studio Project File - Name="LIB" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=LIB - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LIB.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LIB.mak" CFG="LIB - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LIB - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "LIB - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LIB - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "LIB - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"D:\FPDCIM\Bin\SysLib.lib"

!ENDIF 

# Begin Target

# Name "LIB - Win32 Release"
# Name "LIB - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Queue.cpp
# End Source File
# Begin Source File

SOURCE=.\Script.cpp
# End Source File
# Begin Source File

SOURCE=.\SharedMem.cpp
# End Source File
# Begin Source File

SOURCE=.\SysLib.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Script.h
# End Source File
# Begin Source File

SOURCE=.\SW3FALib.h
# End Source File
# Begin Source File

SOURCE=.\SysLib.h
# End Source File
# End Group
# End Target
# End Project
# Section LIB : {59C5E42F-B7E4-4FEA-98ED-8630FBF009B7}
# 	2:11:XmlHandle.h:XmlHandle.h
# 	2:13:XmlHandle.cpp:XmlHandle.cpp
# 	2:17:CLASS: CXmlHandle:CXmlHandle
# 	2:19:Application Include:LIB.H
# End Section
# Section LIB : {24750C10-6F21-495B-99C6-0481A3D03359}
# 	2:14:CLASS: CScript:CScript
# 	2:10:Script.cpp:Script.cpp
# 	2:19:Application Include:LIB.H
# 	2:8:Script.h:Script.h
# End Section
# Section LIB : {9E14AFA0-7726-4D5D-9BFE-714E99F58789}
# 	2:13:SharedMem.cpp:SharedMem.cpp
# 	2:13:CLASS: CShMem:CShMem
# 	2:13:CLASS: CQueue:CQueue
# 	2:10:SW3FALib.h:SW3FALib.h
# End Section
# Section LIB : {58DDA3FF-E695-46E5-8677-7C6F609BA8BE}
# 	2:13:CLASS: CShMem:CShMem
# 	2:13:CLASS: CQueue:CQueue
# 	2:10:SW3FALib.h:SW3FALib1.h
# 	2:9:Queue.cpp:Queue.cpp
# End Section
# Section LIB : {B5F9BF22-7E74-427B-AEEE-B95D26858D4A}
# 	2:8:SysLib.h:SysLib.h
# 	2:10:SysLib.cpp:SysLib.cpp
# 	2:13:CLASS: SysLib:SysLib
# End Section
