#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <vector>
#include "entity.h"

class GameMaster;

class Gameboard : public Entity
{
private:
	GameMaster* _gm;
    std::vector<glm::vec3> _obstacleDrawVec;
	std::vector<glm::vec3> _foodDrawVec;
	int _tile = 0;
	Shader _foodShader;
public:
    Gameboard(GameMaster* gm, int tile);

    void update() override;

    void render() override {
		Entity::render();

		glm::mat4 _transform;

		_shader.use();
		for (glm::vec3 transVec : _obstacleDrawVec){
			_transform = glm::mat4(1.0f);
			_transform = glm::translate(_transform, transVec);
			_transform = glm::scale(_transform, glm::vec3(1.0f/_tile, 1.0f/_tile, 1.0f/_tile));
			_shader.setMat4("transform", _transform);

        	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // for square
		}

		_foodShader.use();
		for (glm::vec3 transVec : _foodDrawVec){
			_transform = glm::mat4(1.0f);
			_transform = glm::translate(_transform, transVec);
			_transform = glm::scale(_transform, glm::vec3(1.0f/_tile, 1.0f/_tile, 1.0f/_tile));
			_shader.setMat4("transform", _transform);

        	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // for square
		}

    }

	void removeFoodFromVec(glm::vec3 pos){
		std::vector<glm::vec3> newFoodVec;

		for (glm::vec3 vec : _foodDrawVec)
		{
			if(vec == pos){
				continue;
			}

			newFoodVec.push_back(vec);
		}

		_foodDrawVec = newFoodVec;
		
	}
};


#endif