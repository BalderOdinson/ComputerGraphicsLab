//
// Created by oshikuru on 19. 12. 2019..
//

#ifndef LAVAVK_PARTICLESSAMPLE_H
#define LAVAVK_PARTICLESSAMPLE_H

#include <lava/lava.h>

class ParticlesSample : public LavaVk::App
{
public:
    std::type_index getType() const override;

protected:
    void initialize() override;
    void installDependencies(LavaVk::DIContainer &container) override;
    void installConfigurations(LavaVk::DIContainer &container) override;
    void update(float deltaTime) override;
    void finish() override;

    LavaVk::SharedSceneLoader startScene() override;
};


#endif //LAVAVK_PARTICLESSAMPLE_H
