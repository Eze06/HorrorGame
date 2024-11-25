#include "Scene1.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "KeyboardController.h"

//GLM HEADERS
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

Scene1::Scene1()
{
}

Scene1::~Scene1()
{
}

void Scene1::Init()
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

}

void Scene1::Update(double dt)
{
	// Check for key press, you can add more for interaction
	HandleKeyPress();

	if (rotating)
	{
		rotateAngle = rotateAngle + rotateSpeed * dt;
	}

	switch (moveInt)
	{
	case 1:
		moveZ = moveZ + moveSpeed * dt;
		break;

	case 2:
		moveX = moveX + moveSpeed * dt;
		break;

	case 3:
		moveZ = moveZ - moveSpeed * dt;
		break;

	case 4:
		moveX = moveX - moveSpeed * dt;
		break;

	default:
		moveX += 0.0f;
		moveY += 0.0f;
		moveZ += 0.0f;
		break;

	}
}

void Scene1::Render()
{
	// Clear color buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup Model View Projection matrix
	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 view = glm::lookAt(glm::vec3(4, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	glm::mat4 projection = glm::mat4(1.f);
	switch (projType) {
	case 0: // ORTHOGRAPHICS
		projection = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -100.f, 100.f);
		break;
	default: // PERSPECTIVE
		projection = glm::perspective(glm::radians(60.f), 4.f / 3.f, 0.1f, 1000.f);
		break;
	}


	glm::mat4 MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, glm::value_ptr(MVP));



	// Render objects
	meshList[GEO_AXES]->Render();



	glm::mat4 quadTrans, quadRotate, quadScale;
	glm::mat4 trans = glm::mat4(1.0f);

	quadTrans = glm::translate(trans, glm::vec3(moveX, moveY, moveZ));

	quadScale = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));

	quadRotate = glm::rotate(trans, glm::radians(rotateAngle), glm::vec3(rotX, rotY, rotZ));

	model = quadTrans * quadRotate * quadScale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, glm::value_ptr(MVP));
	

	meshList[GEO_QUAD]->Render();
}

void Scene1::Exit()
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

void Scene1::HandleKeyPress() 
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

	if (KeyboardController::GetInstance()->IsKeyPressed('X'))
	{
		rotX = 1.0f;
		rotY = 0.0f;
		rotZ = 0.0f;
		rotateAngle = 0.0f;
		rotating = true;
	}

	if (KeyboardController::GetInstance()->IsKeyPressed('Y'))
	{
		rotX = 0.0f;
		rotY = 1.0f;
		rotZ = 0.0f;
		rotateAngle = 0.0f;

		rotating = true;

	}

	if (KeyboardController::GetInstance()->IsKeyPressed('Z'))
	{
		rotX = 0.0f;
		rotY = 0.0f;
		rotZ = 1.0f;
		rotateAngle = 0.0f;

		rotating = true;

	}

	if (KeyboardController::GetInstance()->IsKeyDown('W'))
	{
		moveInt = 1;
	}

	else if (KeyboardController::GetInstance()->IsKeyDown('A'))
	{
		moveInt = 2;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown('S'))
	{
		moveInt = 3;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown('D'))
	{
		moveInt = 4;
	}
	else moveInt = 0;

}
