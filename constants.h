#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H	
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum direction { up = 1, right = 2, down = -1, left = -2 };
enum objectType { enemyAndFood = -1, none = 0, player = 1, enemy = 2, obstacle = 3, food = 4};

#endif // !CONSTANTS_H
