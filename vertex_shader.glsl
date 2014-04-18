#version 430
	
in layout(location = 0) vec4 vert_in_position;
in layout(location = 1) vec4 vert_in_color;

// Note: This mat4 actually takes up locations 2, 3, 4, and 5 because
// each row has to be sent as its own vertex attribute object.  There
// is no way to send them all at once, sadly.
in layout(location = 2) mat4 full_transform_matrix;

out vec4 vert_out_color;

void main()
{
   gl_Position = full_transform_matrix * vert_in_position;
   
   vert_out_color = vert_in_color;
}

