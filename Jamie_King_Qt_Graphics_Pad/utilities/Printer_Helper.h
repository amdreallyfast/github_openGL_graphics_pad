#ifndef ENGINE_PRINTER_HELPER_H
#define ENGINE_PRINTER_HELPER_H

#include <glm\glm\vec3.hpp>
#include <glm\glm\mat4x4.hpp>

namespace Utilities
{
   class __declspec(dllexport) Printer_Helper
   {
   public:
      static void print_vec(const char *foreword, const glm::vec3 &vec);
      static void print_mat(const char *foreword, const glm::mat4 & mat);

   private:
      // enforce staticness
      Printer_Helper();
      Printer_Helper(const Printer_Helper &);
      Printer_Helper &operator=(const Printer_Helper &);
   };
}

#endif