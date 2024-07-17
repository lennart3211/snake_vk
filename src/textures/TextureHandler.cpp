#include "TextureHandler.h"
#include "descriptors/DescriptorPool.h"
#include "SwapChain.h"
#include "descriptors/DescriptorSetLayout.h"
#include "descriptors/DescriptorWriter.h"

namespace engine {
    TextureHandler::TextureHandler(Device &device, uint32_t maxTextures) : mDevice(device), mMaxTextures(maxTextures) {
        GeneratePool();
    }

    void TextureHandler::GeneratePool() {
        mDescriptorPool = DescriptorPool::Builder(mDevice)
                .setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
                .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, SwapChain::MAX_FRAMES_IN_FLIGHT * mMaxTextures)
                .build();
    }

    void TextureHandler::LoadTexture(const std::string &filepath) {
        assert(mTextures.size() < mMaxTextures && "Number of textures exceeds mMaxTextures!");
        auto texture = std::make_unique<TextureImage>(mDevice, filepath);
        mTextures.push_back(std::move(texture));
    }

    void TextureHandler::GenerateSetLayout() {
        mDescriptorSetLayout = DescriptorSetLayout::Builder(mDevice)
                .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, mMaxTextures)
                .build();
    }

    void TextureHandler::GenerateDescriptorSets() {
        VkDescriptorImageInfo *imageInfos = new VkDescriptorImageInfo[mTextures.size()];
        for (uint32_t i = 0; i < mTextures.size(); i++) {
            imageInfos[i].sampler = mTextures[i]->sampler();
            imageInfos[i].imageView = mTextures[i]->imageView();
            imageInfos[i].imageLayout = mTextures[i]->imageLayout();
        }

        for (auto & descriptorSet : mDescriptorSets) {
            DescriptorWriter(*mDescriptorSetLayout, *mDescriptorPool)
                    .writeImage(0, imageInfos, mTextures.size())
                    .build(descriptorSet);
        }
        delete[] imageInfos;
    }
} // engine