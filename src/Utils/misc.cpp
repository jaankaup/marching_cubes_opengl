#include "misc.h"

void joopajoo()
{
  std::cout << "1234kljh1234lkjh" << std::endl;
}

std::string addPadding(const std::string& str, const uint32_t count, const char paddingChar)
{
  std::string result = str;
  if (count > str.size())
  {
    result.insert(0,count - str.size(), paddingChar);
  }
  return result;
}

//template <typename T>
//void logGLM(const std::string& name, const T& object) {}

void logGLM(const std::string& name, const glm::vec2& object)
{
  Log::getDebug().log("% = (%,%)",name,std::to_string(object.x),std::to_string(object.y));
}
   
void logGLM(const std::string& name, const glm::vec3& object)
{
  Log::getDebug().log("% = (%,%,%)",name,std::to_string(object.x),std::to_string(object.y),std::to_string(object.z));
}

void logGLM(const std::string& name, const glm::vec4& object)
{
  Log::getDebug().log("% = (%,%,%,%)",name,std::to_string(object.x),std::to_string(object.y),std::to_string(object.w));
}

void logGLM(const std::string& name, const glm::mat3& object)
{
  std::string padded = addPadding("",name.size(),' ');
  Log::getDebug().log("% = (%,%,%)",name   ,std::to_string(object[0][0]),std::to_string(object[0][1]),std::to_string(object[0][2]));
  Log::getDebug().log("%   (%,%,%)",padded ,std::to_string(object[1][0]),std::to_string(object[1][1]),std::to_string(object[1][2]));
  Log::getDebug().log("%   (%,%,%)",padded ,std::to_string(object[2][0]),std::to_string(object[2][1]),std::to_string(object[2][2]));
}

void logGLM(const std::string& name, const glm::mat4& object)
{
  std::string padded = addPadding("",name.size(),' ');
  Log::getDebug().log("% = (%,%,%,%)",name   ,std::to_string(object[0][0]),std::to_string(object[0][1]),std::to_string(object[0][2]),std::to_string(object[0][3]));
  Log::getDebug().log("%   (%,%,%,%)",padded ,std::to_string(object[1][0]),std::to_string(object[1][1]),std::to_string(object[1][2]),std::to_string(object[1][3]));
  Log::getDebug().log("%   (%,%,%,%)",padded ,std::to_string(object[2][0]),std::to_string(object[2][1]),std::to_string(object[2][2]),std::to_string(object[2][3]));
  Log::getDebug().log("%   (%,%,%,%)",padded ,std::to_string(object[3][0]),std::to_string(object[3][1]),std::to_string(object[3][2]),std::to_string(object[3][3]));
}

// Plug; fzf
TextureData createRandom3Ddata(const int width, const int height, const int depth)
{
  int size = width*height*depth*4;
  TextureData td(size,width,height,depth);
  auto data = td.getData();

  // Reinterpret the one dimensional array as multi-dimensional array.
  uint8_t (&multiArray)[width][height][depth][4] = *reinterpret_cast<uint8_t (*)[width][height][depth][4]>(data);

  //MyRandom<int> mr("kisuli");
  MyRandom<int> mr;
  mr.setDistribution(0,16);

//  MyRandom<int> mr2 ("hauveli");
  MyRandom<int> mr2;
  mr2.setDistribution(-12,12);

//  MyRandom<int> mr3 ("ohari");
  MyRandom<int> mr3;
  mr3.setDistribution(-12,12);
  bool dark = false;
  
  for (int z = 0; z < depth ; z++) {
  for (int y = 0; y < height ; y++){
  for (int x = 0; x < width ; x++) {
    int density = mr();
    multiArray[z][y][x][0] = 5 + y*2;
    multiArray[z][y][x][1] = 51; //  + mr3(); // * density*0.5f;
    multiArray[z][y][x][2] = 0 ; //density / 5; // std::clamp(abs(mr3() * density / 20 ),0,255) ;
    multiArray[z][y][x][3] = density ;
  }}};

//  for (int z = 0; z < depth ; z++) {
//  for (int y = 0; y < height ; y++) {
//  for (int x = 0; x < width ; x++) {
//    uint8_t density = mr();
//    int index = 4 * x + height*4 *  y + 4 * depth * depth * z; 
//    data[index] = 102;   
//    data[index+1] = 51 ;   
//    data[index+2] = 0;   
//    data[index+3] = mr();   
////    if (0.8 > 255.0f / (y+1.0)) data[index+3] = 255 ;
////    else data[index+3] = static_cast<uint8_t>(std::clamp(centerY(y)  + 34.0 * sin(index*0.3) + 40.0 * cos(index*0.8) ,0.0,255.0));  
////    if (height / (y+1.0f) < 0.5) data[index+3] = 0; 
////    data[index+3] = std::clamp(( 255.0f / (height*height)) * y + 10 * sin(1.0f * index / 3.5f),0.0,255.0);  ; // hah ? 255 : 0;   
//    hah = !hah;
//  }}};
  return std::move(td);
}

