#include "StoreLogger.h"
#include "Game.h"
#include "Snake.h"
#include "Food.h"
#include <time.h>
#include <string>
#include <iostream>

// Implementacao copiada do Snake.cpp original
std::string StoreLogger::getCurrentDateTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    // Formato seguro para nome de arquivo: YYYY-MM-DD_HH-MM-SS
    strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &tstruct);
    return std::string(buf);
}

StoreLogger::StoreLogger() : logFile(nullptr), enabled(false) {
}

StoreLogger::~StoreLogger() {
    if (logFile) {
        log(*reinterpret_cast<const Game*>(0), "FIM"); // Log final pode ser problematico se Game ja foi destruido
        fflush(logFile);
        fclose(logFile);
        logFile = nullptr;
    }
}

void StoreLogger::setEnabled(bool en) {
    enabled = en;
}

bool StoreLogger::isEnabled() const {
    return enabled;
}

void StoreLogger::log(const Game& game, const char* event) {
    if (!enabled) {
        return;
    }

    if (!logFile) {
        std::string filename = "snake_execucao_" + getCurrentDateTime() + ".log";
        logFile = fopen(filename.c_str(), "a");
        if (!logFile) {
            std::cerr << "ERRO: Nao foi possivel criar o arquivo de log: " << filename << std::endl;
            enabled = false; // Desativa o log se nao conseguir criar o arquivo
            return;
        }
    }

    // Para o evento final, nao temos um objeto Game valido
    if (event == std::string("FIM")) {
         fprintf(logFile, "[%s]\n\n", event);
         fflush(logFile);
         return;
    }

    // Acesso aos membros privados de Game e Snake (requer 'friend class')
    Position snakePos = game.snake.getHeadPosition();
    Position foodPos = game.food.getPosition();

    fprintf(logFile,
            "[%s]\nMAX_LEVEL=%d, MAX_TAIL=%d, fase=%d, lifes=%d, countTail=%d, currentScreen=%d, statusGame=%d, snake[0]=(%d,%d), food=(%d,%d), loopCount=%u\n\n",
            event,
            MAX_LEVEL,
            MAX_TAIL,
            game.fase,
            game.lifes,
            game.snake.getTailLength(),
            game.currentScreen,
            game.statusGame,
            snakePos.x,
            snakePos.y,
            foodPos.x,
            foodPos.y,
            game.loopCount
    );
    fflush(logFile);
}
