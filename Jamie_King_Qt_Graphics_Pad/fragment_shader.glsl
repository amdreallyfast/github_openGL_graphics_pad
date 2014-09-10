#version 430
   
in vec3 vertex_out_normal;
in vec3 vertex_out_position;
uniform vec3 light_position;

out vec4 out_color;
   
void main()
{
   vec3 light_vector = normalize(light_position - vertex_out_position);
   float brightness = dot(light_vector, vertex_out_normal);
   out_color = vec4(brightness, brightness, brightness, 1.0f);
}

