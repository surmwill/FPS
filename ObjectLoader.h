#pragma once

#include "Libraries.h"
#include <vector>
#include <iostream>
using namespace std;

//This class is no longer used due to me using assimp in a much more effient way, this again was just for learning purposes

//using namespace std;


class ObjectLoader
{
public:
	bool loadOBJ(const char* path);

	//vertices, uvs, and normals of the model
	std::vector <glm::vec3> out_vertices;
	std::vector <glm::vec2> out_uvs;
	std::vector <glm::vec3> out_normals;

	bool loadAssImp(const char * path);
};

