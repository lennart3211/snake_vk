#pragma once

#include "Device.h"


#include <string>
#include <array>

namespace engine {
class SkyBox {
public:
  SkyBox(Device &device, const std::array<std::string, 6> &filepaths);
  ~SkyBox();

  SkyBox(const SkyBox &) = delete;
  SkyBox &operator=(const SkyBox &) = delete;

  [[nodiscard]] VkSampler sampler() const { return mSampler; }
  [[nodiscard]] VkImageView imageView() const { return mImageView; }
  [[nodiscard]] VkImageLayout imageLayout() const { return mImageLayout; }
  [[nodiscard]] VkDescriptorImageInfo descriptorInfo() const { return {mSampler, mImageView, mImageLayout}; }
  [[nodiscard]] uint32_t width() const { return m_width; }
  [[nodiscard]] uint32_t height() const { return m_height; }

private:
  void transitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout, VkImageSubresourceRange subresourceRange);
  void generateMipmaps();

  int m_width, m_height, mipLevels;
  Device &mDevice;
  VkImage mImage;
  VkDeviceMemory mImageMemory;
  VkImageView mImageView;
  VkSampler mSampler;
  VkFormat mImageFormat;
  VkImageLayout mImageLayout;
};
}