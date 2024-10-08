#ifndef MODEL_HPP
#define MODEL_HPP

#include <json/json.h>
#include <vector>
#include "Mesh.hpp"
#include "Texture.hpp"
#include "glm/fwd.hpp"

using json = nlohmann::json;

class Model
{
public:
    Model(const char* file);
    void draw(Shader& shader, Camera& camera);

    glm::vec3 externalTranslation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 externalScale = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::quat externalRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

private:
    const char* file;
    std::vector<unsigned char> data;
    json JSON;

    std::vector<Mesh> meshes;
    std::vector<glm::vec3> translationsMeshes;
    std::vector<glm::vec3> scalesMeshes;
    std::vector<glm::quat> rotationsMeshes;
    std::vector<glm::mat4> matricesMeshes;

    std::vector<std::string> loadedTexName;
    std::vector<Texture> loadedTex;

    void loadMesh(unsigned int meshInd);
    void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

    std::vector<unsigned char> getData();
    std::vector<float> getFloats(json accessor);
    std::vector<GLuint> getIndices(json accessor);

    std::vector<Texture> getTextures();

    std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
    std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
    std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);

    std::vector<Vertex> assembleVertices
    (
        std::vector<glm::vec3> positions,
        std::vector<glm::vec3> normals,
        std::vector<glm::vec2> texUVs
    );

};

#endif