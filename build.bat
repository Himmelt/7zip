cd /d D:\Projects\7zip\CPP\7zip
nmake /A NEW_COMPILER=1 MY_CPU_AMD64=1

cd /d D:\Projects\7zip\C\Util\7zipInstall
nmake /A NEW_COMPILER=1 MY_CPU_AMD64=1 Z7_64BIT_INSTALLER=1

cd /d D:\Projects\7zip\C\Util\7zipUninstall
nmake /A NEW_COMPILER=1 MY_CPU_AMD64=1 Z7_64BIT_INSTALLER=1

cd /d D:\Projects\7zip
rmdir /s /q D:\Projects\7zip\build

robocopy "C:\Program Files\7-Zip" D:\Projects\7zip\build\appfiles /e /xd Lang
robocopy D:\Projects\7zip\Lang D:\Projects\7zip\build\appfiles\Lang /e

xcopy D:\Projects\7zip\CPP\7zip\Bundles\Fm\x64\7zFM.exe D:\Projects\7zip\build\appfiles\7zFM.exe /Y
xcopy D:\Projects\7zip\CPP\7zip\UI\GUI\x64\7zG.exe D:\Projects\7zip\build\appfiles\7zG.exe /Y
xcopy D:\Projects\7zip\CPP\7zip\Bundles\Format7zF\x64\7z.dll D:\Projects\7zip\build\appfiles\7z.dll /Y
xcopy D:\Projects\7zip\CPP\7zip\UI\Console\x64\7z.exe D:\Projects\7zip\build\appfiles\7z.exe /Y
xcopy D:\Projects\7zip\CPP\7zip\UI\Explorer\x64\7-zip.dll D:\Projects\7zip\build\appfiles\7-zip.dll /Y
xcopy D:\Projects\7zip\CPP\7zip\Bundles\SFXWin\x64\7z.sfx D:\Projects\7zip\build\appfiles\7z.sfx /Y
xcopy D:\Projects\7zip\CPP\7zip\Bundles\SFXCon\x64\7zCon.sfx D:\Projects\7zip\build\appfiles\7zCon.sfx /Y
xcopy D:\Projects\7zip\C\Util\7zipUninstall\x64\7zipUninstall.exe D:\Projects\7zip\build\appfiles\Uninstall.exe /Y

7z a D:\Projects\7zip\build\content.7z D:\Projects\7zip\build\appfiles\* -xtd -mx -mm=lzma

copy /b D:\Projects\7zip\C\Util\7zipInstall\x64\7zipInstall.exe+D:\Projects\7zip\build\content.7z D:\Projects\7zip\build\7zip_setup.exe
