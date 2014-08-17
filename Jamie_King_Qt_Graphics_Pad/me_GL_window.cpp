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
#include <QtGui/qkeyevent>

my_camera g_camera;

// The buffer IDs are as follows:
// - vertex buffer: stores position and color data
// - index buffer: stores indices of position/color combos in the vertex buffer
// - vertex array object: stores vertex attrib array and pointer attribues, relieving you of the burden of having to re-specify them manually on every draw call if you are drawing from different vertex buffers

my_shape_data g_cube;
GLuint g_cube_vertex_buffer_ID;
GLuint g_cube_index_buffer_ID;
GLuint g_cube_vertex_array_object_ID;
GLuint g_cube_num_indices = 0;

my_shape_data g_arrow;
GLuint g_arrow_vertex_buffer_ID;
GLuint g_arrow_index_buffer_ID;
GLuint g_arrow_vertex_array_object_ID;
GLuint g_arrow_num_indices = 0;

GLuint g_transformation_matrix_buffer_ID;
GLuint g_transformation_matrix_vertex_array_object_ID;
GLint g_transform_matrix_uniform_location;


void me_GL_window::send_data_to_open_GL()
{
   // cube
   g_cube = my_shape_generator::make_cube();
   g_cube_num_indices = g_cube.num_indices;

   glGenBuffers(1, &g_cube_vertex_buffer_ID);
   glBindBuffer(GL_ARRAY_BUFFER, g_cube_vertex_buffer_ID);
   glBufferData(GL_ARRAY_BUFFER, g_cube.vertex_buffer_size(), g_cube.vertices, GL_STATIC_DRAW);

   glGenBuffers(1, &g_cube_index_buffer_ID);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_cube_index_buffer_ID);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_cube.index_buffer_size(), g_cube.indices, GL_STATIC_DRAW);

   // oddly enough, the "gen vertex array" only generates an ID, while the "bind" call sets aside the array memory if it hasn't been done yet
   //glGenVertexArrays(1, &g_cube_vertex_array_object_ID);
   //glBindVertexArray(g_cube_vertex_array_object_ID);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, g_cube.num_position_entries_per_vertex, GL_FLOAT, GL_FALSE, g_cube.size_bytes_per_vertex, 0);
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, g_cube.num_color_entries_per_vertex, GL_FLOAT, GL_FALSE, g_cube.size_bytes_per_vertex, (char*)(g_cube.size_bytes_per_position_vertex));

   // take care of any allocated memory
   g_cube.cleanup();


   // arrow
   g_arrow = my_shape_generator::make_3d_arrow();
   g_arrow_num_indices = g_arrow.num_indices;
   glGenBuffers(1, &g_arrow_vertex_buffer_ID);
   glBindBuffer(GL_ARRAY_BUFFER, g_arrow_vertex_buffer_ID);
   GLsizeiptr x = g_arrow.vertex_buffer_size();
   glBufferData(GL_ARRAY_BUFFER, g_arrow.vertex_buffer_size(), g_arrow.vertices, GL_STATIC_DRAW);

   glGenBuffers(1, &g_arrow_index_buffer_ID);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_arrow_index_buffer_ID);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_arrow.index_buffer_size(), g_arrow.indices, GL_STATIC_DRAW);

   //glGenVertexArrays(1, &g_arrow_vertex_array_object_ID);
   //glBindVertexArray(g_arrow_vertex_array_object_ID);
   glEnableVertexAttribArray(0);
   //glVertexAttribPointer(0, g_arrow.num_position_entries_per_vertex, GL_FLOAT, GL_FALSE, g_arrow.size_bytes_per_position_vertex, 0);
   glEnableVertexAttribArray(1);
   //glVertexAttribPointer(1, g_arrow.num_color_entries_per_vertex, GL_FLOAT, GL_FALSE, g_arrow.size_bytes_per_color_vertex, (char*)(g_arrow.size_bytes_per_position_vertex));

   g_arrow.cleanup();
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

   g_transform_matrix_uniform_location = 
      glGetUniformLocation(shader_thingy.get_shader_program_ID(), "full_transform_matrix");
}

float g_rotation_angle_radians = 0.0f;

