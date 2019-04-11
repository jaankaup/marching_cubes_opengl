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

const float ambiquousCases[107] = float[107](5.0 , 10.0 , 18.0 , 21.0 , 22.0 , 24.0 , 26.0 , 28.0 , 30.0 , 33.0 , 37.0 , 41.0 , 42.0 , 45.0 , 53.0 , 56.0 , 58.0 , 60.0 , 62.0 , 66.0 , 67.0 , 69.0 , 72.0 , 73.0 , 74.0 , 75.0 , 80.0 , 81.0 , 82.0 , 83.0 , 84.0 , 85.0 , 86.0 , 87.0 , 88.0 , 89.0 , 90.0 , 91.0 , 92.0 , 93.0 , 94.0 , 95.0 , 97.0 , 101.0 , 104.0 , 105.0 , 106.0 , 107.0 , 109.0 , 117.0 , 120.0 , 121.0 , 122.0 , 123.0 , 124.0 , 126.0 , 129.0 , 131.0 , 133.0 , 135.0 , 138.0 , 146.0 , 149.0 , 150.0 , 154.0 , 158.0 , 160.0 , 161.0 , 162.0 , 163.0 , 164.0 , 165.0 , 166.0 , 167.0 , 168.0 , 169.0 , 170.0 , 171.0 , 172.0 , 173.0 , 174.0 , 175.0 , 181.0 , 186.0 , 193.0 , 194.0 , 195.0 , 197.0 , 199.0 , 202.0 , 203.0 , 210.0 , 211.0 , 213.0 , 214.0 , 218.0 , 219.0 , 222.0 , 225.0 , 227.0 , 229.0 , 231.0 , 233.0 , 234.0 , 237.0 , 245.0 , 250.0);

// https://github.com/QianMo/GPU-Gems-Book-Source-Code/blob/master/GPU-Gems-3-CD-Content/content/01/demo/models/tables.nma


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

//        v5                        v6
//         +------------------------+
//        /|                       /|
//       / |                      / |
//      /  |                     /  |
//     /   |                    /   |  
//    /    |                   /    |
//v1 +------------------------+ v2  |
//   |     |                  |     |
//   |     |                  |     |
//   |     |                  |     |
//   |  v4 +------------------|-----+ v7
//   |    /                   |    /
//   |   /                    |   /
//   |  /                     |  /    
//   | /                      | /
//   |/                       |/
//   +------------------------+
//  v0                       v3
//
//ambiquous cases: v0, v2 = 1.0 + 4.0 = 5.0
//                 v1, v3 = 2.0 + 8.0 = 10.0
//                 v1, v3 = 2.0 + 8.0 = 10.0

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

