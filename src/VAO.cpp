#include "VAO.hpp"

VAO::VAO()
{
    glGenVertexArrays(1, &this->ID);
}

void VAO::linkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
    VBO.bind();
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
    VBO.unbind();
}

void VAO::bind()
{
    glBindVertexArray(this->ID);
}

void VAO::unbind()
{
    glBindVertexArray(0);
}

void VAO::destroy()
{
    glDeleteVertexArrays(1, &this->ID);
}