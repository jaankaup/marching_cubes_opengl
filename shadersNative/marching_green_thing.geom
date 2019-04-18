#version 430

// The input data is a point. A coordinate for base cube corner. 
layout(points) in;

layout(triangle_strip, max_vertices = 15) out;

// Output vertex normal.
out vec3 fPosIn;
out vec3 fNormalIn;
uniform sampler3D diffuse3DTexture;
uniform sampler1D tri_table;
uniform float voxels_per_block;
uniform vec3 startPoint;
uniform mat4 MVP;
uniform float cubeMask;
uniform float debugMask;
//uniform vec3 cameraPosition;
//uniform vec3 lookAt;
//uniform float time;

uniform float isovalue = 1.0;

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
// https://prideout.net/blog/distance_fields

float calculateDensity(vec3 v)
{
//  float ball = pow(v.x,2.0) + pow(v.y,2.0) + pow(v.z,2.0) ; // - pow(2.2,2.0);  
  float noise = texture(diffuse3DTexture,v).w;
//  float noise2 = texture(diffuse3DTexture,v+vec3(0.2,0.1,0.3)).w;
//  float noise3 = texture(diffuse3DTexture,v+vec3(sin(v.x),tan(v.y),cos(v.z))).w;
//  float hils = noise2*33.0*sin(v.z);
//  float hils2 = noise*13.0*cos(v.z);

// return ball + hils + hils2 - noise2*35.0 - noise*(30*sin(v.z*v.x));
 return v.y - noise;
}

vec3 calculateNormal(vec3 v)
{
  float d = 1.0 / voxels_per_block;
  vec3 grad;
  grad.x = calculateDensity(v + vec3(d,0,0)) - calculateDensity(v + vec3(-d,0,0)); //texture(diffuse3DTexture,v + vec3(d,0,0)).w - texture(diffuse3DTexture,v + vec3(-d,0,0)).w;
  grad.y = calculateDensity(v + vec3(0,d,0)) - calculateDensity(v + vec3(0,-d,0)); //texture(diffuse3DTexture,v + vec3(0,d,0)).w - texture(diffuse3DTexture,v + vec3(0,-d,0)).w;
  grad.z = calculateDensity(v + vec3(0,0,d)) - calculateDensity(v + vec3(0,0,-d)); //texture(diffuse3DTexture,v + vec3(0,0,d)).w - texture(diffuse3DTexture,v + vec3(0,0,-d)).w;
  return normalize(grad); 
}

Cube createCube(vec4 position)
{
  // The length of a cube edge.
  float d = 1.0/voxels_per_block;

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
  
  if (c.v7.w < isovalue) { result += 128.0;} 
  if (c.v6.w < isovalue) { result += 64.0;}
  if (c.v5.w < isovalue) { result += 32.0;} 
  if (c.v4.w < isovalue) { result += 16.0;} 
  if (c.v3.w < isovalue) { result += 8.0; }
  if (c.v2.w < isovalue) { result += 4.0; }
  if (c.v1.w < isovalue) { result += 2.0; }
  if (c.v0.w < isovalue) { result += 1.0; }
  
  return result;
} 

vec3 interpolateV(vec4 va, vec4 vb)
{
   if (abs(isovalue - va.w) < 0.00001) { return va.xyz; }
   else if (abs(isovalue - vb.w) < 0.00001) { return vb.xyz; }
   else if (abs(va.w-vb.w) < 0.00001) { return va.xyz; }
   
   else
   {
     vec3 p;
     float mu = (isovalue - va.w) / (vb.w - va.w);
     p.x = va.x + mu * (vb.x - va.x);
     p.y = va.y + mu * (vb.y - va.y);
     p.z = va.z + mu * (vb.z - va.z);
     return p;
   }
}

vec3 interpolateN(vec3 na, vec3 nb, float densityA, float densityB)
{
   if (abs(isovalue - densityA) < 0.00001) { return na; }
   else if (abs(isovalue - densityB) < 0.00001) { return nb; }
   else if (abs(densityA-densityB) < 0.00001) { return na; }
   
   else
   {
     vec3 p;
     float mu = (isovalue - densityA) / (densityB - densityA);
     p.x = na.x + mu * (nb.x - na.x);
     p.y = na.y + mu * (nb.y - na.y);
     p.z = na.z + mu * (nb.z - na.z);
     return normalize(p);
   }
}

