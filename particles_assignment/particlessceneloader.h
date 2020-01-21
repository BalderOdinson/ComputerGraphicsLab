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
    const glm::vec3 ContainerPosition{0.025f, 0.19f, 0.35f};
    const glm::vec3 ContainerScale{0.07f, 0.12f, 1.0f};
    const glm::vec3 SwordPosition{0.f, 0.1f, 0.8f};
    const glm::vec3 SwordScale{1.f / 25.f};
    static constexpr const char *FOUNTAIN_PATH = "../assets/objects/garden/source/garden.zip/garden.obj";
    static constexpr const char *SWORD_PATH = "../assets/objects/stone-sword/source/sword_lp_5_002/sword_lp_5_002.obj";
    static constexpr const char *CONTAINER_PATH = "../assets/objects/container/container.obj";
    static constexpr const char *CUBEMAP_RIGHT_PATH = "../assets/skyboxes/PiazzaDelPopolo2/posx.jpg";
    static constexpr const char *CUBEMAP_LEFT_PATH = "../assets/skyboxes/PiazzaDelPopolo2/negx.jpg";
    static constexpr const char *CUBEMAP_UP_PATH = "../assets/skyboxes/PiazzaDelPopolo2/posy.jpg";
    static constexpr const char *CUBEMAP_DOWN_PATH = "../assets/skyboxes/PiazzaDelPopolo2/negy.jpg";
    static constexpr const char *CUBEMAP_BACKWARD_PATH = "../assets/skyboxes/PiazzaDelPopolo2/posz.jpg";
    static constexpr const char *CUBEMAP_FORWARD_PATH = "../assets/skyboxes/PiazzaDelPopolo2/negz.jpg";
    static constexpr const char *CUBEMAP_MODEL = "../assets/skyboxes/cube.obj";


private:
    std::type_index getType() const override;
    std::pair<LavaVk::SharedScene, LavaVk::SharedRenderPipeline> load(const LavaVk::Core::SharedCommandBuffer &cmd) override;
public:
    void onSceneLoaded(LavaVk::DIContainer &container) override;
};


#endif //PARTICLES_PARTICLESSCENELOADER_H
