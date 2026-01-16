//
// Created by Filip on 28.12.2025.
//

#include "Drawable.hpp"

#include "Bullet.hpp"
#include "Bullet.hpp"
#include "Bullet.hpp"
#include "Bullet.hpp"
#include "Bullet.hpp"
#include "Bullet.hpp"

Drawable::Drawable(const std::string &folder, const std::string &file): mesh(folder, file)
{
	shaderMatrix.model = glm::mat4(1.0f);
	shaderMatrix.view = glm::mat4(1.0f);
	shaderMatrix.projection = glm::mat4(1.0f);
}

Drawable::Drawable(std::vector<Vertex> const &vertices, std::vector<unsigned int> const &indices)
: mesh(vertices, indices)
{
	shaderMatrix.model = glm::mat4(1.0f);
	shaderMatrix.view = glm::mat4(1.0f);
	shaderMatrix.projection = glm::mat4(1.0f);
}

void Drawable::drawMesh(Shader &shader, glm::vec3 cameraPos, glm::vec3 lampPos)
{
	shader.use();

	shader.setVec3("lightPos", lampPos);
	shader.setVec3("cameraPos", cameraPos);
	shader.setMat4("model", shaderMatrix.model);
	shader.setMat4("view", shaderMatrix.view);
	shader.setMat4("projection", shaderMatrix.projection);

	mesh.draw(shader.ID);
}
