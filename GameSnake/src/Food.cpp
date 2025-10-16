#include "Food.h"
#include "Game.h" // Para HRES, VRES
#include <stdlib.h> // Para rand() e srand()
#include <time.h>   // Para time()

#include "Utils.h"

Food::Food() {
    animationTimer = 0.0f;
    colorIntensity = 0.0f;
    position.x = 0;
    position.y = 0;
}

void Food::update(float deltaTime) {
    animationTimer += deltaTime;

    const float riseTime = 0.3f;
    const float holdStrongTime = 0.2f;
    const float fallTime = 0.3f;
    const float holdNormalTime = 0.2f;
    const float cycleDuration = riseTime + holdStrongTime + fallTime + holdNormalTime; // 6 seconds total

    if (animationTimer > cycleDuration) {
        animationTimer -= cycleDuration;
    }

    if (animationTimer < riseTime) {
        // 0s -> 2s: Transition to strong
        colorIntensity = animationTimer / riseTime;
    } else if (animationTimer < riseTime + holdStrongTime) {
        // 2s -> 3s: Hold strong
        colorIntensity = 1.0f;
    } else if (animationTimer < riseTime + holdStrongTime + fallTime) {
        // 3s -> 5s: Transition to normal
        colorIntensity = 1.0f - ((animationTimer - (riseTime + holdStrongTime)) / fallTime);
    } else {
        // 5s -> 6s: Hold normal
        colorIntensity = 0.0f;
    }
    // Clamp value just in case of float inaccuracies
    colorIntensity = std::max(0.0f, std::min(1.0f, colorIntensity));
}

void Food::draw() {
    // Cores normal e forte (ajustado para melhor visibilidade)
    const float normal_r = 1.0f, normal_g = 1.0f, normal_b = 0.7f;
    const float strong_r = 1.0f, strong_g = 0.8f, strong_b = 0.1f;

    // Interpolar entre as cores com base na intensidade
    float r = normal_r + (strong_r - normal_r) * colorIntensity;
    float g = normal_g + (strong_g - normal_g) * colorIntensity;
    float b = normal_b + (strong_b - normal_b) * colorIntensity;

    glColor3f(r, g, b);

    // Tamanhos normal e maximo para o efeito de pulsar
    const float normalHalfSize = 4.5f;
    const float maxHalfSize = 5.0f; // Um pouco maior para "inchar"

    // Interpolar o tamanho usando a mesma intensidade da animacao de cor
    float currentHalfSize = normalHalfSize + (maxHalfSize - normalHalfSize) * colorIntensity;

    glBegin(GL_QUADS);
    glVertex2f(position.x - currentHalfSize, position.y - currentHalfSize);
    glVertex2f(position.x + currentHalfSize, position.y - currentHalfSize);
    glVertex2f(position.x + currentHalfSize, position.y + currentHalfSize);
    glVertex2f(position.x - currentHalfSize, position.y + currentHalfSize);
    glEnd();
}

void Food::spawn(int minX, int maxX, int minY, int maxY) {
    position.x = getRandom(minX + 10, maxX - 10);
    position.y = getRandom(minY + 10, maxY - 10);
}

Position Food::getPosition() const {
    return position;
}

void Food::setPosition(int x, int y) {
    position.x = x;
    position.y = y;
}