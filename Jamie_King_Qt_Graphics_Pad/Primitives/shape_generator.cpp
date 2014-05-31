#include "shape_generator.h"

// for my shape object (technically included in the "shape generator" header, but included here anyway)
#include "Primitives\shape_data.h"

// for my vertex object (included indirectly via "shape generator" header, then "shape data" header, then "vertex" header, but included directly here anyway)
#include "Primitives\vertex.h"

// for the glm functions (technically included indirectly through "vertex" header)
#include <glm\glm\glm.hpp>

using glm::vec4;

#include <glm\glm\gtc\matrix_transform.hpp>
using glm::translate;
using glm::perspective;
using glm::mat4;
using glm::vec3;
#include <iostream>

my_shape_data my_shape_generator::make_double_triangle()
{
   // declare some vertices and make a shape out of it
   my_shape_data double_tri;

   const float RED_TRIANGLE_Z = +0.5f;
   const float BLUE_TRIANGLE_Z = -0.5f;
   my_vertex verts[] =
   {
      // first triangle
      vec4(-1.0f, -1.0f, RED_TRIANGLE_Z, 1.0f),    // left bottom corner
      vec4(1.0f, 0.0f, 0.0f, 1.0f),          // all red
      vec4(+1.0f, -1.0f, RED_TRIANGLE_Z, 1.0f),    // right bottom corner
      vec4(1.0f, 1.0f, 0.0f, 1.0f),          // red + green (apparently this makes yellow)
      vec4(+0.0f, +1.0f, -1.0f, 1.0f),    // center top
      vec4(1.0f, 0.0f, 1.0f, 1.0f),          // red + blue (apparently this makes pink

      // second triangle
      vec4(-1.0f, +1.0f, BLUE_TRIANGLE_Z, 1.0f),   // left top corner
      vec4(0.0f, 0.0f, 1.0f, 0.1f),          // all blue
      vec4(+1.0f, +1.0f, BLUE_TRIANGLE_Z, 1.0f),   // right top corner
      vec4(1.0f, 0.0f, 1.0f, 0.5f),          // blue + red
      vec4(+0.0f, -1.0f, BLUE_TRIANGLE_Z, 1.0f),   // center bottom
      vec4(0.0f, 1.0f, 1.0f, 1.0f),          // blue + green
   };

   // record some size data
   // Note: The type of the vertex array is my_vertex, which means that each index refers to
   // two sets of vec4 objects as per the definition of my_vertex.
   double_tri.size_bytes_per_vertex = sizeof(verts[0]);
   double_tri.size_bytes_per_position_vertex = sizeof(verts[0].position);
   double_tri.size_bytes_per_color_vertex = sizeof(verts[0].color);
   double_tri.num_position_entries_per_vertex = sizeof(verts[0].position) / sizeof(verts[0].position[0]);
   double_tri.num_color_entries_per_vertex = sizeof(verts[0].color) / sizeof(verts[0].color[0]);


   // count the number of vertices (not my custom vertex, which includes color vectors, but total vertices)
   // Note: Instead of manually typing in the type value for the denominator, get the size of the first element in the array.  This is type safe.
   double_tri.num_vertices = sizeof(verts) / sizeof(*verts);
   
   // allocate memory on the heap for the vertices
   double_tri.vertices = new my_vertex[double_tri.num_vertices];

   // copy the vertex data onto the heap-allocated memory
   memcpy(double_tri.vertices, verts, sizeof(verts));

   
   // vertex index data
   GLushort indices[] = { 0, 1, 2, 3, 4, 5 };
   //GLushort indices[] = { 0, 1, 2 };

   // count the number of indices
   double_tri.num_indices = sizeof(indices) / sizeof(*indices);

   // allocate memory on the heap for the indices
   double_tri.indices = new GLushort[double_tri.num_indices];

   // copy the vertex index data onto the heap-allocated memory
   memcpy(double_tri.indices, indices, sizeof(indices));

   
   // finally, recturn a copy of the structure, which actually isn't that big (two integers and two pointers)
   return double_tri;
}

