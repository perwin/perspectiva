# SConstruct file for raytracer2 project

# To build with debugging turned on:
# scons define=DEBUG raytracer2

cflags = ["-g", "-Wall", "-std=c++11"]
# Libraries: yaml-cpp, IlmImf [part of OpenEXF]
lib_list = ["yaml-cpp", "IlmImf", "m"]
include_paths = ["."]

extra_defines = []
# Add any additional, user-specified preprocessor definitions (e.g., "define=DEBUG")
for key, value in ARGLIST:
    if key == 'define':
        extra_defines.append(value)

main_source_files = """commandline_parser.cpp utilities.cpp shapes.cpp transform.cpp
 render.cpp image_io.cpp sampler.cpp uniform_sampler.cpp uniform_jitter_sampler.cpp
 mersenne_twister.cpp scenefile_parser.cpp raytrace2_main.cpp"""
main_source_files_list = main_source_files.split()
main_source_files_list = ["src/" + fname for fname in main_source_files_list]

env = Environment(CC = 'clang', CXX="clang++", CXXFLAGS=cflags, CPPPATH=include_paths,
				LIBS=lib_list, CPPDEFINES=extra_defines)

env.Program("raytracer2", main_source_files_list)

