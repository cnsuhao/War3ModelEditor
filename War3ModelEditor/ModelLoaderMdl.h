//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_LOADER_MDL_H
#define MAGOS_MODEL_LOADER_MDL_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelLoader.h"


//+-----------------------------------------------------------------------------
//| Function pointer types
//+-----------------------------------------------------------------------------
typedef BOOL (*MDL_LOADER)(MODEL&, TOKEN_IN_STREAM&);


//+-----------------------------------------------------------------------------
//| Model loader mdl class
//+-----------------------------------------------------------------------------
class MODEL_LOADER_MDL : public MODEL_LOADER
{
	public:
		CONSTRUCTOR MODEL_LOADER_MDL();
		DESTRUCTOR ~MODEL_LOADER_MDL();

		virtual BOOL Save(MODEL& Model, CONST std::string& FileName, BUFFER& Buffer);
		virtual BOOL Load(MODEL& Model, CONST std::string& FileName, BUFFER& Buffer);

	protected:
		VOID RegisterMdlLoader(CONST std::string& Name, MDL_LOADER Loader);

		static BOOL SaveVersion(MODEL& Model, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveModel(MODEL& Model, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveSequences(MODEL& Model, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveSequence(MODEL_SEQUENCE& Sequence, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveGlobalSequences(MODEL& Model, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveTextures(MODEL& Model, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveTexture(MODEL_TEXTURE& Texture, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveMaterials(MODEL& Model, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveMaterial(MODEL_MATERIAL& Material, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveTextureAnimations(MODEL& Model, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveTextureAnimation(MODEL_TEXTURE_ANIMATION& TextureAnimation, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveGeosets(MODEL& Model, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveGeoset(MODEL_GEOSET& Geoset, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveGeosetAnimations(MODEL& Model, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveGeosetAnimation(MODEL_GEOSET_ANIMATION& GeosetAnimation, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveBones(MODEL& Model, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveBone(MODEL_BONE& Bone, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveLights(MODEL& Model, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveLight(MODEL_LIGHT& Light, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveHelpers(MODEL& Model, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveHelper(MODEL_HELPER& Helper, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveAttachments(MODEL& Model, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveAttachment(MODEL_ATTACHMENT& Attachment, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SavePivotPoints(MODEL& Model, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveParticleEmitters(MODEL& Model, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveParticleEmitter(MODEL_PARTICLE_EMITTER& ParticleEmitter, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveParticleEmitters2(MODEL& Model, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveParticleEmitter2(MODEL_PARTICLE_EMITTER_2& ParticleEmitter2, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveRibbonEmitters(MODEL& Model, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveRibbonEmitter(MODEL_RIBBON_EMITTER& RibbonEmitter, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveEventObjects(MODEL& Model, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveEventObject(MODEL_EVENT_OBJECT& EventObject, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveCameras(MODEL& Model, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveCamera(MODEL_CAMERA& Camera, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveCollisionShapes(MODEL& Model, TOKEN_OUT_STREAM& TokenStream);
		static BOOL SaveCollisionShape(MODEL_COLLISION_SHAPE& CollisionShape, TOKEN_OUT_STREAM& TokenStream);

		static BOOL LoadVersion(MODEL& Model, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadModel(MODEL& Model, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadSequences(MODEL& Model, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadSequence(MODEL_SEQUENCE& Sequence, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadGlobalSequences(MODEL& Model, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadTextures(MODEL& Model, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadTexture(MODEL_TEXTURE& Texture, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadMaterials(MODEL& Model, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadMaterial(MODEL_MATERIAL& Material, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadMaterialLayer(MODEL_MATERIAL_LAYER& Layer, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadTextureAnimations(MODEL& Model, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadTextureAnimation(MODEL_TEXTURE_ANIMATION& TextureAnimation, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadGeosets(MODEL& Model, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadGeoset(MODEL_GEOSET& Geoset, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadGeosetAnimations(MODEL& Model, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadGeosetAnimation(MODEL_GEOSET_ANIMATION& GeosetAnimation, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadBones(MODEL& Model, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadBone(MODEL_BONE& Bone, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadLights(MODEL& Model, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadLight(MODEL_LIGHT& Light, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadHelpers(MODEL& Model, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadHelper(MODEL_HELPER& Helper, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadAttachments(MODEL& Model, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadAttachment(MODEL_ATTACHMENT& Attachment, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadPivotPoints(MODEL& Model, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadParticleEmitters(MODEL& Model, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadParticleEmitter(MODEL_PARTICLE_EMITTER& ParticleEmitter, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadParticleEmitters2(MODEL& Model, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadParticleEmitter2(MODEL_PARTICLE_EMITTER_2& ParticleEmitter2, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadRibbonEmitters(MODEL& Model, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadRibbonEmitter(MODEL_RIBBON_EMITTER& RibbonEmitter, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadEventObjects(MODEL& Model, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadEventObject(MODEL_EVENT_OBJECT& EventObject, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadCameras(MODEL& Model, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadCamera(MODEL_CAMERA& Camera, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadCollisionShapes(MODEL& Model, TOKEN_IN_STREAM& TokenStream);
		static BOOL LoadCollisionShape(MODEL_COLLISION_SHAPE& CollisionShape, TOKEN_IN_STREAM& TokenStream);

		static BOOL SaveBaseData(MODEL_BASE_DATA* BaseData, TOKEN_OUT_STREAM& TokenStream);
		static BOOL LoadBaseData(BOOL& ErrorOccured, MODEL_BASE_DATA* BaseData, TOKEN_IN_STREAM& TokenStream, CONST std::string& Token);
		static BOOL LoadStaticBaseData(BOOL& ErrorOccured, MODEL_BASE_DATA* BaseData, TOKEN_IN_STREAM& TokenStream, CONST std::string& Token);

		static BOOL SkipGroup(TOKEN_IN_STREAM& TokenStream);

		static std::string GetCurrentDate();

		std::map<std::string, MDL_LOADER> MdlLoaderMap;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern MODEL_LOADER_MDL ModelLoaderMdl;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
