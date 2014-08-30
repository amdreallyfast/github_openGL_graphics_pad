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
float g_rotation_angle_radians = 0.0f;
bool g_mouse_is_pressed = false;


// The buffer IDs are as follows:
// - vertex buffer: stores position and color data
// - index buffer: stores indices of position/color combos in the vertex buffer
// - vertex array object: stores vertex attrib array and pointer attribues, relieving you of the burden of having to re-specify them manually on every draw call if you are drawing from different vertex buffers

GLuint g_vertex_buffer_ID;
GLuint g_index_buffer_ID;

GLuint g_cube_vertex_array_object_ID;
GLuint g_cube_num_indices = 0;

GLuint g_arrow_vertex_array_object_ID;
GLuint g_arrow_num_indices = 0;
GLuint g_arrow_index_byte_offset = 0;

GLuint g_transformation_matrix_buffer_ID;
GLuint g_transformation_matrix_vertex_array_object_ID;
GLint g_transform_matrix_uniform_location;


me_GL_window::~me_GL_window()
{
   glDeleteBuffers(1, &g_vertex_buffer_ID);
   glDeleteBuffers(1, &g_index_buffer_ID);
}


void me_GL_window::initializeGL()
{
   bool ret_val = false;

   setMouseTracking(true);

   // sets up all the open GL pointers 
   glewInit();
   glEnable(GL_DEPTH_TEST);

   // initialize the things to be drawn
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
   float far_plane_dist = 20.0f;
   mat4 projection_matrix = perspective(fov_radians, aspect_ratio, near_plane_dist, far_plane_dist);
   mat4 world_to_projection_matrix = projection_matrix * g_camera.get_world_to_view_matrix();

   mat4 full_transform_matrix;

   // cube
   //glBindVertexArray(g_cube_vertex_array_object_ID);
   //mat4 cube_1_model_to_world_matrix =
   //   translate(mat4(), vec3(1.0f, 0.0f, +1.0f)) *
   //   rotate(mat4(), (1.0f / 6.0f) * 3.14159f, vec3(0.0f, 1.0f, 1.0f));
   //full_transform_matrix = world_to_projection_matrix * cube_1_model_to_world_matrix;
   //glUniformMatrix4fv(g_transform_matrix_uniform_location, 1, GL_FALSE, &full_transform_matrix[0][0]);
   //glDrawElements(GL_TRIANGLES, g_cube_num_indices, GL_UNSIGNED_SHORT, 0);

   glBindVertexArray(g_cube_vertex_array_object_ID);
   mat4 cube_2_model_to_world_matrix =
      translate(mat4(), vec3(0.0f, -1.0f, +0.0f)) *
      rotate(mat4(), g_rotation_angle_radians, vec3(0.0f, 0.0f, 1.0f));
   full_transform_matrix = world_to_projection_matrix * cube_2_model_to_world_matrix;
   glUniformMatrix4fv(g_transform_matrix_uniform_location, 1, GL_FALSE, &full_transform_matrix[0][0]);
   glDrawElements(GL_TRIANGLES, g_cube_num_indices, GL_UNSIGNED_SHORT, 0);


   // arrow
   glBindVertexArray(g_arrow_vertex_array_object_ID);
   mat4 arrow_1_model_to_world_matrix =
      translate(mat4(), vec3(1.0f, -1.0f, -5.0f)) *
      rotate(mat4(), (0.0f / 3.0f) * 3.14159f, vec3(0.0f, 0.0f, 1.0f));
   full_transform_matrix = world_to_projection_matrix * arrow_1_model_to_world_matrix;
   glUniformMatrix4fv(g_transform_matrix_uniform_location, 1, GL_FALSE, &full_transform_matrix[0][0]);
   glDrawElements(GL_TRIANGLES, g_arrow_num_indices, GL_UNSIGNED_SHORT, reinterpret_cast<void *>(g_arrow_index_byte_offset));

   //GLenum e = glGetError();
   //cout << "GL error: " << e << endl;

   glBindVertexArray(0);
}

