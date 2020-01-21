//
// Created by oshikuru on 20. 01. 2020..
//

#include "deferredsceneloader.h"

std::type_index DeferredSceneLoader::getType() const
{
    return typeid(DeferredSceneLoader);
}

std::pair<LavaVk::SharedScene, LavaVk::SharedRenderPipeline>
DeferredSceneLoader::load(const LavaVk::Core::SharedCommandBuffer &cmd)
{
    auto &container = LavaVk::Application::instance->container;
    auto geometrySubpassOptions = LavaVk::make<LavaVk::GeometrySubpassOptions>();
    geometrySubpassOptions->renderers.push_back(
            container.resolve<LavaVk::DeferredSpecularRenderer, LavaVk::DeferredRendererOptions>(
                    container.resolve<LavaVk::SpecularMeshRenderer, LavaVk::GeometryRenderOptions>(
                            LavaVk::AlphaMode::Opaque)));
    geometrySubpassOptions->renderers.push_back(
            container.resolve<LavaVk::DeferredSpecularRenderer, LavaVk::DeferredRendererOptions>(
                    container.resolve<LavaVk::SpecularMeshRenderer, LavaVk::GeometryRenderOptions>(
                            LavaVk::AlphaMode::Blend)));
    auto geometrySubpass = container.resolve<LavaVk::GeometrySubpass, LavaVk::GeometrySubpassOptions>(geometrySubpassOptions);

    auto lightSubpassOptions = LavaVk::make<LavaVk::LightSubpassOptions>(LavaVk::make<LavaVk::SpecularLightRenderer>());
    auto lightSubpass = container.resolve<LavaVk::LightSubpass, LavaVk::LightSubpassOptions>(lightSubpassOptions);

    auto renderPipeline = LavaVk::make<LavaVk::RenderPipeline>(LavaVk::RenderPipelineOptions({geometrySubpass, lightSubpass}));

    auto scene = LavaVk::make<LavaVk::Scene>("Deferred scene");

    auto cameraNode = LavaVk::Node::make("cameraNode");
    auto lightNode = LavaVk::Node::make("lightNode");
    auto containerNode = LavaVk::Node::make("containerNode");
    scene->addChild(cameraNode);
    scene->addChild(lightNode);
    scene->addChild(containerNode);

    auto containerMesh = containerNode->setComponent<LavaVk::Mesh>(
            LavaVk::make<LavaVk::ObjectFileMeshSource>(CONTAINER_PATH, true, true, false), "container");
    containerNode->getTransform()->setTranslation(ContainerPosition);
    containerNode->getTransform()->setScale(ContainerScale);
    containerMesh->load(cmd);

    auto camera = cameraNode->setComponent<LavaVk::PerspectiveCamera>("mainCamera");
    auto cameraScript = cameraNode->setComponent<LavaVk::FreeCamera>();
    cameraScript->translationMoveStep = CameraSpeed;
    auto window = container.resolve<LavaVk::Window>();
    camera->setAspectRatio(static_cast<float>(window->getWidth()) / window->getHeight());
    camera->setFieldOfView(1.0f);
    camera->setNearPlane(0.1f);
    camera->setFarPlane(1000.0f);
    cameraNode->getTransform()->setRotation(
            CameraRotation * glm::inverse(cameraNode->getTransform()->getRotation()));
    cameraNode->getTransform()->setTranslation(CameraPosition);

    auto pointLight = lightNode->setComponent<LavaVk::SpecularLight>("pointLight");
    lightNode->getTransform()->setTranslation(LightPosition);
    pointLight->color = {.5f, .5f, .5f};
    pointLight->ambient = {.01f, .01f, .01f};
    pointLight->specular = {8.f, 8.f, 8.f};
    pointLight->intensityConstant = 1.f;
    pointLight->intensityLinear = 0.7f;
    pointLight->intensityQuadratic = 1.8f;
    pointLight->lightType = LavaVk::LightType::Point;

    return {scene, renderPipeline};
}

void DeferredSceneLoader::onSceneLoaded(LavaVk::DIContainer &container)
{
    container.addOption<LavaVk::RenderTargetOptions>([]()
                                                     { return new LavaVk::DeferredSpecularRenderTargetOptions(); });
    container.resolve<LavaVk::RenderContext>()->recreate();
}
