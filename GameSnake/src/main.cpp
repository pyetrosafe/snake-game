#include "Game.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "Iniciando a versao refatorada do Snake Game." << std::endl;

    // TODO: Processar argumentos de linha de comando (argc, argv)
    // e configurar o jogo de acordo.

    Game snakeGame;
    snakeGame.init(argc, argv);
    snakeGame.run();

    return 0;
}
