// Inicialmente trabalho de TCC feito em 2011
// Para universidade UNIMAR
// Autor: Pyetro S. F. da Costa
// Compilador/IDE: Dev-C++ 5.11 (Dev-Cpp 5.11 TDM-GCC 4.9.2 Setup.exe)
// Bibliotecas:
// GLUT - The OpenGL Utility Toolkit - Versção 3.7.6
// FMOD - FMOD Sound System Ex 3.75 - API 4.09.05

#include <fmod.h>
#include <gl/glut.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include <cmath>
#include <cstdio>   // Para usar fopen e fclose
#include <cstdlib>  // For atexit
#include <iostream>

// CONSTANTES
#define HRES 480      // Resolucao Inicial da Window na Horizontal
#define VRES 320      // Resolucao Inicial da Window na Vertical
#define PIXEL_INC 10  // Incremento em pixel para X e Y da Cobra
#define MAX_TAIL 48   // Define maximo de partes o corpo da cobra
#define ON 1          // define constante ON ativado
#define OFF 0         // define constante OFF desativado
#define LIFES 5
#define MAX_LEVEL 30
#define LOAD_TIME_LONG 5
#define LOAD_TIME_SHORT 3
#define DELAY_INITIAL 100

// Screens
#define SCREEN_OFF 0
#define SCREEN_MENU 1
#define SCREEN_EXIT 2
#define SCREEN_PAUSE 3
#define SCREEN_LEVEL 4
#define SCREEN_HIT 5
#define SCREEN_GAMEOVER 6
#define SCREEN_FINISHED 7

// For Debug Log
#define LOG_PATH "snake_execucao"

// Tamanho minimo e maximo em X e Y da Janela
int winXmin = -(HRES / 2), winXmax = (HRES / 2);
int winYmin = -(VRES / 2), winYmax = (VRES / 2);

// Estrutura pra guardar pontos da comida
struct Posicao {
    GLint x, y;
};

// Parâmetros da cobra
GLint snake[MAX_TAIL + 1][2];
GLint countTail = 0;

// Parâmetro comida
struct Posicao food;
GLint blinkInterval = 0;
GLint Vkey = 0, Hkey = 0;        // Fator que define a direção da cobra (vertical / horizontal)
GLint moveToX = 0, moveToY = 0;  // Indica se a cobra vai andar horizontal ou vertical

// Parâmetros de tempos
GLfloat start_time, end_time, clock_time, pause_time, paused_time, showsound_time = 0;

// Parâmetros de informações
GLint sizefont;

// Parâmetros de estado
GLint fase = 1;
GLint difficultyLevel = 0;
GLint lifes = 5;
GLint statusGame = OFF;
GLint currentScreen = SCREEN_MENU;
GLint delay = DELAY_INITIAL;
GLint pause = 0;
GLint informations = ON;
GLint showVolume = OFF;
GLint incVolume = 6;
GLfloat ScorePoints = 0;

GLuint loopCount = 0;  // Conta quantos loops de delay ja passaram

// Usado para mostrar tecla do teclado que foi pressionada
unsigned char pressedKey;
GLint pressedSpecial;

time_t seed;

// Parâmetros de Som
FSOUND_STREAM* somMenu;      // Som menu
FSOUND_SAMPLE* somComeu;     // Som quando come
FSOUND_SAMPLE* somChoque;    // Som choque
FSOUND_SAMPLE* somParabens;  // Som ganhou
FSOUND_SAMPLE* somPerdeu;    // Som perdeu

// Parâmetro para testar o ultimo level
bool endScreenTestEnabled = false;
bool maxLevelTestEnabled = false;

// Parâmetros para Debug
FILE* logFile;  // Arquivo de Log
bool storeLogEnabled = false;
bool showLogEnabled = false;

const std::string currentDateTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];

    tstruct = *localtime(&now);
    // Formato seguro para nome de arquivo: YYYY-MM-DD_HH-MM-SS
    strftime(buf, sizeof(buf), "%Y-%m-%d_%H-%M-%S", &tstruct);

    return std::string(buf);
}

void StoreDebugLog(const char* evento) {
    if (!storeLogEnabled)
        return;

    if (!logFile) {
        std::string filename = std::string(LOG_PATH) + "_" + currentDateTime() + ".log";
        logFile = fopen(filename.c_str(), "a");
        if (!logFile) return;
    }

    fprintf(logFile,
            "[%s]\nMAX_LEVEL=%d, MAX_TAIL=%d, fase=%d, lifes=%d, countTail=%d, currentScreen=%d, statusGame=%d, moveToX=%d, Hkey=%d, moveToY=%d, Vkey=%d, snake[0]=(%d,%d), food=(%d,%d), loopCount=%u\n\n",
            evento, MAX_LEVEL, MAX_TAIL, fase, lifes, countTail, currentScreen, statusGame, moveToX, Hkey, moveToY, Vkey, snake[0][0], snake[0][1], food.x, food.y, loopCount);
    fflush(logFile);
}

