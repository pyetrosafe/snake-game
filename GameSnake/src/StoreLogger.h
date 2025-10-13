#ifndef STORELOGGER_H
#define STORELOGGER_H

#include <string>
#include <cstdio>

// Forward-declaration para evitar dependencia circular
class Game;

class StoreLogger {
public:
    StoreLogger();
    ~StoreLogger();

    void log(const Game& game, const char* event);
    void setEnabled(bool enabled);
    bool isEnabled() const;

private:
    std::string getCurrentDateTime();
    FILE* logFile;
    bool enabled;
};

#endif // STORELOGGER_H
