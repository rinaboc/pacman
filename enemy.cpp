#include "enemy.h"
#include "gamemaster.h"
#include <cstdlib>

Enemy::Enemy(GameMaster* gm, glm::vec3 position, int x, int y, glm::vec3 colormask) : Entity(x,y),_gm(gm), _colorMask(colormask) {
    float vertices[] = {
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top right
			 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, // bottom right
			 -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, // bottom left
			 -1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f // top left 
		};

		unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

		// vertex attribute pointer set up
		// location = 0 -> vertex position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// location = 1 -> color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		setShader("shaders/blockShader.vs", "shaders/enemyShader.fs");

		_dir = right;
		_movementVec = position;
		srand((unsigned) time(NULL));
}

void Enemy::update(){

        if(++movementTimer >= _movementSpeed){
			int tmpx = boardPosX;
			int tmpy = boardPosY;

			// at intersections generate a random direction
			std::vector<direction> possibleDirs = _gm->possibleDirs(boardPosX, boardPosY);
			if(possibleDirs.size() > 2){
				
				int i = 0;
				direction newDir[possibleDirs.size()-1];
				for(direction dir: possibleDirs){
					if(dir != _dir * (-1)){
						newDir[i++] = dir;
					}
				}

				_dir = newDir[rand() % (possibleDirs.size()-1)];
			}

            _gm->moveObjectOnBoard(enemy, boardPosX, boardPosY, _dir, this);

			if(boardPosX == tmpx && boardPosY == tmpy){ // when running into wall change direction
				_dir = (direction)(((int)(floor(glfwGetTime()*10)) % 4)-2);
			} 
			else { // otherwise reset timer and move
				_movementVec = _gm->transformFromBoard(boardPosX, boardPosY);				
				movementTimer = 0;
			}

        }
}

void Enemy::render(){
        Entity::render();

		// draw position
		_transform = glm::mat4(1.0f);
		_transform = glm::translate(_transform, _movementVec);
		_transform = glm::scale(_transform, glm::vec3(1.0f/GameMaster::_boardSize, 1.0f/GameMaster::_boardSize, 1.0f/GameMaster::_boardSize));

		_shader.setMat4("transform", _transform);
		_shader.setVec3("colorMask", _colorMask);
		_shader.setFloat("wave", glfwGetTime()*8); // animation

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // for square
}