#pragma once

#include "systems/SnakeGame.h"

#include <vulkan/vulkan.h>

#include <glm/glm.hpp>

namespace engine {

#define MAX_LIGHTS 10

    struct PointLight {
        glm::vec4 position{};
        glm::vec4 color{};
    };

    struct GlobalUbo {
        glm::mat4 projection{1.0f};
        glm::mat4 view{1.0f};
        glm::vec4 ambientLightColor{0.3f, 0.3f, 1.0f, .02f};
        PointLight pointLights[MAX_LIGHTS];
        int numLights;
    };

    struct FrameInfo {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        std::vector<VkDescriptorSet> descriptorSets;
        SnakeGame &particleSystem;
    };
}

