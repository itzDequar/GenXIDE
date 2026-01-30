#include "bgfx/bgfx.h"
#include <iostream>
#include <renderer.h>

bool Renderer::init( SDL_SysWMinfo wmi, int width, int height )
{
  bool success{ true };

  bgfx::Init init{};

  init.type = bgfx::RendererType::Count;
  init.vendorId = BGFX_PCI_ID_NONE;
  init.resolution.width  = width;
  init.resolution.height = height;
  init.resolution.reset  = BGFX_RESET_VSYNC;

  #if BX_PLATFORM_OSX
    bgfx::renderFrame();
  #endif

  #if BX_PLATFORM_WINDOWS
    pd.nwh = wmi.info.win.window;
  #elif BX_PLATFORM_OSX
    pd.nwh = wmi.info.cocoa.window;
  #elif BX_PLATFORM_LINUX
    pd.ndt = wmi.info.x11.display;
    pd.nwh = (void*)wmi.info.x11.window;
  #endif

  bgfx::setPlatformData(pd);

  if ( !bgfx::init(init) )
  {
    std::cerr << "BGFX Not Inited: " << SDL_GetError() << std::endl;
    success = false;
  }

  bgfx::setViewClear(
    0,
    BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
    0x1a1a1a1a,
    1.0f,
    0
  );

  bgfx::setViewRect(0, 0, 0, width, height);

  return success;
}

bool Renderer::destroy()
{
  bgfx::shutdown();
  return true;
}

void Renderer::sizeChanged( SDL_Event event )
{
  if ( event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED )
  {
    bgfx::reset
    (
      event.window.data1,
      event.window.data2,
      BGFX_RESET_VSYNC
    );

    bgfx::setViewRect
    (
      0, 0, 0,
      (uint16_t)event.window.data1,
      (uint16_t)event.window.data2
    );
  }
}


void Renderer::renderer()
{
  bgfx::touch(0);

  bgfx::frame();

  return;
}
