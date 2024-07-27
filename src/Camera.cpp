#include "Camera.hpp"
#include "GLFW/glfw3.h"
#include "GUI.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "glm/trigonometric.hpp"

Camera::Camera(glm::vec3 pos, int width, int height)
: pos(pos), width(width), height(height)
{}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);

    view = glm::lookAt(this->pos, this->pos + this->orientation, this->up);
    proj = glm::perspective(glm::radians(FOVdeg), (float)this->width/this->height, nearPlane, farPlane);

    this->camMatrix = proj * view;
}

void Camera::matrix(Shader& shader, const char* uniform)
{
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(this->camMatrix));
}

void Camera::inputs(GLFWwindow* window)
{
    // W, A, S, D, Space
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        this->pos += this->speed * this->orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        this->pos -= this->speed * this->orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        this->pos += this->speed * (-glm::normalize(glm::cross(this->orientation, this->up)));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        this->pos += this->speed * (glm::normalize(glm::cross(this->orientation, this->up)));
    }

    // Space, Control
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        this->pos += this->speed * this->up;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        this->pos += this->speed * -this->up;
    }

    // Ctrl 
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        this->speed = 0.04f;
    } 
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) 
    {
        this->speed = 0.01f;
    }

    GUI::handleMouse(window, this);

}

void Camera::mouseInputs(GLFWwindow* window)
{
    // Mouse
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (this->firstClick)
        {
            glfwSetCursorPos(window, this->width/2.0f, this->height/2.0f);
            this->firstClick = false;
        }

        double mouseX;
        double mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float rotX = this->sens * (float)(mouseY - (this->height/2.0f)) / height;
        float rotY = this->sens * (float)(mouseX - (this->width/2.0f)) / width;

        glm::vec3 newOrientation = glm::rotate(this->orientation, glm::radians(-rotX), glm::normalize(glm::cross(this->orientation, this->up)));

        if (!(glm::angle(newOrientation, this->up) <= glm::radians(5.0f)) && !(glm::angle(newOrientation, -this->up) <= glm::radians(5.0f)))
        {
            this->orientation = newOrientation;
        }

        this->orientation = glm::rotate(this->orientation, glm::radians(-rotY), this->up);

        glfwSetCursorPos(window, this->width/2.0f, this->height/2.0f);
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        this->firstClick = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}