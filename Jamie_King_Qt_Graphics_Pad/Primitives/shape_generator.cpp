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
using glm::mat3;
using glm::vec3;


vec3 random_color()
{
   vec3 ret;
   ret.x = rand() / (float)(RAND_MAX);
   ret.y = rand() / (float)(RAND_MAX);
   ret.z = rand() / (float)(RAND_MAX);
   return ret;
}

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
      vec3(+0.0f, +0.0f, +1.0f),          // normal points out of screen
      
      vec3(+1.0f, -1.0f, RED_TRIANGLE_Z), // right bottom corner
      vec3(1.0f, 1.0f, 0.0f),             // red + green (apparently this makes yellow)
      vec3(+0.0f, +0.0f, +1.0f),          // normal points out of screen

      vec3(+0.0f, +1.0f, -1.0f),          // center top
      vec3(1.0f, 0.0f, 1.0f),             // red + blue (apparently this makes pink
      vec3(+0.0f, +0.0f, +1.0f),          // normal points out of screen

      // second triangle
      vec3(-1.0f, +1.0f, BLUE_TRIANGLE_Z),   // left top corner
      vec3(0.0f, 0.0f, 1.0f),                // all blue
      vec3(+0.0f, +0.0f, +1.0f),          // normal points out of screen

      vec3(+1.0f, +1.0f, BLUE_TRIANGLE_Z),   // right top corner
      vec3(1.0f, 0.0f, 1.0f),                // blue + red
      vec3(+0.0f, +0.0f, +1.0f),          // normal points out of screen

      vec3(+0.0f, -1.0f, BLUE_TRIANGLE_Z),   // center bottom
      vec3(0.0f, 1.0f, 1.0f),                // blue + green
      vec3(+0.0f, +0.0f, +1.0f),          // normal points out of screen
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

   // Note: I made my own shape, but later copied Jamie King's my_shape_generator.cpp.
   my_vertex verts[] =
   {
		vec3(-1.0f, +1.0f, +1.0f),  // 0
		random_color(),
		vec3(+0.0f, +1.0f, +0.0f),  // Normal
		vec3(+1.0f, +1.0f, +1.0f),  // 1
		random_color(),
		vec3(+0.0f, +1.0f, +0.0f),  // Normal
		vec3(+1.0f, +1.0f, -1.0f),  // 2
		random_color(),
		vec3(+0.0f, +1.0f, +0.0f),  // Normal
		vec3(-1.0f, +1.0f, -1.0f),  // 3
		random_color(),
		vec3(+0.0f, +1.0f, +0.0f),  // Normal
			
		vec3(-1.0f, +1.0f, -1.0f),  // 4
		random_color(),
		vec3(+0.0f, +0.0f, -1.0f),  // Normal
		vec3(+1.0f, +1.0f, -1.0f),  // 5
		random_color(),
		vec3(+0.0f, +0.0f, -1.0f),  // Normal
		vec3(+1.0f, -1.0f, -1.0f),  // 6
		random_color(),
		vec3(+0.0f, +0.0f, -1.0f),  // Normal
		vec3(-1.0f, -1.0f, -1.0f),  // 7
		random_color(),
		vec3(+0.0f, +0.0f, -1.0f),  // Normal

		vec3(+1.0f, +1.0f, -1.0f),  // 8
		random_color(),
		vec3(+1.0f, +0.0f, +0.0f),  // Normal
		vec3(+1.0f, +1.0f, +1.0f),  // 9
		random_color(),
		vec3(+1.0f, +0.0f, +0.0f),  // Normal
		vec3(+1.0f, -1.0f, +1.0f),  // 10
		random_color(),
		vec3(+1.0f, +0.0f, +0.0f),  // Normal
		vec3(+1.0f, -1.0f, -1.0f),  // 11
		random_color(),
		vec3(+1.0f, +0.0f, +0.0f),  // Normal

		vec3(-1.0f, +1.0f, +1.0f),  // 12
		random_color(),
		vec3(-1.0f, +0.0f, +0.0f),  // Normal
		vec3(-1.0f, +1.0f, -1.0f),  // 13
		random_color(),
		vec3(-1.0f, +0.0f, +0.0f),  // Normal
		vec3(-1.0f, -1.0f, -1.0f),  // 14
		random_color(),
		vec3(-1.0f, +0.0f, +0.0f),  // Normal
		vec3(-1.0f, -1.0f, +1.0f),  // 15
		random_color(),
		vec3(-1.0f, +0.0f, +0.0f),  // Normal

		vec3(+1.0f, +1.0f, +1.0f),  // 16
		random_color(),
		vec3(+0.0f, +0.0f, +1.0f),  // Normal
		vec3(-1.0f, +1.0f, +1.0f),  // 17
		random_color(),
		vec3(+0.0f, +0.0f, +1.0f),  // Normal
		vec3(-1.0f, -1.0f, +1.0f),  // 18
		random_color(),
		vec3(+0.0f, +0.0f, +1.0f),  // Normal
		vec3(+1.0f, -1.0f, +1.0f),  // 19
		random_color(),
		vec3(+0.0f, +0.0f, +1.0f),  // Normal

		vec3(+1.0f, -1.0f, -1.0f),  // 20
		random_color(),
		vec3(+0.0f, -1.0f, +0.0f),  // Normal
		vec3(-1.0f, -1.0f, -1.0f),  // 21
		random_color(),
		vec3(+0.0f, -1.0f, +0.0f),  // Normal
		vec3(-1.0f, -1.0f, +1.0f),  // 22
		random_color(),
		vec3(+0.0f, -1.0f, +0.0f),  // Normal
		vec3(+1.0f, -1.0f, +1.0f),  // 23
		random_color(),
		vec3(+0.0f, -1.0f, +0.0f),  // Normal
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
      0, 1, 2, 0, 2, 3,       // top
      4, 5, 6, 4, 6, 7,       // front
      8, 9, 10, 8, 10, 11,    // right
      12, 13, 14, 12, 14, 15, // left
      16, 17, 18, 16, 18, 19, // back
      20, 22, 21, 20, 23, 22, // bottom
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

   // Note: I made my own shape, but later copied Jamie King's my_shape_generator.cpp.
   my_vertex verts[] =
   {
		// Top side of arrow head
		vec3(+0.00f, +0.25f, -0.25f),    // 0
		vec3(+1.00f, +0.00f, +0.00f),    // Color
		vec3(+0.00f, +1.00f, +0.00f),    // Normal
		vec3(+0.50f, +0.25f, -0.25f),    // 1
		vec3(+1.00f, +0.00f, +0.00f),    // Color
		vec3(+0.00f, +1.00f, +0.00f),    // Normal
		vec3(+0.00f, +0.25f, -1.00f),    // 2
		vec3(+1.00f, +0.00f, +0.00f),    // Color
		vec3(+0.00f, +1.00f, +0.00f),    // Normal
		vec3(-0.50f, +0.25f, -0.25f),    // 3
		vec3(+1.00f, +0.00f, +0.00f),    // Color
		vec3(+0.00f, +1.00f, +0.00f),    // Normal
		// Bottom side of arrow head     
		vec3(+0.00f, -0.25f, -0.25f),    // 4
		vec3(+0.00f, +0.00f, +1.00f),    // Color
		vec3(+0.00f, -1.00f, +0.00f),    // Normal
		vec3(+0.50f, -0.25f, -0.25f),    // 5
		vec3(+0.00f, +0.00f, +1.00f),    // Color
		vec3(+0.00f, -1.00f, +0.00f),    // Normal
		vec3(+0.00f, -0.25f, -1.00f),    // 6
		vec3(+0.00f, +0.00f, +1.00f),    // Color
		vec3(+0.00f, -1.00f, +0.00f),    // Normal
		vec3(-0.50f, -0.25f, -0.25f),    // 7
		vec3(+0.00f, +0.00f, +1.00f),    // Color
		vec3(+0.00f, -1.00f, +0.00f),    // Normal
		// Right side of arrow tip    
		vec3(+0.50f, +0.25f, -0.25f),    // 8
		vec3(+0.60f, +1.00f, +0.00f),    // Color
		vec3(0.83205032f, 0.00f, -0.55470026f), // Normal
		vec3(+0.00f, +0.25f, -1.00f),    // 9
		vec3(+0.60f, +1.00f, +0.00f),		// Color
		vec3(0.83205032f, 0.00f, -0.55470026f), // Normal
		vec3(+0.00f, -0.25f, -1.00f),    // 10
		vec3(+0.60f, +1.00f, +0.00f),		// Color
		vec3(0.83205032f, 0.00f, -0.55470026f), // Normal
		vec3(+0.50f, -0.25f, -0.25f),    // 11
		vec3(+0.60f, +1.00f, +0.00f),		// Color
		vec3(0.83205032f, 0.00f, -0.55470026f), // Normal
		// Left side of arrow tip
		vec3(+0.00f, +0.25f, -1.00f),    // 12
		vec3(+0.00f, +1.00f, +0.00f),		// Color
		vec3(-0.55708605f, 0.00f, -0.37139067f), // Normal
		vec3(-0.50f, +0.25f, -0.25f),    // 13
		vec3(+0.00f, +1.00f, +0.00f),		// Color
		vec3(-0.55708605f, 0.00f, -0.37139067f), // Normal
		vec3(+0.00f, -0.25f, -1.00f),    // 14
		vec3(+0.00f, +1.00f, +0.00f),		// Color
		vec3(-0.55708605f, 0.00f, -0.37139067f), // Normal
		vec3(-0.50f, -0.25f, -0.25f),    // 15
		vec3(+0.00f, +1.00f, +0.00f),		// Color
		vec3(-0.55708605f, 0.00f, -0.37139067f), // Normal
		// Back side of arrow tip
		vec3(-0.50f, +0.25f, -0.25f),    // 16
		vec3(+0.50f, +0.50f, +0.50f),		// Color
		vec3(+0.00f, +0.00f, +1.00f),    // Normal
		vec3(+0.50f, +0.25f, -0.25f),    // 17
		vec3(+0.50f, +0.50f, +0.50f),		// Color
		vec3(+0.00f, +0.00f, +1.00f),    // Normal
		vec3(-0.50f, -0.25f, -0.25f),    // 18
		vec3(+0.50f, +0.50f, +0.50f),		// Color
		vec3(+0.00f, +0.00f, +1.00f),    // Normal
		vec3(+0.50f, -0.25f, -0.25f),    // 19
		vec3(+0.50f, +0.50f, +0.50f),		// Color
		vec3(+0.00f, +0.00f, +1.00f),    // Normal
		// Top side of back of arrow
		vec3(+0.25f, +0.25f, -0.25f),    // 20
		vec3(+1.00f, +0.00f, +0.00f),		// Color
		vec3(+0.00f, +1.00f, +0.00f),    // Normal
		vec3(+0.25f, +0.25f, +1.00f),    // 21
		vec3(+1.00f, +0.00f, +0.00f),		// Color
		vec3(+0.00f, +1.00f, +0.00f),    // Normal
		vec3(-0.25f, +0.25f, +1.00f),    // 22
		vec3(+1.00f, +0.00f, +0.00f),		// Color
		vec3(+0.00f, +1.00f, +0.00f),    // Normal
		vec3(-0.25f, +0.25f, -0.25f),    // 23
		vec3(+1.00f, +0.00f, +0.00f),		// Color
		vec3(+0.00f, +1.00f, +0.00f),    // Normal
		// Bottom side of back of arrow
		vec3(+0.25f, -0.25f, -0.25f),    // 24
		vec3(+0.00f, +0.00f, +1.00f),		// Color
		vec3(+0.00f, -1.00f, +0.00f),    // Normal
		vec3(+0.25f, -0.25f, +1.00f),    // 25
		vec3(+0.00f, +0.00f, +1.00f),		// Color
		vec3(+0.00f, -1.00f, +0.00f),    // Normal
		vec3(-0.25f, -0.25f, +1.00f),    // 26
		vec3(+0.00f, +0.00f, +1.00f),		// Color
		vec3(+0.00f, -1.00f, +0.00f),    // Normal
		vec3(-0.25f, -0.25f, -0.25f),    // 27
		vec3(+0.00f, +0.00f, +1.00f),		// Color
		vec3(+0.00f, -1.00f, +0.00f),    // Normal
		// Right side of back of arrow
		vec3(+0.25f, +0.25f, -0.25f),    // 28
		vec3(+0.60f, +1.00f, +0.00f),		// Color
		vec3(+1.00f, +0.00f, +0.00f),    // Normal
		vec3(+0.25f, -0.25f, -0.25f),    // 29
		vec3(+0.60f, +1.00f, +0.00f),		// Color
		vec3(+1.00f, +0.00f, +0.00f),    // Normal
		vec3(+0.25f, -0.25f, +1.00f),    // 30
		vec3(+0.60f, +1.00f, +0.00f),		// Color
		vec3(+1.00f, +0.00f, +0.00f),    // Normal
		vec3(+0.25f, +0.25f, +1.00f),    // 31
		vec3(+0.60f, +1.00f, +0.00f),		// Color
		vec3(+1.00f, +0.00f, +0.00f),    // Normal
		// Left side of back of arrow
		vec3(-0.25f, +0.25f, -0.25f),    // 32
		vec3(+0.00f, +1.00f, +0.00f),		// Color
		vec3(-1.00f, +0.00f, +0.00f),    // Normal
		vec3(-0.25f, -0.25f, -0.25f),    // 33
		vec3(+0.00f, +1.00f, +0.00f),		// Color
		vec3(-1.00f, +0.00f, +0.00f),    // Normal
		vec3(-0.25f, -0.25f, +1.00f),    // 34
		vec3(+0.00f, +1.00f, +0.00f),		// Color
		vec3(-1.00f, +0.00f, +0.00f),    // Normal
		vec3(-0.25f, +0.25f, +1.00f),    // 35
		vec3(+0.00f, +1.00f, +0.00f),		// Color
		vec3(-1.00f, +0.00f, +0.00f),    // Normal
		// Back side of back of arrow
		vec3(-0.25f, +0.25f, +1.00f),    // 36
		vec3(+0.50f, +0.50f, +0.50f),		// Color
		vec3(+0.00f, +0.00f, +1.00f),    // Normal
		vec3(+0.25f, +0.25f, +1.00f),    // 37
		vec3(+0.50f, +0.50f, +0.50f),		// Color
		vec3(+0.00f, +0.00f, +1.00f),    // Normal
		vec3(-0.25f, -0.25f, +1.00f),    // 38
		vec3(+0.50f, +0.50f, +0.50f),		// Color
		vec3(+0.00f, +0.00f, +1.00f),    // Normal
		vec3(+0.25f, -0.25f, +1.00f),    // 39
		vec3(+0.50f, +0.50f, +0.50f),		// Color
		vec3(+0.00f, +0.00f, +1.00f),    // Normal   
   };

   GLushort indices[] =
   {
		0, 1, 2, // Top
		0, 2, 3,
		4, 6, 5, // Bottom
		4, 7, 6,
		8, 10, 9, // Right side of arrow tip
		8, 11, 10,
		12, 15, 13, // Left side of arrow tip
		12, 14, 15,
		16, 19, 17, // Back side of arrow tip
		16, 18, 19,
		20, 22, 21, // Top side of back of arrow
		20, 23, 22,
		24, 25, 26, // Bottom side of back of arrow
		24, 26, 27,
		28, 30, 29, // Right side of back of arrow
		28, 31, 30,
		32, 33, 34, // Left side of back of arrow
		32, 34, 35,
		36, 39, 37, // Back side of back of arrow
		36, 38, 39,
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
         this_vert.position.x = col_count - col_half_length;
         this_vert.position.y = 0.0f;
         this_vert.position.z = row_half_length - row_count;
         this_vert.color = random_color();
         this_vert.normal = vec3(+0.0f, +1.0f, +0.0f);
      }
   }

   // 6 indices to draw a square (every adjacent set of 4 vertices (including overlap))
   // Note: 3x3 is a 2x2 set of 4 vertices, 4x4 is a 3x3 set of 4, etc.
   plane.num_indices = (row_max_count - 1) * (col_max_count - 1) * 6;
   plane.indices = new GLushort[plane.num_indices];
   int index_counter = 0;
   for (int row_count = 0; row_count < (row_max_count - 1); row_count++)
   {
      for (int col_count = 0; col_count < (col_max_count - 1); col_count++)
      {
         plane.indices[index_counter++] = row_count * row_max_count + col_count;
         plane.indices[index_counter++] = row_count * row_max_count + (col_count + 1);
         plane.indices[index_counter++] = (row_count + 1) * row_max_count + (col_count + 1);

         plane.indices[index_counter++] = row_count * row_max_count + col_count;
         plane.indices[index_counter++] = (row_count + 1) * row_max_count + (col_count + 1);
         plane.indices[index_counter++] = (row_count + 1) * row_max_count + col_count;
      }
   }

   return plane;
}


