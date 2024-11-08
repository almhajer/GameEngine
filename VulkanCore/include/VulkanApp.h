//
// Created by mac on 06/11/2024.
//

#ifndef GAMEENGINE_VULKANAPP_H
#define GAMEENGINE_VULKANAPP_H
#include "BK_vulkan_core.h"

class VulkanApp {
public:
    VulkanApp() {
    }

    ~VulkanApp() {
    }

    void Init(const char *pAppName,GLFWwindow* pWindow) {
        m_vkCore.Init(pAppName,pWindow);
    }

    void RenderScene() {
    }

private:
    BK::VulkanCore m_vkCore;

};
#endif //GAMEENGINE_VULKANAPP_H
