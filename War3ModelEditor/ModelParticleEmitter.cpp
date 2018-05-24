//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ModelParticleEmitter.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_PARTICLE_EMITTER::MODEL_PARTICLE_EMITTER()
{
	ModelBaseData = &ParticleEmitterData;
	ModelBaseData->Type = NODE_TYPE_PARTICLE_EMITTER;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL_PARTICLE_EMITTER::~MODEL_PARTICLE_EMITTER()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Clears the particle emitter
//+-----------------------------------------------------------------------------
VOID MODEL_PARTICLE_EMITTER::Clear()
{
	ParticleEmitterData = MODEL_PARTICLE_EMITTER_DATA();
}


//+-----------------------------------------------------------------------------
//| Returns the mdx size of the particle emitter
//+-----------------------------------------------------------------------------
INT MODEL_PARTICLE_EMITTER::GetSize()
{
	return GetBaseSize() + 288 + ParticleEmitterData.Visibility.GetSize();
}


//+-----------------------------------------------------------------------------
//| Returns a reference to the data
//+-----------------------------------------------------------------------------
MODEL_PARTICLE_EMITTER_DATA& MODEL_PARTICLE_EMITTER::Data()
{
	return ParticleEmitterData;
}
