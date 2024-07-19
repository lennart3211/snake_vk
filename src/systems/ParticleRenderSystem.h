#pragma once

#include "Device.h"
#include "Pipeline.h"
#include "FrameInfo.h"

#include <memory>

namespace engine {

    typedef uint32_t particle_id;

    class ParticleRenderSystem {
    private:
        Device &m_device;
        std::unique_ptr<Pipeline> m_pipeline;
        VkPipelineLayout m_pipelineLayout;

        std::vector<Particle> m_particles;
        const uint32_t m_maxParticles;

        std::unique_ptr<Buffer> m_vertexBuffer;

    public:
        ParticleRenderSystem(Device &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout, uint32_t maxParticles);

        ~ParticleRenderSystem();

        ParticleRenderSystem(const ParticleRenderSystem &) = delete;

        ParticleRenderSystem &operator=(const ParticleRenderSystem &) = delete;

        void Render(FrameInfo &frameInfo);

        Particle *AddParticle(const glm::vec2 &position, const glm::vec4 &color, float size);

        void RemoveParticle(const Particle *particle);

    private:
        void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void CreatePipeline(VkRenderPass renderPass);

        void CreateVertexBuffer();
        void UpdateVertexBuffer();

        void Bind(VkCommandBuffer commandBuffer);
    };

} // engine
