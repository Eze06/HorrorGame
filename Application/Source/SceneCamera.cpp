#include "SceneCamera.h"
#include "GL\glew.h"

// GLM Headers
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>

//Include GLFW
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "KeyboardController.h"
#include "LoadTGA.h"

#include <Windows.h>
#include <mmsystem.h>


SceneCamera::SceneCamera()
{
}

SceneCamera::~SceneCamera()
{
}

void SceneCamera::Init()
{
	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	glEnable(GL_CULL_FACE);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Load the shader programs
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
	glUseProgram(m_programID);

	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");


	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");


	m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
	m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
	m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
	m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
	m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
	m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
	m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
	m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");



	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");


	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");


	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT],
		m_parameters[U_MATERIAL_DIFFUSE],
		m_parameters[U_MATERIAL_SPECULAR],
		m_parameters[U_MATERIAL_SHININESS]);



	// Initialise playerCamera properties
	playerCamera.Init(glm::vec3(-3.f, 1.3f, 3.0f), glm::vec3(0.0f, 1.f, 1.f), glm::vec3(0,1,0));

	// Init VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = nullptr;
	}

	// 16 x 16 is the number of columns and rows for the text
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");


	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("Axes", 10000.f, 10000.f, 10000.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Sun", glm::vec3(1.f, 1.f, 1.f), 1.f, 16, 16);
	meshList[GEO_PLANE] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 10.f);
	meshList[GEO_SKYBOX] = MeshBuilder::GenerateQuad("Skybox", glm::vec3(1.f, 1.f, 1.f), 10.f);


	
	meshList[GEO_FLOOR] = MeshBuilder::GenerateCube("Arm", glm::vec3(0.5f, 0.5f, 0.5f), 1.f);
	meshList[GEO_SKYBOX]->textureID = LoadTGA("Image//skybox.tga");

	meshList[GEO_GUI] = MeshBuilder::GenerateQuad("Plane", glm::vec3(1.f, 1.f, 1.f), 10.f);

	meshList[GEO_JUMPSCARE] = MeshBuilder::GenerateQuad("JumpScare", glm::vec3(1.f, 1.f, 1.f), 10.f);
	meshList[GEO_JUMPSCARE]->textureID = LoadTGA("Image//jumpScare.tga");

	meshList[GEO_WALL] = MeshBuilder::GenerateOBJMTL("Wall", "OBJ//dungeonWall.obj", "OBJ//dungeonWall.mtl");
	meshList[GEO_WALL]->textureID = LoadTGA("Image//brickTex.tga");

	meshList[GEO_PILLAR] = MeshBuilder::GenerateOBJMTL("Pillar", "OBJ//dungeonPillar.obj", "OBJ//dungeonPillar.mtl");
	meshList[GEO_PILLAR]->textureID = LoadTGA("Image//brickTex.tga");

	meshList[GEO_COLUMN] = MeshBuilder::GenerateOBJMTL("Column", "OBJ//dungeonColumn.obj", "OBJ//dungeonColumn.mtl");
	meshList[GEO_COLUMN]->textureID = LoadTGA("Image//brickTex.tga");

	meshList[GEO_FENCE] = MeshBuilder::GenerateOBJMTL("Column", "OBJ//dungeonFence.obj", "OBJ//dungeonFence.mtl");
	meshList[GEO_FENCE]->textureID = LoadTGA("Image//brickTex.tga");

	meshList[GEO_OPENFENCE] = MeshBuilder::GenerateOBJMTL("Column", "OBJ//dungeonOpen.obj", "OBJ//dungeonOpen.mtl");
	meshList[GEO_OPENFENCE]->textureID = LoadTGA("Image//brickTex.tga");

	meshList[GEO_LIGHTBOX] = MeshBuilder::GenerateOBJMTL("LightBox", "OBJ//lightBox.obj", "OBJ//lightBox.mtl");
	meshList[GEO_LIGHTBOX]->textureID = LoadTGA("Image//lightBox.tga");

	meshList[GEO_TABLE] = MeshBuilder::GenerateOBJMTL("Table", "OBJ//table.obj", "OBJ//table.mtl");
	meshList[GEO_TABLE]->textureID = LoadTGA("Image//table.tga");

	meshList[GEO_TV] = MeshBuilder::GenerateOBJMTL("TV", "OBJ//tv.obj", "OBJ//tv.mtl");
	meshList[GEO_TV]->textureID = LoadTGA("Image//tvTex.tga");

	meshList[GEO_TORCH] = MeshBuilder::GenerateOBJMTL("Torch", "OBJ//wallTorch.obj", "OBJ//wallTorch.mtl");
	meshList[GEO_TORCH]->textureID = LoadTGA("Image//wallTorch.tga");

	meshList[GEO_FLASHLIGHT] = MeshBuilder::GenerateOBJMTL("FlashLight", "OBJ//torchLight.obj", "OBJ//torchLight.mtl");
	meshList[GEO_FLASHLIGHT]->textureID = LoadTGA("Image//flashLight.tga");

	meshList[GEO_BUTTON] = MeshBuilder::GenerateOBJMTL("Button", "OBJ//button.obj", "OBJ//button.mtl");
	meshList[GEO_BUTTON]->textureID = LoadTGA("Image//button.tga");

	meshList[GEO_HINT] = MeshBuilder::GenerateQuad("Hint", glm::vec3(1.f, 1.f, 1.f), 10.f);
	meshList[GEO_HINT]->textureID = LoadTGA("Image//Hint.tga");

	meshList[GEO_ZOMBIE] = MeshBuilder::GenerateOBJMTL("Zombie", "OBJ//Zombie.obj", "OBJ//Zombie.mtl");
	meshList[GEO_ZOMBIE]->textureID = LoadTGA("Image//Zombie.tga");

	meshList[GEO_BOSS] = MeshBuilder::GenerateOBJMTL("Boss", "OBJ//Boss.obj", "OBJ//Boss.mtl");
	meshList[GEO_BOSS]->textureID = LoadTGA("Image//Boss.tga");

	meshList[GEO_ZOMBIEJUMPSCARE] = MeshBuilder::GenerateQuad("zombieScare", glm::vec3(1.f, 1.f, 1.f), 10.f);
	meshList[GEO_ZOMBIEJUMPSCARE]->textureID = LoadTGA("Image//zombieScare.tga");

	meshList[GEO_YOUDIED] = MeshBuilder::GenerateQuad("youDied", glm::vec3(1.f, 1.f, 1.f), 10.f);
	meshList[GEO_YOUDIED]->textureID = LoadTGA("Image//youDied.tga");

	meshList[GEO_HITEFFECT] = MeshBuilder::GenerateQuad("hitEffect", glm::vec3(1.f, 1.f, 1.f), 10.f);
	meshList[GEO_HITEFFECT]->textureID = LoadTGA("Image//hitEffect.tga");

	meshList[GEO_STATUE] = MeshBuilder::GenerateOBJMTL("Boss", "OBJ//Statue.obj", "OBJ//Statue.mtl");
	meshList[GEO_STATUE]->textureID = LoadTGA("Image//statue.tga");

	meshList[GEO_ESCAPED] = MeshBuilder::GenerateQuad("escaped", glm::vec3(1.f, 1.f, 1.f), 10.f);
	meshList[GEO_ESCAPED]->textureID = LoadTGA("Image//escaped.tga");

	meshList[GEO_MENUBUTTONS] = MeshBuilder::GenerateQuad("menuButtons", glm::vec3(1.f, 1.f, 1.f), 10.f);
	meshList[GEO_MENUBUTTONS]->textureID = LoadTGA("Image//menuButtons.tga");

	meshList[GEO_GUILLOTINE] = MeshBuilder::GenerateOBJMTL("Boss", "OBJ//Guillotine.obj", "OBJ//Guillotine.mtl");
	meshList[GEO_GUILLOTINE]->textureID = LoadTGA("Image//brickTex.tga");

	meshList[GEO_CAGE] = MeshBuilder::GenerateOBJMTL("Boss", "OBJ//Cage.obj", "OBJ//Cage.mtl");
	meshList[GEO_CAGE]->textureID = LoadTGA("Image//Cage.tga");

	meshList[GEO_LAMP] = MeshBuilder::GenerateOBJMTL("Lamp", "OBJ//Lamp.obj", "OBJ//Lamp.mtl");
	meshList[GEO_LAMP]->textureID = LoadTGA("Image//lamp.tga");

	meshList[GEO_FLOOR2] = MeshBuilder::GenerateOBJMTL("Floor", "OBJ//wood.obj", "OBJ//wood.mtl");
	meshList[GEO_FLOOR2]->textureID = LoadTGA("Image//brickTex.tga");


	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);

	glUniform1i(m_parameters[U_NUMLIGHTS], NUM_LIGHTS);

	light[0].position = glm::vec3(-25, 2.5, 22.5);
	light[0].color = glm::vec3(1, 1, 1);
	light[0].type = Light::LIGHT_POINT;
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = 45.f;
	light[0].cosInner = 30.f;
	light[0].exponent = 3.f;
	light[0].spotDirection = glm::vec3(0.f, 1.f, 0.f);

	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, glm::value_ptr(light[0].color));
	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], cosf(glm::radians<float>(light[0].cosCutoff)));
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], cosf(glm::radians<float>(light[0].cosInner)));
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	light[1].position = glm::vec3(10, 10, 10);
	light[1].color = glm::vec3(250/255.f, 175 / 255.f, 40 / 255.f);
	light[1].type = Light::LIGHT_SPOT;
	light[1].power = 0;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = 45.f;
	light[1].cosInner = 30.f;
	light[1].exponent = 3.f;
	light[1].spotDirection = glm::vec3(0.f, 1.f, 0.f);


	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, glm::value_ptr(light[1].color));
	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], cosf(glm::radians<float>(light[1].cosCutoff)));
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], cosf(glm::radians<float>(light[1].cosInner)));
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);

	light[2].position = glm::vec3(-5, 5, 15);
	light[2].color = glm::vec3(250/255.f ,87/255.f, 39/255.f);
	light[2].type = Light::LIGHT_POINT;
	light[2].power = 1;
	light[2].kC = 1.f;
	light[2].kL = 0.01f;
	light[2].kQ = 0.001f;
	light[2].cosCutoff = 45.f;
	light[2].cosInner = 30.f;
	light[2].exponent = 3.f;
	light[2].spotDirection = glm::vec3(0.f, 1.f, 0.f);

	glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, glm::value_ptr(light[2].color));
	glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);
	glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
	glUniform1f(m_parameters[U_LIGHT2_KC], light[2].kC);
	glUniform1f(m_parameters[U_LIGHT2_KL], light[2].kL);
	glUniform1f(m_parameters[U_LIGHT2_KQ], light[2].kQ);
	glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], cosf(glm::radians<float>(light[2].cosCutoff)));
	glUniform1f(m_parameters[U_LIGHT2_COSINNER], cosf(glm::radians<float>(light[2].cosInner)));
	glUniform1f(m_parameters[U_LIGHT2_EXPONENT], light[2].exponent);

	enableLight = true;


	//Initialize Game Objects

	//LEFT WALLS
	for (int i = 0; i < 10; i++)
	{
		Walls[i].scale = glm::vec3(1.f);
		Walls[i].pos = glm::vec3(2, 0, 2 + (i * 3));
		Walls[i].colliderSize = glm::vec3(0.5, 1, 1.5);
		Walls[i].mesh = meshList[GEO_WALL];
	}
	
	//FRONT WALLS
	for (int i = 10; i < 25; i++)
	{
		Walls[i].scale = glm::vec3(1.f);
		Walls[i].pos = glm::vec3(2 - ((i - 10) * 3), 0, 30);
		Walls[i].colliderSize = glm::vec3(1.5, 1, 0.5);
		Walls[i].rotationAngle = 90;
		Walls[i].rotation = glm::vec3(0, 1, 0);
		Walls[i].mesh = meshList[GEO_WALL];
	}

	//RIGHT WALLS
	for (int i = 25; i < 35; i++)
	{
		Walls[i].scale = glm::vec3(1.f);
		Walls[i].pos = glm::vec3(-41, 0, 1.3 + ((i-25) * 3));
		Walls[i].colliderSize = glm::vec3(0.5, 1, 1.5);
		Walls[i].mesh = meshList[GEO_WALL];
	}

	//BACK WALLS
	for (int i = 35; i < 50; i++)
	{
		Walls[i].scale = glm::vec3(1.f);
		Walls[i].pos = glm::vec3(2 - ((i - 35) * 3), 0, 0);
		Walls[i].colliderSize = glm::vec3(1.5, 1, 0.5);
		Walls[i].rotationAngle = 90;
		Walls[i].rotation = glm::vec3(0, 1, 0);
		Walls[i].mesh = meshList[GEO_WALL];
	}

	for (int i = 50; i < 55; i++)
	{
		Walls[i].scale = glm::vec3(1.f);
		Walls[i].pos = glm::vec3(-15, 0, 2.3 + ((i-50) * 3));
		Walls[i].colliderSize = glm::vec3(0.5, 1, 1.5);
		Walls[i].mesh = meshList[GEO_WALL];
	}

	for (int i = 55; i < 65; i++)
	{
		Walls[i].scale = glm::vec3(1.f);
		Walls[i].pos = glm::vec3(-16.5 - ((i - 55) * 3), 0, 16);
		Walls[i].colliderSize = glm::vec3(1.5, 1, 0.5);
		Walls[i].rotationAngle = 90;
		Walls[i].rotation = glm::vec3(0, 1, 0);
		Walls[i].mesh = meshList[GEO_WALL];
	}

	
	Pillar.scale = glm::vec3(1.f);
	Pillar.pos = glm::vec3(-15, 0, 16);
	Pillar.colliderSize = glm::vec3(0.5);
	Pillar.mesh = meshList[GEO_PILLAR];

	Column.scale = glm::vec3(1.f);
	Column.pos = glm::vec3(-15, 0, 20);
	Column.colliderSize = glm::vec3(0.5);
	Column.mesh = meshList[GEO_COLUMN];


	
	Walls[65] = Pillar;

	for (int i = 66; i < 72; i++)
	{
		Walls[i] = Column;
		Walls[i].pos = glm::vec3(-15 - ((i-66) * 5), 0, 20);
	}

	for (int i = 72; i < 78; i++)
	{
		Walls[i] = Column;
		Walls[i].pos = glm::vec3(-15 - ((i - 72) * 5), 0, 25);
	}

	LightBox.scale = glm::vec3(0.005);
	LightBox.pos = glm::vec3(-5, 1, 29.2);
	LightBox.colliderSize = glm::vec3(2, 2, 2);
	LightBox.rotationAngle = 180.f;
	LightBox.rotation = glm::vec3(1, 0, 0);
	LightBox.mesh = meshList[GEO_LIGHTBOX];

	Table.scale = glm::vec3(4, 1, 2);
	Table.pos = glm::vec3(0.1, 0, 10);
	Table.rotationAngle = 90.f;
	Table.colliderSize = glm::vec3(1, 1, 4);
	Table.rotation = glm::vec3(0, 1, 0);
	Table.mesh = meshList[GEO_TABLE];

	Tv.scale = glm::vec3(0.8f);
	Tv.pos = glm::vec3(-0.3, 1.3, 10);
	Tv.colliderSize = glm::vec3(2, 2, 2);
	Tv.rotationAngle = -90.f;
	Tv.rotation = glm::vec3(0, 1, 0);
	Tv.mesh = meshList[GEO_TV];

	Walls[78] = Table;

	Torch.scale = glm::vec3(0.01);

	Guillotine.scale = glm::vec3(0.05);
	Guillotine.pos = glm::vec3(-0., 0.8, 27);
	Guillotine.rotationAngle = 180;
	Guillotine.rotation = glm::vec3(0, 1, 0);
	Guillotine.colliderSize = glm::vec3(2, 1, 2);
	Guillotine.mesh = meshList[GEO_GUILLOTINE];

	Walls[79] = Guillotine;

	Torch.pos = glm::vec3(-7, 1 , 0.3);
	Torch.mesh = meshList[GEO_TORCH];

	flashLight.scale = glm::vec3(0.001);
	flashLight.pos = glm::vec3(-0.7, 0.8, 13.5);
	flashLight.rotationAngle = 45;
	flashLight.rotation = glm::vec3(0, 1, 0);
	flashLight.colliderSize = glm::vec3(1.5);
	flashLight.mesh = meshList[GEO_FLASHLIGHT];

	for (int i = 0; i < 6; i++)
	{
		buttons[i].scale = glm::vec3(0.05f);
		buttons[i].pos = Walls[66 + i].pos;
		buttons[i].pos.x += .35f;
		buttons[i].pos.y += 0.5f;
		buttons[i].rotation = glm::vec3(0, 1, 0);
		buttons[i].rotationAngle = 90.f;
		buttons[i].colliderSize = glm::vec3(2.f);
		buttons[i].mesh = meshList[GEO_BUTTON];
		buttonClicked[i] = false;
	}
	

	for (int i = 6; i < 12; i++)
	{
		buttons[i].scale = glm::vec3(0.05f);
		buttons[i].pos = Walls[72 + i - 6].pos;
		buttons[i].pos.x += .35f;
		buttons[i].pos.y += 0.5f;
		buttons[i].rotation = glm::vec3(0, 1, 0);
		buttons[i].rotationAngle = 90.f;
		buttons[i].colliderSize = glm::vec3(2.f);
		buttons[i].mesh = meshList[GEO_BUTTON];
		buttonClicked[i] = false;
	}

	for (int i = 0; i < 6; i++)
	{
		statues[i].scale = glm::vec3(1.3f);
		statues[i].pos = glm::vec3(-18 - (i * 5), 0, 17);
		statues[i].rotation = glm::vec3(0, 1, 0);
		statues[i].rotationAngle = 0.f;
		statues[i].colliderSize = glm::vec3(2.f);
		statues[i].mesh = meshList[GEO_STATUE];
		buttonClicked[i] = false;
	}

	for (int i = 6; i < 12; i++)
	{
		statues[i].scale = glm::vec3(1.3f);
		statues[i].pos = glm::vec3(-18 - ((i-6) * 5), 0, 29);
		statues[i].rotation = glm::vec3(0, 1, 0);
		statues[i].rotationAngle = 180.f;
		statues[i].colliderSize = glm::vec3(2.f);
		statues[i].mesh = meshList[GEO_STATUE];
		buttonClicked[i] = false;
	}

	Fence.scale = glm::vec3(1);
	Fence.pos = Walls[52].pos;
	Fence.mesh = meshList[GEO_FENCE];
	Fence.colliderSize = glm::vec3(0.5, 1, 2.5);

	gate.scale = glm::vec3(1);
	gate.pos = Walls[27].pos;
	gate.mesh = meshList[GEO_FENCE];
	gate.colliderSize = glm::vec3(0.5, 1, 2.5);

	openFence.scale = glm::vec3(1);
	openFence.pos = Walls[52].pos;
	openFence.mesh = meshList[GEO_OPENFENCE];
	openFence.colliderSize = glm::vec3(0);

	Walls[52] = Fence;
	Walls[27] = gate;

	player.scale = glm::vec3(1.f);
	player.pos = glm::vec3(playerCamera.position);
	player.rotationAngle = 0.f;
	player.colliderSize = glm::vec3(0.3f);

	light[2].position = Torch.pos + glm::vec3(0, 0.5, 0.2);

	cage.mesh = meshList[GEO_CAGE];
	cage.pos = glm::vec3(0, 0, 18);
	cage.colliderSize = glm::vec3(1);
	Walls[80] = cage;

	doorCam.Init(glm::vec3(0, 2, 2), Walls[52].pos, glm::vec3(0, 1, 0));
	
	zombiePos[0] = glm::vec3(-39, 0, 22);
	zombiePos[1] = glm::vec3(-33, 0, 24);
	zombiePos[2] = glm::vec3(-28, 0, 20);
	zombiePos[3] = glm::vec3(-23, 0, 19);
	zombiePos[4] = glm::vec3(-13, 0, 19);

	Zombie.pos = zombiePos[0];
	Zombie.scale = glm::vec3(0.025);
	Zombie.rotationAngle = 90;
	Zombie.rotation = glm::vec3(0, 1, 0);
	Zombie.mesh = meshList[GEO_ZOMBIE];

	monsterCam.Init(glm::vec3(0, 2, 25), zombiePos[0], glm::vec3(0, 1, 0));

	Boss.pos = glm::vec3(-39, 0, 8);
	Boss.scale = glm::vec3(0.015);
	Boss.rotation = glm::vec3(0, 1, 0);
	Boss.rotationAngle = 90;
	Boss.colliderSize = glm::vec3(1);
	Boss.mesh = meshList[GEO_BOSS];

	Lamp.scale = glm::vec3(0.02);
	Lamp.pos = glm::vec3(-25, 2.5, 22.5);
	Lamp.mesh = meshList[GEO_LAMP];

	bossCam.Init(glm::vec3(-30, 2, 15), Boss.pos, glm::vec3(0, 1, 0));

	PlaySound(TEXT("BGM.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

void SceneCamera::Update(double dt)
{

	HandleKeyPress();
	HandleMouseInput();

	view = glm::normalize(playerCamera.target - playerCamera.position); // calculate the new view vector
	right = glm::normalize(glm::cross(view, playerCamera.up));
	front = glm::normalize(view - playerCamera.up * glm::dot(view, playerCamera.up));

	zombiePos[5] = player.pos;

	float temp = 1.f / dt;
	fps = glm::round(temp * 100.f) / 100.f;

	interacting = false;

	if (movementEnabled)
	{
		playerMove(dt);
	}

	player.pos = playerCamera.position;

	if (player.isCollidingBox(Tv))
	{
		interacting = true;
		if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_E))
		{
			InteractWithTV();
		}
	}

	if (player.isCollidingBox(LightBox) && !tvOn)
	{
		interacting = true;
		if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_E) && !tvOn)
		{
			light[0].power = 0;
			tvOn = true;
			shakeCamera = true;
			findElecText = false;
			glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);

		}
	}

	if (player.isCollidingBox(flashLight) && !lightPicked)
	{
		interacting = true;
		if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_E))
		{
			flashLight.rotationAngle = 0;
			lightPicked = true;
		}
	}

	if (bossDied)
	{
		if (Walls[27].mesh == gate.mesh)
		{
			GameObject gateTrigger = gate;
			gateTrigger.colliderSize = glm::vec3(2);
			if (player.isCollidingBox(gateTrigger))
			{
				interacting = true;
				if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_E))
				{

					if (keyAquired)
					{
						winGame = true;
					}
					else
					{
						showLockedScreen = true;;
					}
				}
			}
		}
		for (int i = 0; i < 12; i++)
		{

			if (player.isCollidingBox(statues[i]))
			{
				interacting = true;
				if(i == 3)
				{ 
					if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_E))
					{
						keyAquired = true;
					}
				}
				else
				{
					if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_E))
					{
						renderNothing = true;

					}
				}		
			}
		}
	}

	if (renderNothing)
	{
		timer += dt;
		if (timer >= 1)
		{
			timer = 0;
			renderNothing = false;
		}
	}

	if (Walls[52].mesh == Fence.mesh)
	{
		GameObject fenceTrigger = Fence;
		fenceTrigger.colliderSize = glm::vec3(2);
		if (player.isCollidingBox(fenceTrigger))
		{
			interacting = true;
			if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_E))
			{
				showLockedMonster = true;
			}
		}
	}
	
	if (showLockedScreen)
	{
		timer += dt;
		if (timer >= 1)
		{
			timer = 0;
			showLockedScreen = false;
		}
	}

	for (int i = 0; i < 12; i++)
	{
		if (player.isCollidingBox(buttons[i]) && buttonClicked[i] != true)
		{
			interacting = true;
			if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_E))
			{
				buttonClicked[i] = true;
				buttonsClicked++;
			}
		}
	}

	if (buttonsClicked >= 4)
	{
		if (buttonClicked[0] && buttonClicked[5] && buttonClicked[7] && buttonClicked[8])
		{
			camType = DOORCAM;
			timer += dt;
			movementEnabled = false;
			findButtonsText = false;
			tvOn = false;
			if (timer >= 1.f)
			{
				Walls[52] = openFence;
			}
			if (timer >= 2.f)
			{
				movementEnabled = true;
				camType = PLAYERCAM;
				buttonsClicked = 0;
			}
		}
		else
		{
			buttonsResetText = true;
		}
	}

	if (lightPicked)
	{
		torchYangle = getTorchAngleY();
		torchXangle = getTorchAngleX();

		light[1].position = player.pos + front;
		light[1].spotDirection = playerCamera.position - playerCamera.target;
	}
	else
	{
		flashLightRotate += dt * 200;
	}
	

	if (showHint && MouseController::GetInstance()->IsButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		showHint = false;
		movementEnabled = true;
		findButtonsText = true;
	}

	if (player.pos.x <= Walls[52].pos.x && player.pos.z <= Walls[54].pos.z && startChase == false)
	{
		playBossScene = true;
		camType = BOSSCAM;
	}

	if (playBossScene)
	{
		if (bossCam.position.z >= 1)
		{
			bossCam.position.z -= 5.f * dt;
		}
		else
		{
			timer += dt;
			if (timer >= 2)
			{
				playBossScene = false;
				startChase = true;
				changingMonsterPosition = true;
				timer = 0;
				startBossFight = true;
			}
		}
	}

	if (!bossDied)
	{
		if (player.pos.x >= Walls[52].pos.x || player.pos.z >= Walls[54].pos.z)
		{
			startBossFight = false;
		}
		if (player.pos.x <= Walls[52].pos.x && player.pos.z <= Walls[54].pos.z && startChase == true)
		{
			startBossFight = true;
		}

		if (startBossFight && !changingMonsterPosition)
		{
			Boss.rotationAngle = getBossRotation();

			if (bossTimer >= 2.f && !startCharge)
			{
				getChargeDir = true;
				startCharge = true;
				playerCanTakeDmg = true;
				bossCanTakeDmg = true;
			}
			else if (!startCharge)
			{
				bossTimer += dt;
				glm::vec3 playerDir = glm::normalize(player.pos - Boss.pos);
				Boss.pos.x = Boss.pos.x + playerDir.x * dt;
				Boss.pos.z = Boss.pos.z + playerDir.z * dt;
			}

			if (startCharge)
			{
				chargeTimer += dt;
				if (getChargeDir)
				{
					chargeDir = glm::normalize(player.pos - Boss.pos);
					getChargeDir = false;
				}

				if (chargeTimer >= 1)
				{
					Boss.pos.x += chargeDir.x * dt * 70;
					Boss.pos.z += chargeDir.z * dt * 70;
					Boss.colliderSize = glm::vec3(0);

					if (chargeTimer >= 1.05)
					{
						Boss.colliderSize = glm::vec3(1);
					}

					for (int i = 0; i < numWalls; i++)
					{
						if (Boss.isCollidingBox(player))
						{
							if (playerCanTakeDmg)
							{
								hitEffect = true;
								playerHealth--;
								bossCanTakeDmg = false;
								playerCanTakeDmg = false;
							}
						}

						if (Boss.isCollidingBox(Walls[i]))
						{
							if (bossCanTakeDmg)
							{
								shakeCamera = true;
								bossHealth--;
								bossCanTakeDmg = false;
							}
							startCharge = false;
							chargeTimer = 0;
							bossTimer = 0;
						}
					}
				}
			}
		}
	}

	if (bossHealth <= 0)
	{
		bossDied = true;
	}

	if (playerHealth <= 0)
	{
		youDied = true;
	}

	if (startChase)
	{
		bossMinsLeft = chaseTimer / 60;
		seconds = chaseTimer - (bossMinsLeft * 60);

		flickerTimer += dt;
		if (flickerTimer >= 1.3)
		{
			flickerTimer = 0.f;
		}

		if (player.isCollidingBox(Zombie))
		{
			zombieJumpscare = true;
		}

		if (tempTimer >= 180 / 4)
		{
			changingMonsterPosition = true;
		}

		if (!changingMonsterPosition)
		{
			tempTimer += dt;
			chaseTimer -= dt;
		}
		else
		{
			changeTimer += dt;
			camType = MONSTERCAM;
			movementEnabled = false;

			if (changeTimer >= 0.5)
			{
				light[0].power = 0;
				glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);

				light[1].power = 0;
				glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);

				light[2].power = 0;
				glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
			}

			if (changeTimer >= 1.5)
			{
				Zombie.pos = zombiePos[monsterPosIndex + 1];
				monsterCam.target = zombiePos[monsterPosIndex + 1];


				light[1].power = 1;
				glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);

				light[2].power = 2;
				glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);


			}

			if (changeTimer >= 3)
			{
				monsterPosIndex++;
				changeTimer = 0;
				tempTimer = 0;
				changingMonsterPosition = false;
				movementEnabled = true;
				camType = PLAYERCAM;
			}
		}
	}

	if (hitEffect)
	{
		hitTimer += dt;
		if (hitTimer >= 0.5f)
		{
			hitEffect = false;
			hitTimer = 0;
		}
	}

	if (showTvText)
	{
		timer += dt;
		if (timer >= 1.f)
		{
			timer = 0;
			showTvText = false;
			findElecText = true;
		}
	}

	if (showLockedMonster)
	{
		timer += dt;
		if (timer >= 1.f)
		{
			timer = 0;
			showLockedMonster = false;
		}
	}

	if (jumpScare)
	{
		interacting = false;
		timer += dt;
		if (timer >= 1.f)
		{
			timer = 0;
			jumpScare = false;
			showHint = true;
		}
	}

	if (buttonsResetText)
	{
		timer += dt;
		buttonsClicked = 0;
		for (int i = 0; i < 12; i++)
		{
			buttonClicked[i] = false;
		}

		if (timer >= 2.f)
		{
			buttonsResetText = false;
		}
	}

	if (zombieJumpscare)
	{
		timer += dt;
		if (timer >= 1.f)
		{
			timer = 0;
			zombieJumpscare = false;
			youDied = true;
		}
	}

	if (shakeCamera)
	{
		cameraShake(playerCamera, 0.1, 0.5, dt);
	}

	if (youDied)
	{
		Application::startPointer();
		movementEnabled = false;
		interacting = false;
		if (reset)
		{
			restart();
			Application::stopPointer();
		}
	}

	if (winGame)
	{
		Application::startPointer();
		movementEnabled = false;
		interacting = false;
		if (reset)
		{
			restart();
			Application::stopPointer();
		}
	}

	if (interacting)
	{
		if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_E))
		{
			interacting = false;
		}
	}
}

