#version 430
	
in layout(location = 0) vec4 vert_in_position;
in layout(location = 1) vec4 vert_in_color;
out vec4 vert_out_color;

uniform mat4 full_transform_matrix;

void main()
{
   //gl_Position = vert_in_position;
   
   gl_Position = full_transform_matrix * vert_in_position;
   
   vert_out_color = vert_in_color;
}

