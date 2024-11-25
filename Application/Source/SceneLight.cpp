#include "SceneLight.h"
#include "GL\glew.h"


#include "GLFW/glfw3.h"
#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "KeyboardController.h"

//GLM HEADERS
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_inverse.hpp>

SceneLight::SceneLight()
{
}

SceneLight::~SceneLight()
{
}

void SceneLight::Init()
{
	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	glEnable(GL_CULL_FACE);

	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//// Load the shader programs
	//m_programID = LoadShaders("Shader//TransformVertexShader.vertexshader",
	//							"Shader//SimpleFragmentShader.fragmentshader");

	// Load the shader programs
	m_programID = LoadShaders("Shader//Texture.vertexshader",
		"Shader//Texture.fragmentshader");

	glUseProgram(m_programID);

	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");


	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, glm::value_ptr(light[0].color));
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);



	// Init VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = nullptr;
	}

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("Axes", 10000.f, 10000.f, 10000.f);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("Quad", glm::vec3(1.0f), 1.0f);
	meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle("Circle", glm::vec3(1.f, 1.f, 1.f), 1.f, 12);
	meshList[GEO_TORUS] = MeshBuilder::GenerateTorus("Torus", glm::vec3(1.f, 1.f, 0.f), 1.f, 2.f, 12, 12);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("Cube", glm::vec3(1.f, 1.f, 1.f), 1.f);
	meshList[GEO_HEMISPHERE] = MeshBuilder::GenerateHemisphere("HemiSphere", glm::vec3(1.f, 0.f, 0.f), 12, 12, 1.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Planet", glm::vec3(1.f, 1.f, 1.f), 1.f, 16, 16);

	light[0].position = glm::vec3(0, 5, 0);
	light[0].color = glm::vec3(1, 1, 1);
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;



	camera.Init(45.0f, 45.0f, 10.0f);

	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);


	currAnim = ANIM_SUN;

	enableLight = true;


}

void SceneLight::Update(double dt)
{
	// Check for key press, you can add more for interaction
	HandleKeyPress();

	camera.Update(dt);
	
	switch (currAnim) {
	case ANIM_MOON:
		moonRotation += static_cast<float>(dt) * 20.f;
		break;
	case ANIM_EARTH:
		earthRotation += static_cast<float>(dt) * 10.f;
		break;
	case ANIM_SUN:
		sunRotation += static_cast<float>(dt) * 4.f;
		break;
	}

	if (KeyboardController::GetInstance()->IsKeyDown('I'))
		light[0].position.z -= static_cast<float>(dt) * 5.f;
	if (KeyboardController::GetInstance()->IsKeyDown('K'))
		light[0].position.z += static_cast<float>(dt) * 5.f;
	if (KeyboardController::GetInstance()->IsKeyDown('J'))
		light[0].position.x -= static_cast<float>(dt) * 5.f;
	if (KeyboardController::GetInstance()->IsKeyDown('L'))
		light[0].position.x += static_cast<float>(dt) * 5.f;
	if (KeyboardController::GetInstance()->IsKeyDown('O'))
		light[0].position.y -= static_cast<float>(dt) * 5.f;
	if (KeyboardController::GetInstance()->IsKeyDown('P'))
		light[0].position.y += static_cast<float>(dt) * 5.f;

}

