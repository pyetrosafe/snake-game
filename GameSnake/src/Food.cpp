#include "Food.h"
#include "Game.h" // Para HRES, VRES
#include <stdlib.h> // Para rand() e srand()
#include <time.h>   // Para time()

// Funcao auxiliar (duplicada em Snake.cpp, idealmente mover para um helper)
int getRandom(int min, int max) {
    static bool first = true;
    if (first) {
        time_t seed;
        time(&seed);
        srand((unsigned)seed);
        first = false;
    }
    int r = min + rand() % (max - min + 1);
    if (r % 10 != 0) {
        r /= 10;
        r *= 10;
    }
    return r;
}

Food::Food() {
    blinkInterval = 0;
    position.x = 0;
    position.y = 0;
}

void Food::draw() {
    glColor3f(1.0f, 1.0f, 0.7f);
    blinkInterval++;
    if (blinkInterval >= 99) blinkInterval = 0;

    if (blinkInterval % 10 != 0) {
        glPointSize(9.0f);
        glBegin(GL_POINTS);
        glVertex2f(position.x, position.y);
        glEnd();
    }
}

void Food::spawn(int minX, int maxX, int minY, int maxY) {
    position.x = getRandom(minX + 10, maxX - 10);
    position.y = getRandom(minY + 10, maxY - 10);
}

Position Food::getPosition() {
    return position;
}

void Food::setPosition(int x, int y) {
    position.x = x;
    position.y = y;
}