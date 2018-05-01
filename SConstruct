# SConstruct file for raytracer2 project

cflags = ["-g", "-Wall", "-std=c++11"]
# Libraries: yaml-cpp, IlmImf [part of OpenEXF]
lib_list = ["yaml-cpp", "IlmImf", "m"]

main_source_files = """commandline_parser.cpp utilities.cpp shapes.cpp transform.cpp
 render.cpp image_io.cpp sampler.cpp uniform_sampler.cpp uniform_jitter_sampler.cpp
 mersenne_twister.cpp scenefile_parser.cpp raytrace2_main.cpp"""
main_source_files_list = main_source_files.split()
main_source_files_list = ["src/" + fname for fname in main_source_files_list]

env = Environment(CC = 'clang', CXX="clang++", CXXFLAGS=cflags, LIBS=lib_list)
#env = Environment(CC = 'gcc-5', CXX="g++-5", CXXFLAGS=cflags)

env.Program("raytracer2", main_source_files_list)

