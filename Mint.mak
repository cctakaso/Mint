# Microsoft Developer Studio Generated NMAKE File, Based on Mint.dsp
!IF "$(CFG)" == ""
CFG=Mint - Win32 Free Debug
!MESSAGE 構成が指定されていません。ﾃﾞﾌｫﾙﾄの Mint - Win32 Free Debug を設定します。
!ENDIF 

!IF "$(CFG)" != "Mint - Win32 Release" && "$(CFG)" != "Mint - Win32 Debug" &&\
 "$(CFG)" != "Mint - Win32 Verify" && "$(CFG)" != "Mint - Win32 Free Release" &&\
 "$(CFG)" != "Mint - Win32 Free Debug"
!MESSAGE 指定された ﾋﾞﾙﾄﾞ ﾓｰﾄﾞ "$(CFG)" は正しくありません。
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "Mint.mak" CFG="Mint - Win32 Free Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "Mint - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "Mint - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE "Mint - Win32 Verify" ("Win32 (x86) Application" 用)
!MESSAGE "Mint - Win32 Free Release" ("Win32 (x86) Application" 用)
!MESSAGE "Mint - Win32 Free Debug" ("Win32 (x86) Application" 用)
!MESSAGE 
!ERROR 無効な構成が指定されています。
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Mint - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Mint.exe"

!ELSE 

ALL : "$(OUTDIR)\Mint.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\action.obj"
	-@erase "$(INTDIR)\chara.obj"
	-@erase "$(INTDIR)\ConfigEdit.obj"
	-@erase "$(INTDIR)\ConfigSheet.obj"
	-@erase "$(INTDIR)\CREJIT.OBJ"
	-@erase "$(INTDIR)\DateTime.obj"
	-@erase "$(INTDIR)\DispPage.obj"
	-@erase "$(INTDIR)\DocTempl.obj"
	-@erase "$(INTDIR)\EnvEdit.obj"
	-@erase "$(INTDIR)\FilePage.obj"
	-@erase "$(INTDIR)\FileSelectBar.obj"
	-@erase "$(INTDIR)\FindDlg.obj"
	-@erase "$(INTDIR)\FormatCharPage.obj"
	-@erase "$(INTDIR)\FormatPage.obj"
	-@erase "$(INTDIR)\Hint.obj"
	-@erase "$(INTDIR)\Ime.obj"
	-@erase "$(INTDIR)\JumpDlg.obj"
	-@erase "$(INTDIR)\KeyGroupDlg.obj"
	-@erase "$(INTDIR)\Keyword.obj"
	-@erase "$(INTDIR)\KeywordPage.obj"
	-@erase "$(INTDIR)\LineHeader.obj"
	-@erase "$(INTDIR)\ListCtrEx.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\Mint.pch"
	-@erase "$(INTDIR)\ObjCash.obj"
	-@erase "$(INTDIR)\ObjLay.obj"
	-@erase "$(INTDIR)\Profile.obj"
	-@erase "$(INTDIR)\Sell.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Tanbun.obj"
	-@erase "$(INTDIR)\TanbunAddDlg.obj"
	-@erase "$(INTDIR)\TanbunEdit.obj"
	-@erase "$(INTDIR)\TextCash.obj"
	-@erase "$(INTDIR)\The.obj"
	-@erase "$(INTDIR)\The.res"
	-@erase "$(INTDIR)\TheDoc.obj"
	-@erase "$(INTDIR)\TheView.obj"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\ViewEd.obj"
	-@erase "$(INTDIR)\ViewEnv.obj"
	-@erase "$(OUTDIR)\Mint.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_DBCS" /D "SHAR" /Fp"$(INTDIR)\Mint.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x411 /fo"$(INTDIR)\The.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Mint.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=imm32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\Mint.pdb" /machine:I386 /out:"$(OUTDIR)\Mint.exe" 
LINK32_OBJS= \
	"$(INTDIR)\action.obj" \
	"$(INTDIR)\chara.obj" \
	"$(INTDIR)\ConfigEdit.obj" \
	"$(INTDIR)\ConfigSheet.obj" \
	"$(INTDIR)\CREJIT.OBJ" \
	"$(INTDIR)\DateTime.obj" \
	"$(INTDIR)\DispPage.obj" \
	"$(INTDIR)\DocTempl.obj" \
	"$(INTDIR)\EnvEdit.obj" \
	"$(INTDIR)\FilePage.obj" \
	"$(INTDIR)\FileSelectBar.obj" \
	"$(INTDIR)\FindDlg.obj" \
	"$(INTDIR)\FormatCharPage.obj" \
	"$(INTDIR)\FormatPage.obj" \
	"$(INTDIR)\Hint.obj" \
	"$(INTDIR)\Ime.obj" \
	"$(INTDIR)\JumpDlg.obj" \
	"$(INTDIR)\KeyGroupDlg.obj" \
	"$(INTDIR)\Keyword.obj" \
	"$(INTDIR)\KeywordPage.obj" \
	"$(INTDIR)\LineHeader.obj" \
	"$(INTDIR)\ListCtrEx.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\ObjCash.obj" \
	"$(INTDIR)\ObjLay.obj" \
	"$(INTDIR)\Profile.obj" \
	"$(INTDIR)\Sell.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Tanbun.obj" \
	"$(INTDIR)\TanbunAddDlg.obj" \
	"$(INTDIR)\TanbunEdit.obj" \
	"$(INTDIR)\TextCash.obj" \
	"$(INTDIR)\The.obj" \
	"$(INTDIR)\The.res" \
	"$(INTDIR)\TheDoc.obj" \
	"$(INTDIR)\TheView.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\ViewEd.obj" \
	"$(INTDIR)\ViewEnv.obj"

"$(OUTDIR)\Mint.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Mint.exe"

!ELSE 

ALL : "$(OUTDIR)\Mint.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\action.obj"
	-@erase "$(INTDIR)\chara.obj"
	-@erase "$(INTDIR)\ConfigEdit.obj"
	-@erase "$(INTDIR)\ConfigSheet.obj"
	-@erase "$(INTDIR)\CREJIT.OBJ"
	-@erase "$(INTDIR)\DateTime.obj"
	-@erase "$(INTDIR)\DispPage.obj"
	-@erase "$(INTDIR)\DocTempl.obj"
	-@erase "$(INTDIR)\EnvEdit.obj"
	-@erase "$(INTDIR)\FilePage.obj"
	-@erase "$(INTDIR)\FileSelectBar.obj"
	-@erase "$(INTDIR)\FindDlg.obj"
	-@erase "$(INTDIR)\FormatCharPage.obj"
	-@erase "$(INTDIR)\FormatPage.obj"
	-@erase "$(INTDIR)\Hint.obj"
	-@erase "$(INTDIR)\Ime.obj"
	-@erase "$(INTDIR)\JumpDlg.obj"
	-@erase "$(INTDIR)\KeyGroupDlg.obj"
	-@erase "$(INTDIR)\Keyword.obj"
	-@erase "$(INTDIR)\KeywordPage.obj"
	-@erase "$(INTDIR)\LineHeader.obj"
	-@erase "$(INTDIR)\ListCtrEx.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\Mint.pch"
	-@erase "$(INTDIR)\ObjCash.obj"
	-@erase "$(INTDIR)\ObjLay.obj"
	-@erase "$(INTDIR)\Profile.obj"
	-@erase "$(INTDIR)\Sell.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Tanbun.obj"
	-@erase "$(INTDIR)\TanbunAddDlg.obj"
	-@erase "$(INTDIR)\TanbunEdit.obj"
	-@erase "$(INTDIR)\TextCash.obj"
	-@erase "$(INTDIR)\The.obj"
	-@erase "$(INTDIR)\The.res"
	-@erase "$(INTDIR)\TheDoc.obj"
	-@erase "$(INTDIR)\TheView.obj"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\ViewEd.obj"
	-@erase "$(INTDIR)\ViewEnv.obj"
	-@erase "$(OUTDIR)\Mint.exe"
	-@erase "$(OUTDIR)\Mint.ilk"
	-@erase "$(OUTDIR)\Mint.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "_VERIFY" /D "WIN32"\
 /D "_WINDOWS" /D "_DBCS" /D "SHAR" /Fp"$(INTDIR)\Mint.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x411 /fo"$(INTDIR)\The.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Mint.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=imm32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\Mint.pdb" /debug /machine:I386 /out:"$(OUTDIR)\Mint.exe"\
 /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\action.obj" \
	"$(INTDIR)\chara.obj" \
	"$(INTDIR)\ConfigEdit.obj" \
	"$(INTDIR)\ConfigSheet.obj" \
	"$(INTDIR)\CREJIT.OBJ" \
	"$(INTDIR)\DateTime.obj" \
	"$(INTDIR)\DispPage.obj" \
	"$(INTDIR)\DocTempl.obj" \
	"$(INTDIR)\EnvEdit.obj" \
	"$(INTDIR)\FilePage.obj" \
	"$(INTDIR)\FileSelectBar.obj" \
	"$(INTDIR)\FindDlg.obj" \
	"$(INTDIR)\FormatCharPage.obj" \
	"$(INTDIR)\FormatPage.obj" \
	"$(INTDIR)\Hint.obj" \
	"$(INTDIR)\Ime.obj" \
	"$(INTDIR)\JumpDlg.obj" \
	"$(INTDIR)\KeyGroupDlg.obj" \
	"$(INTDIR)\Keyword.obj" \
	"$(INTDIR)\KeywordPage.obj" \
	"$(INTDIR)\LineHeader.obj" \
	"$(INTDIR)\ListCtrEx.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\ObjCash.obj" \
	"$(INTDIR)\ObjLay.obj" \
	"$(INTDIR)\Profile.obj" \
	"$(INTDIR)\Sell.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Tanbun.obj" \
	"$(INTDIR)\TanbunAddDlg.obj" \
	"$(INTDIR)\TanbunEdit.obj" \
	"$(INTDIR)\TextCash.obj" \
	"$(INTDIR)\The.obj" \
	"$(INTDIR)\The.res" \
	"$(INTDIR)\TheDoc.obj" \
	"$(INTDIR)\TheView.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\ViewEd.obj" \
	"$(INTDIR)\ViewEnv.obj"

