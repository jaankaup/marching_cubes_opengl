#version 430

layout(location = 0) in vec3 in_position;
//layout(location = 1) in vec3 in_blah;

void main()
{
  //gl_Position =  vec4(in_position + in_blah,1.0);
  gl_Position =  vec4(in_position,1.0);
}
