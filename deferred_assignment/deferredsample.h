//
// Created by oshikuru on 19. 01. 2020..
//

#ifndef DEFERRED_DEFERREDSAMPLE_H
#define DEFERRED_DEFERREDSAMPLE_H

#include <lava/lava.h>

class DeferredSample : public LavaVk::App
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


#endif //DEFERRED_DEFERREDSAMPLE_H