void SceneCamera::Render()
{
	// Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Load view matrix stack and set it with playerCamera position, target position and up direction
	viewStack.LoadIdentity();
	switch (camType)
	{
	case PLAYERCAM:
		viewStack.LookAt(
			playerCamera.position.x, playerCamera.position.y, playerCamera.position.z,
			playerCamera.target.x, playerCamera.target.y, playerCamera.target.z,
			playerCamera.up.x, playerCamera.up.y, playerCamera.up.z
		);
		break;

	case DOORCAM:
		viewStack.LookAt(
			doorCam.position.x, doorCam.position.y, doorCam.position.z,
			doorCam.target.x, doorCam.target.y, doorCam.target.z,
			doorCam.up.x, doorCam.up.y, doorCam.up.z
		);
		break;

	case MONSTERCAM:
		viewStack.LookAt(
			monsterCam.position.x, monsterCam.position.y, monsterCam.position.z,
			monsterCam.target.x, monsterCam.target.y, monsterCam.target.z,
			monsterCam.up.x, monsterCam.up.y, monsterCam.up.z
		);
		break;
	case BOSSCAM:
		viewStack.LookAt(
			bossCam.position.x, bossCam.position.y, bossCam.position.z,
			bossCam.target.x, bossCam.target.y, bossCam.target.z,
			bossCam.up.x, bossCam.up.y, bossCam.up.z
		);

	default:
		break;

	}



	// Load identity matrix into the model stack
	modelStack.LoadIdentity();


	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		glm::vec3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		glm::vec3 lightDirection_cameraspace = viewStack.Top() * glm::vec4(lightDir, 0);
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, glm::value_ptr(lightDirection_cameraspace));
	}
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[0].position, 1);
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
		glm::vec3 spotDirection_cameraspace = viewStack.Top() * glm::vec4(light[0].spotDirection, 0);
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, glm::value_ptr(spotDirection_cameraspace));
	}
	else
	{
		glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[0].position, 1);
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
	}



	if (light[1].type == Light::LIGHT_DIRECTIONAL)
	{
		glm::vec3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
		glm::vec3 lightDirection_cameraspace = viewStack.Top() * glm::vec4(lightDir, 0);
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, glm::value_ptr(lightDirection_cameraspace));
	}
	else if (light[1].type == Light::LIGHT_SPOT)
	{
		glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[1].position, 1);
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
		glm::vec3 spotDirection_cameraspace = viewStack.Top() * glm::vec4(light[1].spotDirection, 0);
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, glm::value_ptr(spotDirection_cameraspace));
	}
	else
	{
		glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[1].position, 1);
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
	}





	if (light[2].type == Light::LIGHT_DIRECTIONAL)
	{
		glm::vec3 lightDir(light[2].position.x, light[2].position.y, light[2].position.z);
		glm::vec3 lightDirection_cameraspace = viewStack.Top() * glm::vec4(lightDir, 2);
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, glm::value_ptr(lightDirection_cameraspace));
	}
	else if (light[2].type == Light::LIGHT_SPOT)
	{
		glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[2].position, 1);
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
		glm::vec3 spotDirection_cameraspace = viewStack.Top() * glm::vec4(light[2].spotDirection, 0);
		glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1, glm::value_ptr(spotDirection_cameraspace));
	}
	else
	{
		glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[2].position, 1);
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));
	}



	if (!tvOn)
	{
		modelStack.PushMatrix();
		// Render light
		modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
		modelStack.Scale(0.1f, 0.1f, 0.1f);
		RenderMesh(meshList[GEO_SPHERE], false);
		modelStack.PopMatrix();
	}



	modelStack.PushMatrix();
	// Render light
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	modelStack.Scale(0.1f, 0.1f, 0.1f);
	meshList[GEO_SPHERE]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_SPHERE]->material.kDiffuse = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_SPHERE]->material.kSpecular = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_SPHERE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_SPHERE], true);
	modelStack.PopMatrix();


	RenderGameObj(Lamp, true);

	for (int i = 0; i < numWalls; i++)
	{
		RenderGameObj(Walls[i], true);
	}

	RenderGameObj(LightBox, true);
	RenderGameObj(Tv, true);

	if (!bossDied)
	{
		RenderGameObj(Boss, true);
	}

	RenderGameObj(Torch, true);

	for (int i = 0; i < 12; i++)
	{
		if (!buttonClicked[i])
		{
			RenderGameObj(buttons[i], true);
		}
	}

	for (int i = 0; i < 12; i++)
	{
		RenderGameObj(statues[i], true);
	}

	if (lightPicked)
	{
		modelStack.PushMatrix();
		modelStack.Translate(player.pos.x, player.pos.y, player.pos.z);
		modelStack.Rotate(torchYangle, 0, 1, 0); //ROTATE Y
		modelStack.Rotate(torchXangle, 1, 0, 0); //ROTATE X

		modelStack.Translate(-0.35, -0.3, 0.7);

		modelStack.Scale(flashLight.scale.x, flashLight.scale.y, flashLight.scale.z);
		RenderMesh(meshList[GEO_FLASHLIGHT], false);
		modelStack.PopMatrix();
	}
	else
	{
		RenderGameObj(flashLight, true);

	}

	if (startChase)
	{
		RenderGameObj(Zombie, true);
	}


	modelStack.PushMatrix();
	modelStack.Translate(-45, 0, 5);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(50, 50, 50);
	RenderMesh(meshList[GEO_SKYBOX], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-19, -1.5, 15);
	modelStack.Scale(50, 3, 50);

	RenderMesh(meshList[GEO_FLOOR2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-19, 4.3, 15);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.Scale(20, 3, 20);

	RenderMesh(meshList[GEO_FLOOR2], true);
	modelStack.PopMatrix();

	if (hitEffect)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		RenderMeshOnScreen(meshList[GEO_HITEFFECT], 400, 300, 80, 70);

	}

	if (showLockedMonster)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "I have to get through there", glm::vec3(1, 1, 1), 20, 100, 300);
	}
	if (showTvText)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Seems like the TV needs power", glm::vec3(1, 1, 1), 20, 100, 300);
	}
	if (lightPicked)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "[F] On/Off Torch", glm::vec3(1, 1, 1), 20, 470, 0);
	}
	else
	{
		modelStack.PushMatrix();
		modelStack.Translate(flashLight.pos.x, 1.5, flashLight.pos.z);
		modelStack.Rotate(flashLightRotate, 0, 1, 0);
		modelStack.Translate(-0.5, 0, 0);
		modelStack.Scale(0.2, 0.2, 0.2);
		RenderText(meshList[GEO_TEXT], "Pick Up!", glm::vec3(0, 1, 0));
		modelStack.PopMatrix();
	}
	if (jumpScare)
	{
		RenderMeshOnScreen(meshList[GEO_JUMPSCARE], 400, 300, 80, 70);
	}

	if (showHint)
	{
		interacting = false;
		RenderMeshOnScreen(meshList[GEO_HINT], 400, 300, 80, 70);
	}

	if (zombieJumpscare)
	{
		RenderMeshOnScreen(meshList[GEO_ZOMBIEJUMPSCARE], 400, 300, 80, 70);
	}

	if (youDied)
	{
		RenderMeshOnScreen(meshList[GEO_YOUDIED], 400, 300, 80, 70);
	}

	if(keyAquired)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Key Aquired", glm::vec3(0, 1, 0), 20, 580, 20);
	}

	if (winGame)
	{
		RenderMeshOnScreen(meshList[GEO_ESCAPED], 400, 300, 80, 70);
	}

	if (findElecText)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Find the electricity box", glm::vec3(0, 1, 0), 20, 0, 550);
	}
	
	if (startChase && !changingMonsterPosition && !youDied && !winGame)
	{
		std::string timeTemp("Time Left:" + std::to_string(bossMinsLeft) + ":" + std::to_string(seconds));
		std::string playerTempHealth("Player Health:" + std::to_string(playerHealth));
		std::string bossTempHealth("Boss Health:" + std::to_string(bossHealth));

		if (!bossDied)
		{
			RenderTextOnScreen(meshList[GEO_TEXT], "The wall damages him!", glm::vec3(0, 1, 0), 20, 0, 515);
			RenderTextOnScreen(meshList[GEO_TEXT], playerTempHealth, glm::vec3(0, 1, 0), 20, 0, 10);
			RenderTextOnScreen(meshList[GEO_TEXT], bossTempHealth, glm::vec3(1, 0, 0), 20, 0, 60);
			if (flickerTimer >= 1)
			{
				RenderTextOnScreen(meshList[GEO_TEXT], "DEFEAT THE BOSS", glm::vec3(1, 0, 0), 20, 0, 530);
			}
		}		
		RenderTextOnScreen(meshList[GEO_TEXT], timeTemp, glm::vec3(1, 1, 1), 20, 0, 500);
	}	

	if (showLockedScreen)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "I need a key, lets check the statues", glm::vec3(1, 1, 1), 15, 100, 300);
	}

	if (renderNothing)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Nothing", glm::vec3(1, 1, 1), 15, 100, 300);
	}

	if (buttonsResetText)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Wrong combination", glm::vec3(1, 1, 1), 15, 300, 300);
	}

	if (interacting)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "[E] Interact", glm::vec3(1, 1, 1), 15, 300, 200);
	}

	if (tvOn && !findButtonsText && !jumpScare && !showHint)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Check the TV", glm::vec3(0, 1, 0), 20, 0, 550);
	}
	if (findButtonsText)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "Hint: Buttons", glm::vec3(0, 1, 0), 20, 0, 550);
	}

}

