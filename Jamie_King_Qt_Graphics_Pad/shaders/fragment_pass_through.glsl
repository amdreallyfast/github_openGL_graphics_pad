#version 430
   
in vec3 pass_through_color;
out vec4 final_color;
   
void main()
{
   final_color = pass_through_color;
}

