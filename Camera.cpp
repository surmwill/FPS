#pragma once

// Std. Includes
#include <vector>
#include "Camera.h"

// GL Includes
#include <GL\glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up , GLfloat yaw, GLfloat pitch ) 
: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(5000.0f), MouseSensitivity(0.25f), Zoom(45.0f)
{
	this->Position = position;
	this->WorldUp = up;
	this->Yaw = yaw;
	this->Pitch = pitch;
	this->updateCameraVectors();
}
// Constructor with scalar values
Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch)
: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(5000.0f), MouseSensitivity(0.25f), Zoom(45.0f)
{
	this->Position = glm::vec3(posX, posY, posZ);
	this->WorldUp = glm::vec3(upX, upY, upZ);
	this->Yaw = yaw;
	this->Pitch = pitch;
	this->updateCameraVectors();
}

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(this->Position, this->Position + this->Front, this->WorldUp);
}

// Processes input received from any keyboard-like input system. 
void Camera::ProcessFutureKeyboard(Camera_Movement direction, GLfloat deltaTime)
{

	//std::cout << this->prevPosition.x << std::endl;
	//this->FuturePosition.y -= this->gravity;
	
	GLfloat velocity = this->MovementSpeed * deltaTime;

	if (direction == FORWARD)
		this->FuturePosition += this->Front * velocity;
	if (direction == BACKWARD)
		this->FuturePosition -= this->Front * velocity;
	if (direction == LEFT)
		this->FuturePosition -= this->Right * velocity;
	if (direction == RIGHT)
		this->FuturePosition += this->Right * velocity;
}

// Processes input received from any keyboard-like input system. 
void Camera::ProcessKeyboard()
{

	//check if we are in a collision, if we are then restrict movment in that dimension
	if (hitX == 1) this->FuturePosition.x = this->Position.x;
	if (hitY == 1)
	{
		if (this->FuturePosition.y < this->Position.y) this->FuturePosition.y = this->Position.y;
	}
	if (hitZ == 1) this->FuturePosition.z = this->Position.z;

	this->Position = this->FuturePosition;


}

// Processes input received from a mouse input system.
void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{

	xoffset *= this->MouseSensitivity;
	yoffset *= this->MouseSensitivity;

	this->Yaw += xoffset;
	this->Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (this->Pitch > 89.0f)
			this->Pitch = 89.0f;
		if (this->Pitch < -89.0f)
			this->Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	this->updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event
void Camera::ProcessMouseScroll(GLfloat yoffset)
{
	if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
		this->Zoom -= yoffset;
	if (this->Zoom <= 1.0f)
		this->Zoom = 1.0f;
	if (this->Zoom >= 45.0f)
		this->Zoom = 45.0f;
}

// Calculates the front vector from the Camera's Eular Angles
void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	front.y = sin(glm::radians(this->Pitch));
	front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	this->Front = glm::normalize(front);

	// Also re-calculate the Right and Up vector
	this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	this->Up = glm::normalize(glm::cross(this->Front, this->Right));
}
