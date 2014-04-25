#include "camera.h"

// if you want to work in radians, you have to #define this in every single source file BEFORE they include
// the neccessary glm files because the source files are all independent of each other; the #define in the
// me_GL_window.cpp file does not carry over to this source file (you could include it in a common header 
// or something
#define GLM_FORCE_RADIANS
#include <glm/glm/glm.hpp>
#include <glm/glm/gtx/transform.hpp>

#include <iostream>
using std::cout;
using std::endl;

my_camera::my_camera() :
   m_position(0.0f, 0.0f, 0.0f),
   m_world_up_vector(0.0f, 1.0f, 0.0f),
   m_view_direction(0.0f, 0.0f, -1.0f),
   m_prev_mouse_position(0.0f, 0.0f)
{
   // default position of the camera will be world origin
   
   // the up vector is a direction, and we will say that the up direction is the Y coordinate (it does not technically have to be a unit vector)
   
   // default view direction will be negative Z direction (this vector must be a unit vector (that is, of length 1))

   // default previous mouse position will be the world origin
}

glm::mat4 my_camera::get_world_to_view_matrix() const
{
   // The glm lookAt(...) function takes vector positions in world coordinates (that is, relative to the world origin).
   // If we want our camera to look in a particular direction relative to our camera, then the second argument must be 
   // the camera's world coordinates + the desired "look at" world coordinates.  The view direction will be treated (by 
   // me, the programmer) as a unit vector because I want to think of it as a direction even though the vector math
   // turns out to be same as if it were a position.
   return glm::lookAt(m_position, m_position + m_view_direction, m_world_up_vector);
}

void my_camera::mouse_update(const glm::vec2& new_mouse_position)
{
   glm::vec2 mouse_delta = new_mouse_position - m_prev_mouse_position;

   // we need to perform a rotation (??why??) on the view direction position, so force the glm rotate(...)
   // function's return value (a mat4) to be a mat3, which will cut off the 4th row and column, which we
   // are not concerned with because the view direction is a unit vector anyway (??is this a valid argument??)
   // Note: We are working in radians, so consider that when calculating your rotation angle.
   float rotate_angle_x_radians = mouse_delta.x * (2.0f * 3.14159f) / 360.0f;
   float rotation_angle_x_degrees = mouse_delta.x / 5;
   m_view_direction = glm::mat3(glm::rotate(rotate_angle_x_radians, m_world_up_vector)) * m_view_direction;

   m_prev_mouse_position = new_mouse_position;
}

