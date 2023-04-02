#pragma once
#include "entity.h"
#include "constants.h"
#include <math.h>

#ifndef PLAYER_H
#define PLAYER_H

class GameMaster;

class Player : public Entity
{
private:
	const int _movementSpeed = 30;
	int boardPosX, boardPosY;
	GameMaster* _gm;
	glm::vec3 _movementVec;
	glm::mat4 _transform;
	direction _dir;
	bool _moving;

public:
	Player(GameMaster* gm, glm::vec3 position, int x, int y);

	void update() override;

	void render() override;

	void setDirection(direction dir) {
		_dir = dir;
	}

	void setMoving(bool moving) {
		_moving = moving;
	}

	direction getDirection() const { return _dir; }

	bool getMoving() const { return _moving; }

	void movePlayer(glm::vec3 mvec) {
		_movementVec = mvec;
	}
};
#endif
