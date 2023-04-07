#include "gamemaster.h"


void GameMaster::startGameLoop(GLFWwindow* window){
    static double limitUPS = 1.0 / 120.0;
    static double limitFPS = 1.0 / 60.0;

    double lastTime = glfwGetTime(), timer = lastTime;
    double deltaTime = 0, nowTime = 0, deltaFrame = 0;
    int frames = 0, updates = 0;

    while (!glfwWindowShouldClose(window)) {

        nowTime = glfwGetTime();
        deltaTime += (nowTime - lastTime) / limitUPS;
        deltaFrame += (nowTime - lastTime) / limitFPS;
        lastTime = nowTime;

        while (deltaTime >= 1.0) { // update according to UPS
            gameUpdate();
            updates++;
            deltaTime--;
        }

        while (deltaFrame >= 1.0) { // render according to FPS
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            gameRender();

            frames++;
            deltaFrame--;
        }

        // FPS & UPS counter, reset every second
        if (glfwGetTime() - timer > 1.0) {
            timer++;
            std::cout << "FPS: " << frames << " Updates:" << updates << std::endl;
            updates = 0, frames = 0;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void GameMaster::gameUpdate() const {
    if(_gameboard != nullptr)
        _gameBoard->update();

    for(Enemy* enemy: _enemies){
        enemy->update();
    }

    if(_player != nullptr)
        _player->update();

    // if((int)(glfwGetTime()*10) % 10 != 0) return;
    // system("clear");
    // for (size_t i = 0; i < 20; i++)
    // {
    // 	for (size_t j = 0; j < 20; j++)
    // 	{
    // 		std::cout << _gameboard[j][i] << " ";
    // 	}
    // 	std::cout << std::endl;
    // }
}

void GameMaster::gameRender() const {
    if(_gameBoard != nullptr) _gameBoard->render();

    for(Enemy* enemy: _enemies)
        enemy->render();

    if(_player != nullptr) _player->render();
}

glm::vec3 GameMaster::transformFromBoard(int x, int y) const {

    // calculate draw coordinates from matrix index
    float _x = ((x*_tileStep + _tileStep/2.0) / _screenWidth - 0.5f)/0.5f;
    float _y = -1*((y*_tileStep + _tileStep/2.0) / _screenHeight - 0.5f)/0.5f;

    return glm::vec3(_x, _y, 0);
}

void GameMaster::moveObjectOnBoard(objectType obj, int x, int y, direction dir, Entity* entity) {

    int ret[2] = {x, y};
    bool canMove = false;
    int * nextLoc;

    if (dir == up && _gameboard[x][y-1] != obstacle) {
        ret[1] = y - 1;
        nextLoc = &_gameboard[x][y-1];

        if(*nextLoc == food && obj == player){ // player eats food
            _gameBoard->removeFoodFromVec(transformFromBoard(x, y-1));
        } 
        canMove = true;
    }

    else if (dir == right && _gameboard[x+1][y] != obstacle) {
        ret[0] = x + 1;
        nextLoc = &_gameboard[x+1][y];

        if(*nextLoc == food && obj == player){
            _gameBoard->removeFoodFromVec(transformFromBoard(x+1, y));
        }
        canMove = true;
    }

    else if (dir == down && _gameboard[x][y+1] != obstacle) {
        ret[1] = y + 1;
        nextLoc = &_gameboard[x][y+1];

        if(*nextLoc == food && obj == player){
            _gameBoard->removeFoodFromVec(transformFromBoard(x, y+1));
        }
        canMove = true;
    }

    else if (dir == left && _gameboard[x - 1][y] != obstacle) {
        ret[0] = x - 1;
        nextLoc = &_gameboard[x-1][y];

        if(*nextLoc == food && obj == player){
            _gameBoard->removeFoodFromVec(transformFromBoard(x-1, y));
        }
        canMove = true;
    }

    if(!canMove) return;

    if(obj == player){
        if(*nextLoc == enemy || *nextLoc == enemyAndFood){ // player collides with enemy
            endGame("You died!");
            return;
        }

        *nextLoc = player;
        _gameboard[x][y] = 0;
    }

    if(obj == enemy){ // ghost wants to move

        if(*nextLoc == food) // stepping over food
            *nextLoc = enemyAndFood;

        else if(*nextLoc == player)	// catching player
        {
            endGame("You died!");
            return;
        }

        else if(*nextLoc == enemyAndFood || *nextLoc == enemy) // don't collide with other ghosts
            return;

        else // move otherwise
            *nextLoc = enemy;


        if(_gameboard[x][y] == enemyAndFood) 
            _gameboard[x][y] = food;
        else
            _gameboard[x][y] = 0;
    }
    
    entity->setPos(ret[0], ret[1]);
}

// get draw coordinates read from board source map
std::vector<glm::vec3> GameMaster::generateBoard(objectType obj) {
    std::vector<glm::vec3> ret;

    static BoardLoader bl("board_src.txt");

    // building up gameboard matrix
    if(obj == obstacle){
        for (glm::vec3& vec : bl.getObstacleVec())
        {
            _gameboard[(int)vec.x][(int)vec.y] = obstacle;
            ret.push_back(transformFromBoard((int)vec.x, (int)vec.y));
        }
        return ret;
    }

    else if(obj == food){
        for (glm::vec3& vec : bl.getFoodVec()){
            _gameboard[(int)vec.x][(int)vec.y] = food;
            ret.push_back(transformFromBoard((int)vec.x, (int)vec.y));
        }
        return ret;
    }
    
    return ret;
}