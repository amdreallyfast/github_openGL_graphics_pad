#include "shader_handler.h"

// for reading shader text from files
#include <fstream>
#include <sstream>

// for printing to the console
#include <iostream>
using std::cout;
using std::endl;

shader_handler::shader_handler()
{
}

shader_handler& shader_handler::get_instance()
{
   static shader_handler S;

   return S;
}

bool shader_handler::check_shader_program_status(
   GLuint object_ID,
   PFNGLGETSHADERIVPROC object_property_getter_func_ptr,
   GLenum object_status_type,
   PFNGLGETSHADERINFOLOGPROC info_log_getter_func_ptr
   )
{
   bool this_ret_val = false;

   GLint status;
   object_property_getter_func_ptr(object_ID, object_status_type, &status);
   if (GL_TRUE == status)
   {
      this_ret_val = true;
   }
   else
   {
      // not true, so must be false; get the status error and print it
      this_ret_val = false;

      GLint err_log_len;
      object_property_getter_func_ptr(object_ID, GL_INFO_LOG_LENGTH, &err_log_len);
      GLchar* err_log_buffer = new GLchar[err_log_len];

      // this variable is left over from when people would put a magic number into the error log memory 
      //   allocation, and then the next function would put the actual lenght into this variable instead
      //   of returning anything
      GLsizei actual_err_log_len;
      info_log_getter_func_ptr(object_ID, err_log_len, &actual_err_log_len, err_log_buffer);
      std::cout << err_log_buffer << std::endl;

      delete[] err_log_buffer;
   }

   return this_ret_val;
}


bool shader_handler::check_shader_status(GLuint shader_ID)
{
   bool this_ret_val = false;

   this_ret_val = check_shader_program_status(shader_ID, glGetShaderiv, GL_COMPILE_STATUS, glGetShaderInfoLog);
   if (this_ret_val)
   {
      cout << "shader ID '" << shader_ID << "' compiled ok" << endl;
   }

   return this_ret_val;
}

bool shader_handler::check_program_status(GLuint program_ID)
{
   bool this_ret_val = false;

   this_ret_val = check_shader_program_status(program_ID, glGetProgramiv, GL_LINK_STATUS, glGetProgramInfoLog);
   if (this_ret_val)
   {
      cout << "program ID '" << program_ID << "' linked ok" << endl;
   }

   return this_ret_val;
}

bool shader_handler::read_text_from_file(const char* file_path, std::string &put_text_here)
{
   bool this_ret_val = false;

   std::ifstream in_file_stream;
   in_file_stream.open(file_path, std::ifstream::in);
   if (in_file_stream.is_open())
   {
      this_ret_val = true;

      // credit for the following code goes to "method rdbuf" at 
      // http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
      std::ostringstream contents;
      contents << in_file_stream.rdbuf();
      in_file_stream.close();

      //cout << "shader code in file '" << file_path << "':" << endl;
      //cout << contents.str() << endl;;

      put_text_here = contents.str();
   }
   else
   {
      this_ret_val = false;
      std::cout << "could not open file '" << file_path << "'" << std::endl;
   }

   return this_ret_val;
}

bool shader_handler::install_shaders()
{
   const char* old_GL_double_pointer_adapter[1];

   bool ok_so_far = false;

   // create shader objects
   GLuint vertex_shader_ID = glCreateShader(GL_VERTEX_SHADER);
   GLuint fragment_shader_ID = glCreateShader(GL_FRAGMENT_SHADER);

   // start with the vertex shader and give it the source material, then compile it into an object
   std::string shader_code_text;
   ok_so_far = read_text_from_file("vertex_shader.glsl", shader_code_text);
   if (ok_so_far)
   {
      old_GL_double_pointer_adapter[0] = shader_code_text.c_str();
      glShaderSource(vertex_shader_ID, 1, old_GL_double_pointer_adapter, 0);
      glCompileShader(vertex_shader_ID);
      ok_so_far = check_shader_status(vertex_shader_ID);
   }

   if (ok_so_far)
   {
      // repeat for the fragment shader
      ok_so_far = read_text_from_file("fragment_shader.glsl", shader_code_text);
      old_GL_double_pointer_adapter[0] = shader_code_text.c_str();
      glShaderSource(fragment_shader_ID, 1, old_GL_double_pointer_adapter, 0);
      glCompileShader(fragment_shader_ID);
      ok_so_far = check_shader_status(fragment_shader_ID);
   }

   if (ok_so_far)
   {
      // create a shader executable for the GPU and link the shader objects that have been compiled so far
      m_shader_program_ID = glCreateProgram();
      glAttachShader(m_shader_program_ID, vertex_shader_ID);
      glAttachShader(m_shader_program_ID, fragment_shader_ID);
      glLinkProgram(m_shader_program_ID);
      ok_so_far = check_program_status(m_shader_program_ID);

      // set the context to use the shader program that we just made
      glUseProgram(m_shader_program_ID);
   }

   if (ok_so_far)
   {
      // don't need the shaders anymore, so clean them up 
      // Note: The program is still required.  Clean up that when this class disappears at program end.
      glDeleteShader(vertex_shader_ID);
      glDeleteShader(fragment_shader_ID);
   }


   return ok_so_far;
}

GLuint shader_handler::get_shader_program_ID()
{
   return m_shader_program_ID;
}

shader_handler::~shader_handler()
{
   // tell the GL context to stop using its current program so that we can delete it
   // Note: Nothing will happen if we tell GL to delete a program that is bound to the context.
   glUseProgram(0);
   glDeleteProgram(m_shader_program_ID);
}