int getRandom(int min, int max) {
    int r;

    if (!seed) {
        // Pega a hora atual e utiliza como semente do gerador de nros.
        time(&seed);
        srand((unsigned)seed);
    }

    r = min + rand() % (max - min + 1);

    if (r % 10 != 0) {
        r /= 10;
        r *= 10;
    }

    return r;
}

// Retorna o delay conforme o nivel de dificuldade
int getDificultyLevelDelay() {
    switch (difficultyLevel) {
        case 0:
            return DELAY_INITIAL;  // Facil
        case 1:
            return DELAY_INITIAL * 0.8;  // Medio
        case 2:
            return DELAY_INITIAL * 0.6;  // Dificil
        default:
            return DELAY_INITIAL;  // Facil
    }
}

int setCurrentLevelDelay() {
    delay = getDificultyLevelDelay();
    delay = fase == 1 ? delay : (delay - ((int)floor(fase / 2)));
    if (showLogEnabled || storeLogEnabled || endScreenTestEnabled)
        delay *= 10;
}

//  Função que recebe como parametros: font, texto, coord X e Y e cores em RGB
//  Exibe na tela usando caracteres bitmap
// void ImprimeTexto(void *font, char *string, int x = 0, int y = 0, float red = 1.0, float green = 1.0, float blue = 1.0) { // modo antigo
void ImprimeTexto(char* string, int x = 0, int y = 0, float red = 1.0, float green = 1.0, float blue = 1.0) {
    glColor3f(red, green, blue);
    glRasterPos2f(x, y);

    //  Exibe caractere a caractere
    while (*string) {
        if (sizefont == 10)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *string++);
        if (sizefont == 20)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *string++);
    }
}

// Desenha as barras
void DesenhaTela(bool showBars = false) {
    GLint widthBar = 5;

    // Fundo Tela Grande
    // glColor3f(.1f, .1f, .1f);
    glColor3f(.0f, .15f, .2f);

    glBegin(GL_QUADS);
    glVertex2f(winXmin, winYmin);
    glVertex2f(winXmin, winYmax);
    glVertex2f(winXmax, winYmax);
    glVertex2f(winXmax, winYmin);
    glEnd();

    // Barras Laterais
    glColor3f(1.0f, 1.0f, 1.0f);

    // Barra horizontal superior
    glBegin(GL_QUADS);
    glVertex2f(winXmin, winYmax - widthBar);
    glVertex2f(winXmin, winYmax);
    glVertex2f(winXmax, winYmax);
    glVertex2f(winXmax, winYmax - widthBar);
    glEnd();

    // Barra horizontal inferior
    glBegin(GL_QUADS);
    glVertex2f(winXmin, winYmin);
    glVertex2f(winXmin, winYmin + widthBar);
    glVertex2f(winXmax, winYmin + widthBar);
    glVertex2f(winXmax, winYmin);
    glEnd();

    // Barra vertical esquerda
    glBegin(GL_QUADS);
    glVertex2f(winXmin, winYmin);
    glVertex2f(winXmin, winYmax);
    glVertex2f(winXmin + widthBar, winYmax);
    glVertex2f(winXmin + widthBar, winYmin);
    glEnd();

    // Barra vertical direita
    glBegin(GL_QUADS);
    glVertex2f(winXmax - widthBar, winYmin);
    glVertex2f(winXmax - widthBar, winYmax);
    glVertex2f(winXmax, winYmax);
    glVertex2f(winXmax, winYmin);
    glEnd();

    // Caso false, não desenha as linhas internas
    if (!showBars)
        return;

    // Linhas abaixo para analise de correção de valores de posição da cobra e dos textos
    int i = 0;
    widthBar++;

    // Desenha GRADE 10px x 10px
    i = -(HRES / 2) + widthBar;
    while (i < (HRES / 2)) {
        glBegin(GL_LINES);

        if (i == 0)
            glColor3f(1, 1, 1);
        else if ((i > -90) && (i < 90))
            glColor3f(1, 1, 0);
        else
            glColor3f(1, 0, 0);

        glVertex3f(i, -(VRES / 2) + widthBar, 0);
        glVertex3f(i, VRES / 2 - widthBar, 0);
        glEnd();
        i += 10;
    }

    // Desenha Grade 9 pieces
    i = (((HRES / 2) - (HRES / 3)) / 10) * 10;
    glBegin(GL_LINES);
    glColor3f(0, 1, 0);
    glVertex2f(i, -VRES / 2 + widthBar);
    glVertex2f(i, VRES / 2 - widthBar);

    glVertex2f(-i, -VRES / 2 + widthBar);
    glVertex2f(-i, VRES / 2 - widthBar);

    i = (((VRES / 2) - (VRES / 3)) / 10) * 10;
    glVertex2f(-HRES / 2 + widthBar, i);
    glVertex2f(HRES / 2 - widthBar, i);

    glVertex2f(-HRES / 2 + widthBar, -i);
    glVertex2f(HRES / 2 - widthBar, -i);
    glEnd();
}

