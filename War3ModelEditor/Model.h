//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_H
#define MAGOS_MODEL_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Misc.h"


//+-----------------------------------------------------------------------------
//| Pre-declared classes
//+-----------------------------------------------------------------------------
class MODEL_BASE;
class MODEL_BONE;
class MODEL_GEOSET;
class MODEL_GEOSET_ANIMATION;
class MODEL_GEOSET_VERTEX;
class MODEL_GEOSET_FACE;
class MODEL_GEOSET_GROUP;
class MODEL_TEXTURE;
class MODEL_TEXTURE_ANIMATION;
class MODEL_MATERIAL;
class MODEL_MATERIAL_LAYER;
class MODEL_SEQUENCE;
class MODEL_GLOBAL_SEQUENCE;
class MODEL_LIGHT;
class MODEL_HELPER;
class MODEL_CAMERA;
class MODEL_ATTACHMENT;
class MODEL_EVENT_OBJECT;
class MODEL_COLLISION_SHAPE;
class MODEL_PARTICLE_EMITTER;
class MODEL_PARTICLE_EMITTER_2;
class MODEL_RIBBON_EMITTER;


//+-----------------------------------------------------------------------------
//| Model info data structure
//+-----------------------------------------------------------------------------
struct MODEL_INFO_DATA
{
	MODEL_INFO_DATA()
	{
		Version = MODEL_DEFAULT_VERSION;
		Name = "Name";
		AnimationFile = "";

		BlendTime = 150;
	}

	DWORD Version;
	std::string Name;
	std::string AnimationFile;

	EXTENT Extent;
	INT BlendTime;
};


//+-----------------------------------------------------------------------------
//| Model data structure
//+-----------------------------------------------------------------------------
struct MODEL_DATA
{
	MODEL_DATA()
	{
		//Empty
	}

	MODEL_INFO_DATA Info;

	SIMPLE_CONTAINER<MODEL_BASE*> BaseContainer;

	SIMPLE_CONTAINER<MODEL_ATTACHMENT*> AttachmentContainer;
	SIMPLE_CONTAINER<MODEL_BONE*> BoneContainer;
	SIMPLE_CONTAINER<MODEL_COLLISION_SHAPE*> CollisionShapeContainer;
	SIMPLE_CONTAINER<MODEL_EVENT_OBJECT*> EventObjectContainer;
	SIMPLE_CONTAINER<MODEL_HELPER*> HelperContainer;
	SIMPLE_CONTAINER<MODEL_LIGHT*> LightContainer;
	SIMPLE_CONTAINER<MODEL_PARTICLE_EMITTER*> ParticleEmitterContainer;
	SIMPLE_CONTAINER<MODEL_PARTICLE_EMITTER_2*> ParticleEmitter2Container;
	SIMPLE_CONTAINER<MODEL_RIBBON_EMITTER*> RibbonEmitterContainer;

	SIMPLE_CONTAINER<MODEL_CAMERA*> CameraContainer;
	SIMPLE_CONTAINER<MODEL_GEOSET*> GeosetContainer;
	SIMPLE_CONTAINER<MODEL_GEOSET_ANIMATION*> GeosetAnimationContainer;
	SIMPLE_CONTAINER<MODEL_GLOBAL_SEQUENCE*> GlobalSequenceContainer;
	SIMPLE_CONTAINER<MODEL_MATERIAL*> MaterialContainer;
	SIMPLE_CONTAINER<D3DXVECTOR3*> PivotPointContainer;
	SIMPLE_CONTAINER<MODEL_SEQUENCE*> SequenceContainer;
	SIMPLE_CONTAINER<MODEL_TEXTURE*> TextureContainer;
	SIMPLE_CONTAINER<MODEL_TEXTURE_ANIMATION*> TextureAnimationContainer;
};


//+-----------------------------------------------------------------------------
//| Model class
//+-----------------------------------------------------------------------------
class MODEL
{
	public:
		CONSTRUCTOR MODEL();
		DESTRUCTOR ~MODEL();

		VOID Clear();

		MODEL_DATA& Data();

		VOID Rebuild();
		VOID Render(INT TimeDifference);
		VOID PlayAnimation(MODEL_SEQUENCE* Sequence);

		VOID CalculateBoundsRadius();
		FLOAT GetBoundsRadius();
		D3DXVECTOR3 GetBoundsCenter();

		MODEL_GEOSET* GetIntersectedGeoset(CONST D3DXVECTOR3& RayPosition, CONST D3DXVECTOR3& RayDirection);

		BOOL AddAttachment(MODEL_ATTACHMENT* Attachment, MODEL_BASE* ParentNode = NULL);
		BOOL AddBone(MODEL_BONE* Bone, MODEL_BASE* ParentNode = NULL);
		BOOL AddCollisionShape(MODEL_COLLISION_SHAPE* CollisionShape, MODEL_BASE* ParentNode = NULL);
		BOOL AddEventObject(MODEL_EVENT_OBJECT* EventObject, MODEL_BASE* ParentNode = NULL);
		BOOL AddHelper(MODEL_HELPER* Helper, MODEL_BASE* ParentNode = NULL);
		BOOL AddLight(MODEL_LIGHT* Light, MODEL_BASE* ParentNode = NULL);
		BOOL AddParticleEmitter(MODEL_PARTICLE_EMITTER* ParticleEmitter, MODEL_BASE* ParentNode = NULL);
		BOOL AddParticleEmitter2(MODEL_PARTICLE_EMITTER_2* ParticleEmitter2, MODEL_BASE* ParentNode = NULL);
		BOOL AddRibbonEmitter(MODEL_RIBBON_EMITTER* RibbonEmitter, MODEL_BASE* ParentNode = NULL);

