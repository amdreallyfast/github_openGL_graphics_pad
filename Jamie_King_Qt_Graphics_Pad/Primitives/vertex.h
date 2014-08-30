#pragma once

#include "glm\glm\glm.hpp"

struct my_vertex
{
   static const unsigned int FLOATS_PER_POSITION = 3;
   static const unsigned int FLOATS_PER_COLOR = 3;
   static const unsigned int BYTES_PER_POSITION = 12;
   static const unsigned int BYTES_PER_COLOR = 12;
   static const unsigned int BYTES_PER_VERTEX = BYTES_PER_POSITION + BYTES_PER_COLOR;

   glm::vec3 position;
   glm::vec3 color;
};
