//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ModelParticleEmitter2.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_PARTICLE_EMITTER_2::MODEL_PARTICLE_EMITTER_2()
{
	ModelBaseData = &ParticleEmitter2Data;
	ModelBaseData->Type = NODE_TYPE_PARTICLE_EMITTER_2;

	TextureNode.SetData(this);
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL_PARTICLE_EMITTER_2::~MODEL_PARTICLE_EMITTER_2()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Clears the particle emitter
//+-----------------------------------------------------------------------------
VOID MODEL_PARTICLE_EMITTER_2::Clear()
{
	ParticleEmitter2Data = MODEL_PARTICLE_EMITTER_2_DATA();
}


//+-----------------------------------------------------------------------------
//| Returns the mdx size of the particle emitter
//+-----------------------------------------------------------------------------
INT MODEL_PARTICLE_EMITTER_2::GetSize()
{
	INT TotalSize;
	INT VisibilitySize;
	INT EmissionRateSize;
	INT WidthSize;
	INT LengthSize;
	INT SpeedSize;
	INT LatitudeSize;

	VisibilitySize = ParticleEmitter2Data.Visibility.GetSize();
	EmissionRateSize = ParticleEmitter2Data.EmissionRate.GetSize();
	WidthSize = ParticleEmitter2Data.Width.GetSize();
	LengthSize = ParticleEmitter2Data.Length.GetSize();
	SpeedSize = ParticleEmitter2Data.Speed.GetSize();
	LatitudeSize = ParticleEmitter2Data.Latitude.GetSize();

	TotalSize = GetBaseSize() + 175 + VisibilitySize + EmissionRateSize + WidthSize + LengthSize + SpeedSize + LatitudeSize;

	return TotalSize;
}


//+-----------------------------------------------------------------------------
//| Returns a reference to the data
//+-----------------------------------------------------------------------------
MODEL_PARTICLE_EMITTER_2_DATA& MODEL_PARTICLE_EMITTER_2::Data()
{
	return ParticleEmitter2Data;
}
