//
// Created by mac on 09/11/2024.
//
#include "BK_vulkan_util.h"
#include "BK_vulkan_wrapper.h"


namespace BK {
    void BeginCommandBuffer(VkCommandBuffer CommandBuffet, VkCommandBufferUsageFlags UsageFlags) {
        VkCommandBufferBeginInfo BeginInfo = {
                .sType=VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
                .pNext=nullptr,
                .flags=UsageFlags,
                .pInheritanceInfo=nullptr
        };
        VkResult res = vkBeginCommandBuffer(CommandBuffet, &BeginInfo);
        CHECK_VK_RESULT(res, "vkBeginCommandBuffer\n");
    }

    VkSemaphore CreateSemaphore(VkDevice Device) {
        VkSemaphoreCreateInfo CreateInfo{
                .sType=VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
                .pNext=nullptr,
                .flags=0
        };
        VkSemaphore Semaphore;
        VkResult res = vkCreateSemaphore(Device, &CreateInfo, nullptr, &Semaphore);
        CHECK_VK_RESULT(res, "vkCreateSemaphore\n");
        return Semaphore;
    }
}