"$(OUTDIR)\Mint.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

OUTDIR=.\Verify
INTDIR=.\Verify
# Begin Custom Macros
OutDir=.\Verify
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Mint.exe"

!ELSE 

ALL : "$(OUTDIR)\Mint.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\action.obj"
	-@erase "$(INTDIR)\chara.obj"
	-@erase "$(INTDIR)\ConfigEdit.obj"
	-@erase "$(INTDIR)\ConfigSheet.obj"
	-@erase "$(INTDIR)\CREJIT.OBJ"
	-@erase "$(INTDIR)\DateTime.obj"
	-@erase "$(INTDIR)\DispPage.obj"
	-@erase "$(INTDIR)\DocTempl.obj"
	-@erase "$(INTDIR)\EnvEdit.obj"
	-@erase "$(INTDIR)\FilePage.obj"
	-@erase "$(INTDIR)\FileSelectBar.obj"
	-@erase "$(INTDIR)\FindDlg.obj"
	-@erase "$(INTDIR)\FormatCharPage.obj"
	-@erase "$(INTDIR)\FormatPage.obj"
	-@erase "$(INTDIR)\Hint.obj"
	-@erase "$(INTDIR)\Ime.obj"
	-@erase "$(INTDIR)\JumpDlg.obj"
	-@erase "$(INTDIR)\KeyGroupDlg.obj"
	-@erase "$(INTDIR)\Keyword.obj"
	-@erase "$(INTDIR)\KeywordPage.obj"
	-@erase "$(INTDIR)\LineHeader.obj"
	-@erase "$(INTDIR)\ListCtrEx.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\Mint.pch"
	-@erase "$(INTDIR)\ObjCash.obj"
	-@erase "$(INTDIR)\ObjLay.obj"
	-@erase "$(INTDIR)\Profile.obj"
	-@erase "$(INTDIR)\Sell.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Tanbun.obj"
	-@erase "$(INTDIR)\TanbunAddDlg.obj"
	-@erase "$(INTDIR)\TanbunEdit.obj"
	-@erase "$(INTDIR)\TextCash.obj"
	-@erase "$(INTDIR)\The.obj"
	-@erase "$(INTDIR)\The.res"
	-@erase "$(INTDIR)\TheDoc.obj"
	-@erase "$(INTDIR)\TheView.obj"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\ViewEd.obj"
	-@erase "$(INTDIR)\ViewEnv.obj"
	-@erase "$(OUTDIR)\Mint.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O1 /D "NDEBUG" /D "_VERIFY" /D "WIN32" /D\
 "_WINDOWS" /D "_DBCS" /D "SHAR" /Fp"$(INTDIR)\Mint.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Verify/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x411 /fo"$(INTDIR)\The.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Mint.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=imm32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\Mint.pdb" /machine:I386 /out:"$(OUTDIR)\Mint.exe" 
LINK32_OBJS= \
	"$(INTDIR)\action.obj" \
	"$(INTDIR)\chara.obj" \
	"$(INTDIR)\ConfigEdit.obj" \
	"$(INTDIR)\ConfigSheet.obj" \
	"$(INTDIR)\CREJIT.OBJ" \
	"$(INTDIR)\DateTime.obj" \
	"$(INTDIR)\DispPage.obj" \
	"$(INTDIR)\DocTempl.obj" \
	"$(INTDIR)\EnvEdit.obj" \
	"$(INTDIR)\FilePage.obj" \
	"$(INTDIR)\FileSelectBar.obj" \
	"$(INTDIR)\FindDlg.obj" \
	"$(INTDIR)\FormatCharPage.obj" \
	"$(INTDIR)\FormatPage.obj" \
	"$(INTDIR)\Hint.obj" \
	"$(INTDIR)\Ime.obj" \
	"$(INTDIR)\JumpDlg.obj" \
	"$(INTDIR)\KeyGroupDlg.obj" \
	"$(INTDIR)\Keyword.obj" \
	"$(INTDIR)\KeywordPage.obj" \
	"$(INTDIR)\LineHeader.obj" \
	"$(INTDIR)\ListCtrEx.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\ObjCash.obj" \
	"$(INTDIR)\ObjLay.obj" \
	"$(INTDIR)\Profile.obj" \
	"$(INTDIR)\Sell.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Tanbun.obj" \
	"$(INTDIR)\TanbunAddDlg.obj" \
	"$(INTDIR)\TanbunEdit.obj" \
	"$(INTDIR)\TextCash.obj" \
	"$(INTDIR)\The.obj" \
	"$(INTDIR)\The.res" \
	"$(INTDIR)\TheDoc.obj" \
	"$(INTDIR)\TheView.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\ViewEd.obj" \
	"$(INTDIR)\ViewEnv.obj"

"$(OUTDIR)\Mint.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

OUTDIR=.\Free_Release
INTDIR=.\Free_Release
# Begin Custom Macros
OutDir=.\Free_Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Mint.exe"

!ELSE 

ALL : "$(OUTDIR)\Mint.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\action.obj"
	-@erase "$(INTDIR)\chara.obj"
	-@erase "$(INTDIR)\ConfigEdit.obj"
	-@erase "$(INTDIR)\ConfigSheet.obj"
	-@erase "$(INTDIR)\CREJIT.OBJ"
	-@erase "$(INTDIR)\DateTime.obj"
	-@erase "$(INTDIR)\DispPage.obj"
	-@erase "$(INTDIR)\DocTempl.obj"
	-@erase "$(INTDIR)\EnvEdit.obj"
	-@erase "$(INTDIR)\FilePage.obj"
	-@erase "$(INTDIR)\FileSelectBar.obj"
	-@erase "$(INTDIR)\FindDlg.obj"
	-@erase "$(INTDIR)\FormatCharPage.obj"
	-@erase "$(INTDIR)\FormatPage.obj"
	-@erase "$(INTDIR)\Hint.obj"
	-@erase "$(INTDIR)\Ime.obj"
	-@erase "$(INTDIR)\JumpDlg.obj"
	-@erase "$(INTDIR)\KeyGroupDlg.obj"
	-@erase "$(INTDIR)\Keyword.obj"
	-@erase "$(INTDIR)\KeywordPage.obj"
	-@erase "$(INTDIR)\LineHeader.obj"
	-@erase "$(INTDIR)\ListCtrEx.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\Mint.pch"
	-@erase "$(INTDIR)\ObjCash.obj"
	-@erase "$(INTDIR)\ObjLay.obj"
	-@erase "$(INTDIR)\Profile.obj"
	-@erase "$(INTDIR)\Sell.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Tanbun.obj"
	-@erase "$(INTDIR)\TanbunAddDlg.obj"
	-@erase "$(INTDIR)\TanbunEdit.obj"
	-@erase "$(INTDIR)\TextCash.obj"
	-@erase "$(INTDIR)\The.obj"
	-@erase "$(INTDIR)\The.res"
	-@erase "$(INTDIR)\TheDoc.obj"
	-@erase "$(INTDIR)\TheView.obj"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\ViewEd.obj"
	-@erase "$(INTDIR)\ViewEnv.obj"
	-@erase "$(OUTDIR)\Mint.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_DBCS" /D "FREE" /Fp"$(INTDIR)\Mint.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Free_Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x411 /fo"$(INTDIR)\The.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Mint.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=imm32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\Mint.pdb" /machine:I386 /out:"$(OUTDIR)\Mint.exe" 
