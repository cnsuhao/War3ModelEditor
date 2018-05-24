//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_PARTICLE_H
#define MAGOS_PARTICLE_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Misc.h"


//+-----------------------------------------------------------------------------
//| Pre-declared classes
//+-----------------------------------------------------------------------------
class MODEL_PARTICLE_EMITTER_2;


//+-----------------------------------------------------------------------------
//| Particle class
//+-----------------------------------------------------------------------------
class PARTICLE
{
	public:
		CONSTRUCTOR PARTICLE();
		DESTRUCTOR ~PARTICLE();

		VOID Create(CONST SEQUENCE_TIME& SequenceTime, MODEL_PARTICLE_EMITTER_2* ParticleEmitter2);
		VOID Clear();

		VOID Render();
		BOOL Update(INT TimeDifference);

	protected:
		D3DXVECTOR3 Position;
		D3DXVECTOR3 Velocity;

		FILTER_MODE FilterMode;
		std::string TextureFileName;

		D3DXVECTOR3 SegmentColor1;
		D3DXVECTOR3 SegmentColor2;
		D3DXVECTOR3 SegmentColor3;
		D3DXVECTOR3 Alpha;
		D3DXVECTOR3 ParticleScaling;
		D3DXVECTOR3 HeadLifeSpan;
		D3DXVECTOR3 HeadDecay;
		D3DXVECTOR3 TailLifeSpan;
		D3DXVECTOR3 TailDecay;

		INT Rows;
		INT Columns;
		INT ReplaceableId;
		FLOAT Time;
		FLOAT Gravity;
		FLOAT LifeSpan;
		FLOAT InitialLifeSpan;
		FLOAT TailLength;
		FLOAT Variation;

		BOOL SortPrimitivesFarZ;
		BOOL LineEmitter;
		BOOL ModelSpace;
		BOOL AlphaKey;
		BOOL Unshaded;
		BOOL Unfogged;
		BOOL XYQuad;
		BOOL Squirt;
		BOOL Head;
		BOOL Tail;

		INT CurrentFrame;
		INT NrOfHeadFrames;
		INT NrOfHeadDecayFrames;
		INT NrOfTailFrames;
		INT NrOfTailDecayFrames;
};


//+-----------------------------------------------------------------------------
//| Post-included files
//+-----------------------------------------------------------------------------
#include "Graphics.h"


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
