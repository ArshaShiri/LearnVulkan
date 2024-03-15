#ifndef SRC_COMMON_INCLUDE_PIPELINE
#define SRC_COMMON_INCLUDE_PIPELINE

#include <string>
#include <vector>

#include <device.hpp>

struct PipelineConfigInfo
{
    PipelineConfigInfo() = default;
    PipelineConfigInfo(const PipelineConfigInfo &) = delete;
    PipelineConfigInfo &operator=(const PipelineConfigInfo &) = delete;

    VkViewport viewport;
    VkRect2D scissor;
    VkPipelineViewportStateCreateInfo viewportInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
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

    static void defaultPipelineConfigInfo(PipelineConfigInfo &configInfo, uint32_t width, uint32_t height);

    ~Pipeline();

  private:
    void createGraphicsPipeline(const std::string &vertFilepath,
                                const std::string &fragFilepath,
                                const PipelineConfigInfo &configInfo);

    void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

    Device &device_;
    VkPipeline graphicsPipeline_;
    VkShaderModule vertexShaderModule_;
    VkShaderModule fragmentShaderModule_;
};

#endif /* SRC_COMMON_INCLUDE_PIPELINE */