my_shape_data my_shape_generator::Jamie_King_makeSphere(unsigned int tesselation)
{
   my_shape_data sphere;

   // longitude line count == latitude line count (somehow this also takes into account the verts at the poles; perhaps there are duplicates at the poles?)
   sphere.num_vertices = (tesselation * tesselation);
   sphere.vertices = new my_vertex[sphere.num_vertices];
   
   unsigned int latitude_lines = tesselation;   // horizontal lines
   unsigned int longitude_lines = tesselation;  // vertical lines

   const float PI = 3.14159265359f;
   const float RADIUS = 1.0f;
   const float CIRCLE_RADIANS = PI * 2;
   const float SLICE_ANGLE = CIRCLE_RADIANS / (tesselation - 1);

   // make the verts
   // NOTE: phi is the angle between the horizontal plane and the Y value, and 
   // theta is the angle in the XZ plane.
   for (unsigned int longitude_counter = 0; longitude_counter < longitude_lines; longitude_counter++)
   {
      float phi = -SLICE_ANGLE * longitude_counter;
      for (unsigned int latitude_counter = 0; latitude_counter < latitude_lines; latitude_counter++)
      {
         float theta = -(SLICE_ANGLE / 2.0f) * latitude_counter;
         size_t vert_index = longitude_counter * tesselation + latitude_counter;
         my_vertex& v = sphere.vertices[vert_index];
         v.position.x = RADIUS * cosf(phi) * sinf(theta);
         v.position.y = RADIUS * sinf(phi) * sinf(theta);
         v.position.z = RADIUS * cosf(theta);
         v.color = random_color();
         v.normal = glm::normalize(v.position);
      }
   }

   // 6 indices for every quad (somehow this also takes into account the points at the poles)
   sphere.num_indices = (tesselation - 1) * (tesselation - 1) * 6;
   sphere.indices = new GLushort[sphere.num_indices];
   int index_counter = 0;

   // piece together the quads of everything but poles like you would with a plane
   for (unsigned int longitude_counter = 0; longitude_counter < (longitude_lines - 1); longitude_counter++)
   {
      for (unsigned int latitude_counter = 0; latitude_counter < (latitude_lines - 1); latitude_counter++)
      {
         // I worked this out on my notepad by hand.  It's a bit much to explain in a comment.
         sphere.indices[index_counter++] = longitude_counter * longitude_lines + latitude_counter;
         sphere.indices[index_counter++] = (longitude_counter + 1) * longitude_lines + (latitude_counter + 1);
         sphere.indices[index_counter++] = longitude_counter * longitude_lines + (latitude_counter + 1);
         
         sphere.indices[index_counter++] = longitude_counter * longitude_lines + latitude_counter;
         sphere.indices[index_counter++] = (longitude_counter + 1) * longitude_lines + (latitude_counter + 1);
         sphere.indices[index_counter++] = (longitude_counter + 1) * longitude_lines + latitude_counter;
      }
   }

	return sphere;
}

