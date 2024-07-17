#pragma once

#include "Device.h"
#include "Utils.h"
#include "Buffer.h"

#include <vector>
#include <memory>



#include <glm/glm.hpp>
#include <vulkan/vulkan_core.h>

namespace engine {
    class Model {
    public:
        struct Vertex {
            glm::vec3 position{};
            glm::vec3 color{};
            glm::vec3 normal{};
            glm::vec2 uv{};
            int textureIndex;

            static std::vector<VkVertexInputBindingDescription>
            getBindingsDescriptions();

            static std::vector<VkVertexInputAttributeDescription>
            getAttributeDescriptions();

            bool operator==(const Vertex &other) const {
                return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
            }
        };

        struct Builder {
            std::vector<Vertex> vertices{};
            std::vector<uint32_t> indices{};

            void LoadModel(const std::string &filepath);
        };

        Model(Device &device, const Builder &builder);

        ~Model() = default;

        Model(const Model &) = delete;

        Model &operator=(const Model &) = delete;

        static std::unique_ptr<Model> CreateModelFromFile(Device &device, const std::string &filepath);

        void Bind(VkCommandBuffer commandBuffer);

        void Draw(VkCommandBuffer commandBuffer) const;

        glm::vec3 GetMinExtents() const;
        glm::vec3 GetMaxExtents() const;


    private:
        void CreateVertexBuffers(const std::vector<Vertex> &vertices);

        void CreateIndexBuffer(const std::vector<uint32_t> &indices);

        void FindMinMaxExtent(const std::vector<Vertex> &vertices);

        Device &m_Device;

        std::unique_ptr<Buffer> m_VertexBuffer;
        uint32_t m_VertexCount;

        bool m_HasIndexBuffer{false};
        std::unique_ptr<Buffer> m_IndexBuffer;
        uint32_t m_IndexCount;

        glm::vec3 mMinExtent, mMaxExtent;
    };
} // namespace engine
