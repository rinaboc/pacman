#ifndef BOARDLOADER_H
#define BOARDLOADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "constants.h"

class BoardLoader 
{
private:
    std::vector<glm::vec3> genObstacle;
    std::vector<glm::vec3> genFood;
public:
    BoardLoader(std::string fileName){
        std::fstream fs(fileName);

        if(fs.is_open()){
            std::string line;
            int j = 0;

            while(getline(fs, line)){
                std::stringstream ss;
                ss << line;

                int i = 0;
                int obj;
                while(ss >> obj){
                    if(obj == obstacle){
                        genObstacle.push_back(glm::vec3(i, j, 0));
                    }
                    else if(obj == food){
                        genFood.push_back(glm::vec3(i, j, 0));
                    }
                    i++;
                }
                j++;
            }
        }

        fs.close();
    }

    std::vector<glm::vec3> getObstacleVec() const { return genObstacle; }

    std::vector<glm::vec3> getFoodVec() const { return genFood; }
};

#endif