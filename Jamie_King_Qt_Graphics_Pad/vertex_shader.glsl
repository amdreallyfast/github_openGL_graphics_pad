#version 430
	
in layout(location = 0) vec3 vertex_position_model;
in layout(location = 1) vec3 vertex_color;
in layout(location = 2) vec3 vertex_normal_model;

uniform mat4 world_to_projection_matrix;
uniform mat4 model_to_world_matrix;


// these outputs are for the lighting that is done in the fragment shader
out vec3 vertex_position_world;
out vec3 vertex_normal_world;


void main()
{
   // let the position be translatable and the normal not translatable
   vec4 position_world = model_to_world_matrix * vec4(vertex_position_model, 1.0f);  
   vec4 normal_world = model_to_world_matrix * vec4(vertex_normal_model, 0.0f);

   gl_Position = world_to_projection_matrix * position_world;

   vertex_position_world = vec3(position_world);
   vertex_normal_world = vec3(normal_world);
}