// This function took a lot of influence from Jamie's torus, but I refactored a lot of it to make it easier to understand.
my_shape_data my_shape_generator::make_torus(unsigned int tesselation)
{
	my_shape_data torus;

   torus.num_vertices = tesselation * tesselation;
   torus.vertices = new my_vertex[torus.num_vertices];
   
   int slices_per_pipe = tesselation;
   int slices_per_circle = tesselation;
   const float PI = 3.14159265359f;
   float pipe_slice_angle = (PI * 2.0f) / slices_per_pipe;
   float circle_slice_angle = (PI * 2.0f) / slices_per_circle;
   const float torus_radius = 1.0f;
   const float pipe_radius = 0.5f;

   for (int pipe_slice_counter = 0; pipe_slice_counter < slices_per_pipe; pipe_slice_counter++)
   {
      // generate the transform for this circle
      mat4 transform = 
         glm::rotate(mat4(), pipe_slice_counter * pipe_slice_angle, vec3(+0.0f, +1.0f, +0.0f)) *
         glm::translate(mat4(), vec3(torus_radius, +0.0f, +0.0f));

      // generate the circle
      for (int circle_slice_counter = 0; circle_slice_counter < slices_per_circle; circle_slice_counter++)
      {
         my_vertex& v = torus.vertices[pipe_slice_counter * slices_per_circle + circle_slice_counter];
         vec4 circle_vert(
            cosf(circle_slice_angle * circle_slice_counter) * pipe_radius,
            sinf(circle_slice_angle * circle_slice_counter) * pipe_radius,
            0.0f,
            1.0f);

         // now transform each vertex
         v.position = vec3(transform * circle_vert);
         
         v.normal = glm::normalize(v.position);
         v.color = random_color();
      }
   }

   torus.num_indices = tesselation * tesselation * 2 * 3; // 2 triangles per square, 3 indices per triangle
   torus.indices = new unsigned short[torus.num_indices];
   int index_counter = 0;

   // treat the torus as a plane whose edge rows and columns wrap around on each other to make the calculations easier to understand
   for (int row_count = 0; row_count < tesselation; row_count++)
   {
      for (int col_count = 0; col_count < tesselation; col_count++)
      {
         int next_row = (row_count == (tesselation - 1) ? 0 : (row_count + 1));
         int next_col = (col_count == (tesselation - 1) ? 0 : (col_count + 1));

         // remember to make these counter-clockwise so that the face normal will project outward, and therefore culling will work properly
         torus.indices[index_counter++] = row_count * tesselation + col_count;
         torus.indices[index_counter++] = next_row * tesselation + next_col;
         torus.indices[index_counter++] = row_count * tesselation + next_col;

         torus.indices[index_counter++] = row_count * tesselation + col_count;
         torus.indices[index_counter++] = next_row * tesselation + col_count;
         torus.indices[index_counter++] = next_row * tesselation + next_col;
      }
   }

	return torus;
}

