#include "player.h"
#include "gamemaster.h"
#include <cstdlib>

Player::Player(GameMaster* gm, glm::vec3 position, int x, int y) : Entity(x, y), _gm(gm) {
		float vertices[] = {
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // top right
			 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom right
			 -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom left
			 -1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f // top left 
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

		setShader("shaders/playerShader.vs", "shaders/playerShader.fs");

		_dir = right;
		_moving = false;
		_movementVec = position;
	}

    void Player::update() {

		static int movementTimer = 0;

        if(_moving && (++movementTimer >= _movementSpeed)){
			int oldX = boardPosX;
			int oldY = boardPosY;

            _gm->moveObjectOnBoard(player, boardPosX, boardPosY, _dir, this);

			// change direction if player hits wall
			if(oldX == boardPosX && oldY == boardPosY){
				for(direction d : _gm->possibleDirs(boardPosX, boardPosY)){
					if(abs(_dir) != abs(d)){
						_dir = d;
						break;
					}
				}
			} else {
            	_movementVec = _gm->transformFromBoard(boardPosX, boardPosY);
				movementTimer = 0;
			}
        }
	}

    void Player::render() {
		Entity::render();

		// player location
		_transform = glm::mat4(1.0f);

		float angle = 0.0f;
		if(_dir == up) angle = 90.0f;
		else if(_dir == down) angle = 270.0f;
		else if(_dir == left) angle = 180.0f;

		_transform = glm::translate(_transform, _movementVec);
		_transform = glm::rotate(_transform, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
		_transform = glm::scale(_transform, glm::vec3(1.0f/GameMaster::_boardSize, 1.0f/GameMaster::_boardSize, 1.0f/GameMaster::_boardSize));

		// mouth animation
		float timeValue = glfwGetTime() * 2;
		float intensity = abs( timeValue*1.5f - floor(timeValue*1.5f + 0.5f));
		intensity = (round(intensity * 10)) / 10;

		_shader.setMat4("transform", _transform);
		_shader.setFloat("intensity", intensity);	

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // for square
	}

	void Player::setDirection(direction dir){
		std::vector<direction> allowedDirs = _gm->possibleDirs(boardPosX, boardPosY);

		for(direction& d : allowedDirs){
			if(d == dir){
				_dir = dir;
				return;
			}
		}
	}