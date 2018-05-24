//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ModelMaterialLayer.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_MATERIAL_LAYER::MODEL_MATERIAL_LAYER()
{
	TextureNode.SetData(this);
	NewTextureNode.SetData(this);
	TextureAnimationNode.SetData(this);
	NewTextureAnimationNode.SetData(this);

	MaterialLayerChanged = FALSE;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL_MATERIAL_LAYER::~MODEL_MATERIAL_LAYER()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Clears the material layer
//+-----------------------------------------------------------------------------
VOID MODEL_MATERIAL_LAYER::Clear()
{
	MaterialLayerData = MODEL_MATERIAL_LAYER_DATA();
	MaterialLayerNewData = MODEL_MATERIAL_LAYER_DATA();
}


//+-----------------------------------------------------------------------------
//| Returns the mdx size of the material layer
//+-----------------------------------------------------------------------------
INT MODEL_MATERIAL_LAYER::GetSize()
{
	return 28 + MaterialLayerData.AnimatedTextureId.GetSize() + MaterialLayerData.Alpha.GetSize();
}


//+-----------------------------------------------------------------------------
//| Returns a reference to the data
//+-----------------------------------------------------------------------------
MODEL_MATERIAL_LAYER_DATA& MODEL_MATERIAL_LAYER::Data()
{
	return MaterialLayerData;
}


//+-----------------------------------------------------------------------------
//| Returns a reference to the new data
//+-----------------------------------------------------------------------------
MODEL_MATERIAL_LAYER_DATA& MODEL_MATERIAL_LAYER::NewData()
{
	return MaterialLayerNewData;
}


//+-----------------------------------------------------------------------------
//| Marks the layer as updated
//+-----------------------------------------------------------------------------
VOID MODEL_MATERIAL_LAYER::MarkAsUpdated()
{
	MaterialLayerChanged = TRUE;
}


//+-----------------------------------------------------------------------------
//| Updates the material layer if neccessary
//+-----------------------------------------------------------------------------
VOID MODEL_MATERIAL_LAYER::UpdateDataIfNeccessary()
{
	if(MaterialLayerChanged)
	{
		MaterialLayerData = MaterialLayerNewData;
		MaterialLayerNewData = MODEL_MATERIAL_LAYER_DATA();

		TextureNode = NewTextureNode;
		NewTextureNode.Detach();

		TextureAnimationNode = NewTextureAnimationNode;
		NewTextureAnimationNode.Detach();

		MaterialLayerChanged = FALSE;
	}
}


//+-----------------------------------------------------------------------------
//| Returns the render order
//+-----------------------------------------------------------------------------
INT MODEL_MATERIAL_LAYER::GetRenderOrder()
{
	switch(MaterialLayerData.FilterMode)
	{
		case FILTER_MODE_NONE:
		{
			return 1;
		}

		case FILTER_MODE_TRANSPARENT:
		{
			return 2;
		}

		case FILTER_MODE_BLEND:
		{
			return 3;
		}

		case FILTER_MODE_ADDITIVE:
		case FILTER_MODE_ADD_ALPHA:
		case FILTER_MODE_MODULATE:
		{
			return 4;
		}
	}

	return 1;
}


//+-----------------------------------------------------------------------------
//| Uses the material
//+-----------------------------------------------------------------------------
VOID MODEL_MATERIAL_LAYER::UseMaterial(CONST SEQUENCE_TIME& Time)
{
	std::string FileName;
	LPDIRECT3DDEVICE9 Direct3DDevice;
	INT TextureId;
	MODEL_TEXTURE* ModelTexture = NULL;
	TEXTURE* Texture = NULL;

	Direct3DDevice = Graphics.GetDevice();
	if(Direct3DDevice == NULL) return;

	Graphics.SetShading(!MaterialLayerData.Unshaded);
	Graphics.SetCulling(!MaterialLayerData.TwoSided);

	Graphics.SetShader();
	Graphics.SetShaderConstants();

	if(MaterialLayerData.AnimatedTextureId.IsStatic())
	{
		if(TextureNode.IsAttached())
		{
			ModelTexture = TextureNode.GetObjectData();
		}
	}
	else
	{
		SEQUENCE_TIME TempTime;

		if(MaterialLayerData.AnimatedTextureId.GlobalSequenceNode.IsAttached())
		{
			MODEL_GLOBAL_SEQUENCE* GlobalSequence;

			GlobalSequence = MaterialLayerData.AnimatedTextureId.GlobalSequenceNode.GetObjectData();

			TempTime.Time = GlobalSequence->Data().Time;
			TempTime.IntervalStart = 0;
			TempTime.IntervalEnd = GlobalSequence->Data().Duration;
		}
		else
		{
			TempTime = Time;
		}

		TextureId = static_cast<INT>(MaterialLayerData.AnimatedTextureId.GetScalar(TempTime));
		ModelTexture = Model.GetTexture(TextureId);
	}

	if(ModelTexture == NULL)
	{
		Direct3DDevice->SetTexture(0, NULL);
		return;
	}

	FileName = ModelTexture->Data().FileName;
	if(FileName == "")
	{
		Texture = TextureManager.GetReplaceableTexture(ModelTexture->Data().ReplaceableId);
	}
	else
	{
		Texture = TextureManager.GetTexture(FileName);
	}

	if(Texture == NULL)
	{
		Direct3DDevice->SetTexture(0, NULL);
		return;
	}

	Direct3DDevice->SetTexture(0, Texture->GetTexture());

	Direct3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, (ModelTexture->Data().WrapWidth ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP));
	Direct3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, (ModelTexture->Data().WrapHeight ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP));

	switch(MaterialLayerData.FilterMode)
	{
		case FILTER_MODE_NONE:
		{
			Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			Direct3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			Direct3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			Direct3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

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

			Direct3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			Direct3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			break;
		}

		case FILTER_MODE_BLEND:
		{
			Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			Direct3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			Direct3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			Direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			Direct3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			Direct3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			break;
		}

		case FILTER_MODE_ADDITIVE:
		{
			Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			Direct3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			Direct3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
			Direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			Direct3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			Direct3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			break;
		}

		case FILTER_MODE_ADD_ALPHA:
		{
			Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			Direct3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			Direct3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			Direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			Direct3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			Direct3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			break;
		}

		case FILTER_MODE_MODULATE:
		{
			Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			Direct3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			Direct3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
			Direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);

			Direct3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			Direct3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			break;
		}
	}

	if(MaterialLayerData.NoDepthTest) Direct3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	if(MaterialLayerData.NoDepthSet) Direct3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
}
