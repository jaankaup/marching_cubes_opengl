#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <vector>
#include <SOIL.h>
#include <GL/glew.h>
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

    private:
        void create(const std::string& fileloc);

        GLuint pId = 0;
};

#endif // TEXTURE_H
