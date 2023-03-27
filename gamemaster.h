#pragma once

#ifndef GAMEMASTER_H
#define GAMEMASTER_H
#include "player.h"
#include "constants.h"

class GameMaster
{
private:
	const int _tileStep = 18;
	int _gameboard[36][36] = { 0 };
	Player* _player;
public:
	GameMaster() {
		_gameboard[1][18] = 1;
		_player = new Player(transformFromBoard(18, 18), 18, 18);
	}

	void gameUpdate() {
		std::cout << _player->boardPosX << " " << _player->boardPosY << std::endl;
		if (_player->getMoving() && (int)(glfwGetTime() * 100) % 2 == 0) {
			int* tmp = moveObjectOnBoard(_player->boardPosX, _player->boardPosY, _player->getDirection());
			_player->boardPosX = (*tmp);
			_player->boardPosY = *(tmp + 1);
			_player->movePlayer(transformFromBoard(*tmp, *(tmp+1)));
		}
		_player->update();
	}

	void gameRender() {
		_player->render();
	}

	Player* getPlayer() const { return _player; }

	glm::vec3 transformFromBoard(int x, int y) const {
		float _x = sqrt((x - _tileStep)*(x - _tileStep)) / _tileStep;
		float _y = sqrt((y - _tileStep)*(y - _tileStep)) / _tileStep;

		if (x < _tileStep) _x *= -1;
		if (y >= _tileStep) _y *= -1;

		std::cout << _x << " " << _y << std::endl;

		return glm::vec3(_x, _y, 0);
	}

	int* moveObjectOnBoard(int x, int y, direction dir) {
		int ret[2] = { x, y };
		switch (dir)
		{
		case up:
			if (y != 1) {
				ret[1] = y - 1;
				_gameboard[x][y - 1] = 1;
				_gameboard[x][y] = 0;
			}
			break;
		case right:
			if (x != _tileStep*2-1) {
				ret[0] = x + 1;
				_gameboard[x+1][y] = 1;
				_gameboard[x][y] = 0;
			}
			break;
		case down:
			if (y != _tileStep*2-1) {
				ret[1] = y + 1;
				_gameboard[x][y + 1] = 1;
				_gameboard[x][y] = 0;
			}
			break;
		case left:
			if (x != 1) {
				ret[0] = x - 1;
				_gameboard[x - 1][y] = 1;
				_gameboard[x][y] = 0;
			}
			break;
		default:
			break;
		}

		return ret;
	}
};


#endif // !GAMEMASTER_H