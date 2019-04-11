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

  vec3 n0;
  vec3 n1;
  vec3 n2;
  vec3 n3;
  vec3 n4;
  vec3 n5;
  vec3 n6;
  vec3 n7;
};

vec3 calculateNormal(vec3 v, float d)
{
  vec3 grad;
  grad.x = texture(diffuse3DTexture,v + vec3(d,0,0)).w - texture(diffuse3DTexture,v + vec3(-d,0,0)).w;
  grad.y = texture(diffuse3DTexture,v + vec3(0,d,0)).w - texture(diffuse3DTexture,v + vec3(0,-d,0)).w;
  grad.z = texture(diffuse3DTexture,v + vec3(0,0,d)).w - texture(diffuse3DTexture,v + vec3(0,0,-d)).w;
  return normalize(grad); 
  //return -normalize(grad); 
}

Cube createCube(vec4 position, float vpb)
{
  // The length of a cube edge.
  float d = 1/vpb;

  // Create cube corner coordinates.
  vec3 p0 = position.xyz*d;
  vec3 p1 = position.xyz*d + vec3(0.0 ,   d , 0.0);
  vec3 p2 = position.xyz*d + vec3(d   ,   d , 0.0);
  vec3 p3 = position.xyz*d + vec3(d   , 0.0 , 0.0);
  vec3 p4 = position.xyz*d + vec3(0.0 , 0.0 , d);
  vec3 p5 = position.xyz*d + vec3(0.0 ,   d , d);
  vec3 p6 = position.xyz*d + vec3(d   ,   d , d);
  vec3 p7 = position.xyz*d + vec3(d   , 0.0 , d);

  // xyz = position, w = density. The density value is taken from the the diffuse3DTexture alpha value. 
  vec4 v0 = vec4(p0, p0.y + texture(diffuse3DTexture,p0).w);  
  vec4 v1 = vec4(p1, p1.y + texture(diffuse3DTexture,p1).w);  
  vec4 v2 = vec4(p2, p2.y + texture(diffuse3DTexture,p2).w);  
  vec4 v3 = vec4(p3, p3.y + texture(diffuse3DTexture,p3).w);  
  vec4 v4 = vec4(p4, p4.y + texture(diffuse3DTexture,p4).w);  
  vec4 v5 = vec4(p5, p5.y + texture(diffuse3DTexture,p5).w);  
  vec4 v6 = vec4(p6, p6.y + texture(diffuse3DTexture,p6).w);  
  vec4 v7 = vec4(p7, p7.y + texture(diffuse3DTexture,p7).w);  

  // Normals.
  vec3 n0 = calculateNormal(p0, d);
  vec3 n1 = calculateNormal(p1, d);
  vec3 n2 = calculateNormal(p2, d);
  vec3 n3 = calculateNormal(p3, d);
  vec3 n4 = calculateNormal(p4, d);
  vec3 n5 = calculateNormal(p5, d);
  vec3 n6 = calculateNormal(p6, d);
  vec3 n7 = calculateNormal(p7, d);

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

  cube.n0 = n0;
  cube.n1 = n1;
  cube.n2 = n2;
  cube.n3 = n3;
  cube.n4 = n4;
  cube.n5 = n5;
  cube.n6 = n6;
  cube.n7 = n7;
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
    if (!(mask == cubeMask)) return;
    float pSize = 1.0;
    float maskColor = float(mask) / float(255);

    // FRONT sideFront = v0 v1 v2 v3
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v0.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v0.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();

    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v1.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v1.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v2.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v2.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v3.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v3.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v0.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v0.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();
    EndPrimitive();

    // TOP sideFront = v0 v5 v6 v2
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v1.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v1.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();

    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v5.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v5.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v6.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v6.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v2.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v2.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v1.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v1.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();
    EndPrimitive();

    // RIGHT sideRight = v3 v2 v6 v7
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v3.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v3.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();

    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v2.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v2.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v6.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v6.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v7.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v7.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v3.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v3.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();
    EndPrimitive();

    // LEFT sideLeft = v0 v1 v5 v4
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v0.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v0.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();

    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v1.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v1.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v5.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v5.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v4.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v4.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v0.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v0.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();
    EndPrimitive();

    // BACK sideBack = v4 v5 v6 v7

    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v4.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v4.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();

    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v5.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v5.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v6.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v6.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v7.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v7.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v4.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v4.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();
    EndPrimitive();

    // BOTTOM sideBottom = v0 v4 v7 v3

    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v0.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v0.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();

    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v4.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v4.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v7.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v7.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v3.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v3.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * vec4(c.v0.xyz,1.0);
    if (!printMask) { fColorIn = vec3(c.v0.w); }
    else { fColorIn = vec3(maskColor);}
          EmitVertex();
    EndPrimitive();
}

void main(){

        Cube c = createCube(vec4(startPoint,0.0) + gl_in[0].gl_Position,voxels_per_block);
        float theCase = calculateCase(c); 
        printCube(c,theCase,false);
}
