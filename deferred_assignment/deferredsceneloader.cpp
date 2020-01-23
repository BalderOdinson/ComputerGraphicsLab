//
// Created by oshikuru on 20. 01. 2020..
//

#define DEFERRED_RENDERING_ON 1

#include "deferredsceneloader.h"

std::type_index DeferredSceneLoader::getType() const
{
    return typeid(DeferredSceneLoader);
}

std::pair<LavaVk::SharedScene, LavaVk::SharedRenderPipeline>
DeferredSceneLoader::load(const LavaVk::Core::SharedCommandBuffer &cmd)
{
    auto &container = LavaVk::Application::instance->container;
#if DEFERRED_RENDERING_ON
    auto geometrySubpassOptions = LavaVk::make<LavaVk::GeometrySubpassOptions>();
    geometrySubpassOptions->renderers.push_back(
            container.resolve<LavaVk::DeferredSpecularRenderer, LavaVk::DeferredRendererOptions>(
                    container.resolve<LavaVk::SpecularMeshRenderer, LavaVk::GeometryRenderOptions>(
                            LavaVk::AlphaMode::Opaque)));
    geometrySubpassOptions->renderers.push_back(
            container.resolve<LavaVk::DeferredSpecularRenderer, LavaVk::DeferredRendererOptions>(
                    container.resolve<LavaVk::SpecularMeshRenderer, LavaVk::GeometryRenderOptions>(
                            LavaVk::AlphaMode::Blend)));
    auto geometrySubpass = container.resolve<LavaVk::GeometrySubpass, LavaVk::GeometrySubpassOptions>(
            geometrySubpassOptions);
    geometrySubpass->setOutputAttachments({1, 2, 3, 4});

    auto lightSubpassOptions = LavaVk::make<LavaVk::LightSubpassOptions>(
            container.resolve<LavaVk::SpecularLightRenderer>());
    auto lightSubpass = container.resolve<LavaVk::LightSubpass, LavaVk::LightSubpassOptions>(lightSubpassOptions);
    lightSubpass->setInputAttachments({1, 2, 3, 4});
    lightSubpass->setOutputAttachments({0});

    auto renderPipeline = LavaVk::make<LavaVk::RenderPipeline>(
            LavaVk::RenderPipelineOptions({geometrySubpass, lightSubpass}));

    // Clear every attachment and store only swapchain
    std::vector<LavaVk::Core::LoadStoreInfo> loadStore{5};
    // Swapchain
    loadStore[0].loadOp = vk::AttachmentLoadOp::eClear;
    loadStore[0].storeOp = vk::AttachmentStoreOp::eStore;

    // Depth
    loadStore[1].loadOp = vk::AttachmentLoadOp::eClear;
    loadStore[1].storeOp = vk::AttachmentStoreOp::eDontCare;

    // Albedo
    loadStore[2].loadOp = vk::AttachmentLoadOp::eClear;
    loadStore[2].storeOp = vk::AttachmentStoreOp::eDontCare;

    // Specular
    loadStore[3].loadOp = vk::AttachmentLoadOp::eClear;
    loadStore[3].storeOp = vk::AttachmentStoreOp::eDontCare;

    // Normal
    loadStore[4].loadOp = vk::AttachmentLoadOp::eClear;
    loadStore[4].storeOp = vk::AttachmentStoreOp::eDontCare;

    // Clear values
    std::vector<vk::ClearValue> clearValue{5};
    clearValue[0].color = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
    clearValue[1].depthStencil = vk::ClearDepthStencilValue{1.0f, ~0U};
    clearValue[2].color = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
    clearValue[3].color = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
    clearValue[4].color = vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});

    renderPipeline->setLoadStore(loadStore);
    renderPipeline->setClearValue(clearValue);
