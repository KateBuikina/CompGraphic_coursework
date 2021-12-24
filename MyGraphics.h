#ifndef _MY_GRAPHICS_H_
#define _MY_GRAPHICS_H_

#include "Graphics.h"

class MyGraphics : public Graphics
{

private:

	std::vector<mesh> figures; // Фигуры;

	float scale; // Для изменения размера
	float _x, _y, _z; // Для передвижения фигур
	float fThetaX, fThetaY, fThetaZ;
	mat4x4 projectMat; // Матрица проекции
	fPoint3D light;
	fPoint3D figureCenter;

	// Overrided methods
private:
	virtual void OnUserCreate() override;
	virtual void OnUserUpdate(float fElapsedTime) override;
};

#endif // !_MY_GRAPHICS_H_
