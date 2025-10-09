#include "UI.h"
#include "Game.h" // Para HRES e VRES
#include <GL/glut.h>
#include <stdio.h> // Para sprintf

UI::UI() {
    showInformations = true;
    sizefont = 20;
}

void UI::drawText(char* string, int x, int y, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    while (*string) {
        if (sizefont == 10) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *string++);
        else glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *string++);
    }
}

void UI::drawBorders() {
    GLint widthBar = 5;
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    glVertex2f(-(HRES / 2), (VRES / 2) - widthBar);
    glVertex2f(-(HRES / 2), (VRES / 2));
    glVertex2f((HRES / 2), (VRES / 2));
    glVertex2f((HRES / 2), (VRES / 2) - widthBar);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(-(HRES / 2), -(VRES / 2));
    glVertex2f(-(HRES / 2), -(VRES / 2) + widthBar);
    glVertex2f((HRES / 2), -(VRES / 2) + widthBar);
    glVertex2f((HRES / 2), -(VRES / 2));
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(-(HRES / 2), -(VRES / 2));
    glVertex2f(-(HRES / 2), (VRES / 2));
    glVertex2f(-(HRES / 2) + widthBar, (VRES / 2));
    glVertex2f(-(HRES / 2) + widthBar, -(VRES / 2));
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f((HRES / 2) - widthBar, -(VRES / 2));
    glVertex2f((HRES / 2) - widthBar, (VRES / 2));
    glVertex2f((HRES / 2), (VRES / 2));
    glVertex2f((HRES / 2), -(VRES / 2));
    glEnd();
}

void UI::drawBackground() {
    glColor3f(.0f, .15f, .2f);
    glBegin(GL_QUADS);
    glVertex2f(-(HRES / 2), -(VRES / 2));
    glVertex2f(-(HRES / 2), (VRES / 2));
    glVertex2f((HRES / 2), (VRES / 2));
    glVertex2f((HRES / 2), -(VRES / 2));
    glEnd();
    drawBorders();
}

void UI::drawMenu(int difficultyLevel) {
    char msg[120];
    sizefont = 20;

    sprintf(msg, "\_/\_Cobra Game_/\_/");
    drawText(msg, -110, VRES / 3, 1.0f, 0.4f, 0.4f);

    sprintf(msg, ">>");
    if (difficultyLevel == 0) drawText(msg, -80, (VRES / 5) + 10, 0.3f, 1.0f, 0.5f);
    else if (difficultyLevel == 1) drawText(msg, -80, (VRES / 5) - 15, 0.3f, 1.0f, 0.5f);
    else drawText(msg, -80, (VRES / 5) - 40, 0.3f, 1.0f, 0.5f);

    drawText(" Facil", -50, (VRES / 5) + 10);
    drawText(" Medio ", -50, (VRES / 5) - 15);
    drawText(" Dificil ", -50, (VRES / 5) - 40);

    drawText("Comandos:", -60, -10);
    drawText("'M'", -100, -40); drawText("= Menu Principal", -50, -40);
    drawText("'I'", -100, -60); drawText("= Mostrar Informacoes", -50, -60);
    drawText("'P'", -100, -80); drawText("= Pause", -50, -80);
    drawText("'S'", -100, -100); drawText("= Sair", -50, -100);
    drawText("'+/-'", -100, -120); drawText("= Volume", -50, -120);
}

void UI::drawExitScreen() {
    sizefont = 20;
    drawText("Para Sair do jogo pressione 'S'", -150, (VRES / 4));
    drawText("Para voltar ao Menu Principal Pressione 'M'", -220, (VRES / 4) - 40);
}

void UI::drawPauseScreen() {
    sizefont = 20;
    drawText("PAUSADO", -60, (VRES / 4));
    drawText("Para voltar pressione 'P'", -110, (VRES / 4) - 40);
}

void UI::drawLevelScreen(int level) {
    char msg[20];
    sizefont = 20;
    sprintf(msg, "LEVEL %i", level);
    drawText(msg, -40, (VRES / 4));
}

void UI::drawHitScreen(int lifes, float time) {
    char msg[20];
    sizefont = 20;
    sprintf(msg, "Restam: %i vidas", lifes);
    drawText(msg, -75, (VRES / 4));
    sprintf(msg, "%0.f", (LOAD_TIME_LONG - time));
    drawText(msg, -10, (VRES / 4) - 40);
}

void UI::drawGameOverScreen() {
    sizefont = 20;
    drawText("VOCE PERDEU", -70, (VRES / 4));
    drawText("Para voltar ao Menu Principal Pressione 'M'", -220, 50 - (VRES / 4));
    drawText("Para Sair do jogo Pressione 'S'", -150, 20 - (VRES / 4));
}

void UI::drawFinishedScreen() {
    sizefont = 20;
    drawText("PARABENS, Voce Finalizou o Jogo!", -170, (VRES / 4));
    drawText("Para voltar ao Menu Principal Pressione 'M'", -220, 50 - (VRES / 4));
    drawText("Para Sair do jogo Pressione 'S'", -150, 20 - (VRES / 4));
}

void UI::drawGameInfo(int level, float score, int lifes, int tailLength, float speed, float time) {
    if (!showInformations) return;
    char msg[120];
    sizefont = 10;

    sprintf(msg, "LEVEL: %i", level);
    drawText(msg, -(HRES / 2) + 40, (VRES / 2) - 20);
    sprintf(msg, "Pontos: %.0f", score);
    drawText(msg, -(HRES / 2) + 40, (VRES / 2) - 30);

    sprintf(msg, "Vidas: %i", lifes);
    drawText(msg, (HRES / 2) - 90, (VRES / 2) - 20);
    sprintf(msg, "Tamanho: %i ", tailLength);
    drawText(msg, (HRES / 2) - 90, (VRES / 2) - 30);
    sprintf(msg, "Velocidade: %.2fx", speed);
    drawText(msg, (HRES / 2) - 90, (VRES / 2) - 40);
    sprintf(msg, "Tempo: %.0f", time);
    drawText(msg, (HRES / 2) - 90, (VRES / 2) - 50);
}

void UI::drawDebugInfo(/* ... */) {}

void UI::drawVolume(int volume) {
    char msg[120];
    sizefont = 10;
    sprintf(msg, "Volume: %i%%", (volume * 100 / 255));
    drawText(msg, -35, (VRES / 2) - 20);
}

void UI::toggleInformations() {
    showInformations = !showInformations;
}