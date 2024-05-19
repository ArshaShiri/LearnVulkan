#ifndef SRC_COMMON_INCLUDE_MODEL
#define SRC_COMMON_INCLUDE_MODEL

#include <vector>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "device.hpp"

class Model
{
  public:
    struct Vertex
    {
        glm::vec3 position{};
        glm::vec3 color{};

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
    };

    struct Builder
    {
        std::vector<Vertex> vertices{};
        std::vector<uint32_t> indices{};
    };

    Model(Device &device, const Model::Builder &builder);
    ~Model();

    Model(const Model &) = delete;
    Model &operator=(const Model &) = delete;

    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);

  private:
    void createVertexBuffers(const std::vector<Vertex> &vertices);
    void createIndexBuffers(const std::vector<uint32_t> &indices);

    Device &device_;
    VkBuffer vertexBuffer_;
    VkDeviceMemory vertexBufferMemory_;
    uint32_t vertexCount_;

    bool hasIndexBuffer_ = false;
    VkBuffer indexBuffer_;
    VkDeviceMemory indexBufferMemory_;
    uint32_t indexCount_;
};

#endif /* SRC_COMMON_INCLUDE_MODEL */
