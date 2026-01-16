//
// Created by Filip on 27.12.2025.
//

#ifndef GAMEENTITY_HPP
#define GAMEENTITY_HPP

#include "Bullet.hpp"
#include "Drawable.hpp"
#include "Maze.hpp"
#include "Mesh.hpp"

class GameEntity : public Drawable
{
	public:
		std::string name;
		int health = 100;
		PositionData posData;
		std::string bulletFolder;
		std::string bulletFile;
		std::vector<std::unique_ptr<Bullet>> bullets;

		GameEntity(const std::string &folder, const std::string &file,
				std::string bulletFolder, std::string bulletFile, std::string name);
		GameEntity(std::vector<Vertex> const & vertices, std::vector<unsigned int> const & indices,
		std::string bulletFolder, std::string bulletFile, std::string name);

		~GameEntity() override = default;

		virtual void Shoot(float currentTime, float deltaTime, Maze& M) = 0;
		void CheckHits(std::vector<std::unique_ptr<Bullet>>& bullets, const Maze& maze);

};

#endif //GAMEENTITY_HPP
