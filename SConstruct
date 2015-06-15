# SConstruct file for raytracer2 project

main_source_files = """commandline_parser.cpp utilities.cpp geometry.cpp raytrace2_main.cpp"""
main_source_files_list = main_source_files.split()

#env = Environment(CC = 'gcc-5', CXX="g++-5")
env = Environment()

env.Program("raytracer2", main_source_files_list)

