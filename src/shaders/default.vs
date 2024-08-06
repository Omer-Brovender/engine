#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoords;

out vec3 currPos;
out vec3 Normal;
out vec3 color;
out vec2 texCoords;

uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 scale;
uniform mat4 rotation;

void main()
{
   currPos = vec3(model * translation * -rotation * scale * vec4(aPos, 1.0f));
   Normal = aNormal;
   color = aColor;
   texCoords = mat2(0.0, -1.0, 1.0, 0.0) * aTexCoords; // Rotate Textures by 90deg

   gl_Position = camMatrix * vec4(currPos, 1.0);
}