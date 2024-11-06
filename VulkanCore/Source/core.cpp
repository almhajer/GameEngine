//
// Created by mac on 06/11/2024.
//
#include "BK_vulkan_core.h"
#include "BK_vulkan_util.h"
#include <iostream>


namespace BK {

    VulkanCore::VulkanCore() {
    }

    VulkanCore::~VulkanCore() {
        vkDestroyInstance(m_instance, nullptr);
        printf("vulkan instance destroyed\n");
    }

    std::vector<const char *> VulkanCore::getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        std::vector<const char *> requiredExtensions;

        for (uint32_t i = 0; i < glfwExtensionCount; i++) {
            requiredExtensions.emplace_back(glfwExtensions[i]);
        }
        requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        requiredExtensions.emplace_back("VK_KHR_get_physical_device_properties2");
        return requiredExtensions;
    }

    void VulkanCore::Init(const char *pAppName) {
        CreateInstance(pAppName);
    }

    void VulkanCore::CreateInstance(const char *pAppName, VkAllocationCallbacks *allocator) {
        std::vector<const char *> Layers = {
                "VK_LAYER_KHRONOS_validation"
        };

/*        std::vector<const char *> Extensions = {
                VK_KHR_DISPLAY_EXTENSION_NAME,
                VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,
                "VK_KHR_get_physical_device_properties2",

#if defined(_WIN32)
                "VK_KHR_win32_surface",
#endif
#if defined(__APPLE__)
                "VK_MVK_macos_surface",
#endif
#if defined(__linux__)
                "VK_KHR_xcb_surface",
#endif
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
                VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
        };*/
        const VkApplicationInfo AppInfo{
                .sType=VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pNext=nullptr,
                .pApplicationName=pAppName,
                .applicationVersion=VK_MAKE_API_VERSION(0, 1, 0, 0),
                .pEngineName="",
                .engineVersion=VK_MAKE_API_VERSION(0, 1, 0, 0),
                .apiVersion=VK_API_VERSION_1_0
        };
        auto Extensions = getRequiredExtensions();

        VkInstanceCreateInfo createInfo = {
                .sType=VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                .pNext=nullptr,
                .flags=0,
                .pApplicationInfo=&AppInfo,
                .enabledLayerCount=static_cast<uint32_t >(Layers.size()),
                .ppEnabledLayerNames=Layers.data(),
                .enabledExtensionCount=static_cast<uint32_t>(Extensions.size()),
                .ppEnabledExtensionNames=Extensions.data()
        };
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
        VkResult result = vkCreateInstance(&createInfo, allocator, &m_instance);
        CHECK_VK_RESULT(result, "create instance");
        printf("vulkan instance createed : ");
        std::cout << reinterpret_cast<uintptr_t>(m_instance) << std::endl;
    }
}