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
        m_vkCore.FreeCommandBuffers((uint32_t)m_cmdBufs.size(),m_cmdBufs.data());
    }

    void Init(const char *pAppName, GLFWwindow *pWindow) {
        m_vkCore.Init(pAppName, pWindow);
        m_numImages = m_vkCore.GetNumImages();
        CreateCommandBuffers();
    }

    void RenderScene() {
    }

private:
    void CreateCommandBuffers() {
        m_cmdBufs.resize(m_numImages);
        m_vkCore.CreateCommandBuffers(m_numImages, m_cmdBufs.data());
    }

    BK::VulkanCore m_vkCore;
    int m_numImages = 0;
    std::vector<VkCommandBuffer> m_cmdBufs;
};

#endif //GAMEENGINE_VULKANAPP_H
