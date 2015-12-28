#pragma once
#include "Libraries.h"
#include <iostream>
#include "ObjectLoader.h"

using namespace glm;

class Buffer
{
public:
	Buffer(ObjectLoader OBJ);
	void Display();
	void cleanup();

private:
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	ObjectLoader OBJ;
};

