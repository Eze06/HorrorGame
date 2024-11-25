#ifndef SCENE_CAMERA_H
#define SCENE_CAMERA_H
#define _USE_MATH_DEFINES

#include "Scene.h"
#include "Mesh.h"
#include "FPCamera.h"
#include "MatrixStack.h"
#include "Light.h"
#include "MouseController.h"
#include <iostream>
#include <math.h>



struct GameObject
{
	GameObject()
	{
		rotation = glm::vec3(0, 0, 1);
		colliderSize = glm::vec3(0.1);
		rotationAngle = 0;
		pos = glm::vec3(0);
		scale = glm::vec3(1);
		mesh = nullptr;
	};

	glm::vec3 pos;
	float rotationAngle;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 colliderSize;


	bool isCollidingBox(GameObject object2);
	bool isCollidingSphere(GameObject object2);

	Mesh* mesh;
};

class SceneCamera : public Scene
{
public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_SPHERE,
		GEO_PLANE,
		GEO_GUI,
		GEO_TREE,
		GEO_SKYBOX,
		GEO_WALL,
		GEO_PILLAR,
		GEO_COLUMN,
		GEO_FLOOR,
		GEO_FLOOR2,
		GEO_LIGHTBOX,
		GEO_TABLE,
		GEO_TV,
		GEO_JUMPSCARE,
		GEO_TORCH,
		GEO_FLASHLIGHT,
		GEO_BUTTON,
		GEO_FENCE,
		GEO_OPENFENCE,
		GEO_HINT,
		GEO_ZOMBIE,
		GEO_BOSS,
		GEO_ZOMBIEJUMPSCARE,
		GEO_YOUDIED,
		GEO_HITEFFECT,
		GEO_STATUE,
		GEO_ESCAPED,
		GEO_MENUBUTTONS,
		GEO_SKULLS,
		GEO_GUILLOTINE,
		GEO_CAGE,
		GEO_LAMP,

		GEO_TEXT,	
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

		U_LIGHT2_TYPE,
		U_LIGHT2_POSITION,
		U_LIGHT2_COLOR,
		U_LIGHT2_POWER,
		U_LIGHT2_KC,
		U_LIGHT2_KL,
		U_LIGHT2_KQ,
		U_LIGHT2_SPOTDIRECTION,
		U_LIGHT2_COSCUTOFF,
		U_LIGHT2_COSINNER,
		U_LIGHT2_EXPONENT,

		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_LIGHTENABLED,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

	SceneCamera();
	~SceneCamera();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:

	void HandleKeyPress();
	void HandleMouseInput();
	void RenderMesh(Mesh* mesh, bool lightEnabled);
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey);
	void RenderText(Mesh* mesh, std::string text, glm::vec3 color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, glm::vec3 color, float size, float x, float y);
	void playerMove(double dt);
	void RenderGameObj(GameObject gameObj, bool lightEnabled);
	void renderColliderBox(GameObject gameObj);

	float getTorchAngleX();
	float getTorchAngleY();
	float getBossRotation();
	
	void restart();

	static const int numWalls = 100; 
	static const int NUM_LIGHTS = 3;

	GameObject Fence;
	GameObject openFence;
	GameObject Walls[numWalls]; // USED TO CHECK FOR MOVEMENT COLLISION
	GameObject Pillar;
	GameObject Column;
	GameObject player;
	GameObject LightBox;
	GameObject Table;
	GameObject Tv;
	GameObject Torch;
	GameObject flashLight;
	GameObject buttons[12];
	GameObject Zombie;
	GameObject Boss;
	GameObject gate;
	GameObject statues[12];
	GameObject skulls[10];
	GameObject Guillotine;
	GameObject cage;
	GameObject Lamp;

	enum cameraType
	{
		PLAYERCAM,
		DOORCAM,
		MONSTERCAM,
		BOSSCAM,
		TOTALCAM
	};

	cameraType camType = PLAYERCAM;

	bool buttonClicked[12];
	int buttonsClicked = 0;

	float chaseTimer = 180;
	float changeTimer = 0; 
	float tempTimer = 0;
	float bossTimer = 0;
	float chargeTimer = 0;
	float flickerTimer = 0;
	float camTimer = 0;
	bool interacting = false;
	bool findElecText = false;
	bool findButtonsText = false;

	float flashLightRotate = 0;
	int bossMinsLeft = 0;
	int seconds = 0;

	glm::vec3 chargeDir = glm::vec3(0);

	bool startCharge = false;
	bool getChargeDir = false;
	bool zombieJumpscare = false;
	bool youDied = false;
	bool bossDied = false;
	bool shakeCamera = false;
	bool hitEffect = false;
	bool keyAquired = false;
	bool showLockedScreen = false;
	bool renderNothing = false;
	bool buttonsResetText = false;


	float hitTimer = 0;

	int monsterPosIndex = 0;

	glm::vec3 zombiePos[6];

	float lightFlickerTimer = 0;
	int lightFlickerCounter = 0;

	void InteractWithTV();
	void cameraShake(FPCamera camera, float intensity, float duration, double dt);

	glm::vec3 shakeOffset = glm::vec3(0);

	float timer = 0;

	bool showTvText = false;
	bool tvOn = false;
	bool movementEnabled = true;
	bool jumpScare = false;
	bool showHint = false;
	bool showLockedMonster = false;


	bool startChase = false;
	bool changingMonsterPosition = false;
	bool playBossScene = false;
	bool startBossFight = false;
	bool playerCanTakeDmg = false;
	bool bossCanTakeDmg = false;
	float shakeTimer = 1;

	bool reset = false;
	bool winGame = false;

	bool lightPicked = false;
	float torchXangle = 0;
	float torchYangle = 0;

	int playerHealth = 3;
	int bossHealth = 10;



	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	float fps = 0;
	int projType = 1; // fix to 0 for orthographic, 1 for projection


	FPCamera playerCamera;
	FPCamera doorCam;
	FPCamera monsterCam;
	FPCamera bossCam;



	MatrixStack modelStack, viewStack, projectionStack;

	Light light[NUM_LIGHTS];
	bool enableLight;


	glm::vec3 view;
	glm::vec3 right;
	glm::vec3 front;
};

#endif