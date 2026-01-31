#include "camera2d.h"
#include <cmath>

Camera2D::Camera2D( float x, float y, float zoom )
  : m_x(x)
  , m_y(y)
  , m_zoom(std::clamp(zoom, Config::MIN_ZOOM, Config::MAX_ZOOM))
{
}

void Camera2D::setPosition( float x, float y )
{
  m_x = x;
  m_y = y;
}

void Camera2D::move( float dx, float dy )
{
  m_x += dx;
  m_y += dy;
}

void Camera2D::setZoom( float zoom )
{
  m_zoom = std::clamp(zoom, Config::MIN_ZOOM, Config::MAX_ZOOM);
}

void Camera2D::zoom( float factor )
{
  m_zoom = std::clamp(m_zoom * factor, Config::MIN_ZOOM, Config::MAX_ZOOM);
}

void Camera2D::zoomAt( float worldX, float worldY, float factor )
{
  float oldZoom = m_zoom;
  m_zoom = std::clamp(m_zoom * factor, Config::MIN_ZOOM, Config::MAX_ZOOM);

  // Adjust position to zoom towards the point
  float zoomRatio = m_zoom / oldZoom;
  m_x = worldX + (m_x - worldX) * zoomRatio;
  m_y = worldY + (m_y - worldY) * zoomRatio;
}

void Camera2D::getViewMatrix( float* matrix, float aspect ) const
{
  // Column-major (OpenGL) view-projection matrix
  float sx = m_zoom / aspect;
  float sy = m_zoom;

  matrix[0]  = sx;   matrix[4]  = 0.0f; matrix[8]  = 0.0f; matrix[12] = -m_x * sx;
  matrix[1]  = 0.0f; matrix[5]  = sy;   matrix[9]  = 0.0f; matrix[13] = -m_y * sy;
  matrix[2]  = 0.0f; matrix[6]  = 0.0f; matrix[10] = 1.0f; matrix[14] = 0.0f;
  matrix[3]  = 0.0f; matrix[7]  = 0.0f; matrix[11] = 0.0f; matrix[15] = 1.0f;
}

void Camera2D::startDrag( int mouseX, int mouseY )
{
  m_dragging = true;
}

void Camera2D::drag( int dx, int dy, int fbHeight )
{
  if (!m_dragging) return;

  float worldPerPixel = 2.0f / (m_zoom * fbHeight);

  m_x -= dx * worldPerPixel;
  m_y += dy * worldPerPixel;
}

void Camera2D::endDrag()
{
  m_dragging = false;
}
