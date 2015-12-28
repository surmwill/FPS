#pragma once
#include "Libraries.h"
#include "Camera.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class Collision
{
public:
	//creates the intial bounding box
	Collision(GLfloat boundingMaxY, GLfloat boundingMinY, GLfloat boundingMaxX, GLfloat boundingMinX, GLfloat boundingMaxZ, GLfloat boundingMinZ, Camera * camera, glm::vec3 translate);
	
	//creates a custom bounding box
	void CreateCustomBoundingBox(Camera * camera, char key);

	//resets all the flags
	void Collision::ResetFlags();

	int h = 0;
	void WriteToFile();
	int closeFile = 0;
	ofstream myfile;
	string line;


private:

	//different functions
	void ProcessCollisions(Camera * camera);
	void TestCustomBoundingBox(Camera * camera);
	void CreateGroundBoundingBox(Camera * camera);

	//our bounding box dimensions
	GLfloat boundingMaxY;
	GLfloat boundingMinY;
	GLfloat boundingMaxX;
	GLfloat boundingMinX;
	GLfloat boundingMaxZ;
	GLfloat boundingMinZ;

	//all of our flags
	bool hitX = false;
	bool hitY = false;
	bool hitZ = false;

	bool prevHitX = false;
	bool prevHitY = false;
	bool prevHitZ = false;

	bool inCollision = false;
	bool prevCollision = false;

	bool unlockFlagX = false;
	bool unlockFlagY = false;
	bool unlockFlagZ = false;


	glm::vec3 translate;

	//other flags
	int flag2 = 0;
	int lastHit = 0;
	bool flag = false;

	GLfloat offset = 3.0f; //tiny offset

};

