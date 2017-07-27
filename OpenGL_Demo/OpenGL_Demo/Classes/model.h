#ifndef __MODEL_H__
#define __MODEL_H__

#include <iostream>

#include <GL/glew.h>

#include <assimp/scene.h>

#include "shader.h"
#include "mesh.h"

using namespace std;

class Model
{
public:
	Model(GLchar* path);
	void Draw(Shader shader, int instanceAmount = 0, bool useInstance = false);

	vector<Mesh> meshes;
	vector<Texture> textures_loaded;

	void addInstanceMatrix(glm::mat4 *modelMatrix, int matrixAmout);
private:
	string directory;

	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

#endif