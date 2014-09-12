#pragma once

// these inclusions are for parameter typing
#include <glew-1.10.0\include\GL\glew.h>
#include <string>

class shader_handler
{
public:
   static shader_handler& get_instance();
   bool install_shaders();
   void activate_pass_through_shader_program();
   void activate_lighting_shader_program();
   GLint get_uniform_location(const char* uniform_name_as_string);

   ~shader_handler();

private:
   // for singleton-ness
   shader_handler();

   //bool check_shader_program_status(
   //   GLuint object_ID,
   //   PFNGLGETSHADERIVPROC object_property_getter_func_ptr,
   //   GLenum object_status_type,
   //   PFNGLGETSHADERINFOLOGPROC info_log_getter_func_ptr
   //   );
   //bool check_shader_status(GLuint shader_ID);
   //bool check_program_status(GLuint program_ID);
   //bool read_text_from_file(const char* file_path, std::string &put_text_here);

   GLuint m_current_shader_program_ID;
   GLuint m_pass_through_shader_program_ID;
   GLuint m_lighting_shader_program_ID;
};


