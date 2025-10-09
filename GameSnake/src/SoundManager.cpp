#include "SoundManager.h"

SoundManager::SoundManager() {
    // Construtor pode ser usado para inicializar ponteiros para nulo
    somMenu = nullptr;
    somComeu = nullptr;
    somChoque = nullptr;
    somParabens = nullptr;
    somPerdeu = nullptr;
}

SoundManager::~SoundManager() {
    // Limpa recursos do FMOD
    FSOUND_StopSound(FSOUND_ALL);
    FSOUND_Close();
}

void SoundManager::init() {
    if (FSOUND_Init(44100, 32, 0)) {
        somMenu = FSOUND_Stream_Open("../../Sons/SomMenu.mp3", 0, 0, 0);
        FSOUND_Stream_SetMode(somMenu, FSOUND_LOOP_NORMAL);

        somComeu = FSOUND_Sample_Load(1, "../../Sons/SomComeu.wav", 0, 0, 0);
        FSOUND_Sample_SetMode(somComeu, FSOUND_LOOP_OFF);

        somChoque = FSOUND_Sample_Load(3, "../../Sons/SomChoque2.wav", 0, 0, 0);
        FSOUND_Sample_SetMode(somChoque, FSOUND_LOOP_OFF);

        somParabens = FSOUND_Sample_Load(4, "../../Sons/SomParabens1.mp3", 0, 0, 0);
        FSOUND_Sample_SetMode(somParabens, FSOUND_LOOP_OFF);

        somPerdeu = FSOUND_Sample_Load(5, "../../Sons/SomPerdeu.wav", 0, 0, 0);
        FSOUND_Sample_SetMode(somPerdeu, FSOUND_LOOP_OFF);

        setMasterVolume(50);
    }
}

void SoundManager::playMenuSound() {
    if (somMenu) {
        FSOUND_Stream_SetTime(somMenu, 0);
        FSOUND_Stream_Play(0, somMenu);
    }
}

void SoundManager::playEatSound() {
    if (somComeu) FSOUND_PlaySound(1, somComeu);
}

void SoundManager::playHitSound() {
    if (somChoque) FSOUND_PlaySound(3, somChoque);
}

void SoundManager::playWinSound() {
    if (somParabens) FSOUND_PlaySound(4, somParabens);
}

void SoundManager::playLoseSound() {
    if (somPerdeu) FSOUND_PlaySound(5, somPerdeu);
}

void SoundManager::stopAllSounds() {
    FSOUND_StopSound(FSOUND_ALL);
}

void SoundManager::setMasterVolume(int volume) {
    if (volume < 0) volume = 0;
    if (volume > 255) volume = 255;
    FSOUND_SetSFXMasterVolume(volume);
}

int SoundManager::getMasterVolume() {
    return FSOUND_GetSFXMasterVolume();
}
