//
// Created by oshikuru on 12. 01. 2020..
//

#ifndef BSPLINE_BSPLINEANIMATION_H
#define BSPLINE_BSPLINEANIMATION_H

#include <lava/lava.h>

class BSplineAnimation : public LavaVk::NodeAnimation
{
public:
    BSplineAnimation();

    void initialize(LavaVk::DIContainer &container) override;
    void animate(const LavaVk::SharedTransform &transform, float delta);

    float speedFactor{1.f};

private:
    float totalTime{0};
    size_t segment{0};
    std::vector<glm::vec3> points;
    std::vector<float> rolls;
};


#endif //BSPLINE_BSPLINEANIMATION_H
