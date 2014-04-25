#pragma once

#include <glm\glm\glm.hpp>

class camera
{
public:
   camera();
   glm::mat4 get_world_to_view_matrix() const;

private:
   glm::vec3 m_world_up_vector;
   glm::vec3 m_position;
   glm::vec3 m_view_direction;
};

