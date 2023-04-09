#ifndef ENTITY_H
#define ENTITY_H
#include "object.h"
#include "constants.h"


class Entity : public Object
{
protected:
	const int _movementSpeed = 20;
	int boardPosX, boardPosY;
	direction _dir;

public:
	Entity(int x, int y) : Object(), boardPosX(x), boardPosY(y), _dir(left){}

	void setDirection(direction dir) {
		_dir = dir;
	}

	void render() override {
		Object::render();
	}

	direction getDirection() const { return _dir; }

	void setPos (int x, int y) { boardPosX = x; boardPosY = y; }

};


#endif // !ENTITY_H