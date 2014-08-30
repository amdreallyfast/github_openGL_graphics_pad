#version 430
   
in vec3 vert_out_color;
   
out vec4 frag_out_color;
   
void main()
{
   frag_out_color = vec4(vert_out_color, 1.0f);
}

