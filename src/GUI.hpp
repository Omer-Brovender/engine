#ifndef GUI_HPP
#define GUI_HPP

#include "Camera.hpp"
#include "FBO.hpp"
#include <GLFW/glfw3.h>

namespace GUI
{
    void init(GLFWwindow* window);
    void startFrame(GLFWwindow* window, Camera* cam, FBO& fbo);
    void endFrame();
    void shutdown();

    bool hoveringWindow();
    void handleMouse(GLFWwindow* window, Camera* cam);
};

#endif