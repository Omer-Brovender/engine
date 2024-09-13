#include "GUI.hpp"

#include <GLFW/glfw3.h>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <pfd/portable-file-dialogs.h>

#include "Model.hpp"
#include "glm/detail/qualifier.hpp"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

static float trans_x = 0.0f;
static float trans_y = 0.0f;
static float trans_z = 0.0f;

static float scale_x = 1.0f;
static float scale_y = 1.0f;
static float scale_z = 1.0f;

static float rotate_x = 0.0f;
static float rotate_y = 0.0f;
static float rotate_z = 0.0f;
static float rotate_w = 1.0f;

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

    float gameWidthScale = 0.7f;

    // OpenGL Rendering Space
    if (ImGui::BeginChild("Game", ImVec2(displaySize.x * gameWidthScale, 0)))
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

    float heirarchyHeightScale = 0.6f;

    static int selected = -1; // Dummy value, since no model can have an index of -1
    ImGui::BeginGroup();
    Model* model;
    if (selected != -1) model = &models[selected];
    if (ImGui::BeginChild("Scene Hierarchy", ImVec2(0, displaySize.y * heirarchyHeightScale)))
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

            //*model = models[i];
            //model->externalTranslation = glm::vec3(trans_x, trans_y, trans_z);
            //model->externalScale       = glm::vec3(scale_x, scale_y, scale_z);
            //model->externalRotation    = glm::quat(rotate_w, rotate_x, rotate_y, rotate_z);
        }
        ImGui::EndChild();
    }
    if (ImGui::BeginChild("Property Editor", ImVec2(0, 0), ImGuiChildFlags_Border))
    {
        const char* title = "Property Editor";
        float windowWidth = ImGui::GetWindowSize().x;
        float windowHeight = ImGui::GetWindowSize().y;
        float textWidth = ImGui::CalcTextSize(title).x;

        ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
        ImGui::Text(title);
        ImGui::Separator();

        if (selected != -1)
        {
            float dragSize = 0.2f;

            if (ImGui::BeginChild("Translation", ImVec2(0, windowHeight/3.0f)))
            {
                ImGui::Text("Translation:");

                ImGui::PushItemWidth(windowWidth * dragSize);
                ImGui::DragFloat("X", &model->externalTranslation.x, 0.01f, 0.0f, 0.0f, "%.2f");
                ImGui::SameLine();
                ImGui::DragFloat("Y", &model->externalTranslation.y, 0.01f, 0.0f, 0.0f, "%.2f");
                ImGui::SameLine();
                ImGui::DragFloat("Z", &model->externalTranslation.z, 0.01f, 0.0f, 0.0f, "%.2f");
                ImGui::PopItemWidth();
                
                ImGui::EndChild();
            }

            if (ImGui::BeginChild("Scale", ImVec2(0, windowHeight/3.0f)))
            {
                ImGui::Text("Scale:");

                ImGui::PushItemWidth(windowWidth * dragSize);
                ImGui::DragFloat("X", &model->externalScale.x, 0.01f, 0.0f, 0.0f, "%.2f");
                ImGui::SameLine();
                ImGui::DragFloat("Y", &model->externalScale.y, 0.01f, 0.0f, 0.0f, "%.2f");
                ImGui::SameLine();
                ImGui::DragFloat("Z", &model->externalScale.z, 0.01f, 0.0f, 0.0f, "%.2f");
                ImGui::PopItemWidth();

                ImGui::EndChild();
            }

            dragSize = 0.15f;
            if (ImGui::BeginChild("Rotation", ImVec2(0, 0)))
            {
                ImGui::Text("Rotation:");

                ImGui::PushItemWidth(windowWidth * dragSize);
                ImGui::DragFloat("X", &model->externalRotation.x, 0.01f, 0.0f, 0.0f, "%.2f");
                ImGui::SameLine();
                ImGui::DragFloat("Y", &model->externalRotation.y, 0.01f, 0.0f, 0.0f, "%.2f");
                ImGui::SameLine();
                ImGui::DragFloat("Z", &model->externalRotation.z, 0.01f, 0.0f, 0.0f, "%.2f");
                ImGui::SameLine();
                ImGui::DragFloat("W", &model->externalRotation.w, 0.01f, 0.0f, 0.0f, "%.2f");
                ImGui::PopItemWidth();

                ImGui::EndChild();
            }
        }

        ImGui::EndChild();
    }
    ImGui::EndGroup();
    
    ImGui::End();
}

void openFileMenu(ImGuiWindowFlags windowFlags, std::vector<Model>& models)
{
    ImGui::SetNextWindowSize(ImVec2(345, 345));
    if (ImGui::Begin("Open File", &fileMenu, windowFlags))
    {
        if (ImGui::BeginChild("Explanation", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())))
        {
            ImGui::Text("Select a .gltf file to load into the 3D Viewer!");
            ImGui::EndChild();
        }
        if (ImGui::BeginChild("File Select"))
        {
            if (ImGui::Button("Select File", ImVec2(100, 18)))
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

                if (!fileVec.empty() && fileVec[0].find_last_of(".gltf") != std::string::npos)
                {
                    auto f = fileVec[0];
                    models.push_back(Model(f.c_str()));
                }
            }
            ImGui::EndChild();
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