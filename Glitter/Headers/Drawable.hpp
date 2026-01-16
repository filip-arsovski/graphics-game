//
// Created by Filip on 27.12.2025.
//

#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP
#include "Camera.hpp"
#include "Mesh.hpp"

class Drawable
{
	public:
		Mirage::Mesh mesh;
		ShaderMatrix shaderMatrix;

		Drawable(const std::string &folder, const std::string &file);
		Drawable(std::vector<Vertex> const & vertices, std::vector<unsigned int> const & indices);

		virtual void UpdateShaderMatrix(::Camera &camera, int mWidth, int mHeight) = 0;
		void drawMesh(Shader &shader, glm::vec3 cameraPos, glm::vec3 lampPos);

		virtual ~Drawable() = default;
};

#endif //DRAWABLE_HPP
