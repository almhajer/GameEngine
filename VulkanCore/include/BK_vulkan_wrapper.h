//
// Created by mac on 09/11/2024.
//

#ifndef GAMEENGINE_BK_VULKAN_WRAPPER_H
#define GAMEENGINE_BK_VULKAN_WRAPPER_H

#include "vulkan/vulkan.h"

namespace BK {
    void BeginCommandBuffer(VkCommandBuffer CommandBuffet, VkCommandBufferUsageFlags UsageFlags);

    VkSemaphore CreateSemaphore(VkDevice Device);
}
#endif //GAMEENGINE_BK_VULKAN_WRAPPER_H
