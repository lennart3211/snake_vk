#pragma once

#include <memory>
#include "TextureImage.h"
#include "descriptors/DescriptorPool.h"
#include "descriptors/DescriptorSetLayout.h"
#include "SwapChain.h"

namespace engine {

    class TextureHandler {
    public:
        TextureHandler(Device &device, uint32_t maxTextures);

        void GenerateSetLayout();
        void GenerateDescriptorSets();

        void LoadTexture(const std::string &filepath);

        std::vector<VkDescriptorSet> GetDescriptorSets() { return mDescriptorSets; }
        std::shared_ptr<DescriptorSetLayout> GetDescriptorSetLayout() { return mDescriptorSetLayout; }

    private:
        void GeneratePool();

        Device &mDevice;
        std::unique_ptr<DescriptorPool> mDescriptorPool{};
        std::vector<VkDescriptorSet> mDescriptorSets{SwapChain::MAX_FRAMES_IN_FLIGHT};
        std::shared_ptr<DescriptorSetLayout> mDescriptorSetLayout{};
        std::vector<std::unique_ptr<TextureImage>> mTextures;
        uint32_t mMaxTextures;
    };

} // engine
