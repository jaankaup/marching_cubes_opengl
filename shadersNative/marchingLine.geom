#version 430

// The input data is a point. A coordinate for base cube corner. 
layout(points) in;

// The output will be a triangle. 
//layout(triangle_strip, max_vertices = 15) out;
layout(line_strip, max_vertices = 30) out;
//layout(line_strip, max_vertices = 15) out;

// Output vertex normal.
//out vec3 fNormalIn;
out vec3 fColorIn;
uniform sampler3D diffuse3DTexture;
uniform sampler1D tri_table;
uniform float voxels_per_block;
uniform mat4 MVP;

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

/*
in gl_PerVertex
{
  vec4 gl_position;
  float gl_PointSize;
  float gl_ClipDistance;
} gl_in[];


The size of gl_in in now 1 because we are using point as input element. 
*/


/*
        v5                        v6
         +------------------------+
        /|                       /|
       / |                      / |
      /  |                     /  |
     /   |                    /   |  
    /    |                   /    |
v1 +------------------------+ v2  |
   |     |                  |     |
   |     |                  |     |
   |     |                  |     |
   |  v4 +------------------|-----+ v7
   |    /                   |    /
   |   /                    |   /
   |  /                     |  /    
   | /                      | /
   |/                       |/
   +------------------------+
  v0                       v3

                   e5               
         +------------------------+
        /|                       /|
       / |                      / |
  e9  /  |                  e10/  |
     /   |e4                  /   |e6
    /    |       e1          /    |
   +------------------------+     |
   |     |                  |     |
   |     |                  |     |
   |     |        e7        |     |
   |     +------------------|-----+   
e0 |    /                   |e2  /
   |   /                    |   /
   |  / e8                  |  /  e11
   | /                      | /
   |/                       |/
   +------------------------+
              e3             
  +y  +z
   ^  ^
   | /
   |/
   +----> +x

 */


// https://github.com/QianMo/GPU-Gems-Book-Source-Code/blob/master/GPU-Gems-3-CD-Content/content/01/demo/models/tables.nma

const int case_to_numpolys[256] = int[]( 0 , 1 , 1 , 2 , 1 , 2 , 2 , 3 ,  1 , 2 , 2 , 3 , 2 , 3 , 3 , 2 ,  1 , 2 , 2 , 3 , 2 , 3 , 3 , 4 ,  2 , 3 , 3 , 4 , 3 , 4 , 4 , 3 , 1 , 2 , 2 , 3 , 2 , 3 , 3 , 4 ,  2 , 3 , 3 , 4 , 3 , 4 , 4 , 3 ,  2 , 3 , 3 , 2 , 3 , 4 , 4 , 3 ,  3 , 4 , 4 , 3 , 4 , 5 , 5 , 2 ,  1 , 2 , 2 , 3 , 2 , 3 , 3 , 4 ,  2 , 3 , 3 , 4 , 3 , 4 , 4 , 3 ,  2 , 3 , 3 , 4 , 3 , 4 , 4 , 5 ,  3 , 4 , 4 , 5 , 4 , 5 , 5 , 4 ,  2 , 3 , 3 , 4 , 3 , 4 , 2 , 3 ,  3 , 4 , 4 , 5 , 4 , 5 , 3 , 2 ,  3 , 4 , 4 , 3 , 4 , 5 , 3 , 2 ,  4 , 5 , 5 , 4 , 5 , 2 , 4 , 1 ,  1 , 2 , 2 , 3 , 2 , 3 , 3 , 4 ,  2 , 3 , 3 , 4 , 3 , 4 , 4 , 3 ,  2 , 3 , 3 , 4 , 3 , 4 , 4 , 5 ,  3 , 2 , 4 , 3 , 4 , 3 , 5 , 2 ,  2 , 3 , 3 , 4 , 3 , 4 , 4 , 5 ,  3 , 4 , 4 , 5 , 4 , 5 , 5 , 4 ,  3 , 4 , 4 , 3 , 4 , 5 , 5 , 4 ,  4 , 3 , 5 , 2 , 5 , 4 , 2 , 1 ,  2 , 3 , 3 , 4 , 3 , 4 , 4 , 5 ,  3 , 4 , 4 , 5 , 2 , 3 , 3 , 2 ,  3 , 4 , 4 , 5 , 4 , 5 , 5 , 2 ,  4 , 3 , 5 , 4 , 3 , 2 , 4 , 1 , 3 , 4 , 4 , 5 , 4 , 5 , 3 , 4 ,  4 , 5 , 5 , 2 , 3 , 4 , 2 , 1 ,  2 , 3 , 3 , 2 , 3 , 4 , 2 , 1 ,  3 , 2 , 4 , 1 , 2 , 1 , 1 , 0); 

