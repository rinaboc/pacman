#pragma once
#include <glad/glad.h>
#include "shader.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef ENTITY_H
#define ENTITY_H

class Entity
{
protected:
	unsigned int _VAO, _VBO, _EBO;
	Shader _shader;

public:
	Entity() {

		glGenBuffers(1, &_VBO);
		glGenVertexArrays(1, &_VAO);

		glBindVertexArray(_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);

		glGenBuffers(1, &_EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);

		// set basic shaders
		_shader = Shader("4.0.shader.vs", "4.0.shader.fs");
	}

	void setShader(const char* vs, const char* fs) {
		_shader = Shader(vs, fs);
	}

	virtual void render() {
		_shader.use();

		glBindVertexArray(_VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3); // for triangle
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // for square
	}

	virtual void update() = 0;
};


#endif // !ENTITY_H