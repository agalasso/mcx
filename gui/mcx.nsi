;
; MCX Installer Script
;

SetCompressor /SOLID lzma

;--------------------------------
;Include Modern UI

  !include "MUI.nsh"

;--------------------------------
;General

  ;Name and file
  Name "MallinCam Control"
  OutFile "mcx_setup.exe"

  RequestExecutionLevel user

;  Icon "mcx2.ico"
;  UninstallIcon "mcx2.ico"
  !define MUI_ICON "mcx2.ico"
  !define MUI_UNICON "mcx2.ico"

  ;Default installation folder
  InstallDir "$LOCALAPPDATA\MCXControl"

  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\adgsoftware\MCXControl" ""

  ; This is required for silent install to not silently fail, then restart app
  AllowSkipFiles off

Function .onInstSuccess
    ifSilent 0 done
      Exec $INSTDIR/mcx.exe
    done:
FunctionEnd

Function .onInstFailed
    ; fixme - testing
    ifSilent 0 done
       MessageBox MB_OK|MB_ICONINFORMATION "silent inst failed"
    done:
FunctionEnd

;Function kill_app
;  !insertmacro TerminateApp
;FunctionEnd

;Function un.kill_app
;  !insertmacro TerminateApp
;FunctionEnd

Function .onInit
; fixme - remove
;   ifSilent 0 +2
;     Call kill_app

  !insertmacro MUI_INSTALLOPTIONS_EXTRACT "install_options.ini"

FunctionEnd

;--------------------------------
;Interface Settings

;  !define MUI_ABORTWARNING

;--------------------------------
;Pages

;MUI_WELCOMEPAGE_TITLE title
;Title to display on the top of the page.
;MUI_WELCOMEPAGE_TITLE_3LINES
;Extra space for the title area.
!define MUI_WELCOMEPAGE_TEXT "This wizard will guide you through the installation of MallinCam Control.\r\n\r\nPress Next to continue."
;Text to display on the page.

  !insertmacro MUI_PAGE_WELCOME

  !define MUI_LICENSEPAGE_CHECKBOX
  !insertmacro MUI_PAGE_LICENSE "LICENSE.txt"

;  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY

  Page custom CustomPageOptions

; fixme - remove
;  !define MUI_PAGE_CUSTOMFUNCTION_PRE kill_app
  !insertmacro MUI_PAGE_INSTFILES

  !define MUI_FINISHPAGE_RUN "$INSTDIR\mcx.exe"
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Reserve Files

  ;If you are using solid compression, files that are required before
  ;the actual installation should be stored first in the data block,
  ;because this will make your installer start faster.

  ReserveFile "install_options.ini"
  !insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

;--------------------------------
;Variables

  Var INI_VALUE

;--------------------------------
;Installer Sections

;Section "ShutdownSection" SecShutdown
;  Call kill_app
;SectionEnd

Section "Dummy Section" SecDummy

  SetShellVarContext current

  SetOutPath "$INSTDIR"

  ;ADD YOUR OWN FILES HERE...

  File mcx.exe
  File LICENSE.txt
  File dso.mcx
  File lunar.mcx
  File planetary.mcx
  File solar.mcx

  ;Store installation folder
  WriteRegStr HKCU "Software\adgsoftware\MCXControl" "" $INSTDIR

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

  ; Programs Folder shortcut
  CreateDirectory "$SMPROGRAMS\MallinCam Control"
  CreateShortcut "$SMPROGRAMS\MallinCam Control\MCX.lnk" "$INSTDIR\mcx.exe"

  ; App Data dir, for log files
  CreateDirectory "$LOCALAPPDATA\MCXControl"

  ; Control Panel "Add or Remove Programs"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MCXControl" \
                 "DisplayName" "MallinCam Control"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MCXControl" \
                 "UninstallString" "$INSTDIR\Uninstall.exe"
  WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MCXControl" \
                 "NoModify" 1
  WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MCXControl" \
                 "NoRepair" 1

  IfSilent skip_shortcut

    ; Read a value from an InstallOptions INI file
    !insertmacro MUI_INSTALLOPTIONS_READ $INI_VALUE "install_options.ini" "Field 2" "State"

    ; Install desktop shortcut if check box was checked
    StrCmp $INI_VALUE "1" 0 skip_shortcut
      CreateShortcut "$DESKTOP\MallinCam Control.lnk" "$INSTDIR\mcx.exe"

skip_shortcut:

SectionEnd

Function CustomPageOptions

  !insertmacro MUI_HEADER_TEXT "Installation Options" "Optional settings for installing MallinCam Control"
  !insertmacro MUI_INSTALLOPTIONS_DISPLAY "install_options.ini"

FunctionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecDummy ${LANG_ENGLISH} "A test section."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDummy} $(DESC_SecDummy)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ; need this or start menu item removal silently fails
  SetShellVarContext current

;  Call un.kill_app

  ;ADD YOUR OWN FILES HERE...

  Delete "$INSTDIR\mcx.exe"
  Delete "$INSTDIR\LICENSE.txt"
  Delete "$INSTDIR\*.mcx"
  Delete "$INSTDIR\Uninstall.exe"
  RMDir "$INSTDIR"

  RMDir /r "$LOCALAPPDATA\MCXControl"

;  DeleteRegKey /ifempty HKCU "Software\Modern UI Test"
  DeleteRegKey /ifempty HKCU "Software\adgsoftware\MCXControl"

  Delete "$DESKTOP\MallinCam Control.lnk"

  Delete "$SMPROGRAMS\MallinCam Control\MCX.lnk"
  RMDir /r "$SMPROGRAMS\MallinCam Control"

  ; Control Panel "Add or Remove Programs"
  DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\MCXControl"

SectionEnd
