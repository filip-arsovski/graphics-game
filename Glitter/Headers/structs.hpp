//
// Created by Filip on 08.12.2025.
//

#ifndef STRUCTS_HPP
#define STRUCTS_HPP
#include <glm/glm.hpp>

struct Triangle
{
	glm::vec3 p1, p2, p3;
	glm::vec3 Normal;

	Triangle() = default;
	Triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
		  : p1(a), p2(b), p3(c)
	{
		Normal = normalize(cross(p2 - p1, p3 - p1));
	}
};

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

struct ShaderMatrix
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
};

struct PositionData
{
	glm::vec3 location;
	glm::vec3 prevLocation;
	glm::vec3 direction;
};

#endif //STRUCTS_HPP
