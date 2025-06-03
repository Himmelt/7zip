@echo off

set "PROJECT_DIR=%cd%"

if exist build (
    rmdir /s /q build
)

if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
) else (
    if exist "D:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
        call "D:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
    ) else (
        echo Error:vcvars64.bat not exist !
        pause
        exit /b 1
    )
)

cd /d "%PROJECT_DIR%\CPP\7zip"
nmake /A NEW_COMPILER=1 MY_CPU_AMD64=1

cd /d "%PROJECT_DIR%\C\Util\7zipInstall"
nmake /A NEW_COMPILER=1 MY_CPU_AMD64=1 Z7_64BIT_INSTALLER=1

cd /d "%PROJECT_DIR%\C\Util\7zipUninstall"
nmake /A NEW_COMPILER=1 MY_CPU_AMD64=1 Z7_64BIT_INSTALLER=1

cd /d "%PROJECT_DIR%"

robocopy "%PROJECT_DIR%\res" "%PROJECT_DIR%\build\appfiles" /e

copy %PROJECT_DIR%\CPP\7zip\Bundles\Fm\x64\7zFM.exe %PROJECT_DIR%\build\appfiles\7zFM.exe /Y
copy %PROJECT_DIR%\CPP\7zip\UI\GUI\x64\7zG.exe %PROJECT_DIR%\build\appfiles\7zG.exe /Y
copy %PROJECT_DIR%\CPP\7zip\Bundles\Format7zF\x64\7z.dll %PROJECT_DIR%\build\appfiles\7z.dll /Y
copy %PROJECT_DIR%\CPP\7zip\UI\Console\x64\7z.exe %PROJECT_DIR%\build\appfiles\7z.exe /Y
copy %PROJECT_DIR%\CPP\7zip\UI\Explorer\x64\7-zip.dll %PROJECT_DIR%\build\appfiles\7-zip.dll /Y
copy %PROJECT_DIR%\CPP\7zip\Bundles\SFXWin\x64\7z.sfx %PROJECT_DIR%\build\appfiles\7z.sfx /Y
copy %PROJECT_DIR%\CPP\7zip\Bundles\SFXCon\x64\7zCon.sfx %PROJECT_DIR%\build\appfiles\7zCon.sfx /Y
copy %PROJECT_DIR%\C\Util\7zipUninstall\x64\7zipUninstall.exe %PROJECT_DIR%\build\appfiles\Uninstall.exe /Y

7z a "%PROJECT_DIR%\build\content.7z" "%PROJECT_DIR%\build\appfiles\*" -xtd -mx -mm=lzma

copy /b "%PROJECT_DIR%\C\Util\7zipInstall\x64\7zipInstall.exe"+"%PROJECT_DIR%\build\content.7z" "%PROJECT_DIR%\build\7zip_setup.exe"
