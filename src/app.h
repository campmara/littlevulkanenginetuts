#ifndef APP_H
#define APP_H

#include "render/descriptors.h"
#include "render/device.h"
#include "render/model.h"
#include "render/renderer.h"
#include "render/window.h"
#include "gameobject.h"

#include <memory>
#include <vector>

namespace XIV {
    class App {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        App();
        ~App();
        App(const App &) = delete;
        App &operator=(const App &) = delete;

        void Run();

    private:
        void LoadGameObjects();

        Window window{WIDTH, HEIGHT, "AYO VULKAN!!!"};
        Device device{window};
        Renderer renderer{window, device};

        // note: order of declarations matters
        std::unique_ptr<DescriptorPool> globalPool{};
        GameObject::Map gameObjects;
    };
} // namespace XIV

#endif
