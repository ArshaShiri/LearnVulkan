#ifndef SRC_COMMON_INCLUDE_PIPELINE
#define SRC_COMMON_INCLUDE_PIPELINE

#include <string>
#include <vector>

#include "device.hpp"

struct PipelineConfigInfo
{
    PipelineConfigInfo() = default;
    PipelineConfigInfo(const PipelineConfigInfo &) = delete;
    PipelineConfigInfo &operator=(const PipelineConfigInfo &) = delete;

    std::vector<VkVertexInputBindingDescription> bindingDescriptions{};
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
    VkPipelineViewportStateCreateInfo viewportInfo{};
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
    VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
    VkPipelineMultisampleStateCreateInfo multisampleInfo{};
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
    std::vector<VkDynamicState> dynamicStateEnables{};
    VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};

class Pipeline
{
  public:
    Pipeline(Device &device,
             const std::string &vertFilepath,
             const std::string &fragFilepath,
             const PipelineConfigInfo &configInfo);

    Pipeline(const Pipeline &) = delete;
    Pipeline &operator=(const Pipeline &) = delete;

    static void defaultPipelineConfigInfo(PipelineConfigInfo &configInfo);
    static void enableAlphaBlending(PipelineConfigInfo &configInfo);

    void bind(VkCommandBuffer commandBuffer);

    ~Pipeline();

  private:
    void createGraphicsPipeline(const std::string &vertFilepath,
                                const std::string &fragFilepath,
                                const PipelineConfigInfo &configInfo);

    void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

    Device &device_;
    VkPipeline graphicsPipeline_{};
    VkShaderModule vertexShaderModule_{};
    VkShaderModule fragmentShaderModule_{};
};

#endif /* SRC_COMMON_INCLUDE_PIPELINE */
