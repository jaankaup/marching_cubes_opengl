#version 430

layout(points) in;

layout(line_strip, max_vertices = 30) out;

out vec3 fColorIn;
uniform vec3 startPoint;
uniform sampler3D diffuse3DTexture;
uniform sampler1D tri_table;
uniform float voxels_per_block;
uniform mat4 MVP;
uniform float cubeMask;

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

Cube createCube(vec4 position)
{
  // The length of a cube edge.
  float d = 1/voxels_per_block;

  // Create cube corner coordinates.
  vec3 p0 = position.xyz*d;
  vec3 p1 = position.xyz*d + vec3(0.0 ,   d , 0.0);
  vec3 p2 = position.xyz*d + vec3(d   ,   d , 0.0);
  vec3 p3 = position.xyz*d + vec3(d   , 0.0 , 0.0);
  vec3 p4 = position.xyz*d + vec3(0.0 , 0.0 , d);
  vec3 p5 = position.xyz*d + vec3(0.0 ,   d , d);
  vec3 p6 = position.xyz*d + vec3(d   ,   d , d);
  vec3 p7 = position.xyz*d + vec3(d   , 0.0 , d);

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

void printCube(Cube c, float mask, bool printMask)
{
    float pSize = 1.0;
//    float maskColor = float(mask) / float(255);
    vec3 maskColor = vec3(1.0);

    // FRONT sideFront = v0 v1 v2 v3
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v0.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v0.w); }
    else { fColorIn = maskColor;}
          EmitVertex();

    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v1.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v1.w); }
    else { fColorIn = maskColor;}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v2.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v2.w); }
    else { fColorIn = maskColor;}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v3.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v3.w); }
    else { fColorIn = maskColor;}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v0.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v0.w); }
    else { fColorIn = maskColor;}
          EmitVertex();
    EndPrimitive();

    // TOP sideFront = v0 v5 v6 v2
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v1.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v1.w); }
    else { fColorIn = maskColor;}
          EmitVertex();

    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v5.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v5.w); }
    else { fColorIn = maskColor;}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v6.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v6.w); }
    else { fColorIn = maskColor;}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v2.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v2.w); }
    else { fColorIn = maskColor;}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v1.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v1.w); }
    else { fColorIn = maskColor;}
          EmitVertex();
    EndPrimitive();

    // RIGHT sideRight = v3 v2 v6 v7
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v3.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v3.w); }
    else { fColorIn = maskColor;}
          EmitVertex();

    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v2.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v2.w); }
    else { fColorIn = maskColor;}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v6.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v6.w); }
    else { fColorIn = maskColor;}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v7.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v7.w); }
    else { fColorIn = maskColor;}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v3.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v3.w); }
    else { fColorIn = maskColor;}
          EmitVertex();
    EndPrimitive();

    // LEFT sideLeft = v0 v1 v5 v4
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v0.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v0.w); }
    else { fColorIn = maskColor;}
          EmitVertex();

    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v1.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v1.w); }
    else { fColorIn = maskColor;}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v5.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v5.w); }
    else { fColorIn = maskColor;}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v4.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v4.w); }
    else { fColorIn = maskColor;}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v0.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v0.w); }
    else { fColorIn = maskColor;}
          EmitVertex();
    EndPrimitive();

    // BACK sideBack = v4 v5 v6 v7

    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v4.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v4.w); }
    else { fColorIn = maskColor;}
          EmitVertex();

    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v5.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v5.w); }
    else { fColorIn = maskColor;}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v6.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v6.w); }
    else { fColorIn = maskColor;}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v7.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v7.w); }
    else { fColorIn = maskColor;}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v4.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v4.w); }
    else { fColorIn = maskColor;}
          EmitVertex();
    EndPrimitive();

    // BOTTOM sideBottom = v0 v4 v7 v3

    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v0.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v0.w); }
    else { fColorIn = maskColor;}
          EmitVertex();

    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v4.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v4.w); }
    else { fColorIn = maskColor;}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v7.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v7.w); }
    else { fColorIn = maskColor;}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v3.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v3.w); }
    else { fColorIn = maskColor;}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v0.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v0.w); }
    else { fColorIn = maskColor;}
          EmitVertex();
    EndPrimitive();
}

void main(){

        Cube c = createCube(vec4(startPoint,0.0) + gl_in[0].gl_Position);
        float theCase = calculateCase(c); 
        if (cubeMask == -1.0) printCube(c,theCase,true);
        else if (theCase == cubeMask) printCube(c,theCase,true);
}
