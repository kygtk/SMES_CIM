# Microsoft Developer Studio Generated NMAKE File, Based on GUIDLL.dsp
!IF "$(CFG)" == ""
CFG=GUIDLL - Win32 Debug
!MESSAGE No configuration specified. Defaulting to GUIDLL - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "GUIDLL - Win32 Release" && "$(CFG)" != "GUIDLL - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GUIDLL.mak" CFG="GUIDLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GUIDLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "GUIDLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GUIDLL - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\..\T8Line\Bin\GUIDLL.dll"


CLEAN :
	-@erase "$(INTDIR)\GUIDLL.obj"
	-@erase "$(INTDIR)\LayoutConfig.obj"
	-@erase "$(INTDIR)\QueueHandle.obj"
	-@erase "$(INTDIR)\RecipeInfo.obj"
	-@erase "$(INTDIR)\SmaHandle.obj"
	-@erase "$(INTDIR)\SystemDataInfo.obj"
	-@erase "$(INTDIR)\SystemRunInfo.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\GUIDLL.exp"
	-@erase "$(OUTDIR)\GUIDLL.lib"
	-@erase "$(OUTDIR)\GUIDLL.pdb"
	-@erase "..\..\..\T8Line\Bin\GUIDLL.dll"
	-@erase "..\..\..\T8Line\Bin\GUIDLL.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Gz /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GUIDLL_EXPORTS" /Fp"$(INTDIR)\GUIDLL.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\GUIDLL.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=D:\T8Line\Bin\SysLib.lib D:\T8Line\Bin\SW3FaLib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\GUIDLL.pdb" /debug /debugtype:both /machine:I386 /def:".\GUIDLL.def" /out:"D:\T8Line\Bin\GUIDLL.dll" /implib:"$(OUTDIR)\GUIDLL.lib" 
DEF_FILE= \
	".\GUIDLL.def"
LINK32_OBJS= \
	"$(INTDIR)\GUIDLL.obj" \
	"$(INTDIR)\LayoutConfig.obj" \
	"$(INTDIR)\QueueHandle.obj" \
	"$(INTDIR)\RecipeInfo.obj" \
	"$(INTDIR)\SmaHandle.obj" \
	"$(INTDIR)\SystemDataInfo.obj" \
	"$(INTDIR)\SystemRunInfo.obj"

"..\..\..\T8Line\Bin\GUIDLL.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "GUIDLL - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "..\..\Bin\GUIDLL.dll" "$(OUTDIR)\GUIDLL.bsc"


CLEAN :
	-@erase "$(INTDIR)\GUIDLL.obj"
	-@erase "$(INTDIR)\GUIDLL.sbr"
	-@erase "$(INTDIR)\LayoutConfig.obj"
	-@erase "$(INTDIR)\LayoutConfig.sbr"
	-@erase "$(INTDIR)\QueueHandle.obj"
	-@erase "$(INTDIR)\QueueHandle.sbr"
	-@erase "$(INTDIR)\RecipeInfo.obj"
	-@erase "$(INTDIR)\RecipeInfo.sbr"
	-@erase "$(INTDIR)\SmaHandle.obj"
	-@erase "$(INTDIR)\SmaHandle.sbr"
	-@erase "$(INTDIR)\SystemDataInfo.obj"
	-@erase "$(INTDIR)\SystemDataInfo.sbr"
	-@erase "$(INTDIR)\SystemRunInfo.obj"
	-@erase "$(INTDIR)\SystemRunInfo.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\GUIDLL.bsc"
	-@erase "$(OUTDIR)\GUIDLL.exp"
	-@erase "$(OUTDIR)\GUIDLL.lib"
	-@erase "$(OUTDIR)\GUIDLL.map"
	-@erase "$(OUTDIR)\GUIDLL.pdb"
	-@erase "..\..\Bin\GUIDLL.dll"
	-@erase "..\..\Bin\GUIDLL.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /Gz /MTd /W3 /Gm /Gi /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GUIDLL_EXPORTS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\GUIDLL.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\GUIDLL.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\GUIDLL.sbr" \
	"$(INTDIR)\LayoutConfig.sbr" \
	"$(INTDIR)\QueueHandle.sbr" \
	"$(INTDIR)\RecipeInfo.sbr" \
	"$(INTDIR)\SmaHandle.sbr" \
	"$(INTDIR)\SystemDataInfo.sbr" \
	"$(INTDIR)\SystemRunInfo.sbr"

