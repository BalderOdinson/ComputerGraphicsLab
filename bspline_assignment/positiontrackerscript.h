//
// Created by oshikuru on 12. 01. 2020..
//

#ifndef BSPLINE_POSITIONTRACKERSCRIPT_H
#define BSPLINE_POSITIONTRACKERSCRIPT_H

#include <lava/lava.h>

class PositionTrackerScript : public LavaVk::Script
{
public:
    PositionTrackerScript();
    ~PositionTrackerScript() override;

    void initialize(LavaVk::DIContainer &container) override;
    void update(float deltaTime) override;

private:
    void saveToFile();

    LavaVk::SharedInputManager inputManager;
    LavaVk::EventToken inputToken;
    float totalTime{0.f};
    bool save;
    std::vector<glm::vec3> positions;
    std::vector<float> rolls;
};


#endif //BSPLINE_POSITIONTRACKERSCRIPT_H