LINK32_OBJS= \
	"$(INTDIR)\action.obj" \
	"$(INTDIR)\chara.obj" \
	"$(INTDIR)\ConfigEdit.obj" \
	"$(INTDIR)\ConfigSheet.obj" \
	"$(INTDIR)\CREJIT.OBJ" \
	"$(INTDIR)\DateTime.obj" \
	"$(INTDIR)\DispPage.obj" \
	"$(INTDIR)\DocTempl.obj" \
	"$(INTDIR)\EnvEdit.obj" \
	"$(INTDIR)\FilePage.obj" \
	"$(INTDIR)\FileSelectBar.obj" \
	"$(INTDIR)\FindDlg.obj" \
	"$(INTDIR)\FormatCharPage.obj" \
	"$(INTDIR)\FormatPage.obj" \
	"$(INTDIR)\Hint.obj" \
	"$(INTDIR)\Ime.obj" \
	"$(INTDIR)\JumpDlg.obj" \
	"$(INTDIR)\KeyGroupDlg.obj" \
	"$(INTDIR)\Keyword.obj" \
	"$(INTDIR)\KeywordPage.obj" \
	"$(INTDIR)\LineHeader.obj" \
	"$(INTDIR)\ListCtrEx.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\ObjCash.obj" \
	"$(INTDIR)\ObjLay.obj" \
	"$(INTDIR)\Profile.obj" \
	"$(INTDIR)\Sell.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Tanbun.obj" \
	"$(INTDIR)\TanbunAddDlg.obj" \
	"$(INTDIR)\TanbunEdit.obj" \
	"$(INTDIR)\TextCash.obj" \
	"$(INTDIR)\The.obj" \
	"$(INTDIR)\The.res" \
	"$(INTDIR)\TheDoc.obj" \
	"$(INTDIR)\TheView.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\ViewEd.obj" \
	"$(INTDIR)\ViewEnv.obj"

"$(OUTDIR)\Mint.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

OUTDIR=.\Free_Debug
INTDIR=.\Free_Debug
# Begin Custom Macros
OutDir=.\Free_Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Mint.exe"

!ELSE 

ALL : "$(OUTDIR)\Mint.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\action.obj"
	-@erase "$(INTDIR)\chara.obj"
	-@erase "$(INTDIR)\ConfigEdit.obj"
	-@erase "$(INTDIR)\ConfigSheet.obj"
	-@erase "$(INTDIR)\CREJIT.OBJ"
	-@erase "$(INTDIR)\DateTime.obj"
	-@erase "$(INTDIR)\DispPage.obj"
	-@erase "$(INTDIR)\DocTempl.obj"
	-@erase "$(INTDIR)\EnvEdit.obj"
	-@erase "$(INTDIR)\FilePage.obj"
	-@erase "$(INTDIR)\FileSelectBar.obj"
	-@erase "$(INTDIR)\FindDlg.obj"
	-@erase "$(INTDIR)\FormatCharPage.obj"
	-@erase "$(INTDIR)\FormatPage.obj"
	-@erase "$(INTDIR)\Hint.obj"
	-@erase "$(INTDIR)\Ime.obj"
	-@erase "$(INTDIR)\JumpDlg.obj"
	-@erase "$(INTDIR)\KeyGroupDlg.obj"
	-@erase "$(INTDIR)\Keyword.obj"
	-@erase "$(INTDIR)\KeywordPage.obj"
	-@erase "$(INTDIR)\LineHeader.obj"
	-@erase "$(INTDIR)\ListCtrEx.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\Mint.pch"
	-@erase "$(INTDIR)\ObjCash.obj"
	-@erase "$(INTDIR)\ObjLay.obj"
	-@erase "$(INTDIR)\Profile.obj"
	-@erase "$(INTDIR)\Sell.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Tanbun.obj"
	-@erase "$(INTDIR)\TanbunAddDlg.obj"
	-@erase "$(INTDIR)\TanbunEdit.obj"
	-@erase "$(INTDIR)\TextCash.obj"
	-@erase "$(INTDIR)\The.obj"
	-@erase "$(INTDIR)\The.res"
	-@erase "$(INTDIR)\TheDoc.obj"
	-@erase "$(INTDIR)\TheView.obj"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\ViewEd.obj"
	-@erase "$(INTDIR)\ViewEnv.obj"
	-@erase "$(OUTDIR)\Mint.exe"
	-@erase "$(OUTDIR)\Mint.ilk"
	-@erase "$(OUTDIR)\Mint.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "_VERIFY" /D "WIN32"\
 /D "_WINDOWS" /D "_DBCS" /D "FREE" /Fp"$(INTDIR)\Mint.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Free_Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x411 /fo"$(INTDIR)\The.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Mint.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=imm32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\Mint.pdb" /debug /machine:I386 /out:"$(OUTDIR)\Mint.exe"\
 /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\action.obj" \
	"$(INTDIR)\chara.obj" \
	"$(INTDIR)\ConfigEdit.obj" \
	"$(INTDIR)\ConfigSheet.obj" \
	"$(INTDIR)\CREJIT.OBJ" \
	"$(INTDIR)\DateTime.obj" \
	"$(INTDIR)\DispPage.obj" \
	"$(INTDIR)\DocTempl.obj" \
	"$(INTDIR)\EnvEdit.obj" \
	"$(INTDIR)\FilePage.obj" \
	"$(INTDIR)\FileSelectBar.obj" \
	"$(INTDIR)\FindDlg.obj" \
	"$(INTDIR)\FormatCharPage.obj" \
	"$(INTDIR)\FormatPage.obj" \
	"$(INTDIR)\Hint.obj" \
	"$(INTDIR)\Ime.obj" \
	"$(INTDIR)\JumpDlg.obj" \
	"$(INTDIR)\KeyGroupDlg.obj" \
	"$(INTDIR)\Keyword.obj" \
	"$(INTDIR)\KeywordPage.obj" \
	"$(INTDIR)\LineHeader.obj" \
	"$(INTDIR)\ListCtrEx.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\ObjCash.obj" \
	"$(INTDIR)\ObjLay.obj" \
	"$(INTDIR)\Profile.obj" \
	"$(INTDIR)\Sell.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Tanbun.obj" \
	"$(INTDIR)\TanbunAddDlg.obj" \
	"$(INTDIR)\TanbunEdit.obj" \
	"$(INTDIR)\TextCash.obj" \
	"$(INTDIR)\The.obj" \
	"$(INTDIR)\The.res" \
	"$(INTDIR)\TheDoc.obj" \
	"$(INTDIR)\TheView.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\ViewEd.obj" \
	"$(INTDIR)\ViewEnv.obj"

"$(OUTDIR)\Mint.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "Mint - Win32 Release" || "$(CFG)" == "Mint - Win32 Debug" ||\
 "$(CFG)" == "Mint - Win32 Verify" || "$(CFG)" == "Mint - Win32 Free Release" ||\
 "$(CFG)" == "Mint - Win32 Free Debug"
SOURCE=.\action.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_ACTIO=\
	".\action.h"\
	".\chara.h"\
	".\globdefs.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	

"$(INTDIR)\action.obj" : $(SOURCE) $(DEP_CPP_ACTIO) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_ACTIO=\
	".\action.h"\
	".\chara.h"\
	".\Sell.h"\
	".\Util.h"\
	