"$(OUTDIR)\GUIDLL.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib D:/FPDCIM/Bin/SysLib.lib D:/FPDCIM/Bin/SW3FaLib.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\GUIDLL.pdb" /map:"$(INTDIR)\GUIDLL.map" /debug /machine:I386 /nodefaultlib:"LIBCD.lib" /def:".\GUIDLL.def" /out:"D:/FPDCIM/Bin/GUIDLL.dll" /implib:"$(OUTDIR)\GUIDLL.lib" /pdbtype:sept /libpath:"/EXPORTS" 
DEF_FILE= \
	".\GUIDLL.def"
LINK32_OBJS= \
	"$(INTDIR)\GUIDLL.obj" \
	"$(INTDIR)\LayoutConfig.obj" \
	"$(INTDIR)\QueueHandle.obj" \
	"$(INTDIR)\RecipeInfo.obj" \
	"$(INTDIR)\SmaHandle.obj" \
	"$(INTDIR)\SystemDataInfo.obj" \
	"$(INTDIR)\SystemRunInfo.obj"

"..\..\Bin\GUIDLL.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("GUIDLL.dep")
!INCLUDE "GUIDLL.dep"
!ELSE 
!MESSAGE Warning: cannot find "GUIDLL.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "GUIDLL - Win32 Release" || "$(CFG)" == "GUIDLL - Win32 Debug"
SOURCE=.\GUIDLL.cpp

!IF  "$(CFG)" == "GUIDLL - Win32 Release"


"$(INTDIR)\GUIDLL.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GUIDLL - Win32 Debug"


"$(INTDIR)\GUIDLL.obj"	"$(INTDIR)\GUIDLL.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\LayoutConfig.cpp

!IF  "$(CFG)" == "GUIDLL - Win32 Release"


"$(INTDIR)\LayoutConfig.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GUIDLL - Win32 Debug"


"$(INTDIR)\LayoutConfig.obj"	"$(INTDIR)\LayoutConfig.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\QueueHandle.cpp

!IF  "$(CFG)" == "GUIDLL - Win32 Release"


"$(INTDIR)\QueueHandle.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GUIDLL - Win32 Debug"


"$(INTDIR)\QueueHandle.obj"	"$(INTDIR)\QueueHandle.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\RecipeInfo.cpp

!IF  "$(CFG)" == "GUIDLL - Win32 Release"


"$(INTDIR)\RecipeInfo.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GUIDLL - Win32 Debug"


"$(INTDIR)\RecipeInfo.obj"	"$(INTDIR)\RecipeInfo.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\SmaHandle.cpp

!IF  "$(CFG)" == "GUIDLL - Win32 Release"


"$(INTDIR)\SmaHandle.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GUIDLL - Win32 Debug"


"$(INTDIR)\SmaHandle.obj"	"$(INTDIR)\SmaHandle.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\SystemDataInfo.cpp

!IF  "$(CFG)" == "GUIDLL - Win32 Release"


"$(INTDIR)\SystemDataInfo.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GUIDLL - Win32 Debug"


"$(INTDIR)\SystemDataInfo.obj"	"$(INTDIR)\SystemDataInfo.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\SystemRunInfo.cpp

!IF  "$(CFG)" == "GUIDLL - Win32 Release"


"$(INTDIR)\SystemRunInfo.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "GUIDLL - Win32 Debug"


"$(INTDIR)\SystemRunInfo.obj"	"$(INTDIR)\SystemRunInfo.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

