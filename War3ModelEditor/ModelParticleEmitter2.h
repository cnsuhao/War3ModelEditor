//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_PARTICLE_EMITTER_2_H
#define MAGOS_MODEL_PARTICLE_EMITTER_2_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelBase.h"


//+-----------------------------------------------------------------------------
//| Model particle emitter 2 data structure
//+-----------------------------------------------------------------------------
struct MODEL_PARTICLE_EMITTER_2_DATA : public MODEL_BASE_DATA
{
	MODEL_PARTICLE_EMITTER_2_DATA()
	{
		FilterMode = FILTER_MODE_NONE;

		Speed.SetStaticScalar(0.0f, "Speed");
		Variation.SetStaticScalar(0.0f, "Variation");
		Latitude.SetStaticScalar(0.0f, "Latitude");
		Gravity.SetStaticScalar(0.0f, "Gravity");
		Visibility.SetStaticScalar(1.0f, "Visibility");
		EmissionRate.SetStaticScalar(0.0f, "EmissionRate");
		Width.SetStaticScalar(0.0f, "Width");
		Length.SetStaticScalar(0.0f, "Length");

		SegmentColor1 = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		SegmentColor2 = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		SegmentColor3 = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		Alpha = D3DXVECTOR3(255.0f, 255.0f, 255.0f);
		ParticleScaling = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		HeadLifeSpan = D3DXVECTOR3(0.0f, 0.0f, 1.0f),
		HeadDecay = D3DXVECTOR3(0.0f, 0.0f, 1.0f),
		TailLifeSpan = D3DXVECTOR3(0.0f, 0.0f, 1.0f),
		TailDecay = D3DXVECTOR3(0.0f, 0.0f, 1.0f),

		Rows = 1;
		Columns = 1;
		TextureId = INVALID_INDEX;
		PriorityPlane = 0;
		ReplaceableId = 0;
		Time = 0.0f;
		LifeSpan = 0.0f;
		TailLength = 0.0f;

		SortPrimitivesFarZ = FALSE;
		LineEmitter = FALSE;
		ModelSpace = FALSE;
		AlphaKey = FALSE;
		Unshaded = FALSE;
		Unfogged = FALSE;
		XYQuad = FALSE;
		Squirt = FALSE;
		Head = FALSE;
		Tail = FALSE;

		CurrentEmission = 0;
	}

	FILTER_MODE FilterMode;

	INTERPOLATOR Speed;
	INTERPOLATOR Variation;
	INTERPOLATOR Latitude;
	INTERPOLATOR Gravity;
	INTERPOLATOR Visibility;
	INTERPOLATOR EmissionRate;
	INTERPOLATOR Width;
	INTERPOLATOR Length;

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
	INT TextureId;
	INT PriorityPlane;
	INT ReplaceableId;
	FLOAT Time;
	FLOAT LifeSpan;
	FLOAT TailLength;

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

	FLOAT CurrentEmission;
};


//+-----------------------------------------------------------------------------
//| Model particle emitter 2 class
//+-----------------------------------------------------------------------------
class MODEL_PARTICLE_EMITTER_2 : public MODEL_BASE
{
	public:
		CONSTRUCTOR MODEL_PARTICLE_EMITTER_2();
		DESTRUCTOR ~MODEL_PARTICLE_EMITTER_2();

		VOID Clear();
		INT GetSize();

		MODEL_PARTICLE_EMITTER_2_DATA& Data();

	protected:
		MODEL_PARTICLE_EMITTER_2_DATA ParticleEmitter2Data;

	public:
		REFERENCE<MODEL_PARTICLE_EMITTER_2*, MODEL_TEXTURE*> TextureNode;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
