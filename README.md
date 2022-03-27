### Overview
We implement procedural surface noise without using texture coordinates. The noise is based on sparse convolution and the Gabor kernel with an isotropic filter. It is mainly based on this article: https://www-sop.inria.fr/reves/Basilic/2009/LLDD09/LLDD09PNSGC_paper.pdf

The project is developed based on the code MyRenderer from the course INF584, so the Building and Running use the same command. A new class "PoissonDisk" is added for the surface noise generation. The program is mainly developed and tested with the "sphere_high_res.off" model. It only works for the unit 3D model for now. Usually, we need to adjust control parameters for each 3D model to adapt the surface noise. So the parameters in the code work best with the unit sphere.

### Building

This is a standard CMake project. Building it consists in running:

```
mkdir Build
cd Build
cmake ..
cd ..
cmake --build Build
```

The resuling MyRenderer executable is automatically copied to the root MyRenderer directory, so that resources (shaders, meshes) can be loaded easily. By default, the program is compile in Debug mode. For a high performance Release binary, just us:

```
cmake --build Build --config Release
```

### Running

To run the program
```
cd <path-to-MyRenderer-directory>
./MyRenderer [[file.off] [material-directory-path]]
```
To visualize the best result
```
./MyRenderer Resources/Models/sphere_high_res.off
```

Note that a collection of example meshes are provided in the Resources/Models directory and a collection of materials are provided in the Resources/Materials directory

When starting to edit the source code, rerun 

```
cmake --build Build --config Release
```
each time you want to recompile MyRenderer

The resulting binary to use is always the onelocated in the MyRenderer directory, you can safely ignore whatever is generated in the Build directory. 

### To change noise filter

The noise filters are computed in the shader file "PBRFragmentShader.glsl". To change the noise filter, uncomment the kernel you want to test in the main function.
