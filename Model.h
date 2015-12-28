#pragma once
#include "Libraries.h"
#include <vector>
#include <string>
#include "Mesh.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flag
#include <SOIL.h>
#include "Camera.h"
#include "Collision.h"
#include "Shader.h"


class Model
{
public:
	Model(GLchar * path, GLchar * textureDirectory, GLboolean collide);
	void Draw(Shader shader, Camera *camera, glm::vec3 translate);
private:
	//properties of the moedl
	std::vector <Texture> textures_loaded;
	std::vector <Mesh> meshes;
	std::string directory;
	std::string textureDirectory;

	//collision variables
	GLfloat boundingMaxY = 0;
	GLfloat boundingMinY = 10000;
	GLfloat boundingMaxX = 0;
	GLfloat boundingMinX = 10000;
	GLfloat boundingMaxZ = 0;
	GLfloat boundingMinZ = 10000;

	//whether we want collision
	GLboolean collide = true;

	GLuint programID;

	//our functions
	void LoadModel(std::string path);
	void ProcessNode(aiNode * node, const aiScene * scene);
	Mesh ProcessMesh(aiMesh * mesh, const aiScene * scene);
	std::vector <Texture> loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName);
	GLuint TextureFromFile(const GLchar * path, std::string directory);
};

