//
// Created by Filip on 31.12.2025.
//

#ifndef RECT_HPP
#define RECT_HPP
#include <vector>
#include <glm/vec2.hpp>

float width = 0.5f;
float height = 0.1f;
glm::vec2 offset(-0.9f, 0.75f);

std::vector<glm::vec2> rectangle =
	{
		offset,
		glm::vec2(width, 0.0f) + offset,
		glm::vec2(width, height) + offset,

		offset,
		glm::vec2(width, height) + offset,
		glm::vec2(0.0f, height) + offset
	};

#endif //RECT_HPP
