#version 430

// The input data is a point. A coordinate for base cube corner. 
layout(points) in;

// The output will be a triangle. 
layout(triangle_strip, max_vertices = 15) out;
//layout(line_strip, max_vertices = 30) out;
//layout(line_strip, max_vertices = 15) out;

// Output vertex normal.
out vec3 fPosIn;
out vec3 fNormalIn;
//out vec3 fColorIn;
uniform sampler3D diffuse3DTexture;
uniform sampler1D tri_table;
uniform float voxels_per_block;
uniform vec3 startPoint;
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


vec3 calculateNormal(vec3 v)
{
  float d = 1.0/voxels_per_block;
  vec3 grad;
  grad.x = texture(diffuse3DTexture,v + vec3(d,0,0)).w - texture(diffuse3DTexture,v + vec3(-d,0,0)).w;
  grad.y = texture(diffuse3DTexture,v + vec3(0,d,0)).w - texture(diffuse3DTexture,v + vec3(0,-d,0)).w;
  grad.z = texture(diffuse3DTexture,v + vec3(0,0,d)).w - texture(diffuse3DTexture,v + vec3(0,0,-d)).w;
  return normalize(grad); 
  //return -normalize(grad); 
}

Cube createCube()
{
  // The length of a cube edge.
  float d = 1.0/voxels_per_block;

  // Create cube corner coordinates.
  vec3 p0 = gl_in[0].gl_Position.xyz*d;
  vec3 p1 = gl_in[0].gl_Position.xyz*d + vec3(0.0 ,   d , 0.0);
  vec3 p2 = gl_in[0].gl_Position.xyz*d + vec3(d   ,   d , 0.0);
  vec3 p3 = gl_in[0].gl_Position.xyz*d + vec3(d   , 0.0 , 0.0);
  vec3 p4 = gl_in[0].gl_Position.xyz*d + vec3(0.0 , 0.0 , d);
  vec3 p5 = gl_in[0].gl_Position.xyz*d + vec3(0.0 ,   d , d);
  vec3 p6 = gl_in[0].gl_Position.xyz*d + vec3(d   ,   d , d);
  vec3 p7 = gl_in[0].gl_Position.xyz*d + vec3(d   , 0.0 , d);

  // xyz = position, w = density. The density value is taken from the the diffuse3DTexture alpha value. 
  vec4 v0 = vec4(p0, texture(diffuse3DTexture,p0).w);  
  vec4 v1 = vec4(p1, texture(diffuse3DTexture,p1).w);  
  vec4 v2 = vec4(p2, texture(diffuse3DTexture,p2).w);  
  vec4 v3 = vec4(p3, texture(diffuse3DTexture,p3).w);  
  vec4 v4 = vec4(p4, texture(diffuse3DTexture,p4).w);  
  vec4 v5 = vec4(p5, texture(diffuse3DTexture,p5).w);  
  vec4 v6 = vec4(p6, texture(diffuse3DTexture,p6).w);  
  vec4 v7 = vec4(p7, texture(diffuse3DTexture,p7).w);  

  // Normals.
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

vec3 interpolateV(vec4 va, vec4 vb)
{
   if (abs(0.5 - va.w) < 0.00001) { return va.xyz; }
   else if (abs(0.5 - vb.w) < 0.00001) { return vb.xyz; }
   else if (abs(va.w-vb.w) < 0.00001) { return va.xyz; }
   
   else
   {
     vec3 p;
     float mu = (0.5 - va.w) / (vb.w - va.w);
     p.x = va.x + mu * (vb.x - va.x);
     p.y = va.y + mu * (vb.y - va.y);
     p.z = va.z + mu * (vb.z - va.z);
     return p;
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
        fNormalIn = c.n0;
      EmitVertex();
    }           
    else if (abs(edgeValue - iterator) < 0.000001)
    {
      gl_Position =  MVP * vec4(interpolateV(c.v1, c.v2),1.0);
      //fColorIn = vec3(0.0,1.0,0.0);
      fPosIn = interpolateV(c.v1, c.v2);
      fNormalIn = c.n1;
      EmitVertex();
    }           
    else if (abs(edgeValue - iterator * 2.0) < 0.000001)
    //else if (edgeValue == iterator*2.0)
    {
      gl_Position =  MVP * vec4(interpolateV(c.v2, c.v3),1.0);
      fPosIn = interpolateV(c.v2, c.v3);
      //fColorIn = vec3(0.0,0.0,1.0);
      fNormalIn = c.n2;
      EmitVertex();
    }           
    else if (abs(edgeValue - iterator * 3.0) < 0.000001)
    {
      gl_Position =  MVP * vec4(interpolateV(c.v3, c.v0),1.0);
      fPosIn = interpolateV(c.v3, c.v0);
      //fColorIn = vec3(0.0,0.3,0.3);
      fNormalIn = c.n3;
      EmitVertex();
    }           
    else if (abs(edgeValue - iterator * 4.0) < 0.000001)
    {
      gl_Position =  MVP * vec4(interpolateV(c.v4, c.v5),1.0);
      fPosIn = interpolateV(c.v4, c.v5);
      //fColorIn = vec3(0.3,0.3,0.0);
      fNormalIn = c.n4;
      EmitVertex();
    }           
    else if (abs(edgeValue - iterator * 5.0) < 0.000001)
    {
      gl_Position =  MVP * vec4(interpolateV(c.v5, c.v6),1.0);
      fPosIn = interpolateV(c.v5, c.v6);
      //fColorIn = vec3(0.0,0.7,0.2);
      fNormalIn = c.n5;
      EmitVertex();
    }           
    else if (abs(edgeValue - iterator * 6.0) < 0.000001)
    {
      gl_Position =  MVP * vec4(interpolateV(c.v6, c.v7),1.0);
      fPosIn = interpolateV(c.v6, c.v7);
      fNormalIn = c.n6;
      //fColorIn = vec3(1.0,0.0,1.0);
      EmitVertex();
    }           
    else if (abs(edgeValue - iterator * 7.0) < 0.000001)
    {
      gl_Position =  MVP * vec4(interpolateV(c.v7, c.v4),1.0);
      fPosIn = interpolateV(c.v7, c.v4);
      fNormalIn = c.n7;
      EmitVertex();
    }           
    else if (abs(edgeValue - iterator * 8.0) < 0.000001)
    {
      gl_Position =  MVP * vec4(interpolateV(c.v0, c.v4),1.0);
      fNormalIn = c.n0;
      fPosIn = interpolateV(c.v0, c.v4);
      //fColorIn = vec3(0.0,1.0,1.0);
      EmitVertex();
    }           
    else if (abs(edgeValue - iterator * 9.0) < 0.000001)
    {
      gl_Position =  MVP * vec4(interpolateV(c.v1, c.v5),1.0);
      fPosIn = interpolateV(c.v1, c.v5);
      fNormalIn = c.n1;
      //fColorIn = vec3(0.0,0.5,1.0);
      EmitVertex();
    }           
    else if (abs(edgeValue - iterator * 10.0) < 0.000001)
    {
      gl_Position =  MVP * vec4(interpolateV(c.v2, c.v6),1.0);
      fPosIn = interpolateV(c.v2, c.v6);
      fNormalIn = c.n2;
      //fColorIn = vec3(0.1,0.1,0.1);
      EmitVertex();
    }           
    else if (abs(edgeValue - iterator * 11.0) < 0.000001)
    {
      gl_Position =  MVP * vec4(interpolateV(c.v3, c.v7),1.0);
      fPosIn = interpolateV(c.v3, c.v7);
      fNormalIn = c.n3;
      //fColorIn = vec3(0.5,0.5,0.5);
      EmitVertex();
    }           
}

void main(){

        //Cube c = createCube(gl_in[0].gl_Position.xyz);
        Cube c = createCube();
        float mask = calculateCase(c); 

        // Check if cube is totally inside or outside of the surface.
        //if (mask == 0.0) return;
        //if (mask == 255.0) return;

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

        //float index = (mask * 15.0 + 0.5) / 1280.0;
        float index = (mask * 5.0) / 1280.0;

        // The first edge. 
        vec3 edges = texture(tri_table,index).rgb;

        // If the first edge is 1.0 (255), there is nothing to do.
        if (abs(edges.r - 1.0) < 0.000001) return;

        // Create 1. triable.
        createVertex(edges.r, c);
        createVertex(edges.g, c);
        createVertex(edges.b, c);

        EndPrimitive();

        float iterator = 1.0 / 1280;

        // The 2. edge. 
        edges = texture(tri_table,index+iterator).rgb;

        // If the first vertex number is 1.0 (normalized from 255), there is no more triables.
        if (abs(edges.r - 1.0) < 0.000001) return;

        // Create the 2.0 triable.
        createVertex(edges.r, c);
        createVertex(edges.g, c);
        createVertex(edges.b, c);

        EndPrimitive();

        // The 3. edge. 
        edges = texture(tri_table,index+iterator*2.0).rgb;

        if (abs(edges.r - 1.0) < 0.000001) return;

        createVertex(edges.r, c);
        createVertex(edges.g, c);
        createVertex(edges.b, c);

        EndPrimitive();

        // The 4. edge. 
        edges = texture(tri_table,index+iterator*3.0).rgb;

        if (abs(edges.r - 1.0) < 0.000001) return;

        createVertex(edges.r, c);
        createVertex(edges.g, c);
        createVertex(edges.b, c);

        EndPrimitive();

        // The 5. edge. 
        edges = texture(tri_table,index+iterator*4.0).rgb;

        if (abs(edges.r - 1.0) < 0.000001) return;

        createVertex(edges.r, c);
        createVertex(edges.g, c);
        createVertex(edges.b, c);

        EndPrimitive();
}
