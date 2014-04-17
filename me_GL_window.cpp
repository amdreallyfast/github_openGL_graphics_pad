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

   // making the matrices
   // Note: Perform a simple transformation 3 units in the negative Z direction.
   // Make a 60 degree perspective with an aspect ratio the same as the QTwidget window.
   // Note: http://glm.g-truc.net/0.9.4/api/a00151.html
   // Find glm::perspective on that web page, and you will see that the first value defaults to degrees,
   // but can be interpreted in radians if GLM_FORCE_RADIANS is defined.
   float fov_radians = (1.0f / 3.0f) * 3.14159f;
   float aspect_ratio = ((float)width()) / ((float)height());
   float near_plane_dist = 0.1f;
   float far_plane_dist = 10.0f;
   mat4 projection_matrix = perspective(fov_radians, aspect_ratio, near_plane_dist, far_plane_dist);
   //float fov_degrees = 60.0f;
   //mat4 projection_matrix = perspective(fov_degrees, aspect_ratio, near_plane_dist, far_plane_dist);

   vec3 translation_vector = vec3(0.0f, 0.0f, -3.0f);
   mat4 translation_matrix = translate(projection_matrix, translation_vector);

   static float current_rotation_radians = 0.0f;
   static float rotation_increment_radians = (1.0f / 64.0f) * 3.14159;
   vec3 rotation_vector = vec3(1.0f, 0.0f, 0.0f);
   mat4 full_transform_matrix = rotate(translation_matrix, current_rotation_radians += rotation_increment_radians, rotation_vector);

   //static float current_rotation_degrees = 0.0f;
   //static float rotation_increment_degrees = 1.0f;
   //mat4 full_transform_matrix = rotate(translation_matrix, current_rotation_degrees += rotation_increment_degrees, rotation_vector);

   shader_handler& shader_thingy = shader_handler::get_instance();
   GLuint shader_program_ID = shader_thingy.get_shader_program_ID();

   GLint full_transform_matrix_uniform_location = 0;
   full_transform_matrix_uniform_location = glGetUniformLocation(shader_program_ID, "full_transform_matrix");
   glUniformMatrix4fv(full_transform_matrix_uniform_location, 1, GL_FALSE, &(full_transform_matrix[0][0]));


   glDrawElements(GL_TRIANGLES, num_indices_to_draw, GL_UNSIGNED_SHORT, 0);
}

