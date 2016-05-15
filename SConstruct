# SConstruct file for raytracer2 project

cflags = ["-g", "-Wall", "-std=c++11"]

main_source_files = """commandline_parser.cpp utilities.cpp geometry.cpp render.cpp
 image_io.cpp sampler.cpp uniform_sampler.cpp raytrace2_main.cpp"""
main_source_files_list = main_source_files.split()

env = Environment(CC = 'clang', CXX="clang++", CXXFLAGS=cflags)
#env = Environment(CC = 'gcc-5', CXX="g++-5", CXXFLAGS=cflags)

env.Program("raytracer2", main_source_files_list)