void me_GL_window::mouseMoveEvent(QMouseEvent * e)
{
   static glm::vec2 prev_mouse_position(0.0f, 0.0f);

   float new_x = e->x();
   float new_y = e->y();

   if (g_mouse_is_pressed)
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
   g_mouse_is_pressed = true;
   cout << "mouse clicked" << endl;
}

void me_GL_window::mouseReleaseEvent(QMouseEvent*)
{
   g_mouse_is_pressed = false;
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

void me_GL_window::send_data_to_open_GL()
{
   my_shape_data cube = my_shape_generator::Jamie_King_makeTeapot(10, mat4());
   g_cube_num_indices = cube.num_indices;

   my_shape_data arrow = my_shape_generator::Jamie_King_makeSphere();
   g_arrow_num_indices = arrow.num_indices;

   // create the buffer objects for vertex and index data
   glGenBuffers(1, &g_vertex_buffer_ID);
   glBindBuffer(GL_ARRAY_BUFFER, g_vertex_buffer_ID);
   glBufferData(GL_ARRAY_BUFFER, cube.vertex_buffer_size() + arrow.vertex_buffer_size(), 0, GL_STATIC_DRAW);
   glBufferSubData(GL_ARRAY_BUFFER, 0, cube.vertex_buffer_size(), cube.vertices);
   glBufferSubData(GL_ARRAY_BUFFER, cube.vertex_buffer_size(), arrow.vertex_buffer_size(), arrow.vertices);

   glGenBuffers(1, &g_index_buffer_ID);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_index_buffer_ID);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube.index_buffer_size() + arrow.index_buffer_size(), 0, GL_STATIC_DRAW);
   glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, cube.index_buffer_size(), cube.indices);
   glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, cube.index_buffer_size(), arrow.index_buffer_size(), arrow.indices);
   g_arrow_index_byte_offset = cube.index_buffer_size();
   

   // create the vertex array objects
   // Note: OpenGL does not actually create the buffer when it "generates" it.  Only the ID is generated.
   // The buffer is not created (that is, memory is not set aside for it) until the vertex array object ID 
   // is bound for the first time.
   glGenVertexArrays(1, &g_cube_vertex_array_object_ID);
   glGenVertexArrays(1, &g_arrow_vertex_array_object_ID);

   void *buffer_start_offset;

   glBindVertexArray(g_cube_vertex_array_object_ID);
   glBindBuffer(GL_ARRAY_BUFFER, g_vertex_buffer_ID);
   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);
   buffer_start_offset = 0;
   glVertexAttribPointer(0, my_vertex::FLOATS_PER_POSITION, GL_FLOAT, GL_FALSE, my_vertex::BYTES_PER_VERTEX, buffer_start_offset);
   buffer_start_offset = reinterpret_cast<void *>(my_vertex::BYTES_PER_POSITION);
   glVertexAttribPointer(1, my_vertex::FLOATS_PER_COLOR, GL_FLOAT, GL_FALSE, my_vertex::BYTES_PER_VERTEX, buffer_start_offset);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_index_buffer_ID);

   glBindVertexArray(g_arrow_vertex_array_object_ID);
   glBindBuffer(GL_ARRAY_BUFFER, g_vertex_buffer_ID);
   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);
   buffer_start_offset = reinterpret_cast<void *>(cube.vertex_buffer_size());
   glVertexAttribPointer(0, my_vertex::FLOATS_PER_POSITION, GL_FLOAT, GL_FALSE, my_vertex::BYTES_PER_VERTEX, buffer_start_offset);
   buffer_start_offset = reinterpret_cast<void *>(cube.vertex_buffer_size() + my_vertex::BYTES_PER_POSITION);
   glVertexAttribPointer(1, my_vertex::FLOATS_PER_COLOR, GL_FLOAT, GL_FALSE, my_vertex::BYTES_PER_VERTEX, buffer_start_offset);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_index_buffer_ID);

   glBindVertexArray(0);


   // take care of any allocated memory
   cube.cleanup();
   arrow.cleanup();
}


