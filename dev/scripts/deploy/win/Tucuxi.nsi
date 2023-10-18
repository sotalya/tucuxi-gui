; example1.nsi
;
; This script is perhaps one of the simplest NSIs you can make. All of the
; optional settings are left to their default settings. The installer simply 
; prompts the user asking them where to install, and drops a copy of example1.nsi
; there. 

;--------------------------------

; The name of the installer
Name "Tucuxi"

; The file to write
OutFile "../../../Setup-${TARGET}${ARCH}.exe"

; The default installation directory
InstallDir c:\Tucuxi

; Request application privileges for Windows Vista
RequestExecutionLevel user

;--------------------------------
; Pages
Page directory
Page instfiles

;--------------------------------
; The stuff to install
Section "Tucuxi"

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  File /r ../../../Deploy\${ARCH}\${TARGET}\*
  
SectionEnd ; end the section
