#ifndef MODEL_HPP
#define MODEL_HPP

#include <json/json.h>
#include <vector>
#include "Mesh.hpp"
#include "Texture.hpp"

using json = nlohmann::json;

class Model
{
public:
    Model(const char* file);
    void draw(Shader& shader, Camera& camera);

private:
    const char* file;
    std::vector<unsigned char> data;
    json JSON;

    std::vector<Mesh> meshes;

    std::vector<std::string> loadedTexName;
    std::vector<Texture> loadedTex;

    void loadMesh(unsigned int meshInd);

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