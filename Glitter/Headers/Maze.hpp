#ifndef MAZE_HPP
#define MAZE_HPP
#include "Camera.hpp"
#include "Drawable.hpp"
#include "Mesh.hpp"


class Maze : public Drawable
{
	public:
		glm::vec3 PlayerSpawnPoint = glm::vec3(0.587220, 0.298243, -0.598933);

		Maze(const std::string &folder, const std::string &file);

		void UpdateShaderMatrix(Camera &camera, int mWidth, int mHeight) override;
};




#endif //MAZE_HPP
