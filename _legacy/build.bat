:: MARA'S SUPER SIMPLE BUILD.BAT FOR XIV
:: Author: Mara Campbell
:: -------------------------------------

:: el clásico
@echo off

:: compile all shaders
pushd shaders
call compile.bat
popd

:: make and run the executable
mingw32-make && bin\xiv.exe
