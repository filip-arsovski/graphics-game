//
// Created by Filip on 11.1.2026.
//

#include "Lamp.hpp"

void Lamp::UpdateShaderMatrix(Camera &camera, const int mWidth, const int mHeight)
{
	shaderMatrix.model = translate(glm::mat4(1.0f), position);
	shaderMatrix.view = camera.GetViewMatrix();
	shaderMatrix.projection = glm::perspective(glm::radians(camera.Zoom), mWidth * 1.0f / mHeight, 0.1f, 100.0f);
}
