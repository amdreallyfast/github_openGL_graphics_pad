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
      vec3(-1.0f, -1.0f, RED_TRIANGLE_Z), // left bottom corner
      vec3(1.0f, 0.0f, 0.0f),             // all red
      vec3(+1.0f, -1.0f, RED_TRIANGLE_Z), // right bottom corner
      vec3(1.0f, 1.0f, 0.0f),             // red + green (apparently this makes yellow)
      vec3(+0.0f, +1.0f, -1.0f),          // center top
      vec3(1.0f, 0.0f, 1.0f),             // red + blue (apparently this makes pink

      // second triangle
      vec3(-1.0f, +1.0f, BLUE_TRIANGLE_Z),   // left top corner
      vec3(0.0f, 0.0f, 1.0f),                // all blue
      vec3(+1.0f, +1.0f, BLUE_TRIANGLE_Z),   // right top corner
      vec3(1.0f, 0.0f, 1.0f),                // blue + red
      vec3(+0.0f, -1.0f, BLUE_TRIANGLE_Z),   // center bottom
      vec3(0.0f, 1.0f, 1.0f),                // blue + green
   };


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
      vec3(-1.0f, +1.0f, +1.0f),    // position 0
      vec3(+1.0f, +0.0f, +0.0f),    // color
      vec3(+1.0f, +1.0f, +1.0f),    // position 1
      vec3(+0.0f, +1.0f, +0.0f),    // color
      vec3(+1.0f, +1.0f, -1.0f),    // position 2
      vec3(+0.0f, +0.0f, +1.0f),    // color
      vec3(-1.0f, +1.0f, -1.0f),    // position 3
      vec3(+1.0f, +1.0f, +1.0f),    // color

      // face 2
      vec3(-1.0f, +1.0f, -1.0f),    // position 4
      vec3(+1.0f, +0.0f, +1.0f),    // color
      vec3(+1.0f, +1.0f, -1.0f),    // position 5
      vec3(+0.0f, +0.5f, +0.2f),    // color
      vec3(+1.0f, -1.0f, -1.0f),    // position 6
      vec3(+0.8f, +0.6f, +0.4f),    // color
      vec3(-1.0f, -1.0f, -1.0f),    // position 7
      vec3(+0.3f, +1.0f, +0.5f),    // color

      // face 3
      vec3(+1.0f, +1.0f, -1.0f),    // position 8
      vec3(+0.2f, +0.5f, +0.2f),    // color
      vec3(+1.0f, +1.0f, +1.0f),    // position 9
      vec3(+0.9f, +0.3f, +0.7f),    // color
      vec3(+1.0f, -1.0f, +1.0f),    // position 10
      vec3(+0.3f, +0.7f, +0.5f),    // color
      vec3(+1.0f, -1.0f, -1.0f),    // position 11
      vec3(+0.5f, +0.7f, +0.5f),    // color

      // face 4
      vec3(-1.0f, +1.0f, +1.0f),    // position 12
      vec3(+0.7f, +0.8f, +0.2f),    // color
      vec3(-1.0f, +1.0f, -1.0f),    // position 13
      vec3(+0.5f, +0.7f, +0.3f),    // color
      vec3(-1.0f, -1.0f, -1.0f),    // position 14
      vec3(+0.4f, +0.7f, +0.7f),    // color
      vec3(-1.0f, -1.0f, +1.0f),    // position 15
      vec3(+0.2f, +0.5f, +1.0f),    // color

      // face 5
      vec3(+1.0f, +1.0f, +1.0f),    // position 16
      vec3(+0.6f, +1.0f, +0.7f),    // color
      vec3(-1.0f, +1.0f, +1.0f),    // position 17
      vec3(+0.6f, +0.4f, +0.8f),    // color
      vec3(-1.0f, -1.0f, +1.0f),    // position 18
      vec3(+0.2f, +0.8f, +0.7f),    // color
      vec3(+1.0f, -1.0f, +1.0f),    // position 19
      vec3(+0.2f, +0.7f, +1.0f),    // color

      // face 6
      vec3(+1.0f, -1.0f, -1.0f),    // position 20
      vec3(+0.8f, +0.3f, +0.7f),    // color
      vec3(-1.0f, -1.0f, -1.0f),    // position 21
      vec3(+0.8f, +0.9f, +0.5f),    // color
      vec3(-1.0f, -1.0f, +1.0f),    // position 22
      vec3(+0.5f, +0.8f, +0.5f),    // color
      vec3(+1.0f, -1.0f, +1.0f),    // position 23
      vec3(+0.9f, +1.0f, +0.2f),    // color
   };


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


