//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ModelLoaderMdl.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
MODEL_LOADER_MDL ModelLoaderMdl;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_LOADER_MDL::MODEL_LOADER_MDL()
{
	RegisterMdlLoader("Version", LoadVersion);
	RegisterMdlLoader("Model", LoadModel);
	RegisterMdlLoader("Sequences", LoadSequences);
	RegisterMdlLoader("GlobalSequences", LoadGlobalSequences);
	RegisterMdlLoader("Textures", LoadTextures);
	RegisterMdlLoader("Materials", LoadMaterials);
	RegisterMdlLoader("TextureAnims", LoadTextureAnimations);
	RegisterMdlLoader("Geoset", LoadGeosets);
	RegisterMdlLoader("GeosetAnim", LoadGeosetAnimations);
	RegisterMdlLoader("Bone", LoadBones);
	RegisterMdlLoader("Light", LoadLights);
	RegisterMdlLoader("Helper", LoadHelpers);
	RegisterMdlLoader("Attachment", LoadAttachments);
	RegisterMdlLoader("PivotPoints", LoadPivotPoints);
	RegisterMdlLoader("ParticleEmitter", LoadParticleEmitters);
	RegisterMdlLoader("ParticleEmitter2", LoadParticleEmitters2);
	RegisterMdlLoader("RibbonEmitter", LoadRibbonEmitters);
	RegisterMdlLoader("EventObject", LoadEventObjects);
	RegisterMdlLoader("Camera", LoadCameras);
	RegisterMdlLoader("CollisionShape", LoadCollisionShapes);
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL_LOADER_MDL::~MODEL_LOADER_MDL()
{
	MdlLoaderMap.clear();
}


//+-----------------------------------------------------------------------------
//| Saves a model to a buffer
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::Save(MODEL& Model, CONST std::string& FileName, BUFFER& Buffer)
{
	TOKEN_OUT_STREAM TokenStream;

	CurrentFileName = FileName;
	TokenStream.SetFileName(FileName);

	Model.GenerateObjectIds();
	Model.WrapPivotPoints();

	if(!SaveVersion(Model, TokenStream)) return FALSE;
	if(!SaveModel(Model, TokenStream)) return FALSE;
	if(!SaveSequences(Model, TokenStream)) return FALSE;
	if(!SaveGlobalSequences(Model, TokenStream)) return FALSE;
	if(!SaveTextures(Model, TokenStream)) return FALSE;
	if(!SaveMaterials(Model, TokenStream)) return FALSE;
	if(!SaveTextureAnimations(Model, TokenStream)) return FALSE;
	if(!SaveGeosets(Model, TokenStream)) return FALSE;
	if(!SaveGeosetAnimations(Model, TokenStream)) return FALSE;
	if(!SaveBones(Model, TokenStream)) return FALSE;
	if(!SaveLights(Model, TokenStream)) return FALSE;
	if(!SaveHelpers(Model, TokenStream)) return FALSE;
	if(!SaveAttachments(Model, TokenStream)) return FALSE;
	if(!SavePivotPoints(Model, TokenStream)) return FALSE;
	if(!SaveParticleEmitters(Model, TokenStream)) return FALSE;
	if(!SaveParticleEmitters2(Model, TokenStream)) return FALSE;
	if(!SaveRibbonEmitters(Model, TokenStream)) return FALSE;
	if(!SaveEventObjects(Model, TokenStream)) return FALSE;
	if(!SaveCameras(Model, TokenStream)) return FALSE;
	if(!SaveCollisionShapes(Model, TokenStream)) return FALSE;

	if(!TokenStream.Save(Buffer)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model from a buffer
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::Load(MODEL& Model, CONST std::string& FileName, BUFFER& Buffer)
{
	std::string Token;
	TOKEN_IN_STREAM TokenStream;
	std::map<std::string, MDL_LOADER>::iterator i;

	CurrentFileName = FileName;
	TokenStream.SetFileName(FileName);

	if(!TokenStream.Load(Buffer)) return FALSE;

	while(!TokenStream.Eof())
	{
		Token = TokenStream.ReadWord();

		Common.RemoveWhiteSpace(Token);
		if(Token == "") break;

		i = MdlLoaderMap.find(Token);
		if(i == MdlLoaderMap.end())
		{
			Error.SetMessage("Unable to load \"" + FileName + "\", unknown group \"" + Token + "\"!");
			return FALSE;
		}

		TokenStream.SetGroup(Token);
		if(!(i->second(Model, TokenStream))) return FALSE;
	}

	Model.ConnectNodes();
	Model.UnwrapPivotPoints();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Registers an mdl loader
//+-----------------------------------------------------------------------------
VOID MODEL_LOADER_MDL::RegisterMdlLoader(CONST std::string& Name, MDL_LOADER Loader)
{
	MdlLoaderMap.insert(std::make_pair(Name, Loader));
}


//+-----------------------------------------------------------------------------
//| Saves the model version
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveVersion(MODEL& Model, TOKEN_OUT_STREAM& TokenStream)
{
	TokenStream.WriteLine("//+-----------------------------------------------------------------------------");
	TokenStream.WriteLine("//| " + Common.GetFileName(CurrentFileName));
	TokenStream.WriteLine("//| Generated by " + EDITOR_NAME);
	TokenStream.WriteLine("//| " + GetCurrentDate());
	TokenStream.WriteLine("//| http://Magos.TheJeffFiles.com/War3ModelEditor/");
	TokenStream.WriteLine("//+-----------------------------------------------------------------------------");

	TokenStream.WriteLine("Version {");
	TokenStream.WriteTab();
	TokenStream.WriteWord("FormatVersion ");
	TokenStream.WriteInt(Model.Data().Info.Version);
	TokenStream.WriteLine(",");
	TokenStream.WriteLine("}");

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model info
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveModel(MODEL& Model, TOKEN_OUT_STREAM& TokenStream)
{
	TokenStream.WriteWord("Model ");
	TokenStream.WriteString(Model.Data().Info.Name);
	TokenStream.WriteLine(" {");

	if(Model.Data().GeosetContainer.GetSize() > 0)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("NumGeosets ");
		TokenStream.WriteInt(Model.Data().GeosetContainer.GetSize());
		TokenStream.WriteLine(",");
	}

	if(Model.Data().GeosetAnimationContainer.GetSize() > 0)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("NumGeosetAnims ");
		TokenStream.WriteInt(Model.Data().GeosetAnimationContainer.GetSize());
		TokenStream.WriteLine(",");
	}

	if(Model.Data().HelperContainer.GetSize() > 0)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("NumHelpers ");
		TokenStream.WriteInt(Model.Data().HelperContainer.GetSize());
		TokenStream.WriteLine(",");
	}

	if(Model.Data().BoneContainer.GetSize() > 0)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("NumBones ");
		TokenStream.WriteInt(Model.Data().BoneContainer.GetSize());
		TokenStream.WriteLine(",");
	}

	if(Model.Data().LightContainer.GetSize() > 0)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("NumLights ");
		TokenStream.WriteInt(Model.Data().LightContainer.GetSize());
		TokenStream.WriteLine(",");
	}

	if(Model.Data().AttachmentContainer.GetSize() > 0)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("NumAttachments ");
		TokenStream.WriteInt(Model.Data().AttachmentContainer.GetSize());
		TokenStream.WriteLine(",");
	}

	if(Model.Data().EventObjectContainer.GetSize() > 0)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("NumEvents ");
		TokenStream.WriteInt(Model.Data().EventObjectContainer.GetSize());
		TokenStream.WriteLine(",");
	}

	if(Model.Data().ParticleEmitterContainer.GetSize() > 0)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("NumParticleEmitters ");
		TokenStream.WriteInt(Model.Data().ParticleEmitterContainer.GetSize());
		TokenStream.WriteLine(",");
	}

	if(Model.Data().ParticleEmitter2Container.GetSize() > 0)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("NumParticleEmitters2 ");
		TokenStream.WriteInt(Model.Data().ParticleEmitter2Container.GetSize());
		TokenStream.WriteLine(",");
	}

	if(Model.Data().RibbonEmitterContainer.GetSize() > 0)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("NumRibbonEmitters ");
		TokenStream.WriteInt(Model.Data().RibbonEmitterContainer.GetSize());
		TokenStream.WriteLine(",");
	}

	TokenStream.WriteTab();
	TokenStream.WriteWord("BlendTime ");
	TokenStream.WriteInt(Model.Data().Info.BlendTime);
	TokenStream.WriteLine(",");

	if(Model.Data().Info.Extent.Min != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("MinimumExtent ");
		TokenStream.WriteVector3(Model.Data().Info.Extent.Min);
		TokenStream.WriteLine(",");
	}

	if(Model.Data().Info.Extent.Max != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("MaximumExtent ");
		TokenStream.WriteVector3(Model.Data().Info.Extent.Max);
		TokenStream.WriteLine(",");
	}

	if(Model.Data().Info.Extent.Radius > 0.0f)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("BoundsRadius ");
		TokenStream.WriteFloat(Model.Data().Info.Extent.Radius);
		TokenStream.WriteLine(",");
	}

	if(Model.Data().Info.AnimationFile != "")
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("AnimationFile ");
		TokenStream.WriteString(Model.Data().Info.AnimationFile);
		TokenStream.WriteLine(",");
	}

	TokenStream.WriteLine("}");

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model sequences
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveSequences(MODEL& Model, TOKEN_OUT_STREAM& TokenStream)
{
	INT i;

	if(Model.Data().SequenceContainer.GetSize() <= 0) return TRUE;

	TokenStream.WriteWord("Sequences ");
	TokenStream.WriteInt(Model.Data().SequenceContainer.GetSize());
	TokenStream.WriteLine(" {");

	for(i = 0; i < Model.Data().SequenceContainer.GetTotalSize(); i++)
	{
		if(Model.Data().SequenceContainer.ValidIndex(i))
		{
			if(!SaveSequence(*(Model.Data().SequenceContainer[i]), TokenStream)) return FALSE;
		}
	}

	TokenStream.WriteLine("}");

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model sequence
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveSequence(MODEL_SEQUENCE& Sequence, TOKEN_OUT_STREAM& TokenStream)
{
	TokenStream.WriteTab();
	TokenStream.WriteWord("Anim ");
	TokenStream.WriteString(Sequence.Data().Name);
	TokenStream.WriteLine(" {");

	TokenStream.WriteTab(2);
	TokenStream.WriteWord("Interval ");
	TokenStream.WriteVector2(Sequence.Data().Interval);
	TokenStream.WriteLine(",");

	if(Sequence.Data().Rarity != 0.0f)
	{
		TokenStream.WriteTab(2);
		TokenStream.WriteWord("Rarity ");
		TokenStream.WriteFloat(Sequence.Data().Rarity);
		TokenStream.WriteLine(",");
	}

	if(Sequence.Data().MoveSpeed != 0.0f)
	{
		TokenStream.WriteTab(2);
		TokenStream.WriteWord("MoveSpeed ");
		TokenStream.WriteFloat(Sequence.Data().MoveSpeed);
		TokenStream.WriteLine(",");
	}

	if(Sequence.Data().NonLooping)
	{
		TokenStream.WriteTab(2);
		TokenStream.WriteLine("NonLooping,");
	}

	if(Sequence.Data().Extent.Min != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		TokenStream.WriteTab(2);
		TokenStream.WriteWord("MinimumExtent ");
		TokenStream.WriteVector3(Sequence.Data().Extent.Min);
		TokenStream.WriteLine(",");
	}

	if(Sequence.Data().Extent.Max != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		TokenStream.WriteTab(2);
		TokenStream.WriteWord("MaximumExtent ");
		TokenStream.WriteVector3(Sequence.Data().Extent.Max);
		TokenStream.WriteLine(",");
	}

	if(Sequence.Data().Extent.Radius > 0.0f)
	{
		TokenStream.WriteTab(2);
		TokenStream.WriteWord("BoundsRadius ");
		TokenStream.WriteFloat(Sequence.Data().Extent.Radius);
		TokenStream.WriteLine(",");
	}

	TokenStream.WriteTab();
	TokenStream.WriteLine("}");

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the global model sequences
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveGlobalSequences(MODEL& Model, TOKEN_OUT_STREAM& TokenStream)
{
	INT i;

	if(Model.Data().GlobalSequenceContainer.GetSize() <= 0) return TRUE;

	TokenStream.WriteWord("GlobalSequences ");
	TokenStream.WriteInt(Model.Data().GlobalSequenceContainer.GetSize());
	TokenStream.WriteLine(" {");

	for(i = 0; i < Model.Data().GlobalSequenceContainer.GetTotalSize(); i++)
	{
		if(Model.Data().GlobalSequenceContainer.ValidIndex(i))
		{
			TokenStream.WriteTab();
			TokenStream.WriteWord("Duration ");
			TokenStream.WriteInt(Model.Data().GlobalSequenceContainer[i]->Data().Duration);
			TokenStream.WriteLine(",");
		}
	}

	TokenStream.WriteLine("}");

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model textures
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveTextures(MODEL& Model, TOKEN_OUT_STREAM& TokenStream)
{
	INT i;

	if(Model.Data().TextureContainer.GetSize() <= 0) return TRUE;

	TokenStream.WriteWord("Textures ");
	TokenStream.WriteInt(Model.Data().TextureContainer.GetSize());
	TokenStream.WriteLine(" {");

	for(i = 0; i < Model.Data().TextureContainer.GetTotalSize(); i++)
	{
		if(Model.Data().TextureContainer.ValidIndex(i))
		{
			if(!SaveTexture(*(Model.Data().TextureContainer[i]), TokenStream)) return FALSE;
		}
	}

	TokenStream.WriteLine("}");

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model texture
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveTexture(MODEL_TEXTURE& Texture, TOKEN_OUT_STREAM& TokenStream)
{
	TokenStream.WriteTab();
	TokenStream.WriteLine("Bitmap {");

	TokenStream.WriteTab(2);
	TokenStream.WriteWord("Image ");
	TokenStream.WriteString(Texture.Data().FileName);
	TokenStream.WriteLine(",");

	if(Texture.Data().ReplaceableId > 0)
	{
		TokenStream.WriteTab(2);
		TokenStream.WriteWord("ReplaceableId ");
		TokenStream.WriteInt(Texture.Data().ReplaceableId);
		TokenStream.WriteLine(",");
	}

	if(Texture.Data().WrapWidth)
	{
		TokenStream.WriteTab(2);
		TokenStream.WriteLine("WrapWidth,");
	}

	if(Texture.Data().WrapHeight)
	{
		TokenStream.WriteTab(2);
		TokenStream.WriteLine("WrapHeight,");
	}

	TokenStream.WriteTab();
	TokenStream.WriteLine("}");

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model materials
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveMaterials(MODEL& Model, TOKEN_OUT_STREAM& TokenStream)
{
	INT i;

	if(Model.Data().MaterialContainer.GetSize() <= 0) return TRUE;

	TokenStream.WriteWord("Materials ");
	TokenStream.WriteInt(Model.Data().MaterialContainer.GetSize());
	TokenStream.WriteLine(" {");

	for(i = 0; i < Model.Data().MaterialContainer.GetTotalSize(); i++)
	{
		if(Model.Data().MaterialContainer.ValidIndex(i))
		{
			if(!SaveMaterial(*(Model.Data().MaterialContainer[i]), TokenStream)) return FALSE;
		}
	}

	TokenStream.WriteLine("}");

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model material
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveMaterial(MODEL_MATERIAL& Material, TOKEN_OUT_STREAM& TokenStream)
{
	INT i;
	MODEL_MATERIAL_LAYER* Layer;

	TokenStream.WriteTab();
	TokenStream.WriteLine("Material {");

	if(Material.Data().ConstantColor)
	{
		TokenStream.WriteTab(2);
		TokenStream.WriteLine("ConstantColor,");
	}

	if(Material.Data().SortPrimitivesFarZ)
	{
		TokenStream.WriteTab(2);
		TokenStream.WriteLine("SortPrimsFarZ,");
	}

	if(Material.Data().FullResolution)
	{
		TokenStream.WriteTab(2);
		TokenStream.WriteLine("FullResolution,");
	}

	if(Material.Data().PriorityPlane != 0)
	{
		TokenStream.WriteTab(2);
		TokenStream.WriteWord("PriorityPlane ");
		TokenStream.WriteInt(Material.Data().PriorityPlane);
		TokenStream.WriteLine(",");
	}

	for(i = 0; i < Material.Data().LayerContainer.GetTotalSize(); i++)
	{
		if(Material.Data().LayerContainer.ValidIndex(i))
		{
			Layer = Material.Data().LayerContainer[i];

			TokenStream.WriteTab(2);
			TokenStream.WriteLine("Layer {");

			switch(Layer->Data().FilterMode)
			{
				case FILTER_MODE_NONE:
				{
					TokenStream.WriteTab(3);
					TokenStream.WriteLine("FilterMode None,");
					break;
				}

				case FILTER_MODE_TRANSPARENT:
				{
					TokenStream.WriteTab(3);
					TokenStream.WriteLine("FilterMode Transparent,");
					break;
				}

				case FILTER_MODE_BLEND:
				{
					TokenStream.WriteTab(3);
					TokenStream.WriteLine("FilterMode Blend,");
					break;
				}

				case FILTER_MODE_ADDITIVE:
				{
					TokenStream.WriteTab(3);
					TokenStream.WriteLine("FilterMode Additive,");
					break;
				}

				case FILTER_MODE_ADD_ALPHA:
				{
					TokenStream.WriteTab(3);
					TokenStream.WriteLine("FilterMode AddAlpha,");
					break;
				}

				case FILTER_MODE_MODULATE:
				{
					TokenStream.WriteTab(3);
					TokenStream.WriteLine("FilterMode Modulate,");
					break;
				}
			}

			while(TRUE)
			{
				if(Layer->Data().Alpha.IsStatic())
				{
					if(Layer->Data().Alpha.GetScalar() == 1.0f) break;
				}

				if(!Layer->Data().Alpha.Save(TokenStream, 3)) return FALSE;

				break;
			}

			if(!Layer->Data().AnimatedTextureId.Save(TokenStream, 3)) return FALSE;

			if(Layer->Data().TwoSided)
			{
				TokenStream.WriteTab(3);
				TokenStream.WriteLine("TwoSided,");
			}

			if(Layer->Data().Unshaded)
			{
				TokenStream.WriteTab(3);
				TokenStream.WriteLine("Unshaded,");
			}

			if(Layer->Data().Unfogged)
			{
				TokenStream.WriteTab(3);
				TokenStream.WriteLine("Unfogged,");
			}

			if(Layer->Data().SphereEnvironmentMap)
			{
				TokenStream.WriteTab(3);
				TokenStream.WriteLine("SphereEnvMap,");
			}

			if(Layer->Data().NoDepthTest)
			{
				TokenStream.WriteTab(3);
				TokenStream.WriteLine("NoDepthTest,");
			}

			if(Layer->Data().NoDepthSet)
			{
				TokenStream.WriteTab(3);
				TokenStream.WriteLine("NoDepthSet,");
			}

			if(Layer->Data().TextureAnimationId != INVALID_INDEX)
			{
				TokenStream.WriteTab(3);
				TokenStream.WriteWord("TVertexAnimId ");
				TokenStream.WriteInt(Layer->Data().TextureAnimationId);
				TokenStream.WriteLine(",");
			}

			TokenStream.WriteTab(2);
			TokenStream.WriteLine("}");
		}
	}

	TokenStream.WriteTab();
	TokenStream.WriteLine("}");

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model texture animations
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveTextureAnimations(MODEL& Model, TOKEN_OUT_STREAM& TokenStream)
{
	INT i;

	if(Model.Data().TextureAnimationContainer.GetSize() <= 0) return TRUE;

	TokenStream.WriteWord("TextureAnims ");
	TokenStream.WriteInt(Model.Data().TextureAnimationContainer.GetSize());
	TokenStream.WriteLine(" {");

	for(i = 0; i < Model.Data().TextureAnimationContainer.GetTotalSize(); i++)
	{
		if(Model.Data().TextureAnimationContainer.ValidIndex(i))
		{
			if(!SaveTextureAnimation(*(Model.Data().TextureAnimationContainer[i]), TokenStream)) return FALSE;
		}
	}

	TokenStream.WriteLine("}");

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model texture animation
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveTextureAnimation(MODEL_TEXTURE_ANIMATION& TextureAnimation, TOKEN_OUT_STREAM& TokenStream)
{
	TokenStream.WriteTab();
	TokenStream.WriteLine("TVertexAnim {");

	while(TRUE)
	{
		if(TextureAnimation.Data().Translation.IsStatic())
		{
			if(TextureAnimation.Data().Translation.GetVector3() == D3DXVECTOR3(0.0f, 0.0f, 0.0f)) break;
		}

		if(!TextureAnimation.Data().Translation.Save(TokenStream, 2)) return FALSE;

		break;
	}

	while(TRUE)
	{
		if(TextureAnimation.Data().Rotation.IsStatic())
		{
			if(TextureAnimation.Data().Rotation.GetVector4() == D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f)) break;
		}

		if(!TextureAnimation.Data().Rotation.Save(TokenStream, 2)) return FALSE;

		break;
	}

	while(TRUE)
	{
		if(TextureAnimation.Data().Scaling.IsStatic())
		{
			if(TextureAnimation.Data().Scaling.GetVector3() == D3DXVECTOR3(1.0f, 1.0f, 1.0f)) break;
		}

		if(!TextureAnimation.Data().Scaling.Save(TokenStream, 2)) return FALSE;

		break;
	}

	TokenStream.WriteTab();
	TokenStream.WriteLine("}");

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model geosets
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveGeosets(MODEL& Model, TOKEN_OUT_STREAM& TokenStream)
{
	INT i;

	if(Model.Data().GeosetContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().GeosetContainer.GetTotalSize(); i++)
	{
		if(Model.Data().GeosetContainer.ValidIndex(i))
		{
			if(!SaveGeoset(*(Model.Data().GeosetContainer[i]), TokenStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model geoset
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveGeoset(MODEL_GEOSET& Geoset, TOKEN_OUT_STREAM& TokenStream)
{
	INT i;
	INT j;
	INT Total;
	BOOL First;
	MODEL_GEOSET_GROUP* Group;

	TokenStream.WriteLine("Geoset {");

	TokenStream.WriteTab();
	TokenStream.WriteWord("Vertices ");
	TokenStream.WriteInt(Geoset.Data().VertexContainer.GetSize());
	TokenStream.WriteLine(" {");

	for(i = 0; i < Geoset.Data().VertexContainer.GetTotalSize(); i++)
	{
		if(Geoset.Data().VertexContainer.ValidIndex(i))
		{
			TokenStream.WriteTab(2);
			TokenStream.WriteVector3(Geoset.Data().VertexContainer[i]->Position);
			TokenStream.WriteLine(",");
		}
	}

	TokenStream.WriteTab();
	TokenStream.WriteLine("}");

	TokenStream.WriteTab();
	TokenStream.WriteWord("Normals ");
	TokenStream.WriteInt(Geoset.Data().VertexContainer.GetSize());
	TokenStream.WriteLine(" {");

	for(i = 0; i < Geoset.Data().VertexContainer.GetTotalSize(); i++)
	{
		if(Geoset.Data().VertexContainer.ValidIndex(i))
		{
			TokenStream.WriteTab(2);
			TokenStream.WriteVector3(Geoset.Data().VertexContainer[i]->Normal);
			TokenStream.WriteLine(",");
		}
	}

	TokenStream.WriteTab();
	TokenStream.WriteLine("}");

	TokenStream.WriteTab();
	TokenStream.WriteWord("TVertices ");
	TokenStream.WriteInt(Geoset.Data().VertexContainer.GetSize());
	TokenStream.WriteLine(" {");

	for(i = 0; i < Geoset.Data().VertexContainer.GetTotalSize(); i++)
	{
		if(Geoset.Data().VertexContainer.ValidIndex(i))
		{
			TokenStream.WriteTab(2);
			TokenStream.WriteVector2(Geoset.Data().VertexContainer[i]->TexturePosition);
			TokenStream.WriteLine(",");
		}
	}

	TokenStream.WriteTab();
	TokenStream.WriteLine("}");

	TokenStream.WriteTab();
	TokenStream.WriteWord("VertexGroup ");
	TokenStream.WriteLine(" {");

	for(i = 0; i < Geoset.Data().VertexContainer.GetTotalSize(); i++)
	{
		if(Geoset.Data().VertexContainer.ValidIndex(i))
		{
			TokenStream.WriteTab(2);
			TokenStream.WriteInt(Geoset.Data().VertexContainer[i]->VertexGroup);
			TokenStream.WriteLine(",");
		}
	}

	TokenStream.WriteTab();
	TokenStream.WriteLine("}");

	TokenStream.WriteTab();
	TokenStream.WriteWord("Faces 1 ");
	TokenStream.WriteInt(Geoset.Data().FaceContainer.GetSize() * 3);
	TokenStream.WriteLine(" {");
	TokenStream.WriteTab(2);
	TokenStream.WriteLine("Triangles {");
	TokenStream.WriteTab(3);
	TokenStream.WriteWord("{ ");

	First = TRUE;
	for(i = 0; i < Geoset.Data().FaceContainer.GetTotalSize(); i++)
	{
		if(Geoset.Data().FaceContainer.ValidIndex(i))
		{
			if(!First) TokenStream.WriteWord(", ");
			TokenStream.WriteInt(Geoset.Data().FaceContainer[i]->Index1);
			TokenStream.WriteWord(", ");
			TokenStream.WriteInt(Geoset.Data().FaceContainer[i]->Index2);
			TokenStream.WriteWord(", ");
			TokenStream.WriteInt(Geoset.Data().FaceContainer[i]->Index3);

			First = FALSE;
		}
	}

	TokenStream.WriteLine(" },");
	TokenStream.WriteTab(2);
	TokenStream.WriteLine("}");
	TokenStream.WriteTab();
	TokenStream.WriteLine("}");

	Total = 0;
	for(i = 0; i < Geoset.Data().GroupContainer.GetTotalSize(); i++)
	{
		if(Geoset.Data().GroupContainer.ValidIndex(i))
		{
			Total += Geoset.Data().GroupContainer[i]->MatrixListSize;
		}
	}

	TokenStream.WriteTab();
	TokenStream.WriteWord("Groups ");
	TokenStream.WriteInt(Geoset.Data().GroupContainer.GetSize());
	TokenStream.WriteWord(" ");
	TokenStream.WriteInt(Total);
	TokenStream.WriteLine(" {");

	for(i = 0; i < Geoset.Data().GroupContainer.GetTotalSize(); i++)
	{
		if(Geoset.Data().GroupContainer.ValidIndex(i))
		{
			Group = Geoset.Data().GroupContainer[i];

			TokenStream.WriteTab(2);
			TokenStream.WriteWord("Matrices { ");

			First = TRUE;
			for(j = 0; j < Group->MatrixList.GetTotalSize(); j++)
			{
				if(Group->MatrixList.ValidIndex(j))
				{
					if(!First) TokenStream.WriteWord(", ");
					TokenStream.WriteInt(Group->MatrixList[j]->NodeId);

					First = FALSE;
				}
			}

			TokenStream.WriteLine(" },");
		}
	}

	TokenStream.WriteTab();
	TokenStream.WriteLine("}");

	if(Geoset.Data().Extent.Min != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("MinimumExtent ");
		TokenStream.WriteVector3(Geoset.Data().Extent.Min);
		TokenStream.WriteLine(",");
	}

	if(Geoset.Data().Extent.Max != D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("MaximumExtent ");
		TokenStream.WriteVector3(Geoset.Data().Extent.Max);
		TokenStream.WriteLine(",");
	}

	if(Geoset.Data().Extent.Radius > 0.0f)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("BoundsRadius ");
		TokenStream.WriteFloat(Geoset.Data().Extent.Radius);
		TokenStream.WriteLine(",");
	}

	for(i = 0; i < Geoset.Data().ExtentContainer.GetTotalSize(); i++)
	{
		if(Geoset.Data().ExtentContainer.ValidIndex(i))
		{
			TokenStream.WriteTab();
			TokenStream.WriteLine("Anim {");

			TokenStream.WriteTab(2);
			TokenStream.WriteWord("MinimumExtent ");
			TokenStream.WriteVector3(Geoset.Data().ExtentContainer[i]->Min);
			TokenStream.WriteLine(",");

			TokenStream.WriteTab(2);
			TokenStream.WriteWord("MaximumExtent ");
			TokenStream.WriteVector3(Geoset.Data().ExtentContainer[i]->Max);
			TokenStream.WriteLine(",");

			if(Geoset.Data().ExtentContainer[i]->Radius > 0.0f)
			{
				TokenStream.WriteTab(2);
				TokenStream.WriteWord("BoundsRadius ");
				TokenStream.WriteFloat(Geoset.Data().ExtentContainer[i]->Radius);
				TokenStream.WriteLine(",");
			}

			TokenStream.WriteTab();
			TokenStream.WriteLine("}");
		}
	}

	if(Geoset.Data().MaterialId != INVALID_INDEX)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("MaterialID ");
		TokenStream.WriteInt(Geoset.Data().MaterialId);
		TokenStream.WriteLine(",");
	}
	
	if(Geoset.Data().SelectionGroup != INVALID_INDEX)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("SelectionGroup ");
		TokenStream.WriteInt(Geoset.Data().SelectionGroup);
		TokenStream.WriteLine(",");
	}

	if(Geoset.Data().Unselectable)
	{
		TokenStream.WriteTab();
		TokenStream.WriteLine("Unselectable,");
	}

	TokenStream.WriteLine("}");

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model geoset animations
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveGeosetAnimations(MODEL& Model, TOKEN_OUT_STREAM& TokenStream)
{
	INT i;

	if(Model.Data().GeosetAnimationContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().GeosetAnimationContainer.GetTotalSize(); i++)
	{
		if(Model.Data().GeosetAnimationContainer.ValidIndex(i))
		{
			if(!SaveGeosetAnimation(*(Model.Data().GeosetAnimationContainer[i]), TokenStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model geoset animation
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveGeosetAnimation(MODEL_GEOSET_ANIMATION& GeosetAnimation, TOKEN_OUT_STREAM& TokenStream)
{
	TokenStream.WriteLine("GeosetAnim {");

	if(GeosetAnimation.Data().GeosetId != INVALID_INDEX)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("GeosetId ");
		TokenStream.WriteInt(GeosetAnimation.Data().GeosetId);
		TokenStream.WriteLine(",");
	}

	while(TRUE)
	{
		if(GeosetAnimation.Data().Alpha.IsStatic())
		{
			if(GeosetAnimation.Data().Alpha.GetScalar() == 1.0f) break;
		}

		if(!GeosetAnimation.Data().Alpha.Save(TokenStream)) return FALSE;

		break;
	}

	while(TRUE)
	{
		if(!GeosetAnimation.Data().UseColor) break;

		if(GeosetAnimation.Data().Color.IsStatic())
		{
			if(GeosetAnimation.Data().Color.GetVector3() == D3DXVECTOR3(1.0f, 1.0f, 1.0f)) break;
		}

		if(!GeosetAnimation.Data().Color.Save(TokenStream)) return FALSE;

		break;
	}

	if(GeosetAnimation.Data().DropShadow)
	{
		TokenStream.WriteTab();
		TokenStream.WriteLine("DropShadow,");
	}

	TokenStream.WriteLine("}");

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model bones
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveBones(MODEL& Model, TOKEN_OUT_STREAM& TokenStream)
{
	INT i;

	if(Model.Data().BoneContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().BoneContainer.GetTotalSize(); i++)
	{
		if(Model.Data().BoneContainer.ValidIndex(i))
		{
			if(!SaveBone(*(Model.Data().BoneContainer[i]), TokenStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model bone
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveBone(MODEL_BONE& Bone, TOKEN_OUT_STREAM& TokenStream)
{
	TokenStream.WriteWord("Bone ");
	TokenStream.WriteString(Bone.Data().Name);
	TokenStream.WriteLine(" {");

	if(!SaveBaseData(&Bone.Data(), TokenStream)) return FALSE;

	if(Bone.Data().GeosetId != INVALID_INDEX)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("GeosetId ");
		TokenStream.WriteInt(Bone.Data().GeosetId);
		TokenStream.WriteLine(",");
	}
	else
	{
		TokenStream.WriteTab();
		TokenStream.WriteLine("GeosetId Multiple,");
	}

	if(Bone.Data().GeosetAnimationId != INVALID_INDEX)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("GeosetAnimId ");
		TokenStream.WriteInt(Bone.Data().GeosetAnimationId);
		TokenStream.WriteLine(",");
	}
	else
	{
		TokenStream.WriteTab();
		TokenStream.WriteLine("GeosetAnimId None,");
	}

	TokenStream.WriteLine("}");

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model lights
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveLights(MODEL& Model, TOKEN_OUT_STREAM& TokenStream)
{
	INT i;

	if(Model.Data().LightContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().LightContainer.GetTotalSize(); i++)
	{
		if(Model.Data().LightContainer.ValidIndex(i))
		{
			if(!SaveLight(*(Model.Data().LightContainer[i]), TokenStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model light
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveLight(MODEL_LIGHT& Light, TOKEN_OUT_STREAM& TokenStream)
{
	TokenStream.WriteWord("Light ");
	TokenStream.WriteString(Light.Data().Name);
	TokenStream.WriteLine(" {");

	if(!SaveBaseData(&Light.Data(), TokenStream)) return FALSE;

	switch(Light.Data().Type)
	{
		case 0:
		{
			TokenStream.WriteTab();
			TokenStream.WriteLine("Omnidirectional,");
			break;
		}

		case 1:
		{
			TokenStream.WriteTab();
			TokenStream.WriteLine("Directional,");
			break;
		}

		case 2:
		{
			TokenStream.WriteTab();
			TokenStream.WriteLine("Ambient,");
			break;
		}
	}

	if(!Light.Data().AttenuationStart.Save(TokenStream)) return FALSE;
	if(!Light.Data().AttenuationEnd.Save(TokenStream)) return FALSE;
	if(!Light.Data().Color.Save(TokenStream)) return FALSE;
	if(!Light.Data().Intensity.Save(TokenStream)) return FALSE;
	if(!Light.Data().AmbientColor.Save(TokenStream)) return FALSE;
	if(!Light.Data().AmbientIntensity.Save(TokenStream)) return FALSE;

	while(TRUE)
	{
		if(Light.Data().Visibility.IsStatic())
		{
			if(Light.Data().Visibility.GetScalar() == 1.0f) break;
		}

		if(!Light.Data().Visibility.Save(TokenStream)) return FALSE;

		break;
	}

	TokenStream.WriteLine("}");

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model helpers
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveHelpers(MODEL& Model, TOKEN_OUT_STREAM& TokenStream)
{
	INT i;

	if(Model.Data().HelperContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().HelperContainer.GetTotalSize(); i++)
	{
		if(Model.Data().HelperContainer.ValidIndex(i))
		{
			if(!SaveHelper(*(Model.Data().HelperContainer[i]), TokenStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model helper
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveHelper(MODEL_HELPER& Helper, TOKEN_OUT_STREAM& TokenStream)
{
	TokenStream.WriteWord("Helper ");
	TokenStream.WriteString(Helper.Data().Name);
	TokenStream.WriteLine(" {");

	if(!SaveBaseData(&Helper.Data(), TokenStream)) return FALSE;

	TokenStream.WriteLine("}");

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model attachments
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveAttachments(MODEL& Model, TOKEN_OUT_STREAM& TokenStream)
{
	INT i;

	if(Model.Data().AttachmentContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().AttachmentContainer.GetTotalSize(); i++)
	{
		if(Model.Data().AttachmentContainer.ValidIndex(i))
		{
			if(!SaveAttachment(*(Model.Data().AttachmentContainer[i]), TokenStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model attachment
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveAttachment(MODEL_ATTACHMENT& Attachment, TOKEN_OUT_STREAM& TokenStream)
{
	TokenStream.WriteWord("Attachment ");
	TokenStream.WriteString(Attachment.Data().Name);
	TokenStream.WriteLine(" {");

	if(!SaveBaseData(&Attachment.Data(), TokenStream)) return FALSE;

	if(Attachment.Data().AttachmentId != INVALID_INDEX)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("AttachmentID ");
		TokenStream.WriteInt(Attachment.Data().AttachmentId);
		TokenStream.WriteLine(",");
	}

	if(Attachment.Data().Path != "")
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("Path ");
		TokenStream.WriteString(Attachment.Data().Path);
		TokenStream.WriteLine(",");
	}

	while(TRUE)
	{
		if(Attachment.Data().Visibility.IsStatic())
		{
			if(Attachment.Data().Visibility.GetScalar() == 1.0f) break;
		}

		if(!Attachment.Data().Visibility.Save(TokenStream)) return FALSE;

		break;
	}

	TokenStream.WriteLine("}");

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model pivot points
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SavePivotPoints(MODEL& Model, TOKEN_OUT_STREAM& TokenStream)
{
	INT i;

	if(Model.Data().PivotPointContainer.GetSize() <= 0) return TRUE;

	TokenStream.WriteWord("PivotPoints ");
	TokenStream.WriteInt(Model.Data().PivotPointContainer.GetSize());
	TokenStream.WriteLine(" {");

	for(i = 0; i < Model.Data().PivotPointContainer.GetTotalSize(); i++)
	{
		if(Model.Data().PivotPointContainer.ValidIndex(i))
		{
			TokenStream.WriteTab();
			TokenStream.WriteVector3(*(Model.Data().PivotPointContainer[i]));
			TokenStream.WriteLine(",");
		}
	}

	TokenStream.WriteLine("}");

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model particle emitters
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveParticleEmitters(MODEL& Model, TOKEN_OUT_STREAM& TokenStream)
{
	INT i;

	if(Model.Data().ParticleEmitterContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().ParticleEmitterContainer.GetTotalSize(); i++)
	{
		if(Model.Data().ParticleEmitterContainer.ValidIndex(i))
		{
			if(!SaveParticleEmitter(*(Model.Data().ParticleEmitterContainer[i]), TokenStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model particle emitter
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveParticleEmitter(MODEL_PARTICLE_EMITTER& ParticleEmitter, TOKEN_OUT_STREAM& TokenStream)
{
	TokenStream.WriteWord("ParticleEmitter ");
	TokenStream.WriteString(ParticleEmitter.Data().Name);
	TokenStream.WriteLine(" {");

	if(!SaveBaseData(&ParticleEmitter.Data(), TokenStream)) return FALSE;

	if(ParticleEmitter.Data().EmitterUsesMdl)
	{
		TokenStream.WriteTab();
		TokenStream.WriteLine("EmitterUsesMDL,");
	}

	if(ParticleEmitter.Data().EmitterUsesTga)
	{
		TokenStream.WriteTab();
		TokenStream.WriteLine("EmitterUsesTGA,");
	}

	if(!ParticleEmitter.Data().EmissionRate.Save(TokenStream)) return FALSE;
	if(!ParticleEmitter.Data().Gravity.Save(TokenStream)) return FALSE;
	if(!ParticleEmitter.Data().Longitude.Save(TokenStream)) return FALSE;
	if(!ParticleEmitter.Data().Latitude.Save(TokenStream)) return FALSE;

	while(TRUE)
	{
		if(ParticleEmitter.Data().Visibility.IsStatic())
		{
			if(ParticleEmitter.Data().Visibility.GetScalar() == 1.0f) break;
		}

		if(!ParticleEmitter.Data().Visibility.Save(TokenStream)) return FALSE;

		break;
	}

	TokenStream.WriteTab();
	TokenStream.WriteLine("Particle {");

	if(!ParticleEmitter.Data().ParticleLifeSpan.Save(TokenStream, 2)) return FALSE;
	if(!ParticleEmitter.Data().ParticleInitialVelocity.Save(TokenStream, 2)) return FALSE;

	TokenStream.WriteTab(2);
	TokenStream.WriteWord("Path ");
	TokenStream.WriteString(ParticleEmitter.Data().ParticleFileName);
	TokenStream.WriteLine(",");

	TokenStream.WriteTab();
	TokenStream.WriteLine("}");

	TokenStream.WriteLine("}");

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model particle emitters 2
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveParticleEmitters2(MODEL& Model, TOKEN_OUT_STREAM& TokenStream)
{
	INT i;

	if(Model.Data().ParticleEmitter2Container.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().ParticleEmitter2Container.GetTotalSize(); i++)
	{
		if(Model.Data().ParticleEmitter2Container.ValidIndex(i))
		{
			if(!SaveParticleEmitter2(*(Model.Data().ParticleEmitter2Container[i]), TokenStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model particle emitter 2
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveParticleEmitter2(MODEL_PARTICLE_EMITTER_2& ParticleEmitter2, TOKEN_OUT_STREAM& TokenStream)
{
	TokenStream.WriteWord("ParticleEmitter2 ");
	TokenStream.WriteString(ParticleEmitter2.Data().Name);
	TokenStream.WriteLine(" {");

	if(!SaveBaseData(&ParticleEmitter2.Data(), TokenStream)) return FALSE;

	switch(ParticleEmitter2.Data().FilterMode)
	{
		case FILTER_MODE_TRANSPARENT:
		{
			TokenStream.WriteTab();
			TokenStream.WriteLine("Transparent,");
			break;
		}

		case FILTER_MODE_BLEND:
		{
			TokenStream.WriteTab();
			TokenStream.WriteLine("Blend,");
			break;
		}

		case FILTER_MODE_ADDITIVE:
		{
			TokenStream.WriteTab();
			TokenStream.WriteLine("Additive,");
			break;
		}

		case FILTER_MODE_ADD_ALPHA:
		{
			TokenStream.WriteTab();
			TokenStream.WriteLine("AlphaKey,");
			break;
		}

		case FILTER_MODE_MODULATE:
		{
			TokenStream.WriteTab();
			TokenStream.WriteLine("Modulate,");
			break;
		}
	}

	if(!ParticleEmitter2.Data().Speed.Save(TokenStream)) return FALSE;
	if(!ParticleEmitter2.Data().Variation.Save(TokenStream)) return FALSE;
	if(!ParticleEmitter2.Data().Latitude.Save(TokenStream)) return FALSE;
	if(!ParticleEmitter2.Data().Gravity.Save(TokenStream)) return FALSE;
	if(!ParticleEmitter2.Data().EmissionRate.Save(TokenStream)) return FALSE;
	if(!ParticleEmitter2.Data().Width.Save(TokenStream)) return FALSE;
	if(!ParticleEmitter2.Data().Length.Save(TokenStream)) return FALSE;

	while(TRUE)
	{
		if(ParticleEmitter2.Data().Visibility.IsStatic())
		{
			if(ParticleEmitter2.Data().Visibility.GetScalar() == 1.0f) break;
		}

		if(!ParticleEmitter2.Data().Visibility.Save(TokenStream)) return FALSE;

		break;
	}

	TokenStream.WriteTab();
	TokenStream.WriteLine("SegmentColor {");
	TokenStream.WriteTab(2);
	TokenStream.WriteWord("Color ");
	TokenStream.WriteVector3(ParticleEmitter2.Data().SegmentColor1);
	TokenStream.WriteLine(",");
	TokenStream.WriteTab(2);
	TokenStream.WriteWord("Color ");
	TokenStream.WriteVector3(ParticleEmitter2.Data().SegmentColor2);
	TokenStream.WriteLine(",");
	TokenStream.WriteTab(2);
	TokenStream.WriteWord("Color ");
	TokenStream.WriteVector3(ParticleEmitter2.Data().SegmentColor3);
	TokenStream.WriteLine(",");
	TokenStream.WriteTab();
	TokenStream.WriteLine("},");

	TokenStream.WriteTab();
	TokenStream.WriteWord("Alpha ");
	TokenStream.WriteVector3(ParticleEmitter2.Data().Alpha);
	TokenStream.WriteLine(",");

	TokenStream.WriteTab();
	TokenStream.WriteWord("ParticleScaling ");
	TokenStream.WriteVector3(ParticleEmitter2.Data().ParticleScaling);
	TokenStream.WriteLine(",");

	TokenStream.WriteTab();
	TokenStream.WriteWord("LifeSpanUVAnim ");
	TokenStream.WriteVector3(ParticleEmitter2.Data().HeadLifeSpan);
	TokenStream.WriteLine(",");

	TokenStream.WriteTab();
	TokenStream.WriteWord("DecayUVAnim ");
	TokenStream.WriteVector3(ParticleEmitter2.Data().HeadDecay);
	TokenStream.WriteLine(",");

	TokenStream.WriteTab();
	TokenStream.WriteWord("TailUVAnim ");
	TokenStream.WriteVector3(ParticleEmitter2.Data().TailLifeSpan);
	TokenStream.WriteLine(",");

	TokenStream.WriteTab();
	TokenStream.WriteWord("TailDecayUVAnim ");
	TokenStream.WriteVector3(ParticleEmitter2.Data().TailDecay);
	TokenStream.WriteLine(",");

	if(ParticleEmitter2.Data().Rows > 0)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("Rows ");
		TokenStream.WriteInt(ParticleEmitter2.Data().Rows);
		TokenStream.WriteLine(",");
	}

	if(ParticleEmitter2.Data().Columns > 0)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("Columns ");
		TokenStream.WriteInt(ParticleEmitter2.Data().Columns);
		TokenStream.WriteLine(",");
	}

	if(ParticleEmitter2.Data().TextureId != INVALID_INDEX)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("TextureID ");
		TokenStream.WriteInt(ParticleEmitter2.Data().TextureId);
		TokenStream.WriteLine(",");
	}

	if(ParticleEmitter2.Data().Time != 0)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("Time ");
		TokenStream.WriteFloat(ParticleEmitter2.Data().Time);
		TokenStream.WriteLine(",");
	}

	if(ParticleEmitter2.Data().LifeSpan != 0)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("LifeSpan ");
		TokenStream.WriteFloat(ParticleEmitter2.Data().LifeSpan);
		TokenStream.WriteLine(",");
	}

	if(ParticleEmitter2.Data().TailLength != 0)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("TailLength ");
		TokenStream.WriteFloat(ParticleEmitter2.Data().TailLength);
		TokenStream.WriteLine(",");
	}

	if(ParticleEmitter2.Data().SortPrimitivesFarZ)
	{
		TokenStream.WriteTab();
		TokenStream.WriteLine("SortPrimsFarZ,");
	}

	if(ParticleEmitter2.Data().LineEmitter)
	{
		TokenStream.WriteTab();
		TokenStream.WriteLine("LineEmitter,");
	}

	if(ParticleEmitter2.Data().ModelSpace)
	{
		TokenStream.WriteTab();
		TokenStream.WriteLine("ModelSpace,");
	}

	if(ParticleEmitter2.Data().AlphaKey)
	{
		TokenStream.WriteTab();
		TokenStream.WriteLine("AlphaKey,");
	}

	if(ParticleEmitter2.Data().Unshaded)
	{
		TokenStream.WriteTab();
		TokenStream.WriteLine("Unshaded,");
	}

	if(ParticleEmitter2.Data().Unfogged)
	{
		TokenStream.WriteTab();
		TokenStream.WriteLine("Unfogged,");
	}

	if(ParticleEmitter2.Data().XYQuad)
	{
		TokenStream.WriteTab();
		TokenStream.WriteLine("XYQuad,");
	}

	if(ParticleEmitter2.Data().Squirt)
	{
		TokenStream.WriteTab();
		TokenStream.WriteLine("Squirt,");
	}

	if(ParticleEmitter2.Data().Head && ParticleEmitter2.Data().Tail)
	{
		TokenStream.WriteTab();
		TokenStream.WriteLine("Both,");
	}
	else if(ParticleEmitter2.Data().Head)
	{
		TokenStream.WriteTab();
		TokenStream.WriteLine("Head,");
	}
	else if(ParticleEmitter2.Data().Tail)
	{
		TokenStream.WriteTab();
		TokenStream.WriteLine("Tail,");
	}

	TokenStream.WriteLine("}");

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model ribbon emitters
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveRibbonEmitters(MODEL& Model, TOKEN_OUT_STREAM& TokenStream)
{
	INT i;

	if(Model.Data().RibbonEmitterContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().RibbonEmitterContainer.GetTotalSize(); i++)
	{
		if(Model.Data().RibbonEmitterContainer.ValidIndex(i))
		{
			if(!SaveRibbonEmitter(*(Model.Data().RibbonEmitterContainer[i]), TokenStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model ribbon emitter
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveRibbonEmitter(MODEL_RIBBON_EMITTER& RibbonEmitter, TOKEN_OUT_STREAM& TokenStream)
{
	TokenStream.WriteWord("RibbonEmitter ");
	TokenStream.WriteString(RibbonEmitter.Data().Name);
	TokenStream.WriteLine(" {");

	if(!SaveBaseData(&RibbonEmitter.Data(), TokenStream)) return FALSE;

	if(!RibbonEmitter.Data().HeightAbove.Save(TokenStream)) return FALSE;
	if(!RibbonEmitter.Data().HeightBelow.Save(TokenStream)) return FALSE;
	if(!RibbonEmitter.Data().Alpha.Save(TokenStream)) return FALSE;
	if(!RibbonEmitter.Data().Color.Save(TokenStream)) return FALSE;
	if(!RibbonEmitter.Data().TextureSlot.Save(TokenStream)) return FALSE;

	while(TRUE)
	{
		if(RibbonEmitter.Data().Visibility.IsStatic())
		{
			if(RibbonEmitter.Data().Visibility.GetScalar() == 1.0f) break;
		}

		if(!RibbonEmitter.Data().Visibility.Save(TokenStream)) return FALSE;

		break;
	}

	TokenStream.WriteTab();
	TokenStream.WriteWord("EmissionRate ");
	TokenStream.WriteFloat(RibbonEmitter.Data().EmissionRate);
	TokenStream.WriteLine(",");

	TokenStream.WriteTab();
	TokenStream.WriteWord("LifeSpan ");
	TokenStream.WriteFloat(RibbonEmitter.Data().LifeSpan);
	TokenStream.WriteLine(",");

	if(RibbonEmitter.Data().Gravity != 0.0f)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("Gravity ");
		TokenStream.WriteFloat(RibbonEmitter.Data().Gravity);
		TokenStream.WriteLine(",");
	}

	TokenStream.WriteTab();
	TokenStream.WriteWord("Rows ");
	TokenStream.WriteInt(RibbonEmitter.Data().Rows);
	TokenStream.WriteLine(",");

	TokenStream.WriteTab();
	TokenStream.WriteWord("Columns ");
	TokenStream.WriteInt(RibbonEmitter.Data().Columns);
	TokenStream.WriteLine(",");

	if(RibbonEmitter.Data().MaterialId != INVALID_INDEX)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("MaterialID ");
		TokenStream.WriteInt(RibbonEmitter.Data().MaterialId);
		TokenStream.WriteLine(",");
	}

	TokenStream.WriteLine("}");

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model event objects
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveEventObjects(MODEL& Model, TOKEN_OUT_STREAM& TokenStream)
{
	INT i;

	if(Model.Data().EventObjectContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().EventObjectContainer.GetTotalSize(); i++)
	{
		if(Model.Data().EventObjectContainer.ValidIndex(i))
		{
			if(!SaveEventObject(*(Model.Data().EventObjectContainer[i]), TokenStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model event object
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveEventObject(MODEL_EVENT_OBJECT& EventObject, TOKEN_OUT_STREAM& TokenStream)
{
	std::list<INT>::iterator i;

	TokenStream.WriteWord("EventObject ");
	TokenStream.WriteString(EventObject.Data().Name);
	TokenStream.WriteLine(" {");

	if(!SaveBaseData(&EventObject.Data(), TokenStream)) return FALSE;

	if(EventObject.Data().EventTrack.size() > 0)
	{
		TokenStream.WriteTab(1);
		TokenStream.WriteWord("EventTrack ");
		TokenStream.WriteInt(static_cast<INT>(EventObject.Data().EventTrack.size()));
		TokenStream.WriteLine(" {");

		i = EventObject.Data().EventTrack.begin();
		while(i != EventObject.Data().EventTrack.end())
		{
			TokenStream.WriteTab(2);
			TokenStream.WriteInt(*i);
			TokenStream.WriteLine(",");

			i++;
		}

		TokenStream.WriteTab(1);
		TokenStream.WriteLine("}");
	}

	TokenStream.WriteLine("}");

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model cameras
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveCameras(MODEL& Model, TOKEN_OUT_STREAM& TokenStream)
{
	INT i;

	if(Model.Data().CameraContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().CameraContainer.GetTotalSize(); i++)
	{
		if(Model.Data().CameraContainer.ValidIndex(i))
		{
			if(!SaveCamera(*(Model.Data().CameraContainer[i]), TokenStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model camera
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveCamera(MODEL_CAMERA& Camera, TOKEN_OUT_STREAM& TokenStream)
{
	TokenStream.WriteWord("Camera ");
	TokenStream.WriteString(Camera.Data().Name);
	TokenStream.WriteLine(" {");

	TokenStream.WriteTab();
	TokenStream.WriteWord("FieldOfView ");
	TokenStream.WriteFloat(Camera.Data().FieldOfView);
	TokenStream.WriteLine(",");

	TokenStream.WriteTab();
	TokenStream.WriteWord("FarClip ");
	TokenStream.WriteFloat(Camera.Data().FarDistance);
	TokenStream.WriteLine(",");

	TokenStream.WriteTab();
	TokenStream.WriteWord("NearClip ");
	TokenStream.WriteFloat(Camera.Data().NearDistance);
	TokenStream.WriteLine(",");

	TokenStream.WriteTab();
	TokenStream.WriteWord("Position ");
	TokenStream.WriteVector3(Camera.Data().Source);
	TokenStream.WriteLine(",");

	while(TRUE)
	{
		if(Camera.Data().SourceTranslation.IsStatic())
		{
			if(Camera.Data().SourceTranslation.GetVector3() == D3DXVECTOR3(0.0f, 0.0f, 0.0f)) break;
		}

		if(Camera.Data().SourceTranslation.Save(TokenStream)) return FALSE;

		break;
	}

	TokenStream.WriteTab();
	TokenStream.WriteLine("Target {");

	TokenStream.WriteTab(2);
	TokenStream.WriteWord("Position ");
	TokenStream.WriteVector3(Camera.Data().Target);
	TokenStream.WriteLine(",");

	while(TRUE)
	{
		if(Camera.Data().TargetTranslation.IsStatic())
		{
			if(Camera.Data().TargetTranslation.GetVector3() == D3DXVECTOR3(0.0f, 0.0f, 0.0f)) break;
		}

		if(Camera.Data().TargetTranslation.Save(TokenStream, 2)) return FALSE;

		break;
	}

	while(TRUE)
	{
		if(Camera.Data().Rotation.IsStatic())
		{
			if(Camera.Data().Rotation.GetScalar() == 0.0f) break;
		}

		if(Camera.Data().Rotation.Save(TokenStream, 2)) return FALSE;

		break;
	}

	TokenStream.WriteTab();
	TokenStream.WriteLine("}");

	TokenStream.WriteLine("}");

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model collision shapes
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveCollisionShapes(MODEL& Model, TOKEN_OUT_STREAM& TokenStream)
{
	INT i;

	if(Model.Data().CollisionShapeContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().CollisionShapeContainer.GetTotalSize(); i++)
	{
		if(Model.Data().CollisionShapeContainer.ValidIndex(i))
		{
			if(!SaveCollisionShape(*(Model.Data().CollisionShapeContainer[i]), TokenStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model collision shape
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveCollisionShape(MODEL_COLLISION_SHAPE& CollisionShape, TOKEN_OUT_STREAM& TokenStream)
{
	std::list<D3DXVECTOR3>::iterator i;

	TokenStream.WriteWord("CollisionShape ");
	TokenStream.WriteString(CollisionShape.Data().Name);
	TokenStream.WriteLine(" {");

	if(!SaveBaseData(&CollisionShape.Data(), TokenStream)) return FALSE;

	switch(CollisionShape.Data().Type)
	{
		case COLLISION_SHAPE_TYPE_BOX:
		{
			if(CollisionShape.Data().VertexList.size() != 2)
			{
				Error.SetMessage("Collision shape boxes needs exactly 2 vertices!");
				return FALSE;
			}

			TokenStream.WriteTab(1);
			TokenStream.WriteLine("Box,");
			break;
		}

		case COLLISION_SHAPE_TYPE_SPHERE:
		{
			if(CollisionShape.Data().VertexList.size() != 1)
			{
				Error.SetMessage("Collision shape spheres needs exactly 1 vertex!");
				return FALSE;
			}

			TokenStream.WriteTab(1);
			TokenStream.WriteLine("Sphere,");
			break;
		}
	}

	if(CollisionShape.Data().VertexList.size() > 0)
	{
		TokenStream.WriteTab(1);
		TokenStream.WriteWord("Vertices ");
		TokenStream.WriteInt(static_cast<INT>(CollisionShape.Data().VertexList.size()));
		TokenStream.WriteLine(" {");

		i = CollisionShape.Data().VertexList.begin();
		while(i != CollisionShape.Data().VertexList.end())
		{
			TokenStream.WriteTab(2);
			TokenStream.WriteVector3(*i);
			TokenStream.WriteLine(",");

			i++;
		}

		TokenStream.WriteTab(1);
		TokenStream.WriteLine("}");
	}

	if(CollisionShape.Data().BoundsRadius > 0.0f)
	{
		TokenStream.WriteTab(1);
		TokenStream.WriteWord("BoundsRadius ");
		TokenStream.WriteFloat(CollisionShape.Data().BoundsRadius);
		TokenStream.WriteLine(",");
	}

	TokenStream.WriteLine("}");

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model version
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadVersion(MODEL& Model, TOKEN_IN_STREAM& TokenStream)
{
	std::string Token;

	if(!TokenStream.ExpectChar('{')) return FALSE;

	while(!TokenStream.Eof())
	{
		Token = TokenStream.ReadWord();
		if(Token == "") break;

		if(Token == "}")
		{
			return TRUE;
		}
		else if(Token == "FormatVersion")
		{
			Model.Data().Info.Version = TokenStream.ReadInt();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"Version\"!");
			return FALSE;
		}
	}

	Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unexpected EOF reached!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Loads the model info
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadModel(MODEL& Model, TOKEN_IN_STREAM& TokenStream)
{
	std::string Token;

	Model.Data().Info.Name = TokenStream.ReadString();
	if(!TokenStream.ExpectChar('{')) return FALSE;

	while(!TokenStream.Eof())
	{
		Token = TokenStream.ReadWord();
		if(Token == "") break;

		if(Token == "}")
		{
			return TRUE;
		}
		else if(Token == "NumGeosets")
		{
			Model.Data().GeosetContainer.Reserve(TokenStream.ReadInt());
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "NumGeosetAnims")
		{
			Model.Data().GeosetAnimationContainer.Reserve(TokenStream.ReadInt());
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "NumHelpers")
		{
			Model.Data().HelperContainer.Reserve(TokenStream.ReadInt());
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "NumBones")
		{
			Model.Data().BoneContainer.Reserve(TokenStream.ReadInt());
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "NumLights")
		{
			Model.Data().LightContainer.Reserve(TokenStream.ReadInt());
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "NumAttachments")
		{
			Model.Data().AttachmentContainer.Reserve(TokenStream.ReadInt());
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "NumEvents")
		{
			Model.Data().EventObjectContainer.Reserve(TokenStream.ReadInt());
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "NumParticleEmitters")
		{
			Model.Data().ParticleEmitterContainer.Reserve(TokenStream.ReadInt());
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "NumParticleEmitters2")
		{
			Model.Data().ParticleEmitter2Container.Reserve(TokenStream.ReadInt());
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "NumRibbonEmitters")
		{
			Model.Data().RibbonEmitterContainer.Reserve(TokenStream.ReadInt());
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "BlendTime")
		{
			Model.Data().Info.BlendTime = TokenStream.ReadInt();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "MinimumExtent")
		{
			if(!TokenStream.ReadVector3(Model.Data().Info.Extent.Min)) return FALSE;
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "MaximumExtent")
		{
			if(!TokenStream.ReadVector3(Model.Data().Info.Extent.Max)) return FALSE;
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "BoundsRadius")
		{
			Model.Data().Info.Extent.Radius = TokenStream.ReadFloat();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "AnimationFile")
		{
			Model.Data().Info.AnimationFile = TokenStream.ReadString();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"Model\"!");
			return FALSE;
		}
	}

	Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unexpected EOF reached!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Loads the model sequences
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadSequences(MODEL& Model, TOKEN_IN_STREAM& TokenStream)
{
	INT NrOfSequences;
	std::string Token;
	MODEL_SEQUENCE* Sequence;

	NrOfSequences = TokenStream.ReadInt();
	if(!TokenStream.ExpectChar('{')) return FALSE;

	while(!TokenStream.Eof())
	{
		Token = TokenStream.ReadWord();
		if(Token == "") break;

		if(Token == "}")
		{
			return TRUE;
		}
		else if(Token == "Anim")
		{
			Sequence = new MODEL_SEQUENCE();
			if(Sequence == NULL)
			{
				Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
				return FALSE;
			}

			if(!LoadSequence((*Sequence), TokenStream))
			{
				delete Sequence;
				return FALSE;
			}

			if(!Model.AddSequence(Sequence))
			{
				delete Sequence;
				return FALSE;
			}
		}
		else
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"Sequence\"!");
			return FALSE;
		}
	}

	Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unexpected EOF reached!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Loads a model sequence
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadSequence(MODEL_SEQUENCE& Sequence, TOKEN_IN_STREAM& TokenStream)
{
	std::string Token;

	Sequence.Data().Name = TokenStream.ReadString();
	if(!TokenStream.ExpectChar('{')) return FALSE;

	while(!TokenStream.Eof())
	{
		Token = TokenStream.ReadWord();
		if(Token == "") break;

		if(Token == "}")
		{
			return TRUE;
		}
		else if(Token == "Interval")
		{
			if(!TokenStream.ReadVector2(Sequence.Data().Interval)) return FALSE;
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "Rarity")
		{
			Sequence.Data().Rarity = TokenStream.ReadFloat();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "MoveSpeed")
		{
			Sequence.Data().MoveSpeed = TokenStream.ReadFloat();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "MinimumExtent")
		{
			if(!TokenStream.ReadVector3(Sequence.Data().Extent.Min)) return FALSE;
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "MaximumExtent")
		{
			if(!TokenStream.ReadVector3(Sequence.Data().Extent.Max)) return FALSE;
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "BoundsRadius")
		{
			Sequence.Data().Extent.Radius = TokenStream.ReadFloat();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "NonLooping,")
		{
			Sequence.Data().NonLooping = TRUE;
		}
		else
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"Sequences.Anim\"!");
			return FALSE;
		}
	}

	return SkipGroup(TokenStream);
}


//+-----------------------------------------------------------------------------
//| Loads the global model sequences
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadGlobalSequences(MODEL& Model, TOKEN_IN_STREAM& TokenStream)
{
	INT i;
	INT TempDuration;
	INT NrOfGlobalSequences;
	MODEL_GLOBAL_SEQUENCE* GlobalSequence;

	NrOfGlobalSequences = TokenStream.ReadInt();
	Model.Data().GlobalSequenceContainer.Reserve(NrOfGlobalSequences);

	if(!TokenStream.ExpectChar('{')) return FALSE;

	for(i = 0; i < NrOfGlobalSequences; i++)
	{
		if(!TokenStream.ExpectWord("Duration")) return FALSE;
		TempDuration = TokenStream.ReadInt();
		if(!TokenStream.ExpectChar(',')) return FALSE;

		GlobalSequence = new MODEL_GLOBAL_SEQUENCE();
		if(GlobalSequence == NULL)
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
			return FALSE;
		}

		GlobalSequence->Data().Duration = TempDuration;

		if(!Model.AddGlobalSequence(GlobalSequence))
		{
			delete GlobalSequence;
			return FALSE;
		}
	}

	if(!TokenStream.ExpectChar('}')) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model textures
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadTextures(MODEL& Model, TOKEN_IN_STREAM& TokenStream)
{
	INT NrOfTextures;
	std::string Token;
	MODEL_TEXTURE* Texture;

	NrOfTextures = TokenStream.ReadInt();
	Model.Data().TextureContainer.Reserve(NrOfTextures);

	if(!TokenStream.ExpectChar('{')) return FALSE;

	while(!TokenStream.Eof())
	{
		Token = TokenStream.ReadWord();
		if(Token == "") break;

		if(Token == "}")
		{
			return TRUE;
		}
		else if(Token == "Bitmap")
		{
			Texture = new MODEL_TEXTURE();
			if(Texture == NULL)
			{
				Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
				return FALSE;
			}

			if(!LoadTexture((*Texture), TokenStream))
			{
				delete Texture;
				return FALSE;
			}

			if(!Model.AddTexture(Texture))
			{
				delete Texture;
				return FALSE;
			}

			if(Texture->Data().FileName != "")
			{
				if(!TextureManager.Load(Texture->Data().FileName)) return FALSE;
			}
		}
		else
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"Textures\"!");
			return FALSE;
		}
	}

	Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unexpected EOF reached!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Loads a model texture
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadTexture(MODEL_TEXTURE& Texture, TOKEN_IN_STREAM& TokenStream)
{
	std::string Token;

	if(!TokenStream.ExpectChar('{')) return FALSE;

	while(!TokenStream.Eof())
	{
		Token = TokenStream.ReadWord();
		if(Token == "") break;

		if(Token == "}")
		{
			return TRUE;
		}
		else if(Token == "Image")
		{
			Texture.Data().FileName = TokenStream.ReadString();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "ReplaceableId")
		{
			Texture.Data().ReplaceableId = TokenStream.ReadInt();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "WrapWidth,")
		{
			Texture.Data().WrapWidth = TRUE;
		}
		else if(Token == "WrapHeight,")
		{
			Texture.Data().WrapHeight = TRUE;
		}
		else
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"Textures.Bitmap\"!");
			return FALSE;
		}
	}

	Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unexpected EOF reached!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Loads the model materials
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadMaterials(MODEL& Model, TOKEN_IN_STREAM& TokenStream)
{
	INT NrOfMaterials;
	std::string Token;
	MODEL_MATERIAL* Object;

	NrOfMaterials = TokenStream.ReadInt();
	Model.Data().MaterialContainer.Reserve(NrOfMaterials);

	if(!TokenStream.ExpectChar('{')) return FALSE;

	while(!TokenStream.Eof())
	{
		Token = TokenStream.ReadWord();
		if(Token == "") break;

		if(Token == "}")
		{
			return TRUE;
		}
		else if(Token == "Material")
		{
			Object = new MODEL_MATERIAL();
			if(Object == NULL)
			{
				Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
				return FALSE;
			}

			if(!LoadMaterial((*Object), TokenStream))
			{
				delete Object;
				return FALSE;
			}

			if(!Model.AddMaterial(Object))
			{
				delete Object;
				return FALSE;
			}
		}
		else
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"Materials\"!");
			return FALSE;
		}
	}

	Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unexpected EOF reached!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Loads a model material
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadMaterial(MODEL_MATERIAL& Material, TOKEN_IN_STREAM& TokenStream)
{
	std::string Token;
	MODEL_MATERIAL_LAYER* Object;

	if(!TokenStream.ExpectChar('{')) return FALSE;

	while(!TokenStream.Eof())
	{
		Token = TokenStream.ReadWord();
		if(Token == "") break;

		if(Token == "}")
		{
			return TRUE;
		}
		else if(Token == "ConstantColor,")
		{
			Material.Data().ConstantColor = TRUE;
		}
		else if(Token == "SortPrimsFarZ,")
		{
			Material.Data().SortPrimitivesFarZ = TRUE;
		}
		else if(Token == "FullResolution,")
		{
			Material.Data().FullResolution = TRUE;
		}
		else if(Token == "Layer")
		{
			Object = new MODEL_MATERIAL_LAYER();
			if(Object == NULL)
			{
				Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
				return FALSE;
			}

			if(!LoadMaterialLayer((*Object), TokenStream))
			{
				delete Object;
				return FALSE;
			}

			if(!Material.AddLayer(Object))
			{
				delete Object;
				return FALSE;
			}
		}
		else if(Token == "PriorityPlane")
		{
			Material.Data().PriorityPlane = TokenStream.ReadInt();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"Materials.Material\"!");
			return FALSE;
		}
	}

	Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unexpected EOF reached!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Loads a model material layer
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadMaterialLayer(MODEL_MATERIAL_LAYER& Layer, TOKEN_IN_STREAM& TokenStream)
{
	std::string Token;

	if(!TokenStream.ExpectChar('{')) return FALSE;

	while(!TokenStream.Eof())
	{
		Token = TokenStream.ReadWord();
		if(Token == "") break;

		if(Token == "}")
		{
			return TRUE;
		}
		else if(Token == "static")
		{
			Token = TokenStream.ReadWord();
			if(Token == "") break;

			if(Token == "TextureID")
			{
				Layer.Data().TextureId = TokenStream.ReadInt();
				Layer.Data().AnimatedTextureId.SetStaticScalarInt(Layer.Data().TextureId);
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else if(Token == "Alpha")
			{
				Layer.Data().Alpha.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else
			{
				Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"static " + Token + "\" in \"Materials.Material.Layer\"!");
				return FALSE;
			}
		}
		else if(Token == "TextureID")
		{
			if(!Layer.Data().AnimatedTextureId.Load(TokenStream)) return FALSE;
			Layer.Data().TextureId = INVALID_INDEX;
		}
		else if(Token == "Alpha")
		{
			if(!Layer.Data().Alpha.Load(TokenStream)) return FALSE;
		}
		else if(Token == "FilterMode")
		{
			Token = TokenStream.ReadWord();
			if(Token == "") break;

			if(Token == "None,")
			{
				Layer.Data().FilterMode = FILTER_MODE_NONE;
			}
			else if(Token == "Transparent,")
			{
				Layer.Data().FilterMode = FILTER_MODE_TRANSPARENT;
			}
			else if(Token == "Blend,")
			{
				Layer.Data().FilterMode = FILTER_MODE_BLEND;
			}
			else if(Token == "Additive,")
			{
				Layer.Data().FilterMode = FILTER_MODE_ADDITIVE;
			}
			else if(Token == "AddAlpha,")
			{
				Layer.Data().FilterMode = FILTER_MODE_ADD_ALPHA;
			}
			else if(Token == "Modulate,")
			{
				Layer.Data().FilterMode = FILTER_MODE_MODULATE;
			}
			else
			{
				Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown filter mode \"" + Token + "\" in \"Materials.Material.Layer\"!");
				return FALSE;
			}
		}
		else if(Token == "TVertexAnimId")
		{
			Layer.Data().TextureAnimationId = TokenStream.ReadInt();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "TwoSided,")
		{
			Layer.Data().TwoSided = TRUE;
		}
		else if(Token == "Unshaded,")
		{
			Layer.Data().Unshaded = TRUE;
		}
		else if(Token == "Unfogged,")
		{
			Layer.Data().Unfogged = TRUE;
		}
		else if(Token == "SphereEnvMap,")
		{
			Layer.Data().SphereEnvironmentMap = TRUE;
		}
		else if(Token == "NoDepthTest,")
		{
			Layer.Data().NoDepthTest = TRUE;
		}
		else if(Token == "NoDepthSet,")
		{
			Layer.Data().NoDepthSet = TRUE;
		}
		else
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"Materials.Material.Layer\"!");
			return FALSE;
		}
	}

	Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unexpected EOF reached!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Loads the model texture animations
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadTextureAnimations(MODEL& Model, TOKEN_IN_STREAM& TokenStream)
{
	INT NrOfTextureAnimations;
	std::string Token;
	MODEL_TEXTURE_ANIMATION* TextureAnimation;

	NrOfTextureAnimations = TokenStream.ReadInt();
	Model.Data().TextureAnimationContainer.Reserve(NrOfTextureAnimations);

	if(!TokenStream.ExpectChar('{')) return FALSE;

	while(!TokenStream.Eof())
	{
		Token = TokenStream.ReadWord();
		if(Token == "") break;

		if(Token == "}")
		{
			return TRUE;
		}
		else if(Token == "TVertexAnim")
		{
			TextureAnimation = new MODEL_TEXTURE_ANIMATION();
			if(TextureAnimation == NULL)
			{
				Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
				return FALSE;
			}

			if(!LoadTextureAnimation((*TextureAnimation), TokenStream))
			{
				delete TextureAnimation;
				return FALSE;
			}

			if(!Model.AddTextureAnimation(TextureAnimation))
			{
				delete TextureAnimation;
				return FALSE;
			}
		}
		else
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"TextureAnims\"!");
			return FALSE;
		}
	}

	Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unexpected EOF reached!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Loads a model texture animation
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadTextureAnimation(MODEL_TEXTURE_ANIMATION& TextureAnimation, TOKEN_IN_STREAM& TokenStream)
{
	std::string Token;

	if(!TokenStream.ExpectChar('{')) return FALSE;

	while(!TokenStream.Eof())
	{
		Token = TokenStream.ReadWord();
		if(Token == "") break;

		if(Token == "}")
		{
			return TRUE;
		}
		else if(Token == "static")
		{
			Token = TokenStream.ReadWord();
			if(Token == "") break;

			if(Token == "Translation")
			{
				D3DXVECTOR3 Vector;

				if(!TokenStream.ReadVector3(Vector)) return TRUE;
				TextureAnimation.Data().Translation.SetStaticVector3(Vector);
				if(!TokenStream.ExpectChar(',')) return TRUE;
			}
			else if(Token == "Rotation")
			{
				D3DXVECTOR4 Vector;

				if(!TokenStream.ReadVector4(Vector)) return TRUE;
				TextureAnimation.Data().Rotation.SetStaticVector4(Vector);
				if(!TokenStream.ExpectChar(',')) return TRUE;
			}
			else if(Token == "Scaling")
			{
				D3DXVECTOR3 Vector;

				if(!TokenStream.ReadVector3(Vector)) return TRUE;
				TextureAnimation.Data().Scaling.SetStaticVector3(Vector);
				if(!TokenStream.ExpectChar(',')) return TRUE;
			}
			else
			{
				Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"static " + Token + "\" in \"TextureAnims.TVertexAnim\"!");
				return FALSE;
			}
		}
		else if(Token == "Translation")
		{
			if(!TextureAnimation.Data().Translation.Load(TokenStream)) return TRUE;
		}
		else if(Token == "Rotation")
		{
			if(!TextureAnimation.Data().Rotation.Load(TokenStream)) return TRUE;
		}
		else if(Token == "Scaling")
		{
			if(!TextureAnimation.Data().Scaling.Load(TokenStream)) return TRUE;
		}
		else
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"TextureAnims.TVertexAnim\"!");
			return FALSE;
		}
	}

	Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unexpected EOF reached!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Loads the model geosets
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadGeosets(MODEL& Model, TOKEN_IN_STREAM& TokenStream)
{
	MODEL_GEOSET* Geoset;

	Geoset = new MODEL_GEOSET();
	if(Geoset == NULL)
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
		return FALSE;
	}

	if(!LoadGeoset((*Geoset), TokenStream))
	{
		delete Geoset;
		return FALSE;
	}

	if(!Model.AddGeoset(Geoset))
	{
		delete Geoset;
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model geoset
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadGeoset(MODEL_GEOSET& Geoset, TOKEN_IN_STREAM& TokenStream)
{
	INT i;
	INT j;
	INT NrOfVertices = 0;
	INT NrOfNormals = 0;
	INT NrOfVertexGroups = 0;
	INT NrOfTexturePositions = 0;
	INT NrOfIndexes = 0;
	INT NrOfMatrices = 0;
	INT NrOfMatrixGroups = 0;
	std::vector<D3DXVECTOR3> PositionList;
	std::vector<D3DXVECTOR3> NormalList;
	std::vector<D3DXVECTOR2> TexturePositionList;
	std::vector<INT> GroupList;
	std::vector<INT> IndexList;
	MODEL_GEOSET_VERTEX* Vertex;
	MODEL_GEOSET_FACE* Face;
	MODEL_GEOSET_GROUP* Group;
	MODEL_GEOSET_GROUP_NODE* GroupNode;
	EXTENT* Extent;
	std::string Token;

	if(!TokenStream.ExpectChar('{')) return FALSE;

	while(!TokenStream.Eof())
	{
		Token = TokenStream.ReadWord();
		if(Token == "") break;

		if(Token == "}")
		{
			INT NrOfFaces;

			if(NrOfNormals != NrOfVertices)
			{
				Error.SetMessage("Normal count mismatch, doesn't match vertex count!");
				return FALSE;
			}

			if(NrOfVertexGroups != NrOfVertices)
			{
				Error.SetMessage("Vertex group count mismatch, doesn't match vertex count!");
				return FALSE;
			}

			if(NrOfTexturePositions != NrOfVertices)
			{
				Error.SetMessage("Texture position count mismatch, doesn't match vertex count!");
				return FALSE;
			}

			for(i = 0; i < NrOfVertices; i++)
			{
				Vertex = new MODEL_GEOSET_VERTEX();
				if(Vertex == NULL)
				{
					Error.SetMessage("Unable to create a new vertex!");
					return FALSE;
				}

				Vertex->Position = PositionList[i];
				Vertex->Normal = NormalList[i];
				Vertex->TexturePosition = TexturePositionList[i];
				Vertex->VertexGroup = GroupList[i];

				if(!Geoset.AddVertex(Vertex)) return FALSE;
			}

			NrOfFaces = NrOfIndexes / 3;
			if((NrOfIndexes % 3) != 0)
			{
				Error.SetMessage("Index miscount, nr of indexes is not a multiple of 3!");
				return FALSE;
			}

			for(i = 0; i < NrOfFaces; i++)
			{
				Face = new MODEL_GEOSET_FACE();
				if(Face == NULL)
				{
					Error.SetMessage("Unable to create a new face!");
					return FALSE;
				}

				Face->Index1 = IndexList[(i * 3) + 0];
				Face->Index2 = IndexList[(i * 3) + 1];
				Face->Index3 = IndexList[(i * 3) + 2];

				if(!Geoset.AddFace(Face)) return FALSE;
			}

			return TRUE;
		}
		else if(Token == "Vertices")
		{
			D3DXVECTOR3 Vector;

			NrOfVertices = TokenStream.ReadInt();
			PositionList.reserve(NrOfVertices);

			if(!TokenStream.ExpectChar('{')) return FALSE;

			for(i = 0; i < NrOfVertices; i++)
			{
				if(!TokenStream.ReadVector3(Vector)) return FALSE;
				if(!TokenStream.ExpectChar(',')) return FALSE;

				PositionList.push_back(Vector);
			}

			if(!TokenStream.ExpectChar('}')) return FALSE;
		}
		else if(Token == "Normals")
		{
			D3DXVECTOR3 Vector;

			NrOfNormals = TokenStream.ReadInt();
			NormalList.reserve(NrOfNormals);

			if(!TokenStream.ExpectChar('{')) return FALSE;

			for(i = 0; i < NrOfNormals; i++)
			{
				if(!TokenStream.ReadVector3(Vector)) return FALSE;
				if(!TokenStream.ExpectChar(',')) return FALSE;

				NormalList.push_back(Vector);
			}

			if(!TokenStream.ExpectChar('}')) return FALSE;
		}
		else if(Token == "TVertices")
		{
			D3DXVECTOR2 Vector;

			NrOfTexturePositions = TokenStream.ReadInt();
			TexturePositionList.reserve(NrOfTexturePositions);

			if(!TokenStream.ExpectChar('{')) return FALSE;

			for(i = 0; i < NrOfTexturePositions; i++)
			{
				if(!TokenStream.ReadVector2(Vector)) return FALSE;
				if(!TokenStream.ExpectChar(',')) return FALSE;

				TexturePositionList.push_back(Vector);
			}

			if(!TokenStream.ExpectChar('}')) return FALSE;
		}
		else if(Token == "VertexGroup")
		{
			INT Int;

			NrOfVertexGroups = NrOfVertices;
			GroupList.reserve(NrOfVertexGroups);

			if(!TokenStream.ExpectChar('{')) return FALSE;

			for(i = 0; i < NrOfVertexGroups; i++)
			{
				Int = TokenStream.ReadInt();
				if(!TokenStream.ExpectChar(',')) return FALSE;

				GroupList.push_back(Int);
			}

			if(!TokenStream.ExpectChar('}')) return FALSE;
		}
		else if(Token == "Faces")
		{
			CHAR TempChar;
			INT NrOfIndexGroups;

			NrOfIndexGroups = TokenStream.ReadInt();
			NrOfIndexes = TokenStream.ReadInt();
			IndexList.reserve(NrOfIndexes);

			if(!TokenStream.ExpectChar('{')) return FALSE;
			if(!TokenStream.ExpectWord("Triangles")) return FALSE;
			if(!TokenStream.ExpectChar('{')) return FALSE;
			if(!TokenStream.ExpectChar('{')) return FALSE;

			for(i = 0; i < NrOfIndexes; i++)
			{
				IndexList.push_back(TokenStream.ReadInt());

				TempChar = TokenStream.ReadChar();
				if(TempChar != ',')
				{
					if(TempChar != '}')
					{
						Error.SetMessage(std::string("Expected \"}\", got \"") + TempChar + "\" in \"" + CurrentFileName + "\"!");
						return FALSE;
					}

					if(!TokenStream.ExpectChar(',')) return FALSE;

					if(i < (NrOfIndexes - 1))
					{
						if(!TokenStream.ExpectChar('{')) return FALSE;
					}
				}
			}

			if(!TokenStream.ExpectChar('}')) return FALSE;
			if(!TokenStream.ExpectChar('}')) return FALSE;
		}
		else if(Token == "Groups")
		{
			CHAR TempChar;
			INT MatrixGroupSize;
			std::vector<INT> MatrixGroupList;

			NrOfMatrixGroups = TokenStream.ReadInt();
			NrOfMatrices = TokenStream.ReadInt();

			if(!TokenStream.ExpectChar('{')) return FALSE;

			for(i = 0; i < NrOfMatrixGroups; i++)
			{
				if(!TokenStream.ExpectWord("Matrices")) return FALSE;
				if(!TokenStream.ExpectChar('{')) return FALSE;

				MatrixGroupSize = 0;
				MatrixGroupList.clear();

				while(TRUE)
				{
					MatrixGroupList.push_back(TokenStream.ReadInt());
					MatrixGroupSize++;

					TempChar = TokenStream.ReadChar();
					if(TempChar != ',')
					{
						if(TempChar != '}')
						{
							Error.SetMessage(std::string("Expected \"}\", got \"") + TempChar + "\" in \"" + CurrentFileName + "\"!");
							return FALSE;
						}

						if(!TokenStream.ExpectChar(',')) return FALSE;

						break;
					}
				}

				Group = new MODEL_GEOSET_GROUP();
				if(Group == NULL)
				{
					Error.SetMessage("Unable to create a new matrix group!");
					return FALSE;
				}

				Group->MatrixListSize = MatrixGroupSize;
				for(j = 0; j < MatrixGroupSize; j++)
				{
					GroupNode = new MODEL_GEOSET_GROUP_NODE();
					if(GroupNode == NULL)
					{
						Error.SetMessage("Unable to create a new matrix group node!");
						delete Group;
						return FALSE;
					}

					GroupNode->NodeId = MatrixGroupList[j];

					if(!Group->MatrixList.Add(GroupNode)) return FALSE;
				}

				if(!Geoset.AddGroup(Group)) return FALSE;
			}

			if(!TokenStream.ExpectChar('}')) return FALSE;
		}
		else if(Token == "MinimumExtent")
		{
			if(!TokenStream.ReadVector3(Geoset.Data().Extent.Min)) return FALSE;
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "MaximumExtent")
		{
			if(!TokenStream.ReadVector3(Geoset.Data().Extent.Max)) return FALSE;
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "BoundsRadius")
		{
			Geoset.Data().Extent.Radius = TokenStream.ReadFloat();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "Anim")
		{
			EXTENT TempExtent;

			if(!TokenStream.ExpectChar('{')) return FALSE;

			while(TRUE)
			{
				Token = TokenStream.ReadWord();
				if(Token == "")
				{
					Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unexpected EOF reached!");
					return FALSE;
				}
				else if(Token == "}")
				{
					break;
				}
				else if(Token == "MinimumExtent")
				{
					if(!TokenStream.ReadVector3(TempExtent.Min)) return FALSE;
					if(!TokenStream.ExpectChar(',')) return FALSE;
				}
				else if(Token == "MaximumExtent")
				{
					if(!TokenStream.ReadVector3(TempExtent.Max)) return FALSE;
					if(!TokenStream.ExpectChar(',')) return FALSE;
				}
				else if(Token == "BoundsRadius")
				{
					TempExtent.Radius = TokenStream.ReadFloat();
					if(!TokenStream.ExpectChar(',')) return FALSE;
				}
				else
				{
					Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"Geoset.Anim\"!");
					return FALSE;
				}
			}

			Extent = new EXTENT(TempExtent);
			if(Extent == NULL)
			{
				Error.SetMessage("Unable to create a new extent!");
				return FALSE;
			}

			if(!Geoset.AddExtent(Extent)) return FALSE;
		}
		else if(Token == "MaterialID")
		{
			Geoset.Data().MaterialId = TokenStream.ReadInt();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "SelectionGroup")
		{
			Geoset.Data().SelectionGroup = TokenStream.ReadInt();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "Unselectable,")
		{
			Geoset.Data().Unselectable = TRUE;
		}
		else
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"Geoset\"!");
			return FALSE;
		}
	}

	Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unexpected EOF reached!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Loads the model geoset animations
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadGeosetAnimations(MODEL& Model, TOKEN_IN_STREAM& TokenStream)
{
	MODEL_GEOSET_ANIMATION* GeosetAnimation;

	GeosetAnimation = new MODEL_GEOSET_ANIMATION();
	if(GeosetAnimation == NULL)
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
		return FALSE;
	}

	if(!LoadGeosetAnimation((*GeosetAnimation), TokenStream))
	{
		delete GeosetAnimation;
		return FALSE;
	}

	if(!Model.AddGeosetAnimation(GeosetAnimation))
	{
		delete GeosetAnimation;
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model geoset animation
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadGeosetAnimation(MODEL_GEOSET_ANIMATION& GeosetAnimation, TOKEN_IN_STREAM& TokenStream)
{
	std::string Token;

	if(!TokenStream.ExpectChar('{')) return FALSE;

	while(!TokenStream.Eof())
	{
		Token = TokenStream.ReadWord();
		if(Token == "") break;

		if(Token == "}")
		{
			return TRUE;
		}
		else if(Token == "static")
		{
			Token = TokenStream.ReadWord();
			if(Token == "") break;

			if(Token == "Alpha")
			{
				GeosetAnimation.Data().Alpha.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return TRUE;
			}
			else if(Token == "Color")
			{
				D3DXVECTOR3 Vector;

				if(!TokenStream.ReadVector3(Vector)) return TRUE;
				GeosetAnimation.Data().Color.SetStaticVector3(Vector);
				GeosetAnimation.Data().UseColor = TRUE;
				if(!TokenStream.ExpectChar(',')) return TRUE;
			}
			else
			{
				Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"static " + Token + "\" in \"GeosetAnim\"!");
				return FALSE;
			}
		}
		else if(Token == "Alpha")
		{
			if(!GeosetAnimation.Data().Alpha.Load(TokenStream)) return TRUE;
		}
		else if(Token == "Color")
		{
			if(!GeosetAnimation.Data().Color.Load(TokenStream)) return TRUE;
			GeosetAnimation.Data().UseColor = TRUE;
		}
		else if(Token == "GeosetId")
		{
			GeosetAnimation.Data().GeosetId = TokenStream.ReadInt();
			if(!TokenStream.ExpectChar(',')) return TRUE;
		}
		else if(Token == "DropShadow,")
		{
			GeosetAnimation.Data().DropShadow = TRUE;
		}
		else
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"GeosetAnim\"!");
			return FALSE;
		}
	}

	Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unexpected EOF reached!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Loads the model bones
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadBones(MODEL& Model, TOKEN_IN_STREAM& TokenStream)
{
	MODEL_BONE* Bone;

	Bone = new MODEL_BONE();
	if(Bone == NULL)
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
		return FALSE;
	}

	if(!LoadBone((*Bone), TokenStream))
	{
		delete Bone;
		return FALSE;
	}

	if(!Model.AddBone(Bone))
	{
		delete Bone;
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model bone
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadBone(MODEL_BONE& Bone, TOKEN_IN_STREAM& TokenStream)
{
	std::string Token;
	BOOL ErrorOccured;

	Bone.Data().Name = TokenStream.ReadString();
	if(!TokenStream.ExpectChar('{')) return FALSE;

	while(!TokenStream.Eof())
	{
		Token = TokenStream.ReadWord();
		if(Token == "") break;

		if(Token == "}")
		{
			return TRUE;
		}
		else if(Token == "static")
		{
			Token = TokenStream.ReadWord();
			if(Token == "") break;

			if(LoadStaticBaseData(ErrorOccured, Bone.BaseData(), TokenStream, Token))
			{
				if(ErrorOccured) return FALSE;
			}
			else
			{
				Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"static " + Token + "\" in \"Bone\"!");
				return FALSE;
			}
		}
		else if(LoadBaseData(ErrorOccured, Bone.BaseData(), TokenStream, Token))
		{
			if(ErrorOccured) return FALSE;
		}
		else if(Token == "GeosetId")
		{
			std::stringstream Stream;

			Token = TokenStream.ReadWord();
			if(Token == "None,")
			{
				Bone.Data().GeosetId = INVALID_INDEX;
			}
			else if(Token == "Multiple,")
			{
				Bone.Data().GeosetId = INVALID_INDEX;
			}
			else
			{
				Stream.str(Token);

				Bone.Data().GeosetId = INVALID_INDEX;
				Stream >> Bone.Data().GeosetId;
			}
		}
		else if(Token == "GeosetAnimId")
		{
			std::stringstream Stream;

			Token = TokenStream.ReadWord();
			if(Token == "None,")
			{
				Bone.Data().GeosetAnimationId = INVALID_INDEX;
			}
			else if(Token == "Multiple,")
			{
				Bone.Data().GeosetAnimationId = INVALID_INDEX;
			}
			else
			{
				Stream.str(Token);

				Bone.Data().GeosetAnimationId = INVALID_INDEX;
				Stream >> Bone.Data().GeosetAnimationId;
			}
		}
		else
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"Bone\"!");
			return FALSE;
		}
	}

	Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unexpected EOF reached!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Loads the model lights
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadLights(MODEL& Model, TOKEN_IN_STREAM& TokenStream)
{
	MODEL_LIGHT* Light;

	Light = new MODEL_LIGHT();
	if(Light == NULL)
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
		return FALSE;
	}

	if(!LoadLight((*Light), TokenStream))
	{
		delete Light;
		return FALSE;
	}

	if(!Model.AddLight(Light))
	{
		delete Light;
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model light
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadLight(MODEL_LIGHT& Light, TOKEN_IN_STREAM& TokenStream)
{
	std::string Token;
	BOOL ErrorOccured;

	Light.Data().Name = TokenStream.ReadString();
	if(!TokenStream.ExpectChar('{')) return FALSE;

	while(!TokenStream.Eof())
	{
		Token = TokenStream.ReadWord();
		if(Token == "") break;

		if(Token == "}")
		{
			return TRUE;
		}
		else if(Token == "static")
		{
			Token = TokenStream.ReadWord();
			if(Token == "") break;

			if(LoadStaticBaseData(ErrorOccured, &Light.Data(), TokenStream, Token))
			{
				if(ErrorOccured) return FALSE;
			}
			else if(Token == "AttenuationStart")
			{
				Light.Data().AttenuationStart.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else if(Token == "AttenuationEnd")
			{
				Light.Data().AttenuationEnd.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else if(Token == "Color")
			{
				D3DXVECTOR3 Vector;

				if(!TokenStream.ReadVector3(Vector)) return FALSE;
				Light.Data().Color.SetStaticVector3(Vector);
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else if(Token == "Intensity")
			{
				Light.Data().Intensity.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else if(Token == "AmbColor")
			{
				D3DXVECTOR3 Vector;

				if(!TokenStream.ReadVector3(Vector)) return FALSE;
				Light.Data().AmbientColor.SetStaticVector3(Vector);
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else if(Token == "AmbIntensity")
			{
				Light.Data().AmbientIntensity.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else if(Token == "Visibility")
			{
				Light.Data().Visibility.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else
			{
				Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"static " + Token + "\" in \"Light\"!");
				return FALSE;
			}
		}
		else if(LoadBaseData(ErrorOccured, Light.BaseData(), TokenStream, Token))
		{
			if(ErrorOccured) return FALSE;
		}
		else if(Token == "Omnidirectional,")
		{
			Light.Data().Type = 0;
		}
		else if(Token == "Directional,")
		{
			Light.Data().Type = 1;
		}
		else if(Token == "Ambient,")
		{
			Light.Data().Type = 2;
		}
		else if(Token == "AttenuationStart")
		{
			if(!Light.Data().AttenuationStart.Load(TokenStream)) return FALSE;
		}
		else if(Token == "AttenuationEnd")
		{
			if(!Light.Data().AttenuationEnd.Load(TokenStream)) return FALSE;
		}
		else if(Token == "Color")
		{
			if(!Light.Data().Color.Load(TokenStream)) return FALSE;
		}
		else if(Token == "Intensity")
		{
			if(!Light.Data().Intensity.Load(TokenStream)) return FALSE;
		}
		else if(Token == "AmbColor")
		{
			if(!Light.Data().AmbientColor.Load(TokenStream)) return FALSE;
		}
		else if(Token == "AmbIntensity")
		{
			if(!Light.Data().AmbientIntensity.Load(TokenStream)) return FALSE;
		}
		else if(Token == "Visibility")
		{
			if(!Light.Data().Visibility.Load(TokenStream)) return FALSE;
		}
		else
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"Light\"!");
			return FALSE;
		}
	}

	Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unexpected EOF reached!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Loads the model helpers
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadHelpers(MODEL& Model, TOKEN_IN_STREAM& TokenStream)
{
	MODEL_HELPER* Helper;

	Helper = new MODEL_HELPER();
	if(Helper == NULL)
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
		return FALSE;
	}

	if(!LoadHelper((*Helper), TokenStream))
	{
		delete Helper;
		return FALSE;
	}

	if(!Model.AddHelper(Helper))
	{
		delete Helper;
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model helper
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadHelper(MODEL_HELPER& Helper, TOKEN_IN_STREAM& TokenStream)
{
	std::string Token;
	BOOL ErrorOccured;

	Helper.Data().Name = TokenStream.ReadString();
	if(!TokenStream.ExpectChar('{')) return FALSE;

	while(!TokenStream.Eof())
	{
		Token = TokenStream.ReadWord();
		if(Token == "") break;

		if(Token == "}")
		{
			return TRUE;
		}
		else if(Token == "static")
		{
			Token = TokenStream.ReadWord();
			if(Token == "") break;

			if(LoadStaticBaseData(ErrorOccured, &Helper.Data(), TokenStream, Token))
			{
				if(ErrorOccured) return FALSE;
			}
			else
			{
				Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"static " + Token + "\" in \"Helper\"!");
				return FALSE;
			}
		}
		else if(LoadBaseData(ErrorOccured, Helper.BaseData(), TokenStream, Token))
		{
			if(ErrorOccured) return FALSE;
		}
		else
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"Helper\"!");
			return FALSE;
		}
	}

	Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unexpected EOF reached!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Loads the model attachments
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadAttachments(MODEL& Model, TOKEN_IN_STREAM& TokenStream)
{
	MODEL_ATTACHMENT* Attachment;

	Attachment = new MODEL_ATTACHMENT();
	if(Attachment == NULL)
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
		return FALSE;
	}

	if(!LoadAttachment((*Attachment), TokenStream))
	{
		delete Attachment;
		return FALSE;
	}

	if(!Model.AddAttachment(Attachment))
	{
		delete Attachment;
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model attachment
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadAttachment(MODEL_ATTACHMENT& Attachment, TOKEN_IN_STREAM& TokenStream)
{
	std::string Token;
	BOOL ErrorOccured;

	Attachment.Data().Name = TokenStream.ReadString();
	if(!TokenStream.ExpectChar('{')) return FALSE;

	while(!TokenStream.Eof())
	{
		Token = TokenStream.ReadWord();
		if(Token == "") break;

		if(Token == "}")
		{
			return TRUE;
		}
		else if(Token == "static")
		{
			Token = TokenStream.ReadWord();
			if(Token == "") break;

			if(LoadStaticBaseData(ErrorOccured, &Attachment.Data(), TokenStream, Token))
			{
				if(ErrorOccured) return FALSE;
			}
			else if(Token == "Visibility")
			{
				Attachment.Data().Visibility.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else
			{
				Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"static " + Token + "\" in \"Attachment\"!");
				return FALSE;
			}
		}
		else if(LoadBaseData(ErrorOccured, Attachment.BaseData(), TokenStream, Token))
		{
			if(ErrorOccured) return FALSE;
		}
		else if(Token == "AttachmentID")
		{
			Attachment.Data().AttachmentId = TokenStream.ReadInt();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "Path")
		{
			Attachment.Data().Path = TokenStream.ReadString();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "Visibility")
		{
			if(!Attachment.Data().Visibility.Load(TokenStream)) return FALSE;
		}
		else
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"Attachment\"!");
			return FALSE;
		}
	}

	Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unexpected EOF reached!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Loads the model pivot points
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadPivotPoints(MODEL& Model, TOKEN_IN_STREAM& TokenStream)
{
	INT i;
	INT NrOfPivotPoints;
	D3DXVECTOR3 TempVector;

	NrOfPivotPoints = TokenStream.ReadInt();

	Model.Data().PivotPointContainer.Clear();
	Model.Data().PivotPointContainer.Reserve(NrOfPivotPoints);

	if(!TokenStream.ExpectChar('{')) return FALSE;

	for(i = 0; i < NrOfPivotPoints; i++)
	{
		if(!TokenStream.ReadVector3(TempVector)) return FALSE;
		if(!TokenStream.ExpectChar(',')) return FALSE;

		Model.AddPivotPoint(new D3DXVECTOR3(TempVector));
	}

	if(!TokenStream.ExpectChar('}')) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model particle emitters
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadParticleEmitters(MODEL& Model, TOKEN_IN_STREAM& TokenStream)
{
	MODEL_PARTICLE_EMITTER* ParticleEmitter;

	ParticleEmitter = new MODEL_PARTICLE_EMITTER();
	if(ParticleEmitter == NULL)
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
		return FALSE;
	}

	if(!LoadParticleEmitter((*ParticleEmitter), TokenStream))
	{
		delete ParticleEmitter;
		return FALSE;
	}

	if(!Model.AddParticleEmitter(ParticleEmitter))
	{
		delete ParticleEmitter;
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model particle emitter
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadParticleEmitter(MODEL_PARTICLE_EMITTER& ParticleEmitter, TOKEN_IN_STREAM& TokenStream)
{
	std::string Token;
	BOOL ErrorOccured;

	ParticleEmitter.Data().Name = TokenStream.ReadString();
	if(!TokenStream.ExpectChar('{')) return FALSE;

	while(!TokenStream.Eof())
	{
		Token = TokenStream.ReadWord();
		if(Token == "") break;

		if(Token == "}")
		{
			return TRUE;
		}
		else if(Token == "static")
		{
			Token = TokenStream.ReadWord();
			if(Token == "") break;

			if(LoadStaticBaseData(ErrorOccured, &ParticleEmitter.Data(), TokenStream, Token))
			{
				if(ErrorOccured) return FALSE;
			}
			else if(Token == "EmissionRate")
			{
				ParticleEmitter.Data().EmissionRate.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else if(Token == "Gravity")
			{
				ParticleEmitter.Data().Gravity.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else if(Token == "Longitude")
			{
				ParticleEmitter.Data().Longitude.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else if(Token == "Latitude")
			{
				ParticleEmitter.Data().Latitude.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else if(Token == "Visibility")
			{
				ParticleEmitter.Data().Visibility.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else
			{
				Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"static " + Token + "\" in \"ParticleEmitter\"!");
				return FALSE;
			}
		}
		else if(LoadBaseData(ErrorOccured, ParticleEmitter.BaseData(), TokenStream, Token))
		{
			if(ErrorOccured) return FALSE;
		}
		else if(Token == "EmissionRate")
		{
			if(!ParticleEmitter.Data().EmissionRate.Load(TokenStream)) return FALSE;
		}
		else if(Token == "Gravity")
		{
			if(!ParticleEmitter.Data().Gravity.Load(TokenStream)) return FALSE;
		}
		else if(Token == "Longitude")
		{
			if(!ParticleEmitter.Data().Longitude.Load(TokenStream)) return FALSE;
		}
		else if(Token == "Latitude")
		{
			if(!ParticleEmitter.Data().Latitude.Load(TokenStream)) return FALSE;
		}
		else if(Token == "Visibility")
		{
			if(!ParticleEmitter.Data().Visibility.Load(TokenStream)) return FALSE;
		}
		else if(Token == "EmitterUsesMDL,")
		{
			ParticleEmitter.Data().EmitterUsesMdl = TRUE;
		}
		else if(Token == "EmitterUsesTGA,")
		{
			ParticleEmitter.Data().EmitterUsesTga = TRUE;
		}
		else if(Token == "Particle")
		{
			if(!TokenStream.ExpectChar('{')) return FALSE;

			while(TRUE)
			{
				Token = TokenStream.ReadWord();
				if(Token == "")
				{
					Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unexpected EOF reached!");
					return FALSE;
				}
				else if(Token == "}")
				{
					break;
				}
				else if(Token == "static")
				{
					Token = TokenStream.ReadWord();
					if(Token == "")
					{
						Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unexpected EOF reached!");
						return FALSE;
					}

					if(Token == "LifeSpan")
					{
						ParticleEmitter.Data().ParticleLifeSpan.SetStaticScalar(TokenStream.ReadFloat());
						if(!TokenStream.ExpectChar(',')) return FALSE;
					}
					else if(Token == "InitVelocity")
					{
						ParticleEmitter.Data().ParticleInitialVelocity.SetStaticScalar(TokenStream.ReadFloat());
						if(!TokenStream.ExpectChar(',')) return FALSE;
					}
					else
					{
						Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"static " + Token + "\" in \"ParticleEmitter.Particle\"!");
						return FALSE;
					}
				}
				else if(Token == "LifeSpan")
				{
					if(!ParticleEmitter.Data().ParticleLifeSpan.Load(TokenStream)) return FALSE;
				}
				else if(Token == "InitVelocity")
				{
					if(!ParticleEmitter.Data().ParticleInitialVelocity.Load(TokenStream)) return FALSE;
				}
				else if(Token == "Path")
				{
					ParticleEmitter.Data().ParticleFileName = TokenStream.ReadString();
					if(!TokenStream.ExpectChar(',')) return FALSE;
				}
				else
				{
					Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"ParticleEmitter.Particle\"!");
					return FALSE;
				}
			}
		}
		else
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"ParticleEmitter\"!");
			return FALSE;
		}
	}

	Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unexpected EOF reached!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Loads the model particle emitters 2
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadParticleEmitters2(MODEL& Model, TOKEN_IN_STREAM& TokenStream)
{
	MODEL_PARTICLE_EMITTER_2* ParticleEmitter2;

	ParticleEmitter2 = new MODEL_PARTICLE_EMITTER_2();
	if(ParticleEmitter2 == NULL)
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
		return FALSE;
	}

	if(!LoadParticleEmitter2((*ParticleEmitter2), TokenStream))
	{
		delete ParticleEmitter2;
		return FALSE;
	}

	if(!Model.AddParticleEmitter2(ParticleEmitter2))
	{
		delete ParticleEmitter2;
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model particle emitter 2
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadParticleEmitter2(MODEL_PARTICLE_EMITTER_2& ParticleEmitter2, TOKEN_IN_STREAM& TokenStream)
{
	std::string Token;
	BOOL ErrorOccured;

	ParticleEmitter2.Data().Name = TokenStream.ReadString();
	if(!TokenStream.ExpectChar('{')) return FALSE;

	while(!TokenStream.Eof())
	{
		Token = TokenStream.ReadWord();
		if(Token == "") break;

		if(Token == "}")
		{
			return TRUE;
		}
		else if(Token == "static")
		{
			Token = TokenStream.ReadWord();
			if(Token == "") break;

			if(LoadStaticBaseData(ErrorOccured, &ParticleEmitter2.Data(), TokenStream, Token))
			{
				if(ErrorOccured) return FALSE;
			}
			else if(Token == "Speed")
			{
				ParticleEmitter2.Data().Speed.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else if(Token == "Variation")
			{
				ParticleEmitter2.Data().Variation.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else if(Token == "Latitude")
			{
				ParticleEmitter2.Data().Latitude.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else if(Token == "Gravity")
			{
				ParticleEmitter2.Data().Gravity.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else if(Token == "Visibility")
			{
				ParticleEmitter2.Data().Visibility.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else if(Token == "EmissionRate")
			{
				ParticleEmitter2.Data().EmissionRate.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else if(Token == "Width")
			{
				ParticleEmitter2.Data().Width.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else if(Token == "Length")
			{
				ParticleEmitter2.Data().Length.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else
			{
				Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"static " + Token + "\" in \"ParticleEmitter2\"!");
				return FALSE;
			}
		}
		else if(LoadBaseData(ErrorOccured, ParticleEmitter2.BaseData(), TokenStream, Token))
		{
			if(ErrorOccured) return FALSE;
		}
		else if(Token == "Transparent,")
		{
			ParticleEmitter2.Data().FilterMode = FILTER_MODE_TRANSPARENT;
		}
		else if(Token == "Blend,")
		{
			ParticleEmitter2.Data().FilterMode = FILTER_MODE_BLEND;
		}
		else if(Token == "Additive,")
		{
			ParticleEmitter2.Data().FilterMode = FILTER_MODE_ADDITIVE;
		}
		else if(Token == "AlphaKey,")
		{
			ParticleEmitter2.Data().FilterMode = FILTER_MODE_ADD_ALPHA;
		}
		else if(Token == "Modulate,")
		{
			ParticleEmitter2.Data().FilterMode = FILTER_MODE_MODULATE;
		}
		else if(Token == "Speed")
		{
			if(!ParticleEmitter2.Data().Speed.Load(TokenStream)) return FALSE;
		}
		else if(Token == "Variation")
		{
			if(!ParticleEmitter2.Data().Variation.Load(TokenStream)) return FALSE;
		}
		else if(Token == "Latitude")
		{
			if(!ParticleEmitter2.Data().Latitude.Load(TokenStream)) return FALSE;
		}
		else if(Token == "Gravity")
		{
			if(!ParticleEmitter2.Data().Gravity.Load(TokenStream)) return FALSE;
		}
		else if(Token == "Visibility")
		{
			if(!ParticleEmitter2.Data().Visibility.Load(TokenStream)) return FALSE;
		}
		else if(Token == "EmissionRate")
		{
			if(!ParticleEmitter2.Data().EmissionRate.Load(TokenStream)) return FALSE;
		}
		else if(Token == "Width")
		{
			if(!ParticleEmitter2.Data().Width.Load(TokenStream)) return FALSE;
		}
		else if(Token == "Length")
		{
			if(!ParticleEmitter2.Data().Length.Load(TokenStream)) return FALSE;
		}
		else if(Token == "SegmentColor")
		{
			if(!TokenStream.ExpectChar('{')) return FALSE;

			if(!TokenStream.ExpectWord("Color")) return FALSE;
			if(!TokenStream.ReadVector3(ParticleEmitter2.Data().SegmentColor1)) return FALSE;
			if(!TokenStream.ExpectChar(',')) return FALSE;

			if(!TokenStream.ExpectWord("Color")) return FALSE;
			if(!TokenStream.ReadVector3(ParticleEmitter2.Data().SegmentColor2)) return FALSE;
			if(!TokenStream.ExpectChar(',')) return FALSE;

			if(!TokenStream.ExpectWord("Color")) return FALSE;
			if(!TokenStream.ReadVector3(ParticleEmitter2.Data().SegmentColor3)) return FALSE;
			if(!TokenStream.ExpectChar(',')) return FALSE;

			if(!TokenStream.ExpectChar('}')) return FALSE;
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "Alpha")
		{
			if(!TokenStream.ReadVector3(ParticleEmitter2.Data().Alpha)) return FALSE;
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "ParticleScaling")
		{
			if(!TokenStream.ReadVector3(ParticleEmitter2.Data().ParticleScaling)) return FALSE;
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "LifeSpanUVAnim")
		{
			if(!TokenStream.ReadVector3(ParticleEmitter2.Data().HeadLifeSpan)) return FALSE;
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "DecayUVAnim")
		{
			if(!TokenStream.ReadVector3(ParticleEmitter2.Data().HeadDecay)) return FALSE;
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "TailUVAnim")
		{
			if(!TokenStream.ReadVector3(ParticleEmitter2.Data().TailLifeSpan)) return FALSE;
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "TailDecayUVAnim")
		{
			if(!TokenStream.ReadVector3(ParticleEmitter2.Data().TailDecay)) return FALSE;
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "Rows")
		{
			ParticleEmitter2.Data().Rows = TokenStream.ReadInt();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "Columns")
		{
			ParticleEmitter2.Data().Columns = TokenStream.ReadInt();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "TextureID")
		{
			ParticleEmitter2.Data().TextureId = TokenStream.ReadInt();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "Time")
		{
			ParticleEmitter2.Data().Time = TokenStream.ReadFloat();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "LifeSpan")
		{
			ParticleEmitter2.Data().LifeSpan = TokenStream.ReadFloat();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "TailLength")
		{
			ParticleEmitter2.Data().TailLength = TokenStream.ReadFloat();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "SortPrimsFarZ,")
		{
			ParticleEmitter2.Data().SortPrimitivesFarZ = TRUE;
		}
		else if(Token == "LineEmitter,")
		{
			ParticleEmitter2.Data().LineEmitter = TRUE;
		}
		else if(Token == "ModelSpace,")
		{
			ParticleEmitter2.Data().ModelSpace = TRUE;
		}
		else if(Token == "AlphaKey,")
		{
			ParticleEmitter2.Data().AlphaKey = TRUE;
		}
		else if(Token == "Unshaded,")
		{
			ParticleEmitter2.Data().Unshaded = TRUE;
		}
		else if(Token == "Unfogged,")
		{
			ParticleEmitter2.Data().Unfogged = TRUE;
		}
		else if(Token == "XYQuad,")
		{
			ParticleEmitter2.Data().XYQuad = TRUE;
		}
		else if(Token == "Squirt,")
		{
			ParticleEmitter2.Data().Squirt = TRUE;
		}
		else if(Token == "Head,")
		{
			ParticleEmitter2.Data().Head = TRUE;
		}
		else if(Token == "Tail,")
		{
			ParticleEmitter2.Data().Tail = TRUE;
		}
		else if(Token == "Both,")
		{
			ParticleEmitter2.Data().Head = TRUE;
			ParticleEmitter2.Data().Tail = TRUE;
		}
		else
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"ParticleEmitter2\"!");
			return FALSE;
		}
	}

	Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unexpected EOF reached!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Loads the model ribbon emitters
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadRibbonEmitters(MODEL& Model, TOKEN_IN_STREAM& TokenStream)
{
	MODEL_RIBBON_EMITTER* RibbonEmitter;

	RibbonEmitter = new MODEL_RIBBON_EMITTER();
	if(RibbonEmitter == NULL)
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
		return FALSE;
	}

	if(!LoadRibbonEmitter((*RibbonEmitter), TokenStream))
	{
		delete RibbonEmitter;
		return FALSE;
	}

	if(!Model.AddRibbonEmitter(RibbonEmitter))
	{
		delete RibbonEmitter;
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model ribbon emitter
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadRibbonEmitter(MODEL_RIBBON_EMITTER& RibbonEmitter, TOKEN_IN_STREAM& TokenStream)
{
	std::string Token;
	BOOL ErrorOccured;

	RibbonEmitter.Data().Name = TokenStream.ReadString();
	if(!TokenStream.ExpectChar('{')) return FALSE;

	while(!TokenStream.Eof())
	{
		Token = TokenStream.ReadWord();
		if(Token == "") break;

		if(Token == "}")
		{
			return TRUE;
		}
		else if(Token == "static")
		{
			Token = TokenStream.ReadWord();
			if(Token == "") break;

			if(LoadStaticBaseData(ErrorOccured, &RibbonEmitter.Data(), TokenStream, Token))
			{
				if(ErrorOccured) return FALSE;
			}
			else if(Token == "HeightAbove")
			{
				RibbonEmitter.Data().HeightAbove.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else if(Token == "HeightBelow")
			{
				RibbonEmitter.Data().HeightBelow.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else if(Token == "Alpha")
			{
				RibbonEmitter.Data().Alpha.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else if(Token == "Color")
			{
				D3DXVECTOR3 Vector;

				if(!TokenStream.ReadVector3(Vector)) return FALSE;
				RibbonEmitter.Data().Color.SetStaticVector3(Vector);
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else if(Token == "TextureSlot")
			{
				RibbonEmitter.Data().TextureSlot.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else if(Token == "Visibility")
			{
				RibbonEmitter.Data().Visibility.SetStaticScalar(TokenStream.ReadFloat());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}
			else
			{
				Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"static " + Token + "\" in \"RibbonEmitter\"!");
				return FALSE;
			}
		}
		else if(LoadBaseData(ErrorOccured, RibbonEmitter.BaseData(), TokenStream, Token))
		{
			if(ErrorOccured) return FALSE;
		}
		else if(Token == "HeightAbove")
		{
			if(!RibbonEmitter.Data().HeightAbove.Load(TokenStream)) return FALSE;
		}
		else if(Token == "HeightBelow")
		{
			if(!RibbonEmitter.Data().HeightBelow.Load(TokenStream)) return FALSE;
		}
		else if(Token == "Alpha")
		{
			if(!RibbonEmitter.Data().Alpha.Load(TokenStream)) return FALSE;
		}
		else if(Token == "Color")
		{
			if(!RibbonEmitter.Data().Color.Load(TokenStream)) return FALSE;
		}
		else if(Token == "TextureSlot")
		{
			if(!RibbonEmitter.Data().TextureSlot.Load(TokenStream)) return FALSE;
		}
		else if(Token == "Visibility")
		{
			if(!RibbonEmitter.Data().Visibility.Load(TokenStream)) return FALSE;
		}
		else if(Token == "EmissionRate")
		{
			RibbonEmitter.Data().EmissionRate = TokenStream.ReadFloat();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "LifeSpan")
		{
			RibbonEmitter.Data().LifeSpan = TokenStream.ReadFloat();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "Gravity")
		{
			RibbonEmitter.Data().Gravity = TokenStream.ReadFloat();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "Rows")
		{
			RibbonEmitter.Data().Rows = TokenStream.ReadInt();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "Columns")
		{
			RibbonEmitter.Data().Columns = TokenStream.ReadInt();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "MaterialID")
		{
			RibbonEmitter.Data().MaterialId = TokenStream.ReadInt();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"RibbonEmitter\"!");
			return FALSE;
		}
	}

	Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unexpected EOF reached!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Loads the model event objects
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadEventObjects(MODEL& Model, TOKEN_IN_STREAM& TokenStream)
{
	MODEL_EVENT_OBJECT* EventObject;

	EventObject = new MODEL_EVENT_OBJECT();
	if(EventObject == NULL)
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
		return FALSE;
	}

	if(!LoadEventObject((*EventObject), TokenStream))
	{
		delete EventObject;
		return FALSE;
	}

	if(!Model.AddEventObject(EventObject))
	{
		delete EventObject;
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model event object
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadEventObject(MODEL_EVENT_OBJECT& EventObject, TOKEN_IN_STREAM& TokenStream)
{
	std::string Token;
	BOOL ErrorOccured;

	EventObject.Data().Name = TokenStream.ReadString();
	if(!TokenStream.ExpectChar('{')) return FALSE;

	while(!TokenStream.Eof())
	{
		Token = TokenStream.ReadWord();
		if(Token == "") break;

		if(Token == "}")
		{
			return TRUE;
		}
		else if(Token == "static")
		{
			Token = TokenStream.ReadWord();
			if(Token == "") break;

			if(LoadStaticBaseData(ErrorOccured, &EventObject.Data(), TokenStream, Token))
			{
				if(ErrorOccured) return FALSE;
			}
			else
			{
				Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"static " + Token + "\" in \"EventObject\"!");
				return FALSE;
			}
		}
		else if(LoadBaseData(ErrorOccured, EventObject.BaseData(), TokenStream, Token))
		{
			if(ErrorOccured) return FALSE;
		}
		else if(Token == "EventTrack")
		{
			INT i;
			INT NrOfEventTracks;

			NrOfEventTracks = TokenStream.ReadInt();
			if(!TokenStream.ExpectChar('{')) return FALSE;

			for(i = 0; i < NrOfEventTracks; i++)
			{
				EventObject.Data().EventTrack.push_back(TokenStream.ReadInt());
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}

			if(!TokenStream.ExpectChar('}')) return FALSE;
		}
		else
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"EventObject\"!");
			return FALSE;
		}
	}

	Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unexpected EOF reached!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Loads the model cameras
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadCameras(MODEL& Model, TOKEN_IN_STREAM& TokenStream)
{
	MODEL_CAMERA* Camera;

	Camera = new MODEL_CAMERA();
	if(Camera == NULL)
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
		return FALSE;
	}

	if(!LoadCamera((*Camera), TokenStream))
	{
		delete Camera;
		return FALSE;
	}

	if(!Model.AddCamera(Camera))
	{
		delete Camera;
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model camera
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadCamera(MODEL_CAMERA& Camera, TOKEN_IN_STREAM& TokenStream)
{
	std::string Token;

	Camera.Data().Name = TokenStream.ReadString();
	if(!TokenStream.ExpectChar('{')) return FALSE;

	while(!TokenStream.Eof())
	{
		Token = TokenStream.ReadWord();
		if(Token == "") break;

		if(Token == "}")
		{
			return TRUE;
		}
		else if(Token == "FieldOfView")
		{
			Camera.Data().FieldOfView = TokenStream.ReadFloat();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "FarClip")
		{
			Camera.Data().FarDistance = TokenStream.ReadFloat();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "NearClip")
		{
			Camera.Data().NearDistance = TokenStream.ReadFloat();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "Position")
		{
			if(!TokenStream.ReadVector3(Camera.Data().Source)) return FALSE;
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "Translation")
		{
			if(!Camera.Data().SourceTranslation.Load(TokenStream)) return FALSE;
		}
		else if(Token == "Rotation")
		{
			if(!Camera.Data().Rotation.Load(TokenStream)) return FALSE;
		}
		else if(Token == "static")
		{
			Token = TokenStream.ReadWord();
			if(Token == "") break;

			if(Token == "Translation")
			{
				D3DXVECTOR3 Vector;

				if(!TokenStream.ReadVector3(Vector)) return TRUE;
				Camera.Data().SourceTranslation.SetStaticVector3(Vector);
				if(!TokenStream.ExpectChar(',')) return TRUE;
			}
			else
			{
				Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"static " + Token + "\" in \"Camera\"!");
				return FALSE;
			}
		}
		else if(Token == "Target")
		{
			if(!TokenStream.ExpectChar('{')) return FALSE;

			while(TRUE)
			{
				Token = TokenStream.ReadWord();
				if(Token == "") break;

				if(Token == "}")
				{
					break;
				}
				else if(Token == "Position")
				{
					if(!TokenStream.ReadVector3(Camera.Data().Target)) return FALSE;
					if(!TokenStream.ExpectChar(',')) return FALSE;
				}
				else if(Token == "Translation")
				{
					if(!Camera.Data().TargetTranslation.Load(TokenStream)) return FALSE;
				}
				else if(Token == "static")
				{
					Token = TokenStream.ReadWord();
					if(Token == "") break;

					if(Token == "Translation")
					{
						D3DXVECTOR3 Vector;

						if(!TokenStream.ReadVector3(Vector)) return TRUE;
						Camera.Data().TargetTranslation.SetStaticVector3(Vector);
						if(!TokenStream.ExpectChar(',')) return TRUE;
					}
					else
					{
						Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"static " + Token + "\" in \"Camera.Target\"!");
						return FALSE;
					}
				}
				else
				{
					Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"Camera.Target\"!");
					return FALSE;
				}
			}
		}
		else
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"Camera\"!");
			return FALSE;
		}
	}

	Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unexpected EOF reached!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Loads the model collision shapes
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadCollisionShapes(MODEL& Model, TOKEN_IN_STREAM& TokenStream)
{
	MODEL_COLLISION_SHAPE* CollisionShape;

	CollisionShape = new MODEL_COLLISION_SHAPE();
	if(CollisionShape == NULL)
	{
		Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
		return FALSE;
	}

	if(!LoadCollisionShape((*CollisionShape), TokenStream))
	{
		delete CollisionShape;
		return FALSE;
	}

	if(!Model.AddCollisionShape(CollisionShape))
	{
		delete CollisionShape;
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model collision shape
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadCollisionShape(MODEL_COLLISION_SHAPE& CollisionShape, TOKEN_IN_STREAM& TokenStream)
{
	std::string Token;
	BOOL ErrorOccured;

	CollisionShape.Data().Name = TokenStream.ReadString();
	if(!TokenStream.ExpectChar('{')) return FALSE;

	while(!TokenStream.Eof())
	{
		Token = TokenStream.ReadWord();
		if(Token == "") break;

		if(Token == "}")
		{
			return TRUE;
		}
		else if(Token == "static")
		{
			Token = TokenStream.ReadWord();
			if(Token == "") break;

			if(LoadStaticBaseData(ErrorOccured, &CollisionShape.Data(), TokenStream, Token))
			{
				if(ErrorOccured) return FALSE;
			}
			else
			{
				Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"static " + Token + "\" in \"CollisionShape\"!");
				return FALSE;
			}
		}
		else if(LoadBaseData(ErrorOccured, CollisionShape.BaseData(), TokenStream, Token))
		{
			if(ErrorOccured) return FALSE;
		}
		else if(Token == "Box,")
		{
			CollisionShape.Data().Type = COLLISION_SHAPE_TYPE_BOX;
		}
		else if(Token == "Sphere,")
		{
			CollisionShape.Data().Type = COLLISION_SHAPE_TYPE_SPHERE;
		}
		else if(Token == "BoundsRadius")
		{
			CollisionShape.Data().BoundsRadius = TokenStream.ReadFloat();
			if(!TokenStream.ExpectChar(',')) return FALSE;
		}
		else if(Token == "Vertices")
		{
			INT i;
			INT NrOfVertices;
			D3DXVECTOR3 Vector;

			NrOfVertices = TokenStream.ReadInt();
			if(!TokenStream.ExpectChar('{')) return FALSE;

			CollisionShape.Data().VertexList.clear();
			for(i = 0; i < NrOfVertices; i++)
			{
				if(!TokenStream.ReadVector3(Vector)) return FALSE;
				CollisionShape.Data().VertexList.push_back(Vector);
				if(!TokenStream.ExpectChar(',')) return FALSE;
			}

			if(!TokenStream.ExpectChar('}')) return FALSE;
		}
		else
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unknown token \"" + Token + "\" in \"CollisionShape\"!");
			return FALSE;
		}
	}

	Error.SetMessage("Unable to load \"" + CurrentFileName + "\", unexpected EOF reached!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Saves the basic data
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SaveBaseData(MODEL_BASE_DATA* BaseData, TOKEN_OUT_STREAM& TokenStream)
{
	INT DontInheritCounter;

	if(BaseData->ObjectId != INVALID_INDEX)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("ObjectId ");
		TokenStream.WriteInt(BaseData->ObjectId);
		TokenStream.WriteLine(",");
	}

	if(BaseData->ParentId != INVALID_INDEX)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("Parent ");
		TokenStream.WriteInt(BaseData->ParentId);
		TokenStream.WriteLine(",");
	}

	DontInheritCounter = 0;
	if(BaseData->DontInheritTranslation) DontInheritCounter++;
	if(BaseData->DontInheritRotation) DontInheritCounter++;
	if(BaseData->DontInheritScaling) DontInheritCounter++;

	while(DontInheritCounter > 0)
	{
		TokenStream.WriteTab();
		TokenStream.WriteWord("DontInherit { ");

		if(BaseData->DontInheritTranslation)
		{
			TokenStream.WriteWord("Translation");

			DontInheritCounter--;
			if(DontInheritCounter <= 0)
			{
				TokenStream.WriteLine(" },");
				break;
			}

			TokenStream.WriteWord(", ");
		}

		if(BaseData->DontInheritRotation)
		{
			TokenStream.WriteWord("Rotation");

			DontInheritCounter--;
			if(DontInheritCounter <= 0)
			{
				TokenStream.WriteLine(" },");
				break;
			}

			TokenStream.WriteWord(", ");
		}

		if(BaseData->DontInheritScaling)
		{
			TokenStream.WriteWord("Scaling");
			TokenStream.WriteLine(" },");
		}

		break;
	}

	if(BaseData->Billboarded)
	{
		TokenStream.WriteTab();
		TokenStream.WriteLine("Billboarded,");
	}

	if(BaseData->BillboardedLockX)
	{
		TokenStream.WriteTab();
		TokenStream.WriteLine("BillboardedLockX,");
	}

	if(BaseData->BillboardedLockY)
	{
		TokenStream.WriteTab();
		TokenStream.WriteLine("BillboardedLockY,");
	}

	if(BaseData->BillboardedLockZ)
	{
		TokenStream.WriteTab();
		TokenStream.WriteLine("BillboardedLockZ,");
	}

	if(BaseData->CameraAnchored)
	{
		TokenStream.WriteTab();
		TokenStream.WriteLine("CameraAnchored,");
	}

	while(TRUE)
	{
		if(BaseData->Translation.IsStatic())
		{
			if(BaseData->Translation.GetVector3() == D3DXVECTOR3(0.0f, 0.0f, 0.0f)) break;
		}

		if(!BaseData->Translation.Save(TokenStream)) return FALSE;

		break;
	}

	while(TRUE)
	{
		if(BaseData->Rotation.IsStatic())
		{
			if(BaseData->Rotation.GetVector4() == D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f)) break;
		}

		if(!BaseData->Rotation.Save(TokenStream)) return FALSE;

		break;
	}

	while(TRUE)
	{
		if(BaseData->Scaling.IsStatic())
		{
			if(BaseData->Scaling.GetVector3() == D3DXVECTOR3(1.0f, 1.0f, 1.0f)) break;
		}

		if(!BaseData->Scaling.Save(TokenStream)) return FALSE;

		break;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the basic data
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadBaseData(BOOL& ErrorOccured, MODEL_BASE_DATA* BaseData, TOKEN_IN_STREAM& TokenStream, CONST std::string& Token)
{
	std::string NewToken;

	ErrorOccured = TRUE;

	if(Token == "ObjectId")
	{
		BaseData->ObjectId = TokenStream.ReadInt();
		if(!TokenStream.ExpectChar(',')) return TRUE;

		ObjectIdManager.AddObjectId(BaseData->ObjectId);
	}
	else if(Token == "Parent")
	{
		BaseData->ParentId = TokenStream.ReadInt();
		if(!TokenStream.ExpectChar(',')) return TRUE;
	}
	else if(Token == "Translation")
	{
		if(!BaseData->Translation.Load(TokenStream)) return TRUE;
	}
	else if(Token == "Rotation")
	{
		if(!BaseData->Rotation.Load(TokenStream)) return TRUE;
	}
	else if(Token == "Scaling")
	{
		if(!BaseData->Scaling.Load(TokenStream)) return TRUE;
	}
	else if(Token == "DontInherit")
	{
		if(!TokenStream.ExpectChar('{')) return TRUE;

		while(TRUE)
		{
			NewToken = TokenStream.ReadWord();
			if(NewToken == "") return TRUE;

			if(NewToken == "}")
			{
				if(!TokenStream.ExpectChar(',')) return TRUE;
				break;
			}
			else if(NewToken == "},")
			{
				break;
			}
			else if((NewToken == "Translation,") || (NewToken == "Translation"))
			{
				BaseData->DontInheritTranslation = TRUE;
			}
			else if((NewToken == "Rotation,") || (NewToken == "Rotation"))
			{
				BaseData->DontInheritRotation = TRUE;
			}
			else if((NewToken == "Scaling,") || (NewToken == "Scaling"))
			{
				BaseData->DontInheritScaling = TRUE;
			}
			else
			{
				Error.SetMessage("Unknown DontInherit token \"" + NewToken + "\"!");
				return TRUE;
			}
		}
	}
	else if(Token == "Billboarded,")
	{
		BaseData->Billboarded = TRUE;
	}
	else if(Token == "BillboardedLockX,")
	{
		BaseData->BillboardedLockX = TRUE;
	}
	else if(Token == "BillboardedLockY,")
	{
		BaseData->BillboardedLockY = TRUE;
	}
	else if(Token == "BillboardedLockZ,")
	{
		BaseData->BillboardedLockZ = TRUE;
	}
	else if(Token == "CameraAnchored,")
	{
		BaseData->CameraAnchored = TRUE;
	}
	else
	{
		return FALSE;
	}

	ErrorOccured = FALSE;
	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the basic static data
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::LoadStaticBaseData(BOOL& ErrorOccured, MODEL_BASE_DATA* BaseData, TOKEN_IN_STREAM& TokenStream, CONST std::string& Token)
{
	ErrorOccured = TRUE;

	if(Token == "Translation")
	{
		D3DXVECTOR3 Vector;

		if(!TokenStream.ReadVector3(Vector)) return TRUE;
		BaseData->Translation.SetStaticVector3(Vector);
		if(!TokenStream.ExpectChar(',')) return TRUE;
	}
	else if(Token == "Rotation")
	{
		D3DXVECTOR4 Vector;

		if(!TokenStream.ReadVector3(Vector)) return TRUE;
		BaseData->Rotation.SetStaticVector4(Vector);
		if(!TokenStream.ExpectChar(',')) return TRUE;
	}
	else if(Token == "Scaling")
	{
		D3DXVECTOR3 Vector;

		if(!TokenStream.ReadVector3(Vector)) return TRUE;
		BaseData->Scaling.SetStaticVector3(Vector);
		if(!TokenStream.ExpectChar(',')) return TRUE;
	}
	else
	{
		return FALSE;
	}

	ErrorOccured = FALSE;
	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Skips a model group
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDL::SkipGroup(TOKEN_IN_STREAM& TokenStream)
{
	INT Counter = 1;
	CHAR Char;

	TokenStream.ReadLine();

	while(Counter > 0)
	{
		Char = TokenStream.ReadChar();

		if(TokenStream.Eof())
		{
			Error.SetMessage("Unexpected EOF while skipping group \"" + TokenStream.GetGroup() + "\"!");
			return FALSE;
		}

		if(Char == '{') Counter++;
		if(Char == '}') Counter--;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Returns the current date as a string
//+-----------------------------------------------------------------------------
std::string MODEL_LOADER_MDL::GetCurrentDate()
{
	INT Size;
	DWORD Locale;
	std::string Date;
	std::string Time;
	std::vector<CHAR> Buffer;

	Locale = MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), SORT_DEFAULT);

	Size = GetDateFormat(Locale, 0, NULL, STRING_DATE_FORMAT.c_str(), NULL, 0);
	Buffer.resize(Size + 1);

	GetDateFormat(Locale, 0, NULL, STRING_DATE_FORMAT.c_str(), &Buffer[0], Size);
	Buffer[Size] = '\0';
	Date = &Buffer[0];

	Size = GetTimeFormat(Locale, 0, NULL, STRING_TIME_FORMAT.c_str(), NULL, 0);
	Buffer.resize(Size + 1);

	GetTimeFormat(Locale, 0, NULL, STRING_TIME_FORMAT.c_str(), &Buffer[0], Size);
	Buffer[Size] = '\0';
	Time = &Buffer[0];

	return Date + " " + Time;
}
