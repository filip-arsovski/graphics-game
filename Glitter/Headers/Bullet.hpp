//
// Created by Filip on 28.12.2025.
//

#ifndef BULLET_HPP
#define BULLET_HPP
#include "Drawable.hpp"

class Bullet : public Drawable
{
	public:
		PositionData posData;
		int damage;

		Bullet(const std::string &folder, const std::string &file,
			const PositionData &posData, int damage);

		void UpdateShaderMatrix(Camera &camera, int mWidth, int mHeight) override;
};

#endif //BULLET_HPP
