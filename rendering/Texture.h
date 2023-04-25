//
// Created by ryanm on 4/16/2023.
//

#ifndef MYPROJECT_TEXTURE_H
#define MYPROJECT_TEXTURE_H

#include <glad/glad.h>

class Texture2D {
public:
    // holds the ID of the texture object, used for all texture operations to reference this texture
    unsigned int ID;
    // texture size
    unsigned int Width, Height;
    //texture format
    unsigned int Internal_Format;
    unsigned int Image_Format;
    //configuration
    unsigned int Wrap_S; //wrapping mode on S axis
    unsigned int Wrap_T; // wrapping mode on T axis
    unsigned int Filter_Min; // filtering mode if texture pixels < screen pixels
    unsigned int Filter_Max; // filtering mode if texture pixels > screen pixels
    //constructor
    Texture2D();
    //generates texture from image data
    void Generate(unsigned int width, unsigned int height,unsigned char* data);
    //binds the texture as the current active GL_TEXTURE_2D texture object
    void Bind() const;
};


#endif //MYPROJECT_TEXTURE_H
