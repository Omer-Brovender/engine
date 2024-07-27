#version 330 core
out vec4 FragColor;

in vec3 currPos;
in vec3 color;
in vec3 Normal;
in vec2 texCoords;

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

vec4 pointLight()
{
   vec3 lightVec = lightPos - currPos;
   float a = 3.0;
   float b = 0.7;
   float d = length(lightVec);
   float intensity = 1.0f/(a*d*d+b*d+1.0f);

   float ambient = 0.2f;

   vec3 normal = normalize(Normal);
   vec3 lightDirection = normalize(lightVec);
   float diffuse = max(dot(normal, lightDirection), 0.0f);

   float specularLight = 0.5f;
   vec3 viewDirection = normalize(camPos - currPos);
   vec3 reflectionDirection = reflect(-lightDirection, normal);
   float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
   float specular = specularAmount * specularLight;

   return (texture(diffuse0, texCoords) * (diffuse * intensity + ambient) + texture(specular0, texCoords).r * specular * intensity) * lightColor;
}

vec4 directLight()
{
   float ambient = 0.2f;

   vec3 normal = normalize(Normal);
   vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
   float diffuse = max(dot(normal, lightDirection), 0.0f);

   float specularLight = 0.5f;
   vec3 viewDirection = normalize(camPos - currPos);
   vec3 reflectionDirection = reflect(-lightDirection, normal);
   float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
   float specular = specularAmount * specularLight;

   return (texture(diffuse0, texCoords) * (diffuse + ambient) + texture(specular0, texCoords).r * specular) * lightColor;
}

vec4 spotLight()
{
   float outerCone = 0.90f;
   float innerCone = 0.95f;

   float ambient = 0.2f;

   vec3 normal = normalize(Normal);
   vec3 lightDirection = normalize(lightPos - currPos);
   float diffuse = max(dot(normal, lightDirection), 0.0f);

   float specularLight = 0.5f;
   vec3 viewDirection = normalize(camPos - currPos);
   vec3 reflectionDirection = reflect(-lightDirection, normal);
   float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
   float specular = specularAmount * specularLight;

   float angle = dot(lightDirection, normal);
   float intensity = clamp((angle - outerCone)/(innerCone - outerCone), 0.0f, 1.0f);

   return (texture(diffuse0, texCoords) * (diffuse * intensity + ambient) + texture(specular0, texCoords).r * specular * intensity) * lightColor;
}

void main()
{
   FragColor = pointLight();
}