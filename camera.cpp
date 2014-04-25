#include "camera.h"

#include <glm/glm/glm.hpp>
#include <glm/glm/gtx/transform.hpp>

camera::camera() :
   m_position(0.0f, 0.0f, 0.0f),
   m_world_up_vector(0.0f, 1.0f, 0.0f),
   m_view_direction(0.0f, 0.0f, -1.0f)
{
   // default position of the camera will be world origin
   // the up vector is a direction, and we will say that the up direction is the Y coordinate (it does not technically have to be a unit vector)
   // default view direction will be negative Z direction (this vector must be a unit vector (that is, of length 1))

}

glm::mat4 camera::get_world_to_view_matrix() const
{
   // The glm lookAt(...) function takes vector positions in world coordinates (that is, relative to the world origin).
   // If we want our camera to look in a particular direction relative to our camera, then the second argument must be 
   // the camera's world coordinates + the desired "look at" world coordinates.  
   return glm::lookAt(m_position, m_position + m_view_direction, m_world_up_vector);
}