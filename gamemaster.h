#ifndef GAMEMASTER_H
#define GAMEMASTER_H
#include "player.h"
#include "constants.h"
#include "gameboard.h"
#include "boardloader.h"
#include "enemy.h"

class GameMaster
{
public:
	const int _tileStep = 22;
	const int _boardSize = 21;

private:
	int _screenWidth, _screenHeight;
	int _gameboard[21][21] = { 0 };
	Player* _player;
	std::vector<Enemy*> _enemies;
	Gameboard* _gameBoard;

	bool _game = false;

public:
	GameMaster(int w, int h) : _screenWidth(w), _screenHeight(h) {
		initClasses();
		_game = true;
	}

	~GameMaster(){
		delete _player;
		for (size_t i = 0; i < (size_t)_enemies.size(); i++)
		{
			delete _enemies[i];
		}
		delete _gameBoard;
	}

	void initClasses(){
		_player = new Player(this, transformFromBoard(10, 1), 10, 1);

		_enemies.push_back(new Enemy(this, transformFromBoard(9,10), 9,10, glm::vec3(1, 0, 1)));
		_enemies.push_back(new Enemy(this, transformFromBoard(10,10), 10,10, glm::vec3(0, 1, 1)));
		_enemies.push_back(new Enemy(this, transformFromBoard(10,11), 10,11, glm::vec3(0.2, 0.1, 1)));

		_gameBoard = new Gameboard(this, _boardSize);
	}

	void startGameLoop(GLFWwindow* window);

	void gameUpdate();

	void gameRender() const;

	Player* getPlayer() const { return _player; }

	glm::vec3 transformFromBoard(int x, int y) const;

	void moveObjectOnBoard(objectType obj, int x, int y, direction dir, Entity* entity);

	std::vector<glm::vec3> generateBoard(objectType obj);

	std::vector<direction> possibleDirs(int x, int y){
		std::vector<direction> ret;

		if(_gameboard[x-1][y] != obstacle)
			ret.push_back(left);
		if(_gameboard[x+1][y] != obstacle)
			ret.push_back(right);
		if(_gameboard[x][y-1] != obstacle)
			ret.push_back(up);
		if(_gameboard[x][y+1] != obstacle)
			ret.push_back(down);

		return ret;
	}

	void endGame(std::string msg);

	int getTileStep() const { return _tileStep; }

	void setGame(bool g) {
		_game = g;
	}
};


#endif // !GAMEMASTER_H