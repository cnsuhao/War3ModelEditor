//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_BUILDER_H
#define MAGOS_MODEL_BUILDER_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Model.h"
#include "War3ModelImporter\\War3ModelBuilder.h"


//+-----------------------------------------------------------------------------
//| Model builder class
//+-----------------------------------------------------------------------------
class MODEL_BUILDER : public WAR3_MODEL_BUILDER
{
	public:
		CONSTRUCTOR MODEL_BUILDER();
		DESTRUCTOR ~MODEL_BUILDER();

		virtual VOID SetErrorMessage(CONST CHAR* ErrorMessage);

		virtual VOID SetModelInfo(CONST WAR3_MODEL_INFO& ModelInfo);

		virtual BOOL CreateAttachment(CONST WAR3_MODEL_ATTACHMENT& Attachment);
		virtual BOOL CreateBone(CONST WAR3_MODEL_BONE& Bone);
		virtual BOOL CreateCollisionShape(CONST WAR3_MODEL_COLLISION_SHAPE& CollisionShape);
		virtual BOOL CreateEventObject(CONST WAR3_MODEL_EVENT_OBJECT& EventObject);
		virtual BOOL CreateHelper(CONST WAR3_MODEL_HELPER& Helper);
		virtual BOOL CreateLight(CONST WAR3_MODEL_LIGHT& Light);
		virtual BOOL CreateParticleEmitter(CONST WAR3_MODEL_PARTICLE_EMITTER& ParticleEmitter);
		virtual BOOL CreateParticleEmitter2(CONST WAR3_MODEL_PARTICLE_EMITTER_2& ParticleEmitter2);
		virtual BOOL CreateRibbonEmitter(CONST WAR3_MODEL_RIBBON_EMITTER& RibbonEmitter);

		virtual BOOL CreateCamera(CONST WAR3_MODEL_CAMERA& Camera);
		virtual BOOL CreateGeoset(CONST WAR3_MODEL_GEOSET& Geoset);
		virtual BOOL CreateGeosetAnimation(CONST WAR3_MODEL_GEOSET_ANIMATION& GeosetAnimation);
		virtual BOOL CreateGlobalSequence(CONST WAR3_MODEL_GLOBAL_SEQUENCE& GlobalSequence);
		virtual BOOL CreateMaterial(CONST WAR3_MODEL_MATERIAL& Material);
		virtual BOOL CreatePivotPoint(CONST WAR3_MODEL_PIVOT_POINT& PivotPoint);
		virtual BOOL CreateSequence(CONST WAR3_MODEL_SEQUENCE& Sequence);
		virtual BOOL CreateTexture(CONST WAR3_MODEL_TEXTURE& Texture);
		virtual BOOL CreateTextureAnimation(CONST WAR3_MODEL_TEXTURE_ANIMATION& TextureAnimation);

		virtual BOOL CreateEventObjectTrack(INT EventObjectId, INT Time);

		virtual BOOL CreateGeosetVertex(INT GeosetId, CONST WAR3_MODEL_VERTEX& Vertex);
		virtual BOOL CreateGeosetFace(INT GeosetId, CONST WAR3_MODEL_FACE& Face);
		virtual BOOL CreateGeosetExtent(INT GeosetId, CONST WAR3_MODEL_EXTENT& Extent);
		virtual BOOL CreateGeosetGroup(INT GeosetId);
		virtual BOOL CreateGeosetGroupBone(INT GeosetId, INT GeosetGroupId, INT BoneId);

		virtual BOOL CreateMaterialLayer(INT MaterialId, CONST WAR3_MODEL_MATERIAL_LAYER& MaterialLayer);

		virtual BOOL CreateInterpolator();
		virtual VOID SetInterpolatorType(WAR3_INTERPOLATOR_TYPE InterpolatorType);
		virtual VOID SetInterpolatorInterpolationType(WAR3_INTERPOLATION_TYPE InterpolationType);
		virtual VOID SetInterpolatorGlobalSequenceId(INT GlobalSequenceId);
		virtual VOID AddScalar(INT Time, FLOAT Scalar, FLOAT InTan, FLOAT OutTan);
		virtual VOID AddScalarInt(INT Time, INT Scalar, INT InTan, INT OutTan);
		virtual VOID AddVector2(INT Time, CONST WAR3_MODEL_VECTOR_2& Vector, CONST WAR3_MODEL_VECTOR_2& InTan, CONST WAR3_MODEL_VECTOR_2& OutTan);
		virtual VOID AddVector3(INT Time, CONST WAR3_MODEL_VECTOR_3& Vector, CONST WAR3_MODEL_VECTOR_3& InTan, CONST WAR3_MODEL_VECTOR_3& OutTan);
		virtual VOID AddVector4(INT Time, CONST WAR3_MODEL_VECTOR_4& Vector, CONST WAR3_MODEL_VECTOR_4& InTan, CONST WAR3_MODEL_VECTOR_4& OutTan);

		virtual VOID SetAnimatedAttachmentTranslation(INT AttachmentId);
		virtual VOID SetAnimatedAttachmentRotation(INT AttachmentId);
		virtual VOID SetAnimatedAttachmentScaling(INT AttachmentId);
		virtual VOID SetAnimatedAttachmentVisibility(INT AttachmentId);

		virtual VOID SetAnimatedBoneTranslation(INT BoneId);
		virtual VOID SetAnimatedBoneRotation(INT BoneId);
		virtual VOID SetAnimatedBoneScaling(INT BoneId);