"$(INTDIR)\action.obj" : $(SOURCE) $(DEP_CPP_ACTIO) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_ACTIO=\
	".\action.h"\
	".\chara.h"\
	".\globdefs.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	

"$(INTDIR)\action.obj" : $(SOURCE) $(DEP_CPP_ACTIO) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_ACTIO=\
	".\action.h"\
	".\chara.h"\
	".\globdefs.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	

"$(INTDIR)\action.obj" : $(SOURCE) $(DEP_CPP_ACTIO) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_ACTIO=\
	".\action.h"\
	".\chara.h"\
	".\globdefs.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	

"$(INTDIR)\action.obj" : $(SOURCE) $(DEP_CPP_ACTIO) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\chara.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_CHARA=\
	".\chara.h"\
	".\globdefs.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	

"$(INTDIR)\chara.obj" : $(SOURCE) $(DEP_CPP_CHARA) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_CHARA=\
	".\chara.h"\
	".\Util.h"\
	

"$(INTDIR)\chara.obj" : $(SOURCE) $(DEP_CPP_CHARA) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_CHARA=\
	".\chara.h"\
	".\globdefs.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	

"$(INTDIR)\chara.obj" : $(SOURCE) $(DEP_CPP_CHARA) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_CHARA=\
	".\chara.h"\
	".\globdefs.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	

"$(INTDIR)\chara.obj" : $(SOURCE) $(DEP_CPP_CHARA) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_CHARA=\
	".\chara.h"\
	".\globdefs.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	

"$(INTDIR)\chara.obj" : $(SOURCE) $(DEP_CPP_CHARA) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\ConfigEdit.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_CONFI=\
	".\chara.h"\
	".\ConfigEdit.h"\
	".\DateTime.h"\
	".\EnvEdit.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\ListCtrEx.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ConfigEdit.obj" : $(SOURCE) $(DEP_CPP_CONFI) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_CONFI=\
	".\chara.h"\
	".\ConfigEdit.h"\
	".\DateTime.h"\
	".\EnvEdit.h"\
	".\Keyword.h"\
	".\ListCtrEx.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ConfigEdit.obj" : $(SOURCE) $(DEP_CPP_CONFI) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_CONFI=\
	".\chara.h"\
	".\ConfigEdit.h"\
	".\DateTime.h"\
	".\EnvEdit.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\ListCtrEx.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ConfigEdit.obj" : $(SOURCE) $(DEP_CPP_CONFI) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_CONFI=\
	".\chara.h"\
	".\ConfigEdit.h"\
	".\DateTime.h"\
	".\EnvEdit.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\ListCtrEx.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ConfigEdit.obj" : $(SOURCE) $(DEP_CPP_CONFI) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_CONFI=\
	".\chara.h"\
	".\ConfigEdit.h"\
	".\DateTime.h"\
	".\EnvEdit.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\ListCtrEx.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ConfigEdit.obj" : $(SOURCE) $(DEP_CPP_CONFI) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\ConfigSheet.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_CONFIG=\
	".\chara.h"\
	".\ConfigSheet.h"\
	".\DateTime.h"\
	".\DispPage.h"\
	".\FilePage.h"\
	".\FormatCharPage.h"\
	".\FormatPage.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\KeywordPage.h"\
	".\ListCtrEx.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ConfigSheet.obj" : $(SOURCE) $(DEP_CPP_CONFIG) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_CONFIG=\
	".\chara.h"\
	".\ConfigSheet.h"\
	".\DateTime.h"\
	".\DispPage.h"\
	".\FilePage.h"\
	".\FormatCharPage.h"\
	".\FormatPage.h"\
	".\Keyword.h"\
	".\KeywordPage.h"\
	".\ListCtrEx.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ConfigSheet.obj" : $(SOURCE) $(DEP_CPP_CONFIG) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_CONFIG=\
	".\chara.h"\
	".\ConfigSheet.h"\
	".\DateTime.h"\
	".\DispPage.h"\
	".\FilePage.h"\
	".\FormatCharPage.h"\
	".\FormatPage.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\KeywordPage.h"\
	".\ListCtrEx.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ConfigSheet.obj" : $(SOURCE) $(DEP_CPP_CONFIG) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_CONFIG=\
	".\chara.h"\
	".\ConfigSheet.h"\
	".\DateTime.h"\
	".\DispPage.h"\
	".\FilePage.h"\
	".\FormatCharPage.h"\
	".\FormatPage.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\KeywordPage.h"\
	".\ListCtrEx.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ConfigSheet.obj" : $(SOURCE) $(DEP_CPP_CONFIG) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_CONFIG=\
	".\chara.h"\
	".\ConfigSheet.h"\
	".\DateTime.h"\
	".\DispPage.h"\
	".\FilePage.h"\
	".\FormatCharPage.h"\
	".\FormatPage.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\KeywordPage.h"\
	".\ListCtrEx.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ConfigSheet.obj" : $(SOURCE) $(DEP_CPP_CONFIG) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\CREJIT.CPP

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_CREJI=\
	".\chara.h"\
	".\CREJIT.H"\
	".\globdefs.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\The.h"\
	".\Util.h"\
	".\version.h"\
	

"$(INTDIR)\CREJIT.OBJ" : $(SOURCE) $(DEP_CPP_CREJI) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_CREJI=\
	".\chara.h"\
	".\CREJIT.H"\
	".\The.h"\
	".\Util.h"\
	".\version.h"\
	

"$(INTDIR)\CREJIT.OBJ" : $(SOURCE) $(DEP_CPP_CREJI) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_CREJI=\
	".\chara.h"\
	".\CREJIT.H"\
	".\globdefs.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\The.h"\
	".\Util.h"\
	".\version.h"\
	

"$(INTDIR)\CREJIT.OBJ" : $(SOURCE) $(DEP_CPP_CREJI) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_CREJI=\
	".\chara.h"\
	".\CREJIT.H"\
	".\globdefs.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\The.h"\
	".\Util.h"\
	".\version.h"\
	

"$(INTDIR)\CREJIT.OBJ" : $(SOURCE) $(DEP_CPP_CREJI) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_CREJI=\
	".\chara.h"\
	".\CREJIT.H"\
	".\globdefs.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\The.h"\
	".\Util.h"\
	".\version.h"\
	

"$(INTDIR)\CREJIT.OBJ" : $(SOURCE) $(DEP_CPP_CREJI) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\DateTime.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_DATET=\
	".\chara.h"\
	".\DateTime.h"\
	".\globdefs.h"\
	".\Profile.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\The.h"\
	".\Util.h"\
	

"$(INTDIR)\DateTime.obj" : $(SOURCE) $(DEP_CPP_DATET) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_DATET=\
	".\chara.h"\
	".\DateTime.h"\
	".\Profile.h"\
	".\The.h"\
	".\Util.h"\
	

"$(INTDIR)\DateTime.obj" : $(SOURCE) $(DEP_CPP_DATET) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_DATET=\
	".\chara.h"\
	".\DateTime.h"\
	".\globdefs.h"\
	".\Profile.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\The.h"\
	".\Util.h"\
	

"$(INTDIR)\DateTime.obj" : $(SOURCE) $(DEP_CPP_DATET) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_DATET=\
	".\chara.h"\
	".\DateTime.h"\
	".\globdefs.h"\
	".\Profile.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\The.h"\
	".\Util.h"\
	

"$(INTDIR)\DateTime.obj" : $(SOURCE) $(DEP_CPP_DATET) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_DATET=\
	".\chara.h"\
	".\DateTime.h"\
	".\globdefs.h"\
	".\Profile.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\The.h"\
	".\Util.h"\
	

