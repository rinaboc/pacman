#include "entity.h"

#ifndef ENEMY_H
#define ENEMY_H

class GameMaster;

class Enemy : public Entity 
{
private:
    int _ID = 2;
    GameMaster* _gm;
    direction _dir;
    glm::vec3 _movementVec;
    glm::mat4 _transform;

public:
    Enemy(GameMaster* gm, glm::vec3 position, int x, int y);

    void update() override;
    void render() override;
};

#endif