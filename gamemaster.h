#pragma once

#ifndef GAMEMASTER_H
#define GAMEMASTER_H
#include "player.h"
#include "constants.h"
#include "gameboard.h"
#include "boardloader.h"

class GameMaster
{
public:
	static const int _tileStep = 18;
	static const int _boardSize = 20;

private:
	int _screenWidth, _screenHeight;
	int _gameboard[20][20] = { 0 };
	std::vector<std::vector<int>>* _board = new std::vector<std::vector<int>>(20, std::vector<int>(20, 0));
	Player* _player;
	Gameboard* _gameBoard;

public:
	GameMaster(int w, int h) : _screenWidth(w), _screenHeight(h) {
		// std::vector<glm::vec3> a;
		_player = new Player(this, transformFromBoard(9, 9), 9, 9);
		_gameBoard = new Gameboard(this, _boardSize, _board);

	}

	void gameUpdate() {
		_player->update();
		_gameBoard->update();
	}

	void gameRender() {
		_gameBoard->render();
		_player->render();
	}

	Player* getPlayer() const { return _player; }

	glm::vec3 transformFromBoard(int x, int y) const {

		float _x = ((x*_tileStep + _tileStep/2.0) / _screenWidth - 0.5f)/0.5f;
        float _y = -1*((y*_tileStep + _tileStep/2.0) / _screenHeight - 0.5f)/0.5f;

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
			if (_gameboard[x][y-1] != obstacle) {
				ret[1] = y - 1;

				if(_gameboard[x][y-1] == food){
					_gameBoard->removeFoodFromVec(transformFromBoard(x, y-1));
				} 
				
				_gameboard[x][y - 1] = obj;
				_gameboard[x][y] = 0;
			}
			break;
		case right:
			if (_gameboard[x+1][y] != obstacle) {
				ret[0] = x + 1;
				if(_gameboard[x+1][y] == food){
					_gameBoard->removeFoodFromVec(transformFromBoard(x+1, y));
				}
				_gameboard[x+1][y] = obj;
				_gameboard[x][y] = 0;
			}
			break;
		case down:
			if (_gameboard[x][y+1] != obstacle) {
				ret[1] = y + 1;
				if(_gameboard[x][y+1] == food){
					_gameBoard->removeFoodFromVec(transformFromBoard(x, y+1));
				}
				_gameboard[x][y + 1] = obj;
				_gameboard[x][y] = 0;
			}
			break;
		case left:
			if (_gameboard[x - 1][y] != obstacle) {
				ret[0] = x - 1;
				if(_gameboard[x-1][y] == food){
					_gameBoard->removeFoodFromVec(transformFromBoard(x-1, y));
				}
				_gameboard[x - 1][y] = obj;
				_gameboard[x][y] = 0;
			}
			break;
		default:
			break;
		}

		return ret;
	}

	std::vector<glm::vec3> generateBoard(objectType obj) {

		static BoardLoader bl("board_src.txt");

		std::vector<glm::vec3> ret;

		if(obj == obstacle){
			for (glm::vec3& vec : bl.getObstacleVec())
			{
				_gameboard[(int)vec.x][(int)vec.y] = obstacle;
				(*_board)[(int)vec.x][(int)vec.y] = obstacle;
				ret.push_back(transformFromBoard((int)vec.x, (int)vec.y));
			}
			return ret;
		}

		else if(obj == food){
			for (glm::vec3& vec : bl.getFoodVec()){
				_gameboard[(int)vec.x][(int)vec.y] = food;
				(*_board)[(int)vec.x][(int)vec.y] = food;
				ret.push_back(transformFromBoard((int)vec.x, (int)vec.y));
			}
			return ret;
		}

		else 
			return ret;


		// // left to right
		// for (size_t i = 0; i < _boardSize; i++)
		// {
		// 	// top to bottom
		// 	for (size_t j = 0; j < _boardSize; j++)
		// 	{
		// 		if(i == 0 || i == _boardSize-1 || j == 0 || j == _boardSize-1){
		// 			_gameboard[i][j] = obstacle;
		// 			ret.push_back(transformFromBoard(i, j));
		// 		}
		// 	}
			
		// }
		
		return ret;
	}

	void endGame(){
		std::cout << "you win!" << std::endl;
		delete _player;
		delete _gameBoard;
		
		_player = new Player(this, transformFromBoard(9, 9), 9, 9);
		_gameBoard = new Gameboard(this, _boardSize, _board);
	}

	int getTileStep() const { return _tileStep; }
};


#endif // !GAMEMASTER_H