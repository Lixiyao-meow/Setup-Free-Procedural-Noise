The project is developped based on the code MyRenderer from the course INF584, so the Building and Running use the same command. A new class PoissonDisk is added for the surface noise generation.

The program is developped and tested with the sphere_high_res.off model. It only work for the unit 3D model for now. As each time we need to change control parameters to adapt the surface noise, the parameters in the code works best with the unit shpere.

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

The noise filters are computed in the shader file "PBRFragmentShader.glsl". To change the noise filter, just uncomment the kernel you want to test in the main function.
