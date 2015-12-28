#pragma once
#include "Libraries.h"
#include <iostream>
#include <glm\gtx\transform.hpp>

using namespace glm;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 FuturePosition;
	glm::vec3 prevFuturePosition;

	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	GLfloat gravity = 5.0f;

	GLboolean stopFuture = false;
	// Eular Angles
	GLfloat Yaw;
	GLfloat Pitch;
	// Camera options
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;


	//flags 
	int hitX = 0;
	int hitY = 0;
	int hitZ = 0;


	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = -90.0f, GLfloat pitch = 0.0f);
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);
	void ProcessFutureKeyboard(Camera_Movement direction, GLfloat deltaTime);
	void ProcessKeyboard();
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(GLfloat yoffset);
	glm::mat4 GetViewMatrix();

private:
	void updateCameraVectors();
};

