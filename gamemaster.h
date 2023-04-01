#pragma once

#ifndef GAMEMASTER_H
#define GAMEMASTER_H
#include "player.h"
#include "constants.h"
#include "gameboard.h"
#include "boardLoader.h"

class GameMaster
{
private:
	const int _tileStep = 18;
	int _gameboard[36][36] = { 0 };
	Player* _player;
	Gameboard* _gameBoard;

public:
	GameMaster() {
		// std::vector<glm::vec3> a;
		_player = new Player(this, transformFromBoard(18, 18), 18, 18);
		_gameBoard = new Gameboard(generateBoard());
	}

	void gameUpdate() {
		_player->update();
	}

	void gameRender() {
		_gameBoard->render();
		_player->render();
	}

	Player* getPlayer() const { return _player; }

	glm::vec3 transformFromBoard(int x, int y) const {

		float _x = ((x*_tileStep + _tileStep/2.0) / 648.0f - 0.5f)/0.5f;
        float _y = -1*((y*_tileStep + _tileStep/2.0) / 648.0f - 0.5f)/0.5f;

		// float _x = sqrt((x - _tileStep)*(x - _tileStep)) / _tileStep;
		// float _y = sqrt((y - _tileStep)*(y - _tileStep)) / _tileStep;

		// if (x < _tileStep) _x *= -1;
		// if (y >= _tileStep) _y *= -1;

		// std::cout << _x << " " << _y << std::endl;

		return glm::vec3(_x, _y, 0);
	}

	int* moveObjectOnBoard(objectType obj, int x, int y, direction dir) {
		static int ret[2];
		ret[0] = x;
		ret[1] = y;

		switch (dir)
		{
		case up:
			if (y != 0 && _gameboard[x][y-1] == none) {
				ret[1] = y - 1;
				_gameboard[x][y - 1] = obj;
				_gameboard[x][y] = 0;
			}
			break;
		case right:
			if (x != _tileStep*2-1 && _gameboard[x+1][y] == none) {
				ret[0] = x + 1;
				_gameboard[x+1][y] = obj;
				_gameboard[x][y] = 0;
			}
			break;
		case down:
			if (y != _tileStep*2-1 && _gameboard[x][y+1] == none) {
				ret[1] = y + 1;
				_gameboard[x][y + 1] = obj;
				_gameboard[x][y] = 0;
			}
			break;
		case left:
			if (x != 0 && _gameboard[x - 1][y] == none) {
				ret[0] = x - 1;
				_gameboard[x - 1][y] = obj;
				_gameboard[x][y] = 0;
			}
			break;
		default:
			break;
		}

		return ret;
	}

	std::vector<glm::vec3> generateBoard() {

		BoardLoader("boardsrc.png");

		std::vector<glm::vec3> ret;

		// left to right
		for (size_t i = 0; i < _tileStep * 2; i++)
		{
			// top to bottom
			for (size_t j = 0; j < _tileStep * 2; j++)
			{
				if(i == 0 || i == _tileStep*2-1 || j == 0 || j == _tileStep * 2-1){
					_gameboard[i][j] = obstacle;
					ret.push_back(transformFromBoard(i, j));
				}
			}
			
		}
		
		return ret;
	}

	int getTileStep() const { return _tileStep; }
};


#endif // !GAMEMASTER_H