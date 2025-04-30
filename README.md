# 7-Zip on GitHub
7-Zip website: [7-zip.org](https://7-zip.org)

## Build
`nmake NEW_COMPILER=1 MY_CPU_AMD64=1`

## Build Installer
`set Z7_64BIT_INSTALLER=1`
`7z a c:\1\a.7z -xtd -mx -mm=lzma`
`copy /b c:\1\7zip-setup.exe+c:\1\a.7z c:\1\setup.exe`
