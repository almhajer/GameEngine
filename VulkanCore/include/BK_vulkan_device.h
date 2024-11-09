//
// Created by Abdulkafi on 07/11/2024.
//

#ifndef BK_VULKAN_DEVICE_H
#define BK_VULKAN_DEVICE_H

#include "vulkan/vulkan.h"
#include<vector>

namespace BK {
    struct PhysicalDevice {
        VkPhysicalDevice m_physDevice;
        VkPhysicalDeviceProperties m_devProps;
        std::vector<VkQueueFamilyProperties> m_qFamilyProps;
        std::vector<VkBool32> m_qSupportsPresent;
        std::vector<VkSurfaceFormatKHR> m_surfaceFormats;
        VkSurfaceCapabilitiesKHR m_surfaceCaps;
        VkPhysicalDeviceMemoryProperties m_memProps;
        std::vector<VkPresentModeKHR> m_presentModes;
        VkPhysicalDeviceFeatures m_features;

    };

    class VulkanPhysicalDevices {
    public:
        VulkanPhysicalDevices();

        ~VulkanPhysicalDevices();

        void Init(const VkInstance &Instance, const VkSurfaceKHR &Surface);

        uint32_t SelectDevice(VkQueueFlags RequiredQueueType, bool SupportsPresent);

        const PhysicalDevice &Selected() const;



    private:
        std::vector<PhysicalDevice> m_devices;
        int m_devIndex = -1;
    };


}


#endif //BK_VULKAN_DEVICE_H
