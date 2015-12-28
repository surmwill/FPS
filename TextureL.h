#pragma once
#include <iostream>
#include "Libraries.h"

//using namespace std;

class TextureL
{
public:
	GLuint loadBMP(const char* filename);
	GLuint loadDDS(const char * filename);
};

