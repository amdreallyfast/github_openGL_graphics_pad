// Note: This header MUST be included before "me GL window", which includes the QT widget header.
// Unhappy header file conflicts happen if glew.h is not included first.
#include <glew-1.10.0\include\GL\glew.h>

// for glm functionality
// Note: #defines must be stated before including the GLM libraries.
#define GLM_FORCE_RADIANS
#include <glm\glm\glm.hpp>
using glm::vec3;
#include <glm\glm\gtc\matrix_transform.hpp>
using glm::mat4;
using glm::translate;
using glm::perspective;
using glm::rotate;

// for reporting errors to console
#include <iostream>
using std::cout;
using std::endl;
#include <iomanip>
using std::setw;
using std::left;
#include <stdio.h>

// for the class that encapsulates the QT window widget
#include "me_GL_window.h"

// for making shapes
#include "Primitives\shape_data.h"
#include "Primitives\shape_generator.h"

#include "utilities\shader_handler.h"


const uint TRIANGLE_NUM_VERTICES = 3;
const uint TRIANGLE_NUM_POSITION_ENTRIES_PER_VERTEX = 4;
const uint TRIANGLE_NUM_COLOR_ENTRIES_PER_VERTEX = 4;
const uint TRIANGLE_NUM_ENTRIES_PER_VERTEX = TRIANGLE_NUM_POSITION_ENTRIES_PER_VERTEX + TRIANGLE_NUM_COLOR_ENTRIES_PER_VERTEX;
const uint TRIANGLE_BYTE_SIZE = TRIANGLE_NUM_VERTICES * (TRIANGLE_NUM_ENTRIES_PER_VERTEX * sizeof(float));
const float TRIANGLE_WIDTH_X = 0.02f;

GLuint num_indices_to_draw;

void send_data_to_open_GL()
{
   // declare some vertices and give them to open GL, and have openGL keep track of them
   //my_shape_data triangle = my_shape_generator::make_double_triangle();
   my_shape_data shape = my_shape_generator::make_cube();

   cout << "number vertices = '" << shape.num_vertices << "'" << endl;

   // make one buffer object to store the vertices in, then put the numbers into it so that they will be stored on the GPU
   //   Note: The text 'GLfloat' cannot describe the third argument in glVertexAttribPointer(...).  
   GLuint vertex_buffer_ID;
   glGenBuffers(1, &vertex_buffer_ID);
   glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_ID);
   glBufferData(GL_ARRAY_BUFFER, shape.vertex_buffer_size(), shape.vertices, GL_STATIC_DRAW);
   glEnableVertexAttribArray(0);

   // arguments are as follows:
   // - index of an enabled vertex attribute array 
   // - the number of items for this vertex attribute
   // - the type of each item for this vertex attribute (must all be identical, and the type is not the same as the type used to declare the item (unfortunately))
   // - boolean to specify whether GL should normalize the items (??what does this mean??); keep "false" until further notice
   // - the number of bytes until the next instance of this vertex attribute appears in the GL_ARRAY_BUFFER object
   // - the offset (in bytes) from the beginning of GL_ARRAY_BUFFER until the first item in this vertex attribute appears
   glVertexAttribPointer(
      0,
      TRIANGLE_NUM_POSITION_ENTRIES_PER_VERTEX,
      GL_FLOAT,
      GL_FALSE,
      (sizeof(GLfloat)* TRIANGLE_NUM_ENTRIES_PER_VERTEX),
      0);
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(
      1, 
      TRIANGLE_NUM_COLOR_ENTRIES_PER_VERTEX, 
      GL_FLOAT, 
      GL_FALSE, 
      (sizeof(GLfloat)* TRIANGLE_NUM_ENTRIES_PER_VERTEX), 
      (char*)(sizeof(GLfloat) * TRIANGLE_NUM_POSITION_ENTRIES_PER_VERTEX)
      );

   GLuint index_buffer_ID;
   glGenBuffers(1, &index_buffer_ID);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_ID);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.index_buffer_size(), shape.indices, GL_STATIC_DRAW);

   num_indices_to_draw = shape.num_indices;

   // take care of any allocated memory
   shape.cleanup();

}


