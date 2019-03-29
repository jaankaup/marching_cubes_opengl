#version 300 es
//#extension GL_EXT_geometry_shader : enable
//#extension GL_ARB_geometry_shader4 : enable
//#extension OES_shader_io_blocks : enable
#extension OES_geometry_shader : enable
precision highp float;

// The input data is a point. A coordinate for base cube corner. 
layout(points) in;

/*
in gl_PerVertex
{
  vec4 gl_position;
  float gl_PointSize;
  float gl_ClipDistance;
} gl_in[];

The size of gl_in in now 1 because we are using point as input element. 
*/

// The output will be a triangle. 
layout(line_strip, max_vertices = 3) out;

out vec3 color;

void main(){

	gl_Position =  gl_in[0].gl_Position + vec4(-0.1,0.0,0.0,0.0);
        EmitVertex();

	gl_Position =  gl_in[0].gl_Position + vec4(0.1,0.0,0.0,0.0);
        EmitVertex();

	gl_Position =  gl_in[0].gl_Position + vec4(0.0,0.0,0.1,0.0);
        EmitVertex();
 
        EndPrimitive();
}
