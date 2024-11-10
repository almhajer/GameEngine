#include <iostream>

#define GLFW_INCLUDE_VULKAN

#include "VulkanApp.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
GLFWwindow *window = nullptr;
#define APP_NAME "GAME ENGINE"


void GLFW_Keycallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS)) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main() {
    if (!glfwInit()) {
        std::runtime_error("Your device not supported Glfw3");
    }

    if (!glfwVulkanSupported()) {
        std::runtime_error("Your device not supported Vulkan");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        std::runtime_error("Your device not can Make Window");
    }
    glfwSetKeyCallback(window, GLFW_Keycallback);

    VulkanApp App;
    App.Init(APP_NAME,window);

    while (!glfwWindowShouldClose(window)) {
        App.RenderScene();
        glfwPollEvents();
    }

    glfwTerminate();
    std::cout << "Good Bye!" << std::endl;
    return 0;
}
