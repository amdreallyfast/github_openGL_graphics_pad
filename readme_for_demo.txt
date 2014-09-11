These projects were created in Visual Studio 2013.  There is a “middleware” 
folder that contains the third-party (Qt, glew, and gtest) functionality that 
my program needs to compile.  I couldn’t get around needing the header, source, 
.lib, and .dll files, so the middleware fold got a little bloated. 

You should be able to open the .sln file, wait for VS to recompile it’s 
intellisense database, build it, and see it happen.
 
If it doesn’t build or display, please let me know the error and I’ll try to 
fix it.
 
In its current state, it displays a plane with a torus, a teapot, and a diffuse 
light positioned above it.  I am only lighting the objects in object space now 
though, and I am learning about what I need to do to translate the vertex 
normals before performing the diffuse light calculations. 
 
I implemented simple controls:
- WASD keys "fly" the camera
- R and F keys raise and lower the camera, respectively
- The mouse rotates the camera in place
- Click the left mouse button and move the mouse left and right to rotate the 
   teapot 
 
John Cox