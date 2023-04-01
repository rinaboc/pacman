#ifndef BOARDLOADER_H
#define BOARDLOADER_H

#include <string>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


class BoardLoader
{
public:
    BoardLoader(std::string filename){
    
        // ... x = width, y = height, n = # 8-bit components per pixel ...
        // ... replace '0' with '1'..'4' to force that many components per pixel
        // ... but 'n' will always be the number that it would have been if you said 0
        int x,y,n;
        unsigned char *data = stbi_load(filename.c_str(), &x, &y, &n, 0);
        
        // ... process data if not NULL ..
        if (data != nullptr && x > 0 && y > 0)
        {
            if (n == 3)
            {
                
                std::cout << "First pixel: RGB "
                        << static_cast<int>(data[0]) << " "
                        << static_cast<int>(data[1]) << " "
                        << static_cast<int>(data[2]);
            }
        }
        else
        {
            std::cout << "Some error\n";
        }

        stbi_image_free(data);
    }
};

#endif