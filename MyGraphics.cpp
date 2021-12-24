#include "MyGraphics.h"

void MyGraphics::OnUserCreate()
{
	figures.resize(2);
	// ������
	figures[0].poligons =
	{
		// �����
		{ 0.0f, 0.0f, 0.0f,    0.0f, 2.0f, 0.0f,    1.0f, 2.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f,    1.0f, 2.0f, 0.0f,    1.0f, 0.0f, 0.0f },
		// �����                                                      
		{ 1.0f, 0.0f, 0.0f,    1.0f, 2.0f, 0.0f,    1.0f, 2.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f,    1.0f, 2.0f, 1.0f,    1.0f, 0.0f, 1.0f },
		// ����                                                      
		{ 0.0f, 0.0f, 0.0f,    0.0f, 2.0f, 0.0f,    1.0f, 2.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 1.0f,    1.0f, 2.0f, 1.0f },
		// �����                                                       
		{ 0.0f, 2.0f, 0.0f,    1.0f, 2.0f, 1.0f,    1.0f, 2.0f, 0.0f },
		// ���                                                    
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f }
	};

	// ��������
	figures[1].poligons =
	{
		// ���������
		{ 0.0f, 0.0f, 0.0f,    2.0f, 0.0f, 0.0f,    1.0f, 0.0f, 2.0f },
		// �����                                                     
		{ 0.0f, 0.0f, 0.0f,    1.0f, 2.0f, 1.0f,    2.0f, 0.0f, 0.0f },
		// �����                                                       
		{ 2.0f, 0.0f, 0.0f,    1.0f, 2.0f, 1.0f,    1.0f, 0.0f, 2.0f },
		// ����                                                    
		{ 1.0f, 0.0f, 2.0f,    1.0f, 2.0f, 1.0f,    0.0f, 0.0f, 0.0f }

	};

	projectMat = Matrix_MakeProjection(90.0f, static_cast<float>(GetConsoleHeight()) / static_cast<float>(GetConsoleWidth()), 0.1f, 1000.0f);

	light.x = 1.0f;
	light.y = -100.0f;
	light.z = 1.0f;

	scale = 1.0f;							
	_x = 0.5f; _y = 0.75f; _z = 4.0f;
	fThetaX = fThetaY = fThetaZ = 0.0f;
}