void createVertex(float edgeValue, Cube c)
{
    float iterator = 1.0 / 255.0;
    if (edgeValue == 0.0) // < 0.000001 )
    {
      gl_Position =  MVP * vec4(interpolateV(c.v0, c.v1),1.0);
//      fColorIn = vec3(1.0,0.0,0.0);
        fPosIn = interpolateV(c.v0, c.v1);
//        fNormalIn = c.n0;
        fNormalIn = interpolateN(c.n0, c.n1, c.v0.w, c.v1.w);
      EmitVertex();
    }           
    else if (abs(edgeValue - iterator) < 0.000001)
    {
      gl_Position =  MVP * vec4(interpolateV(c.v1, c.v2),1.0);
      //fColorIn = vec3(0.0,1.0,0.0);
      fPosIn = interpolateV(c.v1, c.v2);
//      fNormalIn = c.n1;
      fNormalIn = interpolateN(c.n1, c.n2, c.v1.w, c.v2.w);
      EmitVertex();
    }           
    else if (abs(edgeValue - iterator * 2.0) < 0.000001)
    //else if (edgeValue == iterator*2.0)
    {
      gl_Position =  MVP * vec4(interpolateV(c.v2, c.v3),1.0);
      fPosIn = interpolateV(c.v2, c.v3);
      //fColorIn = vec3(0.0,0.0,1.0);
      //fNormalIn = c.n2;
      fNormalIn = interpolateN(c.n2, c.n3, c.v2.w, c.v3.w);
      EmitVertex();
    }           
    else if (abs(edgeValue - iterator * 3.0) < 0.000001)
    {
      gl_Position =  MVP * vec4(interpolateV(c.v3, c.v0),1.0);
      fPosIn = interpolateV(c.v3, c.v0);
      //fColorIn = vec3(0.0,0.3,0.3);
      fNormalIn = interpolateN(c.n3, c.n0, c.v3.w, c.v0.w);
//      fNormalIn = c.n3;
      EmitVertex();
    }           
    else if (abs(edgeValue - iterator * 4.0) < 0.000001)
    {
      gl_Position =  MVP * vec4(interpolateV(c.v4, c.v5),1.0);
      fPosIn = interpolateV(c.v4, c.v5);
      //fColorIn = vec3(0.3,0.3,0.0);
      //fNormalIn = c.n4;
      fNormalIn = interpolateN(c.n4, c.n5, c.v4.w, c.v5.w);
      EmitVertex();
    }           
    else if (abs(edgeValue - iterator * 5.0) < 0.000001)
    {
      gl_Position =  MVP * vec4(interpolateV(c.v5, c.v6),1.0);
      fPosIn = interpolateV(c.v5, c.v6);
      //fColorIn = vec3(0.0,0.7,0.2);
      //fNormalIn = c.n5;
      fNormalIn = interpolateN(c.n5, c.n6, c.v5.w, c.v6.w);
      EmitVertex();
    }           
    else if (abs(edgeValue - iterator * 6.0) < 0.000001)
    {
      gl_Position =  MVP * vec4(interpolateV(c.v6, c.v7),1.0);
      fPosIn = interpolateV(c.v6, c.v7);
      //fNormalIn = c.n6;
      fNormalIn = interpolateN(c.n6, c.n7, c.v6.w, c.v7.w);
      //fColorIn = vec3(1.0,0.0,1.0);
      EmitVertex();
    }           
    else if (abs(edgeValue - iterator * 7.0) < 0.000001)
    {
      gl_Position =  MVP * vec4(interpolateV(c.v7, c.v4),1.0);
      fPosIn = interpolateV(c.v7, c.v4);
      //fNormalIn = c.n7;
      fNormalIn = interpolateN(c.n7, c.n4, c.v7.w, c.v4.w);
      EmitVertex();
    }           
    else if (abs(edgeValue - iterator * 8.0) < 0.000001)
    {
      gl_Position =  MVP * vec4(interpolateV(c.v0, c.v4),1.0);
      //fNormalIn = c.n0;
      fNormalIn = interpolateN(c.n0, c.n4, c.v0.w, c.v4.w);
      fPosIn = interpolateV(c.v0, c.v4);
      //fColorIn = vec3(0.0,1.0,1.0);
      EmitVertex();
    }           
    else if (abs(edgeValue - iterator * 9.0) < 0.000001)
    {
      gl_Position =  MVP * vec4(interpolateV(c.v1, c.v5),1.0);
      fPosIn = interpolateV(c.v1, c.v5);
      //fNormalIn = c.n1;
      fNormalIn = interpolateN(c.n1, c.n5, c.v1.w, c.v5.w);
      //fColorIn = vec3(0.0,0.5,1.0);
      EmitVertex();
    }           
    else if (abs(edgeValue - iterator * 10.0) < 0.000001)
    {
      gl_Position =  MVP * vec4(interpolateV(c.v2, c.v6),1.0);
      fPosIn = interpolateV(c.v2, c.v6);
      //fNormalIn = c.n2;
      fNormalIn = interpolateN(c.n2, c.n6, c.v2.w, c.v6.w);
      //fColorIn = vec3(0.1,0.1,0.1);
      EmitVertex();
    }           
    else if (abs(edgeValue - iterator * 11.0) < 0.000001)
    {
      gl_Position =  MVP * vec4(interpolateV(c.v3, c.v7),1.0);
      fPosIn = interpolateV(c.v3, c.v7);
      //fNormalIn = c.n3;
      fNormalIn = interpolateN(c.n3, c.n7, c.v3.w, c.v7.w);
      //fColorIn = vec3(0.5,0.5,0.5);
      EmitVertex();
    }           
}