//paramName "g_case_to_vertOnEdge_mask"
const int[256] case_to_vertOnEdge_mask = int[](
          0   , 265  , 515  , 778  , 103  , 1295 , 1541 , 1804 , 
          206 , 2309 , 2575 , 2822 , 3082 , 3331 , 3593 , 384  , 
          400 , 153  , 915  , 666  , 143  , 1183 , 1941 , 1692 , 
          246 , 2197 , 2975 , 271  , 3482 , 3219 , 3993 , 3728 , 
          560 , 825  , 51   , 314  , 159  , 1855 , 1077 , 134  , 
          262 , 2869 , 2111 , 2358 , 3642 , 3891 , 3129 , 3376 , 
          928 , 681  , 419  , 170  , 1958 , 1711 , 1445 , 1196 , 
          2988, 2725 , 2479 , 2214 , 401  , 3747 , 3497 , 3232 , 
          112 , 1385 , 1635 , 1898 , 102  , 367  , 613  , 876  , 
          318 , 3429 , 3695 , 3942 , 2154 , 2403 , 2665 , 2912 , 
          152 , 1273 , 2035 , 1786 , 502  , 255  , 1013 , 764  , 
          358 , 3317 , 4095 , 383  , 2554 , 2291 , 3065 , 280  , 
          1616, 1881 , 1107 , 137  , 598  , 863  , 85   , 348  , 
          3676, 3925 , 3167 , 3414 , 265  , 2899 , 2137 , 2384 , 
          1984, 1737 , 1475 , 1226 , 966  , 719  , 453  , 204  , 
          4044, 3781 , 3535 , 327  , 3018 , 2755 , 2505 , 224  , 
          224 , 2505 , 2755 , 3018 , 327  , 3535 , 3781 , 4044 , 
          204 , 453  , 719  , 966  , 1226 , 1475 , 1737 , 1984 , 
          2384, 2137 , 2899 , 265  , 3414 , 3167 , 3925 , 3676 , 
          348 , 85   , 863  , 598  , 137  , 1107 , 1881 , 1616 , 
          280 , 3065 , 2291 , 2554 , 383  , 4095 , 3317 , 358  , 
          764 , 1013 , 255  , 502  , 1786 , 2035 , 1273 , 152  , 
          2912, 2665 , 2403 , 2154 , 3942 , 3695 , 3429 , 318  , 
          876 , 613  , 367  , 102  , 1898 , 1635 , 1385 , 112  , 
          3232, 3497 , 3747 , 401  , 2214 , 2479 , 2725 , 2988 , 
          1196, 1445 , 1711 , 1958 , 170  , 419  , 681  , 928  , 
          3376, 3129 , 3891 , 3642 , 2358 , 2111 , 2869 , 262  , 
          134 , 1077 , 1855 , 159  , 314  , 51   , 825  , 560  , 
          3728, 3993 , 3219 , 3482 , 271  , 2975 , 2197 , 246  , 
          1692, 1941 , 1183 , 143  , 666  , 915  , 153  , 400  , 
          384 , 3593 , 3331 , 3082 , 2822 , 2575 , 2309 , 206  , 
          1804, 1541 , 1295 , 103  , 778  , 515  , 265  , 0);     

