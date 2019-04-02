#version 430

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
layout(triangle_strip, max_vertices = 4) out;

out vec3 fColorIn;

void main(){

	gl_Position =  gl_in[0].gl_Position + vec4(-15.0,0.0,0.0,0.0);
        fColorIn = vec3(0.5,0.3,0.1);
        EmitVertex();

	gl_Position =  gl_in[0].gl_Position + vec4(0.0,13.0,0.0,0.0);
        fColorIn = vec3(0.1,0.2,0.8);
        EmitVertex();

	gl_Position =  gl_in[0].gl_Position + vec4(0.0,0.0,-13.3,0.0);
        fColorIn = vec3(1.0,1.0,1.0);
        EmitVertex();
 
	gl_Position =  gl_in[0].gl_Position + vec4(-15.0,0.0,0.0,0.0);
        fColorIn = vec3(0.5,0.3,0.1);
        EmitVertex();

        EndPrimitive();
}
