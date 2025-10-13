#ifndef FOOD_H
#define FOOD_H

#include <GL/glut.h>

struct Position {
    GLint x, y;
};

class Food {
public:
    Food();
    void draw();
    void spawn(int minX, int maxX, int minY, int maxY);
    Position getPosition() const;

    void setPosition(int x, int y);

private:
    Position position;
    int blinkInterval;
};

#endif // FOOD_H
