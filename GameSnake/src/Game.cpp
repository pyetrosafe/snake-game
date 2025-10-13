#include "Game.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <ctime>
#include <cmath>
#include <limits.h>

static Game* currentGame = nullptr;

// --- Implementacao da Classe Game ---

int Game::getDificultyLevelDelay() {
    switch (difficultyLevel) {
        case 0: return DELAY_INITIAL; // Facil
        case 1: return DELAY_INITIAL * 0.8; // Medio
        case 2: return DELAY_INITIAL * 0.6; // Dificil
        default: return DELAY_INITIAL;
    }
}

void Game::setCurrentLevelDelay() {
    delay = getDificultyLevelDelay();
    delay = fase == 1 ? delay : (delay - ((int)floor(fase / 2)));
    if (logger.isEnabled() || endScreenTestEnabled) {
        delay *= 10;
    }
}

Game::Game() {
    currentGame = this;
    statusGame = OFF;
    currentScreen = SCREEN_MENU;
    delay = DELAY_INITIAL;
    fase = 1;
    lifes = LIFES;
    scorePoints = 0;
    difficultyLevel = 0;
    pause = false;
    loopCount = 0;
    startTime = endTime = clockTime = pauseTime = pausedTime = 0;
    pressedKey = 0;
    pressedSpecial = 0;
    showVolume = false;
    showVolumeTime = 0;
    maxLevelTestEnabled = false;
    endScreenTestEnabled = false;
}

Game::~Game() {}

void Game::init(int argc, char* argv[]) {
    // Processar argumentos de linha de comando
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-maxlevel") {
            maxLevelTestEnabled = true;
            std::cout << "DEBUG: Modo Teste de Ultima Fase ativado." << std::endl;
        } else if (arg == "-endscreen") {
            endScreenTestEnabled = true;
            std::cout << "DEBUG: Modo Teste de Tela Final ativado." << std::endl;
        } else if (arg == "-show") {
            showDebugInfo = true;
            std::cout << "DEBUG: Exibicao de debug na tela ativada." << std::endl;
        } else if (arg == "-storelog") {
            logger.setEnabled(true);
            std::cout << "DEBUG: Log de armazenamento ativado." << std::endl;
        }
        std::string msg = "Processando argumento: " + arg;
        logger.log(*this, msg.c_str());
    }

    logger.log(*this, "INICIO");

    glutInit(&argc, argv);
    soundManager.init();

    size_t width = (size_t)GetSystemMetrics(SM_CXSCREEN);
    size_t height = (size_t)GetSystemMetrics(SM_CYSCREEN);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(HRES, VRES);
    glutInitWindowPosition((width / 2) - (HRES / 2), (height / 2) - (VRES / 2));
    glutCreateWindow("Snake Game OOP");

    glutDisplayFunc(Game::drawCallback);
    glutKeyboardFunc(Game::keyboardCallback);
    glutSpecialFunc(Game::specialKeysCallback);
    glutReshapeFunc(Game::reshapeCallback);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    reset(SCREEN_MENU);
    glutTimerFunc(delay, Game::timerCallback, 1);
}

void Game::run() {
    glutMainLoop();
}

