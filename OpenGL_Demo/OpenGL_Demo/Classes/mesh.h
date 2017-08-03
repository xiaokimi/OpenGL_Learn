#ifndef __MESH_H__
#define __MESH_H__

#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/types.h>

#include "shader.h"

using namespace std;

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 BiTangent;
};

struct Texture
{
	GLuint id;
	string type;
	aiString path;
};

class Mesh
{
public:
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
	void Draw(Shader shader, int instanceAmount = 0, bool useInstance = false);

	void addInstanceMatrix(glm::mat4 *modelMatrix, int matrixAmount);

private:
	GLuint VAO;
	GLuint VBO, EBO;
	
	void setupMesh();
};

#endif