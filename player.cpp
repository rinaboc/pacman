#include "player.h"
#include "gamemaster.h"

Player::Player(GameMaster* gm, glm::vec3 position, int x, int y) : Entity(), boardPosX(x), boardPosY(y), _gm(gm) {
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

        // std::cout << boardPosX << " " << boardPosY << " ";


        if(_moving && (++movementTimer == _movementSpeed)){
            int* newPos = _gm->moveObjectOnBoard(player, boardPosX, boardPosY, _dir);

            boardPosX = *newPos;
            boardPosY = *(newPos+1);
            _movementVec = _gm->transformFromBoard(boardPosX, boardPosY);
			
			movementTimer = 0;
        }
        
		_transform = glm::mat4(1.0f);
		_transform = glm::translate(_transform, _movementVec);

		switch (_dir)
		{
		case up:
			_transform = glm::rotate(_transform, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
			break;
		case right:
			_transform = glm::rotate(_transform, glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0));
			break;
		case down:
			_transform = glm::rotate(_transform, glm::radians(270.0f), glm::vec3(0.0, 0.0, 1.0));
			break;
		case left:
			_transform = glm::rotate(_transform, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
			break;
		default:
			break;
		}
		_transform = glm::scale(_transform, glm::vec3(1.0f/GameMaster::_boardSize, 1.0f/GameMaster::_boardSize, 1.0f/GameMaster::_boardSize));
		_shader.setMat4("transform", _transform);

		// std::cout << _movementVec.x << " " << _movementVec.y << " " << _movementVec.z << std::endl;
	}

    void Player::render() {
		Entity::render();

		float timeValue = glfwGetTime();
		float intensity = abs( timeValue*1.5f - floor(timeValue*1.5f + 0.5f));

		// steps
		intensity = (round(intensity * 10)) / 10;

		_shader.setFloat("intensity", intensity);	

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // for square
	}