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

void generateImage()
{
    std::ofstream file;
    file.open("picture.ppm");
    file << "P3 " << width << " " << height << " " << max_colour << " ";
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            file << x % max_colour << " " << y % max_colour << " " << x * y % max_colour << " ";
        }
    }
    file.close();
    system("xdg-open picture.ppm");
}

#endif //LIBEVENTAPP_IMAGEGENERATOR_H
