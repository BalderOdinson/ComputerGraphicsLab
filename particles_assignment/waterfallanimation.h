//
// Created by oshikuru on 14. 01. 2020..
//

#ifndef PARTICLES_WATERFALLANIMATION_H
#define PARTICLES_WATERFALLANIMATION_H

#include <lava/lava.h>

class WaterFallAnimation : public LavaVk::Script
{
public:
    void initialize(LavaVk::DIContainer &container) override;
    void update(float deltaTime) override;

private:
    LavaVk::SharedParticleEffect particles;
    float t{-1.f};
};


#endif //PARTICLES_WATERFALLANIMATION_H
