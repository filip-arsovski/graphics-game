//
// Created by Filip on 29.12.2025.
//

#ifndef GUNVIEW_HPP
#define GUNVIEW_HPP
#include "Drawable.hpp"

class GunView : public Drawable
{
	public:
		explicit GunView(const std::string &folder, const std::string &file) : Drawable(folder, file){}

		void UpdateShaderMatrix(Camera &camera, int mWidth, int mHeight) override;
};


#endif //GUNVIEW_HPP
