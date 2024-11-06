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

        void Init(const char *pAppName);

    private:

        std::vector<const char*>getRequiredExtensions();
        void CreateInstance(const char *pAppName, VkAllocationCallbacks *allocator = nullptr);

        VkInstance m_instance = nullptr;

    };
}


#endif //GAMEENGINE_BK_VULKAN_CORE_H
