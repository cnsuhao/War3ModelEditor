//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "GraphicsWindow.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
GRAPHICS_WINDOW::GRAPHICS_WINDOW()
{
	Window = NULL;
	SwapChain = NULL;
	BackBuffer = NULL;
	ZBuffer = NULL;

	Width = 0;
	Height = 0;
	ScreenAspect = 0.0f;

	GraphicsWindowReference.SetData(this);
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
GRAPHICS_WINDOW::~GRAPHICS_WINDOW()
{
	Destroy();
}


//+-----------------------------------------------------------------------------
//| Creates a new graphics window
//+-----------------------------------------------------------------------------
BOOL GRAPHICS_WINDOW::Create(HWND NewWindow)
{
	RECT TempRect;
	GRAPHICS_INFO* NewGraphicsInfo;

	Destroy();

	Window = NewWindow;

	::GetClientRect(Window, &TempRect);
	Width = TempRect.right;
	Height = TempRect.bottom;
	ScreenAspect = (Height == 0) ? 0.0f : (static_cast<FLOAT>(Width) / static_cast<FLOAT>(Height));

	NewGraphicsInfo = Graphics.GetGraphicsInfo();
	if(NewGraphicsInfo == NULL)
	{
		Error.SetMessage("Unable to retrieve the Direct3D device info!");
		return FALSE;
	}

	GraphicsInfo = (*NewGraphicsInfo);
	GraphicsInfo.DeviceInfo.BackBufferWidth = Width;
	GraphicsInfo.DeviceInfo.BackBufferHeight = Height;

	if(!Acquire()) return FALSE;

	GraphicsWindowReference.Attach(Graphics.GraphicsWindowReferenceObject);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Destroys the graphics window
//+-----------------------------------------------------------------------------
VOID GRAPHICS_WINDOW::Destroy()
{
	GraphicsWindowReference.Detach();

	Release();

	Window =  NULL;
	SwapChain = NULL;
	BackBuffer = NULL;
	ZBuffer = NULL;

	Width = 0;
	Height = 0;
	ScreenAspect = 0.0f;
}


//+-----------------------------------------------------------------------------
//| Resets the graphics window
//+-----------------------------------------------------------------------------
BOOL GRAPHICS_WINDOW::Reset()
{
	Release();

	if(!Acquire()) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Resizes the graphics window
//+-----------------------------------------------------------------------------
BOOL GRAPHICS_WINDOW::Resize()
{
	RECT TempRect;

	::GetClientRect(Window, &TempRect);
	Width = TempRect.right;
	Height = TempRect.bottom;
	ScreenAspect = (Height == 0) ? 0.0f : (static_cast<FLOAT>(Width) / static_cast<FLOAT>(Height));

	GraphicsInfo.DeviceInfo.BackBufferWidth = Width;
	GraphicsInfo.DeviceInfo.BackBufferHeight = Height;

	if(!Reset()) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Returns the window pointer
//+-----------------------------------------------------------------------------
HWND GRAPHICS_WINDOW::GetWindow()
{
	return Window;
}


//+-----------------------------------------------------------------------------
//| Returns the swapchain pointer
//+-----------------------------------------------------------------------------
LPDIRECT3DSWAPCHAIN9 GRAPHICS_WINDOW::GetSwapChain()
{
	return SwapChain;
}


//+-----------------------------------------------------------------------------
//| Returns the Z-buffer
//+-----------------------------------------------------------------------------
LPDIRECT3DSURFACE9 GRAPHICS_WINDOW::GetZBuffer()
{
	return ZBuffer;
}


//+-----------------------------------------------------------------------------
//| Returns the back buffer
//+-----------------------------------------------------------------------------
LPDIRECT3DSURFACE9 GRAPHICS_WINDOW::GetBackBuffer()
{
	return BackBuffer;
}


//+-----------------------------------------------------------------------------
//| Returns the width
//+-----------------------------------------------------------------------------
INT GRAPHICS_WINDOW::GetWidth()
{
	return Width;
}


//+-----------------------------------------------------------------------------
//| Returns the height
//+-----------------------------------------------------------------------------
INT GRAPHICS_WINDOW::GetHeight()
{
	return Height;
}


//+-----------------------------------------------------------------------------
//| Returns the screen aspect
//+-----------------------------------------------------------------------------
FLOAT GRAPHICS_WINDOW::GetScreenAspect()
{
	return ScreenAspect;
}


//+-----------------------------------------------------------------------------
//| Acquires the swap chain
//+-----------------------------------------------------------------------------
BOOL GRAPHICS_WINDOW::Acquire()
{
	LPDIRECT3DDEVICE9 Direct3DDevice;

	Direct3DDevice = Graphics.GetDevice();
	if(Direct3DDevice == NULL)
	{
		Error.SetMessage("Unable to retrieve the Direct3D device!");
		return FALSE;
	}

	if(FAILED(Direct3DDevice->CreateAdditionalSwapChain(&GraphicsInfo.DeviceInfo, &SwapChain)))
	{
		Error.SetMessage("Unable to create a new swap chain!");
		return FALSE;
	}

	if(FAILED(SwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &BackBuffer)))
	{
		Error.SetMessage("Unable to retrieve the swap chain back buffer!");
		return FALSE;
	}

	if(Width <= 0) return TRUE;
	if(Height <= 0) return TRUE;

	if(FAILED(Direct3DDevice->CreateDepthStencilSurface(Width, Height, GraphicsInfo.DeviceInfo.AutoDepthStencilFormat, D3DMULTISAMPLE_NONE, 0, TRUE, &ZBuffer, NULL)))
	{
		Error.SetMessage("Unable to create a new swap chain Z-buffer!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Releases the swap chain
//+-----------------------------------------------------------------------------
VOID GRAPHICS_WINDOW::Release()
{
	SAFE_RELEASE(ZBuffer);
	SAFE_RELEASE(BackBuffer);
	SAFE_RELEASE(SwapChain);
}