//    isovalues[0] = data.getValue(i  ,j  ,k);
//    isovalues[1] = data.getValue(i+1,j  ,k);
//    isovalues[2] = data.getValue(i+1,j  ,k+1);
//    isovalues[3] = data.getValue(i  ,j  ,k+1);
//    isovalues[4] = data.getValue(i  ,j+1,k);
//    isovalues[5] = data.getValue(i+1,j+1,k);
//    isovalues[6] = data.getValue(i+1,j+1,k+1);
//    isovalues[7] = data.getValue(i  ,j+1,+k+1);
//	vec3 surfaceColor = texture(diffuse3DTexture,vec3(tFrag_in,0.5)).rgb;

vec3 calculateNormal(vec3 v)
{
  float d = 1.0/voxels_per_block;
  vec3 grad;
  grad.x = texture(diffuse3DTexture,v + vec3(d,0,0)).w - texture(diffuse3DTexture,v + vec3(-d,0,0)).w;
  grad.y = texture(diffuse3DTexture,v + vec3(0,d,0)).w - texture(diffuse3DTexture,v + vec3(0,-d,0)).w;
  grad.z = texture(diffuse3DTexture,v + vec3(0,0,d)).w - texture(diffuse3DTexture,v + vec3(0,0,-d)).w;
  //return normalize(grad); 
  return -normalize(grad); 
}

