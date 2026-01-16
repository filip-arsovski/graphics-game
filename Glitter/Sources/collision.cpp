//
// Created by Filip on 05.12.2025.
//

#include "collision.hpp"
#include <glm/glm.hpp>
#include "Mesh.hpp"

bool pointInTriangle(const glm::vec3 &P,
                     const glm::vec3 &A,
                     const glm::vec3 &B,
                     const glm::vec3 &C)
{
	glm::vec3 v0 = C - A;
	glm::vec3 v1 = B - A;
	glm::vec3 v2 = P - A;

	float dot00 = glm::dot(v0, v0);
	float dot01 = glm::dot(v0, v1);
	float dot02 = glm::dot(v0, v2);
	float dot11 = glm::dot(v1, v1);
	float dot12 = glm::dot(v1, v2);

	float denom = dot00 * dot11 - dot01 * dot01;
	if (denom == 0) return false; // Degenerate triangle

	float u = (dot11 * dot02 - dot01 * dot12) / denom;
	float v = (dot00 * dot12 - dot01 * dot02) / denom;

	return (u >= 0) && (v >= 0) && (u + v <= 1);
}

bool isColliding(const glm::vec3 position, const Triangle& T, float threshold)
{
	float t = dot(T.Normal, T.p1 - position);
	glm::vec3 projection = position + t*T.Normal;
	const glm::vec3 diff = projection - position;

	if (pointInTriangle(projection, T.p1, T.p2, T.p3))
		return dot(diff, diff) < threshold * threshold;

	return false;
}

bool isColliding(glm::vec3 position, const Mirage::Mesh& mesh, float threshold)
{
	for (int i = 0; i < mesh.mIndices.size(); i += 3)
	{
		glm::vec3 p1 = mesh.mVertices[mesh.mIndices[i]].position;
		glm::vec3 p2 = mesh.mVertices[mesh.mIndices[i + 1]].position;
		glm::vec3 p3 = mesh.mVertices[mesh.mIndices[i + 2]].position;
		Triangle T(p1, p2, p3);
		if (isColliding(position, T, threshold)) return true;
	}

	bool colliding = false;
	for (const auto& i: mesh.mSubMeshes)
	{
		colliding = colliding || isColliding(position, *i, threshold);
	}
	return colliding;
}

bool isColliding(const glm::vec3 position, const Triangle& T, const float threshold, Triangle& collidingT)
{
	float t = dot(T.Normal, T.p1 - position);
	glm::vec3 projection = position + t*T.Normal;
	const glm::vec3 diff = projection - position;

	if (pointInTriangle(projection, T.p1, T.p2, T.p3) && dot(diff, diff) < threshold * threshold)
	{
		collidingT.Normal = T.Normal;
		return true;
	}

	return false;
}

bool isColliding(glm::vec3 position, const Mirage::Mesh& mesh, float threshold, Triangle& collidingT)
{
	for (int i = 0; i < mesh.mIndices.size(); i += 3)
	{
		glm::vec3 p1 = mesh.mVertices[mesh.mIndices[i]].position;
		glm::vec3 p2 = mesh.mVertices[mesh.mIndices[i + 1]].position;
		glm::vec3 p3 = mesh.mVertices[mesh.mIndices[i + 2]].position;
		Triangle T(p1, p2, p3);
		if (isColliding(position, T, threshold, collidingT)) return true;
	}

	bool colliding = false;
	for (const auto& i: mesh.mSubMeshes)
	{
		colliding = colliding || isColliding(position, *i, threshold, collidingT);
	}
	return colliding;
}


