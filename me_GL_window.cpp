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

// for our camera
// Note: QT's "mouse event" file has no extension.
#include "camera.h"
#include <QtGui/qmouseevent>


GLsizei num_indices_to_draw = 0;
my_camera Camera;

GLuint transformation_matrix_buffer_ID;


void me_GL_window::send_data_to_open_GL()
{
   my_shape_data shape = my_shape_generator::make_cube();

   GLuint vertex_buffer_ID;
   glGenBuffers(1, &vertex_buffer_ID);
   glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_ID);
   glBufferData(GL_ARRAY_BUFFER, shape.vertex_buffer_size(), shape.vertices, GL_STATIC_DRAW);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(
      0,
      shape.num_position_entries_per_vertex,
      GL_FLOAT,
      GL_FALSE,
      shape.size_bytes_per_vertex,
      0
      );

   glEnableVertexAttribArray(1);
   glVertexAttribPointer(
      1, 
      shape.num_color_entries_per_vertex, 
      GL_FLOAT, 
      GL_FALSE, 
      shape.size_bytes_per_vertex, 
      (char*)(shape.size_bytes_per_position_vertex)
      );

   GLuint index_buffer_ID;
   glGenBuffers(1, &index_buffer_ID);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_ID);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.index_buffer_size(), shape.indices, GL_STATIC_DRAW);

   num_indices_to_draw = shape.num_indices;

   // take care of any allocated memory
   shape.cleanup();

   // we are going to be sending two transformation 4x4 matrices to the vertex shader, but we don't
   // want to send the data until the mouse starts moving

   // Note: mat4 objects are specified in the vertex attribute object by row, so 
   // they are sent in 4 sets of 4 floats.  Unfortunately, they cannot be sent 
   // as a whole with a single attribute object (for example, you could not 
   // specify the type as "4 * GL_FLOAT").  To send them, you have to send a row
   // in its own vertex attribute object, which means that a matrix takes up 4 
   // attribute objects.
   transformation_matrix_buffer_ID;
   glGenBuffers(1, &transformation_matrix_buffer_ID);
   glBindBuffer(GL_ARRAY_BUFFER, transformation_matrix_buffer_ID);
   glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * 2, 0, GL_DYNAMIC_DRAW);
   glEnableVertexAttribArray(2);
   glEnableVertexAttribArray(3);
   glEnableVertexAttribArray(4);
   glEnableVertexAttribArray(5);

   // stride = size of one entry in the full_transforms buffer
   // offset for each row = size of a row * row number 
   glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float) * 0));
   glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float) * 4));
   glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float) * 8));
   glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void*)(sizeof(float) * 12));

   // send down one matrix per draw instance
   glVertexAttribDivisor(2, 1);
   glVertexAttribDivisor(3, 1);
   glVertexAttribDivisor(4, 1);
   glVertexAttribDivisor(5, 1);

   // clean up
   //glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void me_GL_window::initializeGL()
{
   bool ret_val = false;

   setMouseTracking(true);

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

float g_rotation_angle_radians = 0.0f;

void me_GL_window::paintGL()
{
   // now set up the transformation matrix buffer
   // Note: The calls to width() and height() are only useful on startup because they are not being
   // calculated on every call to paintGL(), so the aspect ratio will get screwed up when you resize.
   float fov_radians = (1.0f / 2.0f) * 3.14159f;
   float aspect_ratio = ((float)this->width()) / ((float)this->height());
   float near_plane_dist = 0.1f;
   float far_plane_dist = 10.0f;
   mat4 projection_matrix = perspective(fov_radians, aspect_ratio, near_plane_dist, far_plane_dist);
   mat4 full_transforms[] =
   {
      //projection_matrix * Camera.get_world_to_view_matrix() * translate(mat4(), vec3(1.0f, 0.0f, -3.0f)) * rotate(mat4(), (1.0f / 3.0f) * 3.14159f, vec3(1.0f, 0.0f, 0.0f)),
      projection_matrix * Camera.get_world_to_view_matrix() * translate(mat4(), vec3(1.0f, 0.0f, -3.0f)) * rotate(mat4(), g_rotation_angle_radians, vec3(1.0f, 0.0f, 0.0f)),
      projection_matrix * Camera.get_world_to_view_matrix() * translate(mat4(), vec3(0.0f, -1.0f, -3.75f)) * rotate(mat4(), (1.0f / 6.0f) * 3.14159f, vec3(0.0f, 1.0f, 1.0f)),
   };

   //glBindBuffer(GL_ARRAY_BUFFER, transformation_matrix_buffer_ID);
   glBufferData(GL_ARRAY_BUFFER, sizeof(full_transforms), full_transforms, GL_DYNAMIC_DRAW);

   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   
   // set the world of viewport dimensions to be the size of the window
   glViewport(0, 0, width(), height());   

   glDrawElementsInstanced(GL_TRIANGLES, num_indices_to_draw, GL_UNSIGNED_SHORT, 0, 2);

   //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool mouse_is_pressed = false;

void me_GL_window::mouseMoveEvent(QMouseEvent * e)
{
   static glm::vec2 prev_mouse_position(0.0f, 0.0f);
   
   float new_x = e->x();
   float new_y = e->y();

   if (mouse_is_pressed)
   {
      // rotate a cube
      glm::vec2 mouse_delta = glm::vec2(new_x, new_y) - prev_mouse_position;
      g_rotation_angle_radians = mouse_delta.x * (2.0f * 3.14159f) / 360.0f;
   }
   else
   {
      // rotate camera
      prev_mouse_position.x = new_x;
      prev_mouse_position.y = new_y;
      Camera.mouse_update(glm::vec2(new_x, new_y));
   }

   this->repaint();
}

void me_GL_window::mousePressEvent(QMouseEvent* e)
{
   mouse_is_pressed = true;
   cout << "mouse clicked" << endl;
}

void me_GL_window::mouseReleaseEvent(QMouseEvent*)
{
   mouse_is_pressed = false;
   cout << "mouse released" << endl;
}
