# glTests

This was a project I did a while back to learn about OpenGL

The program is made to compile in Visual C++, although it may compile fine in others
Note: program includes code for VC++ memory leak detection, this will need to be removed if you try and use a different compiler

### Dependancies:

* SDL2
* GLAD
* STB
* GLM

### Compilation:

Include locations of dependancies\
Include locations of libraries for SDL2\
Set C++ version to C++17\
(In visual studio) add "glad.c" to project\
File "SDL2.dll" is required in the folder of the .exe (at least on windows)\

### Limitations:

While there is .obj file support, only certain formats are supported as of now\
The files need to include:
* Vertex coordinates (v)
* Vertex normals (vn)

Polygonal face elements need to be in the format:\
    
`f v/vt/vn` or `f v//vn`

.mtl files now have good support\
The program can interpret lots of parameters and links to texture maps\

### Use:

Models can have unlimited textures and/or materials\
They can go in (probably) any directory\
