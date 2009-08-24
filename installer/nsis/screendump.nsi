!include "nfUtils.nsh"

; MUI 1.67 compatible ------
!include "MUI.nsh"
!include "Library.nsh"
!include "x64.nsh"

SetCompressor /FINAL /SOLID lzma

var MajorVersion
var MinorVersion
var CopyGdiDll

!define PRODUCT_NAME "screendump"
!define PRODUCT_PUBLISHER "Indie Code Labs"
!define PRODUCT_WEB_SITE "http://www.indiecodelabs.com"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\screendump.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"
!define PRODUCT_VERSION "0.0.0.0"

${nfu.!FileVersion} PRODUCT_VERSION "..\..\Release\screendump.exe"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "license.txt"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_COMPONENTS
; Start menu page
var ICONS_GROUP
var AreShort
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_SHOWREADME
!define MUI_FINISHPAGE_SHOWREADME_TEXT "Start screendump when Windows Starts"
!define MUI_FINISHPAGE_SHOWREADME_FUNCTION MakeStartupLink
!define MUI_FINISHPAGE_RUN "$INSTDIR\screendump.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; Reserve files
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "screendump-${PRODUCT_VERSION}-Setup.exe"

ShowInstDetails show
ShowUnInstDetails show
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""

Function .onInit
  ${If} ${RunningX64}
    SetRegView 64
    StrCpy $INSTDIR "$PROGRAMFILES64\screendump"
  ${Else}
    StrCpy $INSTDIR "$PROGRAMFILES\screendump"
  ${EndIf}
  
  Version::GetWindowsVersion
  Pop $MajorVersion
  Pop $MinorVersion

  ${If} $MajorVersion == 5
  ${AndIF} $MinorVersion == 0
    StrCpy $CopyGdiDll "1"
  ${Else}
    StrCpy $CopyGdiDll "0"
  ${EndIf}
FunctionEnd

Function MakeStartupLink
  CreateShortCut "$SMSTARTUP\Start screendump.lnk" "$INSTDIR\screendump.exe"
FunctionEnd

Section "Program and Dependencies (required)" SEC01
  ClearErrors
  SetShellVarContext all
  SectionIn RO
  SetOutPath "$INSTDIR"
  SetOverwrite on 
  ${If} ${RunningX64}
    File /oname=screendump.exe "..\..\x64\Release\screendump-x64.exe"
  ${Else}
    File "..\..\Release\screendump.exe"
  ${EndIf}
  File "license.txt"
  File "..\..\help\HelpManual.chm"
  File "..\..\res\dontdeletethisfile"
  ${If} $CopyGdiDll == "1"
      File "gdiplus.dll"
  ${EndIf}
SectionEnd

Section "Start Menu Shortcut"
  StrCpy $ICONS_GROUP "screendump"
  IntFmt $AreShort "%u" "1"
  CreateDirectory "$SMPROGRAMS\$ICONS_GROUP"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\screendump.lnk" "$INSTDIR\screendump.exe"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section "Desktop Shortcut"
  CreateShortCut "$DESKTOP\screendump.lnk" "$INSTDIR\screendump.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\screendump.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\screendump.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "${PRODUCT_STARTMENU_REGVAL}" "$ICONS_GROUP"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDNO +2
  MessageBox MB_OK "Please make sure that the program IS NOT running. Check that the icon isn't on the System Tray. Afterwards, click on OK." IDOK +2
  Abort
FunctionEnd

Section Uninstall
  ClearErrors
  SetShellVarContext all
  ReadRegStr $ICONS_GROUP ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "${PRODUCT_STARTMENU_REGVAL}"
  Delete /REBOOTOK "$INSTDIR\screendump.exe"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\HelpManual.chm"
  Delete "$INSTDIR\*.*"
  Delete "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk"
  Delete "$DESKTOP\screendump.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\screendump.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\Changes.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\Help.lnk"
  Delete "$SMSTARTUP\Start screendump.lnk"
  RMDir "$SMPROGRAMS\$ICONS_GROUP"
  RMDir "$INSTDIR"
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd
