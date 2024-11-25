#ifndef SCENE_2_H
#define SCENE_2_H

#include "Scene.h"
#include "Mesh.h"
#include "AltAzCamera.h"
#include "MatrixStack.h"

class Scene2 : public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_QUAD,
		GEO_CIRCLE,
		GEO_SPHERE,
		GEO_TORUS,
		NUM_GEOMETRY,
	};

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_TOTAL,
	};

	Scene2();
	~Scene2();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:

	AltAzCamera camera;

	void HandleKeyPress();

	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	

	int projType = 1;

	float rotX = 1.0f, rotY = 0.0f, rotZ = 0.0f;
	float scaleVal = 1.0f;
	float rotateAngle = 0.0f;
	float rotateSpeed = 10.0f;
	float moveX = 0.0f , moveY = 0.0f , moveZ = 0.0f;

	bool rotating = false;
	int moveInt = 0;
	float moveSpeed = 10.f;





};

#endif