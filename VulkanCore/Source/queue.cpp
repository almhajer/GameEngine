//
// Created by mac on 10/11/2024.
//
#include <cstdio>
#include "BK_vulkan_queue.h"
#include "BK_vulkan_wrapper.h"
#include "BK_vulkan_util.h"

namespace BK {

    void VulkanQueue::Init(VkDevice Device, VkSwapchainKHR Swapchain, uint32_t QueueFamily, uint32_t QueueIndex) {
        m_device = Device;
        m_swapChain = Swapchain;
        vkGetDeviceQueue(Device, QueueFamily, QueueIndex, &m_queue);
        printf("Queue Acquired\n");
        CreateSemaphores();
    }


    void VulkanQueue::Destroy() {

        vkDestroySemaphore(m_device, m_presentCompleteSem, nullptr);
        vkDestroySemaphore(m_device, m_renderCompleteSem, nullptr);

    }

    void VulkanQueue::CreateSemaphores() {
        m_presentCompleteSem = CreateSemaphore(m_device);
        m_renderCompleteSem = CreateSemaphore(m_device);
    }

    void VulkanQueue::WaitIdle() {
        vkQueueWaitIdle(m_queue);
    }

    uint32_t VulkanQueue::AcquireNextImage() {
        uint32_t ImageIndex = 0;
        VkResult res = vkAcquireNextImageKHR(m_device, m_swapChain, UINT64_MAX, m_presentCompleteSem, nullptr,
                                             &ImageIndex);
        CHECK_VK_RESULT(res, "vkAcquireNextImageKHR\n");
        return ImageIndex;

    }


    void VulkanQueue::SubmitSync(VkCommandBuffer CmbBuf) {
        VkSubmitInfo SubmitInfo{
                .sType=VK_STRUCTURE_TYPE_SUBMIT_INFO,
                .pNext=nullptr,
                .waitSemaphoreCount=0,
                .pWaitSemaphores=VK_NULL_HANDLE,
                .pWaitDstStageMask=VK_NULL_HANDLE,
                .commandBufferCount=1,
                .pCommandBuffers=&CmbBuf,
                .signalSemaphoreCount=0,
                .pSignalSemaphores=VK_NULL_HANDLE
        };
        VkResult res = vkQueueSubmit(m_queue, 1, &SubmitInfo, nullptr);
        CHECK_VK_RESULT(res, "vkQueueSubmit\n");
    }

    void VulkanQueue::SubmitAsync(VkCommandBuffer CmbBuf) {
        VkPipelineStageFlags waitFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo SubmitInfo = {
                .sType=VK_STRUCTURE_TYPE_SUBMIT_INFO,
                .pNext=nullptr,
                .waitSemaphoreCount=1,
                .pWaitSemaphores=&m_presentCompleteSem,
                .pWaitDstStageMask=&waitFlags,
                .commandBufferCount=1,
                .pCommandBuffers=&CmbBuf,
                .signalSemaphoreCount=1,
                .pSignalSemaphores=&m_renderCompleteSem
        };
        VkResult res = vkQueueSubmit(m_queue, 1, &SubmitInfo, nullptr);
        CHECK_VK_RESULT(res, "vkQueueSubmit\n");
    }

    void VulkanQueue::Present(uint32_t ImageIndex) {
        VkPresentInfoKHR PresentInfo = {
                .sType=VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
                .pNext=nullptr,
                .waitSemaphoreCount=1,
                .pWaitSemaphores=&m_renderCompleteSem,
                .swapchainCount=1,
                .pSwapchains=&m_swapChain,
                .pImageIndices=&ImageIndex
        };
        VkResult res= vkQueuePresentKHR(m_queue,&PresentInfo);
        CHECK_VK_RESULT(res,"vkQueuePresentKHR\n");
    }
}