#include "entity.h"

#ifndef ENEMY_H
#define ENEMY_H

class GameMaster;

class Enemy : public Entity 
{
private:
    GameMaster* _gm;
    direction _dir;
    glm::vec3 _movementVec;
    glm::mat4 _transform;
    glm::vec3 _colorMask;

    int movementTimer = 0;

public:
    Enemy(GameMaster* gm, glm::vec3 position, int x, int y, glm::vec3 colormask);

    void update() override;
    void render() override;
};

#endif