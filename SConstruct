# SConstruct file for Perspectiva (was: raytracer2) project

# To build with debugging turned on:
#   scons define=DEBUG perspectiva
# To build with OpenMP:
#   scons --openmp perspectiva

AddOption("--opt", dest="optimize", action="store_true", 
    default=False, help="compile with -O3")
AddOption("--openmp", dest="useOpenMP", action="store_true", 
    default=False, help="compile with OpenMP support")

useOpenMP = False
if GetOption("useOpenMP") is True:
    useOpenMP = True
    usingClangOpenMP = True


cflags = ["-g", "-Wall", "-std=c++17"]
if GetOption("optimize") is True:
    cflags.append("-O3")
else:
    cflags.append("-O0")
# Libraries: yaml-cpp, IlmImf [part of OpenEXF]
lib_list = ["yaml-cpp", "libOpenEXR", "m"]
include_paths = [".", "/usr/local/include/Imath", "/usr/local/include/OpenEXR"]
link_flags = []

extra_defines = []
# Add any additional, user-specified preprocessor definitions (e.g., "define=DEBUG")
for key, value in ARGLIST:
    if key == 'define':
        extra_defines.append(value)

if useOpenMP:
    if usingClangOpenMP:
        # special flags for Apple clang++ (assumes libomp is installed)
        cflags.append("-Xpreprocessor")
        cflags.append("-fopenmp")
        link_flags.append("-Xpreprocessor")
        link_flags.append("-fopenmp")
        # dynamic linking to libomp
        link_flags.append("-lomp")
    else:
        # flags for (real) g++
        cflags.append("-fopenmp")
        cflags_db.append("-fopenmp")
        link_flags.append("-fopenmp")


main_source_files = """commandline_parser.cpp utilities.cpp shapes.cpp transform.cpp
 cameras.cpp render.cpp render_utils.cpp image_io.cpp sampler.cpp uniform_sampler.cpp 
 uniform_jitter_sampler.cpp mersenne_twister.cpp scenefile_parser.cpp 
 environment_map.cpp perspectiva_main.cpp"""
main_source_files_list = main_source_files.split()
main_source_files_list = ["src/" + fname for fname in main_source_files_list]

env = Environment(CC = 'clang', CXX="clang++", CXXFLAGS=cflags, CPPPATH=include_paths,
				LIBS=lib_list, CPPDEFINES=extra_defines, LINKFLAGS=link_flags)

env.Program("perspectiva", main_source_files_list)