		virtual VOID SetAnimatedCollisionShapeTranslation(INT CollisionShapeId);
		virtual VOID SetAnimatedCollisionShapeRotation(INT CollisionShapeId);
		virtual VOID SetAnimatedCollisionShapeScaling(INT CollisionShapeId);

		virtual VOID SetAnimatedEventObjectTranslation(INT EventObjectId);
		virtual VOID SetAnimatedEventObjectRotation(INT EventObjectId);
		virtual VOID SetAnimatedEventObjectScaling(INT EventObjectId);

		virtual VOID SetAnimatedHelperTranslation(INT HelperId);
		virtual VOID SetAnimatedHelperRotation(INT HelperId);
		virtual VOID SetAnimatedHelperScaling(INT HelperId);

		virtual VOID SetAnimatedLightTranslation(INT LightId);
		virtual VOID SetAnimatedLightRotation(INT LightId);
		virtual VOID SetAnimatedLightScaling(INT LightId);
		virtual VOID SetAnimatedLightAttenuationStart(INT LightId);
		virtual VOID SetAnimatedLightAttenuationEnd(INT LightId);
		virtual VOID SetAnimatedLightColor(INT LightId);
		virtual VOID SetAnimatedLightIntensity(INT LightId);
		virtual VOID SetAnimatedLightAmbientColor(INT LightId);
		virtual VOID SetAnimatedLightAmbientIntensity(INT LightId);
		virtual VOID SetAnimatedLightVisibility(INT LightId);

		virtual VOID SetAnimatedParticleEmitterTranslation(INT ParticleEmitterId);
		virtual VOID SetAnimatedParticleEmitterRotation(INT ParticleEmitterId);
		virtual VOID SetAnimatedParticleEmitterScaling(INT ParticleEmitterId);
		virtual VOID SetAnimatedParticleEmitterEmissionRate(INT ParticleEmitterId);
		virtual VOID SetAnimatedParticleEmitterGravity(INT ParticleEmitterId);
		virtual VOID SetAnimatedParticleEmitterLongitude(INT ParticleEmitterId);
		virtual VOID SetAnimatedParticleEmitterLatitude(INT ParticleEmitterId);
		virtual VOID SetAnimatedParticleEmitterVisibility(INT ParticleEmitterId);
		virtual VOID SetAnimatedParticleEmitterParticleLifeSpan(INT ParticleEmitterId);
		virtual VOID SetAnimatedParticleEmitterParticleInitialVelocity(INT ParticleEmitterId);

		virtual VOID SetAnimatedParticleEmitter2Translation(INT ParticleEmitter2Id);
		virtual VOID SetAnimatedParticleEmitter2Rotation(INT ParticleEmitter2Id);
		virtual VOID SetAnimatedParticleEmitter2Scaling(INT ParticleEmitter2Id);
		virtual VOID SetAnimatedParticleEmitter2Speed(INT ParticleEmitter2Id);
		virtual VOID SetAnimatedParticleEmitter2Variation(INT ParticleEmitter2Id);
		virtual VOID SetAnimatedParticleEmitter2Latitude(INT ParticleEmitter2Id);
		virtual VOID SetAnimatedParticleEmitter2Gravity(INT ParticleEmitter2Id);
		virtual VOID SetAnimatedParticleEmitter2Visibility(INT ParticleEmitter2Id);
		virtual VOID SetAnimatedParticleEmitter2EmissionRate(INT ParticleEmitter2Id);
		virtual VOID SetAnimatedParticleEmitter2Width(INT ParticleEmitter2Id);
		virtual VOID SetAnimatedParticleEmitter2Length(INT ParticleEmitter2Id);

		virtual VOID SetAnimatedRibbonEmitterTranslation(INT RibbonEmitterId);
		virtual VOID SetAnimatedRibbonEmitterRotation(INT RibbonEmitterId);
		virtual VOID SetAnimatedRibbonEmitterScaling(INT RibbonEmitterId);
		virtual VOID SetAnimatedRibbonEmitterHeightAbove(INT RibbonEmitterId);
		virtual VOID SetAnimatedRibbonEmitterHeightBelow(INT RibbonEmitterId);
		virtual VOID SetAnimatedRibbonEmitterAlpha(INT RibbonEmitterId);
		virtual VOID SetAnimatedRibbonEmitterColor(INT RibbonEmitterId);
		virtual VOID SetAnimatedRibbonEmitterTextureSlot(INT RibbonEmitterId);
		virtual VOID SetAnimatedRibbonEmitterVisibility(INT RibbonEmitterId);

		virtual VOID SetAnimatedCameraSourceTranslation(INT CameraId);
		virtual VOID SetAnimatedCameraTargetTranslation(INT CameraId);
		virtual VOID SetAnimatedCameraRotation(INT CameraId);

		virtual VOID SetAnimatedGeosetAnimationColor(INT GeosetAnimationId);
		virtual VOID SetAnimatedGeosetAnimationAlpha(INT GeosetAnimationId);

		virtual VOID SetAnimatedMaterialLayerAlpha(INT MaterialId, INT MaterialLayerId);
		virtual VOID SetAnimatedMaterialLayerTextureId(INT MaterialId, INT MaterialLayerId);

		virtual VOID SetAnimatedTextureAnimationTranslation(INT TextureAnimationId);
		virtual VOID SetAnimatedTextureAnimationRotation(INT TextureAnimationId);
		virtual VOID SetAnimatedTextureAnimationScaling(INT TextureAnimationId);

	protected:
		INTERPOLATOR CurrentInterpolator;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern MODEL_BUILDER ModelBuilder;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
