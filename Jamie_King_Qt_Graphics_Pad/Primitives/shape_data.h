#pragma once

#include "vertex.h"
#include <glew-1.10.0\include\GL\glew.h>

struct my_shape_data
{
   my_shape_data()
   {
      vertices = 0;
      num_vertices = 0;
      num_entries_per_vertex = 0;
      num_position_entries_per_vertex = 0;
      num_color_entries_per_vertex = 0;
      size_bytes_per_vertex = 0;
      size_bytes_per_position_vertex = 0;
      size_bytes_per_color_vertex = 0;
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
   GLuint num_entries_per_vertex;
   GLuint num_position_entries_per_vertex;
   GLuint num_color_entries_per_vertex;
   GLsizei size_bytes_per_vertex;
   GLsizei size_bytes_per_position_vertex;
   GLsizei size_bytes_per_color_vertex;
   GLushort *indices;
   GLuint num_indices;
};