//
// Created by dio on 8/7/20.
//

#ifndef LIBEVENTSERVER_RPYTGENERATOR_H
#define LIBEVENTSERVER_RPYTGENERATOR_H

#include <stdlib.h>

int getRoll() { return rand() % 9000 - 4500; }
int getPitch() { return rand() % 9000 - 4500; }
int getYaw() { return rand() % 9000 - 4500; }

#endif //LIBEVENTSERVER_RPYTGENERATOR_H
