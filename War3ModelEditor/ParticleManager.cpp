//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ParticleManager.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
PARTICLE_MANAGER ParticleManager;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
PARTICLE_MANAGER::PARTICLE_MANAGER()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
PARTICLE_MANAGER::~PARTICLE_MANAGER()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Clears all particles
//+-----------------------------------------------------------------------------
VOID PARTICLE_MANAGER::Clear()
{
	std::list<PARTICLE*>::iterator i;

	i = ParticleList.begin();
	while(i != ParticleList.end())
	{
		delete (*i);
		i++;
	}

	ParticleList.clear();
}


//+-----------------------------------------------------------------------------
//| Renders all particles
//+-----------------------------------------------------------------------------
VOID PARTICLE_MANAGER::Render()
{
	std::list<PARTICLE*>::iterator i;

	i = ParticleList.begin();
	while(i != ParticleList.end())
	{
		(*i)->Render();
		i++;
	}
}


//+-----------------------------------------------------------------------------
//| Updates all particles
//+-----------------------------------------------------------------------------
VOID PARTICLE_MANAGER::Update(CONST SEQUENCE_TIME& Time, INT TimeDifference, BOOL CreateNewParticles)
{
	INT i;
	std::list<PARTICLE*>::iterator j;
	FLOAT TimeFactor;
	PARTICLE* Particle;
	MODEL_PARTICLE_EMITTER_2* ParticleEmitter2;

	TimeFactor = static_cast<FLOAT>(TimeDifference) / 1000.0f;

	j = ParticleList.begin();
	while(j != ParticleList.end())
	{
		if((*j)->Update(TimeDifference))
		{
			j++;
		}
		else
		{
			j = ParticleList.erase(j);
		}
	}

	if(!CreateNewParticles) return;

	for(i = 0; i < Model.Data().ParticleEmitter2Container.GetTotalSize(); i++)
	{
		if(Model.Data().ParticleEmitter2Container.ValidIndex(i))
		{
			ParticleEmitter2 = Model.Data().ParticleEmitter2Container[i];

			if(ParticleEmitter2->Data().Visibility.GetScalar(Time) > 0.0f)
			{
				ParticleEmitter2->Data().CurrentEmission += (ParticleEmitter2->Data().EmissionRate.GetScalar(Time) * TimeFactor);
				while(ParticleEmitter2->Data().CurrentEmission >= 1.0f)
				{
					Particle = new PARTICLE();

					Particle->Create(Time, ParticleEmitter2);
					ParticleList.push_back(Particle);

					ParticleEmitter2->Data().CurrentEmission -= 1.0f;
				}
			}
		}
	}
}


//+-----------------------------------------------------------------------------
//| Returns the nr of particles
//+-----------------------------------------------------------------------------
INT PARTICLE_MANAGER::GetNrOfParticles()
{
	return static_cast<INT>(ParticleList.size());
}
