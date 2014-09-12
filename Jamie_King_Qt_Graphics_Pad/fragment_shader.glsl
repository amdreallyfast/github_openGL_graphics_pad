#version 430
   
in vec3 vertex_position_world;
in vec3 vertex_normal_world;
uniform vec3 light_position_world;
uniform vec3 ambient_light;

out vec4 final_color;
   
void main()
{
   vec3 light_vector = normalize(light_position_world - vertex_position_world);
   float brightness = dot(light_vector, vertex_normal_world);
   vec4 diffuse_color = vec4(brightness, brightness, brightness, 1.0f);

   final_color = clamp(diffuse_color, 0, 1) + vec4(ambient_light, 0.0f);
}

