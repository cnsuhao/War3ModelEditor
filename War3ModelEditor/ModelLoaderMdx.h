//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_LOADER_MDX_H
#define MAGOS_MODEL_LOADER_MDX_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelLoader.h"


//+-----------------------------------------------------------------------------
//| Function pointer types
//+-----------------------------------------------------------------------------
typedef BOOL (*MDX_LOADER)(MODEL&, DATA_IN_STREAM&, INT);


//+-----------------------------------------------------------------------------
//| Model loader mdx class
//+-----------------------------------------------------------------------------
class MODEL_LOADER_MDX : public MODEL_LOADER
{
	public:
		CONSTRUCTOR MODEL_LOADER_MDX();
		DESTRUCTOR ~MODEL_LOADER_MDX();

		virtual BOOL Save(MODEL& Model, CONST std::string& FileName, BUFFER& Buffer);
		virtual BOOL Load(MODEL& Model, CONST std::string& FileName, BUFFER& Buffer);

	protected:
		VOID RegisterMdxLoader(DWORD Group, MDX_LOADER Loader);

		static DWORD ReverseDWord(DWORD DWord);
		static std::string GroupToString(DWORD Group);

		static BOOL SaveVersion(MODEL& Model, DATA_OUT_STREAM& DataStream);
		static BOOL SaveModel(MODEL& Model, DATA_OUT_STREAM& DataStream);
		static BOOL SaveSequences(MODEL& Model, DATA_OUT_STREAM& DataStream);
		static BOOL SaveSequence(MODEL_SEQUENCE& Sequence, DATA_OUT_STREAM& DataStream);
		static BOOL SaveGlobalSequences(MODEL& Model, DATA_OUT_STREAM& DataStream);
		static BOOL SaveTextures(MODEL& Model, DATA_OUT_STREAM& DataStream);
		static BOOL SaveTexture(MODEL_TEXTURE& Texture, DATA_OUT_STREAM& DataStream);
		static BOOL SaveMaterials(MODEL& Model, DATA_OUT_STREAM& DataStream);
		static BOOL SaveMaterial(MODEL_MATERIAL& Material, DATA_OUT_STREAM& DataStream);
		static BOOL SaveTextureAnimations(MODEL& Model, DATA_OUT_STREAM& DataStream);
		static BOOL SaveTextureAnimation(MODEL_TEXTURE_ANIMATION& TextureAnimation, DATA_OUT_STREAM& DataStream);
		static BOOL SaveGeosets(MODEL& Model, DATA_OUT_STREAM& DataStream);
		static BOOL SaveGeoset(MODEL_GEOSET& Geoset, DATA_OUT_STREAM& DataStream);
		static BOOL SaveGeosetAnimations(MODEL& Model, DATA_OUT_STREAM& DataStream);
		static BOOL SaveGeosetAnimation(MODEL_GEOSET_ANIMATION& GeosetAnimation, DATA_OUT_STREAM& DataStream);
		static BOOL SaveBones(MODEL& Model, DATA_OUT_STREAM& DataStream);
		static BOOL SaveBone(MODEL_BONE& Bone, DATA_OUT_STREAM& DataStream);
		static BOOL SaveLights(MODEL& Model, DATA_OUT_STREAM& DataStream);
		static BOOL SaveLight(MODEL_LIGHT& Light, DATA_OUT_STREAM& DataStream);
		static BOOL SaveHelpers(MODEL& Model, DATA_OUT_STREAM& DataStream);
		static BOOL SaveHelper(MODEL_HELPER& Helper, DATA_OUT_STREAM& DataStream);
		static BOOL SaveAttachments(MODEL& Model, DATA_OUT_STREAM& DataStream);
		static BOOL SaveAttachment(MODEL_ATTACHMENT& Attachment, DATA_OUT_STREAM& DataStream);
		static BOOL SavePivotPoints(MODEL& Model, DATA_OUT_STREAM& DataStream);
		static BOOL SaveParticleEmitters(MODEL& Model, DATA_OUT_STREAM& DataStream);
		static BOOL SaveParticleEmitter(MODEL_PARTICLE_EMITTER& ParticleEmitter, DATA_OUT_STREAM& DataStream);
		static BOOL SaveParticleEmitters2(MODEL& Model, DATA_OUT_STREAM& DataStream);
		static BOOL SaveParticleEmitter2(MODEL_PARTICLE_EMITTER_2& ParticleEmitter2, DATA_OUT_STREAM& DataStream);
		static BOOL SaveRibbonEmitters(MODEL& Model, DATA_OUT_STREAM& DataStream);
		static BOOL SaveRibbonEmitter(MODEL_RIBBON_EMITTER& RibbonEmitter, DATA_OUT_STREAM& DataStream);
		static BOOL SaveEventObjects(MODEL& Model, DATA_OUT_STREAM& DataStream);
		static BOOL SaveEventObject(MODEL_EVENT_OBJECT& EventObject, DATA_OUT_STREAM& DataStream);
		static BOOL SaveCameras(MODEL& Model, DATA_OUT_STREAM& DataStream);
		static BOOL SaveCamera(MODEL_CAMERA& Camera, DATA_OUT_STREAM& DataStream);
		static BOOL SaveCollisionShapes(MODEL& Model, DATA_OUT_STREAM& DataStream);
		static BOOL SaveCollisionShape(MODEL_COLLISION_SHAPE& CollisionShape, DATA_OUT_STREAM& DataStream);

