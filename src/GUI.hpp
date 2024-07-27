#ifndef GUI_HPP
#define GUI_HPP

#include "Camera.hpp"
#include <GLFW/glfw3.h>

namespace GUI
{
    void init(GLFWwindow* window);
    void startFrame();
    void endFrame();
    void shutdown();

    bool hoveringWindow();
    void handleMouse(GLFWwindow* window, Camera* cam);
};

#endif