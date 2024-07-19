////
//// Created by lennart on 7/3/24.
////
//
//#include <memory>
//#include <glm/vec3.hpp>
//#include <random>
//#include <ctime>
//#include <glm/geometric.hpp>
//#include <iostream>
//#include "SnakeGame.h"
//#include "Buffer.h"
//
//namespace engine {
//
//    float SnakeGame::RandF(float min, float max) {
//
//    }
//
//    SnakeGame::SnakeGame(Device& device, uint32_t maxParticles)
//            : m_device(device), m_maxParticles(maxParticles) {
//        m_particles.reserve(maxParticles);
////        m_particles.emplace_back(glm::vec2(0), glm::vec2(0), 0.2);
////        m_particles.emplace_back(glm::vec2(-0.4, 0), glm::vec2(0.005775, 0), 0.01);
//        CreateVertexBuffer();
//
//    }
//
//    void SnakeGame::Update(float dt, const glm::vec2 &mousPos) {
//        if (m_particles.empty()) return;
//
//        if (m_apples.empty()) {
//            m_apples.emplace_back(glm::vec2(RandF(-1, 1), RandF(-1, 1)),
//                                  glm::vec2(0), 0.02);
//        }
//
//
//
//        UpdateVertexBuffer();
//    }
//
//    void SnakeGame::Render(VkCommandBuffer commandBuffer) {
//
//    }
//
//
//
//    void SnakeGame::SetNumParticles(uint32_t n) {
//        while (n > m_particles.size()) {
//            m_particles.push_back({
//                  glm::vec2(RandF(-1, 1), RandF(-1, 1)),
//                  glm::vec2(0),
//                  glm::vec4(1, 1, 1, 1),
//                  RandF(0.005, 0.01),
//                  RandF(1.0f, 5.0f),
//            });
//        }
//
//        while (n < m_particles.size() && m_particles.size() > 0) {
//            m_particles.pop_back();
//        }
//
//    }
//
//    void SnakeGame::SetPointForce(std::shared_ptr<PointForce> &force) {
//        m_pointForce = force;
//    }
//
//    void SnakeGame::AddParticle(const Particle &particle) {
//        if (m_particles.size() >= m_maxParticles) return;
//
//        m_particles.push_back(particle);
//    }
//
//      void SnakeGame::ApplyGravity(float dt) {
//        static const float G = 6.674300e-9f;
//        for (uint32_t i = 0; i < m_particles.size(); i++) {
//            for (uint32_t j = i + 1; j < m_particles.size(); ) {
//                float m1 = m_particles[i].size * 10000000;
//                float m2 = m_particles[j].size * 10000000;
//
//                glm::vec2 r = m_particles[j].position - m_particles[i].position;
//                float distanceSquared = glm::dot(r, r) + 1e-6f;
//                float distance = std::sqrt(distanceSquared);
//
//                glm::vec2 force = distance < 0.01 ? glm::vec2(0) : G * m1 * m2 * r / (distanceSquared * distance);
//
//                glm::vec2 a1 = force / m1;
//                glm::vec2 a2 = force / m2;
//
//                m_particles[i].velocity += a1 * dt;
//                m_particles[j].velocity -= a2 * dt;
//
//                float combinedRadius = m_particles[i].size + m_particles[j].size;
//                if (distance < combinedRadius) {
//                    // Merge particles
//                    float totalMass = m1 + m2;
//                    m_particles[i].position = (m_particles[i].position * m1 + m_particles[j].position * m2) / totalMass;
//                    m_particles[i].velocity = (m_particles[i].velocity * m1 + m_particles[j].velocity * m2) / totalMass;
//                    m_particles[i].size = std::cbrt(m_particles[i].size*m_particles[i].size*m_particles[i].size +
//                                                    m_particles[j].size*m_particles[j].size*m_particles[j].size);
//
//                    // Remove the j-th particle
//                    m_particles.erase(m_particles.begin() + j);
//
//                    // Don't increment j, as we've removed an element
//                } else {
//                    // Only increment j if we didn't merge
//                    ++j;
//                }
//            }
//        }
//    }
//
//    void SnakeGame::MakeLinked() {
//        m_linkedParticles.clear();
//        for (uint32_t i = 0; i + 1 < m_particles.size(); ++i) {
//            m_linkedParticles.emplace_back(m_particles[i], &m_particles[i + 1]);
//        }
//
//        if (!m_particles.empty()) {
//            m_linkedParticles.emplace_back(m_particles.back(), nullptr);
//        }
//    }
//
//    void SnakeGame::GameOver() const {
//        ImGui::Begin("Menu");
//        {
//            ImGui::Text("Game Over!!!");
//        }
//        ImGui::End();
//    }
//
//
//} // engine