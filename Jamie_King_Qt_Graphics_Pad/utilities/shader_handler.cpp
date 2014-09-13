#include "shader_handler.h"

// for reading shader text from files
#include <fstream>
using std::ifstream;
#include <sstream>
using std::ostringstream;
#include <string>
using std::string;

// for printing to the console
#include <iostream>
using std::cout;
using std::endl;


shader_handler::shader_handler()
{
}

shader_handler::~shader_handler()
{
   // tell the GL context to stop using its current program so that we can delete it
   // Note: Nothing will happen if we tell GL to delete a program that is bound to the context.
   glUseProgram(0);
   glDeleteProgram(m_lighting_shader_program_ID);
}

shader_handler& shader_handler::get_instance()
{
   static shader_handler S;

   return S;
}

bool helper_check_shader_program_status(
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


bool helper_check_shader_status(GLuint shader_ID)
{
   bool this_ret_val = false;

   this_ret_val = helper_check_shader_program_status(shader_ID, glGetShaderiv, GL_COMPILE_STATUS, glGetShaderInfoLog);
   if (this_ret_val)
   {
      cout << "shader ID '" << shader_ID << "' compiled ok" << endl;
   }

   return this_ret_val;
}

bool helper_check_program_status(GLuint program_ID)
{
   bool this_ret_val = false;

   this_ret_val = helper_check_shader_program_status(program_ID, glGetProgramiv, GL_LINK_STATUS, glGetProgramInfoLog);
   if (this_ret_val)
   {
      cout << "program ID '" << program_ID << "' linked ok" << endl;
   }

   return this_ret_val;
}

bool helper_read_text_from_file(const char* file_path, string &put_text_here)
{
   bool this_ret_val = false;

   ifstream in_file_stream;
   in_file_stream.open(file_path, ifstream::in);
   if (in_file_stream.is_open())
   {
      this_ret_val = true;

      // credit for the following code goes to "method rdbuf" at 
      // http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
      ostringstream contents;
      contents << in_file_stream.rdbuf();
      in_file_stream.close();

      //cout << "shader code in file '" << file_path << "':" << endl;
      //cout << contents.str() << endl;;

      put_text_here = contents.str();
   }
   else
   {
      this_ret_val = false;
      cout << "could not open file '" << file_path << "'" << endl;
   }

   return this_ret_val;
}

bool helper_install_shader(GLenum shader_type, const char* shader_file_path, GLuint *put_shader_ID_here)
{
   if (0 == shader_file_path) { return false; }
   else if (0 == put_shader_ID_here) { return false; }

   const char* old_GL_double_pointer_adapter[1];
   bool success = false;
   string shader_code_text;

   // read the text from the shader file
   success = helper_read_text_from_file(shader_file_path, shader_code_text);
   if (!success) { return false; }

   // text was read ok, so now create the shader ID
   GLuint shader_ID = glCreateShader(shader_type);
   old_GL_double_pointer_adapter[0] = shader_code_text.c_str();
   glShaderSource(shader_ID, 1, old_GL_double_pointer_adapter, 0);
   glCompileShader(shader_ID);

   success = helper_check_shader_status(shader_ID);
   if (!success) { return false; }

   // shader creation and compilation successful, so record the shader ID in the provided argument
   *put_shader_ID_here = shader_ID;

   return true;
}

bool helper_install_generic_shader_program(const char *vertex_shader_path, const char *fragment_shader_path, GLuint *put_program_ID_here)
{
   bool success = false;

   // create shader objects
   GLuint vertex_shader_ID;
   GLuint fragment_shader_ID;

   success = helper_install_shader(GL_VERTEX_SHADER, vertex_shader_path, &vertex_shader_ID);
   if (!success) { return false; }
   success = helper_install_shader(GL_FRAGMENT_SHADER, fragment_shader_path, &fragment_shader_ID);
   if (!success)
   {
      glDeleteShader(vertex_shader_ID);
      return false;
   }

   // create a shader executable for the GPU and link the shader objects that have been compiled so far
   GLuint shader_program_ID = glCreateProgram();
   glAttachShader(shader_program_ID, vertex_shader_ID);
   glAttachShader(shader_program_ID, fragment_shader_ID);
   glLinkProgram(shader_program_ID);
   success = helper_check_program_status(shader_program_ID);
   if (!success)
   {
      glDeleteShader(vertex_shader_ID);
      glDeleteShader(fragment_shader_ID);
      return false;
   }

   // the shaders compiled and linked ok, so put the program ID in the provided argument
   *put_program_ID_here = shader_program_ID;

   // don't need the shaders anymore, so clean them up 
   // Note: The program is still required.  Clean up that when this class disappears at program end.
   glDeleteShader(vertex_shader_ID);
   glDeleteShader(fragment_shader_ID);

   return success;
}

bool shader_handler::install_shaders()
{
   bool success = false;

   success = helper_install_generic_shader_program(
      "shaders/vertex_lighting.glsl",
      "shaders/fragment_lighting.glsl",
      &m_lighting_shader_program_ID);
   if (!success) { return false; }
   
   glUseProgram(m_lighting_shader_program_ID);
   m_current_shader_program_ID = m_lighting_shader_program_ID;



   return success;
}


void shader_handler::activate_pass_through_shader_program()
{
   
}

void shader_handler::activate_lighting_shader_program()
{
   glUseProgram(m_current_shader_program_ID);
}

GLint shader_handler::get_uniform_location(const char* uniform_name_as_string)
{
   return glGetUniformLocation(m_current_shader_program_ID, uniform_name_as_string);
}



