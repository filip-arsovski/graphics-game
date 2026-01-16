//
// Created by Filip on 04.12.2025.
//

#ifndef COLLISION_HPP
#define COLLISION_HPP
#include <glm/glm.hpp>
#include "Mesh.hpp"

bool pointInTriangle(const glm::vec3 &P,
                     const glm::vec3 &A,
                     const glm::vec3 &B,
                     const glm::vec3 &C);

bool isColliding(glm::vec3 position, const Triangle& T, float threshold);

bool isColliding(glm::vec3 position, const Mirage::Mesh& mesh, float threshold);

bool isColliding(glm::vec3 position, const Mirage::Mesh& mesh, float threshold, Triangle& collidingT);

#endif //COLLISION_HPP
