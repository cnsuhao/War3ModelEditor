//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_GEOSET_H
#define MAGOS_MODEL_GEOSET_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelGeosetVertex.h"
#include "ModelGeosetFace.h"
#include "ModelGeosetGroup.h"
#include "ModelBase.h"
#include "Mesh.h"


//+-----------------------------------------------------------------------------
//| Model geoset data structure
//+-----------------------------------------------------------------------------
struct MODEL_GEOSET_DATA
{
	MODEL_GEOSET_DATA()
	{
		MaterialId = INVALID_INDEX;
		SelectionGroup = 0;

		Unselectable = FALSE;

		InternalGeosetId = INVALID_INDEX;
	}

	INT MaterialId;
	INT SelectionGroup;

	BOOL Unselectable;

	SIMPLE_CONTAINER<MODEL_GEOSET_VERTEX*> VertexContainer;
	SIMPLE_CONTAINER<MODEL_GEOSET_FACE*> FaceContainer;
	SIMPLE_CONTAINER<MODEL_GEOSET_GROUP*> GroupContainer;
	SIMPLE_CONTAINER<EXTENT*> ExtentContainer;

	EXTENT Extent;

	INT InternalGeosetId;
};


//+-----------------------------------------------------------------------------
//| Model geoset class
//+-----------------------------------------------------------------------------
class MODEL_GEOSET
{
	public:
		CONSTRUCTOR MODEL_GEOSET();
		DESTRUCTOR ~MODEL_GEOSET();

		VOID Clear();
		INT GetSize();

		MODEL_GEOSET_DATA& Data();

		INT GetRenderOrder();

		FLOAT& Alpha();
		D3DXCOLOR& Color();

		VOID Rebuild();
		VOID Render(CONST SEQUENCE_TIME& Time, BOOL Animated);
		VOID RenderInView();

		BOOL BuildMesh();
		BOOL BuildAnimatedMesh(CONST SEQUENCE_TIME& Time);
		BOOL BuildAnimatedTexture(CONST SEQUENCE_TIME& Time, MODEL_TEXTURE_ANIMATION* TextureAnimation, FLOAT Alpha);

		BOOL AddVertex(MODEL_GEOSET_VERTEX* Vertex);
		BOOL AddFace(MODEL_GEOSET_FACE* Face);
		BOOL AddGroup(MODEL_GEOSET_GROUP* Group);
		BOOL AddExtent(EXTENT* Extent);

		VOID CalculateExtent(EXTENT& Extent);
		BOOL RayIntersects(CONST D3DXVECTOR3& RayPosition, CONST D3DXVECTOR3& RayDirection, BOOL Animated, FLOAT* Distance);

	protected:
		VOID NullifyMatrix(D3DXMATRIX& Matrix);
		VOID AddMatrix(D3DXMATRIX& Matrix, CONST D3DXMATRIX& AddedMatrix);
		VOID ScaleMatrix(D3DXMATRIX& Matrix, INT Factor);

		MODEL_GEOSET_DATA GeosetData;

		BOOL MeshBuildt;

		FLOAT GeosetAlpha;
		D3DXCOLOR GeosetColor;

		MESH Mesh;
		MESH AnimatedMesh;

	public:
		REFERENCE<MODEL_GEOSET*, MODEL_MATERIAL*> MaterialNode;
		REFERENCE_OBJECT<MODEL_BONE*, MODEL_GEOSET*> BoneNodes;
		REFERENCE_OBJECT<MODEL_GEOSET_ANIMATION*, MODEL_GEOSET*> GeosetAnimationNodes;

		std::set<INT> SelectedVertices;
		std::set<INT> SelectedFaces;
};


//+-----------------------------------------------------------------------------
//| Post-included files
//+-----------------------------------------------------------------------------
#include "Graphics.h"


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