"$(INTDIR)\DateTime.obj" : $(SOURCE) $(DEP_CPP_DATET) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\DispPage.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_DISPP=\
	".\chara.h"\
	".\DateTime.h"\
	".\DispPage.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\ListCtrEx.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\DispPage.obj" : $(SOURCE) $(DEP_CPP_DISPP) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_DISPP=\
	".\chara.h"\
	".\DateTime.h"\
	".\DispPage.h"\
	".\Keyword.h"\
	".\ListCtrEx.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\DispPage.obj" : $(SOURCE) $(DEP_CPP_DISPP) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_DISPP=\
	".\chara.h"\
	".\DateTime.h"\
	".\DispPage.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\ListCtrEx.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\DispPage.obj" : $(SOURCE) $(DEP_CPP_DISPP) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_DISPP=\
	".\chara.h"\
	".\DateTime.h"\
	".\DispPage.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\ListCtrEx.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\DispPage.obj" : $(SOURCE) $(DEP_CPP_DISPP) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_DISPP=\
	".\chara.h"\
	".\DateTime.h"\
	".\DispPage.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\ListCtrEx.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\DispPage.obj" : $(SOURCE) $(DEP_CPP_DISPP) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\DocTempl.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_DOCTE=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\DocTempl.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\DocTempl.obj" : $(SOURCE) $(DEP_CPP_DOCTE) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_DOCTE=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\DocTempl.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\DocTempl.obj" : $(SOURCE) $(DEP_CPP_DOCTE) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_DOCTE=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\DocTempl.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\DocTempl.obj" : $(SOURCE) $(DEP_CPP_DOCTE) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_DOCTE=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\DocTempl.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\DocTempl.obj" : $(SOURCE) $(DEP_CPP_DOCTE) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_DOCTE=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\DocTempl.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\DocTempl.obj" : $(SOURCE) $(DEP_CPP_DOCTE) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\EnvEdit.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_ENVED=\
	".\chara.h"\
	".\DateTime.h"\
	".\EnvEdit.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\EnvEdit.obj" : $(SOURCE) $(DEP_CPP_ENVED) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_ENVED=\
	".\chara.h"\
	".\DateTime.h"\
	".\EnvEdit.h"\
	".\Keyword.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\EnvEdit.obj" : $(SOURCE) $(DEP_CPP_ENVED) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_ENVED=\
	".\chara.h"\
	".\DateTime.h"\
	".\EnvEdit.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\EnvEdit.obj" : $(SOURCE) $(DEP_CPP_ENVED) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_ENVED=\
	".\chara.h"\
	".\DateTime.h"\
	".\EnvEdit.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\EnvEdit.obj" : $(SOURCE) $(DEP_CPP_ENVED) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_ENVED=\
	".\chara.h"\
	".\DateTime.h"\
	".\EnvEdit.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\EnvEdit.obj" : $(SOURCE) $(DEP_CPP_ENVED) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\FilePage.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_FILEP=\
	".\chara.h"\
	".\DateTime.h"\
	".\FilePage.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\FilePage.obj" : $(SOURCE) $(DEP_CPP_FILEP) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_FILEP=\
	".\chara.h"\
	".\DateTime.h"\
	".\FilePage.h"\
	".\Keyword.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\FilePage.obj" : $(SOURCE) $(DEP_CPP_FILEP) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_FILEP=\
	".\chara.h"\
	".\DateTime.h"\
	".\FilePage.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\FilePage.obj" : $(SOURCE) $(DEP_CPP_FILEP) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_FILEP=\
	".\chara.h"\
	".\DateTime.h"\
	".\FilePage.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\FilePage.obj" : $(SOURCE) $(DEP_CPP_FILEP) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_FILEP=\
	".\chara.h"\
	".\DateTime.h"\
	".\FilePage.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\FilePage.obj" : $(SOURCE) $(DEP_CPP_FILEP) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\FileSelectBar.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_FILES=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\DocTempl.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\FileSelectBar.obj" : $(SOURCE) $(DEP_CPP_FILES) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_FILES=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\DocTempl.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\FileSelectBar.obj" : $(SOURCE) $(DEP_CPP_FILES) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_FILES=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\DocTempl.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\FileSelectBar.obj" : $(SOURCE) $(DEP_CPP_FILES) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_FILES=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\DocTempl.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\FileSelectBar.obj" : $(SOURCE) $(DEP_CPP_FILES) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_FILES=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\DocTempl.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\FileSelectBar.obj" : $(SOURCE) $(DEP_CPP_FILES) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\FindDlg.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_FINDD=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\FindDlg.obj" : $(SOURCE) $(DEP_CPP_FINDD) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_FINDD=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FindDlg.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\FindDlg.obj" : $(SOURCE) $(DEP_CPP_FINDD) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_FINDD=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\FindDlg.obj" : $(SOURCE) $(DEP_CPP_FINDD) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_FINDD=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\FindDlg.obj" : $(SOURCE) $(DEP_CPP_FINDD) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_FINDD=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\FindDlg.obj" : $(SOURCE) $(DEP_CPP_FINDD) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\FormatCharPage.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_FORMA=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\FormatCharPage.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\FormatCharPage.obj" : $(SOURCE) $(DEP_CPP_FORMA) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_FORMA=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\FormatCharPage.h"\
	".\Hint.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\FormatCharPage.obj" : $(SOURCE) $(DEP_CPP_FORMA) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_FORMA=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\FormatCharPage.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\FormatCharPage.obj" : $(SOURCE) $(DEP_CPP_FORMA) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_FORMA=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\FormatCharPage.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\FormatCharPage.obj" : $(SOURCE) $(DEP_CPP_FORMA) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_FORMA=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\FormatCharPage.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\FormatCharPage.obj" : $(SOURCE) $(DEP_CPP_FORMA) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\FormatPage.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_FORMAT=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\FormatPage.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\FormatPage.obj" : $(SOURCE) $(DEP_CPP_FORMAT) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_FORMAT=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\FormatPage.h"\
	".\Hint.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\FormatPage.obj" : $(SOURCE) $(DEP_CPP_FORMAT) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_FORMAT=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\FormatPage.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\FormatPage.obj" : $(SOURCE) $(DEP_CPP_FORMAT) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_FORMAT=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\FormatPage.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\FormatPage.obj" : $(SOURCE) $(DEP_CPP_FORMAT) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_FORMAT=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\FormatPage.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\FormatPage.obj" : $(SOURCE) $(DEP_CPP_FORMAT) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\Hint.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_HINT_=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TextCash.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\Hint.obj" : $(SOURCE) $(DEP_CPP_HINT_) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_HINT_=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\Tanbun.h"\
	".\TextCash.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\Hint.obj" : $(SOURCE) $(DEP_CPP_HINT_) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_HINT_=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TextCash.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\Hint.obj" : $(SOURCE) $(DEP_CPP_HINT_) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_HINT_=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TextCash.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\Hint.obj" : $(SOURCE) $(DEP_CPP_HINT_) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_HINT_=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TextCash.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\Hint.obj" : $(SOURCE) $(DEP_CPP_HINT_) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\Ime.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_IME_C=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\Ime.obj" : $(SOURCE) $(DEP_CPP_IME_C) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_IME_C=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FindDlg.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\Ime.obj" : $(SOURCE) $(DEP_CPP_IME_C) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_IME_C=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\Ime.obj" : $(SOURCE) $(DEP_CPP_IME_C) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_IME_C=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\Ime.obj" : $(SOURCE) $(DEP_CPP_IME_C) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_IME_C=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\Ime.obj" : $(SOURCE) $(DEP_CPP_IME_C) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\JumpDlg.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_JUMPD=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\JumpDlg.h"\
	".\Keyword.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\JumpDlg.obj" : $(SOURCE) $(DEP_CPP_JUMPD) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_JUMPD=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FindDlg.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\JumpDlg.h"\
	".\Keyword.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\JumpDlg.obj" : $(SOURCE) $(DEP_CPP_JUMPD) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_JUMPD=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\JumpDlg.h"\
	".\Keyword.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\JumpDlg.obj" : $(SOURCE) $(DEP_CPP_JUMPD) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_JUMPD=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\JumpDlg.h"\
	".\Keyword.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\JumpDlg.obj" : $(SOURCE) $(DEP_CPP_JUMPD) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_JUMPD=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\JumpDlg.h"\
	".\Keyword.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\JumpDlg.obj" : $(SOURCE) $(DEP_CPP_JUMPD) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\KeyGroupDlg.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_KEYGR=\
	".\chara.h"\
	".\globdefs.h"\
	".\KeyGroupDlg.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\The.h"\
	".\Util.h"\
	

"$(INTDIR)\KeyGroupDlg.obj" : $(SOURCE) $(DEP_CPP_KEYGR) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_KEYGR=\
	".\chara.h"\
	".\KeyGroupDlg.h"\
	".\The.h"\
	".\Util.h"\
	

