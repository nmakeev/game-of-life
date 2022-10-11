#include "screen_settings.h"

ScreenSettings::ScreenSettings(int width, int height) : m_width(width), m_height(height) {
}

int ScreenSettings::GetWidth() const {
    return m_width;
}

int ScreenSettings::GetHeight() const {
    return m_height;
}