:: SHADER COMPILE SCRIPT FOR XIV
:: Author: Mara Campbell
:: -----------------------------

@echo off

:: run glslc to compile the shaders from GLSL to SPIR-V
glslc simple.vert -o simple.vert.spv
glslc simple.frag -o simple.frag.spv