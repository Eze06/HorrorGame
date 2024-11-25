#include "AltAzCamera.h"
#include "KeyboardController.h"

//Include GLFW
#include <GLFW/glfw3.h>

AltAzCamera::AltAzCamera() : phi(0.0f), theta(0.0f), distance(0.0f), isDirty(false)
{
}

AltAzCamera::~AltAzCamera()
{
}

void AltAzCamera::Init(float theta, float phi, float distance)
{
	this->theta = theta;
	this->phi = phi;
	this->distance = distance;
	this->isDirty = true;

	Refresh();
}

void AltAzCamera::Reset()
{
}

void AltAzCamera::Update(double dt)
{
	static const float ROTATE_SPEED = 100.0f;
	static const float ZOOM_SPEED = 10.0f;

	if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_LEFT)) {
		theta -= ROTATE_SPEED * static_cast<float>(dt);
		isDirty = true;
	} 
	else if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_RIGHT)) {
		theta += ROTATE_SPEED * static_cast<float>(dt);
		isDirty = true;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_UP)) {
		phi += ROTATE_SPEED * static_cast<float>(dt);
		isDirty = true;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_DOWN)) {
		phi -= ROTATE_SPEED * static_cast<float>(dt);
		isDirty = true;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_W)) {
		distance -= ZOOM_SPEED * static_cast<float>(dt);
		isDirty = true;
	}
	else if (KeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_S)) {
		distance += ZOOM_SPEED * static_cast<float>(dt);
		isDirty = true;
	}

	this->Refresh();
}

void AltAzCamera::Refresh()
{
	if (!isDirty) return;

	// Calculate the position based on distance
	float x = this->distance * cosf(glm::radians(this->phi)) * cosf(glm::radians(this->theta));
	float y = this->distance * sinf(glm::radians(this->phi));
	float z = this->distance * cosf(glm::radians(this->phi)) * sinf(glm::radians(this->theta));
	this->position = glm::vec3(x, y, z);

	// Set default target to origin for this camera
	this->target = glm::vec3(0.f);
	glm::vec3 view = glm::normalize(target - position);

	// Find the right vector using default up (0,1,0) first
	glm::vec3 right = glm::normalize(glm::cross(view, glm::vec3(0.f, 1.f, 0.f)));

	// Recalculate the up vector
	this->up = glm::normalize(glm::cross(right, view));

	isDirty = false;
}