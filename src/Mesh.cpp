#include "Mesh.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/transform.hpp"
#include <string>

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures)
: 
vertices(vertices), 
indices(indices), 
textures(textures)
{
    this->meshVAO.bind();

    VBO VBO1(vertices);
    EBO EBO1(indices);

    VBO1.bind();
    EBO1.bind();
    
    this->meshVAO.linkAttrib(VBO1, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    this->meshVAO.linkAttrib(VBO1, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3*sizeof(float)));
    this->meshVAO.linkAttrib(VBO1, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6*sizeof(float)));
    this->meshVAO.linkAttrib(VBO1, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9*sizeof(float)));

    meshVAO.unbind();
    VBO1.unbind();
    EBO1.unbind();
}

void Mesh::draw
(
    Shader& shader, 
    Camera& camera,
    glm::mat4 matrix, 
    glm::vec3 translation,
    glm::vec3 scale,
    glm::quat rotation
)
{
    shader.activate();
    this->meshVAO.bind();

    unsigned int numDiffuse  = 0;
    unsigned int numSpecular = 0;

    for (unsigned int i = 0; i < this->textures.size(); ++i)
    {
        std::string num;
        std::string type = this->textures[i].type;
        if (type == "diffuse")
        {
            num = std::to_string(numDiffuse++);
        }
        else if (type == "specular")
        {
            num = std::to_string(numSpecular++);
        }
        textures[i].texUnit(shader, (type + num).c_str(), i);
        textures[i].bind();
    }
    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.pos.x, camera.pos.y, camera.pos.z);
    camera.matrix(shader, "camMatrix");

    glm::mat4 trans = glm::mat4(1.0f);
    glm::mat4 scl = glm::mat4(1.0f);
    glm::mat4 rot = glm::mat4(1.0f);

    trans = glm::translate(trans, translation);
    scl = glm::scale(scl, scale);
    rot = glm::mat4_cast(rotation);

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(scl));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));


    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
}
