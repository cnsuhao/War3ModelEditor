//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_GRAPHICS_INFO_H
#define MAGOS_GRAPHICS_INFO_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Misc.h"


//+-----------------------------------------------------------------------------
//| Graphics info structure
//+-----------------------------------------------------------------------------
struct GRAPHICS_INFO
{
	GRAPHICS_INFO()
	{
		Title = "";
		
		Width = 0;
		Height = 0;

		TextureFormat = D3DFMT_A8R8G8B8;

		DeviceInfo.BackBufferWidth = 0;
		DeviceInfo.BackBufferHeight = 0;
		DeviceInfo.BackBufferFormat = D3DFMT_X8R8G8B8;
		DeviceInfo.BackBufferCount = 1;
		DeviceInfo.MultiSampleType = D3DMULTISAMPLE_NONE;
		DeviceInfo.MultiSampleQuality = 0;
		DeviceInfo.SwapEffect = D3DSWAPEFFECT_DISCARD;
		DeviceInfo.hDeviceWindow = NULL;
		DeviceInfo.Windowed = TRUE;
		DeviceInfo.EnableAutoDepthStencil = TRUE;
		DeviceInfo.AutoDepthStencilFormat = D3DFMT_D16;
		DeviceInfo.Flags = 0;
		DeviceInfo.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		DeviceInfo.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	}

	std::string Title;
	
	INT Width;
	INT Height;

	D3DFORMAT TextureFormat;

	D3DPRESENT_PARAMETERS DeviceInfo;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
