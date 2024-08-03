#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "Shader.hpp"

class Texture
{
public:
    GLuint ID;
    const char* type;
    GLuint unit;
    Texture(const char* image, const char* texType, GLuint slot);

    void texUnit(Shader& shader, const char* uniform, GLuint unit);
    void bind();
    void unbind();
    void destroy();
};

#endif