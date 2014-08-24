#pragma once

#include "glm\glm\glm.hpp"

struct my_vertex
{
   static const unsigned int NUM_FLOATS_PER_POSITION = 4;
   static const unsigned int NUM_FLOATS_PER_COLOR = 4;

   // we are using vec4 structures, each of which have 4 floats, each of which is 32bits (4 bytes)
   static const unsigned int SIZE_BYTES_PER_POSITION = 16;
   static const unsigned int SIZE_BYTES_PER_COLOR = 16;
   static const unsigned int SIZE_BYTES_PER_VERTEX = 32;

   glm::vec4 position;
   glm::vec4 color;
};
