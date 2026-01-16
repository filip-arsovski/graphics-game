//
// Created by Filip on 26.12.2025.
//
#include "Player.hpp"

#include <utility>

Player::Player(const std::string &folder, const std::string &file,
	const std::string& bulletFolder, const std::string &bulletFile, std::string name,
	Camera &camera):
GameEntity(folder, file, bulletFolder, bulletFile, std::move(name)), playerCamera(camera)
{
	posData.location = camera.Position;
	for (auto& vertex: this->mesh.mVertices)
	{
		glm::mat4 scaling = scale(glm::mat4(1.0f), glm::vec3(0.3f)); // I * S
		glm::mat4 translation = translate(glm::mat4(1.0f), posData.location); // I * T

		vertex.position = translation * scaling * glm::vec4(vertex.position, 1.0f); // v = T * S * v
	}
}

Player::Player(std::vector<Vertex> const &vertices, std::vector<unsigned int> const &indices,
	const std::string &bulletFolder, const std::string &bulletFile, std::string name, Camera &camera):
GameEntity(vertices, indices, bulletFolder, bulletFile, std::move(name)), playerCamera(camera)
{
	posData.location = camera.Position;
	for (auto& vertex: this->mesh.mVertices)
	{
		glm::mat4 scaling = scale(glm::mat4(1.0f), glm::vec3(0.3f)); // I * S
		glm::mat4 translation = translate(glm::mat4(1.0f), posData.location); // I * T

		vertex.position = translation * scaling * glm::vec4(vertex.position, 1.0f); // v = T * S * v
	}
}

void Player::Shoot(const float currentTime, const float deltaTime, Maze& M)
{
	if (playerCamera.shootStartTime == currentTime)
	{
		PositionData data
		{
			.location = playerCamera.Position,
			.direction = normalize(playerCamera.Front)
		};
		bullets.push_back(std::unique_ptr<Bullet>(new Bullet(bulletFolder, bulletFile, data, 35)));
	}

	if (bullets.size() > 10) bullets.erase(bullets.begin());

	for (auto &bullet: bullets)
		bullet->posData.location += playerCamera.MovementSpeed*deltaTime*3.0f*bullet->posData.direction;
}

void Player::Move()
{
	// update player position
	posData.location = playerCamera.Position;
	posData.prevLocation = playerCamera.previousPosition;

	// update hitbox position
	mesh.Move(posData.location, posData.prevLocation);
	posData.prevLocation = posData.location;
}

void Player::UpdateShaderMatrix(Camera &camera, const int mWidth, const int mHeight)
{
	shaderMatrix.model = glm::mat4(1.0f);
	shaderMatrix.model = translate(shaderMatrix.model, posData.location);
	shaderMatrix.model = scale(shaderMatrix.model, glm::vec3(0.3f));
	shaderMatrix.view = camera.GetViewMatrix();
	shaderMatrix.projection = glm::perspective(glm::radians(camera.Zoom), mWidth * 1.0f / mHeight, 0.1f, 100.0f);
}
