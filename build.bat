:: MARA'S SUPER SIMPLE BUILD.BAT FOR XIV
:: Author: Mara Campbell
:: -------------------------------------

@echo off

if not exist bin mkdir bin
cd bin
if not exist mingw mkdir mingw
cd mingw
:: cmake --fresh -S ../../ -B . -G "MinGW Makefiles"
cmake -S ../../  -B . -G "MinGW Makefiles"
mingw32-make.exe && mingw32-make.exe Shaders && XIV.exe
cd ..
cd ..

@pause