//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_PARTICLE_EMITTER_H
#define MAGOS_MODEL_PARTICLE_EMITTER_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelBase.h"


//+-----------------------------------------------------------------------------
//| Model particle emitter data structure
//+-----------------------------------------------------------------------------
struct MODEL_PARTICLE_EMITTER_DATA : public MODEL_BASE_DATA
{
	MODEL_PARTICLE_EMITTER_DATA()
	{
		EmissionRate.SetStaticScalar(0.0f, "EmissionRate");
		Gravity.SetStaticScalar(0.0f, "Gravity");
		Longitude.SetStaticScalar(0.0f, "Longitude");
		Latitude.SetStaticScalar(0.0f, "Latitude");
		Visibility.SetStaticScalar(1.0f, "Visibility");

		ParticleFileName = "";
		ParticleLifeSpan.SetStaticScalar(0.0f, "LifeSpan");
		ParticleInitialVelocity.SetStaticScalar(0.0f, "InitVelocity");

		EmitterUsesMdl = FALSE;
		EmitterUsesTga = FALSE;
	}

	INTERPOLATOR EmissionRate;
	INTERPOLATOR Gravity;
	INTERPOLATOR Longitude;
	INTERPOLATOR Latitude;
	INTERPOLATOR Visibility;

	std::string ParticleFileName;
	INTERPOLATOR ParticleLifeSpan;
	INTERPOLATOR ParticleInitialVelocity;

	BOOL EmitterUsesMdl;
	BOOL EmitterUsesTga;
};


//+-----------------------------------------------------------------------------
//| Model particle emitter class
//+-----------------------------------------------------------------------------
class MODEL_PARTICLE_EMITTER : public MODEL_BASE
{
	public:
		CONSTRUCTOR MODEL_PARTICLE_EMITTER();
		DESTRUCTOR ~MODEL_PARTICLE_EMITTER();

		VOID Clear();
		INT GetSize();

		MODEL_PARTICLE_EMITTER_DATA& Data();

	protected:
		MODEL_PARTICLE_EMITTER_DATA ParticleEmitterData;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
