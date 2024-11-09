//
// Created by mac on 06/11/2024.
//
#include "BK_vulkan_core.h"
#include "BK_vulkan_util.h"
#include <iostream>
#include <assert.h>

namespace BK {
#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                          const VkAllocationCallbacks *pAllocator,
                                          VkDebugUtilsMessengerEXT *pDebugMessenger) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(
                instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                       const VkAllocationCallbacks *pAllocator) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(
                instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }

    VulkanCore::VulkanCore() {
    }


    std::vector<const char *> VulkanCore::getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        std::vector<const char *> requiredExtensions;

        for (uint32_t i = 0; i < glfwExtensionCount; i++) {
            requiredExtensions.emplace_back(glfwExtensions[i]);
        }
        requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
        requiredExtensions.emplace_back("VK_KHR_get_physical_device_properties2");
        if (enableValidationLayers) {
            requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        return requiredExtensions;
    }

    void VulkanCore::Init(const char *pAppName, GLFWwindow *pWindow) {
        CreateInstance(pAppName);
        setupDebugMessenger();
        CreateSurface(pWindow);
        m_physDevices.Init(m_instance, m_surface);
        m_queueFamily = m_physDevices.SelectDevice(VK_QUEUE_GRAPHICS_BIT, true);
        CreateDevice();
        CreateSwapChain();
        CreateCommandBufferPoll();
    }

    void VulkanCore::CreateInstance(const char *pAppName, VkAllocationCallbacks *allocator) {
        const VkApplicationInfo AppInfo{
                .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pNext = nullptr,
                .pApplicationName = pAppName,
                .applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
                .pEngineName = "",
                .engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
                .apiVersion = VK_API_VERSION_1_0
        };
        auto Extensions = getRequiredExtensions();
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        populateDebugMessengerCreateInfo(debugCreateInfo);
        VkInstanceCreateInfo createInfo = {
                .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                .pApplicationInfo = &AppInfo,
                .enabledExtensionCount = static_cast<uint32_t>(Extensions.size()),
                .ppEnabledExtensionNames = Extensions.data()
        };
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();

            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debugCreateInfo;
        } else {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
        VkResult result = vkCreateInstance(&createInfo, allocator, &m_instance);
        CHECK_VK_RESULT(result, "create instance");
        printf("vulkan instance createed : ");
        std::cout << reinterpret_cast<uintptr_t>(m_instance) << std::endl;
    }

    void VulkanCore::setupDebugMessenger() {
        if (!enableValidationLayers) return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        populateDebugMessengerCreateInfo(createInfo);

        if (CreateDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug messenger!");
        }
    }


    void VulkanCore::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                     VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL VulkanCore::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                             VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                             const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                             void *pUserData) {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }


    bool VulkanCore::checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char *layerName: validationLayers) {
            bool layerFound = false;

            for (const auto &layerProperties: availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }


    void VulkanCore::CreateSurface(GLFWwindow *pWindow) {
        if (glfwCreateWindowSurface(m_instance, pWindow, nullptr, &m_surface) != VK_SUCCESS) {
            fprintf(stderr, "Erorr Create Glfw surface \n");
        }
        printf("GLFW window surface create \n");
    }

    void VulkanCore::CreateDevice() {
        float qPriorities[] = {1.0f};
        VkDeviceQueueCreateInfo qInfo = {
                .sType=VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .pNext=nullptr,
                .flags=0,
                .queueFamilyIndex=m_queueFamily,
                .queueCount=1,
                .pQueuePriorities=&qPriorities[0]
        };
        std::vector<const char *> DevExts = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME,
                VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME,
                "VK_KHR_portability_subset"
        };
        if (m_physDevices.Selected().m_features.geometryShader == VK_FALSE) {
            printf("the Geomtery Shader is not Supported\n");
        }
        if (m_physDevices.Selected().m_features.tessellationShader == VK_FALSE) {
            printf("the tessellation Shader is not Supported\n");
        }
        VkPhysicalDeviceFeatures deviceFeatures = {};
        // deviceFeatures.geometryShader = VK_TRUE;
        //  deviceFeatures.tessellationShader = VK_TRUE;
        VkDeviceCreateInfo deviceCreateInfo = {
                .sType=VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
                .pNext=nullptr,
                .flags=0,
                .queueCreateInfoCount=1,
                .pQueueCreateInfos=&qInfo,
                .enabledLayerCount=0,
                .ppEnabledLayerNames=nullptr,
                .enabledExtensionCount=static_cast<uint32_t>(DevExts.size()),
                .ppEnabledExtensionNames=DevExts.data(),
                .pEnabledFeatures=&deviceFeatures
        };
        VkResult res = vkCreateDevice(m_physDevices.Selected().m_physDevice, &deviceCreateInfo, nullptr, &m_device);
        CHECK_VK_RESULT(res, "create device\n");
        printf("\nDevice created\n");
    }


    VkSurfaceFormatKHR
    VulkanCore::ChooseSurfaceFormatAndColorSpace(const std::vector<VkSurfaceFormatKHR> &SurfaceFormats) {
        for (int i = 0; i < SurfaceFormats.size(); ++i) {
            if ((SurfaceFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB) &&
                (SurfaceFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)) {
                return SurfaceFormats[i];
            }
        }
        return SurfaceFormats[0];
    }

    uint32_t VulkanCore::ChooseNumImages(const VkSurfaceCapabilitiesKHR &Capabilities) {
        uint32_t RequsetNumImages = Capabilities.minImageCount + 1;
        int FinalNumImages = 0;
        if ((Capabilities.maxImageCount > 0) && (RequsetNumImages > Capabilities.maxImageCount)) {
            FinalNumImages = Capabilities.maxImageCount;
        } else {
            FinalNumImages = RequsetNumImages;
        }
        return FinalNumImages;
    }

    VkPresentModeKHR VulkanCore::ChoosePresentMode(const std::vector<VkPresentModeKHR> &PresentModes) {
        for (int i = 0; i < PresentModes.size(); ++i) {
            if (PresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
                return PresentModes[i];
            }
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkImageView
    VulkanCore::CreateImageView(VkDevice Device, VkImage Image, VkFormat Format, VkImageAspectFlags AspectFlags,
                                VkImageViewType ViewType, uint32_t LayerCount, uint32_t mipLevels) {
        VkImageViewCreateInfo ViewInfo = {
                .sType=VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .pNext=nullptr,
                .flags=0,
                .image=Image,
                .viewType=ViewType,
                .format=Format,
                .components={
                        .r=VK_COMPONENT_SWIZZLE_IDENTITY,
                        .g=VK_COMPONENT_SWIZZLE_IDENTITY,
                        .b=VK_COMPONENT_SWIZZLE_IDENTITY,
                        .a=VK_COMPONENT_SWIZZLE_IDENTITY
                },
                .subresourceRange={
                        .aspectMask=AspectFlags,
                        .baseMipLevel=0,
                        .levelCount=mipLevels,
                        .baseArrayLayer=0,
                        .layerCount=LayerCount
                }
        };
        VkImageView ImageView;
        VkResult res = vkCreateImageView(Device, &ViewInfo, nullptr, &ImageView);
        CHECK_VK_RESULT(res, "vkCreateImageView");
        return ImageView;
    }

    void VulkanCore::CreateSwapChain() {
        const VkSurfaceCapabilitiesKHR &SurfaceCaps = m_physDevices.Selected().m_surfaceCaps;
        uint32_t NumImages = ChooseNumImages(SurfaceCaps);
        const std::vector<VkPresentModeKHR> &PresentModes = m_physDevices.Selected().m_presentModes;
        VkPresentModeKHR PresentMode = ChoosePresentMode(PresentModes);
        VkSurfaceFormatKHR SurfaceFormat = ChooseSurfaceFormatAndColorSpace(m_physDevices.Selected().m_surfaceFormats);
        VkSwapchainCreateInfoKHR SwapchainCreateInfo = {
                .sType=VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
                .pNext=nullptr,
                .flags=0,
                .surface=m_surface,
                .minImageCount=NumImages,
                .imageFormat=SurfaceFormat.format,
                .imageColorSpace=SurfaceFormat.colorSpace,
                .imageExtent=SurfaceCaps.currentExtent,
                .imageArrayLayers=1,
                .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
                .imageSharingMode=VK_SHARING_MODE_EXCLUSIVE,
                .queueFamilyIndexCount=1,
                .pQueueFamilyIndices=&m_queueFamily,
                .preTransform=SurfaceCaps.currentTransform,
                .compositeAlpha=VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
                .presentMode=PresentMode,
                .clipped=VK_TRUE
        };
        VkResult res = vkCreateSwapchainKHR(m_device, &SwapchainCreateInfo, nullptr, &m_swapChain);
        CHECK_VK_RESULT(res, "vkCreateSwapchainKHR\n");
        printf("Swap chain created\n");


        uint32_t NumSwapChainImages = 0;
        res = vkGetSwapchainImagesKHR(m_device, m_swapChain, &NumSwapChainImages, nullptr);
        assert(NumImages == NumSwapChainImages);
        printf("Number of Images %d\n", NumSwapChainImages);

        m_images.resize(NumSwapChainImages);
        m_imageViews.resize(NumSwapChainImages);
        res = vkGetSwapchainImagesKHR(m_device, m_swapChain, &NumSwapChainImages, m_images.data());
        int LayerCount = 1;
        int MipLevels = 1;
        for (uint32_t i = 0; i < NumSwapChainImages; ++i) {
            m_imageViews[i] = CreateImageView(m_device, m_images[i], SurfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT,
                                              VK_IMAGE_VIEW_TYPE_2D, LayerCount, MipLevels);
        }
    }

    void VulkanCore::CreateCommandBufferPoll() {
        VkCommandPoolCreateInfo cmdPoolCreateInfo = {
                .sType=VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
                .pNext=nullptr,
                .flags=0,
                .queueFamilyIndex=m_queueFamily
        };
        VkResult res = vkCreateCommandPool(m_device, &cmdPoolCreateInfo, nullptr, &m_cmdBufPool);
        CHECK_VK_RESULT(res, "vkCreateCommandPool\n");
        printf("command buffer pool created\n");

    }

    void VulkanCore::CreateCommandBuffers(uint32_t count, VkCommandBuffer *cmdBufs) {
        VkCommandBufferAllocateInfo cmdBufAllocInfo = {
                .sType=VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
                .pNext=nullptr,
                .commandPool=m_cmdBufPool,
                .level=VK_COMMAND_BUFFER_LEVEL_PRIMARY,
                .commandBufferCount=count
        };
        VkResult res = vkAllocateCommandBuffers(m_device, &cmdBufAllocInfo, cmdBufs);
        CHECK_VK_RESULT(res, "vkAllocateCommandBuffers\n");
        printf("%d command buffers created\n", count);

    }

    void VulkanCore::FreeCommandBuffers(uint32_t count, const VkCommandBuffer *pCmdBufs) {
        vkFreeCommandBuffers(m_device, m_cmdBufPool, count, pCmdBufs);

    }

    int VulkanCore::GetNumImages() const { return (int) m_images.size(); }

    const VkImage& VulkanCore::GetImage(int Index) const
    {
        if (Index >= m_images.size()) {
            printf("Invalid image index %d\n", Index);
            exit(1);
        }

        return m_images[Index];
    }
    VulkanCore::~VulkanCore() {
        printf("___________________________________________________________\n");
        for (uint32_t i = 0; i < m_imageViews.size(); i++) {
            vkDestroyImageView(m_device, m_imageViews[i], nullptr);
        }
        vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);
        vkDestroyDevice(m_device, nullptr);
        vkDestroySurfaceKHR(m_instance, m_surface, nullptr);

        DestroyDebugUtilsMessengerEXT(m_instance, debugMessenger, nullptr);
        vkDestroyInstance(m_instance, nullptr);
        printf("Vulkan instance destroyed\n");
    }
}
