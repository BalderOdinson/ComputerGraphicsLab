//
// Created by oshikuru on 12. 01. 2020..
//

#ifndef BSPLINE_BSPLINESCENELOADER_H
#define BSPLINE_BSPLINESCENELOADER_H

#include <lava/lava.h>

class BSplineSceneLoader : public LavaVk::SceneLoader
{
public:
    static constexpr float CameraSpeed = 60.f;
    const glm::vec3 LightPosition = {-9.344397, -1.719311, 3.621786};
    const glm::vec3 LightDirection{0.045222, 0.819241, 0.571664};
    const glm::quat CameraRotation{0.585863,-0.008033, -0.810360, -0.004083};
    const glm::vec3 CameraPosition{-1734.173950, 465.458954, -1474.270996};
    const glm::vec3 F14Position{190.051376, 595.729614, 2201.358887};
    static constexpr const char *BSPLINE_PATH = "../assets/paths/bspline.obj";
    static constexpr const char *BSPLINE_ROLL_PATH = "../assets/paths/rolls.rot";
    static constexpr const char *CITY_PATH = "../assets/objects/persian-city/source/Desktop/Arabic+City.obj";
    static constexpr const char *F14_MODEL_PATH = "../assets/objects/jet/source/finalzip/objFinalLP.obj";
    static constexpr const char *CUBEMAP_RIGHT_PATH = "../assets/skyboxes/cloudy/graycloud_rt.jpg";
    static constexpr const char *CUBEMAP_LEFT_PATH = "../assets/skyboxes/cloudy/graycloud_lf.jpg";
    static constexpr const char *CUBEMAP_UP_PATH = "../assets/skyboxes/cloudy/graycloud_up.jpg";
    static constexpr const char *CUBEMAP_DOWN_PATH = "../assets/skyboxes/cloudy/graycloud_dn.jpg";
    static constexpr const char *CUBEMAP_BACKWARD_PATH = "../assets/skyboxes/cloudy/graycloud_bk.jpg";
    static constexpr const char *CUBEMAP_FORWARD_PATH = "../assets/skyboxes/cloudy/graycloud_ft.jpg";
    static constexpr const char *CUBEMAP_MODEL = "../assets/skyboxes/cube.obj";

    std::type_index getType() const override;
    std::pair<LavaVk::SharedScene, LavaVk::SharedRenderPipeline> load(const LavaVk::Core::SharedCommandBuffer &cmd) override;
    void onSceneLoaded(LavaVk::DIContainer &container) override;
};


#endif //BSPLINE_BSPLINESCENELOADER_H
