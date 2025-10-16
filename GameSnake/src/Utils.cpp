#include "Utils.h"
#include <stdlib.h> // Para rand() e srand()
#include <time.h>   // Para time()

// Implementacao da funcao movida de Food.cpp/Snake.cpp
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
