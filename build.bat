cd /d D:\Projects\7zip\CPP\7zip
nmake NEW_COMPILER=1 MY_CPU_AMD64=1

cd /d D:\Projects\7zip\C\Util\7zipInstall
nmake NEW_COMPILER=1 MY_CPU_AMD64=1 Z7_64BIT_INSTALLER=1

cd /d D:\Projects\7zip
rmdir /s /q D:\Projects\7zip\build

robocopy "C:\Program Files\7-Zip" D:\Projects\7zip\build\appfiles /e /xd Lang
robocopy D:\Projects\7zip\Lang D:\Projects\7zip\build\appfiles\Lang /e
rem robocopy D:\Projects\7zip\CPP\7zip\Bundles\Fm\x64 D:\Projects\7zip\build\appfiles 7zFM.exe /is
xcopy D:\Projects\7zip\CPP\7zip\Bundles\Fm\x64\7zFM.exe D:\Projects\7zip\build\appfiles\7zFM.exe /Y
7z a D:\Projects\7zip\build\content.7z D:\Projects\7zip\build\appfiles\* -xtd -mx -mm=lzma

copy /b D:\Projects\7zip\C\Util\7zipInstall\x64\7zipInstall.exe+D:\Projects\7zip\build\content.7z D:\Projects\7zip\build\7zip_setup.exe