void me_GL_window::paintGL()
{
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   glViewport(0, 0, width(), height());

   // set up the transformation matrix 
   // Note: The calls to width() and height() are only useful on startup because they are not being
   // calculated on every call to paintGL(), so the aspect ratio will get screwed up when you resize.
   float fov_radians = (1.0f / 2.0f) * 3.14159f;
   float aspect_ratio = ((float)this->width()) / ((float)this->height());
   float near_plane_dist = 0.1f;
   float far_plane_dist = 10.0f;
   mat4 projection_matrix = perspective(fov_radians, aspect_ratio, near_plane_dist, far_plane_dist);
   mat4 world_to_projection_matrix = projection_matrix * g_camera.get_world_to_view_matrix();
   
   mat4 full_transform_matrix;


   // cube
   glBindBuffer(GL_ARRAY_BUFFER, g_cube_vertex_buffer_ID);
   glVertexAttribPointer(0, g_cube.num_position_entries_per_vertex, GL_FLOAT, GL_FALSE, g_cube.size_bytes_per_vertex, 0);
   glVertexAttribPointer(1, g_cube.num_color_entries_per_vertex, GL_FLOAT, GL_FALSE, g_cube.size_bytes_per_vertex, (char*)(g_cube.size_bytes_per_position_vertex));
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_cube_index_buffer_ID);

   mat4 cube_1_model_to_world_matrix =
      translate(mat4(), vec3(1.0f, 0.0f, -9.0f)) *
      rotate(mat4(), (1.0f / 6.0f) * 3.14159f, vec3(0.0f, 1.0f, 1.0f));
   full_transform_matrix = world_to_projection_matrix * cube_1_model_to_world_matrix;
   glUniformMatrix4fv(g_transform_matrix_uniform_location, 1, GL_FALSE, &full_transform_matrix[0][0]);
   glDrawElements(GL_TRIANGLES, g_cube_num_indices, GL_UNSIGNED_SHORT, 0);

   mat4 cube_2_model_to_world_matrix =
      translate(mat4(), vec3(0.0f, -1.0f, -3.75f)) *
      rotate(mat4(), g_rotation_angle_radians, vec3(0.0f, 0.0f, 1.0f));
   full_transform_matrix = world_to_projection_matrix * cube_2_model_to_world_matrix;
   glUniformMatrix4fv(g_transform_matrix_uniform_location, 1, GL_FALSE, &full_transform_matrix[0][0]);
   glDrawElements(GL_TRIANGLES, g_cube_num_indices, GL_UNSIGNED_SHORT, 0);


   // arrow
   glBindBuffer(GL_ARRAY_BUFFER, g_arrow_vertex_buffer_ID);
   glVertexAttribPointer(0, g_arrow.num_position_entries_per_vertex, GL_FLOAT, GL_FALSE, g_arrow.size_bytes_per_vertex, 0);
   glVertexAttribPointer(1, g_arrow.num_color_entries_per_vertex, GL_FLOAT, GL_FALSE, g_arrow.size_bytes_per_vertex, (char*)(g_arrow.size_bytes_per_position_vertex));
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_arrow_index_buffer_ID);

   mat4 arrow_1_model_to_world_matrix =
      translate(mat4(), vec3(1.0f, -1.0f, -9.0f)) *
      rotate(mat4(), (1.0f / 3.0f) * 3.14159f, vec3(0.0f, 0.0f, 1.0f));
   full_transform_matrix = world_to_projection_matrix * arrow_1_model_to_world_matrix;
   glUniformMatrix4fv(g_transform_matrix_uniform_location, 1, GL_FALSE, &full_transform_matrix[0][0]);
   glDrawElements(GL_TRIANGLES, g_arrow_num_indices, GL_UNSIGNED_SHORT, 0);


   GLenum e = glGetError();
   cout << "GL error: " << e << endl;

   //glBindBuffer(GL_ARRAY_BUFFER, 0);
   //glBindVertexArray(0);
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
      g_camera.mouse_update(glm::vec2(new_x, new_y));
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

void me_GL_window::keyPressEvent(QKeyEvent* e)
{
   switch (e->key())
   {
   case Qt::Key::Key_W:
      g_camera.move_forward();
      //g_rotation_angle_radians += 0.1f;
      break;
   case Qt::Key::Key_A:
      g_camera.strafe_left();
      break;
   case Qt::Key::Key_S:
      g_camera.move_back();
      //g_rotation_angle_radians -= 0.1f;
      break;
   case Qt::Key::Key_D:
      g_camera.strafe_right();
      break;
   case Qt::Key::Key_R:
      g_camera.move_up();
      break;
   case Qt::Key::Key_F:
      g_camera.move_down();
      break;

   default:
      break;
   }

   this->repaint();
}
