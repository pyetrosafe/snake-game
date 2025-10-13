#ifndef UI_H
#define UI_H

class UI {
public:
    UI();
    void drawBackground();
    void drawMenu(int difficultyLevel);
    void drawExitScreen();
    void drawPauseScreen();
    void drawLevelScreen(int level);
    void drawHitScreen(int lifes, float time);
    void drawGameOverScreen();
    void drawFinishedScreen();
    void drawGameInfo(int level, float score, int lifes, int tailLength, float speed, float time);
    void drawDebugInfo(int sX, int sY, int fX, int fY, int screen, int status, unsigned char pKey, int spKey);
    void drawVolume(int volume);

    void toggleInformations();

private:
    void drawText(char* string, int x, int y, float r = 1.0, float g = 1.0, float b = 1.0);
    void drawBorders();

    bool showInformations;
    int sizefont;
};

#endif // UI_H
