//
// Created by dorian on 08. 12. 2019..
//

#ifndef LAVAVK_CHALETSAMPLE_H
#define LAVAVK_CHALETSAMPLE_H

#include <lava/lava.h>

class BSplineSample : public LavaVk::App
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


#endif //LAVAVK_CHALETSAMPLE_H