my_shape_data my_shape_generator::Jamie_King_makeTeapot(unsigned int tesselation, const glm::mat4& lidTransform)
{
	my_shape_data ret;

	ret.num_vertices = 32 * (tesselation + 1) * (tesselation + 1);
	unsigned int faces = tesselation * tesselation * 32;
	float* vertices = new float[ret.num_vertices * 3];
	float* normals = new float[ret.num_vertices * 3];
	float* textureCoordinates = new float[ret.num_vertices * 2 ];
	ret.num_indices = faces * 6;
	ret.indices = new unsigned short[ret.num_indices];

	generatePatches( vertices, normals, textureCoordinates, ret.indices, tesselation );
	moveLid(tesselation, vertices, lidTransform);

	// Adapt/convert their data format to mine
	ret.vertices = new my_vertex[ret.num_vertices];
	for(unsigned int i = 0; i < ret.num_vertices; i++)
	{
		my_vertex& v = ret.vertices[i];
		v.position.x = vertices[i * 3 + 0];
		v.position.y = vertices[i * 3 + 1];
		v.position.z = vertices[i * 3 + 2];
		v.normal.x = normals[i * 3 + 0];
		v.normal.y = normals[i * 3 + 1];
		v.normal.z = normals[i * 3 + 2];
		v.color = random_color();
	}
	return ret;
}

