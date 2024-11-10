//
// Created by mac on 06/11/2024.
//

#ifndef GAMEENGINE_VULKANAPP_H
#define GAMEENGINE_VULKANAPP_H

#include "BK_vulkan_core.h"
#include "BK_vulkan_util.h"
#include "BK_vulkan_wrapper.h"
class VulkanApp {
public:
    VulkanApp() {
    }

    ~VulkanApp() {
        m_vkCore.FreeCommandBuffers((uint32_t) m_cmdBufs.size(), m_cmdBufs.data());
    }

    void Init(const char *pAppName, GLFWwindow *pWindow) {
        m_vkCore.Init(pAppName, pWindow);
        m_numImages = m_vkCore.GetNumImages();
        m_pQueue = m_vkCore.GetQueue();
        CreateCommandBuffers();
        RecordCommandBuffers();
    }

    void RenderScene() {
        uint32_t ImageIndex = m_pQueue->AcquireNextImage();
        m_pQueue->SubmitAsync(m_cmdBufs[ImageIndex]);
        m_pQueue->Present(ImageIndex);
    }

private:
    void CreateCommandBuffers() {
        m_cmdBufs.resize(m_numImages);
        m_vkCore.CreateCommandBuffers(m_numImages, m_cmdBufs.data());
    }

    void RecordCommandBuffers() {
        VkClearColorValue ClearColor = {1.0f, 0.0f, 0.0f, 1.0f};
        VkImageSubresourceRange ImageRange = {
                .aspectMask=VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel=0,
                .levelCount=1,
                .baseArrayLayer=0,
                .layerCount=1
        };
        for (uint i = 0; i < m_cmdBufs.size(); ++i) {
            BK::BeginCommandBuffer(m_cmdBufs[i], VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
            vkCmdClearColorImage(m_cmdBufs[i], m_vkCore.GetImage(i), VK_IMAGE_LAYOUT_GENERAL, &ClearColor, 1,
                                 &ImageRange);
            VkResult res = vkEndCommandBuffer(m_cmdBufs[i]);
            CHECK_VK_RESULT(res, "vkEndCommandBuffer \n");

        }
        printf("command bufers recorded\n");
    }

    BK::VulkanCore m_vkCore;
    int m_numImages = 0;
    std::vector<VkCommandBuffer> m_cmdBufs;
    BK::VulkanQueue* m_pQueue = NULL;
};

#endif //GAMEENGINE_VULKANAPP_H
