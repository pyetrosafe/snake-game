#include "Snake.h"
#include "Game.h" // Para HRES, VRES
#include "..\..\inc\GL\glut.h" // Apenas para GLint
#include <stdlib.h> // Para rand

#include "Utils.h"

Snake::Snake() {
    reset(true);
}

void Snake::reset(bool randomPos) {
    tailLength = 0;

    if (randomPos) {
        // Logica de `ResetValues` para posicao aleatoria
        int winXmax = HRES / 2;
        int winYmax = VRES / 2;
        body[0][0] = getRandom((winXmax / 3) + 10, winXmax - 10);
        body[0][1] = getRandom((winYmax / 3) + 10, winYmax - 10);

        body[0][0] *= getRandom(-HRES, HRES) < 0 ? -1 : 1;
        body[0][1] *= getRandom(-VRES, VRES) < 0 ? -1 : 1;

        hKey = body[0][0] > 0 ? 1 : 0;
        vKey = body[0][1] > 0 ? 1 : 0;

        if (getRandom(-HRES, HRES) < 0) {
            moveToX = 1;
            moveToY = 0;
        } else {
            moveToX = 0;
            moveToY = 1;
        }
    } else {
        body[0][0] = 0;
        body[0][1] = 0;
        moveToX = 1;
        moveToY = 0;
        hKey = 0;
        vKey = 0;
    }

    for (int i = 1; i <= MAX_TAIL; i++) {
        body[i][0] = 100000;
        body[i][1] = 100000;
    }
}

void Snake::draw() {
    // Desenha a cabeca
    glColor3f(1.0f, 0.4f, 0.4f);
    glPointSize(9.0f);
    glBegin(GL_POINTS);
    glVertex2f(body[0][0], body[0][1]);
    glEnd();

    // Desenha o corpo
    if (tailLength > 0) {
        for (int j = 1; j <= tailLength; j++) {
            glColor3f(1.0f, 0.7f, 0.7f);
            glPointSize(9.0f);
            glBegin(GL_POINTS);
            glVertex2f(body[j][0], body[j][1]);
            glEnd();
        }
    }
}

void Snake::move() {
    // Guarda coordenadas do corpo da cobra
    for (int j = tailLength; j > 0; j--) {
        body[j][0] = body[j - 1][0];
        body[j][1] = body[j - 1][1];
    }

    // Move a cabeca
    if (moveToX == 1) {
        if (hKey == 0) body[0][0] += PIXEL_INC;
        if (hKey == 1) body[0][0] -= PIXEL_INC;
    }

    if (moveToY == 1) {
        if (vKey == 0) body[0][1] += PIXEL_INC;
        if (vKey == 1) body[0][1] -= PIXEL_INC;
    }
}

void Snake::grow() {
    if (tailLength < MAX_TAIL) {
        tailLength++;
    }
}

bool Snake::checkSelfCollision() {
    for (int j = 1; j <= tailLength; j++) {
        if ((body[0][0] == body[j][0]) && (body[0][1] == body[j][1])) {
            return true;
        }
    }
    return false;
}

bool Snake::checkWallCollision(int minX, int maxX, int minY, int maxY) {
    if ((body[0][0] >= maxX - 5) || (body[0][0] <= minX + 5) || 
        (body[0][1] >= maxY - 5) || (body[0][1] <= minY + 5)) {
        return true;
    }
    return false;
}

bool Snake::ateFood(Position foodPos) {
    return (body[0][0] == foodPos.x) && (body[0][1] == foodPos.y);
}

void Snake::changeDirection(int h, int v) {
    if (h != -1) { // Movimento horizontal
        if (moveToY == 1) { // So pode mudar se estiver se movendo na vertical
            moveToX = 1;
            moveToY = 0;
            hKey = h;
        }
    } else if (v != -1) { // Movimento vertical
        if (moveToX == 1) { // So pode mudar se estiver se movendo na horizontal
            moveToX = 0;
            moveToY = 1;
            vKey = v;
        }
    }
}

Position Snake::getHeadPosition() {
    Position pos = { body[0][0], body[0][1] };
    return pos;
}

int Snake::getTailLength() {
    return tailLength;
}

bool Snake::isMovingX() { return moveToX == 1; }
bool Snake::isMovingY() { return moveToY == 1; }

// Setters for testing
void Snake::setPosition(int x, int y, int tail) {
    body[0][0] = x;
    body[0][1] = y;
    tailLength = tail;
}

void Snake::setDirection(int mx, int my, int hk, int vk) {
    moveToX = mx;
    moveToY = my;
    hKey = hk;
    vKey = vk;
}