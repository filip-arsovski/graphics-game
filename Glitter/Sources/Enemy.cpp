//
// Created by Filip on 27.12.2025.
//

#include <utility>

#include "enemy.hpp"
#include "Maze.hpp"

PositionData Enemy::playerPosData;

Enemy::Enemy(const std::string &folder, const std::string &file,
const std::string &bulletFolder, const std::string &bulletFile, std::string name,
const PositionData &posData, std::vector<glm::vec3> motionPath):
GameEntity(folder, file, bulletFolder, bulletFile, std::move(name)), motionPath(std::move(motionPath))
{
	this->posData = posData;
}

void Enemy::Shoot(float currentTime, float deltaTime, Maze& M)
{
	bool spottedNow = PlayerSpotted(M.mesh);

	if (spottedNow == true && playerSpotted == false)
		shootBegin = currentTime;

	if (abs(currentTime - shootBegin) < deltaTime && spottedNow == true)
	{
		PositionData data{
			.location = posData.location,
			.direction = normalize(playerPosData.location - posData.location)
		};

		bullets.push_back(std::unique_ptr<Bullet>(new Bullet(bulletFolder, bulletFile, data, 5)));
		shootBegin += shootDelay;
	}

	playerSpotted = spottedNow;

	if (bullets.size() > 10) bullets.erase(bullets.begin());
	for (auto &bullet: bullets)
		bullet->posData.location += bulletSpeed*deltaTime*bullet->posData.direction;
}

void Enemy::Move(const float deltaTime, Maze& maze)
{
	const float velocity = 0.5f * deltaTime;
	const float threshold = 1.1f * velocity;

	if (distance(posData.location, motionPath[motionPathCurrentIndex + pathTraversal]) < threshold)
	{
		posData.location = motionPath[motionPathCurrentIndex + pathTraversal];

		motionPathCurrentIndex += pathTraversal;
		if (motionPathCurrentIndex == motionPath.size() - 1) pathTraversal = BACKWARDS;
		if (motionPathCurrentIndex == 0) pathTraversal = FORWARDS;

		posData.direction = motionPath[motionPathCurrentIndex + pathTraversal] - motionPath[motionPathCurrentIndex];
		posData.direction = normalize(posData.direction);
	}
	else if (!PlayerSpotted(maze.mesh)) posData.location += velocity * posData.direction;

	// move enemy model
	mesh.Move(posData.location, posData.prevLocation);
	posData.prevLocation = posData.location;
}

void Enemy::UpdateShaderMatrix(Camera &camera, const int mWidth, const int mHeight)
{
	shaderMatrix.model = glm::mat4(1.0f);
	shaderMatrix.model = translate(shaderMatrix.model, posData.location);
	//shaderMatrix.model = scale(shaderMatrix.model, glm::vec3(0.05f));
	shaderMatrix.view = camera.GetViewMatrix();
	shaderMatrix.projection = glm::perspective(glm::radians(camera.Zoom), mWidth * 1.0f / mHeight, 0.1f, 100.0f);
}

bool Enemy::PlayerSpotted(Mirage::Mesh& mesh)
{
	for (int i = 0; i < mesh.mIndices.size(); i += 3)
	{
		glm::vec3 p1 = mesh.mVertices[mesh.mIndices[i]].position;
		glm::vec3 p2 = mesh.mVertices[mesh.mIndices[i + 1]].position;
		glm::vec3 p3 = mesh.mVertices[mesh.mIndices[i + 2]].position;
		Triangle T(p1, p2, p3);
		if (LineOfSightObstructed(T)) return false;
	}

	bool spotted = true;
	for (const auto& i: mesh.mSubMeshes)
	{
		spotted = spotted && PlayerSpotted(*i);
	}

	return spotted;
}

bool Enemy::LineOfSightObstructed(const Triangle& triangle) const
{
	glm::vec3 t = normalize(posData.location - playerPosData.location);
	float quotient = dot(t, triangle.Normal);
	if (quotient == 0) return false;

	float intersection = dot((triangle.p1 - playerPosData.location), triangle.Normal) / quotient;
	glm::vec3 p = playerPosData.location + intersection*t;
	return distance(p, posData.location) < distance(playerPosData.location, posData.location)
			&& distance(p, playerPosData.location) < distance(playerPosData.location, posData.location)
			&& pointInTriangle(p, triangle.p1, triangle.p2, triangle.p3);
}

bool Enemy::IsDead() const {return health <= 0;}
