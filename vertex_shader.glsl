#version 430
	
in layout(location = 0) vec4 vert_in_position;
in layout(location = 1) float vert_offset_x;
out vec4 vert_out_color;

void main()
{
   gl_Position = vec4(
      vert_in_position.x + vert_offset_x, 
      vert_in_position.y, 
      vert_in_position.z, 
      vert_in_position.w);
   
   vert_out_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}

