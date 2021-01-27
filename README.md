# glTests

This is a basic opengl framework I'm slowly making. I wish to grow it into quite a capable program, although that will take time.

The program is made to compile in Visual C++, although it may compile fine in others

### Dependancies:

* SDL2
* GLAD
* STB
* GLM

### Compilation:

Include locations of dependancies
Include locations of libraries for SDL2
File "SDL2.dll" is required in the folder of the .exe (at least on windows)

### Limitations:

While there is .obj file support, only certain formats are supported as of now
The files need to include:
* Vertex coordinates (v)
* Vertex normals (vn)
* Texture coordinates (vt)

Polygonal face elements need to be in the format:
    
    **v/vn/vt**

.mtl files are not yet supported, although they will not cause an error.