void SceneCamera::RenderGameObj(GameObject gameObj, bool lightEnabled)
{
	if (gameObj.mesh != nullptr)
	{
		modelStack.PushMatrix();
		modelStack.Translate(gameObj.pos.x, gameObj.pos.y, gameObj.pos.z);
		modelStack.Rotate(gameObj.rotationAngle, gameObj.rotation.x, gameObj.rotation.y, gameObj.rotation.z);
		modelStack.Scale(gameObj.scale.x, gameObj.scale.y, gameObj.scale.z);
		RenderMesh(gameObj.mesh, lightEnabled);
		modelStack.PopMatrix();
	}
	else return;
	return;
}

void SceneCamera::renderColliderBox(GameObject gameObj)
{
	modelStack.PushMatrix();
	modelStack.Translate(gameObj.pos.x, 1, gameObj.pos.z);
	modelStack.Scale(gameObj.colliderSize.x, gameObj.colliderSize.y, gameObj.colliderSize.z);
	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	RenderMesh(meshList[GEO_FLOOR], false);
	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	modelStack.PopMatrix();
}

float SceneCamera::getTorchAngleX()
{
	//TORCH ROTATE UP AND DOWN BASED ON THE FRONT AND VIEW VECTOR ALL FROM PLAYER


	float dotProd = glm::dot(view, front);
	float lengthMult = glm::length(front) * glm::length(view);

	float angle = (acos(dotProd / lengthMult) * (180 / M_PI));

	if (view.y >= front.y)
	{
		return -angle;
	}
	else
	{
		return angle;
	}
}

