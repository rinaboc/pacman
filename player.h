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
	GameMaster* _gm;
	glm::vec3 _movementVec;
	glm::mat4 _transform;
	bool _moving;

public:
	Player(GameMaster* gm, glm::vec3 position, int x, int y);

	void update() override;

	void render() override;

	void setMoving(bool moving) {
		_moving = moving;
	}

	bool getMoving() const { return _moving; }

	void movePlayer(glm::vec3 mvec) {
		_movementVec = mvec;
	}
};
#endif
