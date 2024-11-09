//
// Created by mac on 09/11/2024.
//
#include "BK_vulkan_util.h"
#include "BK_vulkan_wrapper.h"
#include <cstdlib>
#include <cstdio>
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

}