#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "camera.h"

Camera::Camera(const ScreenSettings& screenSettings) : m_screenSettings(screenSettings), m_position(), m_zoom(1.f) {
}

void Camera::SetPosition(glm::vec2 position) {
    m_position = position;
}

void Camera::SetZoom(float zoom) {
    m_zoom = zoom;
}

glm::vec2 Camera::GetPosition() const {
    return m_position;
}

float Camera::GetZoom() const {
    return m_zoom;
}

glm::mat4x4 Camera::GetProjectionMatrix() const {
    float half_width = static_cast<float>(m_screenSettings.GetWidth()) / 2;
    float half_height = static_cast<float>(m_screenSettings.GetHeight()) / 2;

    float left = m_position.x - half_width;
    float right = m_position.x + half_width;
    float top = m_position.y - half_height;
    float bottom = m_position.y + half_height;

    glm::mat4 projection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    return glm::scale(projection, glm::vec3(m_zoom, m_zoom, m_zoom));
}