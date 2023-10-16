@echo off
set DefaultPath=%TUCUXI_ROOT%\..\

REM Source -> https://stackoverflow.com/questions/8666225/how-to-test-if-a-path-is-a-file-or-directory-in-windows-batch-file
REM Check if a path as been provided, if not use the default path
for /f "tokens=1,2 delims=d" %%A in ("-%~a1") do if "%%B" neq "" (
  echo %1 is a folder  
  goto :set_path
) else (
  echo %1 is either a file or does not exist, check the input path!!!
  echo Default path %DefaultPath% will be used
  goto :set_default
)

:set_default
set FolderPath=%DefaultPath%
goto :continue

:set_path
set FolderPath=%1
goto :continue

:continue
@echo on

call Deploy64.bat
call Deploy64-Demo.bat
call deploy.bat 64 Release-extreport
call deploy.bat 64 Release-extreport-nolicense
copy %FolderPath%\tucuxi-gui\dev\Setup-Release64.exe %FolderPath%\tucuxi-gui\build\Setup-Release64.exe
copy %FolderPath%\tucuxi-gui\dev\Setup-Release-Demo64.exe %FolderPath%\tucuxi-gui\build\Setup-Release-Demo64.exe
copy %FolderPath%\tucuxi-gui\dev\Setup-Release-extreport64.exe %FolderPath%\tucuxi-gui\build\Setup-Release-extreport64.exe
copy %FolderPath%\tucuxi-gui\dev\Setup-Release-extreport-nolicense64.exe %FolderPath%\tucuxi-gui\build\Setup-Release-extreport-nolicense64.exe

pause
