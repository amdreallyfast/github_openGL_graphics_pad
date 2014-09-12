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

GLuint g_teapot_vertex_array_object_ID;
GLuint g_teapot_num_indices = 0;
GLuint g_teapot_index_byte_offset = 0;

GLuint g_torus_vertex_array_object_ID;
GLuint g_torus_num_indices = 0;
GLuint g_torus_index_byte_offset = 0;

GLuint g_plane_vertex_array_object_ID;
GLuint g_plane_num_indices = 0;
GLuint g_plane_index_byte_offset = 0;

GLint g_world_to_projection_matrix_uniform_location;
GLint g_model_to_world_matrix_uniform_location;
GLint g_ambient_light_uniform_location;
GLint g_diffuse_light_uniform_location;




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
   //glEnable(GL_CULL_FACE);

   // initialize the things to be drawn
   send_data_to_open_GL();

   shader_handler& shader_thingy = shader_handler::get_instance();
   ret_val = shader_thingy.install_shaders();
   if (!ret_val)
   {
      // something didn't compile or link correctly; ??do something??
      std::cout << "something bad happened during shader initialization" << std::endl;
   }

   g_world_to_projection_matrix_uniform_location =
      glGetUniformLocation(shader_thingy.get_shader_program_ID(), "world_to_projection_matrix");
   g_model_to_world_matrix_uniform_location =
      glGetUniformLocation(shader_thingy.get_shader_program_ID(), "model_to_world_matrix");
   g_ambient_light_uniform_location =
      glGetUniformLocation(shader_thingy.get_shader_program_ID(), "ambient_light");
   g_diffuse_light_uniform_location =
      glGetUniformLocation(shader_thingy.get_shader_program_ID(), "light_position_world");
}


