//
// Created by dorian on 08. 12. 2019..
//

#include "bsplinesample.h"
#include "bsplinesceneloader.h"

#include <array>
#include <iostream>

std::type_index BSplineSample::getType() const
{
    return typeid(BSplineSample);
}

void BSplineSample::initialize()
{
	LavaVk::App::initialize();
}

void BSplineSample::installDependencies(LavaVk::DIContainer &container)
{
	LavaVk::App::installDependencies(container);
}

void BSplineSample::installConfigurations(LavaVk::DIContainer &container)
{
    LavaVk::App::installConfigurations(container);

    container.option<LavaVk::WindowOptions>()->fullscreenMode = false;
    container.option<LavaVk::WindowOptions>()->displayId = 0;
    container.option<LavaVk::ApplicationOption>()->appName = "B-spline sample";
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
    container.option<LavaVk::MipmapsOptions>()->maxLod = 0;
}

void BSplineSample::update(float deltaTime)
{
}

void BSplineSample::finish()
{
	LavaVk::App::finish();
}

LavaVk::SharedSceneLoader BSplineSample::startScene()
{
    return LavaVk::make<BSplineSceneLoader>();
}

int main()
{
#ifdef NDEBUG
    try
    {
#endif
        BSplineSample::make<BSplineSample>()->run();
#ifdef NDEBUG
    }
    catch (const std::exception &exc)
    {
        BSplineSample::instance.reset();
        std::cerr << "[ERROR] " << exc.what() << std::endl;
    }
#endif
    return EXIT_SUCCESS;
}
