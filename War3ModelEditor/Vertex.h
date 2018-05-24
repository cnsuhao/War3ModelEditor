//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_VERTEX_H
#define MAGOS_VERTEX_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Misc.h"


//+-----------------------------------------------------------------------------
//| Vertex structure
//+-----------------------------------------------------------------------------
struct VERTEX
{
	VERTEX()
	{
		Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Color = 0xFFFFFFFF;
		TexturePosition = D3DXVECTOR2(0.0f, 0.0f);
	}

	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DCOLOR Color;
	D3DXVECTOR2 TexturePosition;

	static CONST DWORD FORMAT = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;
};


//+-----------------------------------------------------------------------------
//| Line vertex structure
//+-----------------------------------------------------------------------------
struct LINE_VERTEX
{
	LINE_VERTEX()
	{
		Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Color = 0xFFFFFFFF;
	}

	D3DXVECTOR3 Position;
	D3DCOLOR Color;

	static CONST DWORD FORMAT = D3DFVF_XYZ | D3DFVF_DIFFUSE;
};


//+-----------------------------------------------------------------------------
//| Particle vertex structure
//+-----------------------------------------------------------------------------
struct PARTICLE_VERTEX
{
	PARTICLE_VERTEX()
	{
		Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Color = 0xFFFFFFFF;
		TexturePosition = D3DXVECTOR2(0.0f, 0.0f);
	}

	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DCOLOR Color;
	D3DXVECTOR2 TexturePosition;

	static CONST DWORD FORMAT = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;
};


//+-----------------------------------------------------------------------------
//| Ground vertex structure
//+-----------------------------------------------------------------------------
struct GROUND_VERTEX
{
	GROUND_VERTEX()
	{
		Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		Color = 0xFFFFFFFF;
		TexturePosition = D3DXVECTOR2(0.0f, 0.0f);
	}

	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DCOLOR Color;
	D3DXVECTOR2 TexturePosition;

	static CONST DWORD FORMAT = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
