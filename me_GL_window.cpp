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


void send_data_to_open_GL()
{
   // make a simple triangle and draw multiple instances of it
   // Note: For this example, all the data will be hard-coded into this function.
   GLfloat triangle_verts[] =
   {
      -1.0f, +0.0f, +0.0f, 1.0f,
      -1.0f, +1.0f, +0.0f, 1.0f,
      -0.9f, +0.0f, +0.0f, 1.0f,
   };

   GLuint vertex_buffer_ID;
   glGenBuffers(1, &vertex_buffer_ID);
   glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_ID);
   glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_verts), triangle_verts, GL_STATIC_DRAW);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
   
   // unbind the array buffer because the vertex attribute object #0 already recorded 
   // where the information was in the vertex buffer object
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   // now make a buffer for offsets
   // Note: We will send this data to the vertex shader one float at time via vertex 
   // attribute object #1.  This data will be close packed, and no offset is required 
   // from the beginning of the array buffer (this is important to know for the last 
   // two arguments of glVertexAttribPointer(...)).
   // Note: No transforms are being done in this version of the program, and we know
   // that the left-most vertex x value in the triangle_verts array is -1.0, so the 
   // most the offset should add is 1.99999 or so to prevent it from surpassing 
   // x = +1.0f, which is outside clipspace for the window.
   GLfloat x_offsets[] = { 0.0f, 0.5f, 1.0f, 1.2, 1.6f };
   GLuint offsets_buffer_ID;
   glGenBuffers(1, &offsets_buffer_ID);
   glBindBuffer(GL_ARRAY_BUFFER, offsets_buffer_ID);
   glBufferData(GL_ARRAY_BUFFER, sizeof(x_offsets), x_offsets, GL_STATIC_DRAW);
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);

   // only send one value from this buffer per element instance (see "draw" call in
   // paintGL() function)
   // Note: This function instructs openGL to send 1 (one) value from vertex attribute
   // object #1 per instance draw.  The default is to send the "number of items to send"
   // (see second argument to glVertexAttribPointer(...)) from the bound GL_ARRAY_BUFFER
   // to the vertex shader on every call to the vertex shader.  We often want this 
   // because we send vertices one by one to the vertex shader to be processes 
   // individually, but in this case, we want one offset every THREE vertices.  The
   // second argument to glVertexAttribPointer(...) can't be 1/3, so the only way to
   // get away with this is to used glDrawElementsInstanced(...) (see paintGL() call) 
   // and specify the number of instances to draw, and use this call in conjunction with
   // glVertexAttribDivisor(...) and specify how many items (second argument) to send 
   // from the vertex attribute object (this is specified in the first argument) per 
   // instanced draw.
   //
   // I hope that makes sense.
   glVertexAttribDivisor(1, 1);
   glBindBuffer(GL_ARRAY_BUFFER, 0);

   GLushort indices[] = { 0, 1, 2 };
   GLuint index_buffer_ID;
   glGenBuffers(1, &index_buffer_ID);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_ID);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
   // ??unbind buffer??
}


void me_GL_window::initializeGL()
{
   bool ret_val = false;

   // sets up all the open GL pointers 
   glewInit();
   glEnable(GL_DEPTH_TEST);

   send_data_to_open_GL();
   
   shader_handler& shader_thingy = shader_handler::get_instance();
   ret_val = shader_thingy.install_shaders();
   if (!ret_val)
   {
      // something didn't compile or link correctly; ??do something??
      std::cout << "something bad happened during shader initialization" << std::endl;
   }
}

void me_GL_window::paintGL()
{

   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
   
   // set the world of viewport dimensions to be the size of the window
   glViewport(0, 0, width(), height());   


   glDrawElementsInstanced(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0, 5);
}

