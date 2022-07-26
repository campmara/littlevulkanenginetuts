:: SHADER COMPILE SCRIPT FOR XIV
:: Author: Mara Campbell
:: -----------------------------

@echo off

:: run glslc to compile the shaders from GLSL to SPIR-V
glslc simple.vert -o simple.vert.spv
glslc simple.frag -o simple.frag.spv

glslc light_point.vert -o light_point.vert.spv
glslc light_point.frag -o light_point.frag.spv