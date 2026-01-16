//
// Created by Filip on 28.12.2025.
//

#include "Bullet.hpp"


Bullet::Bullet(const std::string &folder, const std::string &file,
	const PositionData &posData, const int damage):
Drawable(folder, file), posData(posData), damage(damage) {}

void Bullet::UpdateShaderMatrix(Camera &camera, const int mWidth, const int mHeight)
{
	const glm::vec3 localDirection(0.0f, 1.0f, 0.0f); // bullet's direction in local space (which way the bullet tip faces)
	const float rotationAngle = glm::acos(dot(localDirection, posData.direction));
	const glm::vec3 rotationAxis = normalize(cross(localDirection, posData.direction));

	shaderMatrix.model = glm::mat4(1.0f);
	shaderMatrix.model = translate(shaderMatrix.model, posData.location);
	shaderMatrix.model = rotate(shaderMatrix.model, rotationAngle, rotationAxis);
	shaderMatrix.model = scale(shaderMatrix.model, glm::vec3(0.0001f, 0.0001f, 0.0001f));
	shaderMatrix.view = camera.GetViewMatrix();
	shaderMatrix.projection = glm::perspective(glm::radians(camera.Zoom), mWidth * 1.0f / mHeight, 0.1f, 100.0f);
}