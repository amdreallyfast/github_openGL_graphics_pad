#version 430
   
in vec3 vertex_position_world;
in vec3 vertex_normal_world;

uniform vec3 light_position_world;
uniform vec3 camera_position_world;
uniform vec3 ambient_light;

out vec4 final_color;
   
void main()
{
   vec3 light_vector_world = normalize(light_position_world - vertex_position_world);
   vec3 re_n_vector_normal_world = normalize(vertex_normal_world);  // renormalize the "normal" in case a scaling linear transformation messes it up 
   
   // diffuse
   float diffuse_brightness = dot(light_vector_world, re_n_vector_normal_world);
   vec4 diffuse_color = vec4(diffuse_brightness, diffuse_brightness, diffuse_brightness, 1.0f);

   // specular
   // Note: GLSL's reflect(...) function reflects the first vector as if it were incident to the base of the second vector.
   // The light vector in world space is pointing away from the surface and at the light position, so unless it is flipped around
   // then it will be considered to be incident to the underside of surface that the normal represents.
   vec3 reflected_light_vector_world = reflect(-light_vector_world, re_n_vector_normal_world);  
   vec3 camera_vector_world = normalize(camera_position_world - vertex_position_world);
   float specular_brightness = pow(dot(reflected_light_vector_world, camera_vector_world), 32);
   vec4 specular_color = vec4(specular_brightness, specular_brightness, specular_brightness, 1.0f);

   // add the colors together
   final_color = vec4(ambient_light, 0.0f) + clamp(diffuse_color, 0, 1) + clamp(specular_color, 0, 1);
   //final_color = clamp(specular_color, 0, 1);
}