float SceneCamera::getTorchAngleY()
{
	//TORCH ROTATE LEFT AND RIGHT BASED ON THE FRONT AND X VECTOR
	glm::vec3 xVec = glm::vec3(1.0f, front.y, player.pos.z);
	float dotProd = glm::dot(front, xVec);
	float lengthMult = glm::length(front) * glm::length(xVec);

	float angle = (acos(dotProd / lengthMult) * (180 / M_PI));
	if (front.x <= 0)
	{
		return -angle;
	}
	return angle;
	
	
}

float SceneCamera::getBossRotation()
{
	glm::vec3 bossFront = glm::vec3(Boss.pos.x, 0,Boss.pos.z + 10) - Boss.pos;
	glm::vec3 playerFromBoss = player.pos - Boss.pos;

	bossFront.y = 0;
	playerFromBoss.y = 0;


	float dotProd = glm::dot(playerFromBoss, bossFront);
	float lengthMult = glm::length(playerFromBoss) * glm::length(bossFront);

	float angle = (acos(dotProd / lengthMult) * (180 / M_PI));
	if (player.pos.x <= Boss.pos.x)
	{
		return -angle;
	}
	return angle;
	

}

void SceneCamera::restart()
{
	 camType = PLAYERCAM;

	 buttonsClicked = 0;

	 chaseTimer = 180;
	 changeTimer = 0;
	 tempTimer = 0;
	 bossTimer = 0;
	 chargeTimer = 0;
	 flickerTimer = 0;
	 camTimer = 0;




	 bossMinsLeft = 0;
	 seconds = 0;


	 chargeDir = glm::vec3(0);

	 startCharge = false;
	 getChargeDir = false;
	 zombieJumpscare = false;
	 youDied = false;
	 bossDied = false;
	 shakeCamera = false;
	 hitEffect = false;
	 keyAquired = false;
	 reset = false;

	 hitTimer = 0;
	 findElecText = false;

	 monsterPosIndex = 0;

	 lightFlickerTimer = 0;
	 lightFlickerCounter = 0;


	shakeOffset = glm::vec3(0);

	 timer = 0;

	 showTvText = false;
	 tvOn = false;
	 movementEnabled = true;
	 jumpScare = false;
	 showHint = false;
	 showLockedMonster = false;
	 showLockedScreen = false;


	 startChase = false;
	 changingMonsterPosition = false;
	 playBossScene = false;
	 startBossFight = false;
	 playerCanTakeDmg = false;
	 bossCanTakeDmg = false;
	 shakeTimer = 1.5;
	 renderNothing = false;
	 buttonsResetText = false;
	 interacting = false;
	 flashLightRotate = 0;
	 findButtonsText = false;

	 winGame = false;

	 lightPicked = false;
	 torchXangle = 0;
	 torchYangle = 0;

	 playerHealth = 3;
	 bossHealth = 10;


	  fps = 0;
	  projType = 1; // fix to 0 for orthographic, 1 for projection


	  //Initialize Game Objects

	  playerCamera.Init(glm::vec3(-3.f, 1.3f, 3.0f), glm::vec3(0.0f, 1.f, 1.f), glm::vec3(0, 1, 0));

	//LEFT WALLS
	  for (int i = 0; i < 10; i++)
	  {
		  Walls[i].scale = glm::vec3(1.f);
		  Walls[i].pos = glm::vec3(2, 0, 2 + (i * 3));
		  Walls[i].colliderSize = glm::vec3(0.5, 1, 1.5);
		  Walls[i].mesh = meshList[GEO_WALL];
	  }

	  //FRONT WALLS
	  for (int i = 10; i < 25; i++)
	  {
		  Walls[i].scale = glm::vec3(1.f);
		  Walls[i].pos = glm::vec3(2 - ((i - 10) * 3), 0, 30);
		  Walls[i].colliderSize = glm::vec3(1.5, 1, 0.5);
		  Walls[i].rotationAngle = 90;
		  Walls[i].rotation = glm::vec3(0, 1, 0);
		  Walls[i].mesh = meshList[GEO_WALL];
	  }

	  //RIGHT WALLS
	  for (int i = 25; i < 35; i++)
	  {
		  Walls[i].scale = glm::vec3(1.f);
		  Walls[i].pos = glm::vec3(-41, 0, 1.3 + ((i - 25) * 3));
		  Walls[i].colliderSize = glm::vec3(0.5, 1, 1.5);
		  Walls[i].mesh = meshList[GEO_WALL];
	  }

	  //BACK WALLS
	  for (int i = 35; i < 50; i++)
	  {
		  Walls[i].scale = glm::vec3(1.f);
		  Walls[i].pos = glm::vec3(2 - ((i - 35) * 3), 0, 0);
		  Walls[i].colliderSize = glm::vec3(1.5, 1, 0.5);
		  Walls[i].rotationAngle = 90;
		  Walls[i].rotation = glm::vec3(0, 1, 0);
		  Walls[i].mesh = meshList[GEO_WALL];
	  }

	  for (int i = 50; i < 55; i++)
	  {
		  Walls[i].scale = glm::vec3(1.f);
		  Walls[i].pos = glm::vec3(-15, 0, 2.3 + ((i - 50) * 3));
		  Walls[i].colliderSize = glm::vec3(0.5, 1, 1.5);
		  Walls[i].mesh = meshList[GEO_WALL];
	  }

	  for (int i = 55; i < 65; i++)
	  {
		  Walls[i].scale = glm::vec3(1.f);
		  Walls[i].pos = glm::vec3(-16.5 - ((i - 55) * 3), 0, 16);
		  Walls[i].colliderSize = glm::vec3(1.5, 1, 0.5);
		  Walls[i].rotationAngle = 90;
		  Walls[i].rotation = glm::vec3(0, 1, 0);
		  Walls[i].mesh = meshList[GEO_WALL];
	  }


	  Pillar.scale = glm::vec3(1.f);
	  Pillar.pos = glm::vec3(-15, 0, 16);
	  Pillar.colliderSize = glm::vec3(0.5);
	  Pillar.mesh = meshList[GEO_PILLAR];

	  Column.scale = glm::vec3(1.f);
	  Column.pos = glm::vec3(-15, 0, 20);
	  Column.colliderSize = glm::vec3(0.5);
	  Column.mesh = meshList[GEO_COLUMN];

	  Guillotine.scale = glm::vec3(1);
	  Guillotine.pos = glm::vec3(-0.3, 1, 20);
	  Guillotine.colliderSize = glm::vec3(2, 1, 3);
	  Guillotine.mesh = meshList[GEO_GUILLOTINE];

	  Walls[65] = Pillar;

	  for (int i = 66; i < 72; i++)
	  {
		  Walls[i] = Column;
		  Walls[i].pos = glm::vec3(-15 - ((i - 66) * 5), 0, 20);
	  }

	  for (int i = 72; i < 78; i++)
	  {
		  Walls[i] = Column;
		  Walls[i].pos = glm::vec3(-15 - ((i - 72) * 5), 0, 25);
	  }

	  LightBox.scale = glm::vec3(0.005);
	  LightBox.pos = glm::vec3(-5, 1, 29.2);
	  LightBox.colliderSize = glm::vec3(2, 2, 2);
	  LightBox.rotationAngle = 180.f;
	  LightBox.rotation = glm::vec3(1, 0, 0);
	  LightBox.mesh = meshList[GEO_LIGHTBOX];


	  Table.scale = glm::vec3(4, 1, 2);
	  Table.pos = glm::vec3(0.1, 0, 10);
	  Table.rotationAngle = 90.f;
	  Table.colliderSize = glm::vec3(1, 1, 4);
	  Table.rotation = glm::vec3(0, 1, 0);
	  Table.mesh = meshList[GEO_TABLE];

	  Tv.scale = glm::vec3(0.8f);
	  Tv.pos = glm::vec3(-0.3, 1.3, 10);
	  Tv.colliderSize = glm::vec3(2, 2, 2);
	  Tv.rotationAngle = -90.f;
	  Tv.rotation = glm::vec3(0, 1, 0);
	  Tv.mesh = meshList[GEO_TV];

	  Walls[78] = Table;

	  Torch.scale = glm::vec3(0.01);




	  Torch.pos = glm::vec3(-7, 1, 0.3);
	  Torch.mesh = meshList[GEO_TORCH];

	  flashLight.scale = glm::vec3(0.001);
	  flashLight.pos = glm::vec3(-0.7, 0.8, 13.5);
	  flashLight.rotationAngle = 45;
	  flashLight.rotation = glm::vec3(0, 1, 0);
	  flashLight.colliderSize = glm::vec3(1.5);
	  flashLight.mesh = meshList[GEO_FLASHLIGHT];

	  for (int i = 0; i < 6; i++)
	  {
		  buttons[i].scale = glm::vec3(0.05f);
		  buttons[i].pos = Walls[66 + i].pos;
		  buttons[i].pos.x += .35f;
		  buttons[i].pos.y += 0.5f;
		  buttons[i].rotation = glm::vec3(0, 1, 0);
		  buttons[i].rotationAngle = 90.f;
		  buttons[i].colliderSize = glm::vec3(2.f);
		  buttons[i].mesh = meshList[GEO_BUTTON];
		  buttonClicked[i] = false;
	  }


	  for (int i = 6; i < 12; i++)
	  {
		  buttons[i].scale = glm::vec3(0.05f);
		  buttons[i].pos = Walls[72 + i - 6].pos;
		  buttons[i].pos.x += .35f;
		  buttons[i].pos.y += 0.5f;
		  buttons[i].rotation = glm::vec3(0, 1, 0);
		  buttons[i].rotationAngle = 90.f;
		  buttons[i].colliderSize = glm::vec3(2.f);
		  buttons[i].mesh = meshList[GEO_BUTTON];
		  buttonClicked[i] = false;
	  }

	  for (int i = 0; i < 6; i++)
	  {
		  statues[i].scale = glm::vec3(1.3f);
		  statues[i].pos = glm::vec3(-18 - (i * 5), 0, 17);
		  statues[i].rotation = glm::vec3(0, 1, 0);
		  statues[i].rotationAngle = 0.f;
		  statues[i].colliderSize = glm::vec3(1.f);
		  statues[i].mesh = meshList[GEO_STATUE];
		  buttonClicked[i] = false;
	  }

	  for (int i = 6; i < 12; i++)
	  {
		  statues[i].scale = glm::vec3(1.3f);
		  statues[i].pos = glm::vec3(-18 - ((i - 6) * 5), 0, 29);
		  statues[i].rotation = glm::vec3(0, 1, 0);
		  statues[i].rotationAngle = 180.f;
		  statues[i].colliderSize = glm::vec3(1.f);
		  statues[i].mesh = meshList[GEO_STATUE];
		  buttonClicked[i] = false;
	  }


	  Fence.scale = glm::vec3(1);
	  Fence.pos = Walls[52].pos;
	  Fence.mesh = meshList[GEO_FENCE];
	  Fence.colliderSize = glm::vec3(0.5, 1, 2.5);

	  gate.scale = glm::vec3(1);
	  gate.pos = Walls[27].pos;
	  gate.mesh = meshList[GEO_FENCE];
	  gate.colliderSize = glm::vec3(0.5, 1, 2.5);

	  openFence.scale = glm::vec3(1);
	  openFence.pos = Walls[52].pos;
	  openFence.mesh = meshList[GEO_OPENFENCE];
	  openFence.colliderSize = glm::vec3(0);

	  Walls[52] = Fence;
	  Walls[27] = gate;

	  player.scale = glm::vec3(1.f);
	  player.pos = glm::vec3(playerCamera.position);
	  player.rotationAngle = 0.f;
	  player.colliderSize = glm::vec3(0.3f);

	  light[2].position = Torch.pos + glm::vec3(0, 0.5, 0.2);



	  doorCam.Init(glm::vec3(0, 2, 2), Walls[52].pos, glm::vec3(0, 1, 0));

	  zombiePos[0] = glm::vec3(-39, 0, 22);
	  zombiePos[1] = glm::vec3(-33, 0, 24);
	  zombiePos[2] = glm::vec3(-28, 0, 20);
	  zombiePos[3] = glm::vec3(-23, 0, 19);
	  zombiePos[4] = glm::vec3(-13, 0, 19);

	  Zombie.pos = zombiePos[0];
	  Zombie.scale = glm::vec3(0.025);
	  Zombie.rotationAngle = 90;
	  Zombie.rotation = glm::vec3(0, 1, 0);
	  Zombie.mesh = meshList[GEO_ZOMBIE];

	  monsterCam.Init(glm::vec3(0, 2, 25), zombiePos[0], glm::vec3(0, 1, 0));

	  Boss.pos = glm::vec3(-39, 0, 8);
	  Boss.scale = glm::vec3(0.015);
	  Boss.rotation = glm::vec3(0, 1, 0);
	  Boss.rotationAngle = 90;
	  Boss.colliderSize = glm::vec3(1);
	  Boss.mesh = meshList[GEO_BOSS];

	  bossCam.Init(glm::vec3(-30, 2, 15), Boss.pos, glm::vec3(0, 1, 0));

	  PlaySound(TEXT("BGM.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

}

void SceneCamera::InteractWithTV()
{
	if (tvOn)
	{
		movementEnabled = false;
		jumpScare = true;
	}
	else
	{
		showTvText = true;
	}

}

void SceneCamera::cameraShake(FPCamera camera, float intensity, float duration, double dt)
{
	shakeTimer -= dt;
	if (shakeTimer <= 0)
	{
		shakeCamera = false;
		shakeTimer = 1;
		playerCamera.position.y = 1.3f;
		playerCamera.target.y = 1.3f;
	}
	else
	{
		float SHAKE_INTENSITY = intensity;    // Adjust the intensity of the shake

		// Generate random displacements for camera shake
		float shakeOffsetX = SHAKE_INTENSITY * (static_cast<float>(rand()) / RAND_MAX - 0.5f);
		float shakeOffsetY = SHAKE_INTENSITY * (static_cast<float>(rand()) / RAND_MAX - 0.5f);

		// Apply camera shake to the player's position
		playerCamera.position.x += shakeOffsetX;
		playerCamera.position.y += shakeOffsetY;

		// Apply camera shake to the player's target
		playerCamera.target.x += shakeOffsetX;
		playerCamera.target.y += shakeOffsetY;
	}

	



}








void SceneCamera::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
		{
			delete meshList[i];
		}
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void SceneCamera::HandleKeyPress()
{
	if (KeyboardController::GetInstance()->IsKeyPressed(0x31))
	{
		// Key press to enable culling
		glEnable(GL_CULL_FACE);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(0x32))
	{
		// Key press to disable culling
		glDisable(GL_CULL_FACE);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(0x33))
	{
		// Key press to enable fill mode for the polygon
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(0x34))
	{
		// Key press to enable wireframe mode for the polygon
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_SPACE))
	{
		// Change to black background
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_0))
	{
		// Toggle light on or off
	/*	enableLight = !enableLight;*/

		if (light[0].power <= 0.1f)
			light[0].power = 1.f;
		else
			light[0].power = 0.1f;
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_TAB))
	{
		if (light[0].type == Light::LIGHT_POINT) {
			light[0].type = Light::LIGHT_DIRECTIONAL;
		}
		else if (light[0].type == Light::LIGHT_DIRECTIONAL) {
			light[0].type = Light::LIGHT_SPOT;
		}
		else {
			light[0].type = Light::LIGHT_POINT;
		}

		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	} 


	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_Z))
	{
		
		Application::startPointer();
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_X))
	{

		Application::stopPointer();
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_LEFT_ALT))
	{
		player.colliderSize = glm::vec3(0);
	}
	if (lightPicked && KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_F))
	{
		if (light[1].power <= 0)
		{
			light[1].power = 1.f;
		}
		else
		{
			light[1].power = 0.f;
		}

		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);

	}

	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_Q))
	{
		std::cout << player.pos.x << ", " << player.pos.y << ", " << player.pos.z << std::endl;
	}
}