void DesenhaTextos() {
    char msg[120];

    // Mensagens na tela!!!
    sizefont = 20;

    // currentScreen SCREEN_MENU - Menu Principal
    if (currentScreen == SCREEN_MENU) {
        sprintf(msg, "\\_/\\_Cobra Game_/\\_/");
        ImprimeTexto(msg, -110, VRES / 3, 1.0f, 0.4f, 0.4f);

        // Desenha flecha dificuldade
        sprintf(msg, ">>");
        if (difficultyLevel == 0) {
            ImprimeTexto(msg, -80, (VRES / 5) + 10, 0.3f, 1.0f, 0.5f);
        } else if (difficultyLevel == 1) {
            ImprimeTexto(msg, -80, (VRES / 5) - 15, 0.3f, 1.0f, 0.5f);
        } else {
            ImprimeTexto(msg, -80, (VRES / 5) - 40, 0.3f, 1.0f, 0.5f);
        }

        sprintf(msg, " Facil");
        ImprimeTexto(msg, -50, (VRES / 5) + 10);
        sprintf(msg, " Medio ");
        ImprimeTexto(msg, -50, (VRES / 5) - 15);
        sprintf(msg, " Dificil ");
        ImprimeTexto(msg, -50, (VRES / 5) - 40);

        sprintf(msg, "Comandos:");
        ImprimeTexto(msg, -60, -10);

        sprintf(msg, "'M'");
        ImprimeTexto(msg, -100, -40);
        sprintf(msg, "= Menu Principal");
        ImprimeTexto(msg, -50, -40);

        sprintf(msg, "'I'");
        ImprimeTexto(msg, -100, -60);
        sprintf(msg, "= Mostrar Informacoes");
        ImprimeTexto(msg, -50, -60);

        sprintf(msg, "'P'");
        ImprimeTexto(msg, -100, -80);
        sprintf(msg, "= Pause");
        ImprimeTexto(msg, -50, -80);

        sprintf(msg, "'S'");
        ImprimeTexto(msg, -100, -100);
        sprintf(msg, "= Sair");
        ImprimeTexto(msg, -50, -100);

        sprintf(msg, "'+/-'");
        ImprimeTexto(msg, -100, -120);
        sprintf(msg, "= Volume");
        ImprimeTexto(msg, -50, -120);
    }

    // currentScreen SCREEN_EXIT - Tela Sair
    if (currentScreen == SCREEN_EXIT) {
        sprintf(msg, "Para Sair do jogo pressione 'S'");
        ImprimeTexto(msg, -150, (VRES / 4));
        sprintf(msg, "Para voltar ao Menu Principal Pressione 'M'");
        ImprimeTexto(msg, -220, (VRES / 4) - 40);
    }

    // currentScreen SCREEN_PAUSE - Tela Pausa
    if (currentScreen == SCREEN_PAUSE) {
        sprintf(msg, "PAUSADO");
        ImprimeTexto(msg, -60, (VRES / 4));
        sprintf(msg, "Para voltar pressione 'P'");
        ImprimeTexto(msg, -110, (VRES / 4) - 40);
    }

    // currentScreen SCREEN_LEVEL - Tela Fase
    if (currentScreen == SCREEN_LEVEL) {
        sprintf(msg, "LEVEL %i", fase);
        ImprimeTexto(msg, -40, (VRES / 4));
    }

    // currentScreen SCREEN_HIT - Tela de Choque
    if (currentScreen == SCREEN_HIT) {
        sprintf(msg, "Restam: %i vidas", lifes);
        ImprimeTexto(msg, -75, (VRES / 4));
        sprintf(msg, "%0.f", (LOAD_TIME_LONG - (start_time - end_time) / CLOCKS_PER_SEC));
        ImprimeTexto(msg, -10, (VRES / 4) - 40);
    }

    // currentScreen SCREEN_GAMEOVER - Game Over
    if (currentScreen == SCREEN_GAMEOVER) {
        sprintf(msg, "VOCE PERDEU");
        ImprimeTexto(msg, -70, (VRES / 4));
        sprintf(msg, "Para voltar ao Menu Principal Pressione 'M'");
        ImprimeTexto(msg, -220, 50 - (VRES / 4));
        sprintf(msg, "Para Sair do jogo Pressione 'S'");
        ImprimeTexto(msg, -150, 20 - (VRES / 4));
    }

    // currentScreen SCREEN_FINISHED - Jogo Finalizado
    if (currentScreen == SCREEN_FINISHED) {
        sprintf(msg, "PARABENS, Voce Finalizou o Jogo!");
        ImprimeTexto(msg, -170, (VRES / 4));
        sprintf(msg, "Para voltar ao Menu Principal Pressione 'M'");
        ImprimeTexto(msg, -220, 50 - (VRES / 4));
        sprintf(msg, "Para Sair do jogo Pressione 'S'");
        ImprimeTexto(msg, -150, 20 - (VRES / 4));
    }
}

