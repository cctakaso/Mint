# Microsoft Developer Studio Project File - Name="Mint" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Mint - Win32 Green Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "Mint.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "Mint.mak" CFG="Mint - Win32 Green Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "Mint - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "Mint - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE "Mint - Win32 Verify" ("Win32 (x86) Application" 用)
!MESSAGE "Mint - Win32 Free Release" ("Win32 (x86) Application" 用)
!MESSAGE "Mint - Win32 Free Debug" ("Win32 (x86) Application" 用)
!MESSAGE "Mint - Win32 Green Release" ("Win32 (x86) Application" 用)
!MESSAGE "Mint - Win32 Green Debug" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Mint - Win32 Release"

# PROP BASE Use_MFC 5
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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_DBCS" /D "SHAR" /D "BLUE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 imm32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "_VERIFY" /D "WIN32" /D "_WINDOWS" /D "_DBCS" /D "SHAR" /D "BLUE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 imm32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Mint___W"
# PROP BASE Intermediate_Dir "Mint___W"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Verify"
# PROP Intermediate_Dir "Verify"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_DBCS" /D "_VERIFY" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /D "NDEBUG" /D "_VERIFY" /D "WIN32" /D "_WINDOWS" /D "_DBCS" /D "SHAR" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 imm32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 imm32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Mint___W"
# PROP BASE Intermediate_Dir "Mint___W"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Free_Release"
# PROP Intermediate_Dir "Free_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_DBCS" /D "SHARWARE" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_DBCS" /D "FREE" /D "YERROW" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 imm32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 imm32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Mint___0"
# PROP BASE Intermediate_Dir "Mint___0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Free_Debug"
# PROP Intermediate_Dir "Free_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "_VERIFY" /D "WIN32" /D "_WINDOWS" /D "_DBCS" /D "SHARWARE" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "_VERIFY" /D "WIN32" /D "_WINDOWS" /D "_DBCS" /D "FREE" /D "YERROW" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 imm32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 imm32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Mint___Win32_Green_Release"
# PROP BASE Intermediate_Dir "Mint___Win32_Green_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Green_Release"
# PROP Intermediate_Dir "Green_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_DBCS" /D "SHAR" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_DBCS" /D "SHAR" /D "GREEN" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 imm32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 imm32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Mint___Win32_Green_Debug"
# PROP BASE Intermediate_Dir "Mint___Win32_Green_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Green_Debug"
# PROP Intermediate_Dir "Green_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "_VERIFY" /D "WIN32" /D "_WINDOWS" /D "_DBCS" /D "SHAR" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "_VERIFY" /D "WIN32" /D "_WINDOWS" /D "_DBCS" /D "SHAR" /D "GREEN" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 imm32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 imm32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Mint - Win32 Release"
# Name "Mint - Win32 Debug"
# Name "Mint - Win32 Verify"
# Name "Mint - Win32 Free Release"
# Name "Mint - Win32 Free Debug"
# Name "Mint - Win32 Green Release"
# Name "Mint - Win32 Green Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\action.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\chara.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ConfigEdit.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ConfigSheet.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CREJIT.CPP

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DateTime.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DispPage.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DocTempl.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\EnvEdit.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FileListDlg.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FilePage.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FileSelectBar.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FindDlg.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FormatCharPage.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FormatPage.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Hint.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Ime.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\JumpDlg.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\KeyGroupDlg.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Keyword.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\KeywordPage.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LineHeader.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ListCtrEx.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ObjCash.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ObjLay.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Profile.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Sell.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Tanbun.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TanbunAddDlg.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TanbunEdit.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TextCash.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\The.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\The.rc

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"
# PROP Exclude_From_Build 1
# ADD BASE RSC /l 0x411
# ADD RSC /l 0x411

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"
# PROP Exclude_From_Build 1
# ADD BASE RSC /l 0x411
# ADD RSC /l 0x411

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\The_green.rc

!IF  "$(CFG)" == "Mint - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP BASE Exclude_From_Build 1
# PROP Intermediate_Dir "Green_Release"
# ADD BASE RSC /l 0x411
# ADD RSC /l 0x411

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP BASE Exclude_From_Build 1
# PROP Intermediate_Dir "Green_Debug"
# ADD BASE RSC /l 0x411
# ADD RSC /l 0x411

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\The_yerrow.rc

!IF  "$(CFG)" == "Mint - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

# ADD BASE RSC /l 0x411
# ADD RSC /l 0x411 /fo"Free_Release/The_yerrow.res"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

