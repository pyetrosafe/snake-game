#ifndef GAME_H
#define GAME_H

#include <GL/glut.h>
#include "Snake.h"
#include "Food.h"
#include "SoundManager.h"
#include "UI.h"
#include "StoreLogger.h" // Adicionado para o logger

// CONSTANTES
#define HRES 480
#define VRES 320
#define PIXEL_INC 10
#define MAX_TAIL 48
#define ON 1
#define OFF 0
#define LIFES 5
#define MAX_LEVEL 30
#define LOAD_TIME_LONG 5
#define LOAD_TIME_SHORT 3
#define DELAY_INITIAL 100

// Telas
#define SCREEN_OFF 0
#define SCREEN_MENU 1
#define SCREEN_EXIT 2
#define SCREEN_PAUSE 3
#define SCREEN_LEVEL 4
#define SCREEN_HIT 5
#define SCREEN_GAMEOVER 6
#define SCREEN_FINISHED 7

class Game {
public:
    Game();
    ~Game();
    void init(int argc, char* argv[]);
    void run();

    // Funcoes de callback do GLUT
    static void drawCallback();
    static void timerCallback(int value);
    static void keyboardCallback(unsigned char key, int x, int y);
    static void specialKeysCallback(int key, int x, int y);
    static void reshapeCallback(int w, int h);

private:
    friend class StoreLogger; // Permite que o logger acesse membros privados

    void update();
    void draw();
    void reset(int nextScreen);
    void processInput(unsigned char key);
    void processSpecialKeys(int key);
    int getDificultyLevelDelay();
    void setCurrentLevelDelay();
    void setupEndScreenTest();

    // Game Objects
    Snake snake;
    Food food;
    SoundManager soundManager;
    UI ui;
    StoreLogger logger; // Objeto de log

    // Game State
    int statusGame;
    int currentScreen;
    int delay;
    int fase;
    int lifes;
    float scorePoints;
    int difficultyLevel;
    bool pause;
    unsigned int loopCount; // Contador de loops para o log

    // Time
    float startTime, endTime, clockTime, pauseTime, pausedTime;

    // Input
    unsigned char pressedKey;
    int pressedSpecial;
    bool showVolume;
    float showVolumeTime;

    // Test-mode flags
    bool maxLevelTestEnabled;
    bool endScreenTestEnabled;
    bool showDebugInfo;
};

#endif // GAME_H
