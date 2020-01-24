//
// Created by oshikuru on 12. 01. 2020..
//

#include "bsplineanimation.h"
#include "bsplinesceneloader.h"
#include "bsplinesample.h"
#include <fstream>
#include <sstream>

BSplineAnimation::BSplineAnimation()
        : NodeAnimation(std::bind(&BSplineAnimation::animate, this, std::placeholders::_1, std::placeholders::_2))
{
    std::ifstream file(BSplineSceneLoader::BSPLINE_PATH);
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream is(line);
        char c;
        is >> c;
        glm::vec3 position;
        is >> position.x >> position.y >> position.z;
        points.emplace_back(position);
    }
    file = std::ifstream(BSplineSceneLoader::BSPLINE_ROLL_PATH);
    while (std::getline(file, line))
    {
        std::istringstream is(line);
        float roll;
        is >> roll;
        rolls.emplace_back(roll);
    }
}

void BSplineAnimation::initialize(LavaVk::DIContainer &container)
{
}

void BSplineAnimation::animate(const LavaVk::SharedTransform &transform, float delta)
{
    static glm::mat4 bspline(glm::vec4(-1.f, 3.f, -3.f, 1.f),
                             glm::vec4(3.f, -6.f, 0.f, 4.f),
                             glm::vec4(-3.f, 3.f, 3.f, 1.f),
                             glm::vec4(1.f, 0.f, 0.f, 0.f));

    static glm::mat4x3 bsplineDerv(glm::vec3(-1.f, 2.f, -1.f),
                                   glm::vec3(3.f, -4.f, 0.f),
                                   glm::vec3(-3.f, 2.f, 1.f),
                                   glm::vec3(1.f, 0.f, 0.f));

    static glm::mat4x2 bsplineSecondDerv(glm::vec2(-1.f, 1.f),
                                         glm::vec2(3.f, -2.f),
                                         glm::vec2(-3.f, 1.f),
                                         glm::vec2(1.f, 0.f));

    totalTime += speedFactor * delta;


    segment = std::floor(totalTime);
    auto t = totalTime - segment;

    segment %= points.size() - 3;

    auto r0 = points[segment];
    auto r1 = points[segment + 1];
    auto r2 = points[segment + 2];
    auto r3 = points[segment + 3];

    auto q0 = rolls[segment];
    auto q1 = rolls[segment + 1];

    auto pointsVector = glm::mat3x4(glm::vec4(r0.x, r1.x, r2.x, r3.x),
                                    glm::vec4(r0.y, r1.y, r2.y, r3.y),
                                    glm::vec4(r0.z, r1.z, r2.z, r3.z));


    auto spline = ((glm::vec4(t * t * t, t * t, t, 1) / 6.f) * bspline);
    glm::vec3 newPoint = spline * pointsVector;
    glm::vec3 tangent = (glm::vec3(t * t, t, 1) / 2.f) * bsplineDerv * pointsVector;
    glm::vec3 normal = glm::normalize(glm::cross(tangent, (glm::vec2(t, 1) * bsplineSecondDerv * pointsVector)));
    glm::vec3 binormal = glm::normalize(glm::cross(tangent, normal));
    tangent = glm::cross(normal, binormal);

    auto newOrientation = glm::quatLookAt(glm::normalize(-tangent), glm::vec3(0.f, 1.f, 0.f));

    auto roll = glm::slerp(glm::angleAxis(q0, glm::vec3(0.f, 0.f, 1.f)),
                           glm::angleAxis(q1, glm::vec3(0.f, 0.f, 1.f)), t);

    transform->setTranslation(newPoint);
    transform->setRotation(glm::normalize(newOrientation * roll));
}

