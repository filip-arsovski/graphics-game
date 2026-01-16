//
// Created by Filip on 24.12.2025.
//

#ifndef ENEMY_HPP
#define ENEMY_HPP
#include "GameEntity.hpp"
#include "collision.hpp"
#include "Maze.hpp"
#include "Mesh.hpp"
#include "structs.hpp"

enum PathTraversal {FORWARDS = 1, BACKWARDS = -1};

class Enemy : public GameEntity
{
	public:
		float shootBegin = -1.0f;
		const float shootDelay = 0.7f;
		const float bulletSpeed = 1.0f;
		static PositionData playerPosData;
		std::vector<glm::vec3> motionPath;
		int motionPathCurrentIndex = 0; // the patrol checkpoint the enemy last reached
		PathTraversal pathTraversal = FORWARDS;
		bool playerSpotted = false;

		Enemy(const std::string &folder, const std::string &file,
		const std::string &bulletFolder, const std::string &bulletFile, std::string name,
		const PositionData &posData, std::vector<glm::vec3> motionPath);

		void Shoot(float currentTime, float deltaTime, Maze& M) override;
		void Move(float deltaTime, Maze& maze);
		void UpdateShaderMatrix(Camera &camera, int mWidth, int mHeight) override;
		bool PlayerSpotted(Mirage::Mesh& mesh);
		bool LineOfSightObstructed(const Triangle& triangle) const;
		bool IsDead() const;
};

#endif //ENEMY_HPP