		static BOOL LoadVersion(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadModel(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadSequences(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadSequence(MODEL_SEQUENCE& Sequence, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadGlobalSequences(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadTextures(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadTexture(MODEL_TEXTURE& Texture, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadMaterials(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadMaterial(MODEL_MATERIAL& Material, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadTextureAnimations(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadTextureAnimation(MODEL_TEXTURE_ANIMATION& TextureAnimation, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadGeosets(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadGeoset(MODEL_GEOSET& Geoset, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadGeosetAnimations(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadGeosetAnimation(MODEL_GEOSET_ANIMATION& GeosetAnimation, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadBones(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadBone(MODEL_BONE& Bone, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadLights(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadLight(MODEL_LIGHT& Light, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadHelpers(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadHelper(MODEL_HELPER& Helper, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadAttachments(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadAttachment(MODEL_ATTACHMENT& Attachment, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadPivotPoints(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadParticleEmitters(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadParticleEmitter(MODEL_PARTICLE_EMITTER& ParticleEmitter, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadParticleEmitters2(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadParticleEmitter2(MODEL_PARTICLE_EMITTER_2& ParticleEmitter2, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadRibbonEmitters(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadRibbonEmitter(MODEL_RIBBON_EMITTER& RibbonEmitter, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadEventObjects(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadEventObject(MODEL_EVENT_OBJECT& EventObject, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadCameras(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadCamera(MODEL_CAMERA& Camera, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadCollisionShapes(MODEL& Model, DATA_IN_STREAM& DataStream, INT Size);
		static BOOL LoadCollisionShape(MODEL_COLLISION_SHAPE& CollisionShape, DATA_IN_STREAM& DataStream, INT Size);

		static BOOL SaveBaseData(MODEL_BASE_DATA* BaseData, DATA_OUT_STREAM& DataStream, DWORD NodeType);
		static BOOL LoadBaseData(MODEL_BASE_DATA* BaseData, DATA_IN_STREAM& DataStream, DWORD* NodeType = NULL);

		static BOOL ExpectTag(DATA_IN_STREAM& DataStream, DWORD Tag);
		static BOOL SkipGroup(DATA_IN_STREAM& DataStream, INT Size);

		std::map<DWORD, MDX_LOADER> MdxLoaderMap;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern MODEL_LOADER_MDX ModelLoaderMdx;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
