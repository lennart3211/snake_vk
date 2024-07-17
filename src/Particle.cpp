//
// Created by lennart on 7/5/24.
//

#include "Particle.h"

namespace engine {
    std::vector<VkVertexInputBindingDescription> Particle::getBindingDescription() {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Particle);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return bindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription> Particle::getAttributeDescriptions() {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

        attributeDescriptions.push_back({0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Particle, position)});
        attributeDescriptions.push_back({1, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Particle, velocity)});
        attributeDescriptions.push_back({2, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Particle, color)});
        attributeDescriptions.push_back({3, 0, VK_FORMAT_R32_SFLOAT, offsetof(Particle, size)});
        attributeDescriptions.push_back({4, 0, VK_FORMAT_R32_SFLOAT, offsetof(Particle, life)});

        return attributeDescriptions;
    }

    glm::vec2 LinkedParticle::distToChild() const {
        return particle.position - child->position;
    }
} // engine