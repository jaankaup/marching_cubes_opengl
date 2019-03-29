#version 300 es
precision highp float;

layout(location = 0) in vec3 in_position;

//out vec3 gl_in;

#define MAX_TRIANGLES = 5;

uniform mat4 MVP;

struct marchingTriangles {
       vec3 position;
       vec3 color;
       vec3 materialSpecularColor;
       float ambientCoeffience;
       float materialShininess;
       float attentuationFactor;
};

void main()
{
  gl_Position =  MVP * vec4(in_position,1);
}
