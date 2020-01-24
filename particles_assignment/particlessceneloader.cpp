//
// Created by oshikuru on 13. 01. 2020..
//

#include "particlessceneloader.h"
#include "particlessample.h"
#include "waterfallanimation.h"

std::type_index ParticlesSceneLoader::getType() const
{
    return typeid(ParticlesSceneLoader);
}

std::pair<LavaVk::SharedScene, LavaVk::SharedRenderPipeline>
ParticlesSceneLoader::load(const LavaVk::Core::SharedCommandBuffer &cmd)
{
    auto &container = LavaVk::Application::instance->container;
    auto subpassOptions = LavaVk::make<LavaVk::GeometrySubpassOptions>();
    subpassOptions->renderers.push_back(
            container.resolve<LavaVk::ForwardSpecularLightsRenderer, LavaVk::ForwardLightsRendererOptions>(
                    container.resolve<LavaVk::SpecularMeshRenderer, LavaVk::GeometryRenderOptions>(
                            LavaVk::AlphaMode::Opaque), 0, 1, "fw"));
    subpassOptions->renderers.push_back(container.resolve<LavaVk::ParticleRenderer>());
    subpassOptions->renderers.push_back(
            container.resolve<LavaVk::ForwardSpecularLightsRenderer, LavaVk::ForwardLightsRendererOptions>(
                    container.resolve<LavaVk::SpecularMeshRenderer, LavaVk::GeometryRenderOptions>(
                            LavaVk::AlphaMode::Blend), 0, 1, "fw"));

    auto subpass = container.resolve<LavaVk::GeometrySubpass, LavaVk::GeometrySubpassOptions>(subpassOptions);
    auto renderPipeline = LavaVk::make<LavaVk::RenderPipeline>(LavaVk::RenderPipelineOptions({subpass}));


    auto scene = LavaVk::make<LavaVk::Scene>("Particles scene");

    auto waterFallNode = LavaVk::Node::make("waterFallNode");
    auto waterNode = LavaVk::Node::make("waterNode");
    auto fountainNode = LavaVk::Node::make("fountainNode");
    auto cameraNode = LavaVk::Node::make("cameraNode");
    auto lightNode = LavaVk::Node::make("lightNode");
    scene->addChild(waterFallNode);
    scene->addChild(waterNode);
    scene->addChild(fountainNode);
    scene->addChild(cameraNode);
    scene->addChild(lightNode);

    auto fountainMesh = fountainNode->setComponent<LavaVk::Mesh>(
            LavaVk::make<LavaVk::ObjectFileMeshSource>(FOUNTAIN_PATH, true, true, false), "fountain");
    fountainNode->getTransform()->setTranslation(FountainPosition);
    fountainNode->getTransform()->setRotation(FountainRotation);
    fountainNode->getTransform()->setScale(FountainScale);
    fountainMesh->load(cmd);

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
    pointLight->intensityLinear = 0.09f;
    pointLight->intensityQuadratic = 0.032f;
    pointLight->lightType = LavaVk::LightType::Point;

    auto waterFall = waterFallNode->setComponent<LavaVk::ParticleEffect>("waterFall");
    waterFallNode->getTransform()->setTranslation(WaterFallPosition);
    waterFallNode->setLayer(1);
    waterFall->particleCount = 100000;
    waterFall->aliveParticlesCount = 0;
    waterFall->sizeMultiplier = 0.004f;
    waterFall->stretchWithVelocity = true;
    waterFall->motionBlur = 0.025f;
    waterFall->particleProperties.emplace_back();
    waterFall->particleProperties[0].source = glm::vec3(0.f, 0.f, 0.f);
    waterFall->particleProperties[0].color = glm::vec4(.0f, .467f, .745f, .6f);
    waterFall->particleProperties[0].colorDecayRate = glm::vec4(.0f, .0f, .0f, .3f);
    waterFall->particleProperties[0].lifetime = 0.85f;
    waterFall->particleProperties[0].radiusType = LavaVk::ParticleProperties::RadiusType::Spherical;
    waterFall->particleProperties[0].radius = glm::vec3(.02f, .0f, .02f);
    waterFall->particleProperties[0].deathProbability = 0.99f;
    waterFall->particleProperties[0].deathDecayRate = 0.9f;
    waterFall->particleProperties[0].forces.emplace_back();
    waterFall->particleProperties[0].forces[0].direction = glm::vec3(0.f, 1.f, 0.f);
    waterFall->particleProperties[0].forces[0].source = glm::vec3(0.f, -0.05f, 0.f);
    waterFall->particleProperties[0].forces[0].force = 2.f;
    waterFall->particleProperties[0].forces[0].directionType = LavaVk::ParticleProperties::ForceDirectionType::FromSourceToParticle;
    waterFall->particleProperties[0].forces[0].time = 0.85f;
    waterFall->particleProperties[0].forces[0].type = LavaVk::ParticleProperties::ForceType::Velocity;
    waterFall->particleProperties[0].forces.emplace_back();
    waterFall->particleProperties[0].forces[1].direction = glm::vec3(0.f, -1.f, 0.f);
    waterFall->particleProperties[0].forces[1].force = 9.81;
    waterFall->particleProperties[0].forces[1].directionType = LavaVk::ParticleProperties::ForceDirectionType::FromParticleInDirection;
    waterFall->particleProperties[0].forces[1].time = 0.85f;
    waterFall->particleProperties[0].forces[1].type = LavaVk::ParticleProperties::ForceType::Acceleration;
    waterFall->particleProperties[0].forces.emplace_back();
    waterFall->particleProperties[0].forces[2].direction = glm::vec3(0.0f, 1.0f, 0.f);
    waterFall->particleProperties[0].forces[2].force = 5.8f;
    waterFall->particleProperties[0].forces[2].directionType = LavaVk::ParticleProperties::ForceDirectionType::FromParticleInDirection;
    waterFall->particleProperties[0].forces[2].time = 0.2f;
    waterFall->particleProperties[0].forces[2].type = LavaVk::ParticleProperties::ForceType::Velocity;
    waterFall->load(cmd);

    auto water = waterNode->setComponent<LavaVk::ParticleEffect>("water");
    waterNode->setLayer(0);
    waterNode->getTransform()->setTranslation(WaterPosition);
    waterNode->setComponent<WaterFallAnimation>();
    water->particleCount = 1000000;
    water->aliveParticlesCount = 0;
    water->sizeMultiplier = 0.015f;
    water->particleProperties.emplace_back();
    water->particleProperties[0].source = glm::vec3(-.05f, -.2f, 0.f);
    water->particleProperties[0].color = glm::vec4(.0f, .467f, .745f, .6f);
    water->particleProperties[0].colorDecayRate = glm::vec4(.0f, .0f, .0f, .0f);
    water->particleProperties[0].lifetime = 0.05f;
    water->particleProperties[0].decayRate = 1.f;
    water->particleProperties[0].radiusType = LavaVk::ParticleProperties::RadiusType::Spherical;
    water->particleProperties[0].radius = glm::vec3(.6f, .02f, .6f);
    water->particleProperties[0].deathProbability = 1.f;
    water->particleProperties[0].deathDecayRate = 1.f;
    water->particleProperties[0].forces.emplace_back();
    water->particleProperties[0].forces[0].direction = glm::vec3(1.f, 0.f, -1.f);
    water->particleProperties[0].forces[0].source = glm::vec3(-.2f, .0f, -.2f);
    water->particleProperties[0].forces[0].radius = glm::vec3(.3f, .3f, .3f);
    water->particleProperties[0].forces[0].radiusType = LavaVk::ParticleProperties::RadiusType::Spherical;
    water->particleProperties[0].forces[0].force = 0.5;
    water->particleProperties[0].forces[0].directionType = LavaVk::ParticleProperties::ForceDirectionType::FromParticleInDirection;
    water->particleProperties[0].forces[0].time = 0.05f;
    water->particleProperties[0].forces[0].type = LavaVk::ParticleProperties::ForceType::Velocity;
    water->particleProperties[0].forces.emplace_back();
    water->particleProperties[0].forces[1].direction = glm::vec3(1.f, 0.f, 1.f);
    water->particleProperties[0].forces[1].source = glm::vec3(.2f, .0f, -.2f);
    water->particleProperties[0].forces[1].radius = glm::vec3(.3f, .3f, .3f);
    water->particleProperties[0].forces[1].radiusType = LavaVk::ParticleProperties::RadiusType::Spherical;
    water->particleProperties[0].forces[1].force = 0.5;
    water->particleProperties[0].forces[1].directionType = LavaVk::ParticleProperties::ForceDirectionType::FromParticleInDirection;
    water->particleProperties[0].forces[1].time = 0.05f;
    water->particleProperties[0].forces[1].type = LavaVk::ParticleProperties::ForceType::Velocity;
    water->particleProperties[0].forces.emplace_back();
    water->particleProperties[0].forces[2].direction = glm::vec3(-1.f, 0.f, 1.f);
    water->particleProperties[0].forces[2].source = glm::vec3(.2f, .0f, .2f);
    water->particleProperties[0].forces[2].radius = glm::vec3(.3f, .3f, .3f);
    water->particleProperties[0].forces[2].radiusType = LavaVk::ParticleProperties::RadiusType::Spherical;
    water->particleProperties[0].forces[2].force = 0.5;
    water->particleProperties[0].forces[2].directionType = LavaVk::ParticleProperties::ForceDirectionType::FromParticleInDirection;
    water->particleProperties[0].forces[2].time = 0.05f;
    water->particleProperties[0].forces[2].type = LavaVk::ParticleProperties::ForceType::Velocity;
    water->particleProperties[0].forces.emplace_back();
    water->particleProperties[0].forces[3].direction = glm::vec3(-1.f, 0.f, -1.f);
    water->particleProperties[0].forces[3].source = glm::vec3(-.2f, .0f, .2f);
    water->particleProperties[0].forces[3].radius = glm::vec3(.3f, .3f, .3f);
    water->particleProperties[0].forces[3].radiusType = LavaVk::ParticleProperties::RadiusType::Spherical;
    water->particleProperties[0].forces[3].force = 0.5;
    water->particleProperties[0].forces[3].directionType = LavaVk::ParticleProperties::ForceDirectionType::FromParticleInDirection;
    water->particleProperties[0].forces[3].time = 0.05f;
    water->particleProperties[0].forces[3].type = LavaVk::ParticleProperties::ForceType::Velocity;
    water->particleProperties[0].forces.emplace_back();
    water->particleProperties[0].forces[4].source = glm::vec3(.0f, -.4f, .0f);
    water->particleProperties[0].forces[4].force = -0.7;
    water->particleProperties[0].forces[4].directionType = LavaVk::ParticleProperties::ForceDirectionType::FromSourceToParticle;
    water->particleProperties[0].forces[4].time = 0.05f;
    water->particleProperties[0].forces[4].type = LavaVk::ParticleProperties::ForceType::Velocity;

    water->load(cmd);

    return {scene, renderPipeline};
}

void ParticlesSceneLoader::onSceneLoaded(LavaVk::DIContainer &container)
{
}
