#pragma once

#include <string>
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <glm/vec2.hpp>

#include "Buffer.h"
#include "Device.h"
#include "FrameInfo.h"
#include "SkyBox.h"
#include "SwapChain.h"
#include "TextureImage.h"
#include "descriptors/DescriptorPool.h"
#include "descriptors/DescriptorSetLayout.h"
#include "descriptors/DescriptorWriter.h"

namespace engine {
    struct UvTransform {
        glm::vec2 uvScale;
        glm::vec2 uvOffset;
        float uvRotation;
    };

    class Texture {
    public:
        Texture(Device &device,
                const std::string &albedo = "../textures/default/albedo.jpg",
                const std::string &roughness = "../textures/default/roughness.jpg",
                const std::string &normal = "../textures/default/normal.jpg",
                const std::string &metallic = "../textures/default/metallic.jpg",
                const std::string &ao = "../textures/default/ao.jpg",
                const std::string &height = "../textures/default/height.jpg",
                const std::string &specular = "../textures/default/specular.jpg",
                const std::string &emissive = "../textures/default/emissive.jpg");

        std::vector<VkDescriptorSet> GetDescriptorSets() { return mDescriptorSets; }
        std::shared_ptr<DescriptorSetLayout> GetDescriptorSetLayout() { return mDescriptorSetLayout; }

        void Update(uint32_t frameIndex, UvTransform &uvTransform) const;
        void UpdateImage(uint32_t textureIndex, const std::string &imagePath);
        void UpdateDescriptorSets();

        std::shared_ptr<TextureImage> albedo() { return m_textures[0]; }
        std::shared_ptr<TextureImage> roughness() { return m_textures[1]; }
        std::shared_ptr<TextureImage> normal() { return m_textures[2]; }
        std::shared_ptr<TextureImage> metallic() { return m_textures[3]; }
        std::shared_ptr<TextureImage> ao() { return m_textures[4]; }
        std::shared_ptr<TextureImage> height() { return m_textures[5]; }
        std::shared_ptr<TextureImage> specular() { return m_textures[6]; }
        std::shared_ptr<TextureImage> emissive() { return m_textures[7]; }

    private:
        Device &m_device;
        std::unique_ptr<DescriptorPool> mDescriptorPool{};
        std::vector<VkDescriptorSet> mDescriptorSets{SwapChain::MAX_FRAMES_IN_FLIGHT};
        std::shared_ptr<DescriptorSetLayout> mDescriptorSetLayout{};
        std::vector<std::shared_ptr<TextureImage>> m_textures;
        std::vector<std::unique_ptr<Buffer>> uboBuffers{SwapChain::MAX_FRAMES_IN_FLIGHT};

        void CreatePool();
        void CreateSetLayout();
        void CreateDescriptorSets();
    };

}
