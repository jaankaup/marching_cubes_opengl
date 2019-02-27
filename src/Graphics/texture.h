#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <SOIL.h>
#include "../Utils/log.h"

class Texture
{
//    friend class TextureControl;
//    friend class TextureManager;

    public:
        Texture();
        ~Texture();
        void init();
        void use(const int unit) const;
        void bind() const;
        GLuint getID() const;
        void create(const std::string& fileloc);

    private:

        GLuint pId = 0;
};

#endif // TEXTURE_H