void SceneCamera::HandleMouseInput() {

	static bool isLeftUp = false;
	static bool isRightUp = false;

	// Process Left button
	if (!isLeftUp && MouseController::GetInstance()->IsButtonDown(GLFW_MOUSE_BUTTON_LEFT))
	{
		isLeftUp = true;
		std::cout << "LBUTTON DOWN" << std::endl;

		double x = MouseController::GetInstance()->GetMousePositionX();
		double y = 600 - MouseController::GetInstance()->GetMousePositionY();

		// Check if mouse click position is within the GUI box
		// Change the boundaries as necessary
		if (x > 0 && x < 100 && y > 0 && y < 100) {
			std::cout << "GUI IS CLICKED" << std::endl;
		}


		std::cout << x << "," << y << std::endl;

		if (youDied || winGame)
		{
			if (x > 348 && x < 428 && y > 181 && y < 212)
			{

				Application::quit();
			}

			if (x > 299 && x < 479 && y > 226 && y < 258)
			{
				reset = true;
			}
		}

	}
	else if (isLeftUp && MouseController::GetInstance()->IsButtonUp(GLFW_MOUSE_BUTTON_LEFT))
	{
		isLeftUp = false;
		std::cout << "LBUTTON UP" << std::endl;
	}

	// Continue to do for right button
	// Process Left button
	if (!isRightUp && MouseController::GetInstance()->IsButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
	{
		isRightUp = true;
		std::cout << "RBUTTON DOWN" << std::endl;
	}
	else if (isRightUp && MouseController::GetInstance()->IsButtonUp(GLFW_MOUSE_BUTTON_RIGHT))
	{
	
		isRightUp = false;
		std::cout << "RBUTTON UP" << std::endl;
	}

	if (MouseController::GetInstance()->IsButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
		std::cout << "LMU PRESSED" << std::endl;
	}

}


bool GameObject::isCollidingBox(GameObject object2)
{

	//CHECK X AND Z AS OUR GAME DOESNT IMPLEMENT JUMPING (DONT NEED TO CHECK FOR Y AXIS COLLISION)
	float aMaxX, aMinX, aMaxZ, aMinZ;
	float bMaxX, bMinX, bMaxZ, bMinZ;

	aMaxX = pos.x + colliderSize.x;
	aMinX = pos.x - colliderSize.x;

	aMaxZ = pos.z + colliderSize.z;
	aMinZ = pos.z - colliderSize.z;

	bMaxX = object2.pos.x + object2.colliderSize.x;
	bMinX = object2.pos.x - object2.colliderSize.x;

	bMaxZ = object2.pos.z + object2.colliderSize.z;
	bMinZ = object2.pos.z - object2.colliderSize.z;


	if (aMaxX <= bMinX || aMinX >= bMaxX || aMaxZ <= bMinZ || aMinZ >= bMaxZ)
	{
		return false;
	}
	else
		return true;

}

bool GameObject::isCollidingSphere(GameObject object2)
{
	return false;
}

void SceneCamera::RenderMesh(Mesh* mesh, bool enableLight)
{
	glm::mat4 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, glm::value_ptr(MVP));
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, glm::value_ptr(modelView));
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = glm::inverseTranspose(modelView);
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, glm::value_ptr(modelView_inverse_transpose));

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);

	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}