"$(INTDIR)\KeyGroupDlg.obj" : $(SOURCE) $(DEP_CPP_KEYGR) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_KEYGR=\
	".\chara.h"\
	".\globdefs.h"\
	".\KeyGroupDlg.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\The.h"\
	".\Util.h"\
	

"$(INTDIR)\KeyGroupDlg.obj" : $(SOURCE) $(DEP_CPP_KEYGR) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_KEYGR=\
	".\chara.h"\
	".\globdefs.h"\
	".\KeyGroupDlg.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\The.h"\
	".\Util.h"\
	

"$(INTDIR)\KeyGroupDlg.obj" : $(SOURCE) $(DEP_CPP_KEYGR) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_KEYGR=\
	".\chara.h"\
	".\globdefs.h"\
	".\KeyGroupDlg.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\The.h"\
	".\Util.h"\
	

"$(INTDIR)\KeyGroupDlg.obj" : $(SOURCE) $(DEP_CPP_KEYGR) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\Keyword.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_KEYWO=\
	".\chara.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\Profile.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	

"$(INTDIR)\Keyword.obj" : $(SOURCE) $(DEP_CPP_KEYWO) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_KEYWO=\
	".\chara.h"\
	".\Keyword.h"\
	".\Profile.h"\
	".\Util.h"\
	

"$(INTDIR)\Keyword.obj" : $(SOURCE) $(DEP_CPP_KEYWO) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_KEYWO=\
	".\chara.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\Profile.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	

"$(INTDIR)\Keyword.obj" : $(SOURCE) $(DEP_CPP_KEYWO) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_KEYWO=\
	".\chara.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\Profile.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	

"$(INTDIR)\Keyword.obj" : $(SOURCE) $(DEP_CPP_KEYWO) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_KEYWO=\
	".\chara.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\Profile.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	

"$(INTDIR)\Keyword.obj" : $(SOURCE) $(DEP_CPP_KEYWO) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\KeywordPage.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_KEYWOR=\
	".\chara.h"\
	".\DateTime.h"\
	".\globdefs.h"\
	".\KeyGroupDlg.h"\
	".\Keyword.h"\
	".\KeywordPage.h"\
	".\ListCtrEx.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\KeywordPage.obj" : $(SOURCE) $(DEP_CPP_KEYWOR) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_KEYWOR=\
	".\chara.h"\
	".\DateTime.h"\
	".\KeyGroupDlg.h"\
	".\Keyword.h"\
	".\KeywordPage.h"\
	".\ListCtrEx.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\KeywordPage.obj" : $(SOURCE) $(DEP_CPP_KEYWOR) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_KEYWOR=\
	".\chara.h"\
	".\DateTime.h"\
	".\globdefs.h"\
	".\KeyGroupDlg.h"\
	".\Keyword.h"\
	".\KeywordPage.h"\
	".\ListCtrEx.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\KeywordPage.obj" : $(SOURCE) $(DEP_CPP_KEYWOR) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_KEYWOR=\
	".\chara.h"\
	".\DateTime.h"\
	".\globdefs.h"\
	".\KeyGroupDlg.h"\
	".\Keyword.h"\
	".\KeywordPage.h"\
	".\ListCtrEx.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\KeywordPage.obj" : $(SOURCE) $(DEP_CPP_KEYWOR) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_KEYWOR=\
	".\chara.h"\
	".\DateTime.h"\
	".\globdefs.h"\
	".\KeyGroupDlg.h"\
	".\Keyword.h"\
	".\KeywordPage.h"\
	".\ListCtrEx.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\KeywordPage.obj" : $(SOURCE) $(DEP_CPP_KEYWOR) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\LineHeader.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_LINEH=\
	".\chara.h"\
	".\globdefs.h"\
	".\LineHeader.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\The.h"\
	".\Util.h"\
	

"$(INTDIR)\LineHeader.obj" : $(SOURCE) $(DEP_CPP_LINEH) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_LINEH=\
	".\chara.h"\
	".\LineHeader.h"\
	".\The.h"\
	".\Util.h"\
	

"$(INTDIR)\LineHeader.obj" : $(SOURCE) $(DEP_CPP_LINEH) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_LINEH=\
	".\chara.h"\
	".\globdefs.h"\
	".\LineHeader.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\The.h"\
	".\Util.h"\
	

"$(INTDIR)\LineHeader.obj" : $(SOURCE) $(DEP_CPP_LINEH) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_LINEH=\
	".\chara.h"\
	".\globdefs.h"\
	".\LineHeader.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\The.h"\
	".\Util.h"\
	

"$(INTDIR)\LineHeader.obj" : $(SOURCE) $(DEP_CPP_LINEH) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_LINEH=\
	".\chara.h"\
	".\globdefs.h"\
	".\LineHeader.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\The.h"\
	".\Util.h"\
	

"$(INTDIR)\LineHeader.obj" : $(SOURCE) $(DEP_CPP_LINEH) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\ListCtrEx.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_LISTC=\
	".\chara.h"\
	".\globdefs.h"\
	".\ListCtrEx.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	

"$(INTDIR)\ListCtrEx.obj" : $(SOURCE) $(DEP_CPP_LISTC) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_LISTC=\
	".\chara.h"\
	".\ListCtrEx.h"\
	".\Util.h"\
	

"$(INTDIR)\ListCtrEx.obj" : $(SOURCE) $(DEP_CPP_LISTC) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_LISTC=\
	".\chara.h"\
	".\globdefs.h"\
	".\ListCtrEx.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	

"$(INTDIR)\ListCtrEx.obj" : $(SOURCE) $(DEP_CPP_LISTC) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_LISTC=\
	".\chara.h"\
	".\globdefs.h"\
	".\ListCtrEx.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	

"$(INTDIR)\ListCtrEx.obj" : $(SOURCE) $(DEP_CPP_LISTC) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_LISTC=\
	".\chara.h"\
	".\globdefs.h"\
	".\ListCtrEx.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	

"$(INTDIR)\ListCtrEx.obj" : $(SOURCE) $(DEP_CPP_LISTC) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\MainFrm.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_MAINF=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_MAINF=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_MAINF=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_MAINF=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_MAINF=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\ObjCash.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_OBJCA=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Keyword.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TextCash.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ObjCash.obj" : $(SOURCE) $(DEP_CPP_OBJCA) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_OBJCA=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\Hint.h"\
	".\Keyword.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\Tanbun.h"\
	".\TextCash.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ObjCash.obj" : $(SOURCE) $(DEP_CPP_OBJCA) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_OBJCA=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Keyword.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TextCash.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ObjCash.obj" : $(SOURCE) $(DEP_CPP_OBJCA) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_OBJCA=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Keyword.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TextCash.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ObjCash.obj" : $(SOURCE) $(DEP_CPP_OBJCA) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_OBJCA=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Keyword.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TextCash.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ObjCash.obj" : $(SOURCE) $(DEP_CPP_OBJCA) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\ObjLay.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_OBJLA=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Keyword.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ObjLay.obj" : $(SOURCE) $(DEP_CPP_OBJLA) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_OBJLA=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\Hint.h"\
	".\Keyword.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\Tanbun.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ObjLay.obj" : $(SOURCE) $(DEP_CPP_OBJLA) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_OBJLA=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Keyword.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ObjLay.obj" : $(SOURCE) $(DEP_CPP_OBJLA) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_OBJLA=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Keyword.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ObjLay.obj" : $(SOURCE) $(DEP_CPP_OBJLA) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_OBJLA=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Keyword.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ObjLay.obj" : $(SOURCE) $(DEP_CPP_OBJLA) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\Profile.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_PROFI=\
	".\chara.h"\
	".\globdefs.h"\
	".\Profile.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	

"$(INTDIR)\Profile.obj" : $(SOURCE) $(DEP_CPP_PROFI) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_PROFI=\
	".\chara.h"\
	".\Profile.h"\
	".\Util.h"\
	

"$(INTDIR)\Profile.obj" : $(SOURCE) $(DEP_CPP_PROFI) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_PROFI=\
	".\chara.h"\
	".\globdefs.h"\
	".\Profile.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	

"$(INTDIR)\Profile.obj" : $(SOURCE) $(DEP_CPP_PROFI) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_PROFI=\
	".\chara.h"\
	".\globdefs.h"\
	".\Profile.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	

