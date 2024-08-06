#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include "VAO.hpp"
#include "VBO.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include "glm/fwd.hpp"

class Mesh 
{
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    VAO meshVAO;

    Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);

    void draw
    (
        Shader& shader, 
        Camera& camera, 
        glm::mat4 matrix = glm::mat4(1.0f), 
        glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f),
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f)

    );
};

#endif