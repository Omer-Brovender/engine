#include "VBO.hpp"
#include <vector>

VBO::VBO(std::vector<Vertex>& vertices)
{
    glGenBuffers(1, &this->ID);
    glBindBuffer(GL_ARRAY_BUFFER, this->ID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    //this->unbind();
}

void VBO::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->ID);
}

void VBO::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::destroy()
{
    glDeleteBuffers(1, &this->ID);
}