void MyGraphics::OnUserUpdate(float fElapsedTime)
{	
	Fill(0, 0, GetConsoleWidth(), GetConsoleHeight());

	// ����������� 
	Fill(0, iConsoleHeight / 2, iConsoleWidth, iConsoleHeight, PIXEL_SOLID, FG_BLACK);

	// �������� �� ����
	if (GetKey(L'1').bHeld)
		fThetaX += 8.0f * fElapsedTime;
	if (GetKey(L'2').bHeld)
		fThetaX -= 8.0f * fElapsedTime;
	if (GetKey(L'3').bHeld)
		fThetaY += 8.0f * fElapsedTime;
	if (GetKey(L'4').bHeld)
		fThetaY -= 8.0f * fElapsedTime;
	if (GetKey(L'5').bHeld)
		fThetaZ += 8.0f * fElapsedTime;
	if (GetKey(L'6').bHeld)
		fThetaZ -= 8.0f * fElapsedTime;

	// ��������� ��������
	if (GetKey(L'Z').bHeld)
		scale = (scale <= 1.2f) ? scale + 0.01f : scale;// ����������
	if (GetKey(L'X').bHeld)		
		scale = (scale >= 0.5f) ? scale - 0.01f : scale;// ����������

	// �����������
	if(GetKey(L'W').bHeld)
		_y -= 0.005f; // �����
	if (GetKey(L'A').bHeld)
		_x -= 0.005f; // �����
	if (GetKey(L'S').bHeld)
		_y += 0.005f; // ����
	if (GetKey(L'D').bHeld)
		_x += 0.005f; // ������
	if (GetKey(L'R').bHeld)
		_z = (_z < 7.0f) ? _z + 0.1f : _z;; // ������ �� z
	if (GetKey(L'F').bHeld)
		_z = (_z > 4.5f) ? _z - 0.1f : _z; // ����� �� z
	

	mat4x4 matRotX, matRotY, matRotZ;
	matRotX = Matrix_MakeRotationX(fThetaX * 0.5f);
	matRotY = Matrix_MakeRotationY(fThetaY * 0.5f);
	matRotZ = Matrix_MakeRotationZ(fThetaZ * 0.5f);
	mat4x4 ScalingMatrix;
	ScalingMatrix = Matrix_MakeScale(scale, scale, scale);
	mat4x4 TranslationMatrix;
	TranslationMatrix = Matrix_MakeTranslation(0.0f, 0.0f, _z);
	mat4x4 WorldMatrix;
	WorldMatrix = Matrix_MakeIdentity();
	WorldMatrix = matRotY * matRotX * matRotZ * ScalingMatrix * TranslationMatrix;

	std::vector<triangle> vecTrianglesToRaster;

	float  t = 0.0f;				
	int16_t count_tris = 0;
	for (auto& sh: figures) 
	{
		for (auto tri : sh.poligons)
		{
			triangle triProjected, triTransformed;

			for (int16_t i = 0; i < 3; i++)
			{
				triTransformed.vertex_list[i] = MultiplyMatrixVector(WorldMatrix, tri.vertex_list[i]);

				// �������������� �� 3D � 2D
				triProjected.vertex_list[i] = MultiplyMatrixVector(projectMat, triTransformed.vertex_list[i]);

				// ������� ������ �� Z ��� ������� �������
				triProjected.vertex_list[i] = triProjected.vertex_list[i] / triProjected.vertex_list[i].w;

				// ����������� X � Y
				triProjected.vertex_list[i].x *= -1.0f;
				triProjected.vertex_list[i].y *= -1.0f;
			}

			// ������ ��� ������ ����
			for (int16_t i = 0; i < 3; i++)
			{
				triProjected.vertex_list[i].x += _x + t;
				triProjected.vertex_list[i].y += _y;

				triProjected.vertex_list[i].x *= 0.5f * static_cast<float>(GetConsoleWidth());
				triProjected.vertex_list[i].y *= 0.5f * static_cast<float>(GetConsoleHeight());

				// ������ ������ ������
				figureCenter += triProjected.vertex_list[i];
			}
			count_tris++;
			
			vecTrianglesToRaster.push_back(triProjected);
		}

		// ��������� ������ ������
		figureCenter /= count_tris * 3;

		// ���������� ������������� ����� � ������
		std::sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](triangle& t1, triangle& t2)
			{
				float z1 = (t1.vertex_list[0].z + t1.vertex_list[1].z + t1.vertex_list[2].z) / 3.0f;
				float z2 = (t2.vertex_list[0].z + t2.vertex_list[1].z + t2.vertex_list[2].z) / 3.0f;
				return z1 > z2; // was >
			});

		// ���������� ���������
		for (auto& tri : vecTrianglesToRaster)
		{
			for (int16_t i = 0; i < 3; i++)
			{
				tri.vertex_list[i].x = roundf(tri.vertex_list[i].x);
				tri.vertex_list[i].y = roundf(tri.vertex_list[i].y);
			}
		}

		// ���������������� ���������
		DrawShadow(vecTrianglesToRaster, light);
		std::vector<triangle> vecVisibleSurfaces;
		fPoint3D view_point = { static_cast<float>(iConsoleWidth) / 2.0f, static_cast<float>(iConsoleHeight) / 2.0f, -100.0f };

		vecVisibleSurfaces = RobertsAlgorithm(vecTrianglesToRaster, view_point, figureCenter, PIXEL_SOLID, FG_BLUE);
		
		t += 1.0f;
		count_tris = 0;
		figureCenter = 0.0f;
		vecTrianglesToRaster.clear();
	}
}