TextureData createChess3Ddata(const int width, const int height, const int depth)
{
  int size = width*height*depth*4;
  TextureData td(size,width,height,depth);
  auto data = td.getData();
//  auto texels = new unsigned char[size];
  bool hah = false;
//
//  MyRandom<int> mr;
//  mr.setDistribution(0,155);

  for (int i=0 ; i<width*height*depth ; i++)
  {
 //   auto value = mr();
//    Log::getDebug().log("index %: value == % ",std::to_string(i),std::to_string(mr()));
    data[i*4] =  static_cast<uint8_t>(240);// 0.5f; //   (i*1.0f/size)*255 < 255 ? (i*1.0f/size)*255 : 255;
    data[i*4+1] = 0; // 1.0f; //(i*1.0f/size) < 1.0f ? (i*1.0f/size) : 1.0f;
    data[i*4+2] = 0; // (i*1.0f/size)*255 < 255 ? (i*1.0f/size)*255 : 255;
    data[i*4+3] = hah ? 0 : 255;// (int)((i/float(size))*255); // mr(); 
//    data[i*4+2] = (uint8_t)(i*3); // (i*1.0f/size)*255 < 255 ? (i*1.0f/size)*255 : 255;
//    data[i*4+3] = (uint8_t)(i*3); // mr(); // hah ? 5 : 250;// (int)((i/float(size))*255); // mr(); 
    hah = !hah;
  }
  return std::move(td);
  //return td;
}

TextureData create2x2()
{

  TextureData td(2*2*2*4,2,2,2);
  auto data = td.getData();

  unsigned char r0 = 255;
  unsigned char g0 = 255;
  unsigned char b0 = 255;
  unsigned char a0 = 255;

  unsigned char r1 = 255;
  unsigned char g1 = 255;
  unsigned char b1 = 255;
  unsigned char a1 = 0;

  unsigned char r2 = 255;
  unsigned char g2 = 255;
  unsigned char b2 = 255;
  unsigned char a2 = 255;

  unsigned char r3 = 255;
  unsigned char g3 = 255;
  unsigned char b3 = 255;
  unsigned char a3 = 0;

  unsigned char r4 = 255;
  unsigned char g4 = 255;
  unsigned char b4 = 255;
  unsigned char a4 = 255;

  unsigned char r5 = 255;
  unsigned char g5 = 255;
  unsigned char b5 = 255;
  unsigned char a5 = 0;

  unsigned char r6 = 255;
  unsigned char g6 = 255;
  unsigned char b6 = 255;
  unsigned char a6 = 255;

  unsigned char r7 = 255;
  unsigned char g7 = 255;
  unsigned char b7 = 255;
  unsigned char a7 = 0;

  //        R                G              B                  A                 
  data[0] = r0  ; data[1] = g0  ; data[2] = b0   ; data[3]  = a0;  
  data[4] = r1  ; data[5] = g1  ; data[6] = b1   ; data[7]  = a1;

  data[8]  = r2 ; data[9]  = g2 ; data[10] = b2  ; data[11] = a2;  
  data[12] = r3 ; data[13] = g3 ; data[14] = b3  ; data[15] = a3;
  
  data[16] = r4 ; data[17] = g4 ; data[18] = b4  ; data[19] = a4;  
  data[20] = r5 ; data[21] = g5 ; data[22] = b5  ; data[23] = a5;

  data[24] = r6 ; data[25] = g6 ; data[26] = b6  ; data[27] = a6;  
  data[28] = r7 ; data[29] = g7 ; data[30] = b7  ; data[31] = a7;

  return std::move(td);
}


// y ~= 128
inline double centerY(int y)
{
    //return static_cast<uint8_t>(std::clamp(exp(0.02 * y*y),0.0, 255.0)) ;   
    return exp(0.005 * y*y) ;   
}