void marchCube(Cube c)
{
        float mask = calculateCase(c); 

        // Return if the cube doesn't create any triangles.
        if (mask == 0.0) return;
        if (mask == 255.0) return;

        if (!(mask == cubeMask) && debugMask == 1.0) return; 

        c.n0 = calculateNormal(c.v0.xyz);
        c.n1 = calculateNormal(c.v1.xyz);
        c.n2 = calculateNormal(c.v2.xyz);
        c.n3 = calculateNormal(c.v3.xyz);
        c.n4 = calculateNormal(c.v4.xyz);
        c.n5 = calculateNormal(c.v5.xyz);
        c.n6 = calculateNormal(c.v6.xyz);
        c.n7 = calculateNormal(c.v7.xyz);

        // Find the center of the texel which has the information of the first ende vertices.
        // The pixel has the followin format: r = first edge, g = second edge, b = third edge.
        // Check the tri_table. The tri_table is now 1D texture. Index is the "pointer" to the 
        // first possible triable information. We move the pointer 0.5 forward to access the center
        // of the texel.

        //        mask + 0.5 / 1280
        //         |
        //         V
        // +----+----+----+----+----+----+----+----+----
        // |rgb |rgb |rgb |rgb |rgb |rgb |rgb |rgb |rgb|
        // +----+----+----+----+----+----+----+----+----
        //        1.   2.   3.   4.   5.
        //         

        float index = (mask * 5.0 + 0.5) / 1280.0;

        // The first edge. 
        vec3 edges1 = texture(tri_table,index).rgb;

        // Create 1. triable.
        createVertex(edges1.r, c);
        createVertex(edges1.g, c);
        createVertex(edges1.b, c);

        EndPrimitive();

        // The 2. edge. 
        vec3 edges2 = texture(tri_table,index + 1.0/1280.0).rgb;

        // If the first vertex number is 1.0 (normalized from 255), there is no more triables.
        if (abs(edges2.r - 1.0) < 0.000001) return;

        // Create the 2. triable.
        createVertex(edges2.r, c);
        createVertex(edges2.g, c);
        createVertex(edges2.b, c);

        EndPrimitive();

        // The 3. edge. 
        vec3 edges3 = texture(tri_table,index+2.0/1280.0).rgb;

        if (abs(edges3.r - 1.0) < 0.000001) return;

        createVertex(edges3.r, c);
        createVertex(edges3.g, c);
        createVertex(edges3.b, c);

        EndPrimitive();

        // The 4. edge. 
        vec3 edges4 = texture(tri_table,index+3.0/1280.0).rgb;

        if (abs(edges4.r - 1.0) < 0.000001) return;

        createVertex(edges4.r, c);
        createVertex(edges4.g, c);
        createVertex(edges4.b, c);

        EndPrimitive();

        // The 5. edge. 
        vec3 edges5 = texture(tri_table,index+4.0/1280.0).rgb;

        if (abs(edges5.r - 1.0) < 0.000001) return;
        createVertex(edges5.r, c);
        createVertex(edges5.g, c);
        createVertex(edges5.b, c);

        EndPrimitive();
}

void main(){

//        if (lookAt)  
        vec4 sp = vec4(startPoint,0.0) + gl_in[0].gl_Position;
//        float distanceToCamera = distance(cameraPosition,sp.xyz);
//        float vpb;
//        if (distanceToCamera < 5.0) vpb = voxels_per_block;
//        else if (distanceToCamera < 10.0) vpb = voxels_per_block/2.0;
//        else if (distanceToCamera < 15.0) vpb = voxels_per_block/4.0;
        Cube c = createCube(sp);
        //Cube c = createCube(vec4(startPoint,0.0) + gl_in[0].gl_Position);
        marchCube(c);
}
