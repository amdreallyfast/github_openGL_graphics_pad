#version 430
	
in layout(location = 0) vec3 vertex_position;
in layout(location = 1) vec3 vertex_color;
in layout(location = 2) vec3 vertex_normal;

uniform mat4 world_to_projection_matrix;
uniform mat4 model_to_world_matrix;

uniform vec3 ambient_light;

// these outputs are for the lighting that is done in the fragment shader
out vec3 vertex_out_normal;
out vec3 vertex_out_position;  // this is for the fragment shader so that it can interpolate the points on the triangle for each angle calculation

void main()
{
   vec4 position_vec4 = vec4(vertex_position, 1.0f);  // let the position be translatable
   vec4 normal_vec4 = vec4(vertex_normal, 0.0f);      // do not let the normal be translated

   gl_Position = world_to_projection_matrix * model_to_world_matrix * position_vec4;

   vertex_out_normal = vec3(model_to_world_matrix * normal_vec4);
   vertex_out_position = vec3(model_to_world_matrix * position_vec4);
}

