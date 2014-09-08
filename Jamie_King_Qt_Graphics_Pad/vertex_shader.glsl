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
uniform vec3 light_position;

out vec3 vert_out_color;

void main()
{
   gl_Position = full_transform_matrix * vec4(vertex_position, 1.0f);

   vec3 light_vector = normalize(light_position - vertex_position);

   //vert_out_color = vertex_color * ambient_light;
   float brightness = dot(light_vector, vertex_normal);
   vert_out_color = vec3(brightness, brightness, brightness);
}

