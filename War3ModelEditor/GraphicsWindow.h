//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_GRAPHICS_WINDOW_H
#define MAGOS_GRAPHICS_WINDOW_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "GraphicsInfo.h"


//+-----------------------------------------------------------------------------
//| Pre-declared classes
//+-----------------------------------------------------------------------------
class GRAPHICS;


//+-----------------------------------------------------------------------------
//| Graphics window class
//+-----------------------------------------------------------------------------
class GRAPHICS_WINDOW
{
	public:
		CONSTRUCTOR GRAPHICS_WINDOW();
		DESTRUCTOR ~GRAPHICS_WINDOW();

		BOOL Create(HWND NewWindow);
		VOID Destroy();

		BOOL Reset();
		BOOL Resize();

		HWND GetWindow();
		LPDIRECT3DSWAPCHAIN9 GetSwapChain();
		LPDIRECT3DSURFACE9 GetBackBuffer();
		LPDIRECT3DSURFACE9 GetZBuffer();

		INT GetWidth();
		INT GetHeight();
		FLOAT GetScreenAspect();

	protected:
		BOOL Acquire();
		VOID Release();

		HWND Window;
		LPDIRECT3DSWAPCHAIN9 SwapChain;
		LPDIRECT3DSURFACE9 BackBuffer;
		LPDIRECT3DSURFACE9 ZBuffer;

		INT Width;
		INT Height;
		FLOAT ScreenAspect;

		GRAPHICS_INFO GraphicsInfo;

		REFERENCE<GRAPHICS_WINDOW*, GRAPHICS*> GraphicsWindowReference;

		friend class GRAPHICS;
};


//+-----------------------------------------------------------------------------
//| Post-included files
//+-----------------------------------------------------------------------------
#include "Graphics.h"


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
