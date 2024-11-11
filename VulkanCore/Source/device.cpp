//
// Created by Abdulkafi on 07/11/2024.
//

#include "BK_vulkan_device.h"
#include "BK_vulkan_util.h"
#include <assert.h>

namespace BK {
    static void PrintImageUserFlags(const VkImageUsageFlags &flags) {
        if (flags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) {
            printf("Image usage transfer src is supported\n");
        }
        if (flags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) {
            printf("Image usage transfer dest is supported\n");
        }
        if (flags & VK_IMAGE_USAGE_SAMPLED_BIT) {
            printf("Image usage SAMPLED  is supported\n");
        }
        if (flags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) {
            printf("Image usage ATTACHMENT  is supported\n");
        }
        if (flags & VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT) {
            printf("Image usage TRANSIENT ATTACHMENT is supported\n");
        }
        if (flags & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT) {
            printf("Image usage INPUT ATTACHMENT is supported\n");
        }
    }

    static void PrintMemoryProperty(VkMemoryPropertyFlags PropertyFlags) {
        if (PropertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
            printf("DEVICE LOCAL ");
        }

        if (PropertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
            printf("HOST VISIBLE ");
        }

        if (PropertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {
            printf("HOST COHERENT ");
        }

        if (PropertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) {
            printf("HOST CACHED ");
        }

        if (PropertyFlags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT) {
            printf("LAZILY ALLOCATED ");
        }

        if (PropertyFlags & VK_MEMORY_PROPERTY_PROTECTED_BIT) {
            printf("PROTECTED ");
        }
    }


    VulkanPhysicalDevices::VulkanPhysicalDevices() {
    }

    VulkanPhysicalDevices::~VulkanPhysicalDevices() {
    }

    void VulkanPhysicalDevices::Init(const VkInstance &Instance, const VkSurfaceKHR &Surface) {
        uint32_t NumDevices = 0;
        VkResult res = vkEnumeratePhysicalDevices(Instance, &NumDevices, nullptr);
        CHECK_VK_RESULT(res, "vkEnumeratePhysicalDevices error (1) \n");
        printf("Num physical devices %d\n\n", NumDevices);
        m_devices.resize(NumDevices);

        std::vector<VkPhysicalDevice> Devices;
        Devices.resize(NumDevices);
        res = vkEnumeratePhysicalDevices(Instance, &NumDevices, Devices.data());
        CHECK_VK_RESULT(res, "vkEnumeratePhysicalDevices error (2) \n");
        for (uint32_t i = 0; i < NumDevices; i++) {
            VkPhysicalDevice PhysDev = Devices[i];
            m_devices[i].m_physDevice = PhysDev;
            vkGetPhysicalDeviceProperties(PhysDev, &m_devices[i].m_devProps);

            printf("Device name: %s\n", m_devices[i].m_devProps.deviceName);
            uint32_t apiVer = m_devices[i].m_devProps.apiVersion;
            printf("     Api Version:%d.%d.%d.%d\n",
                   VK_API_VERSION_VARIANT(apiVer),
                   VK_API_VERSION_MAJOR(apiVer),
                   VK_API_VERSION_MINOR(apiVer),
                   VK_API_VERSION_PATCH(apiVer)
            );
            uint32_t NumQFamilies = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(PhysDev, &NumQFamilies, nullptr);
            printf("    Num of Family Queues %d\n", NumQFamilies);

            m_devices[i].m_qFamilyProps.resize(NumQFamilies);
            m_devices[i].m_qSupportsPresent.resize(NumQFamilies);

            vkGetPhysicalDeviceQueueFamilyProperties(PhysDev, &NumQFamilies, m_devices[i].m_qFamilyProps.data());


            for (uint32_t q = 0; q < NumQFamilies; q++) {
                const VkQueueFamilyProperties &QFamilyProp = m_devices[i].m_qFamilyProps[q];
                printf("    Family %d Num queue: %d ", q, QFamilyProp.queueCount);

                VkQueueFlags Flags = QFamilyProp.queueFlags;
                printf("    GFX %s ,    Compute %s ,Transfer %s, Sparse binding %s \n",
                       (Flags & VK_QUEUE_GRAPHICS_BIT) ? "YES" : "NO",
                       (Flags & VK_QUEUE_COMPUTE_BIT) ? "YES" : "NO",
                       (Flags & VK_QUEUE_TRANSFER_BIT) ? "YES" : "NO",
                       (Flags & VK_QUEUE_SPARSE_BINDING_BIT) ? "YES" : "NO"
                );

                res = vkGetPhysicalDeviceSurfaceSupportKHR(PhysDev, q, Surface, &(m_devices[i].m_qSupportsPresent[q]));
                CHECK_VK_RESULT(res, "vkGetPhysicalDeviceSurfaceSupportKHR error(2) \n");
            }
            uint32_t NumFormats = 0;
            res = vkGetPhysicalDeviceSurfaceFormatsKHR(PhysDev, Surface, &NumFormats, nullptr);
            CHECK_VK_RESULT(res, "   vkGetPhysicalDeviceSurfaceFormatsKHR (1)\n");
            assert(NumFormats > 0);

            m_devices[i].m_surfaceFormats.resize(NumFormats);
            res = vkGetPhysicalDeviceSurfaceFormatsKHR(PhysDev, Surface, &NumFormats,
                                                       m_devices[i].m_surfaceFormats.data());
            CHECK_VK_RESULT(res, "vkGetPhysicalDeviceSurfaceFormatsKHR (2)\n");
            for (uint32_t j = 0; j < NumFormats; ++j) {
                const VkSurfaceFormatKHR SurfaceFormat = m_devices[i].m_surfaceFormats[j];
                printf("    Format %x color space %x\n", SurfaceFormat.format, SurfaceFormat.colorSpace);
            }
            res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(PhysDev, Surface, &(m_devices[i].m_surfaceCaps));
            CHECK_VK_RESULT(res, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR \n");
            PrintImageUserFlags(m_devices[i].m_surfaceCaps.supportedUsageFlags);

            uint32_t NumPresentModes = 0;
            res = vkGetPhysicalDeviceSurfacePresentModesKHR(PhysDev, Surface, &NumPresentModes, nullptr);
            CHECK_VK_RESULT(res, "vkGetPhysicalDeviceSurfacePresentModesKHR(1) errors\n");


            assert(NumPresentModes != 0);

            m_devices[i].m_presentModes.resize(NumPresentModes);

            res = vkGetPhysicalDeviceSurfacePresentModesKHR(PhysDev, Surface, &NumPresentModes,
                                                            m_devices[i].m_presentModes.data());
            CHECK_VK_RESULT(res, "vkGetPhysicalDeviceSurfacePresentModesKHR (2) error \n");
            printf("NUmber of prsentation modes %d\n", NumPresentModes);


            vkGetPhysicalDeviceMemoryProperties(PhysDev, &(m_devices[i].m_memProps));

            printf("Num memory Types %d\n", m_devices[i].m_memProps.memoryTypeCount);
            for (uint32_t j = 0; j < m_devices[i].m_memProps.memoryTypeCount; ++j) {
                printf("%d flags %x hean %d ", j,
                       m_devices[i].m_memProps.memoryTypes[j].propertyFlags,
                       m_devices[i].m_memProps.memoryTypes[j].heapIndex);
                PrintMemoryProperty(m_devices[i].m_memProps.memoryTypes[j].propertyFlags);
                printf("\n");
            }
            printf("Num heap types %d\n", m_devices[i].m_memProps.memoryHeapCount);

            printf("\n");

        }
    }

    uint32_t VulkanPhysicalDevices::SelectDevice(VkQueueFlags RequiredQueueType, bool SupportsPresent) {
        for (uint32_t i = 0; i < m_devices.size(); ++i) {

            for (uint32_t j = 0; j < m_devices[i].m_qFamilyProps.size(); ++j) {
                const VkQueueFamilyProperties &QFamilyProp = m_devices[i].m_qFamilyProps[j];
                if ((QFamilyProp.queueFlags & RequiredQueueType) &&
                    ((bool) m_devices[i].m_qSupportsPresent[j] == SupportsPresent)) {
                    m_devIndex = i;
                    int QueueFamily = j;
                    printf("Using GFX device %d and queue family %d\n", m_devIndex, QueueFamily);
                    return QueueFamily;
                }
            }
        }
        printf("Required queue type %x and supports present %d not found\n", RequiredQueueType, SupportsPresent);
        return 0;
    }

    const PhysicalDevice &VulkanPhysicalDevices::Selected() const {
        if (m_devIndex < 0) {
            printf("A physical device has not been selected\n");
        }
        return m_devices[m_devIndex];
    }
}
