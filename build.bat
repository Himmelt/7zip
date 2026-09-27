@echo off
setlocal EnableExtensions
rem ============================================================
rem build.bat - unified build script (works locally and in CI)
rem Replicates the packaging flow of .github/workflows/release.yml
rem Output: build\7zip_setup.exe
rem
rem Usage:
rem   build.bat        full rebuild (nmake /A), identical to CI
rem   build.bat fast   incremental build (no /A), for local iteration
rem ============================================================

set "REBUILD=/A"
if /i "%~1"=="fast" set "REBUILD="

rem --- Locate Visual Studio with VC tools (same check as CI; works on runner and dev PC) ---
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
  echo ERROR: vswhere.exe not found at "%VSWHERE%".
  exit /b 1
)
set "VSPATH="
for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSPATH=%%i"
if not defined VSPATH (
  echo ERROR: no Visual Studio installation with VC tools found.
  exit /b 1
)
echo [build] Visual Studio: %VSPATH%
call "%VSPATH%\VC\Auxiliary\Build\vcvars64.bat" || exit /b 1

cd /d "%~dp0"

rem --- Locate 7z.exe: PATH first, then default install dir ---
set "SEVENZIP="
for /f "tokens=*" %%i in ('where 7z.exe 2^>nul') do if not defined SEVENZIP set "SEVENZIP=%%i"
if not defined SEVENZIP if exist "%ProgramFiles%\7-Zip\7z.exe" set "SEVENZIP=%ProgramFiles%\7-Zip\7z.exe"
if not defined SEVENZIP (
  echo ERROR: 7z.exe not found in PATH or "%%ProgramFiles%%\7-Zip\7z.exe".
  exit /b 1
)
echo [build] 7-Zip: %SEVENZIP%

echo [build] === 1/4 CPP\7zip (7z.exe, 7z.dll, 7zFM.exe, 7zG.exe, sfx, shell ext) ===
pushd CPP\7zip
nmake %REBUILD% NEW_COMPILER=1 MY_CPU_AMD64=1 || (popd & exit /b 1)
popd

echo [build] === 2/4 C\Util\7zipInstall (installer SFX module) ===
pushd C\Util\7zipInstall
nmake %REBUILD% NEW_COMPILER=1 MY_CPU_AMD64=1 Z7_64BIT_INSTALLER=1 || (popd & exit /b 1)
popd

echo [build] === 3/4 C\Util\7zipUninstall (uninstaller) ===
pushd C\Util\7zipUninstall
nmake %REBUILD% NEW_COMPILER=1 MY_CPU_AMD64=1 Z7_64BIT_INSTALLER=1 || (popd & exit /b 1)
popd

echo [build] === 4/4 Stage app files and package setup ===
robocopy res build\appfiles /e >nul
if %ERRORLEVEL% GEQ 8 exit /b 1

copy /y CPP\7zip\Bundles\Fm\x64\7zFM.exe            build\appfiles\ || exit /b 1
copy /y CPP\7zip\UI\GUI\x64\7zG.exe                 build\appfiles\ || exit /b 1
copy /y CPP\7zip\Bundles\Format7zF\x64\7z.dll       build\appfiles\ || exit /b 1
copy /y CPP\7zip\UI\Console\x64\7z.exe              build\appfiles\ || exit /b 1
copy /y CPP\7zip\UI\Explorer\x64\7-zip.dll          build\appfiles\ || exit /b 1
copy /y CPP\7zip\Bundles\SFXWin\x64\7z.sfx          build\appfiles\ || exit /b 1
copy /y CPP\7zip\Bundles\SFXCon\x64\7zCon.sfx       build\appfiles\ || exit /b 1
copy /y C\Util\7zipUninstall\x64\7zipUninstall.exe  build\appfiles\Uninstall.exe || exit /b 1

"%SEVENZIP%" a build\content.7z ".\build\appfiles\*" -xtd -mx -mm=lzma || exit /b 1

copy /b C\Util\7zipInstall\x64\7zipInstall.exe+build\content.7z build\7zip_setup.exe >nul || exit /b 1

echo.
dir build\7zip_setup.exe
echo [build] === Done: %CD%\build\7zip_setup.exe ===
endlocal