"$(INTDIR)\Profile.obj" : $(SOURCE) $(DEP_CPP_PROFI) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_PROFI=\
	".\chara.h"\
	".\globdefs.h"\
	".\Profile.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	

"$(INTDIR)\Profile.obj" : $(SOURCE) $(DEP_CPP_PROFI) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\Sell.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_SELL_=\
	".\chara.h"\
	".\globdefs.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	

"$(INTDIR)\Sell.obj" : $(SOURCE) $(DEP_CPP_SELL_) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_SELL_=\
	".\chara.h"\
	".\Sell.h"\
	".\Util.h"\
	

"$(INTDIR)\Sell.obj" : $(SOURCE) $(DEP_CPP_SELL_) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_SELL_=\
	".\chara.h"\
	".\globdefs.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	

"$(INTDIR)\Sell.obj" : $(SOURCE) $(DEP_CPP_SELL_) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_SELL_=\
	".\chara.h"\
	".\globdefs.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	

"$(INTDIR)\Sell.obj" : $(SOURCE) $(DEP_CPP_SELL_) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_SELL_=\
	".\chara.h"\
	".\globdefs.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	

"$(INTDIR)\Sell.obj" : $(SOURCE) $(DEP_CPP_SELL_) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_STDAF=\
	".\chara.h"\
	".\globdefs.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_DBCS" /D "SHAR" /Fp"$(INTDIR)\Mint.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Mint.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_STDAF=\
	".\chara.h"\
	".\globdefs.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "_VERIFY" /D\
 "WIN32" /D "_WINDOWS" /D "_DBCS" /D "SHAR" /Fp"$(INTDIR)\Mint.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Mint.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_STDAF=\
	".\chara.h"\
	".\globdefs.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O1 /D "NDEBUG" /D "_VERIFY" /D "WIN32" /D\
 "_WINDOWS" /D "_DBCS" /D "SHAR" /Fp"$(INTDIR)\Mint.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Mint.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_STDAF=\
	".\chara.h"\
	".\globdefs.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_DBCS" /D "FREE" /Fp"$(INTDIR)\Mint.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Mint.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_STDAF=\
	".\chara.h"\
	".\globdefs.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "_VERIFY" /D\
 "WIN32" /D "_WINDOWS" /D "_DBCS" /D "FREE" /Fp"$(INTDIR)\Mint.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Mint.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Tanbun.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_TANBU=\
	".\chara.h"\
	".\globdefs.h"\
	".\Profile.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	

"$(INTDIR)\Tanbun.obj" : $(SOURCE) $(DEP_CPP_TANBU) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_TANBU=\
	".\chara.h"\
	".\Profile.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	

"$(INTDIR)\Tanbun.obj" : $(SOURCE) $(DEP_CPP_TANBU) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_TANBU=\
	".\chara.h"\
	".\globdefs.h"\
	".\Profile.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	

"$(INTDIR)\Tanbun.obj" : $(SOURCE) $(DEP_CPP_TANBU) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_TANBU=\
	".\chara.h"\
	".\globdefs.h"\
	".\Profile.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	

"$(INTDIR)\Tanbun.obj" : $(SOURCE) $(DEP_CPP_TANBU) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_TANBU=\
	".\chara.h"\
	".\globdefs.h"\
	".\Profile.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	

"$(INTDIR)\Tanbun.obj" : $(SOURCE) $(DEP_CPP_TANBU) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\TanbunAddDlg.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_TANBUN=\
	".\chara.h"\
	".\globdefs.h"\
	".\Profile.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TanbunAddDlg.h"\
	".\The.h"\
	".\Util.h"\
	

"$(INTDIR)\TanbunAddDlg.obj" : $(SOURCE) $(DEP_CPP_TANBUN) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_TANBUN=\
	".\chara.h"\
	".\Profile.h"\
	".\Tanbun.h"\
	".\TanbunAddDlg.h"\
	".\The.h"\
	".\Util.h"\
	

"$(INTDIR)\TanbunAddDlg.obj" : $(SOURCE) $(DEP_CPP_TANBUN) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_TANBUN=\
	".\chara.h"\
	".\globdefs.h"\
	".\Profile.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TanbunAddDlg.h"\
	".\The.h"\
	".\Util.h"\
	

"$(INTDIR)\TanbunAddDlg.obj" : $(SOURCE) $(DEP_CPP_TANBUN) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_TANBUN=\
	".\chara.h"\
	".\globdefs.h"\
	".\Profile.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TanbunAddDlg.h"\
	".\The.h"\
	".\Util.h"\
	

"$(INTDIR)\TanbunAddDlg.obj" : $(SOURCE) $(DEP_CPP_TANBUN) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_TANBUN=\
	".\chara.h"\
	".\globdefs.h"\
	".\Profile.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TanbunAddDlg.h"\
	".\The.h"\
	".\Util.h"\
	

"$(INTDIR)\TanbunAddDlg.obj" : $(SOURCE) $(DEP_CPP_TANBUN) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\TanbunEdit.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_TANBUNE=\
	".\chara.h"\
	".\DateTime.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\ListCtrEx.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TanbunAddDlg.h"\
	".\TanbunEdit.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\TanbunEdit.obj" : $(SOURCE) $(DEP_CPP_TANBUNE) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_TANBUNE=\
	".\chara.h"\
	".\DateTime.h"\
	".\Keyword.h"\
	".\ListCtrEx.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\Tanbun.h"\
	".\TanbunAddDlg.h"\
	".\TanbunEdit.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\TanbunEdit.obj" : $(SOURCE) $(DEP_CPP_TANBUNE) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_TANBUNE=\
	".\chara.h"\
	".\DateTime.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\ListCtrEx.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TanbunAddDlg.h"\
	".\TanbunEdit.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\TanbunEdit.obj" : $(SOURCE) $(DEP_CPP_TANBUNE) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_TANBUNE=\
	".\chara.h"\
	".\DateTime.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\ListCtrEx.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TanbunAddDlg.h"\
	".\TanbunEdit.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\TanbunEdit.obj" : $(SOURCE) $(DEP_CPP_TANBUNE) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_TANBUNE=\
	".\chara.h"\
	".\DateTime.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\ListCtrEx.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TanbunAddDlg.h"\
	".\TanbunEdit.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\TanbunEdit.obj" : $(SOURCE) $(DEP_CPP_TANBUNE) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\TextCash.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_TEXTC=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TextCash.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\TextCash.obj" : $(SOURCE) $(DEP_CPP_TEXTC) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_TEXTC=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\Hint.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\Tanbun.h"\
	".\TextCash.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\TextCash.obj" : $(SOURCE) $(DEP_CPP_TEXTC) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_TEXTC=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TextCash.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\TextCash.obj" : $(SOURCE) $(DEP_CPP_TEXTC) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_TEXTC=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TextCash.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\TextCash.obj" : $(SOURCE) $(DEP_CPP_TEXTC) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_TEXTC=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TextCash.h"\
	".\The.h"\
	".\Util.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\TextCash.obj" : $(SOURCE) $(DEP_CPP_TEXTC) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\The.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_THE_C=\
	".\action.h"\
	".\chara.h"\
	".\CREJIT.H"\
	".\DateTime.h"\
	".\DocTempl.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TextCash.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\version.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\The.obj" : $(SOURCE) $(DEP_CPP_THE_C) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_THE_C=\
	".\action.h"\
	".\chara.h"\
	".\CREJIT.H"\
	".\DateTime.h"\
	".\DocTempl.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\Tanbun.h"\
	".\TextCash.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\version.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\The.obj" : $(SOURCE) $(DEP_CPP_THE_C) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_THE_C=\
	".\action.h"\
	".\chara.h"\
	".\CREJIT.H"\
	".\DateTime.h"\
	".\DocTempl.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TextCash.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\version.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\The.obj" : $(SOURCE) $(DEP_CPP_THE_C) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_THE_C=\
	".\action.h"\
	".\chara.h"\
	".\CREJIT.H"\
	".\DateTime.h"\
	".\DocTempl.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TextCash.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\version.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\The.obj" : $(SOURCE) $(DEP_CPP_THE_C) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_THE_C=\
	".\action.h"\
	".\chara.h"\
	".\CREJIT.H"\
	".\DateTime.h"\
	".\DocTempl.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TextCash.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\version.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\The.obj" : $(SOURCE) $(DEP_CPP_THE_C) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\The.rc
