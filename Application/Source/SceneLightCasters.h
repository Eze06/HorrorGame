#ifndef SCENE_LIGHT_CASTERS_H
#define SCENE_LIGHT_CASTERS_H

#include "Scene.h"
#include "Mesh.h"
#include "AltAzCamera.h"
#include "MatrixStack.h"
#include "Light.h"

class SceneLightCasters : public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_QUAD,
		GEO_CIRCLE,
		GEO_TORUS,
		GEO_CUBE,
		GEO_HEMISPHERE,
		GEO_SPHERE,
		NUM_GEOMETRY,
	};

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,

		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		U_NUMLIGHTS,
		U_LIGHTENABLED,
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



	SceneLightCasters();
	~SceneLightCasters();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:

	ANIMATION currAnim;

	static const int NUM_LIGHTS = 2;

	Light light[NUM_LIGHTS];
	bool enableLight;


	AltAzCamera camera;

	void HandleKeyPress();

	void RenderMesh(Mesh* mesh, bool enableLight);


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