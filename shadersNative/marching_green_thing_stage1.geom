#version 430

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// First step to optimize the green thing data. Output all case numbers.  //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

layout(points) in;

layout(triangle_strip, max_vertices = 3) out;

// layout(binding=0, offset=0) uniform atomic_uint potential_cube_count;

out float outputCase;

uniform vec3 startPoint;
uniform sampler3D diffuse3DTexture;
uniform sampler1D tri_table;
uniform float voxels_per_block;
//uniform mat4 MVP;

struct Cube
{
  vec4 v0;
  vec4 v1;
  vec4 v2;
  vec4 v3;
  vec4 v4;
  vec4 v5;
  vec4 v6;
  vec4 v7;
};

float calculateDensity(vec3 v)
{
//  vec3 again = vec3(mod(v.x,8.0) - 4.0,mod(v.y,15.0f),mod(v.z,2.0f));
  float ball = pow(v.x,2.0) + pow(v.y,2.0) + pow(v.z+3.0,2.0) - pow(2.2,2.0);  
  float noise = texture(diffuse3DTexture,v).w;
  float noise2 = texture(diffuse3DTexture,v+vec3(0.2,0.1,0.3)).w;
  float noise3 = texture(diffuse3DTexture,v+vec3(sin(v.x),tan(v.y),cos(v.z))).w;
  float hils = noise2*33.0*sin(v.z);
  float hils2 = noise*13.0*cos(v.z);

 return ball + hils + hils2 - noise2*35.0 - noise*(30*sin(v.z*v.x));
// else return 0.0;
 // float noise = texture(diffuse3DTexture,v).w;
 // float noise2 = texture(diffuse3DTexture,v+vec3(0.2,0.1,0.3)).w;
//  float circle = clamp(noise * pow(again.x*again.x + again.y*again.y, 2.0),-12.0,20.0) - 2.0;

//  return v.y + noise + circle + hils - hils2 + 20.0*v.y + 3.0 * noise ; // + circle; // - circle2;
 //   return v.y + noise - hils + hils2;
}

Cube createCube(vec3 position)
{
  // The length of a cube edge.
  float d = 1/voxels_per_block;

  // Create cube corner coordinates.
  vec3 p0 = position*d;
  vec3 p1 = position*d + vec3(0.0 ,   d , 0.0);
  vec3 p2 = position*d + vec3(d   ,   d , 0.0);
  vec3 p3 = position*d + vec3(d   , 0.0 , 0.0);
  vec3 p4 = position*d + vec3(0.0 , 0.0 , d);
  vec3 p5 = position*d + vec3(0.0 ,   d , d);
  vec3 p6 = position*d + vec3(d   ,   d , d);
  vec3 p7 = position*d + vec3(d   , 0.0 , d);

  vec4 v0 = vec4(p0, calculateDensity(p0));  
  vec4 v1 = vec4(p1, calculateDensity(p1));  
  vec4 v2 = vec4(p2, calculateDensity(p2));  
  vec4 v3 = vec4(p3, calculateDensity(p3));  
  vec4 v4 = vec4(p4, calculateDensity(p4));  
  vec4 v5 = vec4(p5, calculateDensity(p5));  
  vec4 v6 = vec4(p6, calculateDensity(p6));  
  vec4 v7 = vec4(p7, calculateDensity(p7));  

  // Create the cube.
  Cube cube;
  cube.v0 = v0;
  cube.v1 = v1;
  cube.v2 = v2;
  cube.v3 = v3;
  cube.v4 = v4;
  cube.v5 = v5;
  cube.v6 = v6;
  cube.v7 = v7;

  return cube; 
}

float calculateCase(Cube c)
{
  float result = 0.0;
  
  if (c.v7.w < 0.5) { result += 128.0;} 
  if (c.v6.w < 0.5) { result += 64.0;}
  if (c.v5.w < 0.5) { result += 32.0;} 
  if (c.v4.w < 0.5) { result += 16.0;} 
  if (c.v3.w < 0.5) { result += 8.0; }
  if (c.v2.w < 0.5) { result += 4.0; }
  if (c.v1.w < 0.5) { result += 2.0; }
  if (c.v0.w < 0.5) { result += 1.0; }
  
  return result;
} 

void main(){

        vec3 pos = startPoint + gl_in[0].gl_Position.xyz;
        Cube c = createCube(pos);
//        outputCase = 123.0;
        float theCase = calculateCase(c);
        if (theCase == 0.0 || theCase == 255.0) return;
        
        outputCase = gl_in[0].gl_Position.x;
        EmitVertex();
        outputCase = gl_in[0].gl_Position.y;
        EmitVertex();
        outputCase = gl_in[0].gl_Position.z;
        EmitVertex();
        EndPrimitive();
}
