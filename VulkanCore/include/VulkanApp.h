//
// Created by mac on 06/11/2024.
//

#ifndef GAMEENGINE_VULKANAPP_H
#define GAMEENGINE_VULKANAPP_H

#include "BK_vulkan_core.h"
#include "BK_vulkan_util.h"
#include "BK_vulkan_wrapper.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

class VulkanApp {
public:
    VulkanApp() {
    }

    ~VulkanApp() {
        m_vkCore.FreeCommandBuffers((uint32_t) m_cmdBufs.size(), m_cmdBufs.data());
        m_vkCore.DestroyFramebuffers(m_FrameBuffers);
        vkDestroyRenderPass(m_vkCore.GetDevice(), m_renderPass, nullptr);
    }

    void Init(const char *pAppName, GLFWwindow *pWindow) {
        m_vkCore.Init(pAppName, pWindow);
        m_numImages = m_vkCore.GetNumImages();
        m_pQueue = m_vkCore.GetQueue();
        m_renderPass = m_vkCore.CreateSimpleRenderPass();
        m_FrameBuffers = m_vkCore.CreateFramebuffer(m_renderPass);
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

/*
    void RecordCommandBuffers() {
        VkClearColorValue ClearColor = {1.0f, 0.0f, 0.0f, 1.0f};

        VkImageSubresourceRange ImageRange = {
                .aspectMask=VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel=0,
                .levelCount=1,
                .baseArrayLayer=0,
                .layerCount=1
        };
        for (int i = 0; i < m_cmdBufs.size(); ++i) {

            VkImageMemoryBarrier PresetToClearBarrier{
                    .sType=VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                    .pNext=nullptr,
                    .srcAccessMask=VK_ACCESS_MEMORY_READ_BIT,
                    .dstAccessMask=VK_ACCESS_TRANSFER_WRITE_BIT,
                    .oldLayout=VK_IMAGE_LAYOUT_UNDEFINED,
                    .newLayout=VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    .srcQueueFamilyIndex=VK_QUEUE_FAMILY_IGNORED,
                    .dstQueueFamilyIndex=VK_QUEUE_FAMILY_IGNORED,
                    .image=m_vkCore.GetImage(i),
                    .subresourceRange=ImageRange
            };
            VkImageMemoryBarrier ClearToPresentBarrier{
                    .sType=VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
                    .pNext=nullptr,
                    .srcAccessMask=VK_ACCESS_TRANSFER_WRITE_BIT,
                    .dstAccessMask=VK_ACCESS_MEMORY_READ_BIT,
                    .oldLayout=VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    .newLayout=VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                    .srcQueueFamilyIndex=VK_QUEUE_FAMILY_IGNORED,
                    .dstQueueFamilyIndex=VK_QUEUE_FAMILY_IGNORED,
                    .image=m_vkCore.GetImage(i),
                    .subresourceRange=ImageRange
            };

            BK::BeginCommandBuffer(m_cmdBufs[i], VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);


            vkCmdPipelineBarrier(m_cmdBufs[i],
                                 VK_PIPELINE_STAGE_TRANSFER_BIT,
                                 VK_PIPELINE_STAGE_TRANSFER_BIT,
                                 0,
                                 0,
                                 nullptr,
                                 0,
                                 nullptr,
                                 1,
                                 &PresetToClearBarrier);


            vkCmdClearColorImage(m_cmdBufs[i],
                                 m_vkCore.GetImage(i),
                                 VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &ClearColor,
                                 1,
                                 &ImageRange);


            vkCmdPipelineBarrier(m_cmdBufs[i], VK_PIPELINE_STAGE_TRANSFER_BIT,
                                 VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                                 0,
                                 0,
                                 nullptr,
                                 0,
                                 nullptr,
                                 1,
                                 &ClearToPresentBarrier);
            VkResult res = vkEndCommandBuffer(m_cmdBufs[i]);
            CHECK_VK_RESULT(res, "vkEndCommandBuffer \n");

        }
        printf("command bufers recorded\n");
    }
*/

    void RecordCommandBuffers() {
        VkClearColorValue ClearColor = {1.0f, 0.0f, 0.0f, 1.0f};
        VkClearValue ClearValue;
        ClearValue.color = ClearColor;
        VkRenderPassBeginInfo RenderPassBegininfo = {
                .sType=VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                .pNext=nullptr,
                .renderPass=m_renderPass,
                .renderArea={
                        .offset{
                                .x=0,
                                .y=0
                        },
                        .extent={
                                .width=WINDOW_WIDTH,
                                .height=WINDOW_HEIGHT
                        }
                },
                .clearValueCount=1,
                .pClearValues=&ClearValue
        };
        for (int i = 0; i < m_cmdBufs.size(); ++i) {
            BK::BeginCommandBuffer(m_cmdBufs[i], VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
            RenderPassBegininfo.framebuffer = m_FrameBuffers[i];

            vkCmdBeginRenderPass(m_cmdBufs[i], &RenderPassBegininfo, VK_SUBPASS_CONTENTS_INLINE);

            CHECK_VK_RESULT(vkEndCommandBuffer(m_cmdBufs[i]), "vkEndCommandBuffer \n");

        }


        printf("command bufers recorded\n");
    }

    BK::VulkanCore m_vkCore;
    BK::VulkanQueue *m_pQueue = nullptr;
    int m_numImages = 0;
    std::vector<VkCommandBuffer> m_cmdBufs;
    VkRenderPass m_renderPass;
    std::vector<VkFramebuffer> m_FrameBuffers;
};

#endif //GAMEENGINE_VULKANAPP_H
