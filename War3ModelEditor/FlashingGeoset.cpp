//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "FlashingGeoset.h"
#include "ModelGeoset.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
FLASHING_GEOSET FlashingGeoset;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
FLASHING_GEOSET::FLASHING_GEOSET()
{
	Time = 0;
	Flashing = FALSE;
	Geoset = NULL;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
FLASHING_GEOSET::~FLASHING_GEOSET()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Updates the flashing geoset
//+-----------------------------------------------------------------------------
VOID FLASHING_GEOSET::Update(INT TimeDifference)
{
	if(Time <= 0)
	{
		Flashing = FALSE;
		return;
	}

	Time -= TimeDifference;
	if(Time <= 0)
	{
		Time = 0;
		Geoset = NULL;
	}
}


//+-----------------------------------------------------------------------------
//| Checks if the geoset is flashing
//+-----------------------------------------------------------------------------
BOOL FLASHING_GEOSET::IsFlashing()
{
	return Flashing;
}


//+-----------------------------------------------------------------------------
//| Sets a new geoset as the flashing geoset
//+-----------------------------------------------------------------------------
VOID FLASHING_GEOSET::SetGeoset(MODEL_GEOSET* NewGeoset)
{
	Time = 300;
	Flashing = TRUE;
	Geoset = NewGeoset;
}


//+-----------------------------------------------------------------------------
//| Returns the currently flashing geoset
//+-----------------------------------------------------------------------------
MODEL_GEOSET* FLASHING_GEOSET::GetGeoset()
{
	return Geoset;
}


//+-----------------------------------------------------------------------------
//| Returns the color factor of the flashing geoset
//+-----------------------------------------------------------------------------
D3DXCOLOR FLASHING_GEOSET::GetColor()
{
	FLOAT Factor;
	FLOAT ColorFactor;
	D3DXCOLOR Color;

	if(Geoset != NULL)
	{
		Factor = (static_cast<FLOAT>(300 - Time) / 300.0f) * D3DX_PI;
		ColorFactor = 1.0f - sin(Factor);
	}
	else
	{
		ColorFactor = 1.0f;
	}

	Color.r = ColorFactor;
	Color.g = ColorFactor;
	Color.b = ColorFactor;
	Color.a = 1.0f;

	return Color;
}