DEP_RSC_THE_R=\
	".\res\bitmap1.bmp"\
	".\res\bmp00001.bmp"\
	".\res\edt_tool.bmp"\
	".\res\ico00001.ico"\
	".\res\ico00002.ico"\
	".\res\ico00003.ico"\
	".\res\ico00004.ico"\
	".\res\ico00005.ico"\
	".\res\ico00006.ico"\
	".\res\ico00007.ico"\
	".\res\ico00008.ico"\
	".\res\icon1.ico"\
	".\res\mainfram.bmp"\
	".\res\stateico.bmp"\
	".\res\The.ico"\
	".\res\The.rc2"\
	".\res\TheDoc.ico"\
	".\res\thetype.ico"\
	".\res\Toolbar.bmp"\
	".\res\txt.ico"\
	{$(INCLUDE)}"l.jpn\afxprint.rc"\
	{$(INCLUDE)}"l.jpn\afxres.rc"\
	

"$(INTDIR)\The.res" : $(SOURCE) $(DEP_RSC_THE_R) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\TheDoc.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_THEDO=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\TheDoc.obj" : $(SOURCE) $(DEP_CPP_THEDO) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_THEDO=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\TheDoc.obj" : $(SOURCE) $(DEP_CPP_THEDO) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_THEDO=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\TheDoc.obj" : $(SOURCE) $(DEP_CPP_THEDO) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_THEDO=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\TheDoc.obj" : $(SOURCE) $(DEP_CPP_THEDO) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_THEDO=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\TheDoc.obj" : $(SOURCE) $(DEP_CPP_THEDO) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\TheView.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_THEVI=\
	".\action.h"\
	".\chara.h"\
	".\ConfigEdit.h"\
	".\ConfigSheet.h"\
	".\DateTime.h"\
	".\DispPage.h"\
	".\FilePage.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\FormatCharPage.h"\
	".\FormatPage.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\JumpDlg.h"\
	".\Keyword.h"\
	".\KeywordPage.h"\
	".\LineHeader.h"\
	".\ListCtrEx.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TanbunAddDlg.h"\
	".\TanbunEdit.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\TheView.obj" : $(SOURCE) $(DEP_CPP_THEVI) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_THEVI=\
	".\action.h"\
	".\chara.h"\
	".\ConfigEdit.h"\
	".\ConfigSheet.h"\
	".\DateTime.h"\
	".\DispPage.h"\
	".\FilePage.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\FormatCharPage.h"\
	".\FormatPage.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\JumpDlg.h"\
	".\Keyword.h"\
	".\KeywordPage.h"\
	".\LineHeader.h"\
	".\ListCtrEx.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\Tanbun.h"\
	".\TanbunAddDlg.h"\
	".\TanbunEdit.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\TheView.obj" : $(SOURCE) $(DEP_CPP_THEVI) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_THEVI=\
	".\action.h"\
	".\chara.h"\
	".\ConfigEdit.h"\
	".\ConfigSheet.h"\
	".\DateTime.h"\
	".\DispPage.h"\
	".\FilePage.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\FormatCharPage.h"\
	".\FormatPage.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\JumpDlg.h"\
	".\Keyword.h"\
	".\KeywordPage.h"\
	".\LineHeader.h"\
	".\ListCtrEx.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TanbunAddDlg.h"\
	".\TanbunEdit.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\TheView.obj" : $(SOURCE) $(DEP_CPP_THEVI) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_THEVI=\
	".\action.h"\
	".\chara.h"\
	".\ConfigEdit.h"\
	".\ConfigSheet.h"\
	".\DateTime.h"\
	".\DispPage.h"\
	".\FilePage.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\FormatCharPage.h"\
	".\FormatPage.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\JumpDlg.h"\
	".\Keyword.h"\
	".\KeywordPage.h"\
	".\LineHeader.h"\
	".\ListCtrEx.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TanbunAddDlg.h"\
	".\TanbunEdit.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\TheView.obj" : $(SOURCE) $(DEP_CPP_THEVI) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_THEVI=\
	".\action.h"\
	".\chara.h"\
	".\ConfigEdit.h"\
	".\ConfigSheet.h"\
	".\DateTime.h"\
	".\DispPage.h"\
	".\FilePage.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\FormatCharPage.h"\
	".\FormatPage.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\JumpDlg.h"\
	".\Keyword.h"\
	".\KeywordPage.h"\
	".\LineHeader.h"\
	".\ListCtrEx.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TanbunAddDlg.h"\
	".\TanbunEdit.h"\
	".\The.h"\
	".\TheDoc.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\TheView.obj" : $(SOURCE) $(DEP_CPP_THEVI) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\util.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_UTIL_=\
	".\chara.h"\
	".\globdefs.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\util.obj" : $(SOURCE) $(DEP_CPP_UTIL_) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_UTIL_=\
	".\chara.h"\
	".\Util.h"\
	

"$(INTDIR)\util.obj" : $(SOURCE) $(DEP_CPP_UTIL_) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_UTIL_=\
	".\chara.h"\
	".\globdefs.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\util.obj" : $(SOURCE) $(DEP_CPP_UTIL_) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_UTIL_=\
	".\chara.h"\
	".\globdefs.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\util.obj" : $(SOURCE) $(DEP_CPP_UTIL_) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_UTIL_=\
	".\chara.h"\
	".\globdefs.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Util.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	

"$(INTDIR)\util.obj" : $(SOURCE) $(DEP_CPP_UTIL_) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\ViewEd.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_VIEWE=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TextCash.h"\
	".\The.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ViewEd.obj" : $(SOURCE) $(DEP_CPP_VIEWE) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_VIEWE=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\Tanbun.h"\
	".\TextCash.h"\
	".\The.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ViewEd.obj" : $(SOURCE) $(DEP_CPP_VIEWE) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_VIEWE=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TextCash.h"\
	".\The.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ViewEd.obj" : $(SOURCE) $(DEP_CPP_VIEWE) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_VIEWE=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TextCash.h"\
	".\The.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ViewEd.obj" : $(SOURCE) $(DEP_CPP_VIEWE) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_VIEWE=\
	".\action.h"\
	".\chara.h"\
	".\DateTime.h"\
	".\FileSelectBar.h"\
	".\FindDlg.h"\
	".\globdefs.h"\
	".\Hint.h"\
	".\Ime.h"\
	".\Keyword.h"\
	".\MainFrm.h"\
	".\ObjCash.h"\
	".\ObjLay.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\TextCash.h"\
	".\The.h"\
	".\TheView.h"\
	".\Util.h"\
	".\ViewEd.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ViewEd.obj" : $(SOURCE) $(DEP_CPP_VIEWE) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 

SOURCE=.\ViewEnv.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

DEP_CPP_VIEWEN=\
	".\chara.h"\
	".\DateTime.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\version.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ViewEnv.obj" : $(SOURCE) $(DEP_CPP_VIEWEN) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

DEP_CPP_VIEWEN=\
	".\chara.h"\
	".\DateTime.h"\
	".\Keyword.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\version.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ViewEnv.obj" : $(SOURCE) $(DEP_CPP_VIEWEN) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

DEP_CPP_VIEWEN=\
	".\chara.h"\
	".\DateTime.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\version.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ViewEnv.obj" : $(SOURCE) $(DEP_CPP_VIEWEN) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

DEP_CPP_VIEWEN=\
	".\chara.h"\
	".\DateTime.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\version.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ViewEnv.obj" : $(SOURCE) $(DEP_CPP_VIEWEN) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

DEP_CPP_VIEWEN=\
	".\chara.h"\
	".\DateTime.h"\
	".\globdefs.h"\
	".\Keyword.h"\
	".\Profile.h"\
	".\Sell.h"\
	".\StdAfx.h"\
	".\swenv.h"\
	".\Tanbun.h"\
	".\The.h"\
	".\Util.h"\
	".\version.h"\
	".\ViewEnv.h"\
	

"$(INTDIR)\ViewEnv.obj" : $(SOURCE) $(DEP_CPP_VIEWEN) "$(INTDIR)"\
 "$(INTDIR)\Mint.pch"


!ENDIF 


!ENDIF 

