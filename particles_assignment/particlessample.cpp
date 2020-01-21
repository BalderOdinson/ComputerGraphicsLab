//
// Created by oshikuru on 19. 12. 2019..
//


#include "particlessample.h"

#include <iostream>
#include "particlessceneloader.h"


std::type_index ParticlesSample::getType() const
{
    return typeid(ParticlesSample);
}

void ParticlesSample::initialize()
{
    Application::initialize();
}

void ParticlesSample::installDependencies(LavaVk::DIContainer &container)
{
    Application::installDependencies(container);
}

void ParticlesSample::installConfigurations(LavaVk::DIContainer &container)
{
    LavaVk::LinuxApplication::installConfigurations(container);

    container.option<LavaVk::WindowOptions>()->fullscreenMode = true;
    container.option<LavaVk::WindowOptions>()->displayId = 1;
    container.option<LavaVk::ApplicationOption>()->appName = "Particles sample";
    container.option<LavaVk::ApplicationOption>()->enableDebug = false;
    /*container.option<LavaVk::Core::SwapchainOptions>()->presentMode = vk::PresentModeKHR::eFifo;*/

    container.addOption<LavaVk::MultisamplingOptions>(
            [](const LavaVk::InjectionContext &context)
            {
                auto samples = context.container.resolve<LavaVk::Core::Device>()->getProperties().limits.framebufferColorSampleCounts;
                if (samples & vk::SampleCountFlagBits::e64)
                    return new LavaVk::MultisamplingOptions(
                            vk::SampleCountFlagBits::e64);
                if (samples & vk::SampleCountFlagBits::e32)
                    return new LavaVk::MultisamplingOptions(
                            vk::SampleCountFlagBits::e32);
                if (samples & vk::SampleCountFlagBits::e16)
                    return new LavaVk::MultisamplingOptions(
                            vk::SampleCountFlagBits::e16);
                if (samples & vk::SampleCountFlagBits::e8)
                    return new LavaVk::MultisamplingOptions(
                            vk::SampleCountFlagBits::e8);
                if (samples & vk::SampleCountFlagBits::e4)
                    return new LavaVk::MultisamplingOptions(
                            vk::SampleCountFlagBits::e4);
                if (samples & vk::SampleCountFlagBits::e2)
                    return new LavaVk::MultisamplingOptions(
                            vk::SampleCountFlagBits::e2);

                return new LavaVk::MultisamplingOptions(
                        vk::SampleCountFlagBits::e1);
            });
}

void ParticlesSample::update(float dt)
{
}

void ParticlesSample::finish()
{
    Application::finish();
}

LavaVk::SharedSceneLoader ParticlesSample::startScene()
{
    return LavaVk::make<ParticlesSceneLoader>();
}


int main()
{
#ifdef NDEBUG
    try
    {
#endif
        ParticlesSample::make<ParticlesSample>()->run();
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