void my_shape_generator::generatePatches(float * v, float * n, float * tc, unsigned short* el, int grid) {
	float * B = new float[4*(grid+1)];  // Pre-computed Bernstein basis functions
	float * dB = new float[4*(grid+1)]; // Pre-computed derivitives of basis functions

	int idx = 0, elIndex = 0, tcIndex = 0;

	// Pre-compute the basis functions  (Bernstein polynomials)
	// and their derivatives
	computeBasisFunctions(B, dB, grid);

	// Build each patch
	// The rim
	buildPatchReflect(0, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
	// The body
	buildPatchReflect(1, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
	buildPatchReflect(2, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
	// The lid
	buildPatchReflect(3, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
	buildPatchReflect(4, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
	// The bottom
	buildPatchReflect(5, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, true, true);
	// The handle
	buildPatchReflect(6, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
	buildPatchReflect(7, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
	// The spout
	buildPatchReflect(8, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);
	buildPatchReflect(9, B, dB, v, n, tc, el, idx, elIndex, tcIndex, grid, false, true);

	delete [] B;
	delete [] dB;
}

void my_shape_generator::moveLid(int grid, float *v, mat4 lidTransform) {

	int start = 3 * 12 * (grid+1) * (grid+1);
	int end = 3 * 20 * (grid+1) * (grid+1);

	for( int i = start; i < end; i+=3 )
	{
		glm::vec4 vert = glm::vec4(v[i], v[i+1], v[i+2], 1.0f );
		vert = lidTransform * vert;
		v[i] = vert.x;
		v[i+1] = vert.y;
		v[i+2] = vert.z;
	}
}

void my_shape_generator::buildPatchReflect(int patchNum,
									   float *B, float *dB,
									   float *v, float *n,
									   float *tc, unsigned short *el,
									   int &index, int &elIndex, int &tcIndex, int grid,
									   bool reflectX, bool reflectY)
{
	glm::vec3 patch[4][4];
	glm::vec3 patchRevV[4][4];
	getPatch(patchNum, patch, false);
	getPatch(patchNum, patchRevV, true);

	// Patch without modification
	buildPatch(patch, B, dB, v, n, tc, el,
		index, elIndex, tcIndex, grid, mat3(1.0f), true);

	// Patch reflected in x
	if( reflectX ) {
		buildPatch(patchRevV, B, dB, v, n, tc, el,
			index, elIndex, tcIndex, grid, glm::mat3(glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f) ), false );
	}

	// Patch reflected in y
	if( reflectY ) {
		buildPatch(patchRevV, B, dB, v, n, tc, el,
			index, elIndex, tcIndex, grid, glm::mat3(glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f) ), false );
	}

	// Patch reflected in x and y
	if( reflectX && reflectY ) {
		buildPatch(patch, B, dB, v, n, tc, el,
			index, elIndex, tcIndex, grid, glm::mat3(glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f) ), true );
	}
}

void my_shape_generator::buildPatch(glm::vec3 patch[][4],
								float *B, float *dB,
								float *v, float *n, float *tc,
								unsigned short *el,
								int &index, int &elIndex, int &tcIndex, int grid, glm::mat3 reflect,
								bool invertNormal)
{
	int startIndex = index / 3;
	float tcFactor = 1.0f / grid;

	for( int i = 0; i <= grid; i++ )
	{
		for( int j = 0 ; j <= grid; j++)
		{
			glm::vec3 pt = reflect * evaluate(i,j,B,patch);
			glm::vec3 norm = reflect * evaluateNormal(i,j,B,dB,patch);
			if( invertNormal )
				norm = -norm;

			v[index] = pt.x;
			v[index+1] = pt.y;
			v[index+2] = pt.z;

			n[index] = norm.x;
			n[index+1] = norm.y;
			n[index+2] = norm.z;

			tc[tcIndex] = i * tcFactor;
			tc[tcIndex+1] = j * tcFactor;

			index += 3;
			tcIndex += 2;
		}
	}

	for( int i = 0; i < grid; i++ )
	{
		int iStart = i * (grid+1) + startIndex;
		int nextiStart = (i+1) * (grid+1) + startIndex;
		for( int j = 0; j < grid; j++)
		{
			el[elIndex] = iStart + j;
			el[elIndex+1] = nextiStart + j + 1;
			el[elIndex+2] = nextiStart + j;

			el[elIndex+3] = iStart + j;
			el[elIndex+4] = iStart + j + 1;
			el[elIndex+5] = nextiStart + j + 1;

			elIndex += 6;
		}
	}
}

#include "TeapotData.h"

void my_shape_generator::getPatch( int patchNum, glm::vec3 patch[][4], bool reverseV )
{
	for( int u = 0; u < 4; u++) {          // Loop in u direction
		for( int v = 0; v < 4; v++ ) {     // Loop in v direction
			if( reverseV ) {
				patch[u][v] = glm::vec3(
					Teapot::cpdata[Teapot::patchdata[patchNum][u*4+(3-v)]][0],
					Teapot::cpdata[Teapot::patchdata[patchNum][u*4+(3-v)]][1],
					Teapot::cpdata[Teapot::patchdata[patchNum][u*4+(3-v)]][2]
				);
			} else {
				patch[u][v] = glm::vec3(
					Teapot::cpdata[Teapot::patchdata[patchNum][u*4+v]][0],
					Teapot::cpdata[Teapot::patchdata[patchNum][u*4+v]][1],
					Teapot::cpdata[Teapot::patchdata[patchNum][u*4+v]][2]
				);
			}
		}
	}
}

void my_shape_generator::computeBasisFunctions( float * B, float * dB, int grid ) {
	float inc = 1.0f / grid;
	for( int i = 0; i <= grid; i++ )
	{
		float t = i * inc;
		float tSqr = t * t;
		float oneMinusT = (1.0f - t);
		float oneMinusT2 = oneMinusT * oneMinusT;

		B[i*4 + 0] = oneMinusT * oneMinusT2;
		B[i*4 + 1] = 3.0f * oneMinusT2 * t;
		B[i*4 + 2] = 3.0f * oneMinusT * tSqr;
		B[i*4 + 3] = t * tSqr;

		dB[i*4 + 0] = -3.0f * oneMinusT2;
		dB[i*4 + 1] = -6.0f * t * oneMinusT + 3.0f * oneMinusT2;
		dB[i*4 + 2] = -3.0f * tSqr + 6.0f * t * oneMinusT;
		dB[i*4 + 3] = 3.0f * tSqr;
	}
}

glm::vec3 my_shape_generator::evaluate( int gridU, int gridV, float *B, glm::vec3 patch[][4] )
{
	glm::vec3 p(0.0f,0.0f,0.0f);
	for( int i = 0; i < 4; i++) {
		for( int j = 0; j < 4; j++) {
			p += patch[i][j] * B[gridU*4+i] * B[gridV*4+j];
		}
	}
	return p;
}

glm::vec3 my_shape_generator::evaluateNormal( int gridU, int gridV, float *B, float *dB, glm::vec3 patch[][4] )
{
	glm::vec3 du(0.0f,0.0f,0.0f);
	glm::vec3 dv(0.0f,0.0f,0.0f);

	for( int i = 0; i < 4; i++) {
		for( int j = 0; j < 4; j++) {
			du += patch[i][j] * dB[gridU*4+i] * B[gridV*4+j];
			dv += patch[i][j] * B[gridU*4+i] * dB[gridV*4+j];
		}
	}
	return glm::normalize( glm::cross( du, dv ) );
}

