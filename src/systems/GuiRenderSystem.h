#pragma once

#include "Device.h"
#include "Pipeline.h"
#include "FrameInfo.h"

#include <memory>


namespace engine {

    struct GuiElement {
        glm::vec2 position;
        glm::vec2 extent;
    };

    struct GuiInfo {
        VkCommandBuffer cmdBuffer;
        std::vector<GuiElement> guiElements;
    };

    class GuiRenderSystem {
    private:
        Device &m_device;
        std::unique_ptr<Pipeline> m_pipeline;
        VkPipelineLayout m_pipelineLayout;

    public:
        GuiRenderSystem(Device &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);

        ~GuiRenderSystem();

        GuiRenderSystem(const GuiRenderSystem &) = delete;

        GuiRenderSystem &operator=(const GuiRenderSystem &) = delete;

        void RenderElements(FrameInfo &frameInfo);

    private:
        void CreatePipelineLayout(VkDescriptorSetLayout globalSetLayout);

        void CreatePipeline(VkRenderPass renderPass);
    };

} // engine
