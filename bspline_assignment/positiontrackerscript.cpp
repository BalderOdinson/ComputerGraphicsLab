//
// Created by oshikuru on 12. 01. 2020..
//

#include "positiontrackerscript.h"

PositionTrackerScript::PositionTrackerScript() : Script("Position script")
{}

PositionTrackerScript::~PositionTrackerScript()
{
    if(inputToken)
        inputManager->event().unsubscribe(inputToken);
}

void PositionTrackerScript::initialize(LavaVk::DIContainer &container)
{
    inputManager = container.resolve<LavaVk::InputManager>();
    inputToken = inputManager->event().subscribe(
            [&](Object& sender, const LavaVk::InputEventArgs& args)
            {
                if (args.getSource() == LavaVk::EventSource::Keyboard)
                {
                    const auto &keyEvent = dynamic_cast<const LavaVk::KeyInputEventArgs &>(args);

                    if(keyEvent.getCode() == LavaVk::KeyCode::_5 && keyEvent.getAction() == LavaVk::KeyAction::Down)
                        save = true;
                    /*else if(keyEvent.getCode() == LavaVk::KeyCode::KP_1 && keyEvent.getAction() == LavaVk::KeyAction::Up)
                        positions.emplace_back(getNode()->getParent()->getTransform()->getTranslation());*/
                }
            });
}

void PositionTrackerScript::update(float deltaTime)
{
    totalTime += deltaTime;
    if(totalTime > 1.0f)
    {
        totalTime = 0;
        if(save)
        {
            save = false;
            saveToFile();
        }
        else
        {
            positions.emplace_back(getNode()->getParent()->getTransform()->getTranslation());
            rolls.emplace_back(glm::roll(getNode()->getParent()->getTransform()->getRotation()));
        }
    }
}

void PositionTrackerScript::saveToFile()
{
    FILE *fp = fopen("bspline.obj", "w+");

    for (auto & position : positions)
        fprintf(fp, "v %f %f %f\n", position.x, position.y, position.z);

    FILE *fpr = fopen("rolls.rot", "w+");

    for (auto & roll : rolls)
        fprintf(fpr, "%f\n", roll);

    fclose(fp);
    fclose(fpr);
}
