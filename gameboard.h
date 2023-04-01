#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <vector>
#include "entity.h"

class Gameboard : public Entity
{
private:
    std::vector<glm::vec3> _drawVec;
public:
    Gameboard(std::vector<glm::vec3> drawVec): Entity(), _drawVec(drawVec) {
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

		setShader("blockShader.vs", "blockShader.fs");
    }

    void update() override {

    }

    void render() override {
		Entity::render();

		glm::mat4 _transform;

		for (glm::vec3 transVec : _drawVec){
			_transform = glm::mat4(1.0f);
			_transform = glm::translate(_transform, transVec);
			_transform = glm::scale(_transform, glm::vec3(1.0f/36, 1.0f/36, 1.0f/36));
			_shader.setMat4("transform", _transform);
        	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // for square
		}
		

    }
};


#endif