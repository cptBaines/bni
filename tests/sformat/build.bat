@echo off
if not exist build mkdir build

SET APP=sformat_test

::pushd build
call :cleanup "build\%APP%.exe"
call :cleanup "build\%APP%.pdb"
call :cleanup "build\%APP%.obj"

SET OLD_PATH=%PATH%
SET PATH=c:\wsr\tc\sys\llvm11\win64\llvm\bin;c:\wsr\tc\sys\llvm11\win64\llvm\bin\amd64
SET BUILD_DEFS=
SET INCLUDE_DIR=-Ic:\wsr\tc\home\bjorn\include\win

:: compiling without crt only link with native dlls 
clang-cl /GS- /Gs9999999 /Od /EHa- /MD -Zi -Zl %INCLUDE_DIR% %BUILD_DEFS% ^
 -Wno-pragma-pack ^
 -Wno-ignored-attributes ^
 -Fo.\build\  %APP%.c -link ^
 /libpath:./build/  ^
 /subsystem:console /debug:full /nodefaultlib /machine:x64 /out:.\build\%APP%.exe ^
 -stack:0x100000,0x100000 build\%APP%.obj ^
 kernel32.lib 

SET PATH=%OLD_PATH%
goto :EOF

:cleanup
if exist %1 del %1
goto :EOF
