#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include "VAO.hpp"
#include "VBO.hpp"
#include "Camera.hpp"
#include "Texture.hpp"

class Mesh 
{
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    VAO meshVAO;

    Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);

    void draw(Shader& shader, Camera& camera);
};

#endif