my_shape_data my_shape_generator::make_3d_arrow()
{
   my_shape_data arrow;

   // only need to record the vertices once and let the indices do the duplication
   // Note: +Z face is red, -Z face is blue, and the sides will be blended.
   my_vertex verts[] =
   {
      // +Z face of rectangle
      vec3(-0.5f, +1.0f, +0.5f),    // 0
      vec3(+1.0f, +0.0f, +0.0f),    // color
      vec3(-0.5f, -2.0f, +0.5f),    // 1
      vec3(+1.0f, +0.0f, +0.0f),    // color
      vec3(+0.5f, -2.0f, +0.5f),    // 2
      vec3(+1.0f, +0.0f, +0.0f),    // color
      vec3(+0.5f, +1.0f, +0.5f),    // 3
      vec3(+1.0f, +0.0f, +0.0f),    // color

      // +Z face of arrow triangle 
      vec3(+0.0f, +2.0f, +0.5f),    // 4
      vec3(+1.0f, +0.0f, +0.0f),    // color
      vec3(-1.0f, +1.0f, +0.5f),    // 5
      vec3(+1.0f, +0.0f, +0.0f),    // color
      vec3(+1.0f, +1.0f, +0.5f),    // 6
      vec3(+1.0f, +0.0f, +0.0f),    // color

      // -Z face of rectangle
      vec3(-0.5f, +1.0f, -0.5f),    // 7
      vec3(+0.0f, +0.0f, +1.0f),    // color
      vec3(-0.5f, -2.0f, -0.5f),    // 8
      vec3(+0.0f, +0.0f, +1.0f),    // color
      vec3(+0.5f, -2.0f, -0.5f),    // 9
      vec3(+0.0f, +0.0f, +1.0f),    // color
      vec3(+0.5f, +1.0f, -0.5f),    // 10
      vec3(+0.0f, +0.0f, +1.0f),    // color

      // -Z face of arrow triangle
      vec3(+0.0f, +2.0f, -0.5f),    // 11
      vec3(+0.0f, +0.0f, +1.0f),    // color
      vec3(-1.0f, +1.0f, -0.5f),    // 12
      vec3(+0.0f, +0.0f, +1.0f),    // color
      vec3(+1.0f, +1.0f, -0.5f),    // 13
      vec3(+0.0f, +0.0f, +1.0f),    // color
   };

   GLushort indices[] =
   {
      // rectangle
      0, 1, 2, 
      0, 2, 3,    // +Z
      7, 8, 9, 7, 9, 10,   // -Z
      3, 2, 9, 3, 9, 10,   // +X
      0, 1, 8, 0, 8, 7,    // -X
      1, 2, 9, 1, 9, 8,    // -Y (base)

      // triangle
      4, 5, 0, 4, 0, 3, 4, 3, 6,          // +Z
      11, 12, 7, 11, 7, 10, 11, 10, 13,   // -Z
      3, 6, 13, 3, 13, 10,    // -Y (overhang on +X)
      5, 0, 7, 5, 7, 12,      // -Y (overhang on -X)
      4, 6, 13, 4, 13, 11,    // +Y (slanted face on +X)
      5, 4, 11, 5, 11, 12,    // +Y (slanted face on -X)
   };


   GLuint array_size_bytes = sizeof(verts);
   arrow.num_vertices = array_size_bytes / sizeof(*verts);
   arrow.vertices = new my_vertex[arrow.num_vertices];
   memcpy(arrow.vertices, verts, array_size_bytes);

   array_size_bytes = sizeof(indices);
   arrow.num_indices = array_size_bytes / sizeof(*indices);
   arrow.indices = new GLushort[arrow.num_indices];
   memcpy(arrow.indices, indices, array_size_bytes);


   return arrow;
}


vec3 random_color()
{
   vec3 ret;
   ret.x = rand() / (float)(RAND_MAX);
   ret.y = rand() / (float)(RAND_MAX);
   ret.z = rand() / (float)(RAND_MAX);
   return ret;
}

my_shape_data my_shape_generator::make_plane(unsigned int side_length)
{
   my_shape_data plane;

   // make a plane that is centered on the origin
   plane.num_vertices = side_length * side_length;
   plane.vertices = new my_vertex[plane.num_vertices];
   int row_max_count = side_length;
   float row_half_length = (float)side_length / 2;
   int col_max_count = side_length;
   float col_half_length = (float)side_length / 2;

   for (int row_count = 0; row_count < row_max_count; row_count++)
   {
      for (int col_count = 0; col_count < col_max_count; col_count++)
      {
         // start at upper left (-X, +Y) and work to lower right (+X, -Y)
         my_vertex& this_vert = plane.vertices[row_count * row_max_count + col_count];
         this_vert.position.x = col_half_length - col_count;
         this_vert.position.y = 0;
         this_vert.position.z = row_count - row_half_length;
         this_vert.color = random_color();
      }
   }

   // 6 indices to draw a square (every adjacent set of 4 vertices (including overlap))
   // Note: 3x3 is 2x2 sets of 4 vertices, 4x4 is 3x3 sets of 4, etc.
   plane.num_indices = (row_max_count - 1) * (col_max_count - 1) * 6;
   plane.indices = new GLushort[plane.num_indices];
   int index_counter = 0;
   for (int row_count = 0; row_count < (row_max_count - 1); row_count++)
   {
      for (int col_count = 0; col_count < (col_max_count - 1); col_count++)
      {
         // I worked this out on my notepad by hand.  It's a bit much to explain in a comment.
         plane.indices[index_counter++] = row_count * row_max_count + col_count;
         plane.indices[index_counter++] = (row_count + 1) * row_max_count + (col_count + 1);
         plane.indices[index_counter++] = row_count * row_max_count + (col_count + 1);
                                          
         plane.indices[index_counter++] = row_count * row_max_count + col_count;
         plane.indices[index_counter++] = (row_count + 1) * row_max_count + (col_count + 1);
         plane.indices[index_counter++] = (row_count + 1) * row_max_count + col_count;
      }
   }

   return plane;
}