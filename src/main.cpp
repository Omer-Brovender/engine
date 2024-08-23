#include "Mesh.hpp"
#include "GUI.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "Model.hpp"
#include "FBO.hpp"
#include <vector>

const unsigned int width = 800;
const unsigned int height = 800;


void glfw_error_callback(int error, const char* description);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSetErrorCallback(glfw_error_callback);
    
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window!\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL();

    glViewport(0, 0, width, height);

    GUI::init(window);

    Shader shaderProgram("../src/shaders/default.vs", "../src/shaders/default.fs");

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    shaderProgram.activate();
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    Camera camera(glm::vec3(0.0f, 0.0f, 2.0f), width, height);

    std::vector<Model> models;
    //models.push_back(Model("../models/sword/scene.gltf"));

    FBO fbo(width, height);

    glfwSwapBuffers(window);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        fbo.bind();
        GUI::startFrame(window, &camera, fbo, models);

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 10000.0f);

        for (int i = 0; i < models.size(); ++i)
            models[i].draw(shaderProgram, camera);

        fbo.unbind();
        GUI::endFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    shaderProgram.destroy();

    GUI::shutdown();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
