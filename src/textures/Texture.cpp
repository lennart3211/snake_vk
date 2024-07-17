//
// Created by lennart on 5/31/24.
//

#include <stdexcept>
#include "Texture.h"

namespace engine {
    Texture::Texture(Device &device,
                     const std::string &albedo, const std::string &roughness,
                     const std::string &normal, const std::string &metallic,
                     const std::string &ao, const std::string &height,
                     const std::string &specular, const std::string &emissive)
                     : m_device(device) {


        std::unique_ptr<TextureImage> texture;

        texture = std::make_unique<TextureImage>(m_device, albedo);
        m_textures.push_back(std::move(texture));

        texture = std::make_unique<TextureImage>(m_device, roughness);
        m_textures.push_back(std::move(texture));

        texture = std::make_unique<TextureImage>(m_device, normal);
        m_textures.push_back(std::move(texture));

        texture = std::make_unique<TextureImage>(m_device, metallic);
        m_textures.push_back(std::move(texture));

        texture = std::make_unique<TextureImage>(m_device, ao);
        m_textures.push_back(std::move(texture));

        texture = std::make_unique<TextureImage>(m_device, height);
        m_textures.push_back(std::move(texture));

        texture = std::make_unique<TextureImage>(m_device, specular);
        m_textures.push_back(std::move(texture));

        texture = std::make_unique<TextureImage>(m_device, emissive);
        m_textures.push_back(std::move(texture));

        for (int i = 0; i < uboBuffers.size(); i++) {
            uboBuffers[i] = std::make_unique<Buffer>(
                    m_device, sizeof(UvTransform), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uboBuffers[i]->map();
        }

        CreatePool();
        CreateSetLayout();
        CreateDescriptorSets();
    }

    void Texture::CreateSetLayout() {
        mDescriptorSetLayout = DescriptorSetLayout::Builder(m_device)
                .addBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
                .addBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
                .addBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
                .addBinding(3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
                .addBinding(4, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
                .addBinding(5, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
                .addBinding(6, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
                .addBinding(7, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
                .addBinding(8, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
                .addBinding(9, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
                .build();
    }

    void Texture::CreateDescriptorSets() {
        VkDescriptorImageInfo *imageInfos = new VkDescriptorImageInfo[m_textures.size()];
        for (uint32_t i = 0; i < m_textures.size(); i++) {
            imageInfos[i].sampler = m_textures[i]->sampler();
            imageInfos[i].imageView = m_textures[i]->imageView();
            imageInfos[i].imageLayout = m_textures[i]->imageLayout();
        }

        for (uint32_t i = 0; i < mDescriptorSets.size(); i++) {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            DescriptorWriter(*mDescriptorSetLayout, *mDescriptorPool)
                    .writeImage(0, imageInfos, m_textures.size())
                    .writeBuffer(9, &bufferInfo)
                    .build(mDescriptorSets[i]);
        }
        delete[] imageInfos;
    }

    void Texture::CreatePool() {
        mDescriptorPool = DescriptorPool::Builder(m_device)
                .setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
                .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, SwapChain::MAX_FRAMES_IN_FLIGHT * (m_textures.size() + 1))
                .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
                .build();
    }

    void Texture::Update(uint32_t frameIndex, UvTransform &uvTransform) const {
        uboBuffers[frameIndex]->writeToBuffer(&uvTransform);
        uboBuffers[frameIndex]->flush();
    }

    void Texture::UpdateImage(uint32_t textureIndex, const std::string &imagePath) {
        if (textureIndex >= m_textures.size()) {
            throw std::runtime_error("Invalid texture index");
        }

        std::unique_ptr<TextureImage> newTexture = std::make_unique<TextureImage>(m_device, imagePath);

        m_textures[textureIndex] = std::move(newTexture);

        UpdateDescriptorSets();
    }

    void Texture::UpdateDescriptorSets() {
        VkDescriptorImageInfo *imageInfos = new VkDescriptorImageInfo[m_textures.size()];
        for (uint32_t i = 0; i < m_textures.size(); i++) {
            imageInfos[i].sampler = m_textures[i]->sampler();
            imageInfos[i].imageView = m_textures[i]->imageView();
            imageInfos[i].imageLayout = m_textures[i]->imageLayout();
        }

        for (uint32_t i = 0; i < mDescriptorSets.size(); i++) {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            DescriptorWriter(*mDescriptorSetLayout, *mDescriptorPool)
                    .writeImage(0, imageInfos, m_textures.size())
                    .writeBuffer(8, &bufferInfo)
                    .overwrite(mDescriptorSets[i]);
        }
        delete[] imageInfos;
    }
}
