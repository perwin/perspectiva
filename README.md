# Perspectiva

<img src="graphics/random_scene_osamp8.jpg" width=95%>

(Image generated with 8x8-pixel oversampling.)


**Perspectiva** is a (relatively) simple C++ raytracing program. It's a hobby project, and so obviously not recommended for anything remotely serious.

Some modest features:

* Scene files that use a YAML-based text format

* Environment maps (using skybox or equiangular spherical-projection images)
    
* Pixel subsampling (uniform or jittered uniform)

* Area lights (and corresponding soft shadows)

* Fresnel reflection and refraction
    
* Depth-of-field, including bokeh from polygonal apertures

* Outputs PPM, PNG, or [OpenEXR](https://en.wikipedia.org/wiki/OpenEXR) images



## Building Perspectiva

Perspectiva can be built with a modern C++ compiler, such as GCC's g++ or Clang's clang++. Since it uses some features from C++-17, it's best to use version 7 or later of GCC or version 5 or later of Clang.

To build:

	$ scons perspectiva

(This will use whatever is the default C++ compiler on your system -- e.g., on macOS, it will probably use clang++.)


It requires the following libraries:

* [yaml-cpp](https://github.com/jbeder/yaml-cpp)

* [OpenEXR](https://github.com/AcademySoftwareFoundation/openexr)

It also requires that SConstruct be installed, in order to run the actual build process.


## Miscellaneous Notes

The name is taken from the titles of various medieval works on optics, such as those by Roger Bacon, Witelo, and John Pecham.


## License

Perspectiva is licensed under the BSD 3-clause License.
