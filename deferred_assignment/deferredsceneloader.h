//
// Created by oshikuru on 20. 01. 2020..
//

#ifndef DEFERRED_DEFERREDSCENELOADER_H
#define DEFERRED_DEFERREDSCENELOADER_H

#include <lava/lava.h>

class DeferredSceneLoader : public LavaVk::SceneLoader
{
public:
    static constexpr float CameraSpeed = 0.3f;
    const glm::vec3 LightPosition = {0, 2, 2};
    const glm::quat CameraRotation = {1.f, .0f, .0f, .0f};
    const glm::vec3 CameraPosition{-0.1, -0.4, 1.9};
    const glm::vec3 ContainerPosition{0.025f, 0.19f, 0.35f};
    const glm::vec3 ContainerScale{1.0f};
    static constexpr const char *CONTAINER_PATH = "../assets/objects/container/container.obj";

    std::type_index getType() const override;
    std::pair<LavaVk::SharedScene, LavaVk::SharedRenderPipeline> load(const LavaVk::Core::SharedCommandBuffer &cmd) override;
    void onSceneLoaded(LavaVk::DIContainer &container) override;
};


#endif //DEFERRED_DEFERREDSCENELOADER_H
