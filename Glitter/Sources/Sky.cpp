//
// Created by Filip on 13.1.2026.
//

#include "Sky.hpp"

void Sky::UpdateShaderMatrix(Camera &camera, const int mWidth, const int mHeight)
{
	shaderMatrix.model = translate(glm::mat4(1.0f), camera.Position);
	shaderMatrix.view = camera.GetViewMatrix();
	shaderMatrix.projection = glm::perspective(glm::radians(camera.Zoom), mWidth * 1.0f / mHeight, 0.1f, 100.0f);
}
