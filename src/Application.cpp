#include "Application.h"


#include "Imgui.h"
#include "imgui/imgui_stdlib.h"
#include "systems/SnakeGame.h"
#include "systems/ParticleRenderSystem.h"
#include <descriptors/DescriptorWriter.h>

#include <GLFW/glfw3.h>

#include <chrono>
#include <memory>
#include <random>

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

        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("../font/MontserratAlternates-Bold.otf", 32.0f);

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

//        const uint32_t MAX_PARTICLES = 23;
//        ParticleRenderSystem particleRenderSystem{mDevice, mRenderer.GetSwapChainRenderPass(),
//                                            globalSetLayout->getDescriptorSetLayout(), MAX_PARTICLES};
        std::unique_ptr<ParticleRenderSystem> particleRenderSystem;
        uint32_t maxScore;

        Particle *apple;

        bool isRunning = false;
        bool showMenu = true;
        bool startNewGame = false;
        bool quitGame = false;

        std::vector<LinkedParticle> snake;


        mBackgroundColor = glm::vec3(0.3f, 0.5f, 1.0f);
        mRenderer.SetClearColor(mBackgroundColor);

        auto currentTime = std::chrono::high_resolution_clock::now();

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
                        {globalDescriptorSets[frameIndex]}
                      };

                mRenderer.SetClearColor(mBackgroundColor);

                GlobalUbo ubo{};
                ubo.ambientLightColor = glm::vec4(mRenderer.GetClearColor(), 0.3f);

                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                if (startNewGame) {
                    particleRenderSystem = std::make_unique<ParticleRenderSystem>(mDevice,
                                                                                  mRenderer.GetSwapChainRenderPass(),
                                                                                  globalSetLayout->getDescriptorSetLayout(),
                                                                                  maxScore + 2);

                    apple = particleRenderSystem->AddParticle(glm::vec2(frand(-1, 1), frand(-0.8, 0.8)),
                                                              glm::vec4(0.7, 0.1, 0.1, 1),
                                                              0.02);

                    snake.clear();
                    Particle *temp = particleRenderSystem->AddParticle(glm::vec2(0, 0.02), glm::vec4(1), 0.01);
                    snake.emplace_back(particleRenderSystem->AddParticle(glm::vec2(0, 0), glm::vec4(1), 0.01), temp);
                    snake.emplace_back(temp, nullptr);

                    startNewGame = false;
                    isRunning = true;
                }

                if (quitGame) {
                    break;
                }

                if (isRunning) {
                    snake[0].particle->position = mWindow.getCursorPosition();

                    glm::vec2 distanceVector = snake[0].particle->position - apple->position;
                    float distanceSquared = glm::dot(distanceVector, distanceVector);
                    float collisionDistanceSquared = (snake[0].particle->size + apple->size) * (snake[0].particle->size + apple->size);

                    if (distanceSquared <= collisionDistanceSquared) {
                        apple->position = glm::vec2(frand(-0.8, 0.8), frand(-0.8, 0.8));
                        snake.emplace_back(particleRenderSystem->AddParticle(snake.back().particle->position, glm::vec4(1), 0.01),nullptr);
                        snake[snake.size() - 2].child = snake.back().particle;

                        if (!snake.back().particle) {
                            isRunning = false;
                            snake.pop_back();
                        }
                    }

                    for (auto &linkedParticle : snake) {
                        if (!linkedParticle.child) continue;
                        glm::vec2 dist = linkedParticle.distToChild();
                        float maxDist = linkedParticle.particle->size + linkedParticle.child->size;
                        if (glm::length(dist) > maxDist) {
                            linkedParticle.child->position = linkedParticle.particle->position - glm::normalize(dist) * maxDist;
                        }
                    }

                    for (uint32_t i{0}; i + 1 < snake.size(); ++i) {
                        for (uint32_t j{i + 1}; j < snake.size(); ++j) {
                            if (snake[i].child != snake[j].particle &&
                                snake[j].child != snake[i].particle &&
                                glm::length(snake[i].particle->position - snake[j].particle->position) < snake[i].particle->size + snake[j].particle->size) {
                                isRunning = false;
                            }
                        }
                    }
                }



                // render
                mRenderer.BeginSwapChainRenderPass(commandBuffer);

                if (isRunning) particleRenderSystem->Render(frameInfo);

                ImGui::Begin("Settings");
                {
                    ImGui::Text("Time since last frame: %f", frameTime);

                    ImGui::ColorPicker3("Background Color", &mBackgroundColor.x);

                }
                ImGui::End();

                ImGui::SetNextWindowPos(ImVec2(0, 0));
                ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 0));
                ImGui::Begin("Snake", nullptr, ImGuiWindowFlags_NoMove |
                                               ImGuiWindowFlags_NoResize |
                                               ImGuiWindowFlags_NoTitleBar |
                                               ImGuiWindowFlags_NoScrollbar |
                                               ImGuiWindowFlags_NoSavedSettings);
                {
                    ImGui::Text("Snake Game");
                    ImGui::SameLine(ImGui::GetWindowWidth() - 150 );
                    ImGui::Text("Score: %d", snake.size() - 2);

                }
                ImGui::End();

                if (!isRunning && !showMenu) {
                    ImVec2 center = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);

                    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
                    ImGui::SetNextWindowSize(ImVec2(400, 200)); // Adjust size as needed

                    ImFont* bigFont = io.Fonts->Fonts[0]; // Assuming this is your large font
                    ImGui::PushFont(bigFont);

                    ImGui::Begin("Centered Window", nullptr, ImGuiWindowFlags_NoMove |
                                                             ImGuiWindowFlags_NoResize |
                                                             ImGuiWindowFlags_NoTitleBar |
                                                             ImGuiWindowFlags_NoScrollbar |
                                                             ImGuiWindowFlags_AlwaysAutoResize);

                    ImGui::Text("Game Over");
                    ImGui::Text("Score: %d", snake.size() - 2);
                    if (ImGui::Button("Menu")) {
                        showMenu = true;
                    }

                    ImGui::End();

                    ImGui::PopFont();
                }

                if (showMenu) {
                    ImVec2 center = ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);

                    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
                    ImGui::SetNextWindowSize(ImVec2(400, 200)); // Adjust size as needed

                    ImFont* bigFont = io.Fonts->Fonts[0]; // Assuming this is your large font
                    ImGui::PushFont(bigFont);

                    ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoMove |
                                                             ImGuiWindowFlags_NoResize |
                                                             ImGuiWindowFlags_NoTitleBar |
                                                             ImGuiWindowFlags_NoScrollbar |
                                                             ImGuiWindowFlags_AlwaysAutoResize);

                    ImGui::Text("Menu");
                    ImGui::SliderInt("Max Score", (int *)&maxScore, 0, 100);

                    if (ImGui::Button("Start Game")) {
                        startNewGame = true;
                        showMenu = false;
                    }

                    if (ImGui::Button("Quit")) {
                        quitGame = true;
                    }

                    ImGui::End();

                    ImGui::PopFont();
                }

                imgui.render(commandBuffer);

                mRenderer.EndSwapChainRenderPass(commandBuffer);
                mRenderer.EndFrame();
            }
        }
    }

    void Application::Update() {}

    void Application::Render() {}

    float Application::frand(float min, float max) {
        static std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr)));
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(generator);
    }

} // namespace engine