void Game::update() {
    logger.log(*this, "update_inicio");

    float t = ((clock() - endTime) / CLOCKS_PER_SEC);
    if (statusGame == OFF) {
        if ((currentScreen == SCREEN_LEVEL && t >= LOAD_TIME_SHORT -1) ||
            (currentScreen == SCREEN_HIT && t >= LOAD_TIME_LONG -1)) {
            logger.log(*this, "Contador_Telas_4_5");
            statusGame = ON;
            currentScreen = SCREEN_OFF;
            clockTime = clock();
            pausedTime = 0;
        }
        return;
    }

    logger.log(*this, "statusGame_ON");
    snake.move();

    int expectedTail = (fase <= 10) ? (fase * 2 - 2) : (fase * 2 - (fase - 8));
    expectedTail += 10;

    bool collided = snake.checkWallCollision(-(HRES/2), HRES/2, -(VRES/2), VRES/2) || snake.checkSelfCollision();

    if (collided) {
        logger.log(*this, "COLISAO");
        lifes--;
        soundManager.playHitSound();

        if (scorePoints > (lifes * 400) * expectedTail)
            scorePoints -= (lifes * 400) * expectedTail;
        else
            scorePoints = 0;

        if (lifes <= 0) {
            reset(SCREEN_GAMEOVER);
            soundManager.playLoseSound();
        } else {
            reset(SCREEN_HIT);
        }
        return;
    }

    if (snake.ateFood(food.getPosition())) {
        logger.log(*this, "PEGOU_COMIDA");
        food.spawn(-(HRES/2), HRES/2, -(VRES/2), VRES/2);
        snake.grow();
        soundManager.playEatSound();

        if ((clock() - clockTime) / CLOCKS_PER_SEC < 90) {
            scorePoints += (lifes * 400);
        } else if ((clock() - clockTime) / CLOCKS_PER_SEC < 180) {
            scorePoints += (lifes * 300);
        } else {
            scorePoints += (lifes * 150);
        }
    }

    if (snake.getTailLength() >= expectedTail) {
        if (fase < MAX_LEVEL) {
            logger.log(*this, "PASSOU_DE_FASE");
            reset(SCREEN_LEVEL);
        } else {
            logger.log(*this, "FINALIZOU_JOGO");
            reset(SCREEN_FINISHED);
            soundManager.playWinSound();
        }
    }
}

void Game::draw() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    ui.drawBackground();

    if (statusGame == ON) {
        snake.draw();
        food.draw();

        float speed = (float)getDificultyLevelDelay() / delay;
        float time = ((clock() - clockTime) / CLOCKS_PER_SEC) - pausedTime;
        ui.drawGameInfo(fase, scorePoints, lifes, snake.getTailLength(), speed, time);
    }

    if (showVolume && (clock() - showVolumeTime) / CLOCKS_PER_SEC <= 3) {
        ui.drawVolume(soundManager.getMasterVolume());
    } else {
        showVolume = false;
    }

    if (showDebugInfo) {
        Position snakePos = snake.getHeadPosition();
        Position foodPos = food.getPosition();
        ui.drawDebugInfo(snakePos.x, snakePos.y, foodPos.x, foodPos.y, currentScreen, statusGame, pressedKey, pressedSpecial);
    }

    switch (currentScreen) {
        case SCREEN_MENU: ui.drawMenu(difficultyLevel); break;
        case SCREEN_EXIT: ui.drawExitScreen(); break;
        case SCREEN_PAUSE: ui.drawPauseScreen(); break;
        case SCREEN_LEVEL: ui.drawLevelScreen(fase); break;
        case SCREEN_HIT: ui.drawHitScreen(lifes, ((clock() - endTime) / CLOCKS_PER_SEC)); break;
        case SCREEN_GAMEOVER: ui.drawGameOverScreen(); break;
        case SCREEN_FINISHED: ui.drawFinishedScreen(); break;
    }

    glutSwapBuffers();
}

void Game::setupEndScreenTest() {
    logger.log(*this, "ResetValuesEndScreen_inicio");
    fase = MAX_LEVEL;
    int tail = (fase <= 10) ? (fase * 2 - 2) : (fase * 2 - (fase - 8));
    tail += 10;
    tail--;

    snake.setPosition(-50, 20, tail);
    snake.setDirection(1, 0, 0, 0);
    food.setPosition(50, 20);
    logger.log(*this, "ResetValuesEndScreen_fim");
}

void Game::reset(int nextScreen) {
    logger.log(*this, "ResetValues");
    statusGame = OFF;
    endTime = clock();
    currentScreen = nextScreen;
    loopCount = 0;

    if (nextScreen == SCREEN_MENU) {
        difficultyLevel = 0;
        scorePoints = 0;
        lifes = LIFES;
        fase = !maxLevelTestEnabled ? 0 : MAX_LEVEL-1;
        soundManager.stopAllSounds();
        soundManager.playMenuSound();
    } else if (nextScreen == SCREEN_LEVEL) {
        soundManager.stopAllSounds();
        fase++;
    }
    setCurrentLevelDelay();

    snake.reset(nextScreen != SCREEN_GAMEOVER && nextScreen != SCREEN_FINISHED);

    if (maxLevelTestEnabled) {
        int tail = (fase <= 10 ? fase * 2 - 2 : fase * 2 - (fase - 8)) + 5;
        snake.setPosition(snake.getHeadPosition().x, snake.getHeadPosition().y, tail);
    }

    food.spawn(-(HRES/2), HRES/2, -(VRES/2), VRES/2);

    if (endScreenTestEnabled) {
        setupEndScreenTest();
    }
}

