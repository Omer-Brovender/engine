#include "Model.hpp"
#include "Shader.hpp"
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

Model::Model(const char* file)
: file(file)
{
    std::string fileData = getFileContents(file);
    this->JSON = json::parse(fileData);

    this->file = file;
    this->data = getData(); 
}

std::vector<unsigned char> Model::getData()
{
    std::string bytesText;
    std::string uri = this->JSON["buffers"][0]["uri"];

    std::string fileStr = std::string(this->file);
    std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
    bytesText = getFileContents((fileDirectory + uri).c_str());

    std::vector<unsigned char> data(bytesText.begin(), bytesText.end());
    return data;
}

std::vector<float> Model::getFloats(json accessor)
{
    std::vector<float> floatVec;

    unsigned int buffViewInd = accessor.value("bufferView", 1);
    unsigned int count = accessor["count"];
    unsigned int accByteOffset = accessor.value("byteOffset", 0);
    std::string type = accessor["type"];

    json bufferView = this->JSON["bufferViews"][buffViewInd];
    unsigned int byteOffset = bufferView["byteOffset"];

    unsigned int numPerVert;
    if (type == "SCALAR") numPerVert = 1;
    else if (type == "VEC2") numPerVert = 2;
    else if (type == "VEC3") numPerVert = 3;
    else if (type == "VEC4") numPerVert = 4;
    else throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3, or VEC4)");

    unsigned int beginningOfData = byteOffset + accByteOffset;
    unsigned int lengthOfData = count * 4 * numPerVert;

    for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; )
    {
        unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };
        float value;
        std::memcpy(&value, bytes, sizeof(float));
        floatVec.push_back(value);
    }

    return floatVec;
}