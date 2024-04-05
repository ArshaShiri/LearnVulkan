#ifndef SRC_COMMON_INCLUDE_RENDERER
#define SRC_COMMON_INCLUDE_RENDERER

#include <cassert>
#include <memory>

#include "device.hpp"
#include "swap_chain.hpp"
#include "window.hpp"

class Renderer
{
  public:
    Renderer(Window &window, Device &device);
    Renderer(const Renderer &) = delete;
    Renderer &operator=(const Renderer &) = delete;

    VkRenderPass getSwapChainRenderPass() const
    {
        return swapChain_->getRenderPass();
    }

    bool isFrameInProgress() const
    {
        return isFrameStarted_;
    }

    VkCommandBuffer getCurrentCommandBuffer() const
    {
        assert(isFrameStarted_ && "Cannot get command buffer when frame not in progress");
        return commandBuffers_[currentImageIndex_];
    }

    VkCommandBuffer beginFrame();
    void endFrame();
    void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

    ~Renderer();

  private:
    void createCommandBuffers();
    void freeCommandBuffers();
    void recreateSwapChain();

    Window &window_;
    Device &device_;
    std::unique_ptr<SwapChain> swapChain_{};
    std::vector<VkCommandBuffer> commandBuffers_{};
    uint32_t currentImageIndex_;
    bool isFrameStarted_{false};
};

#endif /* SRC_COMMON_INCLUDE_RENDERER */