my_shape_data my_shape_generator::make_cube()
{
   my_shape_data cube;

   my_vertex verts[] =
   {
      // face 1
      vec4(-1.0f, +1.0f, +1.0f, 1.0f),    // position 0
      vec4(+1.0f, +0.0f, +0.0f, 1.0f),    // color
      vec4(+1.0f, +1.0f, +1.0f, 1.0f),    // position 1
      vec4(+0.0f, +1.0f, +0.0f, 1.0f),    // color
      vec4(+1.0f, +1.0f, -1.0f, 1.0f),    // position 2
      vec4(+0.0f, +0.0f, +1.0f, 1.0f),    // color
      vec4(-1.0f, +1.0f, -1.0f, 1.0f),    // position 3
      vec4(+1.0f, +1.0f, +1.0f, 1.0f),    // color

      // face 2
      vec4(-1.0f, +1.0f, -1.0f, 1.0f),    // position 4
      vec4(+1.0f, +0.0f, +1.0f, 1.0f),    // color
      vec4(+1.0f, +1.0f, -1.0f, 1.0f),    // position 5
      vec4(+0.0f, +0.5f, +0.2f, 1.0f),    // color
      vec4(+1.0f, -1.0f, -1.0f, 1.0f),    // position 6
      vec4(+0.8f, +0.6f, +0.4f, 1.0f),    // color
      vec4(-1.0f, -1.0f, -1.0f, 1.0f),    // position 7
      vec4(+0.3f, +1.0f, +0.5f, 1.0f),    // color

      // face 3
      vec4(+1.0f, +1.0f, -1.0f, 1.0f),    // position 8
      vec4(+0.2f, +0.5f, +0.2f, 1.0f),    // color
      vec4(+1.0f, +1.0f, +1.0f, 1.0f),    // position 9
      vec4(+0.9f, +0.3f, +0.7f, 1.0f),    // color
      vec4(+1.0f, -1.0f, +1.0f, 1.0f),    // position 10
      vec4(+0.3f, +0.7f, +0.5f, 1.0f),    // color
      vec4(+1.0f, -1.0f, -1.0f, 1.0f),    // position 11
      vec4(+0.5f, +0.7f, +0.5f, 1.0f),    // color

      // face 4
      vec4(-1.0f, +1.0f, +1.0f, 1.0f),    // position 12
      vec4(+0.7f, +0.8f, +0.2f, 1.0f),    // color
      vec4(-1.0f, +1.0f, -1.0f, 1.0f),    // position 13
      vec4(+0.5f, +0.7f, +0.3f, 1.0f),    // color
      vec4(-1.0f, -1.0f, -1.0f, 1.0f),    // position 14
      vec4(+0.4f, +0.7f, +0.7f, 1.0f),    // color
      vec4(-1.0f, -1.0f, +1.0f, 1.0f),    // position 15
      vec4(+0.2f, +0.5f, +1.0f, 1.0f),    // color

      // face 5
      vec4(+1.0f, +1.0f, +1.0f, 1.0f),    // position 16
      vec4(+0.6f, +1.0f, +0.7f, 1.0f),    // color
      vec4(-1.0f, +1.0f, +1.0f, 1.0f),    // position 17
      vec4(+0.6f, +0.4f, +0.8f, 1.0f),    // color
      vec4(-1.0f, -1.0f, +1.0f, 1.0f),    // position 18
      vec4(+0.2f, +0.8f, +0.7f, 1.0f),    // color
      vec4(+1.0f, -1.0f, +1.0f, 1.0f),    // position 19
      vec4(+0.2f, +0.7f, +1.0f, 1.0f),    // color

      // face 6
      vec4(+1.0f, -1.0f, -1.0f, 1.0f),    // position 20
      vec4(+0.8f, +0.3f, +0.7f, 1.0f),    // color
      vec4(-1.0f, -1.0f, -1.0f, 1.0f),    // position 21
      vec4(+0.8f, +0.9f, +0.5f, 1.0f),    // color
      vec4(-1.0f, -1.0f, +1.0f, 1.0f),    // position 22
      vec4(+0.5f, +0.8f, +0.5f, 1.0f),    // color
      vec4(+1.0f, -1.0f, +1.0f, 1.0f),    // position 23
      vec4(+0.9f, +1.0f, +0.2f, 1.0f),    // color
   };

   // record some size data
   // Note: The type of the vertex array is my_vertex, which means that each index refers to
   // two sets of vec4 objects as per the definition of my_vertex.
   cube.size_bytes_per_vertex = sizeof(verts[0]);
   cube.size_bytes_per_position_vertex = sizeof(verts[0].position);
   cube.size_bytes_per_color_vertex = sizeof(verts[0].color);
   cube.num_position_entries_per_vertex = sizeof(verts[0].position) / sizeof(verts[0].position[0]);
   cube.num_color_entries_per_vertex = sizeof(verts[0].color) / sizeof(verts[0].color[0]);



   GLuint array_size_bytes = sizeof(verts);
   cube.num_vertices = array_size_bytes / sizeof(*verts);
   cube.vertices = new my_vertex[cube.num_vertices];
   memcpy(cube.vertices, verts, array_size_bytes);


   // copied from video
   // Note: Recall that the following indexs to the vertices array will access "my_vertex" structs, not vec4(...) 
   // items (they seem to be some kind of template, but I don't know exactly what, so I won't call them structs or
   // objects).  At each "my_vertex" struct, that address will point to the first of four position floats, and 
   // following that are 4 color floats.  
   // So this all works out just peachy.
   GLushort indices[] =
   {
      0, 1, 2, 0, 2, 3,     // top
      4, 5, 6, 4, 6, 7,     // front
      8, 9, 10, 8, 10, 11,     // right
      12, 13, 14, 12, 14, 15,     // left
      16, 17, 18, 16, 18, 19,     // back
      20, 22, 21, 20, 23, 22,     // bottom
   };

   array_size_bytes = sizeof(indices);
   cube.num_indices = array_size_bytes / sizeof(*indices);
   cube.indices = new GLushort[cube.num_indices];
   memcpy(cube.indices, indices, array_size_bytes);


   return cube;
}