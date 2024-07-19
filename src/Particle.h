//
// Created by lennart on 7/5/24.
//

#ifndef ENGINE_PARTICLE_H
#define ENGINE_PARTICLE_H

#include <glm/vec2.hpp>
#include <vector>
#include <glm/vec4.hpp>
#include <vulkan/vulkan.h>

namespace engine {

    struct Particle {
        alignas(8) glm::vec2 position;
        alignas(8) glm::vec4 color;
        float size;

        Particle()
                : position(glm::vec2(0)), size(0), color(glm::vec4(1)) {}

        Particle(const glm::vec2 &position, const float size)
            : position(position), size(size), color(glm::vec4(1)) {}

        Particle(const glm::vec2 &position, const glm::vec4 &color, const float size)
                : position(position), size(size), color(color) {}

        static std::vector<VkVertexInputBindingDescription> getBindingDescription();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
    };

    struct LinkedParticle {
        Particle *particle;
        Particle *child;

        LinkedParticle(Particle *particle, Particle *child) : particle(particle), child(child) {}

        glm::vec2 distToChild() const;
    };

    struct Apple {
        Particle *particle;
    };

} // engine

#endif //ENGINE_PARTICLE_H
