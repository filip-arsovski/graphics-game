//
// Created by Filip on 29.12.2025.
//

#include "GunView.hpp"

void GunView::UpdateShaderMatrix(Camera &camera, const int mWidth, const int mHeight)
{
	shaderMatrix.model = glm::mat4(1.0f);
	shaderMatrix.model = translate(shaderMatrix.model, camera.Position);
	shaderMatrix.model = rotate(shaderMatrix.model, glm::radians(-camera.Yaw + 90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	shaderMatrix.model = rotate(shaderMatrix.model, glm::radians(-camera.Pitch - 90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shaderMatrix.model = scale(shaderMatrix.model, glm::vec3(0.002f, 0.002f, 0.002f));
	shaderMatrix.view = camera.GetViewMatrix();
	shaderMatrix.projection = glm::perspective(glm::radians(30.0f), mWidth * 1.0f / mHeight, 0.1f, 100.0f);
}
