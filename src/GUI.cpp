#include "GUI.hpp"

#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

void render()
{
    // Window Configuration
    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_NoResize;

    // Display Size As Percentage
    ImVec2 displaySize = ImGui::GetMainViewport()->Size;

    float widthScale   = 0.4f;
    float heightScale  = 0.6f;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(displaySize.x * widthScale, displaySize.y * heightScale));

    // Titlebar & Initialization
    if (!ImGui::Begin("Loader", NULL, windowFlags))
    {
        ImGui::End();
        return;
    }
    ImGui::Text("dear imgui says hello! (%s) (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);

    ImGui::End();
}

void GUI::init(GLFWwindow* window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

void GUI::startFrame()
{
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    render();
}

void GUI::endFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::shutdown()
{
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui::DestroyContext();
}

bool GUI::hoveringWindow()
{
    return ImGui::IsWindowHovered();
}

void GUI::handleMouse(GLFWwindow* window, Camera* cam)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseButtonEvent(1, false);

    if (!io.WantCaptureMouse)
        cam->mouseInputs(window);
}