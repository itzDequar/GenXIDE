#pragma once
#include "core/config.h"
#include <algorithm>

class Camera2D
{
  public:
  Camera2D() = default;
  Camera2D( float x, float y, float zoom );

  void setPosition( float x, float y );
  void move( float dx, float dy );
  void setZoom( float zoom );
  void zoom( float factor );
  void zoomAt( float worldX, float worldY, float factor );

  inline float getX() const { return m_x; }
  inline float getY() const { return m_y; }
  inline float getZoom() const { return m_zoom; }

  void getViewMatrix( float* matrix, float aspect ) const;

  void startDrag( int mouseX, int mouseY );
  void drag( int dx, int dy, int fbHeight );
  void endDrag();
  inline bool isDragging() const { return m_dragging; }

  private:
  float m_x{ 0.0f };
  float m_y{ 0.0f };
  float m_zoom{ 1.0f };

  bool m_dragging{ false };
};
