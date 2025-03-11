#ifndef PONG_H
#define PONG_H

#include <gtk/gtk.h>

typedef struct {
    int x; 
    int y;
} Position;

typedef struct {
    int width;
    int height;
} Size;

typedef struct {
    int number;
    Position pos;
    Size size;    
} Player;

typedef struct {
    Position pos;
    int radius;
} Ball;

#endif