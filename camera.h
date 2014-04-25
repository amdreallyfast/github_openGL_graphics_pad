#pragma once

#include <glm\glm\glm.hpp>

class my_camera
{
public:
   my_camera();
   glm::mat4 get_world_to_view_matrix() const;
   void mouse_update(const glm::vec2& new_mouse_position);

private:
   glm::vec3 m_world_up_vector;
   glm::vec3 m_position;
   glm::vec3 m_view_direction;
   glm::vec2 m_prev_mouse_position;
};

