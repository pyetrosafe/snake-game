#include "UI.h"
#include "Game.h" // Para HRES e VRES
#include <GL/glut.h>
#include <stdio.h> // Para sprintf
#define _USE_MATH_DEFINES // Para M_PI
#include <cmath> // Para fmax

UI::UI() {
    showInformations = true;
    sizefont = 20;
    windowWidth = HRES;
    windowHeight = VRES;
}

void UI::reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
}

float UI::getViewDimensionSize() {
    if (windowWidth > windowHeight)
        return (windowWidth / (float)HRES);
    else
        return (windowHeight / (float)VRES);
}

// Nova função para medir a largura do texto de traço
float UI::getStrokeStringWidth(const char* string, float scale) {
    float width = 0;
    for (const char* c = string; *c != '\0'; c++) {
        width += glutStrokeWidth(GLUT_STROKE_MONO_ROMAN, *c);
    }
    return width * (scale);
}

float UI::getTextVerticalPosition(float position) {
    float dinamicHeight = (windowHeight / (float)VRES);
    return (windowHeight * position / dinamicHeight);
}

float UI::getTextHorizontalPosition(float position) {
    float dinamicWidth = (windowWidth / (float)HRES);
    return (windowWidth * position / dinamicWidth);
}

// Nova implementação de drawText com fontes de traço
void UI::drawText(const char* string, float x, float y, float r, float g, float b, float scale) {
    glColor3f(r, g, b);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, scale);
    // A espessura da linha agora é proporcional à escala para manter a legibilidade.
    glLineWidth(fmax(1.0f, getViewDimensionSize()));
    for (const char* c = string; *c != '\0'; c++) {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *c);
    }
    glPopMatrix();
}

void UI::drawTextBitmap(const char* string, int x, int y, float r, float g, float b) {
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

void UI::drawRectangle(int x, int y, int width, int height, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x, y + height);
    glVertex2f(x + width, y + height);
    glVertex2f(x + width, y);
    glEnd();
    glFlush(); // Ensure all commands are executed
}

void UI::drawRectangleLine(int x, int y, int width, int height, float borderR, float borderG, float borderB, int borderWidth) {
    glColor3f(borderR, borderG, borderB);
    glLineWidth(borderWidth * (float) getViewDimensionSize());
    borderWidth /= 2 * (float) getViewDimensionSize();
    glBegin(GL_LINE_LOOP);
    glVertex2f(x - borderWidth, y - borderWidth); // Vertex Bottom Left
    glVertex2f(x + width + borderWidth, y - borderWidth); // Vertex Bottom Right
    glVertex2f(x + width + borderWidth, y + height + borderWidth); // Vertex Top Right
    glVertex2f(x - borderWidth, y + height + borderWidth); // Vertex Top Left
    glEnd();
    glFlush();
}

void UI::drawRoundedRectangle(float x, float y, float width, float height, float radius, int segments) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    // Top-left corner
    for (int i = 0; i <= segments; ++i) {
        float angle = M_PI + (M_PI_2 * i / segments);
        glVertex2f(x + radius + radius * cos(angle), y + height - radius + radius * sin(angle));
    }
    // // Top-right corner
    // for (int i = 0; i <= segments; ++i) {
    //     float angle = M_PI + (M_PI_2 * i / segments);
    //     glVertex2f(x + radius + radius * cos(angle), y + height - radius + radius * sin(angle));
    // }
    // // Bottom-left corner
    // for (int i = 0; i <= segments; ++i) {
    //     float angle = M_PI + (M_PI_2 * i / segments);
    //     glVertex2f(x + radius + radius * cos(angle), y + height - radius + radius * sin(angle));
    // }
    // // Bottom-right corner
    // for (int i = 0; i <= segments; ++i) {
    //     float angle = M_PI + (M_PI_2 * i / segments);
    //     glVertex2f(x + radius + radius * cos(angle), y + height - radius + radius * sin(angle));
    // }
    glEnd();
}

