#include "texture.h"

Texture::Texture()
{
    Log::getDebug().log("Creating texture object.");
//  pType = t;
//  pUnit = unit;
  //    glGenTextures(1,&pId);
}

Texture::~Texture()
{
}

void Texture::init(const TextureType t)
{
    Log::getDebug().log("1. Texture::init(). Creating texture (%).", std::to_string(pId));

    GLuint i = 0;
    glGenTextures(1,&i);
    Log::getDebug().log("2. Texture::init(). Creating texture (%).", std::to_string(i));
    pType = t;
    pId = i;
}

void Texture::create3D()
{
    int width  = 16;
    int height = 16;
    int depth  = 16;
    int size = width*height*depth*3;

  auto texels = new unsigned char[size];
  bool hah = false;
  for (int i=0 ; i<size ; i = (i+1)*3)
  {
    texels[i] = 66;// 0.5f; //   (i*1.0f/size)*255 < 255 ? (i*1.0f/size)*255 : 255;
    texels[i+1] = i % 255; // 1.0f; //(i*1.0f/size) < 1.0f ? (i*1.0f/size) : 1.0f;
    texels[i+2] = hah ? 13 : 99; // (i*1.0f/size)*255 < 255 ? (i*1.0f/size)*255 : 255;
    hah = !hah;
  }
  use(0);
  //bind3D();
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_MIRRORED_REPEAT);
  glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB8, width, height, depth, 0, GL_RGB, GL_UNSIGNED_BYTE, texels);
  delete[] texels;
}

/* For more information: https://open.gl/textures */
void Texture::create(const std::string& fileloc)
{
    Log::getDebug().log("%","Texture::create(%): Creating texture from file.",fileloc);
    int width, height;
    unsigned char* image = SOIL_load_image(fileloc.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

    if (image == 0)
    {
        Log::getError().log("Error: Texture::create(%)", fileloc);
        Log::getError().log("%",SOIL_last_result());
//        throw std::runtime_error(SOIL_last_result());
    }

    /* HATARATKAISU: jos polku alkaa nain, niin bindataan tekstuuri texture unit 1:seen. */
//    if (Misc::startswith(fileloc, "data/textures/displace"))
//    {
//        use(1);
//    }
//    else use(0);
    use(0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);
}

void Texture::bind() const
{
  switch (pType)
  {
    case TextureType::d2:
      glBindTexture(GL_TEXTURE_2D,pId);
      break;
    case TextureType::d3:
      glBindTexture(GL_TEXTURE_3D,pId);
      break;
  }
}

void Texture::use(const GLuint unit) const
{
  glActiveTexture(GL_TEXTURE0 + unit);
  bind();
}

GLuint Texture::getID() const
{
    return pId;
}

void Texture::createExample2D()
{
    Log::getDebug().log("Texture::createExample2D()");
    int width =  32;
    int height = 32;
    int size = width*height*3;
    bool hah = true;
    auto image = new uint8_t[size];
    //unsigned char image[size];
    for (int i=0 ; i<size ; i = (i+1)*3)
    {
//      image[i] = 123;
//      image[i+1] = 123;
//      image[i+2] = 123;
      image[i] = 66;// 0.5f; //   (i*1.0f/size)*255 < 255 ? (i*1.0f/size)*255 : 255;
      image[i+1] = i % 255; // 1.0f; //(i*1.0f/size) < 1.0f ? (i*1.0f/size) : 1.0f;
      image[i+2] = hah ? 13 : 99; // (i*1.0f/size)*255 < 255 ? (i*1.0f/size)*255 : 255;
      hah = !hah;
//      Log::getDebug().log("% / % = %", std::to_string(i),std::to_string(size),std::to_string(i*1.0f/size));
//      Log::getDebug().log("tex: (%,%,%)", std::to_string(image[i]),std::to_string(image[i+1]),std::to_string(image[i+2]));
    }
    //unsigned char pixels[] = { 0, 0, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0 };
//    const unsigned char pixels[] = { 0, 0, 0, 255, 0, 0, 0, 0, 0, 0, 0, 0 };
    use(0);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    delete[] image;
}

void Texture::dispose() const
{
    if (pId != 0) glDeleteTextures(1,&pId);
}