void SceneCamera::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey)
{
	glDisable(GL_DEPTH_TEST);
	glm::mat4 ortho = glm::ortho(0.f, 800.f, 0.f, 600.f, -1000.f, 1000.f); // dimension of screen

	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need playerCamera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0.f);
	modelStack.Scale(sizex, sizey, 1.f);
	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneCamera::RenderText(Mesh* mesh, std::string text, glm::vec3 color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Disable back face culling
	glDisable(GL_CULL_FACE);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		glm::mat4 characterSpacing = glm::translate(
			glm::mat4(1.f),
			glm::vec3(i * 1.0f, 0, 0));
		glm::mat4 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, glm::value_ptr(MVP));
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);

}

void SceneCamera::RenderTextOnScreen(Mesh* mesh, std::string text, glm::vec3 color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glm::mat4 ortho = glm::ortho(0.f, 800.f, 0.f, 600.f, -100.f, 100.f); // dimension of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need playerCamera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	for (unsigned i = 0; i < text.length(); ++i)
	{
		glm::mat4 characterSpacing = glm::translate(
			glm::mat4(1.f),
			glm::vec3(0.5f + i * 1.0f, 0.5f, 0)
		);
		glm::mat4 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, glm::value_ptr(MVP));
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);


}

void SceneCamera::playerMove(double dt)
{
	static const float ROTATE_SPEED = 5.f;
	static const float MOVE_SPEED = 5.f;
	static const float ZOOM_SPEED = 10.0f;

	static const float BOB_AMPLITUDE = 0.005f;  // Adjust the amplitude of the bobbing
	static const float BOB_FREQUENCY = 5.0f;   // Adjust the frequency of the bobbing

	float bobOffset = BOB_AMPLITUDE * sin(static_cast<float>(glfwGetTime()) * BOB_FREQUENCY);

	// Apply bobbing to the player's vertical position
	playerCamera.position.y += bobOffset;
	playerCamera.target.y += bobOffset;


	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_W))
	{
		GameObject playerFront = player;
		playerFront.pos = player.pos + front;


		bool colliding = false;
		for (int i = 0; i < numWalls; i++)
		{
			if (playerFront.isCollidingBox(Walls[i]))
			{
				colliding = true;
				break;
			}
			continue;
		}

		if (!colliding)
		{
			playerCamera.position += front * MOVE_SPEED * static_cast<float>(dt);
			playerCamera.target += front * MOVE_SPEED * static_cast<float>(dt);

			playerCamera.isDirty = true;
		}



	}
	else if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_S))
	{
		GameObject playerBack = player;
		playerBack.pos = player.pos - front;


		bool colliding = false;

		for (int i = 0; i < numWalls; i++)
		{
			if (playerBack.isCollidingBox(Walls[i]))
			{
				colliding = true;
				break;
			}
			continue;
		}
		// To do for opposite direction
		if (!colliding)
		{
			playerCamera.position -= front * MOVE_SPEED * static_cast<float>(dt);
			playerCamera.target -= front * MOVE_SPEED * static_cast<float>(dt);

			playerCamera.isDirty = true;
		}

	}

	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_D)) {


		GameObject playerRight = player;
		playerRight.pos = player.pos + right;
		bool colliding = false;

		for (int i = 0; i < numWalls; i++)
		{
			if (playerRight.isCollidingBox(Walls[i]))
			{
				colliding = true;
				break;
			}
			continue;
		}

		if (!colliding)
		{
			playerCamera.position += right * MOVE_SPEED * static_cast<float>(dt); // Use right vector
			playerCamera.target += right * MOVE_SPEED * static_cast<float>(dt); // Use right vector

			playerCamera.isDirty = true;
		}

	}
	else if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_A)) {

		GameObject playerLeft = player;
		playerLeft.pos = player.pos - right;

		bool colliding = false;
		for (int i = 0; i < numWalls; i++)
		{
			if (playerLeft.isCollidingBox(Walls[i]))
			{
				colliding = true;
				break;
			}
			continue;
		}

		if (!colliding)
		{
			// To do: for opposite direction
			playerCamera.position -= right * MOVE_SPEED * static_cast<float>(dt); // Use right vector
			playerCamera.target -= right * MOVE_SPEED * static_cast<float>(dt); // Use right vector

			playerCamera.isDirty = true;
		}

	}

	double deltaX = MouseController::GetInstance()->GetMouseDeltaX();

	float angle = -deltaX * ROTATE_SPEED * static_cast<float>(dt);
	glm::mat4 yaw = glm::rotate(
		glm::mat4(1.f),// matrix to modify
		glm::radians(angle),// rotation angle in degree and converted to radians
		glm::vec3(playerCamera.up.x, playerCamera.up.y, playerCamera.up.z)// the axis to rotate along
	);

	glm::vec3 yawView = yaw * glm::vec4(view, 0.f);

	playerCamera.isDirty = true;

	double deltaY = MouseController::GetInstance()->GetMouseDeltaY();

	float udAngle = deltaY * ROTATE_SPEED * static_cast<float>(dt);
	glm::mat4 udYaw = glm::rotate(
		glm::mat4(1.f),
		glm::radians(udAngle),
		glm::vec3(right.x, right.y, right.z)
	);

	glm::vec3 udYawView = udYaw * glm::vec4(view, 0.f);
	playerCamera.target = playerCamera.position + udYawView + yawView;

	playerCamera.isDirty = true;





	playerCamera.Refresh();
}
