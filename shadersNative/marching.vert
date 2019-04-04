#version 430

layout(location = 0) in vec3 in_position;

//out vec3 gl_in;

#define MAX_TRIANGLES = 5;

//uniform mat4 MVP;

void main()
{
  //gl_Position =  MVP * vec4(in_position,1);
//  gl_PointSize = 15.0;
  gl_Position =  vec4(in_position,1);
}
