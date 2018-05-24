//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ModelLoaderMdx.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
MODEL_LOADER_MDX ModelLoaderMdx;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_LOADER_MDX::MODEL_LOADER_MDX()
{
	RegisterMdxLoader('VERS', LoadVersion);
	RegisterMdxLoader('MODL', LoadModel);
	RegisterMdxLoader('SEQS', LoadSequences);
	RegisterMdxLoader('GLBS', LoadGlobalSequences);
	RegisterMdxLoader('TEXS', LoadTextures);
	RegisterMdxLoader('MTLS', LoadMaterials);
	RegisterMdxLoader('TXAN', LoadTextureAnimations);
	RegisterMdxLoader('GEOS', LoadGeosets);
	RegisterMdxLoader('GEOA', LoadGeosetAnimations);
	RegisterMdxLoader('BONE', LoadBones);
	RegisterMdxLoader('LITE', LoadLights);
	RegisterMdxLoader('HELP', LoadHelpers);
	RegisterMdxLoader('ATCH', LoadAttachments);
	RegisterMdxLoader('PIVT', LoadPivotPoints);
	RegisterMdxLoader('PREM', LoadParticleEmitters);
	RegisterMdxLoader('PRE2', LoadParticleEmitters2);
	RegisterMdxLoader('RIBB', LoadRibbonEmitters);
	RegisterMdxLoader('EVTS', LoadEventObjects);
	RegisterMdxLoader('CAMS', LoadCameras);
	RegisterMdxLoader('CLID', LoadCollisionShapes);
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL_LOADER_MDX::~MODEL_LOADER_MDX()
{
	MdxLoaderMap.clear();
}


//+-----------------------------------------------------------------------------
//| Saves a model to a buffer
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::Save(MODEL& Model, CONST std::string& FileName, BUFFER& Buffer)
{
	DATA_OUT_STREAM DataStream;

	CurrentFileName = FileName;
	DataStream.SetFileName(FileName);

	Model.GenerateObjectIds();
	Model.WrapPivotPoints();

	DataStream.WriteDWord(ReverseDWord('MDLX'));

	if(!SaveVersion(Model, DataStream)) return FALSE;
	if(!SaveModel(Model, DataStream)) return FALSE;
	if(!SaveSequences(Model, DataStream)) return FALSE;
	if(!SaveGlobalSequences(Model, DataStream)) return FALSE;
	if(!SaveMaterials(Model, DataStream)) return FALSE;
	if(!SaveTextures(Model, DataStream)) return FALSE;
	if(!SaveTextureAnimations(Model, DataStream)) return FALSE;
	if(!SaveGeosets(Model, DataStream)) return FALSE;
	if(!SaveGeosetAnimations(Model, DataStream)) return FALSE;
	if(!SaveBones(Model, DataStream)) return FALSE;
	if(!SaveLights(Model, DataStream)) return FALSE;
	if(!SaveHelpers(Model, DataStream)) return FALSE;
	if(!SaveAttachments(Model, DataStream)) return FALSE;
	if(!SavePivotPoints(Model, DataStream)) return FALSE;
	if(!SaveParticleEmitters(Model, DataStream)) return FALSE;
	if(!SaveParticleEmitters2(Model, DataStream)) return FALSE;
	if(!SaveRibbonEmitters(Model, DataStream)) return FALSE;
	if(!SaveCameras(Model, DataStream)) return FALSE;
	if(!SaveEventObjects(Model, DataStream)) return FALSE;
	if(!SaveCollisionShapes(Model, DataStream)) return FALSE;

	if(!DataStream.Save(Buffer)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model from a buffer
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::Load(MODEL& Model, CONST std::string& FileName, BUFFER& Buffer)
{
	INT Size;
	DWORD Group;
	DATA_IN_STREAM DataStream;
	std::map<DWORD, MDX_LOADER>::iterator i;

	CurrentFileName = FileName;
	DataStream.SetFileName(FileName);

	if(!DataStream.Load(Buffer)) return FALSE;

	if(ReverseDWord(DataStream.ReadDWord()) != 'MDLX')
	{
		Error.SetMessage("The file is not an MDX model!");
		return FALSE;
	}

	while(!DataStream.Eof())
	{
		Group = ReverseDWord(DataStream.ReadDWord());
		Size = static_cast<INT>(DataStream.ReadDWord());

		i = MdxLoaderMap.find(Group);
		if(i == MdxLoaderMap.end())
		{
			Error.SetMessage("Unable to load \"" + FileName + "\", unknown group \"" + GroupToString(Group) + "\"!");
			return FALSE;
		}

		if(!(i->second(Model, DataStream, Size))) return FALSE;
	}

	Model.ConnectNodes();
	Model.UnwrapPivotPoints();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Registers an mdx loader
//+-----------------------------------------------------------------------------
VOID MODEL_LOADER_MDX::RegisterMdxLoader(DWORD Group, MDX_LOADER Loader)
{
	MdxLoaderMap.insert(std::make_pair(Group, Loader));
}


//+-----------------------------------------------------------------------------
//| Reverses a DWORD
//+-----------------------------------------------------------------------------
DWORD MODEL_LOADER_MDX::ReverseDWord(DWORD DWord)
{
	DWORD NewDWord;
	CHAR* Source;
	CHAR* Target;

	Source = reinterpret_cast<CHAR*>(&DWord);
	Target = reinterpret_cast<CHAR*>(&NewDWord);

	Target[0] = Source[3];
	Target[1] = Source[2];
	Target[2] = Source[1];
	Target[3] = Source[0];

	return NewDWord;
}


//+-----------------------------------------------------------------------------
//| Converts a group to a string
//+-----------------------------------------------------------------------------
std::string MODEL_LOADER_MDX::GroupToString(DWORD Group)
{
	CHAR Buffer[5];
	CHAR* Pointer;

	Pointer = reinterpret_cast<CHAR*>(&Group);

	Buffer[0] = Pointer[3];
	Buffer[1] = Pointer[2];
	Buffer[2] = Pointer[1];
	Buffer[3] = Pointer[0];
	Buffer[4] = '\0';

	return Buffer;
}


//+-----------------------------------------------------------------------------
//| Saves the model version
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveVersion(MODEL& Model, DATA_OUT_STREAM& DataStream)
{
	DataStream.WriteDWord(ReverseDWord('VERS'));
	DataStream.WriteDWord(4);
	DataStream.WriteDWord(Model.Data().Info.Version);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model info
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveModel(MODEL& Model, DATA_OUT_STREAM& DataStream)
{
	DataStream.WriteDWord(ReverseDWord('MODL'));
	DataStream.WriteDWord(MODEL_NAME_SIZE + 36);

	DataStream.WriteString(Model.Data().Info.Name, MODEL_NAME_SIZE);

	DataStream.WriteDWord(0); //What is this? (always 0?)

	DataStream.WriteFloat(Model.Data().Info.Extent.Radius);
	DataStream.WriteFloat(Model.Data().Info.Extent.Min.x);
	DataStream.WriteFloat(Model.Data().Info.Extent.Min.y);
	DataStream.WriteFloat(Model.Data().Info.Extent.Min.z);
	DataStream.WriteFloat(Model.Data().Info.Extent.Max.x);
	DataStream.WriteFloat(Model.Data().Info.Extent.Max.y);
	DataStream.WriteFloat(Model.Data().Info.Extent.Max.z);

	DataStream.WriteDWord(Model.Data().Info.BlendTime);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model sequences
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveSequences(MODEL& Model, DATA_OUT_STREAM& DataStream)
{
	INT i;
	INT TotalSize = 0;

	if(Model.Data().SequenceContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().SequenceContainer.GetTotalSize(); i++)
	{
		if(Model.Data().SequenceContainer.ValidIndex(i))
		{
			TotalSize += Model.Data().SequenceContainer[i]->GetSize();
		}
	}

	DataStream.WriteDWord(ReverseDWord('SEQS'));
	DataStream.WriteDWord(TotalSize);

	for(i = 0; i < Model.Data().SequenceContainer.GetTotalSize(); i++)
	{
		if(Model.Data().SequenceContainer.ValidIndex(i))
		{
			if(!SaveSequence(*(Model.Data().SequenceContainer[i]), DataStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model sequence
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveSequence(MODEL_SEQUENCE& Sequence, DATA_OUT_STREAM& DataStream)
{
	DataStream.WriteString(Sequence.Data().Name, MODEL_NAME_SEQUENCE_SIZE);

	DataStream.WriteDWord(static_cast<DWORD>(Sequence.Data().Interval.x));
	DataStream.WriteDWord(static_cast<DWORD>(Sequence.Data().Interval.y));
	DataStream.WriteFloat(Sequence.Data().MoveSpeed);
	DataStream.WriteDWord(Sequence.Data().NonLooping ? 1 : 0);
	DataStream.WriteFloat(Sequence.Data().Rarity);

	DataStream.WriteDWord(0); //What is this?

	DataStream.WriteFloat(Sequence.Data().Extent.Radius);
	DataStream.WriteFloat(Sequence.Data().Extent.Min.x);
	DataStream.WriteFloat(Sequence.Data().Extent.Min.y);
	DataStream.WriteFloat(Sequence.Data().Extent.Min.z);
	DataStream.WriteFloat(Sequence.Data().Extent.Max.x);
	DataStream.WriteFloat(Sequence.Data().Extent.Max.y);
	DataStream.WriteFloat(Sequence.Data().Extent.Max.z);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the global model sequences
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveGlobalSequences(MODEL& Model, DATA_OUT_STREAM& DataStream)
{
	INT i;
	INT TotalSize = 0;

	if(Model.Data().GlobalSequenceContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().GlobalSequenceContainer.GetTotalSize(); i++)
	{
		if(Model.Data().GlobalSequenceContainer.ValidIndex(i))
		{
			TotalSize += Model.Data().GlobalSequenceContainer[i]->GetSize();
		}
	}

	DataStream.WriteDWord(ReverseDWord('GLBS'));
	DataStream.WriteDWord(TotalSize);

	for(i = 0; i < Model.Data().GlobalSequenceContainer.GetTotalSize(); i++)
	{
		if(Model.Data().GlobalSequenceContainer.ValidIndex(i))
		{
			DataStream.WriteDWord(Model.Data().GlobalSequenceContainer[i]->Data().Duration);
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model textures
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveTextures(MODEL& Model, DATA_OUT_STREAM& DataStream)
{
	INT i;
	INT TotalSize = 0;

	if(Model.Data().TextureContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().TextureContainer.GetTotalSize(); i++)
	{
		if(Model.Data().TextureContainer.ValidIndex(i))
		{
			TotalSize += Model.Data().TextureContainer[i]->GetSize();
		}
	}

	DataStream.WriteDWord(ReverseDWord('TEXS'));
	DataStream.WriteDWord(TotalSize);

	for(i = 0; i < Model.Data().TextureContainer.GetTotalSize(); i++)
	{
		if(Model.Data().TextureContainer.ValidIndex(i))
		{
			if(!SaveTexture(*(Model.Data().TextureContainer[i]), DataStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model texture
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveTexture(MODEL_TEXTURE& Texture, DATA_OUT_STREAM& DataStream)
{
	DWORD Flags;

	DataStream.WriteDWord((Texture.Data().ReplaceableId == INVALID_INDEX) ? 0 : Texture.Data().ReplaceableId);
	DataStream.WriteString(Texture.Data().FileName, MODEL_NAME_TEXTURE_SIZE);

	DataStream.WriteDWord(0); //What is this? (always 0?)

	Flags = 0;
	if(Texture.Data().WrapWidth) Flags |= 1;
	if(Texture.Data().WrapHeight) Flags |= 2;

	DataStream.WriteDWord(Flags);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model materials
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveMaterials(MODEL& Model, DATA_OUT_STREAM& DataStream)
{
	INT i;
	INT TotalSize = 0;

	if(Model.Data().MaterialContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().MaterialContainer.GetTotalSize(); i++)
	{
		if(Model.Data().MaterialContainer.ValidIndex(i))
		{
			TotalSize += Model.Data().MaterialContainer[i]->GetSize();
		}
	}

	DataStream.WriteDWord(ReverseDWord('MTLS'));
	DataStream.WriteDWord(TotalSize);

	for(i = 0; i < Model.Data().MaterialContainer.GetTotalSize(); i++)
	{
		if(Model.Data().MaterialContainer.ValidIndex(i))
		{
			if(!SaveMaterial(*(Model.Data().MaterialContainer[i]), DataStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model material
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveMaterial(MODEL_MATERIAL& Material, DATA_OUT_STREAM& DataStream)
{
	INT i;
	DWORD Flags;
	DWORD Temp;
	MODEL_MATERIAL_LAYER* Layer;

	DataStream.WriteDWord(Material.GetSize());

	Flags = 0;
	if(Material.Data().ConstantColor) Flags |= 1;
	if(Material.Data().SortPrimitivesFarZ) Flags |= 16;
	if(Material.Data().FullResolution) Flags |= 32;

	DataStream.WriteDWord(Material.Data().PriorityPlane);
	DataStream.WriteDWord(Flags);

	DataStream.WriteDWord(ReverseDWord('LAYS'));

	DataStream.WriteDWord(Material.Data().LayerContainer.GetSize());
	for(i = 0; i < Material.Data().LayerContainer.GetTotalSize(); i++)
	{
		if(Material.Data().LayerContainer.ValidIndex(i))
		{
			Layer = Material.Data().LayerContainer[i];

			DataStream.WriteDWord(Layer->GetSize());

			switch(Layer->Data().FilterMode)
			{
				case FILTER_MODE_TRANSPARENT:
				{
					DataStream.WriteDWord(1);
					break;
				}

				case FILTER_MODE_BLEND:
				{
					DataStream.WriteDWord(2);
					break;
				}

				case FILTER_MODE_ADDITIVE:
				{
					DataStream.WriteDWord(3);
					break;
				}

				case FILTER_MODE_ADD_ALPHA:
				{
					DataStream.WriteDWord(4);
					break;
				}

				case FILTER_MODE_MODULATE:
				{
					DataStream.WriteDWord(5);
					break;
				}

				default:
				{
					DataStream.WriteDWord(0);
					break;
				}
			}

			Flags = 0;
			if(Layer->Data().Unshaded) Flags |= 1;
			if(Layer->Data().SphereEnvironmentMap) Flags |= 2;
			if(Layer->Data().TwoSided) Flags |= 16;
			if(Layer->Data().Unfogged) Flags |= 32;
			if(Layer->Data().NoDepthTest) Flags |= 64;
			if(Layer->Data().NoDepthSet) Flags |= 128;

			DataStream.WriteDWord(Flags);

			Temp = static_cast<DWORD>(Layer->Data().AnimatedTextureId.GetScalar());

			DataStream.WriteDWord((Temp == INVALID_INDEX) ? 0xFFFFFFFF : Temp);
			DataStream.WriteDWord((Layer->Data().TextureAnimationId == INVALID_INDEX) ? 0xFFFFFFFF : Layer->Data().TextureAnimationId);

			DataStream.WriteDWord(0); //What is this? (always 0?)

			DataStream.WriteFloat(Layer->Data().Alpha.GetScalar());

			if(Layer->Data().Alpha.GetNrOfNodes() > 0)
			{
				DataStream.WriteDWord(ReverseDWord('KMTA'));
				if(!Layer->Data().Alpha.Save(DataStream)) return FALSE;
			}

			if(Layer->Data().AnimatedTextureId.GetNrOfNodes() > 0)
			{
				DataStream.WriteDWord(ReverseDWord('KMTF'));
				if(!Layer->Data().AnimatedTextureId.Save(DataStream)) return FALSE;
			}
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model texture animations
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveTextureAnimations(MODEL& Model, DATA_OUT_STREAM& DataStream)
{
	INT i;
	INT TotalSize = 0;

	if(Model.Data().TextureAnimationContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().TextureAnimationContainer.GetTotalSize(); i++)
	{
		if(Model.Data().TextureAnimationContainer.ValidIndex(i))
		{
			TotalSize += Model.Data().TextureAnimationContainer[i]->GetSize();
		}
	}

	DataStream.WriteDWord(ReverseDWord('TXAN'));
	DataStream.WriteDWord(TotalSize);

	for(i = 0; i < Model.Data().TextureAnimationContainer.GetTotalSize(); i++)
	{
		if(Model.Data().TextureAnimationContainer.ValidIndex(i))
		{
			if(!SaveTextureAnimation(*(Model.Data().TextureAnimationContainer[i]), DataStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model texture animation
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveTextureAnimation(MODEL_TEXTURE_ANIMATION& TextureAnimation, DATA_OUT_STREAM& DataStream)
{
	DataStream.WriteDWord(TextureAnimation.GetSize());

	if(TextureAnimation.Data().Translation.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KTAT'));
		if(!TextureAnimation.Data().Translation.Save(DataStream)) return FALSE;
	}

	if(TextureAnimation.Data().Rotation.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KTAR'));
		if(!TextureAnimation.Data().Rotation.Save(DataStream)) return FALSE;
	}

	if(TextureAnimation.Data().Scaling.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KTAS'));
		if(!TextureAnimation.Data().Scaling.Save(DataStream)) return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model geosets
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveGeosets(MODEL& Model, DATA_OUT_STREAM& DataStream)
{
	INT i;
	INT TotalSize = 0;

	if(Model.Data().GeosetContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().GeosetContainer.GetTotalSize(); i++)
	{
		if(Model.Data().GeosetContainer.ValidIndex(i))
		{
			TotalSize += Model.Data().GeosetContainer[i]->GetSize();
		}
	}

	DataStream.WriteDWord(ReverseDWord('GEOS'));
	DataStream.WriteDWord(TotalSize);

	for(i = 0; i < Model.Data().GeosetContainer.GetTotalSize(); i++)
	{
		if(Model.Data().GeosetContainer.ValidIndex(i))
		{
			if(!SaveGeoset(*(Model.Data().GeosetContainer[i]), DataStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model geoset
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveGeoset(MODEL_GEOSET& Geoset, DATA_OUT_STREAM& DataStream)
{
	INT i;
	INT j;
	INT Size;
	std::list<INT> GroupList;
	std::list<INT>::iterator GroupListIterator;
	MODEL_GEOSET_FACE* Face;
	MODEL_GEOSET_GROUP* Group;
	MODEL_GEOSET_VERTEX* Vertex;
	EXTENT* Extent;

	DataStream.WriteDWord(Geoset.GetSize());

	DataStream.WriteDWord(ReverseDWord('VRTX'));

	DataStream.WriteDWord(Geoset.Data().VertexContainer.GetSize());
	for(i = 0; i < Geoset.Data().VertexContainer.GetTotalSize(); i++)
	{
		if(Geoset.Data().VertexContainer.ValidIndex(i))
		{
			Vertex = Geoset.Data().VertexContainer[i];

			DataStream.WriteFloat(Vertex->Position.x);
			DataStream.WriteFloat(Vertex->Position.y);
			DataStream.WriteFloat(Vertex->Position.z);
		}
	}

	DataStream.WriteDWord(ReverseDWord('NRMS'));

	DataStream.WriteDWord(Geoset.Data().VertexContainer.GetSize());
	for(i = 0; i < Geoset.Data().VertexContainer.GetTotalSize(); i++)
	{
		if(Geoset.Data().VertexContainer.ValidIndex(i))
		{
			Vertex = Geoset.Data().VertexContainer[i];

			DataStream.WriteFloat(Vertex->Normal.x);
			DataStream.WriteFloat(Vertex->Normal.y);
			DataStream.WriteFloat(Vertex->Normal.z);
		}
	}

	DataStream.WriteDWord(ReverseDWord('PTYP'));

	DataStream.WriteDWord(1);
	DataStream.WriteDWord(4);

	DataStream.WriteDWord(ReverseDWord('PCNT'));

	DataStream.WriteDWord(1);
	DataStream.WriteDWord(Geoset.Data().FaceContainer.GetSize() * 3);

	DataStream.WriteDWord(ReverseDWord('PVTX'));

	DataStream.WriteDWord(Geoset.Data().FaceContainer.GetSize() * 3);
	for(i = 0; i < Geoset.Data().FaceContainer.GetTotalSize(); i++)
	{
		if(Geoset.Data().FaceContainer.ValidIndex(i))
		{
			Face = Geoset.Data().FaceContainer[i];

			DataStream.WriteWord(Face->Index1);
			DataStream.WriteWord(Face->Index2);
			DataStream.WriteWord(Face->Index3);
		}
	}

	DataStream.WriteDWord(ReverseDWord('GNDX'));

	DataStream.WriteDWord(Geoset.Data().VertexContainer.GetSize());
	for(i = 0; i < Geoset.Data().VertexContainer.GetTotalSize(); i++)
	{
		if(Geoset.Data().VertexContainer.ValidIndex(i))
		{
			Vertex = Geoset.Data().VertexContainer[i];

			DataStream.WriteChar(Vertex->VertexGroup);
		}
	}

	DataStream.WriteDWord(ReverseDWord('MTGC'));

	DataStream.WriteDWord(Geoset.Data().GroupContainer.GetSize());
	for(i = 0; i < Geoset.Data().GroupContainer.GetTotalSize(); i++)
	{
		if(Geoset.Data().GroupContainer.ValidIndex(i))
		{
			DataStream.WriteDWord(Geoset.Data().GroupContainer[i]->MatrixListSize);
		}
	}

	DataStream.WriteDWord(ReverseDWord('MATS'));

	Size = 0;
	for(i = 0; i < Geoset.Data().GroupContainer.GetTotalSize(); i++)
	{
		if(Geoset.Data().GroupContainer.ValidIndex(i))
		{
			Group = Geoset.Data().GroupContainer[i];

			Size += Group->MatrixListSize;
			for(j = 0; j < Group->MatrixList.GetTotalSize(); j++)
			{
				if(Group->MatrixList.ValidIndex(j))
				{
					GroupList.push_back(Group->MatrixList[j]->NodeId);
				}
			}
		}
	}

	DataStream.WriteDWord(Size);

	GroupListIterator = GroupList.begin();
	while(GroupListIterator != GroupList.end())
	{
		DataStream.WriteDWord(*GroupListIterator);
		GroupListIterator++;
	}

	DataStream.WriteDWord((Geoset.Data().MaterialId == INVALID_INDEX) ? 0 : Geoset.Data().MaterialId);
	DataStream.WriteDWord((Geoset.Data().SelectionGroup == INVALID_INDEX) ? 0 : Geoset.Data().SelectionGroup);
	DataStream.WriteDWord(Geoset.Data().Unselectable ? 4 : 0);

	DataStream.WriteFloat(Geoset.Data().Extent.Radius);
	DataStream.WriteFloat(Geoset.Data().Extent.Min.x);
	DataStream.WriteFloat(Geoset.Data().Extent.Min.y);
	DataStream.WriteFloat(Geoset.Data().Extent.Min.z);
	DataStream.WriteFloat(Geoset.Data().Extent.Max.x);
	DataStream.WriteFloat(Geoset.Data().Extent.Max.y);
	DataStream.WriteFloat(Geoset.Data().Extent.Max.z);

	DataStream.WriteDWord(Geoset.Data().ExtentContainer.GetSize());
	for(i = 0; i < Geoset.Data().ExtentContainer.GetTotalSize(); i++)
	{
		if(Geoset.Data().ExtentContainer.ValidIndex(i))
		{
			Extent = Geoset.Data().ExtentContainer[i];

			DataStream.WriteFloat(Extent->Radius);
			DataStream.WriteFloat(Extent->Min.x);
			DataStream.WriteFloat(Extent->Min.y);
			DataStream.WriteFloat(Extent->Min.z);
			DataStream.WriteFloat(Extent->Max.x);
			DataStream.WriteFloat(Extent->Max.y);
			DataStream.WriteFloat(Extent->Max.z);
		}
	}

	DataStream.WriteDWord(ReverseDWord('UVAS'));

	DataStream.WriteDWord(1);

	DataStream.WriteDWord(ReverseDWord('UVBS'));

	DataStream.WriteDWord(Geoset.Data().VertexContainer.GetSize());
	for(i = 0; i < Geoset.Data().VertexContainer.GetTotalSize(); i++)
	{
		if(Geoset.Data().VertexContainer.ValidIndex(i))
		{
			Vertex = Geoset.Data().VertexContainer[i];

			DataStream.WriteFloat(Vertex->TexturePosition.x);
			DataStream.WriteFloat(Vertex->TexturePosition.y);
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model geoset animations
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveGeosetAnimations(MODEL& Model, DATA_OUT_STREAM& DataStream)
{
	INT i;
	INT TotalSize = 0;

	if(Model.Data().GeosetAnimationContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().GeosetAnimationContainer.GetTotalSize(); i++)
	{
		if(Model.Data().GeosetAnimationContainer.ValidIndex(i))
		{
			TotalSize += Model.Data().GeosetAnimationContainer[i]->GetSize();
		}
	}

	DataStream.WriteDWord(ReverseDWord('GEOA'));
	DataStream.WriteDWord(TotalSize);

	for(i = 0; i < Model.Data().GeosetAnimationContainer.GetTotalSize(); i++)
	{
		if(Model.Data().GeosetAnimationContainer.ValidIndex(i))
		{
			if(!SaveGeosetAnimation(*(Model.Data().GeosetAnimationContainer[i]), DataStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model geoset animation
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveGeosetAnimation(MODEL_GEOSET_ANIMATION& GeosetAnimation, DATA_OUT_STREAM& DataStream)
{
	DWORD Flags;

	DataStream.WriteDWord(GeosetAnimation.GetSize());

	DataStream.WriteFloat(GeosetAnimation.Data().Color.GetScalar());

	Flags = 0;
	if(GeosetAnimation.Data().DropShadow) Flags |= 1;
	if(GeosetAnimation.Data().UseColor) Flags |= 2;

	DataStream.WriteDWord(Flags);

	DataStream.WriteFloat(GeosetAnimation.Data().Color.GetVector3().z);
	DataStream.WriteFloat(GeosetAnimation.Data().Color.GetVector3().y);
	DataStream.WriteFloat(GeosetAnimation.Data().Color.GetVector3().x);

	DataStream.WriteDWord((GeosetAnimation.Data().GeosetId == INVALID_INDEX) ? 0xFFFFFFFF : GeosetAnimation.Data().GeosetId);

	if(GeosetAnimation.Data().Alpha.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KGAO'));
		if(!GeosetAnimation.Data().Alpha.Save(DataStream)) return FALSE;
	}

	if(GeosetAnimation.Data().Color.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KGAC'));
		if(!GeosetAnimation.Data().Color.Save(DataStream)) return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model bones
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveBones(MODEL& Model, DATA_OUT_STREAM& DataStream)
{
	INT i;
	INT TotalSize = 0;

	if(Model.Data().BoneContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().BoneContainer.GetTotalSize(); i++)
	{
		if(Model.Data().BoneContainer.ValidIndex(i))
		{
			TotalSize += Model.Data().BoneContainer[i]->GetSize();
		}
	}

	DataStream.WriteDWord(ReverseDWord('BONE'));
	DataStream.WriteDWord(TotalSize);

	for(i = 0; i < Model.Data().BoneContainer.GetTotalSize(); i++)
	{
		if(Model.Data().BoneContainer.ValidIndex(i))
		{
			if(!SaveBone(*(Model.Data().BoneContainer[i]), DataStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model bone
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveBone(MODEL_BONE& Bone, DATA_OUT_STREAM& DataStream)
{
	if(!SaveBaseData(Bone.BaseData(), DataStream, NODE_FLAG_BONE)) return FALSE;

	DataStream.WriteDWord((Bone.Data().GeosetId == INVALID_INDEX) ? 0xFFFFFFFF : Bone.Data().GeosetId);
	DataStream.WriteDWord((Bone.Data().GeosetAnimationId == INVALID_INDEX) ? 0xFFFFFFFF : Bone.Data().GeosetAnimationId);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model lights
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveLights(MODEL& Model, DATA_OUT_STREAM& DataStream)
{
	INT i;
	INT TotalSize = 0;

	if(Model.Data().LightContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().LightContainer.GetTotalSize(); i++)
	{
		if(Model.Data().LightContainer.ValidIndex(i))
		{
			TotalSize += Model.Data().LightContainer[i]->GetSize();
		}
	}

	DataStream.WriteDWord(ReverseDWord('LITE'));
	DataStream.WriteDWord(TotalSize);

	for(i = 0; i < Model.Data().LightContainer.GetTotalSize(); i++)
	{
		if(Model.Data().LightContainer.ValidIndex(i))
		{
			if(!SaveLight(*(Model.Data().LightContainer[i]), DataStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model light
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveLight(MODEL_LIGHT& Light, DATA_OUT_STREAM& DataStream)
{
	D3DXVECTOR3 TempVector;

	DataStream.WriteDWord(Light.GetSize());

	if(!SaveBaseData(Light.BaseData(), DataStream, NODE_FLAG_LIGHT)) return FALSE;

	DataStream.WriteDWord(Light.Data().Type);

	DataStream.WriteFloat(Light.Data().AttenuationStart.GetScalar());
	DataStream.WriteFloat(Light.Data().AttenuationEnd.GetScalar());

	TempVector = Light.Data().Color.GetVector3();

	DataStream.WriteFloat(TempVector.z);
	DataStream.WriteFloat(TempVector.y);
	DataStream.WriteFloat(TempVector.x);
	DataStream.WriteFloat(Light.Data().Intensity.GetScalar());

	TempVector = Light.Data().AmbientColor.GetVector3();

	DataStream.WriteFloat(TempVector.z);
	DataStream.WriteFloat(TempVector.y);
	DataStream.WriteFloat(TempVector.x);
	DataStream.WriteFloat(Light.Data().AmbientIntensity.GetScalar());

	if(Light.Data().Visibility.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KLAV'));
		if(!Light.Data().Visibility.Save(DataStream)) return FALSE;
	}

	if(Light.Data().Color.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KLAC'));
		if(!Light.Data().Color.Save(DataStream)) return FALSE;
	}

	if(Light.Data().Intensity.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KLAI'));
		if(!Light.Data().Intensity.Save(DataStream)) return FALSE;
	}

	if(Light.Data().AmbientColor.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KLBC'));
		if(!Light.Data().AmbientColor.Save(DataStream)) return FALSE;
	}

	if(Light.Data().AmbientIntensity.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KLBI'));
		if(!Light.Data().AmbientIntensity.Save(DataStream)) return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model helpers
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveHelpers(MODEL& Model, DATA_OUT_STREAM& DataStream)
{
	INT i;
	INT TotalSize = 0;

	if(Model.Data().HelperContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().HelperContainer.GetTotalSize(); i++)
	{
		if(Model.Data().HelperContainer.ValidIndex(i))
		{
			TotalSize += Model.Data().HelperContainer[i]->GetSize();
		}
	}

	DataStream.WriteDWord(ReverseDWord('HELP'));
	DataStream.WriteDWord(TotalSize);

	for(i = 0; i < Model.Data().HelperContainer.GetTotalSize(); i++)
	{
		if(Model.Data().HelperContainer.ValidIndex(i))
		{
			if(!SaveHelper(*(Model.Data().HelperContainer[i]), DataStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model helper
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveHelper(MODEL_HELPER& Helper, DATA_OUT_STREAM& DataStream)
{
	if(!SaveBaseData(Helper.BaseData(), DataStream, NODE_FLAG_HELPER)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model attachments
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveAttachments(MODEL& Model, DATA_OUT_STREAM& DataStream)
{
	INT i;
	INT TotalSize = 0;

	if(Model.Data().AttachmentContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().AttachmentContainer.GetTotalSize(); i++)
	{
		if(Model.Data().AttachmentContainer.ValidIndex(i))
		{
			TotalSize += Model.Data().AttachmentContainer[i]->GetSize();
		}
	}

	DataStream.WriteDWord(ReverseDWord('ATCH'));
	DataStream.WriteDWord(TotalSize);

	for(i = 0; i < Model.Data().AttachmentContainer.GetTotalSize(); i++)
	{
		if(Model.Data().AttachmentContainer.ValidIndex(i))
		{
			if(!SaveAttachment(*(Model.Data().AttachmentContainer[i]), DataStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model attachment
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveAttachment(MODEL_ATTACHMENT& Attachment, DATA_OUT_STREAM& DataStream)
{
	DataStream.WriteDWord(Attachment.GetSize());

	if(!SaveBaseData(Attachment.BaseData(), DataStream, NODE_FLAG_ATTACHMENT)) return FALSE;

	DataStream.WriteString(Attachment.Data().Path, MODEL_NAME_ATTACHMENT_SIZE);
	DataStream.WriteDWord(0); //What's this? (always 0?)
	DataStream.WriteDWord(Attachment.Data().AttachmentId);

	if(Attachment.Data().Visibility.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KATV'));
		if(!Attachment.Data().Visibility.Save(DataStream)) return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model pivot points
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SavePivotPoints(MODEL& Model, DATA_OUT_STREAM& DataStream)
{
	INT i;
	D3DXVECTOR3* Vector;

	if(Model.Data().PivotPointContainer.GetSize() <= 0) return TRUE;

	DataStream.WriteDWord(ReverseDWord('PIVT'));
	DataStream.WriteDWord(Model.Data().PivotPointContainer.GetSize() * 12);

	for(i = 0; i < Model.Data().PivotPointContainer.GetTotalSize(); i++)
	{
		if(Model.Data().PivotPointContainer.ValidIndex(i))
		{
			Vector = Model.Data().PivotPointContainer[i];

			DataStream.WriteFloat(Vector->x);
			DataStream.WriteFloat(Vector->y);
			DataStream.WriteFloat(Vector->z);
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model particle emitters
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveParticleEmitters(MODEL& Model, DATA_OUT_STREAM& DataStream)
{
	INT i;
	INT TotalSize = 0;

	if(Model.Data().ParticleEmitter2Container.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().ParticleEmitterContainer.GetTotalSize(); i++)
	{
		if(Model.Data().ParticleEmitterContainer.ValidIndex(i))
		{
			TotalSize += Model.Data().ParticleEmitterContainer[i]->GetSize();
		}
	}

	DataStream.WriteDWord(ReverseDWord('PREM'));
	DataStream.WriteDWord(TotalSize);

	for(i = 0; i < Model.Data().ParticleEmitterContainer.GetTotalSize(); i++)
	{
		if(Model.Data().ParticleEmitterContainer.ValidIndex(i))
		{
			if(!SaveParticleEmitter(*(Model.Data().ParticleEmitterContainer[i]), DataStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model particle emitter
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveParticleEmitter(MODEL_PARTICLE_EMITTER& ParticleEmitter, DATA_OUT_STREAM& DataStream)
{
	DWORD NodeType;

	DataStream.WriteDWord(ParticleEmitter.GetSize());

	NodeType = NODE_FLAG_PARTICLE_EMITTER;
	if(ParticleEmitter.Data().EmitterUsesMdl) NodeType |= NODE_FLAG_EMITTER_USES_MDL;
	if(ParticleEmitter.Data().EmitterUsesTga) NodeType |= NODE_FLAG_EMITTER_USES_TGA;

	if(!SaveBaseData(ParticleEmitter.BaseData(), DataStream, NodeType)) return FALSE;

	DataStream.WriteFloat(ParticleEmitter.Data().EmissionRate.GetScalar());
	DataStream.WriteFloat(ParticleEmitter.Data().Gravity.GetScalar());
	DataStream.WriteFloat(ParticleEmitter.Data().Longitude.GetScalar());
	DataStream.WriteFloat(ParticleEmitter.Data().Latitude.GetScalar());

	DataStream.WriteString(ParticleEmitter.Data().ParticleFileName, MODEL_NAME_PARTICLE_EMITTER_SIZE);

	DataStream.WriteDWord(0); //What's this? Always 0?

	DataStream.WriteFloat(ParticleEmitter.Data().ParticleLifeSpan.GetScalar());
	DataStream.WriteFloat(ParticleEmitter.Data().ParticleInitialVelocity.GetScalar());

	if(ParticleEmitter.Data().Visibility.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KPEV'));
		if(!ParticleEmitter.Data().Visibility.Save(DataStream)) return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model particle emitters 2
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveParticleEmitters2(MODEL& Model, DATA_OUT_STREAM& DataStream)
{
	INT i;
	INT TotalSize = 0;

	if(Model.Data().ParticleEmitter2Container.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().ParticleEmitter2Container.GetTotalSize(); i++)
	{
		if(Model.Data().ParticleEmitter2Container.ValidIndex(i))
		{
			TotalSize += Model.Data().ParticleEmitter2Container[i]->GetSize();
		}
	}

	DataStream.WriteDWord(ReverseDWord('PRE2'));
	DataStream.WriteDWord(TotalSize);

	for(i = 0; i < Model.Data().ParticleEmitter2Container.GetTotalSize(); i++)
	{
		if(Model.Data().ParticleEmitter2Container.ValidIndex(i))
		{
			if(!SaveParticleEmitter2(*(Model.Data().ParticleEmitter2Container[i]), DataStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model particle emitter 2
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveParticleEmitter2(MODEL_PARTICLE_EMITTER_2& ParticleEmitter2, DATA_OUT_STREAM& DataStream)
{
	DWORD NodeType;

	DataStream.WriteDWord(ParticleEmitter2.GetSize());

	NodeType = NODE_FLAG_PARTICLE_EMITTER;
	if(ParticleEmitter2.Data().XYQuad) NodeType |= NODE_FLAG_XY_QUAD;
	if(ParticleEmitter2.Data().Unshaded) NodeType |= NODE_FLAG_UNSHADED;
	if(ParticleEmitter2.Data().Unfogged) NodeType |= NODE_FLAG_UNFOGGED;
	if(ParticleEmitter2.Data().ModelSpace) NodeType |= NODE_FLAG_MODEL_SPACE;
	if(ParticleEmitter2.Data().LineEmitter) NodeType |= NODE_FLAG_LINE_EMITTER;
	if(ParticleEmitter2.Data().SortPrimitivesFarZ) NodeType |= NODE_FLAG_SORT_PRIMITIVES_FAR_Z;

	if(!SaveBaseData(ParticleEmitter2.BaseData(), DataStream, NodeType)) return FALSE;

	DataStream.WriteFloat(ParticleEmitter2.Data().Speed.GetScalar());
	DataStream.WriteFloat(ParticleEmitter2.Data().Variation.GetScalar());
	DataStream.WriteFloat(ParticleEmitter2.Data().Latitude.GetScalar());
	DataStream.WriteFloat(ParticleEmitter2.Data().Gravity.GetScalar());
	DataStream.WriteFloat(ParticleEmitter2.Data().LifeSpan);
	DataStream.WriteFloat(ParticleEmitter2.Data().EmissionRate.GetScalar());
	DataStream.WriteFloat(ParticleEmitter2.Data().Length.GetScalar());
	DataStream.WriteFloat(ParticleEmitter2.Data().Width.GetScalar());

	switch(ParticleEmitter2.Data().FilterMode)
	{
		case FILTER_MODE_BLEND:
		{
			DataStream.WriteDWord(0);
			break;
		}

		case FILTER_MODE_ADDITIVE:
		{
			DataStream.WriteDWord(1);
			break;
		}

		case FILTER_MODE_MODULATE:
		{
			DataStream.WriteDWord(2);
			break;
		}

		case FILTER_MODE_ADD_ALPHA:
		{
			DataStream.WriteDWord(4);
			break;
		}

		default:
		{
			DataStream.WriteDWord(1);
			break;
		}
	}

	DataStream.WriteDWord(ParticleEmitter2.Data().Rows);
	DataStream.WriteDWord(ParticleEmitter2.Data().Columns);

	if(ParticleEmitter2.Data().Head)
	{
		if(ParticleEmitter2.Data().Tail)
		{
			DataStream.WriteDWord(2);
		}
		else
		{
			DataStream.WriteDWord(0);
		}
	}
	else
	{
		if(ParticleEmitter2.Data().Tail)
		{
			DataStream.WriteDWord(1);
		}
		else
		{
			DataStream.WriteDWord(0); //What value if neither???
		}
	}

	DataStream.WriteFloat(ParticleEmitter2.Data().TailLength);
	DataStream.WriteFloat(ParticleEmitter2.Data().Time);

	DataStream.WriteFloat(ParticleEmitter2.Data().SegmentColor1.z);
	DataStream.WriteFloat(ParticleEmitter2.Data().SegmentColor1.y);
	DataStream.WriteFloat(ParticleEmitter2.Data().SegmentColor1.x);

	DataStream.WriteFloat(ParticleEmitter2.Data().SegmentColor2.z);
	DataStream.WriteFloat(ParticleEmitter2.Data().SegmentColor2.y);
	DataStream.WriteFloat(ParticleEmitter2.Data().SegmentColor2.x);

	DataStream.WriteFloat(ParticleEmitter2.Data().SegmentColor3.z);
	DataStream.WriteFloat(ParticleEmitter2.Data().SegmentColor3.y);
	DataStream.WriteFloat(ParticleEmitter2.Data().SegmentColor3.x);

	DataStream.WriteByte(static_cast<BYTE>(ParticleEmitter2.Data().Alpha.x));
	DataStream.WriteByte(static_cast<BYTE>(ParticleEmitter2.Data().Alpha.y));
	DataStream.WriteByte(static_cast<BYTE>(ParticleEmitter2.Data().Alpha.z));

	DataStream.WriteFloat(ParticleEmitter2.Data().ParticleScaling.x);
	DataStream.WriteFloat(ParticleEmitter2.Data().ParticleScaling.y);
	DataStream.WriteFloat(ParticleEmitter2.Data().ParticleScaling.z);

	DataStream.WriteDWord(static_cast<DWORD>(ParticleEmitter2.Data().HeadLifeSpan.x));
	DataStream.WriteDWord(static_cast<DWORD>(ParticleEmitter2.Data().HeadLifeSpan.y));
	DataStream.WriteDWord(static_cast<DWORD>(ParticleEmitter2.Data().HeadLifeSpan.z));

	DataStream.WriteDWord(static_cast<DWORD>(ParticleEmitter2.Data().HeadDecay.x));
	DataStream.WriteDWord(static_cast<DWORD>(ParticleEmitter2.Data().HeadDecay.y));
	DataStream.WriteDWord(static_cast<DWORD>(ParticleEmitter2.Data().HeadDecay.z));

	DataStream.WriteDWord(static_cast<DWORD>(ParticleEmitter2.Data().TailLifeSpan.x));
	DataStream.WriteDWord(static_cast<DWORD>(ParticleEmitter2.Data().TailLifeSpan.y));
	DataStream.WriteDWord(static_cast<DWORD>(ParticleEmitter2.Data().TailLifeSpan.z));

	DataStream.WriteDWord(static_cast<DWORD>(ParticleEmitter2.Data().TailDecay.x));
	DataStream.WriteDWord(static_cast<DWORD>(ParticleEmitter2.Data().TailDecay.y));
	DataStream.WriteDWord(static_cast<DWORD>(ParticleEmitter2.Data().TailDecay.z));

	DataStream.WriteDWord((ParticleEmitter2.Data().TextureId == INVALID_INDEX) ? 0xFFFFFFFF : ParticleEmitter2.Data().TextureId);
	DataStream.WriteDWord(ParticleEmitter2.Data().Squirt ? 1 : 0);
	DataStream.WriteDWord(ParticleEmitter2.Data().PriorityPlane);
	DataStream.WriteDWord(ParticleEmitter2.Data().ReplaceableId);

	if(ParticleEmitter2.Data().Visibility.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KP2V'));
		if(!ParticleEmitter2.Data().Visibility.Save(DataStream)) return FALSE;
	}

	if(ParticleEmitter2.Data().EmissionRate.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KP2E'));
		if(!ParticleEmitter2.Data().EmissionRate.Save(DataStream)) return FALSE;
	}

	if(ParticleEmitter2.Data().Width.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KP2W'));
		if(!ParticleEmitter2.Data().Width.Save(DataStream)) return FALSE;
	}

	if(ParticleEmitter2.Data().Length.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KP2N'));
		if(!ParticleEmitter2.Data().Length.Save(DataStream)) return FALSE;
	}

	if(ParticleEmitter2.Data().Speed.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KP2S'));
		if(!ParticleEmitter2.Data().Speed.Save(DataStream)) return FALSE;
	}

	if(ParticleEmitter2.Data().Latitude.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KP2L'));
		if(!ParticleEmitter2.Data().Latitude.Save(DataStream)) return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model ribbon emitters
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveRibbonEmitters(MODEL& Model, DATA_OUT_STREAM& DataStream)
{
	INT i;
	INT TotalSize = 0;

	if(Model.Data().RibbonEmitterContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().RibbonEmitterContainer.GetTotalSize(); i++)
	{
		if(Model.Data().RibbonEmitterContainer.ValidIndex(i))
		{
			TotalSize += Model.Data().RibbonEmitterContainer[i]->GetSize();
		}
	}

	DataStream.WriteDWord(ReverseDWord('RIBB'));
	DataStream.WriteDWord(TotalSize);

	for(i = 0; i < Model.Data().RibbonEmitterContainer.GetTotalSize(); i++)
	{
		if(Model.Data().RibbonEmitterContainer.ValidIndex(i))
		{
			if(!SaveRibbonEmitter(*(Model.Data().RibbonEmitterContainer[i]), DataStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model ribbon emitter
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveRibbonEmitter(MODEL_RIBBON_EMITTER& RibbonEmitter, DATA_OUT_STREAM& DataStream)
{
	DataStream.WriteDWord(RibbonEmitter.GetSize());

	if(!SaveBaseData(RibbonEmitter.BaseData(), DataStream, NODE_FLAG_RIBBON_EMITTER)) return FALSE;

	DataStream.WriteFloat(RibbonEmitter.Data().HeightAbove.GetScalar());
	DataStream.WriteFloat(RibbonEmitter.Data().HeightBelow.GetScalar());
	DataStream.WriteFloat(RibbonEmitter.Data().Alpha.GetScalar());

	DataStream.WriteFloat(RibbonEmitter.Data().Color.GetVector3().z);
	DataStream.WriteFloat(RibbonEmitter.Data().Color.GetVector3().y);
	DataStream.WriteFloat(RibbonEmitter.Data().Color.GetVector3().x);

	DataStream.WriteFloat(RibbonEmitter.Data().LifeSpan);

	DataStream.WriteDWord(0); //What's this? (Always 0?)

	DataStream.WriteDWord(static_cast<DWORD>(RibbonEmitter.Data().EmissionRate));
	DataStream.WriteDWord(RibbonEmitter.Data().Rows);
	DataStream.WriteDWord(RibbonEmitter.Data().Columns);
	DataStream.WriteDWord((RibbonEmitter.Data().MaterialId == INVALID_INDEX) ? 0xFFFFFFFF : RibbonEmitter.Data().MaterialId);
	DataStream.WriteFloat(RibbonEmitter.Data().Gravity);

	if(RibbonEmitter.Data().Visibility.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KRVS'));
		if(!RibbonEmitter.Data().Visibility.Save(DataStream)) return FALSE;
	}

	if(RibbonEmitter.Data().HeightAbove.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KRHA'));
		if(!RibbonEmitter.Data().HeightAbove.Save(DataStream)) return FALSE;
	}

	if(RibbonEmitter.Data().HeightBelow.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KRHB'));
		if(!RibbonEmitter.Data().HeightBelow.Save(DataStream)) return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model event objects
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveEventObjects(MODEL& Model, DATA_OUT_STREAM& DataStream)
{
	INT i;
	INT TotalSize = 0;

	if(Model.Data().EventObjectContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().EventObjectContainer.GetTotalSize(); i++)
	{
		if(Model.Data().EventObjectContainer.ValidIndex(i))
		{
			TotalSize += Model.Data().EventObjectContainer[i]->GetSize();
		}
	}

	DataStream.WriteDWord(ReverseDWord('EVTS'));
	DataStream.WriteDWord(TotalSize);

	for(i = 0; i < Model.Data().EventObjectContainer.GetTotalSize(); i++)
	{
		if(Model.Data().EventObjectContainer.ValidIndex(i))
		{
			if(!SaveEventObject(*(Model.Data().EventObjectContainer[i]), DataStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model event object
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveEventObject(MODEL_EVENT_OBJECT& EventObject, DATA_OUT_STREAM& DataStream)
{
	std::list<INT>::iterator i;

	if(!SaveBaseData(EventObject.BaseData(), DataStream, NODE_FLAG_EVENT_OBJECT)) return FALSE;

	DataStream.WriteDWord(ReverseDWord('KEVT'));

	DataStream.WriteDWord(static_cast<DWORD>(EventObject.Data().EventTrack.size()));
	DataStream.WriteDWord((EventObject.Data().GlobalSequenceId == INVALID_INDEX) ? 0xFFFFFFFF : EventObject.Data().GlobalSequenceId);

	i = EventObject.Data().EventTrack.begin();
	while(i != EventObject.Data().EventTrack.end())
	{
		DataStream.WriteDWord(*i);
		i++;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model cameras
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveCameras(MODEL& Model, DATA_OUT_STREAM& DataStream)
{
	INT i;
	INT TotalSize = 0;

	if(Model.Data().CameraContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().CameraContainer.GetTotalSize(); i++)
	{
		if(Model.Data().CameraContainer.ValidIndex(i))
		{
			TotalSize += Model.Data().CameraContainer[i]->GetSize();
		}
	}

	DataStream.WriteDWord(ReverseDWord('CAMS'));
	DataStream.WriteDWord(TotalSize);

	for(i = 0; i < Model.Data().CameraContainer.GetTotalSize(); i++)
	{
		if(Model.Data().CameraContainer.ValidIndex(i))
		{
			if(!SaveCamera(*(Model.Data().CameraContainer[i]), DataStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model camera
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveCamera(MODEL_CAMERA& Camera, DATA_OUT_STREAM& DataStream)
{
	DataStream.WriteDWord(Camera.GetSize());

	DataStream.WriteString(Camera.Data().Name, MODEL_NAME_CAMERA_SIZE);

	DataStream.WriteFloat(Camera.Data().Source.x);
	DataStream.WriteFloat(Camera.Data().Source.y);
	DataStream.WriteFloat(Camera.Data().Source.z);

	DataStream.WriteFloat(Camera.Data().FieldOfView);
	DataStream.WriteFloat(Camera.Data().FarDistance);
	DataStream.WriteFloat(Camera.Data().NearDistance);

	DataStream.WriteFloat(Camera.Data().Target.x);
	DataStream.WriteFloat(Camera.Data().Target.y);
	DataStream.WriteFloat(Camera.Data().Target.z);

	if(Camera.Data().SourceTranslation.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KCTR'));
		if(!Camera.Data().SourceTranslation.Save(DataStream)) return FALSE;
	}

	if(Camera.Data().TargetTranslation.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KTTR'));
		if(!Camera.Data().TargetTranslation.Save(DataStream)) return FALSE;
	}

	if(Camera.Data().Rotation.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KCRL'));
		if(!Camera.Data().Rotation.Save(DataStream)) return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the model collision shapes
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveCollisionShapes(MODEL& Model, DATA_OUT_STREAM& DataStream)
{
	INT i;
	INT TotalSize = 0;

	if(Model.Data().CollisionShapeContainer.GetSize() <= 0) return TRUE;

	for(i = 0; i < Model.Data().CollisionShapeContainer.GetTotalSize(); i++)
	{
		if(Model.Data().CollisionShapeContainer.ValidIndex(i))
		{
			TotalSize += Model.Data().CollisionShapeContainer[i]->GetSize();
		}
	}

	DataStream.WriteDWord(ReverseDWord('CLID'));
	DataStream.WriteDWord(TotalSize);

	for(i = 0; i < Model.Data().CollisionShapeContainer.GetTotalSize(); i++)
	{
		if(Model.Data().CollisionShapeContainer.ValidIndex(i))
		{
			if(!SaveCollisionShape(*(Model.Data().CollisionShapeContainer[i]), DataStream)) return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves a model collision shape
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveCollisionShape(MODEL_COLLISION_SHAPE& CollisionShape, DATA_OUT_STREAM& DataStream)
{
	std::list<D3DXVECTOR3>::iterator i;

	if(!SaveBaseData(CollisionShape.BaseData(), DataStream, NODE_FLAG_COLLISION_SHAPE)) return FALSE;

	switch(CollisionShape.Data().Type)
	{
		case COLLISION_SHAPE_TYPE_BOX:
		{
			if(CollisionShape.Data().VertexList.size() != 2)
			{
				Error.SetMessage("Collision shape boxes needs exactly 2 vertices!");
				return FALSE;
			}

			DataStream.WriteDWord(0);
			break;
		}

		case COLLISION_SHAPE_TYPE_SPHERE:
		{
			if(CollisionShape.Data().VertexList.size() != 1)
			{
				Error.SetMessage("Collision shape spheres needs exactly 1 vertex!");
				return FALSE;
			}

			DataStream.WriteDWord(2);
			break;
		}

		default:
		{
			Error.SetMessage("Unknown collision shape type!");
			return FALSE;
		}
	}

	i = CollisionShape.Data().VertexList.begin();
	while(i != CollisionShape.Data().VertexList.end())
	{
		DataStream.WriteFloat(i->x);
		DataStream.WriteFloat(i->y);
		DataStream.WriteFloat(i->z);

		i++;
	}

	if(CollisionShape.Data().Type == COLLISION_SHAPE_TYPE_SPHERE)
	{
		DataStream.WriteFloat(CollisionShape.Data().BoundsRadius);
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model version
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadVersion(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	Model.Data().Info.Version = DataStream.ReadDWord();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model info
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadModel(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	CHAR Buffer[MODEL_NAME_SIZE + 1];

	Buffer[MODEL_NAME_SIZE] = '\0';

	if(!DataStream.Read(Buffer, MODEL_NAME_SIZE)) return FALSE;
	Model.Data().Info.Name = Buffer;

	DataStream.ReadDWord(); //What is this? (always 0?)

	Model.Data().Info.Extent.Radius = DataStream.ReadFloat();
	Model.Data().Info.Extent.Min.x = DataStream.ReadFloat();
	Model.Data().Info.Extent.Min.y = DataStream.ReadFloat();
	Model.Data().Info.Extent.Min.z = DataStream.ReadFloat();
	Model.Data().Info.Extent.Max.x = DataStream.ReadFloat();
	Model.Data().Info.Extent.Max.y = DataStream.ReadFloat();
	Model.Data().Info.Extent.Max.z = DataStream.ReadFloat();

	Model.Data().Info.BlendTime = static_cast<INT>(DataStream.ReadDWord());

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model sequences
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadSequences(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	INT CurrentSize;
	INT SequenceSize;
	MODEL_SEQUENCE* Sequence;

	CurrentSize = 0;
	while(CurrentSize < Size)
	{
		Sequence = new MODEL_SEQUENCE();
		if(Sequence == NULL)
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
			return FALSE;
		}

		if(!LoadSequence((*Sequence), DataStream, 0))
		{
			delete Sequence;
			return FALSE;
		}

		if(!Model.AddSequence(Sequence))
		{
			delete Sequence;
			return FALSE;
		}

		SequenceSize = Sequence->GetSize();
		CurrentSize += SequenceSize;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model sequence
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadSequence(MODEL_SEQUENCE& Sequence, DATA_IN_STREAM& DataStream, INT Size)
{
	CHAR Buffer[MODEL_NAME_SEQUENCE_SIZE + 1];

	Buffer[MODEL_NAME_SEQUENCE_SIZE] = '\0';

	if(!DataStream.Read(Buffer, MODEL_NAME_SEQUENCE_SIZE)) return FALSE;
	Sequence.Data().Name = Buffer;

	Sequence.Data().Interval.x = static_cast<FLOAT>(DataStream.ReadDWord());
	Sequence.Data().Interval.y = static_cast<FLOAT>(DataStream.ReadDWord());
	Sequence.Data().MoveSpeed = DataStream.ReadFloat();
	Sequence.Data().NonLooping = (DataStream.ReadDWord() == 1);
	Sequence.Data().Rarity = DataStream.ReadFloat();

	DataStream.ReadDWord(); //What is this?

	Sequence.Data().Extent.Radius = DataStream.ReadFloat();
	Sequence.Data().Extent.Min.x = DataStream.ReadFloat();
	Sequence.Data().Extent.Min.y = DataStream.ReadFloat();
	Sequence.Data().Extent.Min.z = DataStream.ReadFloat();
	Sequence.Data().Extent.Max.x = DataStream.ReadFloat();
	Sequence.Data().Extent.Max.y = DataStream.ReadFloat();
	Sequence.Data().Extent.Max.z = DataStream.ReadFloat();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the global model sequences
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadGlobalSequences(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	INT CurrentSize;
	INT GlobalSequenceSize;
	MODEL_GLOBAL_SEQUENCE* GlobalSequence;

	CurrentSize = 0;
	while(CurrentSize < Size)
	{
		GlobalSequence = new MODEL_GLOBAL_SEQUENCE();
		if(GlobalSequence == NULL)
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
			return FALSE;
		}

		GlobalSequence->Data().Duration = DataStream.ReadDWord();

		if(!Model.AddGlobalSequence(GlobalSequence))
		{
			delete GlobalSequence;
			return FALSE;
		}

		GlobalSequenceSize = GlobalSequence->GetSize();
		CurrentSize += GlobalSequenceSize;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model textures
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadTextures(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	INT CurrentSize;
	INT TextureSize;
	MODEL_TEXTURE* Texture;

	TextureSize = 268;

	CurrentSize = 0;
	while(CurrentSize < Size)
	{
		CurrentSize += TextureSize;

		Texture = new MODEL_TEXTURE();
		if(Texture == NULL)
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
			return FALSE;
		}

		if(!LoadTexture((*Texture), DataStream, TextureSize))
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

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model texture
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadTexture(MODEL_TEXTURE& Texture, DATA_IN_STREAM& DataStream, INT Size)
{
	DWORD Flags;
	CHAR Buffer[MODEL_NAME_TEXTURE_SIZE + 1];

	Buffer[MODEL_NAME_TEXTURE_SIZE] = '\0';

	Texture.Data().ReplaceableId = static_cast<INT>(DataStream.ReadDWord());

	if(!DataStream.Read(Buffer, MODEL_NAME_TEXTURE_SIZE)) return FALSE;
	Texture.Data().FileName = Buffer;

	DataStream.ReadDWord(); //What is this? (always 0?)

	Flags = DataStream.ReadDWord();
	Texture.Data().WrapWidth = (Flags & 1);
	Texture.Data().WrapHeight = (Flags & 2);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model materials
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadMaterials(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	INT CurrentSize;
	INT MaterialSize;
	MODEL_MATERIAL* Material;

	CurrentSize = 0;
	while(CurrentSize < Size)
	{
		MaterialSize = DataStream.ReadDWord();
		CurrentSize += MaterialSize;

		Material = new MODEL_MATERIAL();
		if(Material == NULL)
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
			return FALSE;
		}

		if(!LoadMaterial((*Material), DataStream, MaterialSize))
		{
			delete Material;
			return FALSE;
		}

		if(!Model.AddMaterial(Material))
		{
			delete Material;
			return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model material
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadMaterial(MODEL_MATERIAL& Material, DATA_IN_STREAM& DataStream, INT Size)
{
	INT i;
	DWORD Tag;
	DWORD Temp;
	DWORD Flags;
	DWORD NrOfLayerBytes;
	DWORD NrOfLayers;
	MODEL_MATERIAL_LAYER* Layer;
	std::stringstream Stream;

	Material.Data().PriorityPlane = DataStream.ReadDWord();
	Flags = DataStream.ReadDWord();

	Material.Data().ConstantColor = (Flags & 1);
	Material.Data().SortPrimitivesFarZ = (Flags & 16);
	Material.Data().FullResolution = (Flags & 32);

	if(!ExpectTag(DataStream, 'LAYS')) return FALSE;

	NrOfLayers = DataStream.ReadDWord();
	for(i = 0; i < static_cast<INT>(NrOfLayers); i++)
	{
		Layer = new MODEL_MATERIAL_LAYER();
		if(Layer == NULL)
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
			return FALSE;
		}

		NrOfLayerBytes = DataStream.ReadDWord();

		Temp = DataStream.ReadDWord();
		switch(Temp)
		{
			case 1:
			{
				Layer->Data().FilterMode = FILTER_MODE_TRANSPARENT;
				break;
			}

			case 2:
			{
				Layer->Data().FilterMode = FILTER_MODE_BLEND;
				break;
			}

			case 3:
			{
				Layer->Data().FilterMode = FILTER_MODE_ADDITIVE;
				break;
			}

			case 4:
			{
				Layer->Data().FilterMode = FILTER_MODE_ADD_ALPHA;
				break;
			}

			case 5:
			{
				Layer->Data().FilterMode = FILTER_MODE_MODULATE;
				break;
			}

			default:
			{
				Layer->Data().FilterMode = FILTER_MODE_NONE;
				break;
			}
		}

		Flags = DataStream.ReadDWord();
		Layer->Data().Unshaded = (Flags & 1);
		Layer->Data().SphereEnvironmentMap = (Flags & 2);
		Layer->Data().TwoSided = (Flags & 16);
		Layer->Data().Unfogged = (Flags & 32);
		Layer->Data().NoDepthTest = (Flags & 64);
		Layer->Data().NoDepthSet = (Flags & 128);

		Layer->Data().TextureId = DataStream.ReadDWord();
		Layer->Data().AnimatedTextureId.SetStaticScalarInt(Layer->Data().TextureId);
		Layer->Data().TextureAnimationId = DataStream.ReadDWord();

		DataStream.ReadDWord(); //What is this? (always 0?)

		Layer->Data().Alpha.SetStaticScalar(DataStream.ReadFloat());

		NrOfLayerBytes -= Layer->GetSize();
		while(NrOfLayerBytes > 0)
		{
			Tag = ReverseDWord(DataStream.ReadDWord());

			if(Tag == 'KMTA')
			{
				if(!Layer->Data().Alpha.Load(DataStream)) return FALSE;
				NrOfLayerBytes -= Layer->Data().Alpha.GetSize();
			}
			else if(Tag == 'KMTF')
			{
				if(!Layer->Data().AnimatedTextureId.Load(DataStream)) return FALSE;
				NrOfLayerBytes -= Layer->Data().AnimatedTextureId.GetSize();
				Layer->Data().TextureId = INVALID_INDEX;
			}
			else
			{
				Error.SetMessage("Unknown tag \"" + GroupToString(Tag) + "\" in Material.Layer!");
				return FALSE;
			}
		}

		if(!Material.AddLayer(Layer)) return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model texture animations
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadTextureAnimations(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	INT CurrentSize;
	INT TextureAnimationSize;
	MODEL_TEXTURE_ANIMATION* TextureAnimation;

	CurrentSize = 0;
	while(CurrentSize < Size)
	{
		TextureAnimationSize = DataStream.ReadDWord();
		CurrentSize += TextureAnimationSize;

		TextureAnimation = new MODEL_TEXTURE_ANIMATION();
		if(TextureAnimation == NULL)
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
			return FALSE;
		}

		if(!LoadTextureAnimation((*TextureAnimation), DataStream, TextureAnimationSize))
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

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model texture animation
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadTextureAnimation(MODEL_TEXTURE_ANIMATION& TextureAnimation, DATA_IN_STREAM& DataStream, INT Size)
{
	DWORD Tag;

	Size -= 4;

	while(Size > 0)
	{
		Tag = ReverseDWord(DataStream.ReadDWord());

		if(Tag == 'KTAT')
		{
			if(!TextureAnimation.Data().Translation.Load(DataStream)) return FALSE;
			Size -= TextureAnimation.Data().Translation.GetSize();
		}
		else if(Tag == 'KTAR')
		{
			if(!TextureAnimation.Data().Rotation.Load(DataStream)) return FALSE;
			Size -= TextureAnimation.Data().Rotation.GetSize();
		}
		else if(Tag == 'KTAS')
		{
			if(!TextureAnimation.Data().Scaling.Load(DataStream)) return FALSE;
			Size -= TextureAnimation.Data().Scaling.GetSize();
		}
		else
		{
			Error.SetMessage("Unknown tag \"" + GroupToString(Tag) + "\" in TextureAnimation!");
			return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model geosets
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadGeosets(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	INT CurrentSize;
	INT GeosetSize;
	MODEL_GEOSET* Geoset;

	CurrentSize = 0;
	while(CurrentSize < Size)
	{
		GeosetSize = DataStream.ReadDWord();
		CurrentSize += GeosetSize;

		Geoset = new MODEL_GEOSET();
		if(Geoset == NULL)
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
			return FALSE;
		}

		if(!LoadGeoset((*Geoset), DataStream, GeosetSize))
		{
			delete Geoset;
			return FALSE;
		}

		if(!Model.AddGeoset(Geoset))
		{
			delete Geoset;
			return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model geoset
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadGeoset(MODEL_GEOSET& Geoset, DATA_IN_STREAM& DataStream, INT Size)
{
	INT i;
	INT j;
	INT IndexSize;
	INT FaceType;
	DWORD NrOfVertices;
	DWORD NrOfNormals;
	DWORD NrOfVertexGroups;
	DWORD NrOfTexturePositions;
	DWORD NrOfIndexes;
	DWORD NrOfFaces;
	DWORD NrOfMTGC;
	DWORD NrOfMatrices;
	DWORD NrOfAnimations;
	DWORD NrOfFaceGroups;
	DWORD NrOfFaceTypeGroups;
	DWORD NrOfTextureVertexGroups;
	D3DXVECTOR3 TempVector3;
	D3DXVECTOR2 TempVector2;
	std::stringstream Stream;
	std::vector<D3DXVECTOR3> PositionList;
	std::vector<D3DXVECTOR3> NormalList;
	std::vector<D3DXVECTOR2> TexturePositionList;
	std::vector<INT> GroupList;
	std::vector<DWORD> GroupIndexList;
	MODEL_GEOSET_VERTEX* Vertex;
	MODEL_GEOSET_FACE* Face;
	MODEL_GEOSET_GROUP* Group;
	MODEL_GEOSET_GROUP_NODE* GroupNode;
	EXTENT* Extent;

	if(!ExpectTag(DataStream, 'VRTX')) return FALSE;

	NrOfVertices = DataStream.ReadDWord();

	PositionList.reserve(NrOfVertices);
	NormalList.reserve(NrOfVertices);
	GroupList.reserve(NrOfVertices);
	TexturePositionList.reserve(NrOfVertices);

	for(i = 0; i < static_cast<INT>(NrOfVertices); i++)
	{
		TempVector3.x = DataStream.ReadFloat();
		TempVector3.y = DataStream.ReadFloat();
		TempVector3.z = DataStream.ReadFloat();

		PositionList.push_back(TempVector3);
	}

	if(!ExpectTag(DataStream, 'NRMS')) return FALSE;

	NrOfNormals = DataStream.ReadDWord();
	if(NrOfNormals != NrOfVertices)
	{
		Stream << "Normal count mismatch in \"" << CurrentFileName << "\", " << NrOfNormals << " normals for " << NrOfVertices << " vertices)!";
		Error.SetMessage(Stream.str());
		return FALSE;
	}

	for(i = 0; i < static_cast<INT>(NrOfNormals); i++)
	{
		TempVector3.x = DataStream.ReadFloat();
		TempVector3.y = DataStream.ReadFloat();
		TempVector3.z = DataStream.ReadFloat();

		NormalList.push_back(TempVector3);
	}

	if(!ExpectTag(DataStream, 'PTYP')) return FALSE;
	NrOfFaceTypeGroups = DataStream.ReadDWord();

	for(i = 0; i < static_cast<INT>(NrOfFaceTypeGroups); i++)
	{
		FaceType = DataStream.ReadDWord();
		if(FaceType != 4)
		{
			Stream << "Can only load triangle lists! Expected type 4, got type " << FaceType << "!";
			Error.SetMessage(Stream.str());
			return FALSE;
		}
	}

	if(!ExpectTag(DataStream, 'PCNT')) return FALSE;
	NrOfFaceGroups = DataStream.ReadDWord();

	for(i = 0; i < static_cast<INT>(NrOfFaceGroups); i++)
	{
		NrOfIndexes = DataStream.ReadDWord();
	}

	if(!ExpectTag(DataStream, 'PVTX')) return FALSE;

	NrOfIndexes = DataStream.ReadDWord();
	NrOfFaces = NrOfIndexes / 3;
	if(NrOfIndexes != (NrOfFaces * 3))
	{
		Error.SetMessage("The number of indexes in \"" + CurrentFileName + "\" is not a multiple of 3!");
		return FALSE;
	}

	for(i = 0; i < static_cast<INT>(NrOfFaces); i++)
	{
		Face = new MODEL_GEOSET_FACE();
		if(Face == NULL)
		{
			Error.SetMessage("Unable to create a new face!");
			return FALSE;
		}

		Face->Index1 = DataStream.ReadWord();
		Face->Index2 = DataStream.ReadWord();
		Face->Index3 = DataStream.ReadWord();

		if(!Geoset.AddFace(Face)) return FALSE;
	}

	if(!ExpectTag(DataStream, 'GNDX')) return FALSE;

	NrOfVertexGroups = DataStream.ReadDWord();
	if(NrOfVertexGroups != NrOfVertices)
	{
		Stream << "Vertex group count mismatch in \"" << CurrentFileName << "\", " << NrOfVertexGroups << " vertex groups for " << NrOfVertices << " vertices)!";
		Error.SetMessage(Stream.str());
		return FALSE;
	}

	for(i = 0; i < static_cast<INT>(NrOfVertexGroups); i++)
	{
		GroupList.push_back(DataStream.ReadChar());
	}

	if(!ExpectTag(DataStream, 'MTGC')) return FALSE;

	NrOfMTGC = DataStream.ReadDWord();
	for(i = 0; i < static_cast<INT>(NrOfMTGC); i++)
	{
		Group = new MODEL_GEOSET_GROUP();
		if(Group == NULL)
		{
			Error.SetMessage("Unable to create a new matrix group!");
			return FALSE;
		}

		Group->MatrixListSize = static_cast<INT>(DataStream.ReadDWord());

		if(!Geoset.AddGroup(Group)) return FALSE;
	}

	if(!ExpectTag(DataStream, 'MATS')) return FALSE;

	j = -1;
	IndexSize = 0;
	NrOfMatrices = DataStream.ReadDWord();
	for(i = 0; i < static_cast<INT>(NrOfMatrices); i++)
	{
		if(IndexSize <= 0)
		{
			j++;
			IndexSize = Geoset.Data().GroupContainer[j]->MatrixListSize;
		}

		GroupNode = new MODEL_GEOSET_GROUP_NODE();
		if(GroupNode == NULL)
		{
			Error.SetMessage("Unable to create a new matrix group node!");
			return FALSE;
		}

		GroupNode->NodeId = DataStream.ReadDWord();
		if(!Geoset.Data().GroupContainer[j]->MatrixList.Add(GroupNode)) return FALSE;

		IndexSize--;
	}

	Geoset.Data().MaterialId = static_cast<INT>(DataStream.ReadDWord());
	Geoset.Data().SelectionGroup = DataStream.ReadDWord();
	Geoset.Data().Unselectable = (DataStream.ReadDWord() == 4);

	Geoset.Data().Extent.Radius = DataStream.ReadFloat();
	Geoset.Data().Extent.Min.x = DataStream.ReadFloat();
	Geoset.Data().Extent.Min.y = DataStream.ReadFloat();
	Geoset.Data().Extent.Min.z = DataStream.ReadFloat();
	Geoset.Data().Extent.Max.x = DataStream.ReadFloat();
	Geoset.Data().Extent.Max.y = DataStream.ReadFloat();
	Geoset.Data().Extent.Max.z = DataStream.ReadFloat();

	NrOfAnimations = DataStream.ReadDWord();
	for(i = 0; i < static_cast<INT>(NrOfAnimations); i++)
	{
		Extent = new EXTENT();
		if(Extent == NULL)
		{
			Error.SetMessage("Unable to create a new extent!");
			return FALSE;
		}

		Extent->Radius = DataStream.ReadFloat();
		Extent->Min.x = DataStream.ReadFloat();
		Extent->Min.y = DataStream.ReadFloat();
		Extent->Min.z = DataStream.ReadFloat();
		Extent->Max.x = DataStream.ReadFloat();
		Extent->Max.y = DataStream.ReadFloat();
		Extent->Max.z = DataStream.ReadFloat();

		if(!Geoset.AddExtent(Extent)) return FALSE;
	}

	if(!ExpectTag(DataStream, 'UVAS')) return FALSE;

	NrOfTextureVertexGroups = DataStream.ReadDWord();

	if(!ExpectTag(DataStream, 'UVBS')) return FALSE;

	NrOfTexturePositions = DataStream.ReadDWord();
	if(NrOfTexturePositions != NrOfVertices)
	{
		Stream << "Texture position count mismatch in \"" << CurrentFileName << "\", " << NrOfTexturePositions << " texture positions for " << NrOfVertices << " vertices)!";
		Error.SetMessage(Stream.str());
		return FALSE;
	}

	for(i = 0; i < static_cast<INT>(NrOfTexturePositions); i++)
	{
		TempVector2.x = DataStream.ReadFloat();
		TempVector2.y = DataStream.ReadFloat();

		TexturePositionList.push_back(TempVector2);
	}

	for(i = 0; i < static_cast<INT>(NrOfVertices); i++)
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

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model geoset animations
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadGeosetAnimations(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	INT CurrentSize;
	INT GeosetAnimationSize;
	MODEL_GEOSET_ANIMATION* GeosetAnimation;

	CurrentSize = 0;
	while(CurrentSize < Size)
	{
		GeosetAnimationSize = DataStream.ReadDWord();
		CurrentSize += GeosetAnimationSize;

		GeosetAnimation = new MODEL_GEOSET_ANIMATION();
		if(GeosetAnimation == NULL)
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
			return FALSE;
		}

		if(!LoadGeosetAnimation((*GeosetAnimation), DataStream, GeosetAnimationSize))
		{
			delete GeosetAnimation;
			return FALSE;
		}

		if(!Model.AddGeosetAnimation(GeosetAnimation))
		{
			delete GeosetAnimation;
			return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model geoset animation
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadGeosetAnimation(MODEL_GEOSET_ANIMATION& GeosetAnimation, DATA_IN_STREAM& DataStream, INT Size)
{
	DWORD Tag;
	DWORD Temp;
	DWORD Flags;
	D3DXVECTOR3 TempVector;

	GeosetAnimation.Data().Alpha.SetStaticScalar(DataStream.ReadFloat());

	Flags = DataStream.ReadDWord();

	if(Flags & 1) GeosetAnimation.Data().DropShadow = TRUE;
	if(Flags & 2) GeosetAnimation.Data().UseColor = TRUE;

	TempVector.z = DataStream.ReadFloat();
	TempVector.y = DataStream.ReadFloat();
	TempVector.x = DataStream.ReadFloat();

	GeosetAnimation.Data().Color.SetStaticVector3(TempVector);

	Temp = DataStream.ReadDWord();
	GeosetAnimation.Data().GeosetId = (Temp == 0xFFFFFFFF) ? INVALID_INDEX : Temp;

	Size -= 28;

	while(Size > 0)
	{
		Tag = ReverseDWord(DataStream.ReadDWord());

		if(Tag == 'KGAO')
		{
			if(!GeosetAnimation.Data().Alpha.Load(DataStream)) return FALSE;
			Size -= GeosetAnimation.Data().Alpha.GetSize();
		}
		else if(Tag == 'KGAC')
		{
			if(!GeosetAnimation.Data().Color.Load(DataStream)) return FALSE;
			Size -= GeosetAnimation.Data().Color.GetSize();
		}
		else
		{
			Error.SetMessage("Unknown tag \"" + GroupToString(Tag) + "\" in GeosetAnimation!");
			return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model bones
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadBones(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	INT CurrentSize;
	INT BoneSize;
	MODEL_BONE* Bone;

	CurrentSize = 0;
	while(CurrentSize < Size)
	{
		Bone = new MODEL_BONE();
		if(Bone == NULL)
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
			return FALSE;
		}

		if(!LoadBone((*Bone), DataStream, 0))
		{
			delete Bone;
			return FALSE;
		}

		if(!Model.AddBone(Bone))
		{
			delete Bone;
			return FALSE;
		}

		BoneSize = Bone->GetSize();
		CurrentSize += BoneSize;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model bone
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadBone(MODEL_BONE& Bone, DATA_IN_STREAM& DataStream, INT Size)
{
	DWORD Temp;

	if(!LoadBaseData(Bone.BaseData(), DataStream)) return FALSE;

	Temp = DataStream.ReadDWord();
	Bone.Data().GeosetId = (Temp == 0xFFFFFFFF) ? INVALID_INDEX : Temp;

	Temp = DataStream.ReadDWord();
	Bone.Data().GeosetAnimationId = (Temp == 0xFFFFFFFF) ? INVALID_INDEX : Temp;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model lights
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadLights(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	INT CurrentSize;
	INT LightSize;
	MODEL_LIGHT* Light;

	CurrentSize = 0;
	while(CurrentSize < Size)
	{
		LightSize = DataStream.ReadDWord();
		CurrentSize += LightSize;

		Light = new MODEL_LIGHT();
		if(Light == NULL)
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
			return FALSE;
		}

		if(!LoadLight((*Light), DataStream, LightSize))
		{
			delete Light;
			return FALSE;
		}

		if(!Model.AddLight(Light))
		{
			delete Light;
			return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model light
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadLight(MODEL_LIGHT& Light, DATA_IN_STREAM& DataStream, INT Size)
{
	DWORD Tag;
	D3DXVECTOR3 TempVector;

	if(!LoadBaseData(Light.BaseData(), DataStream)) return FALSE;

	Light.Data().Type = DataStream.ReadDWord();

	Light.Data().AttenuationStart.SetStaticScalar(DataStream.ReadFloat());
	Light.Data().AttenuationEnd.SetStaticScalar(DataStream.ReadFloat());

	TempVector.z = DataStream.ReadFloat();
	TempVector.y = DataStream.ReadFloat();
	TempVector.x = DataStream.ReadFloat();

	Light.Data().Color.SetStaticVector3(TempVector);
	Light.Data().Intensity.SetStaticScalar(DataStream.ReadFloat());

	TempVector.z = DataStream.ReadFloat();
	TempVector.y = DataStream.ReadFloat();
	TempVector.x = DataStream.ReadFloat();

	Light.Data().AmbientColor.SetStaticVector3(TempVector);
	Light.Data().AmbientIntensity.SetStaticScalar(DataStream.ReadFloat());

	Size -= (Light.GetBaseSize() + 48);

	while(Size > 0)
	{
		Tag = ReverseDWord(DataStream.ReadDWord());

		if(Tag == 'KLAV')
		{
			if(!Light.Data().Visibility.Load(DataStream)) return FALSE;
			Size -= Light.Data().Visibility.GetSize();
		}
		else if(Tag == 'KLAC')
		{
			if(!Light.Data().Color.Load(DataStream)) return FALSE;
			Size -= Light.Data().Color.GetSize();
		}
		else if(Tag == 'KLAI')
		{
			if(!Light.Data().Intensity.Load(DataStream)) return FALSE;
			Size -= Light.Data().Intensity.GetSize();
		}
		else if(Tag == 'KLBC')
		{
			if(!Light.Data().AmbientColor.Load(DataStream)) return FALSE;
			Size -= Light.Data().AmbientColor.GetSize();
		}
		else if(Tag == 'KLBI')
		{
			if(!Light.Data().AmbientIntensity.Load(DataStream)) return FALSE;
			Size -= Light.Data().AmbientIntensity.GetSize();
		}
		else
		{
			Error.SetMessage("Unknown tag \"" + GroupToString(Tag) + "\" in Light!");
			return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model helpers
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadHelpers(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	INT CurrentSize;
	INT HelperSize;
	MODEL_HELPER* Helper;

	CurrentSize = 0;
	while(CurrentSize < Size)
	{
		Helper = new MODEL_HELPER();
		if(Helper == NULL)
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
			return FALSE;
		}

		if(!LoadHelper((*Helper), DataStream, 0))
		{
			delete Helper;
			return FALSE;
		}

		if(!Model.AddHelper(Helper))
		{
			delete Helper;
			return FALSE;
		}

		HelperSize = Helper->GetSize();
		CurrentSize += HelperSize;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model helper
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadHelper(MODEL_HELPER& Helper, DATA_IN_STREAM& DataStream, INT Size)
{
	if(!LoadBaseData(Helper.BaseData(), DataStream)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model attachments
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadAttachments(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	INT CurrentSize;
	INT AttachmentSize;
	MODEL_ATTACHMENT* Attachment;

	CurrentSize = 0;
	while(CurrentSize < Size)
	{
		AttachmentSize = DataStream.ReadDWord();
		CurrentSize += AttachmentSize;

		Attachment = new MODEL_ATTACHMENT();
		if(Attachment == NULL)
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
			return FALSE;
		}

		if(!LoadAttachment((*Attachment), DataStream, AttachmentSize))
		{
			delete Attachment;
			return FALSE;
		}

		if(!Model.AddAttachment(Attachment))
		{
			delete Attachment;
			return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model attachment
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadAttachment(MODEL_ATTACHMENT& Attachment, DATA_IN_STREAM& DataStream, INT Size)
{
	DWORD Tag;
	CHAR Buffer[MODEL_NAME_ATTACHMENT_SIZE + 1];

	if(!LoadBaseData(Attachment.BaseData(), DataStream)) return FALSE;

	Buffer[MODEL_NAME_ATTACHMENT_SIZE] = '\0';

	if(!DataStream.Read(Buffer, MODEL_NAME_ATTACHMENT_SIZE)) return FALSE;
	Attachment.Data().Path = Buffer;

	DataStream.ReadDWord(); //What's this? (always 0?)
	Attachment.Data().AttachmentId = DataStream.ReadDWord();

	Size -= (Attachment.GetBaseSize() + 12 + 256);

	while(Size > 0)
	{
		Tag = ReverseDWord(DataStream.ReadDWord());

		if(Tag == 'KATV')
		{
			if(!Attachment.Data().Visibility.Load(DataStream)) return FALSE;
			Size -= Attachment.Data().Visibility.GetSize();
		}
		else
		{
			Error.SetMessage("Unknown tag \"" + GroupToString(Tag) + "\" in Attachment!");
			return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model pivot points
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadPivotPoints(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	INT i;
	INT NrOfPivotPoints;
	D3DXVECTOR3 TempVector;

	NrOfPivotPoints = Size / 12;

	Model.Data().PivotPointContainer.Clear();
	Model.Data().PivotPointContainer.Reserve(NrOfPivotPoints);

	for(i = 0; i < NrOfPivotPoints; i++)
	{
		TempVector.x = DataStream.ReadFloat();
		TempVector.y = DataStream.ReadFloat();
		TempVector.z = DataStream.ReadFloat();

		Model.AddPivotPoint(new D3DXVECTOR3(TempVector));
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model particle emitters
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadParticleEmitters(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	INT CurrentSize;
	INT ParticleEmitterSize;
	MODEL_PARTICLE_EMITTER* ParticleEmitter;

	CurrentSize = 0;
	while(CurrentSize < Size)
	{
		ParticleEmitterSize = DataStream.ReadDWord();
		CurrentSize += ParticleEmitterSize;

		ParticleEmitter = new MODEL_PARTICLE_EMITTER();
		if(ParticleEmitter == NULL)
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
			return FALSE;
		}

		if(!LoadParticleEmitter((*ParticleEmitter), DataStream, ParticleEmitterSize))
		{
			delete ParticleEmitter;
			return FALSE;
		}

		if(!Model.AddParticleEmitter(ParticleEmitter))
		{
			delete ParticleEmitter;
			return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model particle emitter
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadParticleEmitter(MODEL_PARTICLE_EMITTER& ParticleEmitter, DATA_IN_STREAM& DataStream, INT Size)
{
	DWORD Tag;
	DWORD NodeType;
	CHAR Buffer[MODEL_NAME_PARTICLE_EMITTER_SIZE + 1];

	if(!LoadBaseData(ParticleEmitter.BaseData(), DataStream, &NodeType)) return FALSE;

	ParticleEmitter.Data().EmitterUsesMdl = (NodeType & NODE_FLAG_EMITTER_USES_MDL);
	ParticleEmitter.Data().EmitterUsesTga = (NodeType & NODE_FLAG_EMITTER_USES_TGA);

	ParticleEmitter.Data().EmissionRate.SetStaticScalar(DataStream.ReadFloat());
	ParticleEmitter.Data().Gravity.SetStaticScalar(DataStream.ReadFloat());
	ParticleEmitter.Data().Longitude.SetStaticScalar(DataStream.ReadFloat());
	ParticleEmitter.Data().Latitude.SetStaticScalar(DataStream.ReadFloat());

	Buffer[MODEL_NAME_PARTICLE_EMITTER_SIZE] = '\0';

	if(!DataStream.Read(Buffer, MODEL_NAME_PARTICLE_EMITTER_SIZE)) return FALSE;
	ParticleEmitter.Data().ParticleFileName = Buffer;

	DataStream.ReadDWord(); //What's this? (Always 0?)

	ParticleEmitter.Data().ParticleLifeSpan.SetStaticScalar(DataStream.ReadFloat());
	ParticleEmitter.Data().ParticleInitialVelocity.SetStaticScalar(DataStream.ReadFloat());

	Size -= ParticleEmitter.GetBaseSize();
	Size -= 288;

	while(Size > 0)
	{
		Tag = ReverseDWord(DataStream.ReadDWord());

		if(Tag == 'KPEV')
		{
			if(!ParticleEmitter.Data().Visibility.Load(DataStream)) return FALSE;
			Size -= ParticleEmitter.Data().Visibility.GetSize();
		}
		else
		{
			Error.SetMessage("Unknown tag \"" + GroupToString(Tag) + "\" in ParticleEmitter!");
			return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model particle emitters 2
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadParticleEmitters2(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	INT CurrentSize;
	INT ParticleEmitter2Size;
	MODEL_PARTICLE_EMITTER_2* ParticleEmitter2;

	CurrentSize = 0;
	while(CurrentSize < Size)
	{
		ParticleEmitter2Size = DataStream.ReadDWord();
		CurrentSize += ParticleEmitter2Size;

		ParticleEmitter2 = new MODEL_PARTICLE_EMITTER_2();
		if(ParticleEmitter2 == NULL)
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
			return FALSE;
		}

		if(!LoadParticleEmitter2((*ParticleEmitter2), DataStream, ParticleEmitter2Size))
		{
			delete ParticleEmitter2;
			return FALSE;
		}

		if(!Model.AddParticleEmitter2(ParticleEmitter2))
		{
			delete ParticleEmitter2;
			return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model particle emitter 2
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadParticleEmitter2(MODEL_PARTICLE_EMITTER_2& ParticleEmitter2, DATA_IN_STREAM& DataStream, INT Size)
{
	DWORD Tag;
	DWORD Temp;
	DWORD NodeType;
	FLOAT Temp1;
	FLOAT Temp2;
	FLOAT Temp3;

	if(!LoadBaseData(ParticleEmitter2.BaseData(), DataStream, &NodeType)) return FALSE;

	ParticleEmitter2.Data().XYQuad = (NodeType & NODE_FLAG_XY_QUAD);
	ParticleEmitter2.Data().Unshaded = (NodeType & NODE_FLAG_UNSHADED);
	ParticleEmitter2.Data().Unfogged = (NodeType & NODE_FLAG_UNFOGGED);
	ParticleEmitter2.Data().ModelSpace = (NodeType & NODE_FLAG_MODEL_SPACE);
	ParticleEmitter2.Data().LineEmitter = (NodeType & NODE_FLAG_LINE_EMITTER);
	ParticleEmitter2.Data().SortPrimitivesFarZ = (NodeType & NODE_FLAG_SORT_PRIMITIVES_FAR_Z);

	ParticleEmitter2.Data().Speed.SetStaticScalar(DataStream.ReadFloat());
	ParticleEmitter2.Data().Variation.SetStaticScalar(DataStream.ReadFloat());
	ParticleEmitter2.Data().Latitude.SetStaticScalar(DataStream.ReadFloat());
	ParticleEmitter2.Data().Gravity.SetStaticScalar(DataStream.ReadFloat());
	ParticleEmitter2.Data().LifeSpan = DataStream.ReadFloat();
	ParticleEmitter2.Data().EmissionRate.SetStaticScalar(DataStream.ReadFloat());
	ParticleEmitter2.Data().Length.SetStaticScalar(DataStream.ReadFloat());
	ParticleEmitter2.Data().Width.SetStaticScalar(DataStream.ReadFloat());

	switch(DataStream.ReadDWord())
	{
		case 0:
		{
			ParticleEmitter2.Data().FilterMode = FILTER_MODE_BLEND;
			break;
		}

		case 1:
		{
			ParticleEmitter2.Data().FilterMode = FILTER_MODE_ADDITIVE;
			break;
		}

		case 2:
		{
			ParticleEmitter2.Data().FilterMode = FILTER_MODE_MODULATE;
			break;
		}

		case 4:
		{
			ParticleEmitter2.Data().FilterMode = FILTER_MODE_ADD_ALPHA;
			break;
		}

		default:
		{
			Error.SetMessage("Unknown filter mode in ParticleEmitter2!");
			return FALSE;
		}
	}

	ParticleEmitter2.Data().Rows = DataStream.ReadDWord();
	ParticleEmitter2.Data().Columns = DataStream.ReadDWord();

	switch(DataStream.ReadDWord())
	{
		case 0:
		{
			ParticleEmitter2.Data().Head = TRUE;
			break;
		}

		case 1:
		{
			ParticleEmitter2.Data().Tail = TRUE;
			break;
		}

		case 2:
		{
			ParticleEmitter2.Data().Head = TRUE;
			ParticleEmitter2.Data().Tail = TRUE;
			break;
		}
	}

	ParticleEmitter2.Data().TailLength = DataStream.ReadFloat();
	ParticleEmitter2.Data().Time = DataStream.ReadFloat();

	Temp1 = DataStream.ReadFloat();
	Temp2 = DataStream.ReadFloat();
	Temp3 = DataStream.ReadFloat();

	ParticleEmitter2.Data().SegmentColor1 = D3DXVECTOR3(Temp3, Temp2, Temp1);

	Temp1 = DataStream.ReadFloat();
	Temp2 = DataStream.ReadFloat();
	Temp3 = DataStream.ReadFloat();

	ParticleEmitter2.Data().SegmentColor2 = D3DXVECTOR3(Temp3, Temp2, Temp1);

	Temp1 = DataStream.ReadFloat();
	Temp2 = DataStream.ReadFloat();
	Temp3 = DataStream.ReadFloat();

	ParticleEmitter2.Data().SegmentColor3 = D3DXVECTOR3(Temp3, Temp2, Temp1);

	Temp1 = static_cast<FLOAT>(DataStream.ReadByte());
	Temp2 = static_cast<FLOAT>(DataStream.ReadByte());
	Temp3 = static_cast<FLOAT>(DataStream.ReadByte());

	ParticleEmitter2.Data().Alpha = D3DXVECTOR3(Temp1, Temp2, Temp3);

	Temp1 = DataStream.ReadFloat();
	Temp2 = DataStream.ReadFloat();
	Temp3 = DataStream.ReadFloat();

	ParticleEmitter2.Data().ParticleScaling = D3DXVECTOR3(Temp1, Temp2, Temp3);

	Temp1 = static_cast<FLOAT>(DataStream.ReadDWord());
	Temp2 = static_cast<FLOAT>(DataStream.ReadDWord());
	Temp3 = static_cast<FLOAT>(DataStream.ReadDWord());

	ParticleEmitter2.Data().HeadLifeSpan = D3DXVECTOR3(Temp1, Temp2, Temp3);

	Temp1 = static_cast<FLOAT>(DataStream.ReadDWord());
	Temp2 = static_cast<FLOAT>(DataStream.ReadDWord());
	Temp3 = static_cast<FLOAT>(DataStream.ReadDWord());

	ParticleEmitter2.Data().HeadDecay = D3DXVECTOR3(Temp1, Temp2, Temp3);

	Temp1 = static_cast<FLOAT>(DataStream.ReadDWord());
	Temp2 = static_cast<FLOAT>(DataStream.ReadDWord());
	Temp3 = static_cast<FLOAT>(DataStream.ReadDWord());

	ParticleEmitter2.Data().TailLifeSpan = D3DXVECTOR3(Temp1, Temp2, Temp3);

	Temp1 = static_cast<FLOAT>(DataStream.ReadDWord());
	Temp2 = static_cast<FLOAT>(DataStream.ReadDWord());
	Temp3 = static_cast<FLOAT>(DataStream.ReadDWord());

	ParticleEmitter2.Data().TailDecay = D3DXVECTOR3(Temp1, Temp2, Temp3);

	Temp = DataStream.ReadDWord();
	ParticleEmitter2.Data().TextureId = (Temp == 0xFFFFFFFF) ? INVALID_INDEX : Temp;

	Temp = DataStream.ReadDWord();
	ParticleEmitter2.Data().Squirt = (Temp != 0);

	ParticleEmitter2.Data().PriorityPlane = DataStream.ReadDWord();
	ParticleEmitter2.Data().ReplaceableId = DataStream.ReadDWord();

	Size -= ParticleEmitter2.GetBaseSize();
	Size -= 175;

	while(Size > 0)
	{
		Tag = ReverseDWord(DataStream.ReadDWord());

		if(Tag == 'KP2V')
		{
			if(!ParticleEmitter2.Data().Visibility.Load(DataStream)) return FALSE;
			Size -= ParticleEmitter2.Data().Visibility.GetSize();
		}
		else if(Tag == 'KP2E')
		{
			if(!ParticleEmitter2.Data().EmissionRate.Load(DataStream)) return FALSE;
			Size -= ParticleEmitter2.Data().EmissionRate.GetSize();
		}
		else if(Tag == 'KP2W')
		{
			if(!ParticleEmitter2.Data().Width.Load(DataStream)) return FALSE;
			Size -= ParticleEmitter2.Data().Width.GetSize();
		}
		else if(Tag == 'KP2N')
		{
			if(!ParticleEmitter2.Data().Length.Load(DataStream)) return FALSE;
			Size -= ParticleEmitter2.Data().Length.GetSize();
		}
		else if(Tag == 'KP2S')
		{
			if(!ParticleEmitter2.Data().Speed.Load(DataStream)) return FALSE;
			Size -= ParticleEmitter2.Data().Speed.GetSize();
		}
		else if(Tag == 'KP2L')
		{
			if(!ParticleEmitter2.Data().Latitude.Load(DataStream)) return FALSE;
			Size -= ParticleEmitter2.Data().Latitude.GetSize();
		}
		else
		{
			Error.SetMessage("Unknown tag \"" + GroupToString(Tag) + "\" in ParticleEmitter2!");
			return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model ribbon emitters
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadRibbonEmitters(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	INT CurrentSize;
	INT RibbonEmitterSize;
	MODEL_RIBBON_EMITTER* RibbonEmitter;

	CurrentSize = 0;
	while(CurrentSize < Size)
	{
		RibbonEmitterSize = DataStream.ReadDWord();
		CurrentSize += RibbonEmitterSize;

		RibbonEmitter = new MODEL_RIBBON_EMITTER();
		if(RibbonEmitter == NULL)
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
			return FALSE;
		}

		if(!LoadRibbonEmitter((*RibbonEmitter), DataStream, RibbonEmitterSize))
		{
			delete RibbonEmitter;
			return FALSE;
		}

		if(!Model.AddRibbonEmitter(RibbonEmitter))
		{
			delete RibbonEmitter;
			return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model ribbon emitter
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadRibbonEmitter(MODEL_RIBBON_EMITTER& RibbonEmitter, DATA_IN_STREAM& DataStream, INT Size)
{
	DWORD Tag;
	DWORD Temp;
	FLOAT Temp1;
	FLOAT Temp2;
	FLOAT Temp3;

	if(!LoadBaseData(RibbonEmitter.BaseData(), DataStream)) return FALSE;

	RibbonEmitter.Data().HeightAbove.SetStaticScalar(DataStream.ReadFloat());
	RibbonEmitter.Data().HeightBelow.SetStaticScalar(DataStream.ReadFloat());
	RibbonEmitter.Data().Alpha.SetStaticScalar(DataStream.ReadFloat());

	Temp1 = DataStream.ReadFloat();
	Temp2 = DataStream.ReadFloat();
	Temp3 = DataStream.ReadFloat();

	RibbonEmitter.Data().Color.SetStaticVector3(D3DXVECTOR3(Temp3, Temp2, Temp1));

	RibbonEmitter.Data().LifeSpan = DataStream.ReadFloat();

	DataStream.ReadDWord(); //What's this? (Always 0?)

	RibbonEmitter.Data().EmissionRate = static_cast<FLOAT>(DataStream.ReadDWord());
	RibbonEmitter.Data().Rows = DataStream.ReadDWord();
	RibbonEmitter.Data().Columns = DataStream.ReadDWord();

	Temp = DataStream.ReadDWord();
	RibbonEmitter.Data().MaterialId = (Temp == 0xFFFFFFFF) ? INVALID_INDEX : Temp;

	RibbonEmitter.Data().Gravity = DataStream.ReadFloat();

	Size -= RibbonEmitter.GetBaseSize();
	Size -= 56;

	while(Size > 0)
	{
		Tag = ReverseDWord(DataStream.ReadDWord());

		if(Tag == 'KRVS')
		{
			if(!RibbonEmitter.Data().Visibility.Load(DataStream)) return FALSE;
			Size -= RibbonEmitter.Data().Visibility.GetSize();
		}
		else if(Tag == 'KRHA')
		{
			if(!RibbonEmitter.Data().HeightAbove.Load(DataStream)) return FALSE;
			Size -= RibbonEmitter.Data().HeightAbove.GetSize();
		}
		else if(Tag == 'KRHB')
		{
			if(!RibbonEmitter.Data().HeightBelow.Load(DataStream)) return FALSE;
			Size -= RibbonEmitter.Data().HeightBelow.GetSize();
		}
		else
		{
			Error.SetMessage("Unknown tag \"" + GroupToString(Tag) + "\" in RibbonEmitter!");
			return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model event object
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadEventObjects(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	INT CurrentSize;
	INT EventObjectSize;
	MODEL_EVENT_OBJECT* EventObject;

	CurrentSize = 0;
	while(CurrentSize < Size)
	{
		EventObject = new MODEL_EVENT_OBJECT();
		if(EventObject == NULL)
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
			return FALSE;
		}

		if(!LoadEventObject((*EventObject), DataStream, 0))
		{
			delete EventObject;
			return FALSE;
		}

		if(!Model.AddEventObject(EventObject))
		{
			delete EventObject;
			return FALSE;
		}

		EventObjectSize = EventObject->GetSize();
		CurrentSize += EventObjectSize;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model event object
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadEventObject(MODEL_EVENT_OBJECT& EventObject, DATA_IN_STREAM& DataStream, INT Size)
{
	INT i;
	DWORD Temp;
	INT NrOfEventTracks;

	if(!LoadBaseData(EventObject.BaseData(), DataStream)) return FALSE;

	if(!ExpectTag(DataStream, 'KEVT')) return FALSE;

	NrOfEventTracks = DataStream.ReadDWord();

	Temp = DataStream.ReadDWord();
	EventObject.Data().GlobalSequenceId = (Temp == 0xFFFFFFFF) ? INVALID_INDEX : Temp;

	for(i = 0; i < NrOfEventTracks; i++)
	{
		EventObject.Data().EventTrack.push_back(DataStream.ReadDWord());
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model cameras
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadCameras(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	INT CurrentSize;
	INT CameraSize;
	MODEL_CAMERA* Camera;

	CurrentSize = 0;
	while(CurrentSize < Size)
	{
		CameraSize = DataStream.ReadDWord();
		CurrentSize += CameraSize;

		Camera = new MODEL_CAMERA();
		if(Camera == NULL)
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
			return FALSE;
		}

		if(!LoadCamera((*Camera), DataStream, CameraSize))
		{
			delete Camera;
			return FALSE;
		}

		if(!Model.AddCamera(Camera))
		{
			delete Camera;
			return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model camera
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadCamera(MODEL_CAMERA& Camera, DATA_IN_STREAM& DataStream, INT Size)
{
	DWORD Tag;
	CHAR Buffer[MODEL_NAME_CAMERA_SIZE + 1];

	Buffer[MODEL_NAME_CAMERA_SIZE] = '\0';

	if(!DataStream.Read(Buffer, MODEL_NAME_CAMERA_SIZE)) return FALSE;
	Camera.Data().Name = Buffer;

	Camera.Data().Source.x = DataStream.ReadFloat();
	Camera.Data().Source.y = DataStream.ReadFloat();
	Camera.Data().Source.z = DataStream.ReadFloat();

	Camera.Data().FieldOfView = DataStream.ReadFloat();
	Camera.Data().FarDistance = DataStream.ReadFloat();
	Camera.Data().NearDistance = DataStream.ReadFloat();

	Camera.Data().Target.x = DataStream.ReadFloat();
	Camera.Data().Target.y = DataStream.ReadFloat();
	Camera.Data().Target.z = DataStream.ReadFloat();

	Size -= 120;

	while(Size > 0)
	{
		Tag = ReverseDWord(DataStream.ReadDWord());

		if(Tag == 'KCTR')
		{
			if(!Camera.Data().SourceTranslation.Load(DataStream)) return FALSE;
			Size -= Camera.Data().SourceTranslation.GetSize();
		}
		else if(Tag == 'KTTR')
		{
			if(!Camera.Data().TargetTranslation.Load(DataStream)) return FALSE;
			Size -= Camera.Data().TargetTranslation.GetSize();
		}
		else if(Tag == 'KCRL')
		{
			if(!Camera.Data().Rotation.Load(DataStream)) return FALSE;
			Size -= Camera.Data().Rotation.GetSize();
		}
		else
		{
			Error.SetMessage("Unknown tag \"" + GroupToString(Tag) + "\" in Camera!");
			return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model collision shapes
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadCollisionShapes(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size)
{
	INT CurrentSize;
	INT CollisionShapeSize;
	MODEL_COLLISION_SHAPE* CollisionShape;

	CurrentSize = 0;
	while(CurrentSize < Size)
	{
		CollisionShape = new MODEL_COLLISION_SHAPE();
		if(CollisionShape == NULL)
		{
			Error.SetMessage("Unable to load \"" + CurrentFileName + "\", memory allocation failed!");
			return FALSE;
		}

		if(!LoadCollisionShape((*CollisionShape), DataStream, 0))
		{
			delete CollisionShape;
			return FALSE;
		}

		if(!Model.AddCollisionShape(CollisionShape))
		{
			delete CollisionShape;
			return FALSE;
		}

		CollisionShapeSize = CollisionShape->GetSize();
		CurrentSize += CollisionShapeSize;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a model collision shape
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadCollisionShape(MODEL_COLLISION_SHAPE& CollisionShape, DATA_IN_STREAM& DataStream, INT Size)
{
	INT i;
	INT NrOfVertices;
	D3DXVECTOR3 TempVector;

	if(!LoadBaseData(CollisionShape.BaseData(), DataStream)) return FALSE;

	switch(DataStream.ReadDWord())
	{
		case 0:
		{
			NrOfVertices = 2;
			CollisionShape.Data().Type = COLLISION_SHAPE_TYPE_BOX;
			break;
		}

		case 2:
		{
			NrOfVertices = 1;
			CollisionShape.Data().Type = COLLISION_SHAPE_TYPE_SPHERE;
			break;
		}

		default:
		{
			Error.SetMessage("Unknown collision shape type!");
			return FALSE;
		}
	}

	CollisionShape.Data().VertexList.clear();
	for(i = 0; i < NrOfVertices; i++)
	{
		TempVector.x = DataStream.ReadFloat();
		TempVector.y = DataStream.ReadFloat();
		TempVector.z = DataStream.ReadFloat();

		CollisionShape.Data().VertexList.push_back(TempVector);
	}

	if(CollisionShape.Data().Type == COLLISION_SHAPE_TYPE_SPHERE)
	{
		CollisionShape.Data().BoundsRadius = DataStream.ReadFloat();
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the base data
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SaveBaseData(MODEL_BASE_DATA* BaseData, DATA_OUT_STREAM& DataStream, DWORD NodeType)
{
	DWORD Flags;
	INT TotalSize;
	INT TranslationSize;
	INT RotationSize;
	INT ScalingSize;

	TranslationSize = BaseData->Translation.GetSize();
	RotationSize = BaseData->Rotation.GetSize();
	ScalingSize = BaseData->Scaling.GetSize();
	TotalSize = 96 + TranslationSize + RotationSize + ScalingSize;

	Flags = NodeType;
	if(BaseData->DontInheritTranslation) Flags |= NODE_FLAG_DONT_INHERIT_TRANSLATION;
	if(BaseData->DontInheritRotation) Flags |= NODE_FLAG_DONT_INHERIT_ROTATION;
	if(BaseData->DontInheritScaling) Flags |= NODE_FLAG_DONT_INHERIT_SCALING;
	if(BaseData->Billboarded) Flags |= NODE_FLAG_BILLBOARDED;
	if(BaseData->BillboardedLockX) Flags |= NODE_FLAG_BILLBOARDED_LOCK_X;
	if(BaseData->BillboardedLockY) Flags |= NODE_FLAG_BILLBOARDED_LOCK_Y;
	if(BaseData->BillboardedLockZ) Flags |= NODE_FLAG_BILLBOARDED_LOCK_Z;
	if(BaseData->CameraAnchored) Flags |= NODE_FLAG_CAMERA_ANCHORED;

	DataStream.WriteDWord(TotalSize);
	DataStream.WriteString(BaseData->Name, MODEL_NAME_NODE_SIZE);
	DataStream.WriteDWord((BaseData->ObjectId == INVALID_INDEX) ? 0xFFFFFFFF : BaseData->ObjectId);
	DataStream.WriteDWord((BaseData->ParentId == INVALID_INDEX) ? 0xFFFFFFFF : BaseData->ParentId);
	DataStream.WriteDWord(Flags);

	if(BaseData->Translation.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KGTR'));
		if(!BaseData->Translation.Save(DataStream)) return FALSE;
	}

	if(BaseData->Rotation.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KGRT'));
		if(!BaseData->Rotation.Save(DataStream)) return FALSE;
	}

	if(BaseData->Scaling.GetNrOfNodes() > 0)
	{
		DataStream.WriteDWord(ReverseDWord('KGSC'));
		if(!BaseData->Scaling.Save(DataStream)) return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the base data
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::LoadBaseData(MODEL_BASE_DATA* BaseData, DATA_IN_STREAM& DataStream, DWORD* NodeType)
{
	INT Size;
	DWORD Tag;
	DWORD Temp;
	DWORD Flags;
	CHAR Buffer[MODEL_NAME_NODE_SIZE + 1];

	Size = DataStream.ReadDWord();

	Buffer[MODEL_NAME_NODE_SIZE] = '\0';

	if(!DataStream.Read(Buffer, MODEL_NAME_NODE_SIZE)) return FALSE;
	BaseData->Name = Buffer;

	Temp = DataStream.ReadDWord();
	BaseData->ObjectId = (Temp == 0xFFFFFFFF) ? INVALID_INDEX : Temp;

	Temp = DataStream.ReadDWord();
	BaseData->ParentId = (Temp == 0xFFFFFFFF) ? INVALID_INDEX : Temp;

	Flags = DataStream.ReadDWord();
	if(NodeType) (*NodeType) = Flags;

	BaseData->DontInheritTranslation = (Flags & NODE_FLAG_DONT_INHERIT_TRANSLATION);
	BaseData->DontInheritRotation = (Flags & NODE_FLAG_DONT_INHERIT_ROTATION);
	BaseData->DontInheritScaling = (Flags & NODE_FLAG_DONT_INHERIT_SCALING);
	BaseData->Billboarded = (Flags & NODE_FLAG_BILLBOARDED);
	BaseData->BillboardedLockX = (Flags & NODE_FLAG_BILLBOARDED_LOCK_X);
	BaseData->BillboardedLockY = (Flags & NODE_FLAG_BILLBOARDED_LOCK_Y);
	BaseData->BillboardedLockZ = (Flags & NODE_FLAG_BILLBOARDED_LOCK_Z);
	BaseData->CameraAnchored = (Flags & NODE_FLAG_CAMERA_ANCHORED);

	Size -= 96;

	while(Size > 0)
	{
		Tag = ReverseDWord(DataStream.ReadDWord());

		if(Tag == 'KGTR')
		{
			if(!BaseData->Translation.Load(DataStream)) return FALSE;
			Size -= BaseData->Translation.GetSize();
		}
		else if(Tag == 'KGRT')
		{
			if(!BaseData->Rotation.Load(DataStream)) return FALSE;
			Size -= BaseData->Rotation.GetSize();
		}
		else if(Tag == 'KGSC')
		{
			if(!BaseData->Scaling.Load(DataStream)) return FALSE;
			Size -= BaseData->Scaling.GetSize();
		}
		else
		{
			Error.SetMessage("Unknown tag \"" + GroupToString(Tag) + "\" in Node!");
			return FALSE;
		}
	}

	ObjectIdManager.AddObjectId(BaseData->ObjectId);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Expects a certain tag
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::ExpectTag(DATA_IN_STREAM& DataStream, DWORD Tag)
{
	DWORD ReceivedTag;

	ReceivedTag = ReverseDWord(DataStream.ReadDWord());
	if(ReceivedTag == Tag) return TRUE;

	Error.SetMessage("Expected a \"" + GroupToString(Tag) + "\" tag, got a \"" + GroupToString(ReceivedTag) + "\" tag!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Skips a model group
//+-----------------------------------------------------------------------------
BOOL MODEL_LOADER_MDX::SkipGroup(DATA_IN_STREAM& DataStream, INT Size)
{
	INT Position;

	Position = DataStream.GetPosition();
	DataStream.SetPosition(Position + Size);

	return TRUE;
}
