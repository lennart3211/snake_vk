#pragma once

#include "Device.h"


#include <string>

namespace engine {
    class TextureImage {
    public:
        TextureImage(Device &device, const std::string &filepath);
        ~TextureImage();

        TextureImage(const TextureImage &) = delete;
        TextureImage &operator=(const TextureImage &) = delete;

        [[nodiscard]] VkSampler sampler() const { return mSampler; }
        [[nodiscard]] VkImageView imageView() const { return mImageView; }
        [[nodiscard]] VkImageLayout imageLayout() const { return mImageLayout; }
        [[nodiscard]] uint32_t width() const { return m_width; }
        [[nodiscard]] uint32_t height() const { return m_height; }

    private:
        void transitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);
        void generateMipmaps();

        int m_width, m_height, m_mipLevels;
        Device &mDevice;
        VkImage mImage;
        VkDeviceMemory mImageMemory;
        VkImageView mImageView;
        VkSampler mSampler;
        VkFormat mImageFormat;
        VkImageLayout mImageLayout;
    };
}