//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ModelModel.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_MODEL::MODEL_MODEL()
{
	/*
	RegisterModelValue(&ModelData.NumGeosets);
	RegisterModelValue(&ModelData.NumGeosetAnims);
	RegisterModelValue(&ModelData.NumHelpers);
	RegisterModelValue(&ModelData.NumBones);
	RegisterModelValue(&ModelData.NumLights);
	RegisterModelValue(&ModelData.NumAttachments);
	RegisterModelValue(&ModelData.NumEvents);
	RegisterModelValue(&ModelData.NumParticleEmitters);
	RegisterModelValue(&ModelData.NumParticleEmitters2);
	RegisterModelValue(&ModelData.NumRibbonEmitters);
	RegisterModelValue(&ModelData.BlendTime);
	RegisterModelValue(&ModelData.MinimumExtent);
	RegisterModelValue(&ModelData.MaximumExtent);
	RegisterModelValue(&ModelData.BoundsRadius);
	*/
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL_MODEL::~MODEL_MODEL()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Clears the model model
//+-----------------------------------------------------------------------------
VOID MODEL_MODEL::Clear()
{
	ModelData = MODEL_MODEL_DATA();
}


//+-----------------------------------------------------------------------------
//| Saves the model model
//+-----------------------------------------------------------------------------
BOOL MODEL_MODEL::Save(TOKEN_OUT_STREAM& TokenStream)
{
	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model model
//+-----------------------------------------------------------------------------
BOOL MODEL_MODEL::Load(TOKEN_IN_STREAM& TokenStream)
{
	BOOL ErrorOccured = false;
	std::string Token;

	while(!TokenStream.Eof())
	{
		Token = TokenStream.ReadWord();
		if(Token == "") break;

		if(Token == "}")
		{
			return TRUE;
		}
		/*
		else if(BasicLoad(ErrorOccured, TokenStream, Token))
		{
			if(ErrorOccured) return FALSE;
		}
		*/
		else
		{
			Error.SetMessage("Unable to load \"" + TokenStream.GetFileName() + "\", unknown token \"" + Token + "\" in \"Model\"!");
			return FALSE;
		}
	}

	Error.SetMessage("Unable to load \"" + TokenStream.GetFileName() + "\", unexpected EOF reached!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Saves the model model
//+-----------------------------------------------------------------------------
BOOL MODEL_MODEL::Save(DATA_OUT_STREAM& DataStream)
{
	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model model
//+-----------------------------------------------------------------------------
BOOL MODEL_MODEL::Load(DATA_IN_STREAM& DataStream)
{
	//SkipGroup(DataStream);
	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Returns the size of the mdx chunk
//+-----------------------------------------------------------------------------
INT MODEL_MODEL::GetMdxSize()
{
	//XXXXXXXXXXXXXXX

	return 0;
}


//+-----------------------------------------------------------------------------
//| Returns a reference to the data
//+-----------------------------------------------------------------------------
MODEL_MODEL_DATA& MODEL_MODEL::Data()
{
	return ModelData;
}
