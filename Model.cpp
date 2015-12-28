#include "Model.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flag

//create bottom left corner point
//create top right point
//create box

double topx = 0.0f;

//sets up the model loading
Model::Model(GLchar * path, GLchar * textureDirectory, GLboolean collide = true)
{
	this->collide = collide;
	this->textureDirectory = textureDirectory;
	this->LoadModel(path);
}

//draws the model
void Model::Draw(Shader shader, Camera *camera, glm::vec3 translate)
{
	if (collide == true) Collision OBJ(boundingMaxY, boundingMinY, boundingMaxX, boundingMinX, boundingMaxZ, boundingMinZ, camera, translate);

	for (GLuint i = 0; i < this->meshes.size(); i++)
	{
		this->meshes[i].Draw(shader);
	}
}

//loads the model and finds the max value of each dimesnion for our collision
void Model::LoadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	this->directory = path.substr(0, path.find_last_of('/'));
	this->ProcessNode(scene->mRootNode, scene);

	cout << "maxX: " << boundingMaxX << endl;
	cout << "minX: " << boundingMinX << endl;
	cout << "maxY: " << boundingMaxY << endl;
	cout << "minY: " << boundingMinY << endl;
	cout << "maxZ: " << boundingMaxZ << endl;
	cout << "minZ: " << boundingMinZ << endl;
	cout << "\n next object \n" << endl;
}

//processes each node of a model
void Model::ProcessNode(aiNode * node, const aiScene* scene)
{
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->ProcessMesh(mesh, scene));
	}
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene);
	}
}

//processes the mesh of a model
Mesh Model::ProcessMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;

	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		//cout << i << endl;

		if (collide == true)
		{
			//bottom right top left
			if ((float)vector.x > boundingMaxX) boundingMaxX = (float)vector.x; //104.631
			if ((float)vector.x < boundingMinX) boundingMinX = (float)vector.x; //-33.8741

			if ((float)vector.y > boundingMaxY) boundingMaxY = (float)vector.y; //22.1784
			if ((float)vector.y < boundingMinY) boundingMinY = (float)vector.y; //-12.7832

			if ((float)vector.z > boundingMaxZ) boundingMaxZ = (float)vector.z; //5.8064
			if ((float)vector.z < boundingMinZ) boundingMinZ = (float)vector.z; // -4.424
		}

		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Normal = vector;
		}

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			//std::cout << vec.x << std::endl;
			vertex.TexCoords = vec;
		}

		else vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	//process indices

	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	
	//process material
	if (mesh->mMaterialIndex >= 0)
	{
		
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		//std::vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		//textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	
	//std::cout << textures.size() << std::endl;
	
	
	

	return Mesh(vertices, indices, textures);
}

// Checks all material textures of a certain type, this will only laod each texture once
std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		GLboolean skip = false;
		for (GLuint j = 0; j < textures_loaded.size(); j++)
		{
			if (textures_loaded[j].path == str)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			this->textures_loaded.push_back(texture); 
		}
	}
	return textures;
}

#include "TextureL.h"


//load a texture from file via soil
GLuint Model::TextureFromFile(const char* path, std::string directory)
{
	//Generate texture ID and load texture data 
	string filename = string(path);
	filename = textureDirectory + filename;
	//cout << filename << endl;
	//GLuint textureID;

	//TextureL tex;
	//textureID = tex.loadBMP(filename.c_str());
	
	cout << filename << endl;
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);


	cout << height << endl;
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	
	return textureID;

}



