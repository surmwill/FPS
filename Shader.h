#pragma once
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

class Shader
{
public:
	// Our program ID
	GLuint Program;
	// Constructor reads and builds our shader
	void CreateShader(const GLchar* vertexSourcePath, const GLchar* fragmentSourcePath);
	// Use our program
	void Use();
	
};

