#version 430
	
in layout(location = 0) vec3 vertex_position_model;
in layout(location = 1) vec3 vertex_color;

uniform mat4 world_to_projection_matrix;
uniform mat4 model_to_world_matrix;

out vec3 pass_through_color;

void main()
{
   gl_Position = world_to_projection_matrix * model_to_world_matrix * vec4(vertex_position_model, 1.0f);

   pass_through_color = vertex_color;
}

