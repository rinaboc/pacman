#pragma once
#include "entity.h"
#include "constants.h"

#ifndef PLAYER_H
#define PLAYER_H

class Player : public Entity
{
public:
	int boardPosX, boardPosY;
private:
	direction _dir;
	bool _moving;
	const int _movementSpeed = 10;
	glm::vec3 _movementVec;
	glm::mat4 _transform;

public:
	Player(glm::vec3 position, int x, int y) : Entity(), boardPosX(x), boardPosY(y) {
		float vertices[] = {
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f,  // top right
			 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom right
			 -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom left
			 -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f // top left 
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

		setShader("playerShader.vs", "playerShader.fs");

		_dir = right;
		_moving = false;
		_movementVec = position;
	}

	void update() override {
		_transform = glm::mat4(1.0f);
		_transform = glm::translate(_transform, _movementVec);

		switch (_dir)
		{
		case up:
			//if (_moving)
			//	_movementVec = _movementVec + glm::vec3(0.0, _movementSpeed, 0.0);
			_transform = glm::rotate(_transform, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
			break;
		case right:
			//if (_moving)
			//	_movementVec = _movementVec + glm::vec3(_movementSpeed, 0.0, 0.0);
			_transform = glm::rotate(_transform, glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0));
			break;
		case down:
			//if (_moving)
			//	_movementVec = _movementVec + glm::vec3(0.0, -_movementSpeed, 0.0);
			_transform = glm::rotate(_transform, glm::radians(270.0f), glm::vec3(0.0, 0.0, 1.0));
			break;
		case left:
			//if (_moving)
			//	_movementVec = _movementVec + glm::vec3(-_movementSpeed, 0.0, 0.0);
			_transform = glm::rotate(_transform, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
			break;
		default:
			break;
		}
		_transform = glm::scale(_transform, glm::vec3(0.1, 0.1, 0.1));
		_shader.setMat4("transform", _transform);

		//std::cout << _movementVec.x << " " << _movementVec.y << " " << _movementVec.z << std::endl;
	}

	void render() override {
		float timeValue = glfwGetTime();
		float intensity = abs( timeValue*1.5f - floor(timeValue*1.5f + 0.5f));

		// steps
		intensity = (round(intensity * 10)) / 10;

		_shader.setFloat("intensity", intensity);	

		Entity::render();
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // for square
	}

	void setDirection(direction dir) {
		_dir = dir;
	}

	direction getDirection() const { return _dir; }

	void setMoving(bool moving) {
		_moving = moving;
	}

	bool getMoving() const {
		return _moving;
	}

	void movePlayer(glm::vec3 mvec) {
		_movementVec = mvec;
	}
};
#endif