void DesenhaCobra() {
    int j;

    if (statusGame == ON) {
        // Desenha Cobra (HEAD)
        glColor3f(1.0f, 0.4f, 0.4f);
        glPointSize(9.0f);
        glBegin(GL_POINTS);
        glVertex2f(snake[0][0], snake[0][1]);
        glEnd();

        // Desenha o corpo da cobra
        if (countTail > 0) {
            for (j = 1; j <= countTail; j++) {
                glColor3f(1.0f, 0.7f, 0.7f);
                glPointSize(9.0f);
                glBegin(GL_POINTS);
                glVertex2f(snake[j][0], snake[j][1]);
                glEnd();
            }
        }

        // Desenha a comida
        glColor3f(1.0f, 1.0f, 0.7f);
        if (blinkInterval % 10 != 0) {
            glPointSize(9.0f);
            glBegin(GL_POINTS);
            glVertex2f(food.x, food.y);
            glEnd();
        }

        if (blinkInterval >= 99)
            blinkInterval = 0;
    }
}

void mostraTeclas() {
    if (!showLogEnabled)
        return;

    char msg[20];
    sizefont = 10;

    if (pressedKey) {
        sprintf(msg, "Key: %c", pressedKey);
    } else if (pressedSpecial) {
        strcpy(msg, "Special: ");

        switch (pressedSpecial) {
            case GLUT_KEY_F1:
                strcat(msg, "KEY_F1");
                break;
            case GLUT_KEY_F2:
                strcat(msg, "KEY_F2");
                break;
            case GLUT_KEY_F3:
                strcat(msg, "KEY_F3");
                break;
            case GLUT_KEY_F4:
                strcat(msg, "KEY_F4");
                break;
            case GLUT_KEY_F5:
                strcat(msg, "KEY_F5");
                break;
            case GLUT_KEY_F6:
                strcat(msg, "KEY_F6");
                break;
            case GLUT_KEY_F7:
                strcat(msg, "KEY_F7");
                break;
            case GLUT_KEY_F8:
                strcat(msg, "KEY_F8");
                break;
            case GLUT_KEY_F9:
                strcat(msg, "KEY_F9");
                break;
            case GLUT_KEY_F10:
                strcat(msg, "KEY_F10");
                break;
            case GLUT_KEY_F11:
                strcat(msg, "KEY_F11");
                break;
            case GLUT_KEY_F12:
                strcat(msg, "KEY_F12");
                break;
            /* directional keys */
            case GLUT_KEY_LEFT:
                strcat(msg, "KEY_LEFT");
                break;
            case GLUT_KEY_UP:
                strcat(msg, "KEY_UP");
                break;
            case GLUT_KEY_RIGHT:
                strcat(msg, "KEY_RIGHT");
                break;
            case GLUT_KEY_DOWN:
                strcat(msg, "KEY_DOWN");
                break;
            case GLUT_KEY_PAGE_UP:
                strcat(msg, "KEY_PAGE_UP");
                break;
            case GLUT_KEY_PAGE_DOWN:
                strcat(msg, "KEY_PAGE_DOWN");
                break;
            case GLUT_KEY_HOME:
                strcat(msg, "KEY_HOME");
                break;
            case GLUT_KEY_END:
                strcat(msg, "KEY_END");
                break;
            case GLUT_KEY_INSERT:
                strcat(msg, "KEY_INSERT");
                break;
        }
    }
    ImprimeTexto(msg, -(HRES / 2) + 20, (VRES / 2) - 220);
}

// Função Informations - Mostrar informações para o jogador
void Informations() {
    char msg[120];
    sizefont = 10;
    float speed = (float)getDificultyLevelDelay() / delay;

    if ((statusGame == ON) && (informations == ON)) {
        sprintf(msg, "LEVEL: %i", fase);
        ImprimeTexto(msg, -(HRES / 2) + 40, (VRES / 2) - 20);
        sprintf(msg, "Pontos: %.0f", ScorePoints);
        ImprimeTexto(msg, -(HRES / 2) + 40, (VRES / 2) - 30);
        sprintf(msg, "Vidas: %i", lifes);
        ImprimeTexto(msg, (HRES / 2) - 90, (VRES / 2) - 20);
        sprintf(msg, "Tamanho: %i ", countTail);
        ImprimeTexto(msg, (HRES / 2) - 90, (VRES / 2) - 30);
        sprintf(msg, "Velocidade: %.2fx", speed);
        ImprimeTexto(msg, (HRES / 2) - 90, (VRES / 2) - 40);
        sprintf(msg, "Tempo: %.0f", ((clock() - clock_time) / CLOCKS_PER_SEC) - paused_time);
        ImprimeTexto(msg, (HRES / 2) - 90, (VRES / 2) - 50);
    }
}

