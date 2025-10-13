#ifndef SNAKE_H
#define SNAKE_H

#include <GL/glut.h>
#include "Food.h" // Para a struct Position

#define MAX_TAIL 48
#define PIXEL_INC 10

class StoreLogger; // Forward declaration

class Snake {
public:
    Snake();
    void draw();
    void move();
    void grow();
    void reset(bool randomPos);

    // Metodos de controle e estado
    void changeDirection(int h, int v);
    bool checkSelfCollision();
    bool checkWallCollision(int minX, int maxX, int minY, int maxY);
    bool ateFood(Position foodPos);

    Position getHeadPosition() const;
    int getTailLength() const;
    bool isMovingX();
    bool isMovingY();

    // Setters for testing
    void setPosition(int x, int y, int tail);
    void setDirection(int mx, int my, int hk, int vk);

private:
    friend class StoreLogger; // Permite acesso para o logger

    GLint body[MAX_TAIL + 1][2];
    int tailLength;

    // Direcao atual
    int moveToX; // 1 para mover no eixo X, 0 para Y
    int moveToY; // 1 para mover no eixo Y, 0 para X

    // "Sugestao" de direcao vinda das teclas
    int hKey; // 0 para direita, 1 para esquerda
    int vKey; // 0 para cima, 1 para baixo
};

#endif // SNAKE_H
