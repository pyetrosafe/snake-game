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
    void reshape(int w, int h);

private:
    float getViewDimensionSize();
    void drawText(const char* string, float x, float y, float r = 1.0, float g = 1.0, float b = 1.0, float scale = 0.1f);
    void drawTextBitmap(const char* string, int x, int y, float r, float g, float b);
    float getStrokeStringWidth(const char* string, float scale);
    float getTextVerticalPosition(float scale);
    float getTextHorizontalPosition(float scale);
    void drawBorders();
    void drawRectangle(int x, int y, int width, int height, float r = 1.0f, float g = 1.0f, float b = 1.0f);
    void drawRectangleLine(int x, int y, int width, int height, float borderR, float borderG, float borderB, int borderWidth);
    void drawFilledRoundedRectangle(float x, float y, float width, float height, float radius, int segments);
    void drawRoundedRectangleLine(float x, float y, float width, float height, float radius, int segments, float r, float g, float b, float borderWidth);

    bool showInformations;
    int sizefont;
    int windowWidth;
    int windowHeight;
};

#endif // UI_H
