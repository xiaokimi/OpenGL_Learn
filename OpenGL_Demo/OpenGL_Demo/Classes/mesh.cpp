#include "mesh.h"

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	this->setupMesh();
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Tangent));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, BiTangent));

	glBindVertexArray(0);
}

void Mesh::addInstanceMatrix(glm::mat4 *modelMatrix, int matrixAmount)
{
	glBindVertexArray(this->VAO);

	GLuint matrixVBO;
	glGenBuffers(1, &matrixVBO);
	glBindBuffer(GL_ARRAY_BUFFER, matrixVBO);

	glBufferData(GL_ARRAY_BUFFER, matrixAmount * sizeof(glm::mat4), &modelMatrix[0], GL_STATIC_DRAW);

	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(4);

	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);

	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(3 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
}

void Mesh::Draw(Shader shader, int instanceAmount /* = 0 */, bool useInstance /* = false */)
{
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	GLuint normalNr = 1;

	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		stringstream ss;
		string number;
		string name = this->textures[i].type;

		if (name == "texture_diffuse")
			ss << diffuseNr++;
		else if (name == "texture_specular")
			ss << specularNr++;
		else if (name == "texture_normal")
			ss << normalNr++;

		number = ss.str();

		glUniform1i(glGetUniformLocation(shader.Program, ("material." + name + number).c_str()), i);
	
		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	}

	glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 32.0f);

	glBindVertexArray(this->VAO);
	if (useInstance)
	{
		glDrawElementsInstanced(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0, instanceAmount);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);

	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}