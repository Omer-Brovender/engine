#ifndef FBO_HPP
#define FBO_HPP

#include <glad/glad.h>

class FBO 
{
public:
    FBO(float width, float height);
    void rescale(float width, float height);
    GLuint getFrameTex();
    void bind();
    void unbind();
    void destroy();
private:
    GLuint fbo;
    GLuint tex;
    GLuint rbo;
};

#endif