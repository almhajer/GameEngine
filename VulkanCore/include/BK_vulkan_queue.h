//
// Created by mac on 10/11/2024.
//

#ifndef GAMEENGINE_BK_VULKAN_QUEUE_H
#define GAMEENGINE_BK_VULKAN_QUEUE_H

#include "vulkan/vulkan.h"

namespace BK {
    class VulkanQueue {
    public:
        VulkanQueue(){}

        ~VulkanQueue(){}

        void Init(VkDevice Device, VkSwapchainKHR Swapchain, uint32_t QueueFamily, uint32_t QueueIndex);

        void Destroy();

        uint32_t AcquireNextImage();

        void SubmitSync(VkCommandBuffer CmbBuf);

        void SubmitAsync(VkCommandBuffer CmbBuf);

        void Present(uint32_t ImageIndex);

        void WaitIdle();

    private:
        void CreateSemaphores();

        VkDevice m_device = VK_NULL_HANDLE;
        VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;
        VkQueue m_queue=VK_NULL_HANDLE;
        VkSemaphore m_renderCompleteSem;
        VkSemaphore m_presentCompleteSem;


    };


}
#endif //GAMEENGINE_BK_VULKAN_QUEUE_H
