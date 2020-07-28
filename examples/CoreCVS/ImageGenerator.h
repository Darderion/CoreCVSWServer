//
// Created by dio on 7/26/20.
//

#ifndef LIBEVENTAPP_IMAGEGENERATOR_H
#define LIBEVENTAPP_IMAGEGENERATOR_H

#include <iostream>
#include <fstream>

int step = 0;
int width = 255;
int height = 255;
int max_colour = 255;

const char *img_filename = "picture.ppm";

void generateImage(int id)
{
    std::ofstream file;
    file.open(img_filename);
    file << "P3 " << width << " " << height << " " << max_colour << " ";
    for(int y = id; y < id+height; y++)
    {
        for(int x = id; x < id+width; x++)
        {
            file << x % max_colour << " " << y % max_colour << " " << x * y % max_colour << " ";
        }
    }
    file.close();
}

#endif //LIBEVENTAPP_IMAGEGENERATOR_H