void me_GL_window::initializeGL()
{
   bool ret_val = false;

   // sets up all the open GL pointers 
   glewInit();
   glEnable(GL_DEPTH_TEST);

   send_data_to_open_GL();
   
   shader_handler& shader_thingy = shader_handler::get_instance();
   ret_val = shader_thingy.install_shaders();
   if (!ret_val)
   {
      // something didn't compile or link correctly; ??do something??
      std::cout << "something bad happened during shader initialization" << std::endl;
   }
}

void me_GL_window::paintGL()
{

   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   
   // set the world of viewport dimensions to be the size of the window
   glViewport(0, 0, width(), height());   

   shader_handler& shader_thingy = shader_handler::get_instance();
   GLuint shader_program_ID = shader_thingy.get_shader_program_ID();

   // declare the matrices up front because you will use them multiple times in this function.
   mat4 projection_matrix;
   mat4 translation_matrix;
   mat4 rotation_matrix;
   mat4 full_transform_matrix;

   GLint full_transform_matrix_uniform_location = 0;
   full_transform_matrix_uniform_location = glGetUniformLocation(shader_program_ID, "full_transform_matrix");


   // making the matrices
   // Note: http://glm.g-truc.net/0.9.4/api/a00151.html
   // Find glm::perspective on that web page, and you will see that the first value defaults to degrees,
   // but can be interpreted in radians if GLM_FORCE_RADIANS is defined.
   float fov_radians = (1.0f / 2.0f) * 3.14159f;
   float aspect_ratio = ((float)width()) / ((float)height());
   float near_plane_dist = 0.1f;
   float far_plane_dist = 10.0f;
   projection_matrix = perspective(fov_radians, aspect_ratio, near_plane_dist, far_plane_dist);

   vec3 translation_vector = vec3(1.0f, 0.0f, -3.0f);
   translation_matrix = translate(mat4(), translation_vector);

   float current_rotation_radians = (1.0f / 3.0f) * 3.14159f;
   vec3 rotation_vector = vec3(1.0f, 0.0f, 0.0f);
   rotation_matrix = rotate(mat4(), current_rotation_radians, rotation_vector);

   // make the transform matrix for this cube
   // Note: The order of multiplication is very important.
   // When rotating, GL will automatically rotate around the world's version of the vector specified.
   // Therefore, we rotate first, then translate, then smash it into the perspective range of -1 to +1
   // on x, y, and x.
   // Note: Oddly enough, we rotate first, then translate, then project by multiplying the projection
   // matrix by the translation matrix, then multiplying the result by the rotation matrix.
   full_transform_matrix = projection_matrix * translation_matrix;
   full_transform_matrix *= rotation_matrix;

   // send the transform to the vertex shader and draw this cube
   glUniformMatrix4fv(full_transform_matrix_uniform_location, 1, GL_FALSE, &(full_transform_matrix[0][0]));
   glDrawElements(GL_TRIANGLES, num_indices_to_draw, GL_UNSIGNED_SHORT, 0);


   // do it all again and draw the same vertices transformed to different locations
   // Note: Don't recreate the perspective matrix because that has nothing to do with vertex 
   // translation and rotation.
   translation_vector = vec3(0.0f, -1.0f, -3.75f);
   translation_matrix = translate(mat4(), translation_vector);

   current_rotation_radians = (1.0f / 3.0f) * 3.14159f;
   rotation_vector = vec3(0.0f, 1.0f, 1.0f);
   rotation_matrix = rotate(mat4(), current_rotation_radians, rotation_vector);

   full_transform_matrix = projection_matrix * translation_matrix;
   full_transform_matrix *= rotation_matrix;

   // send the transform to the vertex shader and draw this cube
   glUniformMatrix4fv(full_transform_matrix_uniform_location, 1, GL_FALSE, &(full_transform_matrix[0][0]));
   glDrawElements(GL_TRIANGLES, num_indices_to_draw, GL_UNSIGNED_SHORT, 0);
}

