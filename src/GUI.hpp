#ifndef GUI_HPP
#define GUI_HPP

#include "Camera.hpp"
#include "FBO.hpp"
#include <GLFW/glfw3.h>
#include <vector>
#include "Model.hpp"

namespace GUI
{
    void init(GLFWwindow* window);
    void startFrame(GLFWwindow* window, Camera* cam, FBO& fbo, std::vector<Model>& models);
    void endFrame();
    void shutdown();

    bool hoveringWindow();
};

#endif