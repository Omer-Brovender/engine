#include "Texture.hpp"
#include "VAO.hpp"
#include <stdexcept>

Texture::Texture(const char* image, const char* texType, GLuint slot)
{
    this->type = texType;

    int widthImg, heightImg, numColChnl;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColChnl, 0);

    glGenTextures(1, &this->ID);
    glActiveTexture(GL_TEXTURE0 + slot);
    this->unit = slot;
    glBindTexture(GL_TEXTURE_2D, this->ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (numColChnl == 4)
        glTexImage2D
        (
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            widthImg,
            heightImg,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            bytes
        );
    else if (numColChnl == 3)
        glTexImage2D
        (
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            widthImg,
            heightImg,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            bytes
        );
    else if (numColChnl == 1)
        glTexImage2D
        (
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            widthImg,
            heightImg,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            bytes
        );
    else
     throw std::invalid_argument("Automatic Texture type recognition failed");

    
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);
    this->unbind();
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	shader.activate();
	glUniform1i(texUni, unit);
}

void Texture::bind()
{
    glActiveTexture(GL_TEXTURE0 + this->unit);
	glBindTexture(GL_TEXTURE_2D, this->ID);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::destroy()
{
	glDeleteTextures(1, &ID);
}