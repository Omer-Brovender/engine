#include "GUI.hpp"

#include <GLFW/glfw3.h>
#include <cstdio>
#include <iostream>
#include <pfd/portable-file-dialogs.h>

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

void openFileMenu(ImGuiWindowFlags windowFlags, std::vector<Model>& models);
static bool fileMenu = false;

void render(ImTextureID tex, GLFWwindow* window, Camera* cam, std::vector<Model>& models)
{
    // Window Configuration
    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_NoResize;
    windowFlags |= ImGuiWindowFlags_NoCollapse;
    windowFlags |= ImGuiWindowFlags_MenuBar;
    windowFlags |= ImGuiWindowFlags_NoDocking;

    // Display Size As Percentage
    ImVec2 displaySize = ImGui::GetMainViewport()->Size;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(displaySize.x, displaySize.y));

    // Titlebar & Initialization
    if (!ImGui::Begin("3D Viewer", NULL, windowFlags | ImGuiWindowFlags_NoBringToFrontOnFocus))
    {
        std::cout << "Error rendering with ImGui!\n";
        ImGui::End();
        return;
    }

    // Menu/Options Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open", "Ctrl+O")) fileMenu = true;
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    if (fileMenu) openFileMenu(windowFlags, models);

    float widthScale   = 0.7f;
    float heightScale  = 1.0f;

    // OpenGL Rendering Space
    if (ImGui::BeginChild("Game", ImVec2(displaySize.x * widthScale, 0)))
    {
        ImGui::Image
        (
            tex,
            ImGui::GetContentRegionAvail(),
            ImVec2(0, 1),
            ImVec2(1, 0)
        );
        if (ImGui::IsItemHovered())
        {
            cam->mouseInputs(window);
        }
        ImGui::EndChild();
    }
    ImGui::SameLine();

    static int selected = 0;
    if (ImGui::BeginChild("Scene Hierarchy", ImVec2(0, 0)))
    {
        const char* title = "Scene Hierarchy";
        float windowWidth = ImGui::GetWindowSize().x;
        float textWidth = ImGui::CalcTextSize(title).x;

        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        ImGui::Text(title);
        ImGui::Separator();

        for (int i = 0; i < models.size(); ++i)
        {
            char name[100];
            sprintf(name, "Model %d", i);
            ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
            if (ImGui::Selectable(name, selected == i))
                selected = i;
            ImGui::PopStyleVar();
        }
        ImGui::EndChild();
    }
    
    ImGui::End();
}

void openFileMenu(ImGuiWindowFlags windowFlags, std::vector<Model>& models)
{
    ImGui::SetNextWindowSize(ImVec2(500, 500));
    if (ImGui::Begin("Open File", &fileMenu, windowFlags))
    {
        ImGui::Text("Hello, world!");
        if (ImGui::Button("Select File"))
        {
            if (!pfd::settings::available())
            {
                std::cout << "Portable File Dialogs are not available on this platform.\n";
                return;
            }

            auto fileVec = pfd::open_file
            (
                "Choose model to read",
                pfd::path::home(),
                {"glTF Files (.gltf)", "*.gltf"},
                pfd::opt::none
            ).result();

            if (!fileVec.empty())
            {
                auto f = fileVec[0];
                models.push_back(Model(f.c_str()));
            }
        }
    }
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
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
}

void GUI::startFrame(GLFWwindow* window, Camera* cam, FBO& fbo, std::vector<Model>& models)
{
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    render((ImTextureID)(size_t)fbo.getFrameTex(), window, cam, models);
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