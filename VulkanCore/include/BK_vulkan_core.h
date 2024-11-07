//
// Created by mac on 06/11/2024.
//

#ifndef GAMEENGINE_BK_VULKAN_CORE_H
#define GAMEENGINE_BK_VULKAN_CORE_H

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <vector>

namespace BK {
    class VulkanCore {
    public:
        VulkanCore();

        ~VulkanCore();

        void Init(const char *pAppName, GLFWwindow *window);

    private:
        std::vector<const char *> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        std::vector<const char *> getRequiredExtensions();

        bool checkValidationLayerSupport();

        void CreateInstance(const char *pAppName, VkAllocationCallbacks *allocator = nullptr);

        VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;

        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

        void setupDebugMessenger();


        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                            VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                            void *pUserData);


        VkInstance m_instance = nullptr;
        VkSurfaceKHR m_surface;

        void CreateSurface(GLFWwindow *pWindow);
    };
}


#endif //GAMEENGINE_BK_VULKAN_CORE_H
