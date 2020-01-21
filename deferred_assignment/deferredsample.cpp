//
// Created by oshikuru on 19. 01. 2020..
//

#include <iostream>
#include "deferredsample.h"
#include "deferredsceneloader.h"

std::type_index DeferredSample::getType() const
{
    return typeid(DeferredSample);
}

void DeferredSample::initialize()
{
    Application::initialize();
}

void DeferredSample::installDependencies(LavaVk::DIContainer &container)
{
    Application::installDependencies(container);
}

void DeferredSample::installConfigurations(LavaVk::DIContainer &container)
{
    LinuxApplication::installConfigurations(container);

    container.option<LavaVk::WindowOptions>()->fullscreenMode = true;
    container.option<LavaVk::WindowOptions>()->displayId = 1;
    container.option<LavaVk::ApplicationOption>()->appName = "Particles sample";
    container.option<LavaVk::ApplicationOption>()->enableDebug = false;
    /*container.option<LavaVk::Core::SwapchainOptions>()->presentMode = vk::PresentModeKHR::eFifo;*/

    container.addOption<LavaVk::MultisamplingOptions>(
            [](const LavaVk::InjectionContext &context)
            {
                return new LavaVk::MultisamplingOptions(
                        vk::SampleCountFlagBits::e1);
            });
}

void DeferredSample::update(float deltaTime)
{

}

void DeferredSample::finish()
{
    Application::finish();
}

LavaVk::SharedSceneLoader DeferredSample::startScene()
{
    return LavaVk::make<DeferredSceneLoader>();
}

int main()
{
#ifdef NDEBUG
    try
    {
#endif
        DeferredSample::make<DeferredSample>()->run();
#ifdef NDEBUG
    }
    catch (const std::exception &exc)
    {
        std::cerr << "[ERROR] " << exc.what() << std::endl;
        return EXIT_FAILURE;
    }
#endif

    return EXIT_SUCCESS;
}