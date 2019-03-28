#pragma once

#include "Device.hpp"

#include <vulkan/vulkan.hpp>
#include <vma/vk_mem_alloc.h>

namespace TTauri { namespace GUI {


class Device_vulkan : public Device {
public:
    struct AllocateMemoryError : virtual Error {};
    struct NonVulkanWindowError : virtual Error {};

    vk::PhysicalDevice physicalIntrinsic;
    vk::Device intrinsic;
    vk::PhysicalDeviceType deviceType = vk::PhysicalDeviceType::eOther;
    vk::PhysicalDeviceMemoryProperties memoryProperties;

    VmaAllocator allocator;

    uint32_t graphicsQueueFamilyIndex = 0;
    uint32_t presentQueueFamilyIndex = 0;
    uint32_t computeQueueFamilyIndex = 0;
    uint32_t graphicsQueueIndex = 0;
    uint32_t presentQueueIndex = 0;
    uint32_t computeQueueIndex = 0;
    vk::Queue graphicsQueue;
    vk::Queue presentQueue;
    vk::Queue computeQueue;
    vk::CommandPool graphicsCommandPool;
    vk::CommandPool presentCommandPool;
    vk::CommandPool computeCommandPool;

    /*! List if extension required on this device.
     */
    std::vector<const char *> requiredExtensions;

    /*! Sorted list of queueFamilies and their capabilities.
     * score(window) must be called before initializeDevice(window);
     */
    std::vector<std::pair<uint32_t, uint8_t>> queueFamilyIndicesAndCapabilities;

    /*! Best surfae format.
     * score(window) must be called before initializeDevice(window);
     */
    vk::SurfaceFormatKHR bestSurfaceFormat = {};

    /*! Best surfae format.
     * score(window) must be called before initializeDevice(window);
     */
    vk::PresentModeKHR bestSurfacePresentMode = vk::PresentModeKHR::eFifo;

    Device_vulkan(vk::PhysicalDevice physicalDevice);
    ~Device_vulkan();

    Device_vulkan(const Device_vulkan &) = delete;
    Device_vulkan &operator=(const Device_vulkan &) = delete;
    Device_vulkan(Device_vulkan &&) = delete;
    Device_vulkan &operator=(Device_vulkan &&) = delete;

    bool updateAndRender(uint64_t nowTimestamp, uint64_t outputTimestamp, bool blockOnVSync) override;

    void initializeDevice(std::shared_ptr<Window> window) override;

    int score(std::shared_ptr<Window> window) override;

    /*! Find the minimum number of queue families to instantiate for a window.
     * This will give priority for having the Graphics and Present in the same
     * queue family.
     *
     * It is possible this method returns an incomplete queue family set. For
     * example without Present.
     */
    std::vector<std::pair<uint32_t, uint8_t>> findBestQueueFamilyIndices(std::shared_ptr<Window> window);

    std::pair<vk::Buffer, VmaAllocation> createBuffer(const vk::BufferCreateInfo &bufferCreateInfo, const VmaAllocationCreateInfo &allocationCreateInfo);

    void destroyBuffer(const vk::Buffer &buffer, const VmaAllocation &allocation);

    template <typename T>
    gsl::span<T> mapMemory(const VmaAllocation &allocation) {
        void *mapping;
        auto const result = static_cast<vk::Result>(vmaMapMemory(allocator, allocation, &mapping));

        VmaAllocationInfo allocationInfo;
        vmaGetAllocationInfo(allocator, allocation, &allocationInfo);

        T *mappingT = reinterpret_cast<T *>(mapping);
        auto const mappingSpan = gsl::span<T>(mappingT, allocationInfo.size / sizeof (T));

        return vk::createResultValue(result, mappingSpan, "TTauri::GUI::Device_vulkan::mapMemory");
    }

    void unmapMemory(const VmaAllocation &allocation);

};

}}