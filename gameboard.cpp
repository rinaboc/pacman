#include "gameboard.h"
#include "gamemaster.h"

Gameboard::Gameboard(GameMaster* gm, int tile, std::vector<std::vector<int>>* &board): Entity(), _gm(gm), _tile(tile), _board(board) {
        _obstacleDrawVec = _gm->generateBoard(obstacle);
        _foodDrawVec = _gm->generateBoard(food);

        float vertices[] = {
			 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // top right
			 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom right
			 -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
			 -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f // top left 
		};

		unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// vertex attribute pointer set up
		// location = 0 -> vertex position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// location = 1 -> color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		setShader("shaders/blockShader.vs", "shaders/blockShader.fs");
		_foodShader = Shader("shaders/blockShader.vs", "shaders/foodShader.fs");
    }

    void Gameboard::update(){
        if(_foodDrawVec.empty()){
            _gm->endGame();
        }
    }