//
// Created by Filip on 01.12.2025.
//

#include "Maze.hpp"


Maze::Maze(const std::string &folder, const std::string &file): Drawable(folder, file){}

void Maze::UpdateShaderMatrix(Camera &camera, const int mWidth, const int mHeight)
{
	shaderMatrix.model = glm::mat4(1.0f);
	shaderMatrix.view = camera.GetViewMatrix();
	shaderMatrix.projection = glm::perspective(glm::radians(camera.Zoom), mWidth * 1.0f / mHeight, 0.1f, 100.0f);
}