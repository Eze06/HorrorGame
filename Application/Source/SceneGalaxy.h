#ifndef SCENE_GALAXY_H
#define SCENE_GALAXY_H

#include "Scene.h"
#include "Mesh.h"
#include "AltAzCamera.h"
#include "MatrixStack.h"

class SceneGalaxy : public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_QUAD,
		GEO_CIRCLE,
		GEO_SPHERE_ORANGE,
		GEO_SPHERE_BLUE,
		GEO_SPHERE_GREY,
		GEO_TORUS,
		GEO_CUBE,
		GEO_HEMISPHERE,
		NUM_GEOMETRY,
	};

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_TOTAL,
	};

	enum ANIMATION
	{
		ANIM_MOON,
		ANIM_EARTH,
		ANIM_SUN,
		NUM_ANIM
	};
	// Keep track of current animation


	SceneGalaxy();
	~SceneGalaxy();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:

	ANIMATION currAnim;


	AltAzCamera camera;

	void HandleKeyPress();

	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	MatrixStack modelStack, viewStack, projectionStack;

	int projType = 1;

	float moonRotation = 0.0f; 
	float sunRotation = 0.0f;
	float earthRotation = 0.0f;




};

#endif