#else
    auto geometrySubpassOptions = LavaVk::make<LavaVk::GeometrySubpassOptions>();
    geometrySubpassOptions->renderers.push_back(
    container.resolve<LavaVk::ForwardSpecularLightsRenderer, LavaVk::ForwardLightsRendererOptions>(
            container.resolve<LavaVk::SpecularMeshRenderer, LavaVk::GeometryRenderOptions>(
                    LavaVk::AlphaMode::Opaque), 0, 1, "fw"));
    geometrySubpassOptions->renderers.push_back(
            container.resolve<LavaVk::ForwardSpecularLightsRenderer, LavaVk::ForwardLightsRendererOptions>(
                    container.resolve<LavaVk::SpecularMeshRenderer, LavaVk::GeometryRenderOptions>(
                            LavaVk::AlphaMode::Blend), 0, 1, "fw"));
    auto geometrySubpass = container.resolve<LavaVk::GeometrySubpass, LavaVk::GeometrySubpassOptions>(
            geometrySubpassOptions);

    auto renderPipeline = LavaVk::make<LavaVk::RenderPipeline>(
            LavaVk::RenderPipelineOptions({geometrySubpass}));
#endif

    auto scene = LavaVk::make<LavaVk::Scene>("Deferred scene");

    auto cameraNode = LavaVk::Node::make("cameraNode");
    auto cityNode = LavaVk::Node::make("cityNode");
    scene->addChild(cameraNode);
    scene->addChild(cityNode);

    auto cityMesh = cityNode->setComponent<LavaVk::Mesh>(
            LavaVk::make<LavaVk::ObjectFileMeshSource>(CITY_PATH, true, true, false), "container");
    cityMesh->load(cmd);

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
    cameraNode->getTransform()->setTranslation(CameraPosition);

    auto sphereObject = LavaVk::make<LavaVk::Geometry>(
            LavaVk::make<LavaVk::ObjectFileVertexSource>(SPHERE_PATH, true, true, false));
    sphereObject->load(cmd);

    auto bounds = cityMesh->getBounds();
    std::mt19937 rng(100);
    std::uniform_real_distribution<float> x(-1500.f, 2000.f);
    std::uniform_real_distribution<float> y(0.f, bounds->getMax().y / 2.f);
    std::uniform_real_distribution<float> z(-2200.f, -500.f);
    std::uniform_real_distribution<float> color;

    for (uint32_t i = 0; i < LightsCount; ++i)
    {
        auto sphereMaterial = LavaVk::make<LavaVk::SpecularMaterial>("sphereMaterial" + std::to_string(i));
        sphereMaterial->diffuse = glm::vec4{color(rng), color(rng), color(rng), 1.f};

#if DEFERRED_RENDERING_ON
        sphereMaterial->emissive = glm::vec4{1.f};
#else
        sphereMaterial->emissive = glm::vec4{color(rng), color(rng), color(rng), 1.f};
        sphereMaterial->specular = glm::vec4(glm::vec3(sphereMaterial->diffuse), 60.f);
#endif

        auto submesh = LavaVk::make<LavaVk::SubMesh>(sphereObject, sphereMaterial, "sphere");

        auto lightNode = LavaVk::Node::make("lightNode" + std::to_string(i));
        scene->addChild(lightNode);

        auto mesh = lightNode->setComponent<LavaVk::Mesh>(
                LavaVk::make<LavaVk::MemoryMeshSource>(std::initializer_list<LavaVk::SharedSubMesh>{submesh}));
        mesh->load(cmd);

        auto pointLight = lightNode->setComponent<LavaVk::SpecularLight>("pointLight" + std::to_string(i));
        lightNode->getTransform()->setTranslation({x(rng), y(rng), z(rng)});
        lightNode->getTransform()->setScale(SphereScale);
        pointLight->color = glm::vec3(sphereMaterial->diffuse);
        pointLight->ambient = {.01f, .01f, .01f};
        pointLight->specular = glm::vec3(sphereMaterial->diffuse);
        pointLight->intensityConstant = 1.f;
        pointLight->intensityLinear = 0.027f;
        pointLight->intensityQuadratic = 0.0028f;
        pointLight->lightType = LavaVk::LightType::Point;
    }

    return {scene, renderPipeline};
}

void DeferredSceneLoader::onSceneLoaded(LavaVk::DIContainer &container)
{
#if DEFERRED_RENDERING_ON
    container.option<LavaVk::MultisamplingOptions>()->samples = vk::SampleCountFlagBits::e1;
    container.addOption<LavaVk::RenderTargetOptions>([]()
                                                     { return new LavaVk::DeferredSpecularRenderTargetOptions(); });
    container.resolve<LavaVk::RenderContext>()->recreate();
#endif
}