// Exibe informações de variáveis na tela para analise (quando sendo desenvolvido)
void Debug() {
    if (!showLogEnabled)
        return;

    char msg[120];
    sizefont = 10;

    sprintf(msg, "Snake(x, y): (%i, %i)", snake[0][0], snake[0][1]);
    ImprimeTexto(msg, -(HRES / 2) + 20, (VRES / 2) - 250);
    sprintf(msg, "Food(x, y): (%i, %i)", food.x, food.y);
    ImprimeTexto(msg, -(HRES / 2) + 20, (VRES / 2) - 260);
    sprintf(msg, "CurrentScreen: %i", currentScreen);
    ImprimeTexto(msg, -(HRES / 2) + 20, (VRES / 2) - 270);
    sprintf(msg, "StatusGame: %i", statusGame);
    ImprimeTexto(msg, -(HRES / 2) + 20, (VRES / 2) - 280);
    sprintf(msg, "winXmax/3: %i", (winXmax / 3));
    ImprimeTexto(msg, -(HRES / 2) + 20, (VRES / 2) - 290);
    sprintf(msg, "winXmax-10: %i", winXmax - 10);
    ImprimeTexto(msg, -(HRES / 2) + 20, (VRES / 2) - 300);
    sprintf(msg, "loopCount: %u", loopCount);
    ImprimeTexto(msg, -(HRES / 2) + 20, (VRES / 2) - 310);
}

void MostraVolume() {
    char msg[120];
    float sound_time = ((clock() - showsound_time) / CLOCKS_PER_SEC);
    if ((sound_time <= 3) && (showVolume == ON)) {
        sizefont = 10;
        sprintf(msg, "Volume: %i%%", (FSOUND_GetSFXMasterVolume() * 100 / 255));
        ImprimeTexto(msg, -35, (VRES / 2) - 20);
    } else {
        showsound_time = 0;
        showVolume = OFF;
    }
}

// Função callback chamada para fazer o desenho
void Desenha(void) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Limpa a janela de visualização com a cor de fundo especificada
    glClear(GL_COLOR_BUFFER_BIT);

    DesenhaTela();
    DesenhaCobra();
    DesenhaTextos();

    mostraTeclas();
    Informations();
    MostraVolume();
    Debug();

    // if (statusGame == ON)
    //     currentScreen = SCREEN_OFF;

    // Executa os comandos OpenGL
    glutSwapBuffers();
}

void ResetValuesEndScreen() {
    StoreDebugLog("ResetValuesEndScreen_inicio");

    // Calculo que determina tamanho do corpo a atingir no level
    fase = MAX_LEVEL;  // Para teste de ultimo level
    if (fase <= 10)
        countTail = fase * 2 - 2;
    else
        countTail = fase * 2 - (fase - 8);
    countTail += 10;
    countTail--;

    snake[0][0] = -50;
    snake[0][1] = 20;

    food.x = 50;
    food.y = 20;

    // Define a direção para começar andar
    moveToX = 1;
    moveToY = 0;

    Hkey = 0;
    Vkey = 0;

    StoreDebugLog("ResetValuesEndScreen_fim");
}

// Limpa variáveis
void ResetValues(int nextScreen = SCREEN_OFF) {
    StoreDebugLog("ResetValues");

    int i;

    fflush(stdin);
    statusGame = OFF;
    end_time = start_time = clock();
    countTail = 0;
    loopCount = 0;

    if (nextScreen == SCREEN_MENU) {
        currentScreen = SCREEN_MENU;
        difficultyLevel = 0;
        ScorePoints = 0;
        lifes = LIFES;
        // fase recomeça com 1 ou com o maximo (para teste)
        fase = !maxLevelTestEnabled ? 0 : MAX_LEVEL;
        delay = setCurrentLevelDelay();
        FSOUND_StopSound(FSOUND_ALL);
        FSOUND_Stream_Play(0, somMenu);
    } else if (nextScreen == SCREEN_HIT) {
        currentScreen = SCREEN_HIT;
    } else if (nextScreen == SCREEN_GAMEOVER) {
        currentScreen = SCREEN_GAMEOVER;
        return;
    } else if (nextScreen == SCREEN_LEVEL) {
        currentScreen = SCREEN_LEVEL;
        fase++;
        // delay--;
        setCurrentLevelDelay();
    } else if (nextScreen == SCREEN_FINISHED) {
        currentScreen = SCREEN_FINISHED;
    }

    // countTail recomeça com 0 ou com o tamanho maximo do level (para teste)
    if (maxLevelTestEnabled)
        countTail = (fase <= 10 ? fase * 2 - 2 : fase * 2 - (fase - 8)) + 5;

    // Cobra começa nos cantos da tela
    snake[0][0] = getRandom((winXmax / 3) + 10, winXmax - 10);
    snake[0][1] = getRandom((winYmax / 3) + 10, winYmax - 10);
    food.x = getRandom(winXmin + 10, winXmax - 10);
    food.y = getRandom(winYmin + 10, winYmax - 10);

    // Define qual canto nascer!
    snake[0][0] *= getRandom(-HRES, HRES) < 0 ? -1 : 1;
    snake[0][1] *= getRandom(-VRES, VRES) < 0 ? -1 : 1;

    // Define a direção para começar andar
    Hkey = snake[0][0] > 0 ? 1 : 0;
    Vkey = snake[0][1] > 0 ? 1 : 0;

    if (getRandom(-HRES, HRES) < 0) {
        moveToX = 1;
        moveToY = 0;
    } else {
        moveToX = 0;
        moveToY = 1;
    }

    // Rastros recomeçam com X Y da snake[0]
    for (i = 1; i <= MAX_TAIL; i++) {
        snake[i][0] = 100000;  // Coord x
        snake[i][1] = 100000;  // Coord y
    }

    // Para teste de ultimo level
    if (endScreenTestEnabled)
        ResetValuesEndScreen();
}

