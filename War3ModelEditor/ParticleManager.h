//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_PARTICLE_MANAGER_H
#define MAGOS_PARTICLE_MANAGER_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Particle.h"


//+-----------------------------------------------------------------------------
//| Particle manager class
//+-----------------------------------------------------------------------------
class PARTICLE_MANAGER
{
	public:
		CONSTRUCTOR PARTICLE_MANAGER();
		DESTRUCTOR ~PARTICLE_MANAGER();

		VOID Clear();

		VOID Render();
		VOID Update(CONST SEQUENCE_TIME& Time, INT TimeDifference, BOOL CreateNewParticles);

		INT GetNrOfParticles();

	protected:
		std::list<PARTICLE*> ParticleList;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern PARTICLE_MANAGER ParticleManager;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
