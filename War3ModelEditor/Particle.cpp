//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Particle.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
PARTICLE::PARTICLE()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
PARTICLE::~PARTICLE()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Creates the particle
//+-----------------------------------------------------------------------------
VOID PARTICLE::Create(CONST SEQUENCE_TIME& SequenceTime, MODEL_PARTICLE_EMITTER_2* ParticleEmitter2)
{
	FLOAT Speed;
	FLOAT Width;
	FLOAT Length;
	FLOAT Latitude;
	D3DXVECTOR3 PivotPoint;
	D3DXVECTOR3 VelocityStart;
	D3DXVECTOR3 VelocityEnd;
	D3DXVECTOR3 TempVector;
	D3DXVECTOR4 TempVector2;
	D3DXMATRIX RotationMatrix;
	D3DXMATRIX RotationMatrixY;
	D3DXMATRIX RotationMatrixZ;

	Speed = ParticleEmitter2->Data().Speed.GetScalar(SequenceTime);
	Width = ParticleEmitter2->Data().Width.GetScalar(SequenceTime) / 2.0f;
	Length = ParticleEmitter2->Data().Length.GetScalar(SequenceTime) / 2.0f;
	Latitude = ParticleEmitter2->Data().Latitude.GetScalar(SequenceTime) * (D3DX_PI / 180.0f);
	Variation = ParticleEmitter2->Data().Variation.GetScalar(SequenceTime);

	Speed *= (1.0f + Random.Float(0.0f, Variation));
	PivotPoint = ParticleEmitter2->Data().PivotPoint;

	TempVector = D3DXVECTOR3(PivotPoint.x + Random.Float(-Width, Width), PivotPoint.y + Random.Float(-Length, Length), PivotPoint.z);
	D3DXVec3Transform(&TempVector2, &TempVector, &ParticleEmitter2->Matrix());
	Position = D3DXVECTOR3(TempVector2.x, TempVector2.y, TempVector2.z);

	D3DXMatrixRotationY(&RotationMatrixY, Random.Float(0, Latitude));
	D3DXMatrixRotationZ(&RotationMatrixZ, Random.Float(0, 2 * D3DX_PI));
	D3DXMatrixMultiply(&RotationMatrix, &RotationMatrixY, &RotationMatrixZ);
	D3DXVec3Transform(&TempVector2, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), &RotationMatrix);
	Velocity = D3DXVECTOR3(TempVector2.x, TempVector2.y, TempVector2.z);
	D3DXVec3Normalize(&Velocity, &Velocity);

	VelocityStart = Position;
	VelocityEnd = Position + Velocity;

	D3DXVec3Transform(&TempVector2, &VelocityStart, &ParticleEmitter2->Matrix());
	VelocityStart = D3DXVECTOR3(TempVector2.x, TempVector2.y, TempVector2.z);
	D3DXVec3Transform(&TempVector2, &VelocityEnd, &ParticleEmitter2->Matrix());
	VelocityEnd = D3DXVECTOR3(TempVector2.x, TempVector2.y, TempVector2.z);
	D3DXVec3Normalize(&Velocity, &(VelocityEnd - VelocityStart));
	Velocity *= Speed;

	FilterMode = ParticleEmitter2->Data().FilterMode;
	TextureFileName = ParticleEmitter2->TextureNode.IsAttached() ? ParticleEmitter2->TextureNode.GetObjectData()->Data().FileName : "";

	SegmentColor1 = ParticleEmitter2->Data().SegmentColor1;
	SegmentColor2 = ParticleEmitter2->Data().SegmentColor2;
	SegmentColor3 = ParticleEmitter2->Data().SegmentColor3;
	Alpha = ParticleEmitter2->Data().Alpha;
	ParticleScaling = ParticleEmitter2->Data().ParticleScaling;
	HeadLifeSpan = ParticleEmitter2->Data().HeadLifeSpan;
	HeadDecay = ParticleEmitter2->Data().HeadDecay;
	TailLifeSpan = ParticleEmitter2->Data().TailLifeSpan;
	TailDecay = ParticleEmitter2->Data().TailDecay;

	Rows = ParticleEmitter2->Data().Rows;
	Columns = ParticleEmitter2->Data().Columns;
	ReplaceableId = ParticleEmitter2->Data().ReplaceableId;
	Time = ParticleEmitter2->Data().Time;
	Gravity = ParticleEmitter2->Data().Gravity.GetScalar(SequenceTime);
	LifeSpan = ParticleEmitter2->Data().LifeSpan;
	InitialLifeSpan = LifeSpan;
	TailLength = ParticleEmitter2->Data().TailLength;

	SortPrimitivesFarZ = ParticleEmitter2->Data().SortPrimitivesFarZ;
	LineEmitter = ParticleEmitter2->Data().LineEmitter;
	ModelSpace = ParticleEmitter2->Data().ModelSpace;
	AlphaKey = ParticleEmitter2->Data().AlphaKey;
	Unshaded = ParticleEmitter2->Data().Unshaded;
	Unfogged = ParticleEmitter2->Data().Unfogged;
	XYQuad = ParticleEmitter2->Data().XYQuad;
	Squirt = ParticleEmitter2->Data().Squirt;
	Head = ParticleEmitter2->Data().Head;
	Tail = ParticleEmitter2->Data().Tail;

	CurrentFrame = 0;

	if(Head)
	{
		NrOfHeadFrames = static_cast<INT>((HeadLifeSpan.y - HeadLifeSpan.x + 1.0f) * HeadLifeSpan.z);
		NrOfHeadDecayFrames = static_cast<INT>((HeadDecay.y - HeadDecay.x + 1.0f) * HeadDecay.z);
	}
	else
	{
		NrOfHeadFrames = 0;
		NrOfHeadDecayFrames = 0;
	}

	if(Tail)
	{
		NrOfTailFrames = static_cast<INT>((TailLifeSpan.y - TailLifeSpan.x + 1.0f) * TailLifeSpan.z);
		NrOfTailDecayFrames = static_cast<INT>((TailDecay.y - TailDecay.x + 1.0f) * TailDecay.z);
	}
	else
	{
		NrOfTailFrames = 0;
		NrOfTailDecayFrames = 0;
	}
}