vec3 interpolateN(vec3 na, vec3 nb, float densityA, float densityB)
{
   if (abs(0.5 - densityA) < 0.00001) { return na; }
   else if (abs(0.5 - densityB) < 0.00001) { return nb; }
   else if (abs(densityA-densityB) < 0.00001) { return na; }
   
   else
   {
     vec3 p;
     float mu = (0.5 - densityA) / (densityB - densityA);
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

bool marchCube(Cube c, float vpb)
{
        float mask = calculateCase(c); 

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
        if (abs(edges.r - 1.0) < 0.000001) return false;

        // Ambiquity cases?
        
//        for (int i=0 ; i<107 ; i++)
//        {
//          if (abs(mask - ambiquousCases[i]) < 0.001) return true;
////          c.v0 = vec4(10.0,10.0,10.0,1.0);
//        }

        // Create 1. triable.
        createVertex(edges.r, c);
        createVertex(edges.g, c);
        createVertex(edges.b, c);

        EndPrimitive();

        float iterator = 1.0 / 1280;

        // The 2. edge. 
        edges = texture(tri_table,index+iterator).rgb;

        // If the first vertex number is 1.0 (normalized from 255), there is no more triables.
        if (abs(edges.r - 1.0) < 0.000001) return false;

        // Create the 2. triable.
        createVertex(edges.r, c);
        createVertex(edges.g, c);
        createVertex(edges.b, c);

        EndPrimitive();

        // The 3. edge. 
        edges = texture(tri_table,index+iterator*2.0).rgb;

        if (abs(edges.r - 1.0) < 0.000001) return false;

        createVertex(edges.r, c);
        createVertex(edges.g, c);
        createVertex(edges.b, c);

        EndPrimitive();

        // The 4. edge. 
        edges = texture(tri_table,index+iterator*3.0).rgb;

        if (abs(edges.r - 1.0) < 0.000001) return false;

        createVertex(edges.r, c);
        createVertex(edges.g, c);
        createVertex(edges.b, c);

        EndPrimitive();

        // The 5. edge. 
        edges = texture(tri_table,index+iterator*4.0).rgb;

        if (abs(edges.r - 1.0) < 0.000001) return false;
        createVertex(edges.r, c);
        createVertex(edges.g, c);
        createVertex(edges.b, c);

        EndPrimitive();
        
        return false;
}

void main(){

        Cube c = createCube(gl_in[0].gl_Position,voxels_per_block);
        bool recursion = marchCube(c, voxels_per_block);
//        if (recursion)
//        {
//          float d = 1 / (voxels_per_block * 2.0);
//
//          vec3 p0 = c.v0.xyz;
//          vec3 p1 = c.v0.xyz + vec3(0.0 ,   d , 0.0);
//          vec3 p2 = c.v0.xyz + vec3(d   ,   d , 0.0);
//          vec3 p3 = c.v0.xyz + vec3(d   , 0.0 , 0.0);
//          vec3 p4 = c.v0.xyz + vec3(0.0 , 0.0 , d);
//          vec3 p5 = c.v0.xyz + vec3(0.0 ,   d , d);
//          vec3 p6 = c.v0.xyz + vec3(d   ,   d , d);
//          vec3 p7 = c.v0.xyz + vec3(d   , 0.0 , d);
// 
//          Cube c0 = createCube(vec4(p0,1.0), voxels_per_block * 2.0);
//          Cube c1 = createCube(vec4(p1,1.0), voxels_per_block * 2.0);
//          Cube c2 = createCube(vec4(p2,1.0), voxels_per_block * 2.0);
//          Cube c3 = createCube(vec4(p3,1.0), voxels_per_block * 2.0);
//          Cube c4 = createCube(vec4(p4,1.0), voxels_per_block * 2.0);
//          Cube c5 = createCube(vec4(p5,1.0), voxels_per_block * 2.0);
//          Cube c6 = createCube(vec4(p6,1.0), voxels_per_block * 2.0);
//          Cube c7 = createCube(vec4(p7,1.0), voxels_per_block * 2.0);
//            
//          bool recursion0 = marchCube(c0, voxels_per_block * 2.0);
//          bool recursion1 = marchCube(c1, voxels_per_block * 2.0);
//          bool recursion2 = marchCube(c2, voxels_per_block * 2.0);
//          bool recursion3 = marchCube(c3, voxels_per_block * 2.0);
//          bool recursion4 = marchCube(c4, voxels_per_block * 2.0);
//          bool recursion5 = marchCube(c5, voxels_per_block * 2.0);
//          bool recursion6 = marchCube(c6, voxels_per_block * 2.0);
//          bool recursion7 = marchCube(c7, voxels_per_block * 2.0);
//          float d2 = 1 / (voxels_per_block);

//          bool recursion0 = marchCube(c0, voxels_per_block * 2.0);
//          if (recursion0)
//          {
//
//          }
//          bool recursion1 = marchCube(c1, voxels_per_block * 2.0);
//          bool recursion2 = marchCube(c2, voxels_per_block * 2.0);
//          bool recursion3 = marchCube(c3, voxels_per_block * 2.0);
//          bool recursion4 = marchCube(c4, voxels_per_block * 2.0);
//          bool recursion5 = marchCube(c5, voxels_per_block * 2.0);
//          bool recursion6 = marchCube(c6, voxels_per_block * 2.0);
//          bool recursion7 = marchCube(c7, voxels_per_block * 2.0);
//        }
}
