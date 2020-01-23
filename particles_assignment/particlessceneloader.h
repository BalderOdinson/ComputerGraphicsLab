//
// Created by oshikuru on 13. 01. 2020..
//

#ifndef PARTICLES_PARTICLESSCENELOADER_H
#define PARTICLES_PARTICLESSCENELOADER_H

#include <lava/lava.h>

class ParticlesSceneLoader : public LavaVk::SceneLoader
{
public:
    static constexpr float CameraSpeed = 0.3f;
    const glm::vec3 LightPosition = {0, 2, 2};
    const glm::vec3 FountainPosition{0, 0, 0};
    const glm::quat FountainRotation = glm::normalize(glm::angleAxis(glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f)));
    const glm::vec3 FountainScale{1.f / 15.f};
    const glm::vec3 WaterFallPosition{-0.05, -0.15, 0};
    const glm::vec3 WaterPosition{0.f, -1.f, 0};
    const glm::quat CameraRotation = {1.f, .0f, .0f, .0f};
    const glm::vec3 CameraPosition{-0.1, -0.4, 1.9};
    static constexpr const char *FOUNTAIN_PATH = "../assets/objects/garden/source/garden.zip/garden.obj";

private:
    std::type_index getType() const override;
    std::pair<LavaVk::SharedScene, LavaVk::SharedRenderPipeline> load(const LavaVk::Core::SharedCommandBuffer &cmd) override;
public:
    void onSceneLoaded(LavaVk::DIContainer &container) override;
};


#endif //PARTICLES_PARTICLESSCENELOADER_H