void me_GL_window::paintGL()
{
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   glViewport(0, 0, width(), height());

   // set up light
   vec3 ambient_light(0.05f, 0.05f, 0.05f);
   glUniform3f(g_ambient_light_uniform_location, 
      ambient_light.r,
      ambient_light.g,
      ambient_light.b);

   vec3 light_position(0.0f, +1.0f, +0.0f);
   glUniform3f(g_diffuse_light_uniform_location,
      light_position.x,
      light_position.y,
      light_position.z);


   // set up the transformation matrix 
   // Note: The calls to width() and height() are only useful on startup because they are not being
   // calculated on every call to paintGL(), so the aspect ratio will get screwed up when you resize.
   float fov_radians = (1.0f / 2.0f) * 3.14159f;
   float aspect_ratio = ((float)this->width()) / ((float)this->height());
   float near_plane_dist = 0.1f;
   float far_plane_dist = 20.0f;
   mat4 projection_matrix = perspective(fov_radians, aspect_ratio, near_plane_dist, far_plane_dist);
   mat4 world_to_projection_matrix = projection_matrix * g_camera.get_world_to_view_matrix();
   glUniformMatrix4fv(g_world_to_projection_matrix_uniform_location, 1, GL_FALSE, &world_to_projection_matrix[0][0]);

   mat4 model_to_world_matrix = mat4();

   // plane 
   // Note: No transformation for the plane, but it does require the matrix to be specified so that it 
   // doesn't have another object's transformation applied to itself.
   glBindVertexArray(g_plane_vertex_array_object_ID);
   glUniformMatrix4fv(g_model_to_world_matrix_uniform_location, 1, GL_FALSE, &model_to_world_matrix[0][0]);
   glDrawElements(GL_TRIANGLES, g_plane_num_indices, GL_UNSIGNED_SHORT, (void *)(g_plane_index_byte_offset));

   // teapot
   // Note: Rotate the teapot so that it is right-side-up (the mathematically generated model assumes +Z as postive vertical)
   glBindVertexArray(g_teapot_vertex_array_object_ID);
   model_to_world_matrix = 
      translate(mat4(), vec3(+3.0f, +1.0f, +1.0f)) *
      rotate(mat4(), -(3.14159f / 2.0f), vec3(1.0f, 0.0f, 0.0f)) * 
      rotate(mat4(), g_rotation_angle_radians, vec3(0.0f, 0.0f, 1.0f));
   glUniformMatrix4fv(g_model_to_world_matrix_uniform_location, 1, GL_FALSE, &model_to_world_matrix[0][0]);
   glDrawElements(GL_TRIANGLES, g_teapot_num_indices, GL_UNSIGNED_SHORT, 0);

   // torus
   glBindVertexArray(g_torus_vertex_array_object_ID);
   model_to_world_matrix = 
      translate(mat4(), vec3(-3.0f, +1.0f, +3.0f)) *
      rotate(mat4(), (0.0f / 3.0f) * 3.14159f, vec3(0.0f, 0.0f, 1.0f));
   glUniformMatrix4fv(g_model_to_world_matrix_uniform_location, 1, GL_FALSE, &model_to_world_matrix[0][0]);
   glDrawElements(GL_TRIANGLES, g_torus_num_indices, GL_UNSIGNED_SHORT, (void *)(g_torus_index_byte_offset));

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
      // rotate a teapot
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
   my_shape_data teapot = my_shape_generator::Jamie_King_makeTeapot(20, mat4());
   g_teapot_num_indices = teapot.num_indices;

   my_shape_data torus = my_shape_generator::make_torus(50);
   g_torus_num_indices = torus.num_indices;

   my_shape_data plane = my_shape_generator::make_plane(20);
   g_plane_num_indices = plane.num_indices;

   int buffer_start_offset = 0;

   // create the buffer objects for vertex and index data
   glGenBuffers(1, &g_vertex_buffer_ID);
   glGenBuffers(1, &g_index_buffer_ID);
   glBindBuffer(GL_ARRAY_BUFFER, g_vertex_buffer_ID);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_index_buffer_ID);
   glBufferData(GL_ARRAY_BUFFER,
      teapot.vertex_buffer_size() + 
      torus.vertex_buffer_size() + 
      plane.vertex_buffer_size(), 
      0, GL_STATIC_DRAW);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
      teapot.index_buffer_size() + 
      torus.index_buffer_size() + 
      plane.index_buffer_size(), 
      0, GL_STATIC_DRAW);

   // send the vertex data
   buffer_start_offset = 0;
   glBufferSubData(GL_ARRAY_BUFFER, buffer_start_offset, teapot.vertex_buffer_size(), teapot.vertices);
   buffer_start_offset += teapot.vertex_buffer_size();
   glBufferSubData(GL_ARRAY_BUFFER, buffer_start_offset, torus.vertex_buffer_size(), torus.vertices);
   buffer_start_offset += torus.vertex_buffer_size();
   glBufferSubData(GL_ARRAY_BUFFER, buffer_start_offset, plane.vertex_buffer_size(), plane.vertices);

   // send the index data
   buffer_start_offset = 0;
   glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, buffer_start_offset, teapot.index_buffer_size(), teapot.indices);
   buffer_start_offset += teapot.index_buffer_size();
   glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, buffer_start_offset, torus.index_buffer_size(), torus.indices);
   buffer_start_offset += torus.index_buffer_size();
   glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, buffer_start_offset, plane.index_buffer_size(), plane.indices);
   g_torus_index_byte_offset = teapot.index_buffer_size();
   g_plane_index_byte_offset = teapot.index_buffer_size() + torus.index_buffer_size();
   

   // create the vertex array objects
   // Note: OpenGL does not actually create the buffer when it "generates" it.  Only the ID is generated.
   // The buffer is not created (that is, memory is not set aside for it) until the vertex array object ID 
   // is bound for the first time.
   glGenVertexArrays(1, &g_teapot_vertex_array_object_ID);
   glGenVertexArrays(1, &g_torus_vertex_array_object_ID);
   glGenVertexArrays(1, &g_plane_vertex_array_object_ID);

   // the teapot
   glBindVertexArray(g_teapot_vertex_array_object_ID);
   glBindBuffer(GL_ARRAY_BUFFER, g_vertex_buffer_ID);
   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);
   glEnableVertexAttribArray(2);
   buffer_start_offset = 0;
   glVertexAttribPointer(0, my_vertex::FLOATS_PER_POSITION, GL_FLOAT, GL_FALSE, my_vertex::BYTES_PER_VERTEX, (void *)buffer_start_offset);
   buffer_start_offset += my_vertex::BYTES_PER_POSITION;
   glVertexAttribPointer(1, my_vertex::FLOATS_PER_COLOR, GL_FLOAT, GL_FALSE, my_vertex::BYTES_PER_VERTEX, (void *)buffer_start_offset);
   buffer_start_offset += my_vertex::BYTES_PER_COLOR;
   glVertexAttribPointer(2, my_vertex::FLOATS_PER_NORMAL, GL_FLOAT, GL_FALSE, my_vertex::BYTES_PER_VERTEX, (void *)buffer_start_offset);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_index_buffer_ID);

   // the torus
   glBindVertexArray(g_torus_vertex_array_object_ID);
   glBindBuffer(GL_ARRAY_BUFFER, g_vertex_buffer_ID);
   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);
   glEnableVertexAttribArray(2);
   buffer_start_offset = teapot.vertex_buffer_size();
   glVertexAttribPointer(0, my_vertex::FLOATS_PER_POSITION, GL_FLOAT, GL_FALSE, my_vertex::BYTES_PER_VERTEX, (void *)buffer_start_offset);
   buffer_start_offset += my_vertex::BYTES_PER_POSITION;
   glVertexAttribPointer(1, my_vertex::FLOATS_PER_COLOR, GL_FLOAT, GL_FALSE, my_vertex::BYTES_PER_VERTEX, (void *)buffer_start_offset);
   buffer_start_offset += my_vertex::BYTES_PER_COLOR;
   glVertexAttribPointer(2, my_vertex::FLOATS_PER_NORMAL, GL_FLOAT, GL_FALSE, my_vertex::BYTES_PER_VERTEX, (void *)buffer_start_offset);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_index_buffer_ID);

   // the plane
   glBindVertexArray(g_plane_vertex_array_object_ID);
   glBindBuffer(GL_ARRAY_BUFFER, g_vertex_buffer_ID);
   glEnableVertexAttribArray(0);
   glEnableVertexAttribArray(1);
   glEnableVertexAttribArray(2);
   buffer_start_offset = teapot.vertex_buffer_size() + torus.vertex_buffer_size();
   glVertexAttribPointer(0, my_vertex::FLOATS_PER_POSITION, GL_FLOAT, GL_FALSE, my_vertex::BYTES_PER_VERTEX, (void *)buffer_start_offset);
   buffer_start_offset += my_vertex::BYTES_PER_POSITION;
   glVertexAttribPointer(1, my_vertex::FLOATS_PER_COLOR, GL_FLOAT, GL_FALSE, my_vertex::BYTES_PER_VERTEX, (void *)buffer_start_offset);
   buffer_start_offset += my_vertex::BYTES_PER_COLOR;
   glVertexAttribPointer(2, my_vertex::FLOATS_PER_NORMAL, GL_FLOAT, GL_FALSE, my_vertex::BYTES_PER_VERTEX, (void *)buffer_start_offset);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_index_buffer_ID);


   glBindVertexArray(0);


   // take care of any allocated memory
   teapot.cleanup();
   torus.cleanup();
   plane.cleanup();
}


