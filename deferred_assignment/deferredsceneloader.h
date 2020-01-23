//
// Created by oshikuru on 20. 01. 2020..
//

#ifndef DEFERRED_DEFERREDSCENELOADER_H
#define DEFERRED_DEFERREDSCENELOADER_H

#include <lava/lava.h>

class DeferredSceneLoader : public LavaVk::SceneLoader
{
public:
    static constexpr float CameraSpeed = 60.f;
    const glm::vec3 LightPosition = {0, 2, 2};
    const glm::quat CameraRotation{0.585863,-0.008033, -0.810360, -0.004083};
    const glm::vec3 CameraPosition{-1734.173950, 465.458954, -1474.270996};
    const glm::vec3 ContainerPosition{0.025f, 0.19f, 0.35f};
    const glm::vec3 SphereScale{10.f};
    static constexpr const uint32_t LightsCount{100};
    static constexpr const char *CITY_PATH = "../assets/objects/persian-city/source/Desktop/Arabic+City.obj";
    static constexpr const char *SPHERE_PATH = "../assets/objects/sphere/globe-sphere.obj";

    std::type_index getType() const override;
    std::pair<LavaVk::SharedScene, LavaVk::SharedRenderPipeline> load(const LavaVk::Core::SharedCommandBuffer &cmd) override;
    void onSceneLoaded(LavaVk::DIContainer &container) override;
};


#endif //DEFERRED_DEFERREDSCENELOADER_H
