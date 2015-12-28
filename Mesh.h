#pragma once
#include "Libraries.h"
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flag
#include "Shader.h"

//properties of each vertex
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

//properties of each structure
struct Texture
{
	GLuint id;
	std::string type;
	aiString path;
};


class Mesh
{
public:
	//mesh data
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;

	//functions
	Mesh(std::vector <Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
	void Draw(Shader shader);

private:
	//different arrays
	GLuint VAO, VBO, EBO;
	void SetupMesh();
};

