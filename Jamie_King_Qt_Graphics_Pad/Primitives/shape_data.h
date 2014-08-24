#pragma once

#include "vertex.h"
#include <glew-1.10.0\include\GL\glew.h>

struct my_shape_data
{
   my_shape_data()
   {
      vertices = 0;
      num_vertices = 0;
      indices = 0;
      num_indices = 0;
   }

   void cleanup()
   {
      delete[] vertices;
      delete[] indices;
      num_vertices = 0;
      num_indices = 0;
   }

   // use a GLsizeiptr because that is what GL wants for glBufferData(...)
   GLsizeiptr vertex_buffer_size() const
   {
      return (num_vertices * sizeof(*vertices));
   }

   // use a GLsizeiptr because that is what GL wants for glBufferData(...)
   GLsizeiptr index_buffer_size() const
   {
      return (num_indices * sizeof(*indices));
   }

   my_vertex* vertices;
   GLuint num_vertices;
   GLushort *indices;
   GLuint num_indices;
};