void Game::processInput(unsigned char key) {
    char msg[120];
    sprintf(msg, "Teclado [tecla == '%c'] (ASCII = '%d')", key, key);
    logger.log(*this, msg);

    switch (key) {
        case 13: if (statusGame == OFF && currentScreen == SCREEN_MENU) reset(SCREEN_LEVEL); break;
        case 'm': case 'M': reset(SCREEN_MENU); break;
        case 's': case 'S':
            if (currentScreen == SCREEN_EXIT) exit(0);
            else if (currentScreen != SCREEN_PAUSE && statusGame == OFF) currentScreen = SCREEN_EXIT;
            break;
        case 'p':
        case 'P':
            if (!pause && statusGame == ON) {
                statusGame = OFF;
                currentScreen = SCREEN_PAUSE;
                pause = true;
                pauseTime = clock();
            } else if (pause && statusGame == OFF) {
                currentScreen = SCREEN_OFF;
                statusGame = ON;
                pause = false;
                pausedTime += (float)(clock() - pauseTime) / CLOCKS_PER_SEC;
            }
            break;
        case 'i': case 'I': ui.toggleInformations(); break;
        case '+': case '=':
            soundManager.setMasterVolume(soundManager.getMasterVolume() + 6);
            showVolume = true;
            showVolumeTime = clock();
            break;
        case '-':
            soundManager.setMasterVolume(soundManager.getMasterVolume() - 6);
            showVolume = true;
            showVolumeTime = clock();
            break;
    }
}


void Game::processSpecialKeys(int key) {
    char msg[120];
    sprintf(msg, "TeclasEspeciais [tecla == %i]", key);
    logger.log(*this, msg);

    if (statusGame == ON) {
        switch (key) {
            case GLUT_KEY_UP:    snake.changeDirection(-1, 0); break;
            case GLUT_KEY_DOWN:  snake.changeDirection(-1, 1); break;
            case GLUT_KEY_LEFT:  snake.changeDirection(1, -1); break;
            case GLUT_KEY_RIGHT: snake.changeDirection(0, -1); break;
        }
    } else if (currentScreen == SCREEN_MENU) {
        switch (key) {
            case GLUT_KEY_UP:
                difficultyLevel = (difficultyLevel > 0) ? difficultyLevel - 1 : 2;
                break;
            case GLUT_KEY_DOWN:
                difficultyLevel = (difficultyLevel < 2) ? difficultyLevel + 1 : 0;
                break;
        }
    }
}

// --- Callbacks Estaticos ---
void Game::drawCallback() { if (currentGame) currentGame->draw(); }
void Game::timerCallback(int value) {
    if (currentGame) {
        if (currentGame->logger.isEnabled()) {
            currentGame->loopCount = (currentGame->loopCount >= UINT_MAX - 1) ? 0 : currentGame->loopCount + 1;
        }
        currentGame->update();
        glutPostRedisplay();
        glutTimerFunc(currentGame->delay, Game::timerCallback, 1);
    }
}
void Game::keyboardCallback(unsigned char key, int x, int y) { if (currentGame) currentGame->processInput(key); }
void Game::specialKeysCallback(int key, int x, int y) { if (currentGame) currentGame->processSpecialKeys(key); }
void Game::reshapeCallback(int w, int h) {
    if (h == 0) h = 1;
    if (w < HRES || h < VRES) { glutReshapeWindow(HRES, VRES); }
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-w / 2, w / 2, -h / 2, h / 2);
    glMatrixMode(GL_MODELVIEW);
}