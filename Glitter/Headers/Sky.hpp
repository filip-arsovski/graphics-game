//
// Created by Filip on 13.1.2026.
//

#ifndef SKY_HPP
#define SKY_HPP
#include "Drawable.hpp"

class Sky : public Drawable
{
	public:
		Sky(const std::string& folder, const std::string& file): Drawable(folder, file) {}

		void UpdateShaderMatrix(Camera &camera, int mWidth, int mHeight) override;
};

#endif //SKY_HPP
