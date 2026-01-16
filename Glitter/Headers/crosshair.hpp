//
// Created by Filip on 24.12.2025.
//

#ifndef CROSSHAIR_HPP
#define CROSSHAIR_HPP

#include <glm/glm.hpp>
#include <vector>

float size = 0.04f;
float thickness = 0.007f;
float gap = 0.015f;

std::vector<glm::vec2> crosshairVertices =
	{
	// left
	glm::vec2(-size - gap, thickness/2),
	glm::vec2(-gap, thickness/2),
	glm::vec2(-size - gap, -thickness/2),

	glm::vec2(-gap, thickness/2),
	glm::vec2(-size -gap, -thickness/2),
	glm::vec2(-gap, -thickness/2),

	// up
	glm::vec2(-thickness/2, size + gap),
	glm::vec2(thickness/2, size + gap),
	glm::vec2(thickness/2, gap),

	glm::vec2(-thickness/2, size + gap),
	glm::vec2(thickness/2, gap),
	glm::vec2(-thickness/2, gap),

	// right
	glm::vec2(size + gap, -thickness/2),
	glm::vec2(gap, -thickness/2),
	glm::vec2(size + gap, thickness/2),

	glm::vec2(gap, -thickness/2),
	glm::vec2(size + gap, thickness/2),
	glm::vec2(gap, thickness/2),

	// down
	glm::vec2(thickness/2, -size - gap),
	glm::vec2(-thickness/2, -size - gap),
	glm::vec2(-thickness/2, -gap),

	glm::vec2(thickness/2, -size - gap),
	glm::vec2(-thickness/2, -gap),
	glm::vec2(thickness/2, -gap)
};

#endif //CROSSHAIR_HPP
