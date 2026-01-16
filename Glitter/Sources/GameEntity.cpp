//
// Created by Filip on 27.12.2025.
//

#include "GameEntity.hpp"
#include <utility>
#include "collision.hpp"

GameEntity::GameEntity(const std::string &folder, const std::string &file,
std::string bulletFolder, std::string bulletFile, std::string name):
Drawable(folder, file), bulletFolder(std::move(bulletFolder)), bulletFile(std::move(bulletFile)), name(std::move(name))
{}

GameEntity::GameEntity(std::vector<Vertex> const &vertices, std::vector<unsigned int> const &indices,
	std::string bulletFolder, std::string bulletFile, std::string name):
Drawable(vertices, indices), name(std::move(name)), bulletFolder(std::move(bulletFolder)), bulletFile(std::move(bulletFile))
{}

void GameEntity::CheckHits(std::vector<std::unique_ptr<Bullet>> &bullets, const Maze& maze)
{
	std::vector<int> bulletsToDelete;

	for (int i = 0; i < bullets.size(); i++)
	{
		if (isColliding(bullets[i]->posData.location, mesh, 0.01f))
		{
			health -= bullets[i]->damage;
			bulletsToDelete.push_back(i);
		}

		if (isColliding(bullets[i]->posData.location, maze.mesh, 0.01f))
		{
			bulletsToDelete.push_back(i);
		}
	}

	for (auto& bulletIndex: bulletsToDelete)
		bullets.erase(std::remove(bullets.begin(), bullets.end(), bullets.at(bulletIndex)), bullets.end());
}
