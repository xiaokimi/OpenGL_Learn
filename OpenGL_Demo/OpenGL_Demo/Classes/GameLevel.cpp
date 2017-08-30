#include "GameLevel.h"

#include <fstream>
#include <sstream>

GameLevel::GameLevel()
{

}

void GameLevel::load(const GLchar *file, GLuint levelWidth, GLuint levelHeight)
{
	this->_bricks.clear();

	GLuint tileCode;
	GameLevel level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<GLuint> > tileData;
	if (fstream)
	{
		while (std::getline(fstream, line))
		{
			std::istringstream sstream(line);
			std::vector<GLuint> row;
			while (sstream >> tileCode)
			{
				row.push_back(tileCode);
			}
			tileData.push_back(row);
		}

		if (tileData.size() > 0)
		{
			this->init(tileData, levelWidth, levelHeight);
		}
	}
}

void GameLevel::draw(SpriteRenderer &renderer)
{
	for (GameObject &tile : this->_bricks)
	{
		if (!tile._destroyed)
		{
			tile.draw(renderer);
		}
	}
}

GLboolean GameLevel::isCompleted()
{
	for (GameObject &tile : this->_bricks)
	{
		if (!tile._isSolid && !tile._destroyed)
		{
			return false;
		}
	}

	return true;
}

void GameLevel::init(std::vector<std::vector<GLuint> > tileData, GLuint levelWidth, GLuint levelHeight)
{
	GLuint height = tileData.size();
	GLuint width = tileData[0].size();

	GLfloat unitWidth = levelWidth * 1.0f / width;
	GLfloat unitHeight = levelHeight * 1.0f / height;

	for (GLuint y = 0; y < height; ++y)
	{
		for (GLuint x = 0; x < width; ++x)
		{
			GLuint tileType = tileData[y][x];
			glm::vec2 pos(unitWidth * x, unitHeight * y);
			glm::vec2 size(unitWidth, unitHeight);

			if (tileType == 1)
			{
				Texture2D block = ResourceManager::getTexture("blockSolid");
				GameObject obj(pos, size, block, glm::vec3(0.8f, 0.8f, 0.7f));
				obj._isSolid = GL_TRUE;
				this->_bricks.push_back(obj);
			}
			else if (tileType > 1)
			{
				glm::vec3 color = glm::vec3(1.0f);
				if (tileType == 2)
				{
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				}
				else if (tileType == 3)
				{
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				}
				else if (tileType == 4)
				{
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				}
				else if (tileType == 5)
				{
					color = glm::vec3(1.0f, 0.5f, 0.0f);
				}

				Texture2D block = ResourceManager::getTexture("block");
				GameObject obj(pos, size, block, color);
				this->_bricks.push_back(obj);
			}
		}
	}
}