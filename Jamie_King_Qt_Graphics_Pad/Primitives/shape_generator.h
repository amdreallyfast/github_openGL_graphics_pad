#pragma once

#include "Primitives\shape_data.h"

class my_shape_generator
{
public:
   static my_shape_data make_double_triangle();
   static my_shape_data make_cube();
   static my_shape_data make_3d_arrow();
   static my_shape_data make_plane(unsigned int dimensions = 10);
   static my_shape_data Jamie_King_makeSphere(unsigned int tesselation = 20);
   static my_shape_data make_torus(unsigned int tesselation = 20);
   static my_shape_data Jamie_King_makeTeapot(unsigned int tesselation, const glm::mat4& lidTransform);

   private:
      // Teapot helpers
      // From Jamie King's adaptation of Mark Kilgard's Teapot generation research.
      static void generatePatches(float * v, float * n, float * tc, unsigned short* el, int grid);
      static void moveLid(int grid, float *v, glm::mat4 lidTransform);
      static void buildPatchReflect(int patchNum,
                        float *B, float *dB,
                        float *v, float *n,
                        float *tc, unsigned short *el,
                        int &index, int &elIndex, int &tcIndex, int grid,
                        bool reflectX, bool reflectY);
      static void buildPatch(glm::vec3 patch[][4],
                  float *B, float *dB,
                  float *v, float *n, float *tc,
                  unsigned short *el,
                  int &index, int &elIndex, int &tcIndex, 
                  int grid, glm::mat3 reflect,
                  bool invertNormal);
      static void getPatch( int patchNum, glm::vec3 patch[][4], bool reverseV);
      static void computeBasisFunctions( float * B, float * dB, int grid);
      static glm::vec3 evaluate( int gridU, int gridV, float *B, glm::vec3 patch[][4]);
      static glm::vec3 evaluateNormal( int gridU, int gridV, 
                          float *B, float *dB, glm::vec3 patch[][4]);
};