# ADD BASE RSC /l 0x411
# ADD RSC /l 0x411 /fo"Free_Debug/The_yerrow.res"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP BASE Exclude_From_Build 1
# PROP Intermediate_Dir "Green_Release"
# PROP Exclude_From_Build 1
# ADD BASE RSC /l 0x411
# ADD RSC /l 0x411

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP BASE Exclude_From_Build 1
# PROP Intermediate_Dir "Green_Debug"
# PROP Exclude_From_Build 1
# ADD BASE RSC /l 0x411
# ADD RSC /l 0x411

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TheDoc.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TheView.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\util.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ViewEd.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ViewEnv.cpp

!IF  "$(CFG)" == "Mint - Win32 Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Verify"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Free Debug"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Release"

# PROP Intermediate_Dir "Green_Release"

!ELSEIF  "$(CFG)" == "Mint - Win32 Green Debug"

# PROP Intermediate_Dir "Green_Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\action.h
# End Source File
# Begin Source File

SOURCE=.\chara.h
# End Source File
# Begin Source File

SOURCE=.\ConfigEdit.h
# End Source File
# Begin Source File

SOURCE=.\ConfigSheet.h
# End Source File
# Begin Source File

SOURCE=.\CREJIT.H
# End Source File
# Begin Source File

SOURCE=.\DateTime.h
# End Source File
# Begin Source File

SOURCE=.\DispPage.h
# End Source File
# Begin Source File

SOURCE=.\DocTempl.h
# End Source File
# Begin Source File

SOURCE=.\EnvEdit.h
# End Source File
# Begin Source File

SOURCE=.\FileListDlg.h
# End Source File
# Begin Source File

SOURCE=.\FilePage.h
# End Source File
# Begin Source File

SOURCE=.\FileSelectBar.h
# End Source File
# Begin Source File

SOURCE=.\FindDlg.h
# End Source File
# Begin Source File

SOURCE=.\FormatCharPage.h
# End Source File
# Begin Source File

SOURCE=.\FormatPage.h
# End Source File
# Begin Source File

SOURCE=.\globdefs.h
# End Source File
# Begin Source File

SOURCE=.\Hint.h
# End Source File
# Begin Source File

SOURCE=.\Ime.h
# End Source File
# Begin Source File

SOURCE=.\JumpDlg.h
# End Source File
# Begin Source File

SOURCE=.\KeyGroupDlg.h
# End Source File
# Begin Source File

SOURCE=.\Keyword.h
# End Source File
# Begin Source File

SOURCE=.\KeywordPage.h
# End Source File
# Begin Source File

SOURCE=.\LineHeader.h
# End Source File
# Begin Source File

SOURCE=.\ListCtrEx.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\ObjCash.h
# End Source File
# Begin Source File

SOURCE=.\ObjLay.h
# End Source File
# Begin Source File

SOURCE=.\PageDlg.h
# End Source File
# Begin Source File

SOURCE=.\Profile.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Sell.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\swenv.h
# End Source File
# Begin Source File

SOURCE=.\Tanbun.h
# End Source File
# Begin Source File

SOURCE=.\TanbunAddDlg.h
# End Source File
# Begin Source File

SOURCE=.\TanbunEdit.h
# End Source File
# Begin Source File

SOURCE=.\TextCash.h
# End Source File
# Begin Source File

SOURCE=.\The.h
# End Source File
# Begin Source File

SOURCE=.\TheDoc.h
# End Source File
# Begin Source File

SOURCE=.\TheView.h
# End Source File
# Begin Source File

SOURCE=.\Util.h
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# Begin Source File

SOURCE=.\ViewEd.h
# End Source File
# Begin Source File

SOURCE=.\ViewEnv.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\edt_tool.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00005.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00006.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00007.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00008.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_yerrow.ico
# End Source File
# Begin Source File

SOURCE=.\res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mainfram_green.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mainfram_yellow.bmp
# End Source File
# Begin Source File

SOURCE=.\res\mainfram_yerrow.bmp
# End Source File
# Begin Source File

SOURCE=.\res\minttype.ico
# End Source File
# Begin Source File

SOURCE=.\res\stateico.bmp
# End Source File
# Begin Source File

SOURCE=.\res\The.ico
# End Source File
# Begin Source File

SOURCE=.\res\the_green.ico
# End Source File
# Begin Source File

SOURCE=.\res\the_read.ico
# End Source File
# Begin Source File

SOURCE=.\res\the_yellow.ico
# End Source File
# Begin Source File

SOURCE=.\res\TheDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\thedoc_green.ico
# End Source File
# Begin Source File

SOURCE=.\res\thedoc_read.ico
# End Source File
# Begin Source File

SOURCE=.\res\thedoc_yellow.ico
# End Source File
# Begin Source File

SOURCE=.\res\thedoc_yerrow.ico
# End Source File
# Begin Source File

SOURCE=.\res\thetype.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar_green.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar_yellow.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar_yerrow.bmp
# End Source File
# Begin Source File

SOURCE=.\res\txt.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
