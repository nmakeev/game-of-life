#ifndef GAME_OF_LIFE_SCREEN_SETTINGS_H
#define GAME_OF_LIFE_SCREEN_SETTINGS_H

class ScreenSettings {
public:
    ScreenSettings(int width, int height);

    int GetWidth() const;
    int GetHeight() const;

private:
    int m_width;
    int m_height;
};


#endif //GAME_OF_LIFE_SCREEN_SETTINGS_H
