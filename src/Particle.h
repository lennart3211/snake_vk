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
        alignas(8) glm::vec2 velocity;
        alignas(16) glm::vec4 color;
        float size;
        float life;

        Particle()
                : position(glm::vec2(0)), velocity(glm::vec2(0)), size(0), color(glm::vec4(1)), life(1) {}

        Particle(const glm::vec2 &position, const glm::vec2 &velocity, const float size)
            : position(position), velocity(velocity), size(size), color(glm::vec4(1)), life(1) {}

        Particle(const glm::vec2 &position, const glm::vec2 &velocity, const glm::vec4 &color, const float size, const float life)
                : position(position), velocity(velocity), size(size), color(color), life(life) {}

        static std::vector<VkVertexInputBindingDescription> getBindingDescription();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
    };

    struct LinkedParticle {
        Particle &particle;
        Particle *child;

        LinkedParticle(Particle &particle, Particle *child) : particle(particle), child(child) {}

        glm::vec2 distToChild() const;
    };

} // engine

#endif //ENGINE_PARTICLE_H