		BOOL AddCamera(MODEL_CAMERA* Camera);
		BOOL AddGeoset(MODEL_GEOSET* Geoset, BOOL Imported = FALSE);
		BOOL AddGeosetAnimation(MODEL_GEOSET_ANIMATION* GeosetAnimation);
		BOOL AddGlobalSequence(MODEL_GLOBAL_SEQUENCE* GlobalSequence);
		BOOL AddMaterial(MODEL_MATERIAL* Material);
		BOOL AddPivotPoint(D3DXVECTOR3* PivotPoint);
		BOOL AddSequence(MODEL_SEQUENCE* Sequence);
		BOOL AddTexture(MODEL_TEXTURE* Texture);
		BOOL AddTextureAnimation(MODEL_TEXTURE_ANIMATION* TextureAnimation);

		BOOL RemoveAttachment(MODEL_ATTACHMENT* Attachment, HWND Window = NULL);
		BOOL RemoveBone(MODEL_BONE* Bone, HWND Window = NULL);
		BOOL RemoveCollisionShape(MODEL_COLLISION_SHAPE* CollisionShape, HWND Window = NULL);
		BOOL RemoveEventObject(MODEL_EVENT_OBJECT* EventObject, HWND Window = NULL);
		BOOL RemoveHelper(MODEL_HELPER* Helper, HWND Window = NULL);
		BOOL RemoveLight(MODEL_LIGHT* Light, HWND Window = NULL);
		BOOL RemoveParticleEmitter(MODEL_PARTICLE_EMITTER* ParticleEmitter, HWND Window = NULL);
		BOOL RemoveParticleEmitter2(MODEL_PARTICLE_EMITTER_2* ParticleEmitter2, HWND Window = NULL);
		BOOL RemoveRibbonEmitter(MODEL_RIBBON_EMITTER* RibbonEmitter, HWND Window = NULL);

		BOOL RemoveCamera(MODEL_CAMERA* Camera, HWND Window = NULL);
		BOOL RemoveGeoset(MODEL_GEOSET* Geoset, HWND Window = NULL);
		BOOL RemoveGeosetAnimation(MODEL_GEOSET_ANIMATION* GeosetAnimation, HWND Window = NULL);
		BOOL RemoveGlobalSequence(MODEL_GLOBAL_SEQUENCE* GlobalSequence, HWND Window = NULL);
		BOOL RemoveMaterial(MODEL_MATERIAL* Material, HWND Window = NULL);
		BOOL RemovePivotPoint(D3DXVECTOR3* PivotPoint, HWND Window = NULL);
		BOOL RemoveSequence(MODEL_SEQUENCE* Sequence, HWND Window = NULL);
		BOOL RemoveTexture(MODEL_TEXTURE* Texture, HWND Window = NULL);
		BOOL RemoveTextureAnimation(MODEL_TEXTURE_ANIMATION* TextureAnimation, HWND Window = NULL);

		MODEL_BASE* GetNode(INT ObjectId);
		MODEL_TEXTURE* GetTexture(INT TextureId);

		BOOL CreateTextureModel(CONST std::string& TextureFileName, INT Width, INT Height, INT RealWidth, INT RealHeight);
		BOOL CreateLoadingScreenModel(CONST std::string& TextureFileName);

		VOID GenerateObjectIds();
		VOID ConnectNodes();

		VOID WrapPivotPoints();
		VOID UnwrapPivotPoints();

	protected:
		BOOL AddBaseNode(MODEL_BASE* Node, MODEL_BASE* ParentNode);
		BOOL RemoveBaseNode(MODEL_BASE* Node, HWND Window);

		VOID InsertNode(MODEL_BASE* Node);

		MODEL_GEOSET_VERTEX* CreateVertex(CONST D3DXVECTOR3& Position, CONST D3DXVECTOR3& Normal, CONST D3DXVECTOR2& TexturePosition, INT VertexGroup);
		MODEL_GEOSET_FACE* CreateFace(INT Index1, INT Index2, INT Index3);
		MODEL_GEOSET_GROUP* CreateGroup(CONST std::vector<INT>& MatrixList);

		VOID RenderBones();
		VOID RenderLights();
		VOID RenderCameras();
		VOID RenderAttachments();
		VOID RenderCollisionShapes();

		MODEL_DATA ModelData;
		MODEL_BASE* Root;

		FLOAT BoundsRadius;
		D3DXVECTOR3 BoundsCenter;

		SEQUENCE_TIME AnimationTime;

		REFERENCE<MODEL*, MODEL_SEQUENCE*> SequenceNode;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern MODEL Model;


//+-----------------------------------------------------------------------------
//| Post-included files
//+-----------------------------------------------------------------------------
#include "ModelBone.h"
#include "ModelGeoset.h"
#include "ModelGeosetAnimation.h"
#include "ModelTexture.h"
#include "ModelTextureAnimation.h"
#include "ModelMaterial.h"
#include "ModelSequence.h"
#include "ModelGlobalSequence.h"
#include "ModelLight.h"
#include "ModelHelper.h"
#include "ModelCamera.h"
#include "ModelAttachment.h"
#include "ModelEventObject.h"
#include "ModelCollisionShape.h"
#include "ModelParticleEmitter.h"
#include "ModelParticleEmitter2.h"
#include "ModelRibbonEmitter.h"


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
