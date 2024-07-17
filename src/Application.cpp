#include "Application.h"


#include "Imgui.h"
#include "imgui/imgui_stdlib.h"
#include "systems/SnakeGame.h"
#include "systems/ParticleRenderSystem.h"
#include <descriptors/DescriptorWriter.h>

#include <GLFW/glfw3.h>

#include <chrono>
#include <memory>

namespace engine {

    Application::Application() {
        mGlobalPool = DescriptorPool::Builder(mDevice)
                .setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
                .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
                .build();
      }

    Application::~Application() = default;

    void Application::run() {
        Imgui imgui{mWindow, mDevice, mRenderer.GetSwapChainRenderPass(), mRenderer.GetImageCount()};

        std::vector<std::unique_ptr<Buffer>> uboBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uboBuffers.size(); i++) {
            uboBuffers[i] = std::make_unique<Buffer>(
                    mDevice, sizeof(GlobalUbo), 1, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uboBuffers[i]->map();
        }

        auto globalSetLayout = DescriptorSetLayout::Builder(mDevice)
                .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                            VK_SHADER_STAGE_ALL_GRAPHICS)
                .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++) {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            DescriptorWriter(*globalSetLayout, *mGlobalPool)
                    .writeBuffer(0, &bufferInfo)
                    .build(globalDescriptorSets[i]);
        }

        const uint32_t MAX_PARTICLES = 20;
        SnakeGame particleSystem{mDevice, MAX_PARTICLES};
        ParticleRenderSystem renderSystem2D{mDevice, mRenderer.GetSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};

        std::shared_ptr<PointForce> pointForce = std::make_shared<PointForce>(glm::vec2(0), -0.01f);
        particleSystem.SetPointForce(pointForce);



        mBackgroundColor = glm::vec3(0.3f, 0.5f, 1.0f);
        mRenderer.SetClearColor(mBackgroundColor);

        auto currentTime = std::chrono::high_resolution_clock::now();

        Particle newParticle{glm::vec2(0), glm::vec2(0), glm::vec4(1), 0.01, 1};
        float velo = 0.2;

        float offset = 0.0f;

        while (!mWindow.shouldClose()) {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            if (auto commandBuffer = mRenderer.BeginFrame()) {
                imgui.newFrame();

                int frameIndex = (int) mRenderer.GetFrameIndex();

                FrameInfo frameInfo{
                        frameIndex,
                        frameTime,
                        commandBuffer,
                        {globalDescriptorSets[frameIndex]},
                        particleSystem};

                mRenderer.SetClearColor(mBackgroundColor);

                GlobalUbo ubo{};
                ubo.ambientLightColor = glm::vec4(mRenderer.GetClearColor(), 0.3f);

                particleSystem.Update(frameTime, mWindow.getCursorPosition());

                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                pointForce->location = mWindow.getCursorPosition();


                // render
                mRenderer.BeginSwapChainRenderPass(commandBuffer);

                renderSystem2D.RenderGameObjects(frameInfo);

                ImGui::Begin("Settings");
                {
                    std::string message = "Time since last frame: " + std::to_string(frameTime) + "ms";
                    ImGui::Text(message.c_str());

                    ImGui::ColorPicker3("Background Color", &mBackgroundColor.x);

                }
                ImGui::End();

                ImGui::Begin("Particles");
                {
                    uint32_t numParticles = particleSystem.NumParticles();
                    ImGui::Text("Number of Particles: %d", numParticles);
                    ImGui::SliderInt("", (int *)&numParticles, 0, MAX_PARTICLES);
                    if (numParticles != particleSystem.NumParticles()) {
                        particleSystem.MakeLinked();
                        particleSystem.SetNumParticles(numParticles);
                    }

                    ImGui::InputFloat2("Position", &newParticle.position.x);
                    ImGui::InputFloat2("Velocity", &newParticle.velocity.x);
                    ImGui::ColorPicker4("Color", &newParticle.color.r);
                    ImGui::InputFloat("Size", &newParticle.size);

                    if (ImGui::Button("Add Particle")) {
//                        newParticle.position.x = offset;
                        newParticle.color.a -= offset;
                        offset += 0.005;
                        particleSystem.AddParticle(newParticle);
                        particleSystem.MakeLinked();
                    }


                    ImGui::SliderFloat("Orbit Velocity", &velo, 0, 0.25);
                    if (ImGui::Button("Orbit")) {
                        particleSystem.AddParticle({glm::vec2(0), glm::vec2(0), glm::vec4(1), 0.2, 1});
                        particleSystem.AddParticle({glm::vec2(-0.4, 0), glm::vec2(0, velo), glm::vec4(1), 0.01, 1});
                    }
                }
                ImGui::End();

                imgui.render(commandBuffer);

                mRenderer.EndSwapChainRenderPass(commandBuffer);
                mRenderer.EndFrame();
            }
        }
    }

    void Application::Update() {}

    void Application::Render() {}

} // namespace engine
