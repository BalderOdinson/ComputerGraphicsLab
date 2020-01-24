//
// Created by oshikuru on 14. 01. 2020..
//

#include "waterfallanimation.h"

void WaterFallAnimation::initialize(LavaVk::DIContainer &container)
{
    particles = getNode()->getComponent<LavaVk::ParticleEffect>();
}

void WaterFallAnimation::update(float deltaTime)
{
    if(t == -1)
    {
        t = 0.f;
        return;
    }

    if(t < 1.f)
    {
        t += deltaTime;
    }
    else if(t < 4.f)
    {
        particles->particleProperties[0].deathProbability -= deltaTime * 0.05f;
        particles->particleProperties[0].source.y += deltaTime * 0.06f;
        t += deltaTime;
    }
}