void UI::drawMenu(int difficultyLevel) {
    char msg[120];

    // O fator de escala pode ser ajustado para um tamanho de fonte desejado
    float titleScale = 0.15f;
    float optionScale = 0.1f;
    float commandScale = 0.08f;
    float yPos = 0;
    float xPos = 0;

    // Tí­tulo
    sprintf(msg, "\\_/\\Snake Game_/\\_/");
    float titleWidth = getStrokeStringWidth(msg, titleScale);
    drawText(msg, -(titleWidth / 2), getTextVerticalPosition(0.3f), 1.0f, 0.4f, 0.4f, titleScale);

    // Alinha a maior palavra centralizada, as outras seguem o mesmo
    sprintf(msg, " Dificil");
    xPos = -getStrokeStringWidth(msg, optionScale) / 2;
    yPos = getTextVerticalPosition(0.2f);

    // Desenha fundo botões dificuldade
    float xPosBG = (xPos - 10.0f);
    float widthBG = xPosBG > 0 ? xPosBG * 2 : xPosBG * -2;
    float yPosBG = (yPos - 5.0f);
    float heightBG = 20.0f;

    drawRectangle(xPosBG, yPosBG, widthBG, heightBG, 0.2f, 0.6f, 0.9f);
    drawRectangleLine(xPosBG, yPosBG, widthBG, heightBG, 1.0f, 1.0f, 1.0f, 3);
    drawRectangle(xPosBG, yPosBG - yPos * 0.5f, widthBG, heightBG, 0.2f, 0.6f, 0.9f);
    drawRectangleLine(xPosBG, yPosBG - yPos * 0.5f, widthBG, heightBG, 1.0f, 1.0f, 1.0f, 3);
    drawRectangle(xPosBG, yPosBG - yPos * 1.0f, widthBG, heightBG, 0.2f, 0.6f, 0.9f);
    drawRectangleLine(xPosBG, yPosBG - yPos * 1.0f, widthBG, heightBG, 1.0f, 1.0f, 1.0f, 3);

    // Desenha Opção escolhida de dificuldade
    if (difficultyLevel == 0) {
        drawRectangle(xPosBG, yPosBG, widthBG, heightBG, 0.0f, 0.4f, 1.0f);
        drawRectangleLine(xPosBG, yPosBG, widthBG, heightBG, 1.0f, 1.0f, 0.0f, 3);
    }
    else if (difficultyLevel == 1) {
        drawRectangle(xPosBG, yPosBG - yPos * 0.5f, widthBG, heightBG, 0.0f, 0.4f, 1.0f);
        drawRectangleLine(xPosBG, yPosBG - yPos * 0.5f, widthBG, heightBG, 1.0f, 1.0f, 0.0f, 3);
    }
    else {
        drawRectangle(xPosBG, yPosBG - yPos * 1.0f, widthBG, heightBG, 0.0f, 0.4f, 1.0f);
        drawRectangleLine(xPosBG, yPosBG - yPos * 1.0f, widthBG, heightBG, 1.0f, 1.0f, 0.0f, 3);
    }

    // Desenha Texto dificuldade
    drawText(" Facil", xPos, yPos, 1.0, 1.0, 1.0, optionScale);
    drawText(" Medio", xPos, yPos - yPos * 0.5f, 1.0, 1.0, 1.0, optionScale);
    drawText(" Dificil", xPos, yPos - yPos * 1.0f, 1.0, 1.0, 1.0, optionScale);

    // Comandos

    float yPosCmd = -getTextVerticalPosition(0.085f);
    yPos = yPosCmd * 1;
    drawRectangle(-(HRES / 2 - 98.0f), yPos + 14.0f, (HRES - 196.0f), -(VRES / 2 - 20.0f), 1.0f, 1.0f, 1.0f);
    drawRectangle(-(HRES / 2 - 100.0f), yPos + 12.0f, (HRES - 200.0f), -(VRES / 2 - 24.0f), 0.2f, 0.4f, 0.7f);
    drawText("Comandos:", -getStrokeStringWidth("Comandos:", commandScale) / 2, yPos, 1.0, 1.0, 1.0, commandScale);

    // Alinha a maior palavra centralizada, as outras seguem o mesmo
    sprintf(msg, "'+/-' = Mostrar Informacoes");
    float xPosMax = -getStrokeStringWidth(msg, commandScale) / 2;
    sprintf(msg, "'+/-' ");
    xPos = xPosMax + getStrokeStringWidth(msg, commandScale);

    yPosCmd = -getTextVerticalPosition(0.06f);

    yPos = yPosCmd * 3;
    drawText("'M'", xPosMax, yPos, 1.0, 1.0, 1.0, commandScale);
    drawText("= Menu Principal", xPos, yPos, 1.0, 1.0, 1.0, commandScale);

    yPos = yPosCmd * 4;
    drawText("'I'", xPosMax, yPos, 1.0, 1.0, 1.0, commandScale);
    drawText("= Mostrar Informacoes", xPos, yPos, 1.0, 1.0, 1.0, commandScale);

    yPos = yPosCmd * 5;
    drawText("'P'", xPosMax, yPos, 1.0, 1.0, 1.0, commandScale);
    drawText("= Pause", xPos, yPos, 1.0, 1.0, 1.0, commandScale);

    yPos = yPosCmd * 6;
    drawText("'S'", xPosMax, yPos, 1.0, 1.0, 1.0, commandScale);
    drawText("= Sair", xPos, yPos, 1.0, 1.0, 1.0, commandScale);

    yPos = yPosCmd * 7;
    drawText("'+/-'", xPosMax, yPos, 1.0, 1.0, 1.0, commandScale);
    drawText("= Volume", xPos, yPos, 1.0, 1.0, 1.0, commandScale);
}

