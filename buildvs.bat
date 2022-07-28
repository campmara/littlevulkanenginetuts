:: MARA'S SUPER SIMPLE BUILD.BAT FOR XIV
:: Author: Mara Campbell
:: -------------------------------------

@echo off

if not exist bin mkdir bin
cd bin
if not exist vs mkdir vs
cd vs
:: cmake --fresh -S ../../ -B .
cmake -S ../../ -B .
cd ..
cd ..

@pause