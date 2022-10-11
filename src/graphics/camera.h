#ifndef GAME_OF_LIFE_CAMERA_H
#define GAME_OF_LIFE_CAMERA_H

#include <glm/glm.hpp>
#include "screen_settings.h"

class Camera {
public:
    Camera(const ScreenSettings& screenSettings);

    glm::vec2 GetPosition() const;
    float GetZoom() const;

    void SetPosition(glm::vec2 position);
    void SetZoom(float zoom);

    glm::mat4x4 GetProjectionMatrix() const;

private:
    const ScreenSettings& m_screenSettings;

    glm::vec2 m_position;
    float m_zoom;
};

#endif //GAME_OF_LIFE_CAMERA_H
