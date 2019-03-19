#pragma once

#include "Window.hpp"

#include <vulkan/vulkan.hpp>

namespace TTauri {
namespace GUI {
class Window_vulkan : public Window {
public:
    vk::SurfaceKHR intrinsic;

    vk::SwapchainCreateInfoKHR swapchainCreateInfo;

    vk::SwapchainKHR swapchain;

    std::vector<vk::Image> swapchainImages;
    std::vector<vk::ImageView> swapchainImageViews;
    std::vector<vk::Framebuffer> swapchainFramebuffers;

    vk::RenderPass firstRenderPass;
    vk::RenderPass followUpRenderPass;

    vk::Semaphore imageAvailableSemaphore;
    vk::Fence renderFinishedFence;

    Window_vulkan(std::shared_ptr<Delegate> delegate, const std::string &title, vk::SurfaceKHR surface);
    ~Window_vulkan() {}

    Window_vulkan(const Window_vulkan &) = delete;
    Window_vulkan &operator=(const Window_vulkan &) = delete;
    Window_vulkan(Window_vulkan &&) = delete;
    Window_vulkan &operator=(Window_vulkan &&) = delete;

    void buildForDeviceChange() override;
    void teardownForDeviceChange() override;
    bool rebuildForSwapchainChange() override;

protected:
    bool render(bool blockOnVSync) override;

private:
    void buildSemaphores();
    void teardownSemaphores();
    std::pair<vk::SwapchainKHR, bool> buildSwapchain(vk::SwapchainKHR oldSwapchain = {});
    void teardownSwapchain();
    void buildRenderPasses();
    void teardownRenderPasses();
    void buildFramebuffers();
    void teardownFramebuffers();
    void buildPipelines();
    void teardownPipelines();

    void waitIdle();
    std::pair<uint32_t, vk::Extent2D> getImageCountAndImageExtent();
    bool isOnScreen();
};

}}