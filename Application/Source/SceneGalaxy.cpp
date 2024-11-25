#include "SceneGalaxy.h"
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

SceneGalaxy::SceneGalaxy()
{
}

SceneGalaxy::~SceneGalaxy()
{
}

void SceneGalaxy::Init()
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

	// Load the shader programs
	m_programID = LoadShaders("Shader//TransformVertexShader.vertexshader",
								"Shader//SimpleFragmentShader.fragmentshader");
	glUseProgram(m_programID);

	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");

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
	meshList[GEO_SPHERE_ORANGE] = MeshBuilder::GenerateSphere("Sun", glm::vec3(0.9f, 0.3f, 0.f), 1.f, 12, 12);
	meshList[GEO_SPHERE_BLUE] = MeshBuilder::GenerateSphere("Earth", glm::vec3(0.4f, 0.2f, 0.8f), 1.f, 12, 12);
	meshList[GEO_SPHERE_GREY] = MeshBuilder::GenerateSphere("Moon", glm::vec3(0.5f, 0.5f, 0.5f), 1.f, 12, 12); 




	camera.Init(45.0f, 45.0f, 10.0f);

	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);


	currAnim = ANIM_SUN;
}

void SceneGalaxy::Update(double dt)
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


}

void SceneGalaxy::Render()
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



	glm::mat4 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, glm::value_ptr(MVP));

	// Render objects
	meshList[GEO_AXES]->Render();

	// Render objects	

	modelStack.LoadIdentity();
	modelStack.PushMatrix();
	//modelStack.Rotate(sunRotation, 0.f, 1.f, 0.f);

	modelStack.PushMatrix();
	modelStack.Translate(3.0f, 0.f, 0.f);
	modelStack.Rotate(earthRotation, 0.f, 1.f, 0.f);

	modelStack.PushMatrix();
	modelStack.Translate(1.5f, 0.f, 0.f);
	modelStack.Rotate(moonRotation, 0.f, 1.f, 0.f);
	modelStack.Scale(0.3f, 0.3f, 0.3f);

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, glm::value_ptr(MVP));

	meshList[GEO_SPHERE_GREY]->Render();
	modelStack.PopMatrix();

	
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, glm::value_ptr(MVP));

	meshList[GEO_SPHERE_BLUE]->Render();
	modelStack.PopMatrix();

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, glm::value_ptr(MVP));

	meshList[GEO_HEMISPHERE]->Render();
	modelStack.PopMatrix();

}

void SceneGalaxy::Exit()
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

void SceneGalaxy::HandleKeyPress() 
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



}