void engineSnake() {
    StoreDebugLog("engineSnake_inicio");

    int j, expectedTail;
    float t = 0;
    bool colidiu = false;

    // Incrementa variavel pra piscar a comida!
    blinkInterval++;

    // Atualiza o tempo jogado e de espera
    start_time = clock();
    t = ((start_time - end_time) / CLOCKS_PER_SEC);

    // Contador para tela 4 e 5 (Level e Choque)
    if (statusGame == OFF && (currentScreen == SCREEN_LEVEL || currentScreen == SCREEN_HIT)) {
        StoreDebugLog("Contador_Telas_4_5");
        // Para a execução da musica inicial (id 2)
        FSOUND_Stream_Stop(somMenu);
        if ((currentScreen == SCREEN_HIT && (t >= LOAD_TIME_LONG - 1)) || (currentScreen == SCREEN_LEVEL && (t >= LOAD_TIME_SHORT - 1))) {
            statusGame = ON;
            // Atualiza o tempo jogado e de espera
            clock_time = clock();
            pause_time = paused_time = 0;
            currentScreen = SCREEN_OFF;
            return;
        }
    }

    if (statusGame == ON) {
        StoreDebugLog("statusGame_ON");

        // Calculo que determina tamanho do corpo a atingir no level
        if (fase <= 10)
            expectedTail = fase * 2 - 2;
        else
            expectedTail = fase * 2 - (fase - 8);
        expectedTail += 10;

        // Verifica se a bola bateu nas barras
        if ((snake[0][0] >= winXmax - 5) || (snake[0][0] <= winXmin + 5) || (snake[0][1] >= winYmax - 5) || (snake[0][1] <= winYmin + 5)) {
            colidiu = true;
        }

        // Perde se bater no próprio corpo
        for (j = 1; j <= countTail; j++) {
            if ((snake[0][0] == snake[j][0]) && (snake[0][1] == snake[j][1])) {
                colidiu = true;
                break;
            }
        }

        // Se colidiu, perde uma vida
        if (colidiu) {
            StoreDebugLog("COLISAO");
            lifes--;

            if (ScorePoints > (lifes * 400) * expectedTail)
                ScorePoints -= (lifes * 400) * expectedTail;
            else
                ScorePoints = 0;

            // Tela GameOver
            if (lifes <= 0) {
                ResetValues(SCREEN_GAMEOVER);
                FSOUND_PlaySound(5, somPerdeu);
                return;
            }
            // Tela Choque
            else {
                ResetValues(SCREEN_HIT);
                FSOUND_PlaySound(3, somChoque);
                return;
            }
            return;
        }

        // Muda Level (fase) se objetivo, rastros, estão completos !
        // Verifica se tamanho do corpo igual tamanho a ser completado
        // Passou de Fase
        if ((countTail == expectedTail) && (fase < MAX_LEVEL)) {
            StoreDebugLog("PASSOU_DE_FASE");
            ResetValues(SCREEN_LEVEL);
            return;
        }
        // Finalizou o jogo
        else if ((countTail == expectedTail) && (fase >= MAX_LEVEL)) {
            StoreDebugLog("FINALIZOU_JOGO");
            ResetValues(SCREEN_FINISHED);
            FSOUND_PlaySound(4, somParabens);
            return;
        }

        // Verifica se Head relou na comida e incrementa o corpo
        if ((snake[0][0] == food.x) && (snake[0][1] == food.y)) {
            StoreDebugLog("PEGOU_COMIDA");
            food.x = getRandom(winXmin + 10, winXmax - 10);
            food.y = getRandom(winYmin + 10, winYmax - 10);

            FSOUND_PlaySound(1, somComeu);
            if ((clock() - clock_time) / CLOCKS_PER_SEC < 90) {
                ScorePoints += (lifes * 400);
            } else if ((clock() - clock_time) / CLOCKS_PER_SEC < 180) {
                ScorePoints += (lifes * 300);
            } else
                ScorePoints += (lifes * 150);

            if (countTail < MAX_TAIL)
                countTail++;
        }

        // Guarda coordenadas do corpo da cobra
        for (j = countTail; j > 0; j--) {
            snake[j][0] = snake[j - 1][0];  // Coord x
            snake[j][1] = snake[j - 1][1];  // Coord y
        }

        // Move a cobra (Head[0])
        if (moveToX == 1) {
            if (Hkey == 0)
                snake[0][0] += PIXEL_INC;

            if (Hkey == 1)
                snake[0][0] -= PIXEL_INC;
        }

        if (moveToY == 1) {
            if (Vkey == 0)
                snake[0][1] += PIXEL_INC;

            if (Vkey == 1)
                snake[0][1] -= PIXEL_INC;
        }

        // FSOUND_PlaySound (0,somAndou); // Som da cobra andando
    }
}

