### Overview
We implement procedural surface noise without using texture coordinates. The noise is based on sparse convolution and the Gabor kernel with an isotropic filter. It is mainly based on this article: https://www-sop.inria.fr/reves/Basilic/2009/LLDD09/LLDD09PNSGC_paper.pdf

Due to the copyright issue, here is not the entire project. I just put the class and shader concerning the generation of surface noise. So this repository contains:

- Class to generate 3D Poisson Disk: PoissonDisk.cpp
- Shader to control the noise texture: PBRFragmentShader.glsl

The visual result is shown below:

![Garbor](https://user-images.githubusercontent.com/76222299/161384039-8badb9b0-2c1b-4874-ba3f-15f58a740ed1.png)
