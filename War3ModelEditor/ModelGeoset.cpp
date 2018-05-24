//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ModelGeoset.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_GEOSET::MODEL_GEOSET()
{
	MeshBuildt = TRUE;

	GeosetAlpha = 1.0f;
	GeosetColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	MaterialNode.SetData(this);
	BoneNodes.SetData(this);
	GeosetAnimationNodes.SetData(this);
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL_GEOSET::~MODEL_GEOSET()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Clears the geoset
//+-----------------------------------------------------------------------------
VOID MODEL_GEOSET::Clear()
{
	INT i;

	for(i = 0; i < GeosetData.GroupContainer.GetTotalSize(); i++)
	{
		if(GeosetData.GroupContainer.ValidIndex(i))
		{
			GeosetData.GroupContainer[i]->Clear();
		}
	}

	SAFE_CLEAR(GeosetData.VertexContainer);
	SAFE_CLEAR(GeosetData.FaceContainer);
	SAFE_CLEAR(GeosetData.GroupContainer);
	SAFE_CLEAR(GeosetData.ExtentContainer);



	MeshBuildt = TRUE;

	GeosetAlpha = 1.0f;
	GeosetColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	GeosetData = MODEL_GEOSET_DATA();
}


//+-----------------------------------------------------------------------------
//| Returns the mdx size of the geoset
//+-----------------------------------------------------------------------------
INT MODEL_GEOSET::GetSize()
{
	INT i;
	INT Size = 136;

	Size += (GeosetData.VertexContainer.GetSize() * 33);
	Size += (GeosetData.FaceContainer.GetSize() * 6);
	Size += (GeosetData.GroupContainer.GetSize() * 4);
	Size += (GeosetData.ExtentContainer.GetSize() * 28);

	for(i = 0; i < GeosetData.GroupContainer.GetTotalSize(); i++)
	{
		if(GeosetData.GroupContainer.ValidIndex(i))
		{
			Size += (GeosetData.GroupContainer[i]->MatrixListSize * 4);
		}
	}

	return Size;
}


//+-----------------------------------------------------------------------------
//| Returns a reference to the data
//+-----------------------------------------------------------------------------
MODEL_GEOSET_DATA& MODEL_GEOSET::Data()
{
	return GeosetData;
}


//+-----------------------------------------------------------------------------
//| Returns the render order
//+-----------------------------------------------------------------------------
INT MODEL_GEOSET::GetRenderOrder()
{
	if(!MaterialNode.IsAttached()) return 0;

	return MaterialNode.GetObjectData()->GetRenderOrder();
}


//+-----------------------------------------------------------------------------
//| Returns a reference to the alpha
//+-----------------------------------------------------------------------------
FLOAT& MODEL_GEOSET::Alpha()
{
	return GeosetAlpha;
}


//+-----------------------------------------------------------------------------
//| Returns a reference to the color
//+-----------------------------------------------------------------------------
D3DXCOLOR& MODEL_GEOSET::Color()
{
	return GeosetColor;
}


//+-----------------------------------------------------------------------------
//| Rebuilds the geoset the next time it's rendered
//+-----------------------------------------------------------------------------
VOID MODEL_GEOSET::Rebuild()
{
	MeshBuildt = FALSE;
}


//+-----------------------------------------------------------------------------
//| Renders the geoset
//+-----------------------------------------------------------------------------
VOID MODEL_GEOSET::Render(CONST SEQUENCE_TIME& Time, BOOL Animated)
{
	INT i;
	D3DXMATRIX Matrix;
	MESH* CurrentMesh;
	MODEL_MATERIAL* Material;
	MODEL_MATERIAL_LAYER* Layer;
	MODEL_TEXTURE_ANIMATION* TextureAnimation;

	D3DXMatrixIdentity(&Matrix);
	Graphics.SetWorldMatrix(Matrix);

	if(FlashingGeoset.IsFlashing()) MeshBuildt = FALSE;

	BuildMesh();

	if(Animated)
	{
		BuildAnimatedMesh(Time);
		CurrentMesh = &AnimatedMesh;

		if(GeosetAlpha <= 0.01f) return;
	}
	else
	{
		CurrentMesh = &Mesh;
	}

	if(!MaterialNode.IsAttached()) return;
	Material = MaterialNode.GetObjectData();

	for(i = 0; i < Material->Data().LayerContainer.GetTotalSize(); i++)
	{
		if(!Material->Data().LayerContainer.ValidIndex(i)) continue;
		Layer = Material->Data().LayerContainer[i];

		TextureAnimation = Layer->TextureAnimationNode.IsAttached() ? Layer->TextureAnimationNode.GetObjectData() : NULL;

		BuildAnimatedTexture(Time, TextureAnimation, Layer->Data().Alpha.GetScalar(Time));
		Layer->UseMaterial(Time);
		CurrentMesh->Render();
	}
}


//+-----------------------------------------------------------------------------
//| Renders the geoset in a view
//+-----------------------------------------------------------------------------
VOID MODEL_GEOSET::RenderInView()
{
	Mesh.Render();
}


//+-----------------------------------------------------------------------------
//| Builds a mesh from the vertex and face information
//+-----------------------------------------------------------------------------
BOOL MODEL_GEOSET::BuildMesh()
{
	INT i;
	INT NrOfVertices;
	INT NrOfFaces;
	VERTEX* VertexPointer;
	VERTEX* AnimatedVertexPointer;
	WORD* IndexPointer;
	WORD* AnimatedIndexPointer;
	D3DCOLOR Color;

	if(MeshBuildt) return TRUE;

	NrOfVertices = GeosetData.VertexContainer.GetSize();
	if(NrOfVertices <= 0) return FALSE;

	NrOfFaces = GeosetData.FaceContainer.GetSize();
	if(NrOfFaces <= 0) return FALSE;

	if(!Mesh.Create(NrOfVertices, NrOfFaces)) return FALSE;
	if(!AnimatedMesh.Create(NrOfVertices, NrOfFaces)) return FALSE;

	if(!Mesh.LockVertexBuffer(&VertexPointer)) return FALSE;
	if(!AnimatedMesh.LockVertexBuffer(&AnimatedVertexPointer))
	{
		Mesh.UnlockVertexBuffer();
		return FALSE;
	}

	if(FlashingGeoset.GetGeoset() == this)
	{
		D3DXCOLOR TempColor;

		TempColor = FlashingGeoset.GetColor();
		Color = D3DCOLOR_COLORVALUE(TempColor.r, TempColor.g, TempColor.b, TempColor.a);
	}
	else
	{
		Color = 0xFFFFFFFF;
	}

	for(i = 0; i < GeosetData.VertexContainer.GetTotalSize(); i++)
	{
		if(GeosetData.VertexContainer.ValidIndex(i))
		{
			VertexPointer->Position = GeosetData.VertexContainer[i]->Position;
			VertexPointer->Normal = GeosetData.VertexContainer[i]->Normal;
			VertexPointer->Color = Color;
			VertexPointer->TexturePosition = GeosetData.VertexContainer[i]->TexturePosition;

			AnimatedVertexPointer->Position = VertexPointer->Position;
			AnimatedVertexPointer->Normal = VertexPointer->Normal;
			AnimatedVertexPointer->Color = VertexPointer->Color;
			AnimatedVertexPointer->TexturePosition = VertexPointer->TexturePosition;

			VertexPointer++;
			AnimatedVertexPointer++;
		}
	}

	AnimatedMesh.UnlockVertexBuffer();
	Mesh.UnlockVertexBuffer();

	if(!Mesh.LockIndexBuffer(&IndexPointer)) return FALSE;
	if(!AnimatedMesh.LockIndexBuffer(&AnimatedIndexPointer))
	{
		Mesh.UnlockIndexBuffer();
		return FALSE;
	}

	for(i = 0; i < GeosetData.FaceContainer.GetTotalSize(); i++)
	{
		if(GeosetData.FaceContainer.ValidIndex(i))
		{
			(*IndexPointer) = GeosetData.FaceContainer[i]->Index1;
			(*AnimatedIndexPointer) = (*IndexPointer);
			IndexPointer++;
			AnimatedIndexPointer++;

			(*IndexPointer) = GeosetData.FaceContainer[i]->Index2;
			(*AnimatedIndexPointer) = (*IndexPointer);
			IndexPointer++;
			AnimatedIndexPointer++;

			(*IndexPointer) = GeosetData.FaceContainer[i]->Index3;
			(*AnimatedIndexPointer) = (*IndexPointer);
			IndexPointer++;
			AnimatedIndexPointer++;
		}
	}

	AnimatedMesh.UnlockIndexBuffer();
	Mesh.UnlockIndexBuffer();

	MeshBuildt = TRUE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Builds an animated mesh from the mesh and group information
//+-----------------------------------------------------------------------------
BOOL MODEL_GEOSET::BuildAnimatedMesh(CONST SEQUENCE_TIME& Time)
{
	INT i;
	INT j;
	INT GroupIndex;
	FLOAT TempAlpha;
	D3DXVECTOR3 TempColor;
	VERTEX* VertexPointer;
	VERTEX* AnimatedVertexPointer;
	MODEL_BASE* Node;
	MODEL_GEOSET_GROUP* GeosetGroup;
	MODEL_GEOSET_GROUP_NODE* GeosetGroupNode;
	REFERENCE<MODEL_GEOSET_ANIMATION*, MODEL_GEOSET*>* CurrentReference;

	if(FlashingGeoset.GetGeoset() == this)
	{
		GeosetAlpha = 1.0f;
		GeosetColor = FlashingGeoset.GetColor();
	}
	else
	{
		GeosetAlpha = 1.0f;
		GeosetColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	CurrentReference = GeosetAnimationNodes.GetFirstReference();
	while(CurrentReference != NULL)
	{
		TempAlpha = CurrentReference->GetData()->Data().Alpha.GetScalar(Time);
		GeosetAlpha *= TempAlpha;

		if(CurrentReference->GetData()->Data().UseColor)
		{
			TempColor = CurrentReference->GetData()->Data().Color.GetVector3(Time);
			GeosetColor.r *= TempColor.z;
			GeosetColor.g *= TempColor.y;
			GeosetColor.b *= TempColor.x;
			GeosetColor.a *= TempAlpha;
		}

		CurrentReference = GeosetAnimationNodes.GetNextReference(CurrentReference);
	}

	if(!Mesh.LockVertexBuffer(&VertexPointer)) return FALSE;
	if(!AnimatedMesh.LockVertexBuffer(&AnimatedVertexPointer))
	{
		Mesh.UnlockVertexBuffer();
		return FALSE;
	}

	for(i = 0; i < GeosetData.VertexContainer.GetTotalSize(); i++)
	{
		if(!GeosetData.VertexContainer.ValidIndex(i)) continue;

		GroupIndex = GeosetData.VertexContainer[i]->VertexGroup;
		if(GeosetData.GroupContainer.ValidIndex(GroupIndex))
		{
			D3DXVECTOR3 TempVector;
			D3DXVECTOR4 TempVector2;
			D3DXVECTOR3 TempNormalTarget;
			D3DXVECTOR4 TempNormalTarget2;
			D3DXVECTOR3 NormalTarget;
			D3DXVECTOR3 TransformedNormalTarget;
			INT MatrixListSize;
			FLOAT Scale;

			GeosetGroup = GeosetData.GroupContainer[GroupIndex];
			MatrixListSize = GeosetGroup->MatrixList.GetTotalSize();
			AnimatedVertexPointer->Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			TransformedNormalTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			for(j = 0; j < MatrixListSize; j++)
			{
				if(!GeosetGroup->MatrixList.ValidIndex(j)) continue;

				GeosetGroupNode = GeosetGroup->MatrixList[j];
				if(!GeosetGroupNode->Node.IsAttached())
				{
					AnimatedMesh.UnlockVertexBuffer();
					Mesh.UnlockVertexBuffer();
					return FALSE;
				}

				Node = GeosetGroupNode->Node.GetObjectData();

				D3DXVec3Transform(&TempVector2, &(VertexPointer->Position), &(Node->Matrix()));
				D3DXVec3Add(&NormalTarget, &VertexPointer->Position, &VertexPointer->Normal);
				D3DXVec3Transform(&TempNormalTarget2, &NormalTarget, &(Node->Matrix()));

				TempVector.x = TempVector2.x;
				TempVector.y = TempVector2.y;
				TempVector.z = TempVector2.z;

				TempNormalTarget.x = TempNormalTarget2.x;
				TempNormalTarget.y = TempNormalTarget2.y;
				TempNormalTarget.z = TempNormalTarget2.z;

				D3DXVec3Add(&(AnimatedVertexPointer->Position), &(AnimatedVertexPointer->Position), &TempVector);
				D3DXVec3Add(&TransformedNormalTarget, &TransformedNormalTarget, &TempNormalTarget);
			}

			Scale = (MatrixListSize == 0) ? 0.0f : (1.0f / static_cast<FLOAT>(MatrixListSize));
			D3DXVec3Scale(&(AnimatedVertexPointer->Position), &(AnimatedVertexPointer->Position), Scale);
			D3DXVec3Scale(&TransformedNormalTarget, &TransformedNormalTarget, Scale);

			AnimatedVertexPointer->Normal = TransformedNormalTarget - AnimatedVertexPointer->Position;
		}

		VertexPointer++;
		AnimatedVertexPointer++;
	}

	AnimatedMesh.UnlockVertexBuffer();
	Mesh.UnlockVertexBuffer();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Builds an animated texture
//+-----------------------------------------------------------------------------
BOOL MODEL_GEOSET::BuildAnimatedTexture(CONST SEQUENCE_TIME& Time, MODEL_TEXTURE_ANIMATION* TextureAnimation, FLOAT Alpha)
{
	INT i;
	INT Size;
	D3DCOLOR RealColor;
	D3DXVECTOR3 Translation;
	VERTEX* VertexPointer;
	VERTEX* AnimatedVertexPointer;

	RealColor = D3DCOLOR_RGBA(static_cast<BYTE>(GeosetColor.r * 255.0f), static_cast<BYTE>(GeosetColor.g * 255.0f), static_cast<BYTE>(GeosetColor.b * 255.0f), static_cast<BYTE>((GeosetColor.a * Alpha) * 255.0f));
	Size = GeosetData.VertexContainer.GetSize();

	if(!Mesh.LockVertexBuffer(&VertexPointer)) return FALSE;
	if(!AnimatedMesh.LockVertexBuffer(&AnimatedVertexPointer))
	{
		Mesh.UnlockVertexBuffer();
		return FALSE;
	}

	for(i = 0; i < Size; i++)
	{
		if(TextureAnimation == NULL)
		{
			AnimatedVertexPointer->TexturePosition = VertexPointer->TexturePosition;
		}
		else
		{
			SEQUENCE_TIME TempTime;

			if(TextureAnimation->Data().Translation.GlobalSequenceNode.IsAttached())
			{
				MODEL_GLOBAL_SEQUENCE* GlobalSequence;

				GlobalSequence = TextureAnimation->Data().Translation.GlobalSequenceNode.GetObjectData();

				TempTime.Time = GlobalSequence->Data().Time;
				TempTime.IntervalStart = 0;
				TempTime.IntervalEnd = GlobalSequence->Data().Duration;
			}
			else
			{
				TempTime = Time;
			}

			Translation = TextureAnimation->Data().Translation.GetVector3(TempTime);

			AnimatedVertexPointer->TexturePosition = VertexPointer->TexturePosition;
			AnimatedVertexPointer->TexturePosition.x += Translation.x;
			AnimatedVertexPointer->TexturePosition.y += Translation.y;
		}

		AnimatedVertexPointer->Color = RealColor;

		VertexPointer++;
		AnimatedVertexPointer++;
	}

	AnimatedMesh.UnlockVertexBuffer();
	Mesh.UnlockVertexBuffer();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a new vertex
//+-----------------------------------------------------------------------------
BOOL MODEL_GEOSET::AddVertex(MODEL_GEOSET_VERTEX* Vertex)
{
	if(!GeosetData.VertexContainer.Add(Vertex))
	{
		Error.SetMessage("Unable to add a new vertex!");
		return FALSE;
	}

	MeshBuildt = FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a new face
//+-----------------------------------------------------------------------------
BOOL MODEL_GEOSET::AddFace(MODEL_GEOSET_FACE* Face)
{
	if(!GeosetData.FaceContainer.Add(Face))
	{
		Error.SetMessage("Unable to add a new face!");
		return FALSE;
	}

	MeshBuildt = FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a new group
//+-----------------------------------------------------------------------------
BOOL MODEL_GEOSET::AddGroup(MODEL_GEOSET_GROUP* Group)
{
	if(!GeosetData.GroupContainer.Add(Group))
	{
		Error.SetMessage("Unable to add a new group!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a new extent
//+-----------------------------------------------------------------------------
BOOL MODEL_GEOSET::AddExtent(EXTENT* Extent)
{
	if(!GeosetData.ExtentContainer.Add(Extent))
	{
		Error.SetMessage("Unable to add a new extent!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Calculates the extent of the geoset
//+-----------------------------------------------------------------------------
VOID MODEL_GEOSET::CalculateExtent(EXTENT& Extent)
{
	INT i;
	INT NrOfSequences;
	D3DXVECTOR3 Center;
	std::vector<D3DXVECTOR3> VertexList;

	NrOfSequences = Model.Data().SequenceContainer.GetSize();
	VertexList.reserve(GeosetData.VertexContainer.GetSize());

	for(i = 0; i < GeosetData.VertexContainer.GetTotalSize(); i++)
	{
		if(GeosetData.VertexContainer.ValidIndex(i))
		{
			VertexList.push_back(GeosetData.VertexContainer[i]->Position);
		}
	}

	if(static_cast<INT>(VertexList.size()) <= 0)
	{
		Extent = EXTENT();
	}
	else
	{
		D3DXComputeBoundingBox(&VertexList[0], static_cast<INT>(VertexList.size()), sizeof(D3DXVECTOR3), &Extent.Min, &Extent.Max);
		D3DXComputeBoundingSphere(&VertexList[0], static_cast<INT>(VertexList.size()), sizeof(D3DXVECTOR3), &Center, &Extent.Radius);
	}

	GeosetData.Extent = Extent;
	SAFE_CLEAR(GeosetData.ExtentContainer);

	for(i = 0; i < NrOfSequences; i++)
	{
		AddExtent(new EXTENT(GeosetData.Extent));
	}
}


//+-----------------------------------------------------------------------------
//| Checks if a ray intersects the geoset
//+-----------------------------------------------------------------------------
BOOL MODEL_GEOSET::RayIntersects(CONST D3DXVECTOR3& RayPosition, CONST D3DXVECTOR3& RayDirection, BOOL Animated, FLOAT* Distance)
{
	if(GeosetAlpha <= 0.01f) return FALSE;

	if(Animated)
	{
		return AnimatedMesh.RayIntersects(RayPosition, RayDirection, Distance);
	}
	else
	{
		return Mesh.RayIntersects(RayPosition, RayDirection, Distance);
	}
}


//+-----------------------------------------------------------------------------
//| Nullifys a matrix
//+-----------------------------------------------------------------------------
VOID MODEL_GEOSET::NullifyMatrix(D3DXMATRIX& Matrix)
{
	Matrix._11 = 0.0f;
	Matrix._12 = 0.0f;
	Matrix._13 = 0.0f;
	Matrix._14 = 0.0f;

	Matrix._21 = 0.0f;
	Matrix._22 = 0.0f;
	Matrix._23 = 0.0f;
	Matrix._24 = 0.0f;

	Matrix._31 = 0.0f;
	Matrix._32 = 0.0f;
	Matrix._33 = 0.0f;
	Matrix._34 = 0.0f;

	Matrix._41 = 0.0f;
	Matrix._42 = 0.0f;
	Matrix._43 = 0.0f;
	Matrix._44 = 0.0f;
}


//+-----------------------------------------------------------------------------
//| Adds two matrices together
//+-----------------------------------------------------------------------------
VOID MODEL_GEOSET::AddMatrix(D3DXMATRIX& Matrix, CONST D3DXMATRIX& AddedMatrix)
{
	Matrix._11 += AddedMatrix._11;
	Matrix._12 += AddedMatrix._12;
	Matrix._13 += AddedMatrix._13;
	Matrix._14 += AddedMatrix._14;

	Matrix._21 += AddedMatrix._21;
	Matrix._22 += AddedMatrix._22;
	Matrix._23 += AddedMatrix._23;
	Matrix._24 += AddedMatrix._24;

	Matrix._31 += AddedMatrix._31;
	Matrix._32 += AddedMatrix._32;
	Matrix._33 += AddedMatrix._33;
	Matrix._34 += AddedMatrix._34;

	Matrix._41 += AddedMatrix._41;
	Matrix._42 += AddedMatrix._42;
	Matrix._43 += AddedMatrix._43;
	Matrix._44 += AddedMatrix._44;
}


//+-----------------------------------------------------------------------------
//| Scales a matrix by a factor
//+-----------------------------------------------------------------------------
VOID MODEL_GEOSET::ScaleMatrix(D3DXMATRIX& Matrix, INT Factor)
{
	FLOAT Scale;

	Scale = (Factor == 0) ? 0.0f : (1.0f / static_cast<FLOAT>(Factor));

	Matrix._11 *= Scale;
	Matrix._22 *= Scale;
	Matrix._33 *= Scale;
}