void UI::drawExitScreen() {
    float scale = 0.1f;
    char msg1[] = "Para Sair do jogo pressione 'S'";
    char msg2[] = "Para voltar ao Menu Principal Pressione 'M'";
    drawText(msg1, -getStrokeStringWidth(msg1, scale) / 2, getTextVerticalPosition(0.1f), 1.0, 1.0, 1.0, scale);
    drawText(msg2, -getStrokeStringWidth(msg2, scale) / 2, -getTextVerticalPosition(0.1f), 1.0, 1.0, 1.0, scale);
}

void UI::drawPauseScreen() {
    float scale = 0.1f;
    char msg1[] = "PAUSADO";
    char msg2[] = "Para voltar pressione 'P'";
    drawText(msg1, -getStrokeStringWidth(msg1, scale) / 2, getTextVerticalPosition(0.1f), 1.0, 1.0, 1.0, scale);
    drawText(msg2, -getStrokeStringWidth(msg2, scale = 0.09f) / 2, -getTextVerticalPosition(0.1f), 1.0, 1.0, 1.0, scale);
}

void UI::drawLevelScreen(int level) {
    char msg[20];
    sprintf(msg, "LEVEL %i", level);
    float scale = 0.2f;
    drawText(msg, -getStrokeStringWidth(msg, scale) / 2, 0, 1.0, 1.0, 1.0, scale);
}

void UI::drawHitScreen(int lifes, float time) {
    char msg[20];
    float scale = 0.15f;

    sprintf(msg, "Restam: %i vidas", lifes);
    drawText(msg, -getStrokeStringWidth(msg, scale) / 2, getTextVerticalPosition(0.1f), 1.0, 1.0, 1.0, scale);

    sprintf(msg, "%0.f", (LOAD_TIME_LONG - time));
    drawText(msg, -getStrokeStringWidth(msg, scale * 1.2f) / 2, -getTextVerticalPosition(0.1f), 1.0, 1.0, 1.0, scale * 1.2f);
}

void UI::drawGameOverScreen() {
    float scale = 0.15f;
    char msg1[] = "VOCE PERDEU";
    char msg2[] = "Para voltar ao Menu Principal Pressione 'M'";
    char msg3[] = "Para Sair do jogo Pressione 'S'";

    drawText(msg1, -getStrokeStringWidth(msg1, scale) / 2, getTextVerticalPosition(0.2f), 1.0, 1.0, 1.0, scale);

    float scale2 = scale * 0.6f;
    drawText(msg2, -getStrokeStringWidth(msg2, scale2) / 2, getTextVerticalPosition(0), 1.0, 1.0, 1.0, scale2);

    drawText(msg3, -getStrokeStringWidth(msg3, scale2) / 2, -getTextVerticalPosition(0.1f), 1.0, 1.0, 1.0, scale2);
}

void UI::drawFinishedScreen() {
    float scale = 0.12f;
    char msg1[] = "PARABENS, Voce Finalizou o Jogo!";
    char msg2[] = "Para voltar ao Menu Principal Pressione 'M'";
    char msg3[] = "Para Sair do jogo Pressione 'S'";

    drawText(msg1, -getStrokeStringWidth(msg1, scale) / 2, getTextVerticalPosition(0.2f), 1.0, 1.0, 1.0, scale);

    float scale2 = scale * 0.7f;
    drawText(msg2, -getStrokeStringWidth(msg2, scale2) / 2, getTextVerticalPosition(0), 1.0, 1.0, 1.0, scale2);

    drawText(msg3, -getStrokeStringWidth(msg3, scale2) / 2, -getTextVerticalPosition(0.1f), 1.0, 1.0, 1.0, scale2);
}

