#version 430
	
in layout(location = 0) vec3 vertex_position;
in layout(location = 1) vec3 vertex_color;
in layout(location = 2) vec3 vertex_normal;

// Note: This mat4 actually takes up locations 2, 3, 4, and 5 because
// each row has to be sent as its own vertex attribute object.  There
// is no way to send them all at once, sadly.
//in layout(location = 2) mat4 full_transform_matrix;
uniform mat4 full_transform_matrix;

uniform vec3 ambient_light;
//uniform vec3 light_position;

out vec3 vertex_out_normal;
out vec3 vertex_out_position;  // this is the vertex position so that the fragment shader can interpolate the points on the triangle without having them transformed

void main()
{
   gl_Position = full_transform_matrix * vec4(vertex_position, 1.0f);

   vertex_out_normal = vertex_normal;
   vertex_out_position = vertex_position;
}

