//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_FLASHING_GEOSET_H
#define MAGOS_FLASHING_GEOSET_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Common.h"


//+-----------------------------------------------------------------------------
//| Pre-declared classes
//+-----------------------------------------------------------------------------
class MODEL_GEOSET;


//+-----------------------------------------------------------------------------
//| Flashing geoset class
//+-----------------------------------------------------------------------------
class FLASHING_GEOSET
{
	public:
		CONSTRUCTOR FLASHING_GEOSET();
		DESTRUCTOR ~FLASHING_GEOSET();

		VOID Update(INT TimeDifference);
		BOOL IsFlashing();

		VOID SetGeoset(MODEL_GEOSET* NewGeoset);

		MODEL_GEOSET* GetGeoset();
		D3DXCOLOR GetColor();

	protected:
		INT Time;
		BOOL Flashing;
		MODEL_GEOSET* Geoset;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern FLASHING_GEOSET FlashingGeoset;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
