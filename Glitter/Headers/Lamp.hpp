//
// Created by Filip on 11.1.2026.
//

#ifndef LAMP_HPP
#define LAMP_HPP
#include "Drawable.hpp"

class Lamp : public Drawable
{
	public:
		glm::vec3 position = glm::vec3(4.561174f, 2.0f, -4.68146f);

		Lamp(const std::string& folder, const std::string& file): Drawable(folder, file){}

		void UpdateShaderMatrix(Camera &camera, int mWidth, int mHeight) override;
};


#endif //LAMP_HPP