// Função callback chamada pela GLUT a cada intervalo de tempo
// (a window não está sendo redimensionada ou movida)
void Timer(int value) {
    engineSnake();

    // Incrementa o loopCount e zera se chegar no valor máximo
    if (showLogEnabled || storeLogEnabled)
        loopCount = (loopCount >= UINT_MAX - 1) ? 0 : loopCount + 1;

    // Redesenha o quadrado com as novas coordenadas
    glutPostRedisplay();
    glutTimerFunc(delay, Timer, 1);
}

// Teclado(): funcao chamada sempre que for pressionada uma tecla ASCII
void Teclado(unsigned char tecla, int x, int y) {
    char msg[120];
    sprintf(msg, "Teclado [tecla == '%c'] (ASCII = '%d')", tecla, tecla);
    StoreDebugLog(msg);

    pressedKey = tecla;
    pressedSpecial = 0;

    int volume = FSOUND_GetSFXMasterVolume();

    if (tecla == 13) {
        if (statusGame == OFF && currentScreen == SCREEN_MENU) {
            ResetValues(SCREEN_LEVEL);
            return;
        }
    }

    if (tecla == 'm' || tecla == 'M')
        ResetValues(SCREEN_MENU);

    if (tecla == 's' || tecla == 'S') {
        if (statusGame == OFF && currentScreen == SCREEN_EXIT)
            exit(0);
        else if (statusGame == OFF && (currentScreen == SCREEN_MENU || currentScreen == SCREEN_GAMEOVER || currentScreen == SCREEN_FINISHED))
            currentScreen = SCREEN_EXIT;
    }

    if (tecla == 'p' || tecla == 'P') {
        if (pause == 0 and statusGame == ON) {
            statusGame = OFF;
            currentScreen = SCREEN_PAUSE;
            pause = 1;
            pause_time = (clock() - clock_time) / CLOCKS_PER_SEC;
        } else if (pause == 1 and statusGame == OFF) {
            currentScreen = SCREEN_OFF;
            statusGame = ON;
            pause = 0;
            pause_time = ((clock() - clock_time) / CLOCKS_PER_SEC) - pause_time;
            paused_time += pause_time;
        }
    }

    if (tecla == 'i' || tecla == 'I') {
        if (informations == ON)
            informations = OFF;
        else
            informations = ON;
    }

    if (tecla == '+' || tecla == '=') {
        if ((volume + incVolume) > 255)
            FSOUND_SetSFXMasterVolume(255);
        else
            FSOUND_SetSFXMasterVolume(volume + incVolume);
        showsound_time = clock();
        showVolume = ON;
    }

    if (tecla == '-') {
        if ((volume - incVolume) < 0)
            FSOUND_SetSFXMasterVolume(0);
        else
            FSOUND_SetSFXMasterVolume(volume - incVolume);
        showsound_time = clock();
        showVolume = ON;
    }
}

// Funcao para coordenadar a cobra
void TeclasEspeciais(int tecla, int x, int y) {
    char msg[120];
    sprintf(msg, "TeclasEspeciais [tecla == %i]", tecla);
    StoreDebugLog(msg);

    pressedKey = false;
    pressedSpecial = tecla;

    switch (tecla) {
        // seta para cima
        case GLUT_KEY_UP:
            if (moveToX == 1 && statusGame == ON) {
                moveToX = 0;
                moveToY = 1;
                Vkey = 0;
            }
            if (statusGame == OFF && currentScreen == SCREEN_MENU) {
                if (difficultyLevel > 0)
                    difficultyLevel--;
                else
                    difficultyLevel = 2;
            }
            break;
        // seta para cima
        case GLUT_KEY_DOWN:
            if (moveToX == 1 && statusGame == ON) {
                moveToX = 0;
                moveToY = 1;
                Vkey = 1;
            }
            if (statusGame == OFF && currentScreen == SCREEN_MENU) {
                if (difficultyLevel < 2)
                    difficultyLevel++;
                else
                    difficultyLevel = 0;
            }
            break;
        // seta para esquerda
        case GLUT_KEY_LEFT:
            if (moveToY == 1 && statusGame == ON) {
                moveToX = 1;
                moveToY = 0;
                Hkey = 1;
            }
            break;
        // seta para direita
        case GLUT_KEY_RIGHT:
            if (moveToY == 1 && statusGame == ON) {
                moveToX = 1;
                moveToY = 0;
                Hkey = 0;
            }
            break;
    }
}