Cube createCube()
{
  float d = 1.0/voxels_per_block;

  vec3 p0 = gl_in[0].gl_Position.xyz*d;
  vec3 p1 = gl_in[0].gl_Position.xyz*d + vec3(0.0,d,0.0);
  vec3 p2 = gl_in[0].gl_Position.xyz*d + vec3(d,d,0.0);
  vec3 p3 = gl_in[0].gl_Position.xyz*d + vec3(d,0.0,0.0);
  vec3 p4 = gl_in[0].gl_Position.xyz*d + vec3(0.0,0.0,d);
  vec3 p5 = gl_in[0].gl_Position.xyz*d + vec3(0.0,d,d);
  vec3 p6 = gl_in[0].gl_Position.xyz*d + vec3(d,d,d);
  vec3 p7 = gl_in[0].gl_Position.xyz*d + vec3(d,0.0,d);

  // xyz = position, w = density
  vec4 v0 = vec4(p0, texture(diffuse3DTexture,p0).w);  
  vec4 v1 = vec4(p1, texture(diffuse3DTexture,p1).w);  
  vec4 v2 = vec4(p2, texture(diffuse3DTexture,p2).w);  
  vec4 v3 = vec4(p3, texture(diffuse3DTexture,p3).w);  
  vec4 v4 = vec4(p4, texture(diffuse3DTexture,p4).w);  
  vec4 v5 = vec4(p5, texture(diffuse3DTexture,p5).w);  
  vec4 v6 = vec4(p6, texture(diffuse3DTexture,p6).w);  
  vec4 v7 = vec4(p7, texture(diffuse3DTexture,p7).w);  

  vec3 n0 = calculateNormal(p0);
  vec3 n1 = calculateNormal(p1);
  vec3 n2 = calculateNormal(p2);
  vec3 n3 = calculateNormal(p3);
  vec3 n4 = calculateNormal(p4);
  vec3 n5 = calculateNormal(p5);
  vec3 n6 = calculateNormal(p6);
  vec3 n7 = calculateNormal(p7);

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

int calculateCase(Cube c)
{
  int result = 0;
  const float offset = 0.5;
  
  if (c.v7.w - offset < 0.0) { result += 128;} 
  if (c.v6.w - offset < 0.0) { result += 64;}
  if (c.v5.w - offset < 0.0) { result += 32;} 
  if (c.v4.w - offset < 0.0) { result += 16;} 
  if (c.v3.w - offset < 0.0) { result += 8; }
  if (c.v2.w - offset < 0.0) { result += 4; }
  if (c.v1.w - offset < 0.0) { result += 2; }
  if (c.v0.w - offset < 0.0) { result += 1; }
  
  return result;

} 

int calculate_polys(int mask)
{
  return case_to_numpolys[mask];
}

vec3 interPolateV(vec4 vb, vec4 va)
{
   return va.xyz;
//   float mu;
//   vec3 p;
//
//   if (abs(va.w-0.5) < 0.00001)
//      return va.xyz;
//   if (abs(vb.w-0.5) < 0.00001)
//      return vb.xyz;
//   if (abs(va.w-vb.w) < 0.00001)
//      return va.xyz;
//   mu = (0.0 - (va.w-0.5)) / ((vb.w-0.5) - (va.w-0.5));
//   p.x = va.x + mu * (vb.x - va.x);
//   p.y = va.y + mu * (vb.y - va.y);
//   p.z = va.z + mu * (vb.z - va.z);
//
//   return p;
}

          // e0 => v0 , v1 
          // e1 => v1 , v2 
          // e2 => v2 , v3 
          // e3 => v0 , v3 
          // e4 => v4 , v5 
          // e5 => v5 , v6 
          // e6 => v6 , v7 
          // e7 => v4 , v7 
          // e8 => v0 , v4 
          // e9 => v1 , v5 
          // e10 => v2 , v6 
          // e11 => v3 , v7 

void printError(int index, int maxValue)
{

    vec3 actColor = vec3(float(index)/float(maxValue));
	  gl_Position =  MVP * (gl_in[0].gl_Position + vec4(0.0,float(index)/voxels_per_block,0.0,0.0));
          fColorIn = actColor;
          EmitVertex();

	  gl_Position =  MVP * (gl_in[0].gl_Position + vec4(float(index)/voxels_per_block,0.0,0.0,0.0));
          fColorIn = actColor;
          EmitVertex();

	  gl_Position =  MVP * (gl_in[0].gl_Position + vec4(0.0,0.0,float(index)/voxels_per_block,0.0));
          fColorIn = actColor;
          EmitVertex();
 
          EndPrimitive();

	  gl_Position =  MVP * (gl_in[0].gl_Position + vec4(0.1,float(index)/voxels_per_block,0.1,0.0));
          fColorIn = vec3(0.0,0.0,0.0);
          EmitVertex();

	  gl_Position =  MVP * (gl_in[0].gl_Position + vec4(float(index)/voxels_per_block,0.1,0.1,0.0));
          fColorIn = vec3(0.0,0.0,0.0);
          EmitVertex();

	  gl_Position =  MVP * (gl_in[0].gl_Position + vec4(0.1,0.1,float(index)/voxels_per_block,0.0));
          fColorIn = vec3(0.0,0.0,0.0);
          EmitVertex();
 
          EndPrimitive();
}

void printError2(int index)
{

    //vec3 actColor = vec3(float(index)/float(maxValue));
	  gl_Position =  MVP * (gl_in[0].gl_Position + vec4(0.0,float(index)/voxels_per_block,0.0,0.0));
          fColorIn = vec3(0.0,0.5,0.0);
          EmitVertex();

	  gl_Position =  MVP * (gl_in[0].gl_Position + vec4(float(index)/voxels_per_block,0.0,0.0,0.0));
          fColorIn = vec3(0.0,0.5,0.0);
          EmitVertex();

	  gl_Position =  MVP * (gl_in[0].gl_Position + vec4(0.0,0.0,float(index)/voxels_per_block,0.0));
          fColorIn = vec3(0.0,0.5,0.0);
          EmitVertex();
 
          EndPrimitive();

	  gl_Position =  MVP * (gl_in[0].gl_Position + vec4(0.1,float(index)/voxels_per_block,0.1,0.0));
          fColorIn = vec3(0.0,0.0,0.0);
          EmitVertex();

	  gl_Position =  MVP * (gl_in[0].gl_Position + vec4(float(index)/voxels_per_block,0.1,0.1,0.0));
          fColorIn = vec3(0.0,0.0,0.0);
          EmitVertex();

	  gl_Position =  MVP * (gl_in[0].gl_Position + vec4(0.1,0.1,float(index)/voxels_per_block,0.0));
          fColorIn = vec3(0.0,0.0,0.0);
          EmitVertex();
 
          EndPrimitive();
}

void printCubeCorners(Cube c)
{
    float pSize = 2.0;

    gl_PointSize = pSize;
	  gl_Position =  MVP * (gl_in[0].gl_Position + vec4(c.v0.xyz,1.0));
          fColorIn = vec3(0.0,0.5,0.0);
          EmitVertex();

    gl_PointSize = pSize;
	  gl_Position =  MVP * (gl_in[0].gl_Position + vec4(c.v1.xyz,1.0));
          fColorIn = vec3(0.0,0.5,0.0);
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * (gl_in[0].gl_Position + vec4(c.v2.xyz,1.0));
          fColorIn = vec3(0.0,0.5,0.0);
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * (gl_in[0].gl_Position + vec4(c.v3.xyz,1.0));
          fColorIn = vec3(0.0,0.5,0.0);
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * (gl_in[0].gl_Position + vec4(c.v4.xyz,1.0));
          fColorIn = vec3(0.0,0.5,0.0);
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * (gl_in[0].gl_Position + vec4(c.v5.xyz,1.0));
          fColorIn = vec3(0.0,0.5,0.0);
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * (gl_in[0].gl_Position + vec4(c.v6.xyz,1.0));
          fColorIn = vec3(0.0,0.5,0.0);
          EmitVertex();
    gl_PointSize = pSize;
	  gl_Position =  MVP * (gl_in[0].gl_Position + vec4(c.v7.xyz,1.0));
          fColorIn = vec3(0.0,0.5,0.0);
          EmitVertex();
 
          EndPrimitive();
}

void printCube(Cube c, int mask, bool printMask)
{
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

        //Cube c = createCube(gl_in[0].gl_Position.xyz);
        Cube c = createCube();
        int mask = calculateCase(c); 
        int polyCount = calculate_polys(mask); 
//        printCubeCorners(c);
        printCube(c, mask, true);  /* draw lines! */
//        printError(mask,255);

//        for (int i=0 ; i<polyCount ; i++) {
//        for (int j=0 ; j<3 ; j++) {
//            int index = 3*i+j;
//            if (tri_table[mask][index] == 0)
//            {}
//        }} 
//        if (mask > 255)
//        {
//          printError();
//        }
//         printError();
   
//        EndPrimitive();          
//
//          printError();
////        for (int i=0 ; i<polyCount ; i++)
////        {
////          for (int j=0 ; j<3 ; j++)
////          {
////            //printError2(255);
////            int index = mask*15+i;
////            float normalizedIndex = (float(index)+0.5)/float(1280);
////            //float normalizedIndex = float(index)/float(1280);
////            int actualIndex = 0;
////            if (j == 0)  actualIndex = int(texture(tri_table,normalizedIndex).r * 255.0);
////            if (j == 1)  actualIndex = int(texture(tri_table,normalizedIndex).g * 255.0);
////            if (j == 2)  actualIndex = int(texture(tri_table,normalizedIndex).b * 255.0);
////
//////// //           if (actualIndex < 11)
//////// //           {
//////// //             printError();
//////// //           }
////////             
////            if (actualIndex == 0)
////            {
//////              printError2(255);
//////                printError(actualIndex,11);
////	            gl_Position =  MVP * vec4(interPolateV(c.v0, c.v1),1.0);
////              //fNormalIn = vec3(1.0,0.0,0.0); // c.n0;
////              fColorIn = vec3(1.0,0.0,0.0);
////              EmitVertex();
////            }           
////            else if (actualIndex == 1)
////            {
//////              printError2(255);
//////              printError(actualIndex,11);
////	            gl_Position =  MVP * vec4(interPolateV(c.v1, c.v2),1.0);
////              //fNormalIn = vec3(1.0,0.0,0.0); // c.n1;
////              fColorIn = vec3(0.0,1.0,0.0);
////              EmitVertex();
////            }           
////            else if (actualIndex == 2)
////            {
//////              printError2(255);
//////              printError(actualIndex,11);
////	            gl_Position =  MVP * vec4(interPolateV(c.v2, c.v3),1.0);
////              //fNormalIn = vec3(1.0,0.0,0.0); //c.n2;
////              fColorIn = vec3(0.0,0.0,1.0);
////              EmitVertex();
////            }           
////            else if (actualIndex == 3)
////            {
//////              printError2(255);
//////              printError(actualIndex,11);
////	            gl_Position =  MVP * vec4(interPolateV(c.v0, c.v3),1.0);
////              //fNormalIn = vec3(1.0,0.0,0.0); //c.n3;
////              fColorIn = vec3(0.0,0.3,0.3);
////              EmitVertex();
////            }           
////            else if (actualIndex == 4)
////            {
//////              printError2(255);
//////              printError(actualIndex,11);
////	            gl_Position =  MVP * vec4(interPolateV(c.v4, c.v5),1.0);
////              //fNormalIn = vec3(1.0,0.0,0.0); //c.n4;
////              fColorIn = vec3(0.3,0.3,0.0);
////              EmitVertex();
////            }           
////            else if (actualIndex == 5)
////            {
//////              printError2(255);
//////              printError(actualIndex,11);
////	            gl_Position =  MVP * vec4(interPolateV(c.v5, c.v6),1.0);
////              //fNormalIn = vec3(1.0,0.0,0.0); //c.n5;
////              fColorIn = vec3(0.0,0.7,0.2);
////              EmitVertex();
////            }           
////            else if (actualIndex == 6)
////            {
//////              printError2(255);
//////              printError(actualIndex,11);
////	            gl_Position =  MVP * vec4(interPolateV(c.v6, c.v7),1.0);
////              //fNormalIn = vec3(1.0,0.0,0.0); //c.n1;
////              fColorIn = vec3(1.0,0.0,1.0);
////              EmitVertex();
////            }           
////            else if (actualIndex == 7)
////            {
//////              printError2(255);
//////              printError(actualIndex,11);
////	            gl_Position =  MVP * vec4(interPolateV(c.v4, c.v7),1.0);
////              //fNormalIn = vec3(1.0,0.0,0.0); //c.n1;
////              fColorIn = vec3(1.0,1.0,0.0);
////              EmitVertex();
////            }           
////            else if (actualIndex == 8)
////            {
//////              printError2(255);
//////              printError(actualIndex,11);
////	            gl_Position =  MVP * vec4(interPolateV(c.v0, c.v4),1.0);
////              //fNormalIn = vec3(1.0,0.0,0.0); //c.n1;
////              fColorIn = vec3(0.0,1.0,1.0);
////              EmitVertex();
////            }           
////            else if (actualIndex == 9)
////            {
//////              printError2(255);
//////              printError(actualIndex,11);
////	            gl_Position =  MVP * vec4(interPolateV(c.v1, c.v5),1.0);
////              //fNormalIn = vec3(1.0,0.0,0.0); //c.n1;
////              fColorIn = vec3(0.0,0.5,1.0);
////              EmitVertex();
////            }           
////            else if (actualIndex == 10)
////            {
//////              printError2(255);
//////              printError(actualIndex,11);
////	            gl_Position =  MVP * vec4(interPolateV(c.v2, c.v6),1.0);
////              //fNormalIn = vec3(1.0,0.0,0.0); //c.n1;
////              fColorIn = vec3(0.1,0.1,0.1);
////              EmitVertex();
////            }           
////            else if (actualIndex == 11)
////            {
//////              printError2(255);
//////              printError(actualIndex,11);
////	            gl_Position =  MVP * vec4(interPolateV(c.v3, c.v7),1.0);
////              //fNormalIn = vec3(1.0,0.0,0.0); //c.n1;
////              fColorIn = vec3(0.5,0.5,0.5);
////              EmitVertex();
////            }           
//////            else if (actualIndex > 11)
//////            {
//////              printError2(255);
//////              //printError2(actualIndex);
//////            }
////            else
////            {
////              printError2(255);
////            }
////
////          }
////          EndPrimitive();          
////        }

}
