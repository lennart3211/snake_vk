#pragma once

#include "Device.h"
#include "Pipeline.h"
#include "Window.h"
#include "Renderer.h"
#include "Buffer.h"
#include "descriptors/DescriptorPool.h"
#include "Components.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_vulkan.h"
#include "textures/TextureHandler.h"
#include "textures/Texture.h"

#include <entt/entt.hpp>

#define GLM_FORCE_RADIANS
#define GLM_FROCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace engine {

    struct ImGuiImage {
        VkDescriptorSet m_image;
        ImVec2 m_size;
    };

    class Application {
    private:
        static constexpr uint16_t WIDTH = 1920;
        static constexpr uint16_t HEIGHT = 1920;
        Window mWindow{WIDTH, HEIGHT, "App"};
        Device mDevice{mWindow};
        Renderer mRenderer{mWindow, mDevice};

        // Declaration order matters!!!!!!
        std::unique_ptr<DescriptorPool> mGlobalPool{};

        glm::vec3 mBackgroundColor;

        std::vector<ImGuiImage> m_images{8};

    public:
        Application();
        ~Application();
        Application(const Application &) = delete;
        Application &operator=(const Application &) = delete;

        void run();

    private:
        void Update();
        void Render();
    };
} // namespace engine
