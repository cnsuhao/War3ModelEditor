//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ModelMaterial.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_MATERIAL::MODEL_MATERIAL()
{
	GeosetNodes.SetData(this);
	RibbonEmitterNodes.SetData(this);
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL_MATERIAL::~MODEL_MATERIAL()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Clears the material
//+-----------------------------------------------------------------------------
VOID MODEL_MATERIAL::Clear()
{
	SAFE_CLEAR(MaterialData.LayerContainer);

	MaterialData = MODEL_MATERIAL_DATA();
}


//+-----------------------------------------------------------------------------
//| Returns the mdx size of the material
//+-----------------------------------------------------------------------------
INT MODEL_MATERIAL::GetSize()
{
	INT i;
	INT Size = 20;

	for(i = 0; i < MaterialData.LayerContainer.GetTotalSize(); i++)
	{
		if(MaterialData.LayerContainer.ValidIndex(i))
		{
			Size += MaterialData.LayerContainer[i]->GetSize();
		}
	}

	return Size;
}


//+-----------------------------------------------------------------------------
//| Returns a reference to the data
//+-----------------------------------------------------------------------------
MODEL_MATERIAL_DATA& MODEL_MATERIAL::Data()
{
	return MaterialData;
}


//+-----------------------------------------------------------------------------
//| Returns the render order
//+-----------------------------------------------------------------------------
INT MODEL_MATERIAL::GetRenderOrder()
{
	INT i;
	INT RenderOrder;
	INT CurrentRenderOrder = 1;

	for(i = 0; i < MaterialData.LayerContainer.GetTotalSize(); i++)
	{
		if(MaterialData.LayerContainer.ValidIndex(i))
		{
			RenderOrder = MaterialData.LayerContainer[i]->GetRenderOrder();
			if(RenderOrder > CurrentRenderOrder) CurrentRenderOrder = RenderOrder;
		}
	}

	return CurrentRenderOrder;
}


//+-----------------------------------------------------------------------------
//| Adds a layer
//+-----------------------------------------------------------------------------
BOOL MODEL_MATERIAL::AddLayer(MODEL_MATERIAL_LAYER* Layer)
{
	if(!MaterialData.LayerContainer.Add(Layer))
	{
		Error.SetMessage("Unable to add a new material layer!");
		return FALSE;
	}

	return TRUE;
}
