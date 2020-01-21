//
// Created by oshikuru on 12. 01. 2020..
//

#include "bsplinesceneloader.h"
#include "positiontrackerscript.h"
#include "bsplineanimation.h"

#define RECORD_SPLINE 0

std::type_index BSplineSceneLoader::getType() const
{
    return typeid(BSplineSceneLoader);
}

std::pair<LavaVk::SharedScene, LavaVk::SharedRenderPipeline>
BSplineSceneLoader::load(const LavaVk::Core::SharedCommandBuffer &cmd)
{
    auto &container = LavaVk::Application::instance->container;
    auto subpassOptions = LavaVk::make<LavaVk::GeometrySubpassOptions>();
    subpassOptions->renderers.push_back(
            container.resolve<LavaVk::ForwardSpecularLightsRenderer, LavaVk::ForwardLightsRendererOptions>(
                    container.resolve<LavaVk::SpecularMeshRenderer, LavaVk::GeometryRenderOptions>(
                            LavaVk::AlphaMode::Opaque), 0, 1, "fw"));
    subpassOptions->renderers.push_back(
            container.resolve<LavaVk::CubicBSplineRenderer, LavaVk::GeometryRenderOptions>(LavaVk::AlphaMode::Opaque));
    subpassOptions->renderers.push_back(container.resolve<LavaVk::SkyboxRenderer>());
    subpassOptions->renderers.push_back(container.resolve<LavaVk::ParticleRenderer>());
    subpassOptions->renderers.push_back(
            container.resolve<LavaVk::ForwardSpecularLightsRenderer, LavaVk::ForwardLightsRendererOptions>(
                    container.resolve<LavaVk::SpecularMeshRenderer, LavaVk::GeometryRenderOptions>(
                            LavaVk::AlphaMode::Blend), 0, 1, "fw"));
    subpassOptions->renderers.push_back(
            container.resolve<LavaVk::CubicBSplineRenderer, LavaVk::GeometryRenderOptions>(LavaVk::AlphaMode::Blend));

    auto subpass = container.resolve<LavaVk::GeometrySubpass, LavaVk::GeometrySubpassOptions>(subpassOptions);
    auto renderPipeline = LavaVk::make<LavaVk::RenderPipeline>(LavaVk::RenderPipelineOptions({subpass}));


    auto scene = LavaVk::make<LavaVk::Scene>("B-spline scene");

    auto cityNode = LavaVk::Node::make("cityNode");
    auto f14Node = LavaVk::Node::make("f14Node");
    auto cameraNode = LavaVk::Node::make("cameraNode");
    auto sunNode = LavaVk::Node::make("sunNode");
    auto skyboxNode = LavaVk::Node::make("skyboxNode");
    auto bsplineNode = LavaVk::Node::make("bsplineNode");
    auto rainNode1 = LavaVk::Node::make("rainNode1");
    auto rainNode2 = LavaVk::Node::make("rainNode2");
    auto rainNode3 = LavaVk::Node::make("rainNode3");
    auto rainNode4 = LavaVk::Node::make("rainNode4");
    auto trackerNode = LavaVk::Node::make("trackerNode");
    scene->addChild(cityNode);
    scene->addChild(cameraNode);
    scene->addChild(sunNode);
    scene->addChild(skyboxNode);
    scene->addChild(rainNode1);
    scene->addChild(rainNode2);
    scene->addChild(rainNode3);
    scene->addChild(rainNode4);
    scene->addChild(bsplineNode);

    auto skybox = skyboxNode->setComponent<LavaVk::Skybox>(LavaVk::make<LavaVk::CubemapFileImageSource>(
            CUBEMAP_RIGHT_PATH,
            CUBEMAP_LEFT_PATH,
            CUBEMAP_UP_PATH,
            CUBEMAP_DOWN_PATH,
            CUBEMAP_BACKWARD_PATH,
            CUBEMAP_FORWARD_PATH),
                                                           LavaVk::make<LavaVk::ObjectFileVertexSource>(
                                                                   CUBEMAP_MODEL, false, false, false));
    skybox->load(cmd);

    auto cityMesh = cityNode->setComponent<LavaVk::Mesh>(
            LavaVk::make<LavaVk::ObjectFileMeshSource>(CITY_PATH, true, true, false), "city");
    cityMesh->load(cmd);

    scene->addChild(f14Node);
    auto f14Mesh = f14Node->setComponent<LavaVk::Mesh>(
            LavaVk::make<LavaVk::ObjectFileMeshSource>(F14_MODEL_PATH, true, true, false), "f14");
    f14Mesh->load(cmd);

    auto animationScript = f14Node->setComponent<BSplineAnimation>();
    animationScript->speedFactor = 4.5f;

    auto camera = cameraNode->setComponent<LavaVk::PerspectiveCamera>("mainCamera");
    auto cameraScript = cameraNode->setComponent<LavaVk::FreeCamera>();
    cameraScript->translationMoveStep = CameraSpeed;
    auto window = container.resolve<LavaVk::Window>();
    camera->setAspectRatio(static_cast<float>(window->getWidth()) / window->getHeight());
    camera->setFieldOfView(1.0f);
    camera->setNearPlane(0.1f);
    camera->setFarPlane(7000.0f);
    cameraNode->getTransform()->setRotation(
            CameraRotation * glm::inverse(cameraNode->getTransform()->getRotation()));
#if RECORD_SPLINE
    cameraNode->getTransform()->setTranslation(F14Position);
#else
    cameraNode->getTransform()->setTranslation(CameraPosition);
#endif

    auto directionalLight = sunNode->setComponent<LavaVk::SpecularLight>("directionalLight");
    directionalLight->color = {.5f, .5f, .5f};
    directionalLight->ambient = {.1f, .1f, .1f};
    directionalLight->specular = {1.f, 1.f, 1.f};
    directionalLight->direction = -LightDirection;
    directionalLight->lightType = LavaVk::LightType::Directional;


    auto rain = rainNode1->setComponent<LavaVk::ParticleEffect>("rain");
    auto nodeTransform = cityNode->getTransform()->getWorldMatrix();
    auto meshBounds = cityMesh->getBounds();
    LavaVk::AABB worldBounds(meshBounds->getMin(), meshBounds->getMax());
    worldBounds.transform(nodeTransform);
    auto radius = (worldBounds.getMax() - worldBounds.getMin()) / glm::vec3(4);
    auto center = worldBounds.getCenter();
    auto center1 = glm::vec3((worldBounds.getMin().x + center.x) * 0.5f, 1500.f,
                             (worldBounds.getMax().z + center.z) * 0.5f);
    radius.y = 500.f;
    center.y = 0;
    rainNode1->getTransform()->setTranslation(center1);
    rain->particleCount = 50000;
    rain->aliveParticlesCount = 0;
    rain->sizeMultiplier = 0.4;
    rain->motionBlur = 0.07f;
    rain->stretchWithVelocity = true;
    rain->particleProperties.emplace_back();
    rain->particleProperties[0].lifetime = 5.f;
    rain->particleProperties[0].decayRate = 1.f;
    rain->particleProperties[0].color = {.731f, .724f, .743f, .7f};
    rain->particleProperties[0].colorDecayRate = glm::vec4{0.f, 0.f, 0.f, .1f};
    rain->particleProperties[0].deathProbability = 0.99f;
    rain->particleProperties[0].deathDecayRate = 0.9f;
    rain->particleProperties[0].radius = radius;
    rain->particleProperties[0].source = {0.f, 0.f, 0.f};
    rain->particleProperties[0].radiusType = LavaVk::ParticleProperties::RadiusType::Cube;
    rain->particleProperties[0].forces.emplace_back();
    rain->particleProperties[0].forces[0].type = LavaVk::ParticleProperties::ForceType::Acceleration;
    rain->particleProperties[0].forces[0].time = 5.f;
    rain->particleProperties[0].forces[0].force = 9.81f;
    rain->particleProperties[0].forces[0].directionType = LavaVk::ParticleProperties::ForceDirectionType::FromParticleInDirection;
    rain->particleProperties[0].forces[0].direction = {0, -1, 0};
    rain->particleProperties[0].forces.emplace_back();
    rain->particleProperties[0].forces[1].type = LavaVk::ParticleProperties::ForceType::Velocity;
    rain->particleProperties[0].forces[1].time = 5.f;
    rain->particleProperties[0].forces[1].force = 500.f;
    rain->particleProperties[0].forces[1].directionType = LavaVk::ParticleProperties::ForceDirectionType::FromParticleInDirection;
    rain->particleProperties[0].forces[1].direction = {0, -1, 0};

    auto center2 = glm::vec3((worldBounds.getMax().x + center.x) * 0.5f, 1500.f,
                             (worldBounds.getMax().z + center.z) * 0.5f);
    rainNode2->setComponent(rain);
    rainNode2->getTransform()->setTranslation(center2);

    auto center3 = glm::vec3((worldBounds.getMin().x + center.x) * 0.5f, 1500.f,
                             (worldBounds.getMin().z + center.z) * 0.5f);
    rainNode3->setComponent(rain);
    rainNode3->getTransform()->setTranslation(center3);

    auto center4 = glm::vec3((worldBounds.getMax().x + center.x) * 0.5f, 1500.f,
                             (worldBounds.getMin().z + center.z) * 0.5f);
    rainNode4->setComponent(rain);
    rainNode4->getTransform()->setTranslation(center4);

    rain->load(cmd);

    auto bspline = bsplineNode->setComponent<LavaVk::CubicBSpline>(
            LavaVk::make<LavaVk::ObjectFileVertexSource>(BSPLINE_PATH, false, false, false));
    bspline->lineColor = {1.f, 1.f, 1.f, .2f};
    bspline->alphaMode = LavaVk::AlphaMode::Blend;
    bspline->load(cmd);

#if RECORD_SPLINE
    cameraNode->addChild(trackerNode);
    trackerNode->setComponent<PositionTrackerScript>();
#endif

    return {scene, renderPipeline};
}

void BSplineSceneLoader::onSceneLoaded(LavaVk::DIContainer &container)
{
}
