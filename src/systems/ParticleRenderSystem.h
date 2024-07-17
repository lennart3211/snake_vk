#pragma once

#include "Device.h"
#include "Pipeline.h"
#include "FrameInfo.h"

#include <memory>

namespace engine {

    class ParticleRenderSystem {
    private:
        Device &m_device;
        std::unique_ptr<Pipeline> m_pipeline;
        VkPipelineLayout m_pipelineLayout;

    public:
        ParticleRenderSystem(Device &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);

        ~ParticleRenderSystem();

        ParticleRenderSystem(const ParticleRenderSystem &) = delete;

        ParticleRenderSystem &operator=(const ParticleRenderSystem &) = delete;

        void RenderGameObjects(FrameInfo &frameInfo);

    private:
        void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout);

        void CreatePipeline(VkRenderPass renderPass);
    };

} // engine
