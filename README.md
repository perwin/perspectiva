# Perspectiva

Perspectiva is a (relatively) simple C++ raytracing program.

Some modest characteristics:

* Scene files using a YAML-based format

* Environment maps (skybox or equiangular spherical-projection maps)
    
* Pixel subsampling (uniform or jittered uniform)

* Area lights (and corresponding soft shadows)

* Fresnel reflection and refraction
    
* Semi-realistic depth-of-field, including bokeh from polygonal apertures

* Outputs JPEG, PNG, or [OpenEXR](https://en.wikipedia.org/wiki/OpenEXR) images

<img src="graphics/random_scene_osamp4.jpg" width=95%>

The name is taken from the titles of various medieval works on optics, such as those by Roger Bacon, Witelo, and John Pecham.
