#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "Device.h"
#include "Buffer.h"
#include "QuadTree.h"
#include "Particle.h"

#define DEBUG(x) std::cout << x << '\n';

namespace engine {

    struct PointForce {
        glm::vec2 location;
        float strength;

        PointForce(const glm::vec2 &location, float strength): location(location), strength(strength) {}
    };

    class SnakeGame {
    public:


        SnakeGame(Device& device, uint32_t maxParticles);

        void Update(float dt, const glm::vec2 &mousPos);
        void Render(VkCommandBuffer commandBuffer);
        void GameOver() const;

        [[nodiscard]] uint32_t NumParticles() const { return m_linkedParticles.size(); }
        [[nodiscard]] bool Running() const { return m_running; }

        void SetNumParticles(uint32_t n);
        void AddParticle(const Particle &particle);
        void SetPointForce(std::shared_ptr<PointForce> &force);
        void MakeLinked();

    private:
        Device& m_device;
//        std::vector<Particle> m_particles;
        std::vector<Particle> m_apples;
        std::vector<LinkedParticle> m_linkedParticles;
        std::unique_ptr<QuadTree> m_quadTree;
        uint32_t m_maxParticles;
        uint32_t m_maxApples{1};
        float m_snakeSize{0.01};
        const float CONTAINER_RADIUS{0.5};

        bool m_running{true};

        float offset{0};

        std::shared_ptr<PointForce> m_pointForce;




        void ApplyGravity(float dt);

        float RandF(float min, float max);

    };

} // engine
