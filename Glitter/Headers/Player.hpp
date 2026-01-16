//
// Created by Filip on 26.12.2025.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Camera.hpp"
#include "GameEntity.hpp"

class Player : public GameEntity
{
	public:
		Camera& playerCamera;

		Player(const std::string &folder, const std::string &file,
			const std::string& bulletFolder, const std::string &bulletFile, std::string name, Camera& camera);

		Player(std::vector<Vertex> const & vertices, std::vector<unsigned int> const & indices,
			const std::string &bulletFolder, const std::string &bulletFile, std::string name, Camera& camera);

		void Shoot(float currentTime, float deltaTime, Maze& M) override;
		void Move();
		void UpdateShaderMatrix(Camera &camera, int mWidth, int mHeight) override;
};

#endif //PLAYER_HPP