//+-----------------------------------------------------------------------------
//| Clears the particle
//+-----------------------------------------------------------------------------
VOID PARTICLE::Clear()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Renders the particle
//+-----------------------------------------------------------------------------
VOID PARTICLE::Render()
{
	FLOAT Scale;
	FLOAT LifeFactor;
	FLOAT TempFactor;
	FLOAT TempInterval;
	FLOAT CellWidth;
	FLOAT CellHeight;
	FLOAT Left;
	FLOAT Top;
	FLOAT Right;
	FLOAT Bottom;
	INT Row;
	INT Column;
	INT Index;
	INT FrameStart;
	INT FrameEnd;
	INT FrameInterval;
	INT FrameIntervalStart;
	INT TotalNrOfFrames;
	D3DCOLOR Color;
	D3DXCOLOR ColorVector;
	D3DXCOLOR ColorVector1;
	D3DXCOLOR ColorVector2;
	D3DXCOLOR ColorVector3;
	TEXTURE* Texture;
	D3DXMATRIX WorldMatrix;
	D3DXMATRIX ScaleMatrix;
	D3DXMATRIX PositionMatrix;
	D3DXQUATERNION CameraRotation;
	D3DXMATRIX RotationMatrix;
	D3DXMATRIX CameraRotationMatrix;
	D3DXMATRIX CameraRotationMatrixY;
	D3DXMATRIX CameraRotationMatrixZ;
	LPDIRECT3DDEVICE9 Direct3DDevice;

	Direct3DDevice = Graphics.GetDevice();
	if(Direct3DDevice == NULL) return;

	LifeFactor = (InitialLifeSpan == 0.0f) ? 0.0f : (1.0f - (LifeSpan / InitialLifeSpan));

	Direct3DDevice->SetVertexShader(NULL);
	Direct3DDevice->SetPixelShader(NULL);
	Direct3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	Direct3DDevice->LightEnable(0, FALSE);

	if(TextureFileName == "")
	{
		Texture = TextureManager.GetReplaceableTexture(ReplaceableId);
	}
	else
	{
		Texture = TextureManager.GetTexture(TextureFileName);
	}

	if(Texture == NULL)
	{
		Direct3DDevice->SetTexture(0, NULL);
	}
	else
	{
		Direct3DDevice->SetTexture(0, Texture->GetTexture());
	}

	switch(FilterMode)
	{
	case FILTER_MODE_NONE:
		{
			Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			Direct3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			break;
		}

	case FILTER_MODE_TRANSPARENT:
		{
			Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			Direct3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

			Direct3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			Direct3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x0000000BE);

			Direct3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			Direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			break;
		}

	case FILTER_MODE_BLEND:
		{
			Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			Direct3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			Direct3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			Direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			break;
		}

	case FILTER_MODE_ADDITIVE:
		{
			Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			Direct3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			Direct3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			Direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			break;
		}

	case FILTER_MODE_ADD_ALPHA:
		{
			Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			Direct3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			Direct3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			Direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			break;
		}

	case FILTER_MODE_MODULATE:
		{
			Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			Direct3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			Direct3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
			Direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);

			break;
		}
	}

	Direct3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	Direct3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	CellWidth = (Columns == 0) ? 1.0f : (1.0f / Columns);
	CellHeight = (Rows == 0) ? 1.0f : (1.0f / Rows);

	ColorVector1.r = SegmentColor1.z;
	ColorVector1.g = SegmentColor1.y;
	ColorVector1.b = SegmentColor1.x;
	ColorVector1.a = Alpha.x / 255.0f;

	ColorVector2.r = SegmentColor2.z;
	ColorVector2.g = SegmentColor2.y;
	ColorVector2.b = SegmentColor2.x;
	ColorVector2.a = Alpha.y / 255.0f;

	ColorVector3.r = SegmentColor3.z;
	ColorVector3.g = SegmentColor3.y;
	ColorVector3.b = SegmentColor3.x;
	ColorVector3.a = Alpha.z / 255.0f;

	if(LifeFactor < Time)
	{
		TempInterval = Time;
		TempFactor = (TempInterval == 0.0f) ? 0.0f : (LifeFactor / TempInterval);

		Scale = ParticleScaling.x + TempFactor * (ParticleScaling.y - ParticleScaling.x);
		D3DXColorLerp(&ColorVector, &ColorVector1, &ColorVector2, TempFactor);
	}
	else
	{
		TempInterval = 1.0f - Time;
		TempFactor = (TempInterval == 0.0f) ? 0.0f : ((LifeFactor - Time) / TempInterval);

		Scale = ParticleScaling.y + TempFactor * (ParticleScaling.z - ParticleScaling.y);
		D3DXColorLerp(&ColorVector, &ColorVector2, &ColorVector3, TempFactor);
	}

	Color = ColorVector;

	TotalNrOfFrames = NrOfHeadFrames + NrOfHeadDecayFrames + NrOfTailFrames + NrOfTailDecayFrames;
	CurrentFrame = static_cast<INT>(LifeFactor * TotalNrOfFrames);

	if(CurrentFrame < NrOfHeadFrames)
	{
		FrameStart = static_cast<INT>(HeadLifeSpan.x);
		FrameEnd = static_cast<INT>(HeadLifeSpan.y);
		FrameInterval = FrameEnd - FrameStart + 1;
		FrameIntervalStart = 0;

		Index = FrameStart + ((CurrentFrame - FrameIntervalStart) % FrameInterval);
	}
	else if(CurrentFrame < (NrOfHeadFrames + NrOfHeadDecayFrames))
	{
		FrameStart = static_cast<INT>(HeadDecay.x);
		FrameEnd = static_cast<INT>(HeadDecay.y);
		FrameInterval = FrameEnd - FrameStart + 1;
		FrameIntervalStart = NrOfHeadFrames;

		Index = FrameStart + ((CurrentFrame - FrameIntervalStart) % FrameInterval);
	}
	else if(CurrentFrame < (NrOfHeadFrames + NrOfHeadDecayFrames + NrOfTailFrames))
	{
		FrameStart = static_cast<INT>(TailLifeSpan.x);
		FrameEnd = static_cast<INT>(TailLifeSpan.y);
		FrameInterval = FrameEnd - FrameStart + 1;
		FrameIntervalStart = NrOfHeadFrames + NrOfHeadDecayFrames;

		Index = FrameStart + ((CurrentFrame - FrameIntervalStart) % FrameInterval);
	}
	else if(CurrentFrame < (NrOfHeadFrames + NrOfHeadDecayFrames + NrOfTailFrames + NrOfTailDecayFrames))
	{
		FrameStart = static_cast<INT>(TailDecay.x);
		FrameEnd = static_cast<INT>(TailDecay.y);
		FrameInterval = FrameEnd - FrameStart + 1;
		FrameIntervalStart = NrOfHeadFrames + NrOfHeadDecayFrames + NrOfTailFrames;

		Index = FrameStart + ((CurrentFrame - FrameIntervalStart) % FrameInterval);
	}
	else
	{
		Index = 0;
	}

	Row = (Columns == 0) ? 0 : (Index / Columns);
	Column = (Columns == 0) ? 0 : (Index % Columns);

	Left = CellWidth * Column;
	Top = CellHeight * Row;
	Right = CellWidth * (Column + 1);
	Bottom = CellHeight * (Row + 1);

	if(Head)
	{
		D3DXMatrixRotationY(&CameraRotationMatrixY, (XYQuad ? -(D3DX_PI / 2.0f) : -Camera.GetPitch()));
		D3DXMatrixRotationZ(&CameraRotationMatrixZ, Camera.GetYaw());
		D3DXMatrixMultiply(&CameraRotationMatrix, &CameraRotationMatrixY, &CameraRotationMatrixZ);
		D3DXQuaternionRotationMatrix(&CameraRotation, &CameraRotationMatrix);
		D3DXMatrixTransformation(&CameraRotationMatrix, NULL, NULL, NULL, NULL, &CameraRotation, NULL);

		D3DXMatrixScaling(&ScaleMatrix, Scale, Scale, Scale);
		D3DXMatrixTranslation(&PositionMatrix, Position.x, Position.y, Position.z);
		D3DXMatrixMultiply(&WorldMatrix, &CameraRotationMatrix, &PositionMatrix);
		D3DXMatrixMultiply(&WorldMatrix, &ScaleMatrix, &WorldMatrix);

		Graphics.SetWorldMatrix(WorldMatrix);

		Graphics.RenderParticle(Color, Left, Top, Right, Bottom);
	}

	if(Tail)
	{
		//Implement me
	}
}


//+-----------------------------------------------------------------------------
//| Updates the particle
//+-----------------------------------------------------------------------------
BOOL PARTICLE::Update(INT TimeDifference)
{
	FLOAT TimeFactor;

	TimeFactor = static_cast<FLOAT>(TimeDifference) / 1000.0f;

	LifeSpan -= TimeFactor;
	if(LifeSpan <= 0.0f) return FALSE;

	Velocity.z -= (Gravity * TimeFactor);
	Position += (Velocity * TimeFactor);

	return TRUE;
}
