call Deploy64.bat
call Deploy64-Demo
call deploy.bat 64 Release-extreport
call deploy.bat 64 Release-extreport-nolicense
copy C:\Projects\Ezechiel\dev\Setup-Release64.exe E:\docs\ezechiel\Setup-Release64.exe
copy C:\Projects\Ezechiel\dev\Setup-Release-Demo64.exe E:\docs\ezechiel\Setup-Release-Demo64.exe
copy C:\Projects\Ezechiel\dev\Setup-Release-extreport64.exe E:\docs\ezechiel\Setup-Release-extreport64.exe
copy C:\Projects\Ezechiel\dev\Setup-Release-extreport-nolicense64.exe E:\docs\ezechiel\Setup-Release-extreport-nolicense64.exe

pause