void UI::drawGameInfo(int level, float score, int lifes, int tailLength, float speed, float time) {
    if (!showInformations) return;
    char msg[120];
    float scale = 0.08f;
    float yPos = getTextVerticalPosition((VRES - 40.0f) / (float) VRES) / 2;
    float xPosLeft = -getTextHorizontalPosition((HRES - 40.0f) / (float) HRES) / 2;
    float xPosRight = getTextHorizontalPosition((HRES - 300.0f) / (float) HRES) / 2;

    sprintf(msg, "LEVEL: %i", level);
    drawText(msg, xPosLeft, yPos, 1.0, 1.0, 1.0, scale);
    sprintf(msg, "Pontos: %.0f", score);
    drawText(msg, xPosLeft, yPos - 15.0f, 1.0, 1.0, 1.0, scale);

    sprintf(msg, "Vidas: %i", lifes);
    drawText(msg, xPosRight, yPos, 1.0, 1.0, 1.0, scale);
    sprintf(msg, "Tamanho: %i", tailLength);
    drawText(msg, xPosRight, yPos - 15.0f, 1.0, 1.0, 1.0, scale);
    sprintf(msg, "Velocidade: %.2fx", speed);
    drawText(msg, xPosRight, yPos - 30.0f, 1.0, 1.0, 1.0, scale);
    sprintf(msg, "Tempo: %.0f", time);
    drawText(msg, xPosRight, yPos - 45.0f, 1.0, 1.0, 1.0, scale);
}

void UI::drawDebugInfo(int sX, int sY, int fX, int fY, int screen, int status, unsigned char pKey, int spKey) {
    if (!showInformations) return; // Debug info is also part of the toggleable info
    char msg[120];
    float scale = 0.07f;
    // float yPos = -windowHeight / 2.0f + (20.0f * (windowHeight / (float)VRES));
    // float xPos = -windowWidth / 2.0f + (20.0f * (windowWidth / (float)HRES));
    float yPos = -getTextVerticalPosition((VRES - 40.0f) / (float) VRES) / 2;
    float xPos = -getTextHorizontalPosition((HRES - 40.0f) / (float) HRES) / 2;

    sprintf(msg, "Snake(x, y): (%i, %i)", sX, sY);
    drawText(msg, xPos, yPos, 1.0, 1.0, 1.0, scale);
    sprintf(msg, "Food(x, y): (%i, %i)", fX, fY);
    drawText(msg, xPos, yPos + 12.0f, 1.0, 1.0, 1.0, scale);
    sprintf(msg, "CurrentScreen: %i", screen);
    drawText(msg, xPos, yPos + 24.0f, 1.0, 1.0, 1.0, scale);
    sprintf(msg, "StatusGame: %i", status);
    drawText(msg, xPos, yPos + 36.0f, 1.0, 1.0, 1.0, scale);

    if (pKey) {
        sprintf(msg, "Key: %c", pKey);
    } else if (spKey) {
        strcpy(msg, "Special: ");
        switch (spKey) {
            case GLUT_KEY_F1: strcat(msg, "F1"); break;
            case GLUT_KEY_UP: strcat(msg, "UP"); break;
            case GLUT_KEY_DOWN: strcat(msg, "DOWN"); break;
            case GLUT_KEY_LEFT: strcat(msg, "LEFT"); break;
            case GLUT_KEY_RIGHT: strcat(msg, "RIGHT"); break;
            default: strcat(msg, "OTHER"); break;
        }
    } else {
        return;
    }
    drawText(msg, xPos, yPos + 60.0f, 1.0, 1.0, 1.0, scale);
}

void UI::drawVolume(int volume) {
    char msg[120];
    float scale = 0.08f;
    sprintf(msg, "Volume: %i%%", (volume * 100 / 255));
    float xPos = -getStrokeStringWidth(msg, scale) / 2;
    float yPos = getTextVerticalPosition((VRES - 40.0f) / (float) VRES) / 2;
    drawText(msg, xPos, yPos, 1.0, 1.0, 1.0, scale);
}

void UI::toggleInformations() {
    showInformations = !showInformations;
}