void SceneLight::Render()
{
	// Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup Model View Projection matrix


	// Load view matrix stack and set it with camera position, target position and up direction
		viewStack.LoadIdentity();
		viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	// Load identity matrix into the model stack
	modelStack.LoadIdentity();



	//glm::mat4 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	//glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, glm::value_ptr(MVP));

	// Render objects

	modelStack.LoadIdentity();

	glm::vec3 lightPosition_cameraspace = viewStack.Top() * glm::vec4(light[0].position, 1);
	glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, glm::value_ptr(lightPosition_cameraspace));

	RenderMesh(meshList[GEO_AXES], false);


	modelStack.PushMatrix();
	// Render light
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	modelStack.Scale(0.1f, 0.1f, 0.1f);
	RenderMesh(meshList[GEO_SPHERE], false);
	modelStack.PopMatrix();


	modelStack.PushMatrix();
	modelStack.Rotate(sunRotation, 0.f, 1.f, 0.f);

	modelStack.PushMatrix();
	modelStack.Translate(3.0f, 0.f, 0.f);
	modelStack.Rotate(earthRotation, 0.f, 1.f, 0.f);

	modelStack.PushMatrix();
	modelStack.Translate(1.5f, 0.f, 0.f);
	modelStack.Rotate(moonRotation, 0.f, 1.f, 0.f);
	modelStack.Scale(0.3f, 0.3f, 0.3f);


	meshList[GEO_SPHERE]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	meshList[GEO_SPHERE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE]->material.kSpecular = glm::vec3(0.9f, 0.9f, 0.9f);
	meshList[GEO_SPHERE]->material.kShininess = 1.0f;
	RenderMesh(meshList[GEO_SPHERE], true);

	modelStack.PopMatrix();

	
	meshList[GEO_SPHERE]->material.kAmbient = glm::vec3(0.1f, 0.1f, 0.7f);
	meshList[GEO_SPHERE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE]->material.kSpecular = glm::vec3(0.9f, 0.9f, 0.9f);
	meshList[GEO_SPHERE]->material.kShininess = 5.0f;
	RenderMesh(meshList[GEO_SPHERE], true);

	modelStack.PopMatrix();

	meshList[GEO_SPHERE]->material.kAmbient = glm::vec3(0.7f, 0.1f, 0.1f);
	meshList[GEO_SPHERE]->material.kDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE]->material.kSpecular = glm::vec3(0.9f, 0.9f, 0.9f);
	meshList[GEO_SPHERE]->material.kShininess = 5.0f;
	RenderMesh(meshList[GEO_SPHERE], true);


	modelStack.PopMatrix();

}

void SceneLight::Exit()
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

void SceneLight::HandleKeyPress() 
{
	if (Application::IsKeyPressed(0x31))
	{
		// Key press to enable culling
		glEnable(GL_CULL_FACE);
	}
	if (Application::IsKeyPressed(0x32))
	{
		// Key press to disable culling
		glDisable(GL_CULL_FACE);
	}
	if (Application::IsKeyPressed(0x33))
	{
		// Key press to enable fill mode for the polygon
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	}
	if (Application::IsKeyPressed(0x34))
	{
		// Key press to enable wireframe mode for the polygon
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
	}

	if(KeyboardController::GetInstance()->IsKeyPressed('P'))
	{
		if (projType == 0)
			projType = 1; // Perspective
		else
			projType = 0; // Orthographic
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('E'))
	{
		// Only animate the earth
		currAnim = ANIM_EARTH;
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('M'))
	{
		// Only aniate the moon
		currAnim = ANIM_MOON;
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_ENTER))
	{
		// Only animate the sun
		currAnim = ANIM_SUN;
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('R'))
	{
		// Reset everything to original state
		currAnim = ANIM_SUN;
		moonRotation = 0.0f;
		earthRotation = 0.0f;
		sunRotation = 0.0f;
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_0))
	{
		// Toggle light on or off
		enableLight = !enableLight;
		if (enableLight)
		{
			light[0].power = 1.f;
		}
		else
		{
			light[0].power = 0.1f;
		}

		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);

	}


	



}

void SceneLight::RenderMesh(Mesh* mesh, bool enableLight)
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

		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE,
			glm::value_ptr(modelView_inverse_transpose));

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, glm::value_ptr(mesh->material.kAmbient));
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, glm::value_ptr(mesh->material.kDiffuse));
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, glm::value_ptr(mesh->material.kSpecular));
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	mesh->Render();
}