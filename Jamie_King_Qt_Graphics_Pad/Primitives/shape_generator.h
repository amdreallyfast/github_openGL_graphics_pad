#pragma once

#include "Primitives\shape_data.h"

class my_shape_generator
{
public:
   static my_shape_data make_double_triangle();
   static my_shape_data make_cube();
   static my_shape_data make_3d_arrow();
};