// Inicializa parâmetros de rendering
void Inicializa(void) {
    // Define a cor de fundo da janela de visualização como preta
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    somMenu = FSOUND_Stream_Open("../Sons/SomMenu.mp3", 0, 0, 0);
    FSOUND_Stream_SetMode(somMenu, FSOUND_LOOP_NORMAL);

    // somAndou=FSOUND_Sample_Load (0,"../Sons/SomAndou3.wav",0, 0, 0);
    // FSOUND_Sample_SetMode(somAndou, FSOUND_LOOP_OFF);

    somComeu = FSOUND_Sample_Load(1, "../Sons/SomComeu.wav", 0, 0, 0);
    FSOUND_Sample_SetMode(somComeu, FSOUND_LOOP_OFF);

    somChoque = FSOUND_Sample_Load(3, "../Sons/SomChoque2.wav", 0, 0, 0);
    FSOUND_Sample_SetMode(somChoque, FSOUND_LOOP_OFF);

    somParabens = FSOUND_Sample_Load(4, "../Sons/SomParabens1.mp3", 0, 0, 0);
    FSOUND_Sample_SetMode(somParabens, FSOUND_LOOP_OFF);

    somPerdeu = FSOUND_Sample_Load(5, "../Sons/SomPerdeu.wav", 0, 0, 0);
    FSOUND_Sample_SetMode(somPerdeu, FSOUND_LOOP_OFF);

    FSOUND_SetSFXMasterVolume(50);

    FSOUND_StopSound(FSOUND_ALL);
    FSOUND_Stream_Play(0, somMenu);

    ResetValues(SCREEN_MENU);
}

// Função callback chamada quando o tamanho da janela é alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h) {
    // Evita a divisao por zero
    if (h == 0)
        h = 1;

    // Dimensoes da Viewport
    if (w < HRES)
        glutReshapeWindow(HRES, VRES);
    if (h < VRES)
        glutReshapeWindow(HRES, VRES);

    // glViewport(x,y,HRES,VRES);
    glViewport(0, 0, w, h);

    // Inicia o sistema de coordenadas
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(-w / 2, w / 2, -h / 2, h / 2);
    glMatrixMode(GL_MODELVIEW);
}

// Função para limpar variáveis e fechar sistema de som FMOD
void cleanup_function() {
    FSOUND_StopSound(FSOUND_ALL);
    FSOUND_Close();

    if (logFile) {
        fclose(logFile);
        logFile = NULL;  // nullptr;
    }
}

// Programa Principal
int main(int argc, char* argv[]) {
    std::cout << "Iniciando programa com 'main'." << std::endl;

    // Register the cleanup function
    std::atexit(cleanup_function);

    // Itera sobre todos os argumentos passados, começando do índice 1 (pula o nome do executável)
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];  // Converte para std::string para facilitar a comparação

        if (arg == "-log") {
            storeLogEnabled = true;
            std::cout << "DEBUG: Log de armazenamento ativado." << std::endl;
        } else if (arg == "-show") {
            showLogEnabled = true;
            std::cout << "DEBUG: Exibição de debug na tela ativada." << std::endl;
        } else if (arg == "-maxlevel") {
            maxLevelTestEnabled = true;
            std::cout << "DEBUG: Modo Teste de Última Fase ativado." << std::endl;
        } else if (arg == "-endscreen") {
            endScreenTestEnabled = true;
            std::cout << "DEBUG: Modo Teste de Última Fase ativado." << std::endl;
        } else {
            std::cerr << "AVISO: Parâmetro desconhecido ignorado: " << arg << std::endl;
        }

        std::string msg = "Processando argumento: " + arg;
        StoreDebugLog(msg.c_str());
    }

    StoreDebugLog("INICIO");

    // Inicializa sistema de som da FMOD
    FSOUND_Init(44100, 32, 0);

    size_t width = (size_t)GetSystemMetrics(SM_CXSCREEN);
    size_t height = (size_t)GetSystemMetrics(SM_CYSCREEN);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(HRES, VRES);
    glutInitWindowPosition((width / 2) - (HRES / 2), (height / 2) - (VRES / 2));
    glutCreateWindow("Snake Game");
    glutKeyboardFunc(Teclado);
    glutSpecialFunc(TeclasEspeciais);
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    Inicializa();
    glutTimerFunc(delay, Timer, 1);
    glutMainLoop();
}
