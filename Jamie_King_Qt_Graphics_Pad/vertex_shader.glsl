#version 430
	
in layout(location = 0) vec3 vert_in_position;
in layout(location = 1) vec3 vert_in_color;

// Note: This mat4 actually takes up locations 2, 3, 4, and 5 because
// each row has to be sent as its own vertex attribute object.  There
// is no way to send them all at once, sadly.
//in layout(location = 2) mat4 full_transform_matrix;
uniform mat4 full_transform_matrix;

out vec3 vert_out_color;

void main()
{
   gl_Position = full_transform_matrix * vec4(vert_in_position, 1.0f);

   vert_out_color = vert_in_color;
}

