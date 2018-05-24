//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WAR3_MODEL_BUILDER_H
#define MAGOS_WAR3_MODEL_BUILDER_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include <windows.h>


//+-----------------------------------------------------------------------------
//| Constants
//+-----------------------------------------------------------------------------
CONST INT WAR3_INVALID_ID = -1;


//+-----------------------------------------------------------------------------
//| Collision shape type enumeration
//+-----------------------------------------------------------------------------
enum WAR3_COLLISION_SHAPE_TYPE
{
	WAR3_COLLISION_SHAPE_TYPE_BOX,
	WAR3_COLLISION_SHAPE_TYPE_SPHERE,
};


//+-----------------------------------------------------------------------------
//| Filter mode enumeration
//+-----------------------------------------------------------------------------
enum WAR3_FILTER_MODE
{
	WAR3_FILTER_MODE_NONE,
	WAR3_FILTER_MODE_TRANSPARENT,
	WAR3_FILTER_MODE_BLEND,
	WAR3_FILTER_MODE_ADDITIVE,
	WAR3_FILTER_MODE_ADD_ALPHA,
	WAR3_FILTER_MODE_MODULATE,
};


//+-----------------------------------------------------------------------------
//| Interpolator type enumeration
//+-----------------------------------------------------------------------------
enum WAR3_INTERPOLATOR_TYPE
{
	WAR3_INTERPOLATOR_TYPE_NONE,
	WAR3_INTERPOLATOR_TYPE_SCALAR,
	WAR3_INTERPOLATOR_TYPE_SCALAR_INT,
	WAR3_INTERPOLATOR_TYPE_VECTOR2,
	WAR3_INTERPOLATOR_TYPE_VECTOR3,
	WAR3_INTERPOLATOR_TYPE_VECTOR4,
};


//+-----------------------------------------------------------------------------
//| Interpolation type enumeration
//+-----------------------------------------------------------------------------
enum WAR3_INTERPOLATION_TYPE
{
	WAR3_INTERPOLATION_TYPE_NONE,
	WAR3_INTERPOLATION_TYPE_LINEAR,
	WAR3_INTERPOLATION_TYPE_HERMITE,
	WAR3_INTERPOLATION_TYPE_BEZIER,
};


//+-----------------------------------------------------------------------------
//| Light type enumeration
//+-----------------------------------------------------------------------------
enum WAR3_LIGHT_TYPE
{
	WAR3_LIGHT_TYPE_OMNIDIRECTIONAL,
	WAR3_LIGHT_TYPE_DIRECTIONAL,
	WAR3_LIGHT_TYPE_AMBIENT,
};


//+-----------------------------------------------------------------------------
//| 2-dimensional vector structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_VECTOR_2
{
	WAR3_MODEL_VECTOR_2()
	{
		X = 0.0f;
		Y = 0.0f;
	}

	FLOAT X;
	FLOAT Y;
};


//+-----------------------------------------------------------------------------
//| 3-dimensional vector structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_VECTOR_3
{
	WAR3_MODEL_VECTOR_3()
	{
		X = 0.0f;
		Y = 0.0f;
		Z = 0.0f;
	}

	FLOAT X;
	FLOAT Y;
	FLOAT Z;
};


//+-----------------------------------------------------------------------------
//| 4-dimensional vector structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_VECTOR_4
{
	WAR3_MODEL_VECTOR_4()
	{
		X = 0.0f;
		Y = 0.0f;
		Z = 0.0f;
		W = 0.0f;
	}

	FLOAT X;
	FLOAT Y;
	FLOAT Z;
	FLOAT W;
};


//+-----------------------------------------------------------------------------
//| Color structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_COLOR
{
	WAR3_MODEL_COLOR()
	{
		R = 1.0f;
		G = 1.0f;
		B = 1.0f;
	}

	FLOAT R;
	FLOAT G;
	FLOAT B;
};


//+-----------------------------------------------------------------------------
//| Extent structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_EXTENT
{
	WAR3_MODEL_EXTENT()
	{
		Radius = 0.0f;
	}

	FLOAT Radius;
	WAR3_MODEL_VECTOR_3 Min;
	WAR3_MODEL_VECTOR_3 Max;
};


//+-----------------------------------------------------------------------------
//| Info structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_INFO
{
	WAR3_MODEL_INFO()
	{
		Version = 800;
		BlendTime = 150;

		Name = "";
		AnimationFile = "";
	}

	INT Version;
	INT BlendTime;

	CONST CHAR* Name;
	CONST CHAR* AnimationFile;

	WAR3_MODEL_EXTENT Extent;
};


//+-----------------------------------------------------------------------------
//| Node structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_NODE
{
	WAR3_MODEL_NODE()
	{
		Name = "";

		ObjectId = WAR3_INVALID_ID;
		ParentId = WAR3_INVALID_ID;

		DontInheritTranslation = FALSE;
		DontInheritRotation = FALSE;
		DontInheritScaling = FALSE;
		Billboarded = FALSE;
		BillboardedLockX = FALSE;
		BillboardedLockY = FALSE;
		BillboardedLockZ = FALSE;
		CameraAnchored = FALSE;
	}

	CONST CHAR* Name;

	INT ObjectId;
	INT ParentId;

	BOOL DontInheritTranslation;
	BOOL DontInheritRotation;
	BOOL DontInheritScaling;
	BOOL Billboarded;
	BOOL BillboardedLockX;
	BOOL BillboardedLockY;
	BOOL BillboardedLockZ;
	BOOL CameraAnchored;
};


//+-----------------------------------------------------------------------------
//| Attachment structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_ATTACHMENT
{
	WAR3_MODEL_ATTACHMENT()
	{
		//Empty
	}

	WAR3_MODEL_NODE Node;
};


//+-----------------------------------------------------------------------------
//| Bone structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_BONE
{
	WAR3_MODEL_BONE()
	{
		GeosetId = WAR3_INVALID_ID;
		GeosetAnimationId = WAR3_INVALID_ID;
	}

	WAR3_MODEL_NODE Node;

	INT GeosetId;
	INT GeosetAnimationId;
};


//+-----------------------------------------------------------------------------
//| Collision shape structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_COLLISION_SHAPE
{
	WAR3_MODEL_COLLISION_SHAPE()
	{
		Type = WAR3_COLLISION_SHAPE_TYPE_BOX;

		Radius = 0.0f;
	}

	WAR3_MODEL_NODE Node;

	WAR3_COLLISION_SHAPE_TYPE Type;

	FLOAT Radius;
	WAR3_MODEL_VECTOR_3 Vertex1;
	WAR3_MODEL_VECTOR_3 Vertex2;
};


//+-----------------------------------------------------------------------------
//| Event object structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_EVENT_OBJECT
{
	WAR3_MODEL_EVENT_OBJECT()
	{
		GlobalSequenceId = WAR3_INVALID_ID;
	}

	WAR3_MODEL_NODE Node;

	INT GlobalSequenceId;
};


//+-----------------------------------------------------------------------------
//| Helper structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_HELPER
{
	WAR3_MODEL_HELPER()
	{
		//Empty
	}

	WAR3_MODEL_NODE Node;
};


//+-----------------------------------------------------------------------------
//| Light structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_LIGHT
{
	WAR3_MODEL_LIGHT()
	{
		Type = WAR3_LIGHT_TYPE_OMNIDIRECTIONAL;

		Visibility = 1.0f;
		AttenuationStart = 0.0f;
		AttenuationEnd = 0.0f;
		Intensity = 0.0f;
		AmbientIntensity = 0.0f;
	}

	WAR3_MODEL_NODE Node;

	WAR3_LIGHT_TYPE Type;

	FLOAT Visibility;
	FLOAT AttenuationStart;
	FLOAT AttenuationEnd;
	FLOAT Intensity;
	FLOAT AmbientIntensity;
	WAR3_MODEL_COLOR Color;
	WAR3_MODEL_COLOR AmbientColor;
};


//+-----------------------------------------------------------------------------
//| Particle emitter structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_PARTICLE_EMITTER
{
	WAR3_MODEL_PARTICLE_EMITTER()
	{
		EmissionRate = 0.0f;
		Gravity = 0.0f;
		Longitude = 0.0f;
		Latitude = 0.0f;
		Visibility = 1.0f;

		ParticleFileName = "";
		ParticleLifeSpan = 0.0f;
		ParticleInitialVelocity = 0.0f;

		EmitterUsesMdl = FALSE;
		EmitterUsesTga = FALSE;
	}

	WAR3_MODEL_NODE Node;

	FLOAT EmissionRate;
	FLOAT Gravity;
	FLOAT Longitude;
	FLOAT Latitude;
	FLOAT Visibility;

	CONST CHAR* ParticleFileName;
	FLOAT ParticleLifeSpan;
	FLOAT ParticleInitialVelocity;

	BOOL EmitterUsesMdl;
	BOOL EmitterUsesTga;
};


//+-----------------------------------------------------------------------------
//| Segment structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_SEGMENT
{
	WAR3_MODEL_SEGMENT()
	{
		Alpha = 1.0f;
		Scaling = 1.0f;
	}

	WAR3_MODEL_COLOR Color;
	FLOAT Alpha;
	FLOAT Scaling;
};


//+-----------------------------------------------------------------------------
//| Head/Tail structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_HEAD_TAIL
{
	WAR3_MODEL_HEAD_TAIL()
	{
		Start = 0;
		End = 0;
		Repeat = 1;
	}

	INT Start;
	INT End;
	INT Repeat;
};


//+-----------------------------------------------------------------------------
//| Particle emitter 2 structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_PARTICLE_EMITTER_2
{
	WAR3_MODEL_PARTICLE_EMITTER_2()
	{
		FilterMode = WAR3_FILTER_MODE_NONE;

		Speed = 0.0f;
		Variation = 0.0f;
		Latitude = 0.0f;
		Gravity = 0.0f;
		Visibility = 1.0f;
		EmissionRate = 0.0f;
		Width = 0.0f;
		Length = 0.0f;

		Rows = 1;
		Columns = 1;
		TextureId = WAR3_INVALID_ID;
		PriorityPlane = 0;
		ReplaceableId = WAR3_INVALID_ID;
		Time = 0.0f;
		LifeSpan = 0.0f;
		TailLength = 0.0f;

		SortPrimitivesFarZ = FALSE;
		LineEmitter = FALSE;
		ModelSpace = FALSE;
		AlphaKey = FALSE;
		Unshaded = FALSE;
		Unfogged = FALSE;
		XYQuad = FALSE;
		Squirt = FALSE;
		Head = FALSE;
		Tail = FALSE;
	}

	WAR3_MODEL_NODE Node;

	WAR3_FILTER_MODE FilterMode;

	FLOAT Speed;
	FLOAT Variation;
	FLOAT Latitude;
	FLOAT Gravity;
	FLOAT Visibility;
	FLOAT EmissionRate;
	FLOAT Width;
	FLOAT Length;

	WAR3_MODEL_SEGMENT Segment1;
	WAR3_MODEL_SEGMENT Segment2;
	WAR3_MODEL_SEGMENT Segment3;

	WAR3_MODEL_HEAD_TAIL HeadLifeSpan;
	WAR3_MODEL_HEAD_TAIL HeadDecay;
	WAR3_MODEL_HEAD_TAIL TailLifeSpan;
	WAR3_MODEL_HEAD_TAIL TailDecay;

	INT Rows;
	INT Columns;
	INT TextureId;
	INT PriorityPlane;
	INT ReplaceableId;
	FLOAT Time;
	FLOAT LifeSpan;
	FLOAT TailLength;

	BOOL SortPrimitivesFarZ;
	BOOL LineEmitter;
	BOOL ModelSpace;
	BOOL AlphaKey;
	BOOL Unshaded;
	BOOL Unfogged;
	BOOL XYQuad;
	BOOL Squirt;
	BOOL Head;
	BOOL Tail;
};


//+-----------------------------------------------------------------------------
//| Ribbon emitter structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_RIBBON_EMITTER
{
	WAR3_MODEL_RIBBON_EMITTER()
	{
		HeightAbove = 0.0f;
		HeightBelow = 0.0f;
		Alpha = 1.0f;
		TextureSlot = 0.0f;
		Visibility = 1.0f;

		EmissionRate = 1.0f;
		LifeSpan = 1.0f;
		Gravity = 1.0f;
		Rows = 1;
		Columns = 1;
		MaterialId = WAR3_INVALID_ID;
	}

	WAR3_MODEL_NODE Node;

	WAR3_MODEL_COLOR Color;

	FLOAT HeightAbove;
	FLOAT HeightBelow;
	FLOAT Alpha;
	FLOAT TextureSlot;
	FLOAT Visibility;

	FLOAT EmissionRate;
	FLOAT LifeSpan;
	FLOAT Gravity;
	INT Rows;
	INT Columns;
	INT MaterialId;
};


//+-----------------------------------------------------------------------------
//| Camera structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_CAMERA
{
	WAR3_MODEL_CAMERA()
	{
		Name = "";

		FieldOfView = 0.0f;
		NearDistance = 0.0f;
		FarDistance = 0.0f;
	}

	CONST CHAR* Name;

	WAR3_MODEL_VECTOR_3 Position;
	WAR3_MODEL_VECTOR_3 TargetPosition;

	FLOAT FieldOfView;
	FLOAT NearDistance;
	FLOAT FarDistance;
};


//+-----------------------------------------------------------------------------
//| Geoset structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_GEOSET
{
	WAR3_MODEL_GEOSET()
	{
		MaterialId = WAR3_INVALID_ID;
		SelectionGroup = 0;
		Unselectable = FALSE;
	}

	INT MaterialId;
	INT SelectionGroup;
	BOOL Unselectable;

	WAR3_MODEL_EXTENT Extent;
};


//+-----------------------------------------------------------------------------
//| Geoset animation structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_GEOSET_ANIMATION
{
	WAR3_MODEL_GEOSET_ANIMATION()
	{
		GeosetId = WAR3_INVALID_ID;

		Alpha = 1.0f;

		UseColor = FALSE;
		DropShadow = FALSE;
	}

	INT GeosetId;

	WAR3_MODEL_COLOR Color;
	FLOAT Alpha;

	BOOL UseColor;
	BOOL DropShadow;
};


//+-----------------------------------------------------------------------------
//| Global sequence structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_GLOBAL_SEQUENCE
{
	WAR3_MODEL_GLOBAL_SEQUENCE()
	{
		Duration = 0;
	}

	INT Duration;
};


//+-----------------------------------------------------------------------------
//| Material structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_MATERIAL
{
	WAR3_MODEL_MATERIAL()
	{
		PriorityPlane = 0;

		ConstantColor = FALSE;
		SortPrimitivesFarZ = FALSE;
		FullResolution = FALSE;
	}

	INT PriorityPlane;

	BOOL ConstantColor;
	BOOL SortPrimitivesFarZ;
	BOOL FullResolution;
};


//+-----------------------------------------------------------------------------
//| Material layer structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_MATERIAL_LAYER
{
	WAR3_MODEL_MATERIAL_LAYER()
	{
		FilterMode = WAR3_FILTER_MODE_NONE;

		TextureId = WAR3_INVALID_ID;
		TextureAnimationId = WAR3_INVALID_ID;

		Alpha = 1.0f;

		Unshaded = FALSE;
		Unfogged = FALSE;
		TwoSided = FALSE;
		SphereEnvironmentMap = FALSE;
		NoDepthTest = FALSE;
		NoDepthSet = FALSE;
	}

	WAR3_FILTER_MODE FilterMode;

	INT TextureId;
	INT TextureAnimationId;

	FLOAT Alpha;

	BOOL Unshaded;
	BOOL Unfogged;
	BOOL TwoSided;
	BOOL SphereEnvironmentMap;
	BOOL NoDepthTest;
	BOOL NoDepthSet;
};


//+-----------------------------------------------------------------------------
//| Pivot point structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_PIVOT_POINT
{
	WAR3_MODEL_PIVOT_POINT()
	{
		//Empty;
	}

	WAR3_MODEL_VECTOR_3 Position;
};


//+-----------------------------------------------------------------------------
//| Sequence structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_SEQUENCE
{
	WAR3_MODEL_SEQUENCE()
	{
		Name = "";

		Rarity = 0.0f;
		MoveSpeed = 0.0f;
		NonLooping = FALSE;
		IntervalStart = 0;
		IntervalEnd = 0;
	}

	CONST CHAR* Name;

	FLOAT Rarity;
	FLOAT MoveSpeed;
	BOOL NonLooping;
	INT IntervalStart;
	INT IntervalEnd;

	WAR3_MODEL_EXTENT Extent;
};


//+-----------------------------------------------------------------------------
//| Texture structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_TEXTURE
{
	WAR3_MODEL_TEXTURE()
	{
		FileName = "";

		ReplaceableId = WAR3_INVALID_ID;

		WrapWidth = FALSE;
		WrapHeight = FALSE;
	}

	CONST CHAR* FileName;

	INT ReplaceableId;

	BOOL WrapWidth;
	BOOL WrapHeight;
};


//+-----------------------------------------------------------------------------
//| Texture animation structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_TEXTURE_ANIMATION
{
	WAR3_MODEL_TEXTURE_ANIMATION()
	{
		//Empty
	}

	BOOL Dummy; //Just a dummy to prevent structure from being empty
};


//+-----------------------------------------------------------------------------
//| Vertex structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_VERTEX
{
	WAR3_MODEL_VERTEX()
	{
		Group = 0;
	}

	WAR3_MODEL_VECTOR_3 Position;
	WAR3_MODEL_VECTOR_3 Normal;
	WAR3_MODEL_VECTOR_2 TexturePosition;
	INT Group;
};


//+-----------------------------------------------------------------------------
//| Face structure
//+-----------------------------------------------------------------------------
struct WAR3_MODEL_FACE
{
	WAR3_MODEL_FACE()
	{
		Index1 = 0;
		Index2 = 0;
		Index3 = 0;
	}

	INT Index1;
	INT Index2;
	INT Index3;
};


//+-----------------------------------------------------------------------------
//| Model builder class
//+-----------------------------------------------------------------------------
class WAR3_MODEL_BUILDER
{
	public:
		virtual VOID SetErrorMessage(CONST CHAR* ErrorMessage) = 0;

		virtual VOID SetModelInfo(CONST WAR3_MODEL_INFO& ModelInfo) = 0;

		virtual BOOL CreateAttachment(CONST WAR3_MODEL_ATTACHMENT& Attachment) = 0;
		virtual BOOL CreateBone(CONST WAR3_MODEL_BONE& Bone) = 0;
		virtual BOOL CreateCollisionShape(CONST WAR3_MODEL_COLLISION_SHAPE& CollisionShape) = 0;
		virtual BOOL CreateEventObject(CONST WAR3_MODEL_EVENT_OBJECT& EventObject) = 0;
		virtual BOOL CreateHelper(CONST WAR3_MODEL_HELPER& Helper) = 0;
		virtual BOOL CreateLight(CONST WAR3_MODEL_LIGHT& Light) = 0;
		virtual BOOL CreateParticleEmitter(CONST WAR3_MODEL_PARTICLE_EMITTER& ParticleEmitter) = 0;
		virtual BOOL CreateParticleEmitter2(CONST WAR3_MODEL_PARTICLE_EMITTER_2& ParticleEmitter2) = 0;
		virtual BOOL CreateRibbonEmitter(CONST WAR3_MODEL_RIBBON_EMITTER& RibbonEmitter) = 0;

		virtual BOOL CreateCamera(CONST WAR3_MODEL_CAMERA& Camera) = 0;
		virtual BOOL CreateGeoset(CONST WAR3_MODEL_GEOSET& Geoset) = 0;
		virtual BOOL CreateGeosetAnimation(CONST WAR3_MODEL_GEOSET_ANIMATION& GeosetAnimation) = 0;
		virtual BOOL CreateGlobalSequence(CONST WAR3_MODEL_GLOBAL_SEQUENCE& GlobalSequence) = 0;
		virtual BOOL CreateMaterial(CONST WAR3_MODEL_MATERIAL& Material) = 0;
		virtual BOOL CreatePivotPoint(CONST WAR3_MODEL_PIVOT_POINT& PivotPoint) = 0;
		virtual BOOL CreateSequence(CONST WAR3_MODEL_SEQUENCE& Sequence) = 0;
		virtual BOOL CreateTexture(CONST WAR3_MODEL_TEXTURE& Texture) = 0;
		virtual BOOL CreateTextureAnimation(CONST WAR3_MODEL_TEXTURE_ANIMATION& TextureAnimation) = 0;

		virtual BOOL CreateEventObjectTrack(INT EventObjectId, INT Time) = 0;

		virtual BOOL CreateGeosetVertex(INT GeosetId, CONST WAR3_MODEL_VERTEX& Vertex) = 0;
		virtual BOOL CreateGeosetFace(INT GeosetId, CONST WAR3_MODEL_FACE& Face) = 0;
		virtual BOOL CreateGeosetExtent(INT GeosetId, CONST WAR3_MODEL_EXTENT& Extent) = 0;
		virtual BOOL CreateGeosetGroup(INT GeosetId) = 0;
		virtual BOOL CreateGeosetGroupBone(INT GeosetId, INT GeosetGroupId, INT BoneId) = 0;

		virtual BOOL CreateMaterialLayer(INT MaterialId, CONST WAR3_MODEL_MATERIAL_LAYER& MaterialLayer) = 0;

		virtual BOOL CreateInterpolator() = 0;
		virtual VOID SetInterpolatorType(WAR3_INTERPOLATOR_TYPE InterpolatorType) = 0;
		virtual VOID SetInterpolatorInterpolationType(WAR3_INTERPOLATION_TYPE InterpolationType) = 0;
		virtual VOID SetInterpolatorGlobalSequenceId(INT GlobalSequenceId) = 0;
		virtual VOID AddScalar(INT Time, FLOAT Scalar, FLOAT InTan, FLOAT OutTan) = 0;
		virtual VOID AddScalarInt(INT Time, INT Scalar, INT InTan, INT OutTan) = 0;
		virtual VOID AddVector2(INT Time, CONST WAR3_MODEL_VECTOR_2& Vector, CONST WAR3_MODEL_VECTOR_2& InTan, CONST WAR3_MODEL_VECTOR_2& OutTan) = 0;
		virtual VOID AddVector3(INT Time, CONST WAR3_MODEL_VECTOR_3& Vector, CONST WAR3_MODEL_VECTOR_3& InTan, CONST WAR3_MODEL_VECTOR_3& OutTan) = 0;
		virtual VOID AddVector4(INT Time, CONST WAR3_MODEL_VECTOR_4& Vector, CONST WAR3_MODEL_VECTOR_4& InTan, CONST WAR3_MODEL_VECTOR_4& OutTan) = 0;

		virtual VOID SetAnimatedAttachmentTranslation(INT AttachmentId) = 0;
		virtual VOID SetAnimatedAttachmentRotation(INT AttachmentId) = 0;
		virtual VOID SetAnimatedAttachmentScaling(INT AttachmentId) = 0;
		virtual VOID SetAnimatedAttachmentVisibility(INT AttachmentId) = 0;

		virtual VOID SetAnimatedBoneTranslation(INT BoneId) = 0;
		virtual VOID SetAnimatedBoneRotation(INT BoneId) = 0;
		virtual VOID SetAnimatedBoneScaling(INT BoneId) = 0;

		virtual VOID SetAnimatedCollisionShapeTranslation(INT CollisionShapeId) = 0;
		virtual VOID SetAnimatedCollisionShapeRotation(INT CollisionShapeId) = 0;
		virtual VOID SetAnimatedCollisionShapeScaling(INT CollisionShapeId) = 0;

		virtual VOID SetAnimatedEventObjectTranslation(INT EventObjectId) = 0;
		virtual VOID SetAnimatedEventObjectRotation(INT EventObjectId) = 0;
		virtual VOID SetAnimatedEventObjectScaling(INT EventObjectId) = 0;

		virtual VOID SetAnimatedHelperTranslation(INT HelperId) = 0;
		virtual VOID SetAnimatedHelperRotation(INT HelperId) = 0;
		virtual VOID SetAnimatedHelperScaling(INT HelperId) = 0;

		virtual VOID SetAnimatedLightTranslation(INT LightId) = 0;
		virtual VOID SetAnimatedLightRotation(INT LightId) = 0;
		virtual VOID SetAnimatedLightScaling(INT LightId) = 0;
		virtual VOID SetAnimatedLightAttenuationStart(INT LightId) = 0;
		virtual VOID SetAnimatedLightAttenuationEnd(INT LightId) = 0;
		virtual VOID SetAnimatedLightColor(INT LightId) = 0;
		virtual VOID SetAnimatedLightIntensity(INT LightId) = 0;
		virtual VOID SetAnimatedLightAmbientColor(INT LightId) = 0;
		virtual VOID SetAnimatedLightAmbientIntensity(INT LightId) = 0;
		virtual VOID SetAnimatedLightVisibility(INT LightId) = 0;

		virtual VOID SetAnimatedParticleEmitterTranslation(INT ParticleEmitterId) = 0;
		virtual VOID SetAnimatedParticleEmitterRotation(INT ParticleEmitterId) = 0;
		virtual VOID SetAnimatedParticleEmitterScaling(INT ParticleEmitterId) = 0;
		virtual VOID SetAnimatedParticleEmitterEmissionRate(INT ParticleEmitterId) = 0;
		virtual VOID SetAnimatedParticleEmitterGravity(INT ParticleEmitterId) = 0;
		virtual VOID SetAnimatedParticleEmitterLongitude(INT ParticleEmitterId) = 0;
		virtual VOID SetAnimatedParticleEmitterLatitude(INT ParticleEmitterId) = 0;
		virtual VOID SetAnimatedParticleEmitterVisibility(INT ParticleEmitterId) = 0;
		virtual VOID SetAnimatedParticleEmitterParticleLifeSpan(INT ParticleEmitterId) = 0;
		virtual VOID SetAnimatedParticleEmitterParticleInitialVelocity(INT ParticleEmitterId) = 0;

		virtual VOID SetAnimatedParticleEmitter2Translation(INT ParticleEmitter2Id) = 0;
		virtual VOID SetAnimatedParticleEmitter2Rotation(INT ParticleEmitter2Id) = 0;
		virtual VOID SetAnimatedParticleEmitter2Scaling(INT ParticleEmitter2Id) = 0;
		virtual VOID SetAnimatedParticleEmitter2Speed(INT ParticleEmitter2Id) = 0;
		virtual VOID SetAnimatedParticleEmitter2Variation(INT ParticleEmitter2Id) = 0;
		virtual VOID SetAnimatedParticleEmitter2Latitude(INT ParticleEmitter2Id) = 0;
		virtual VOID SetAnimatedParticleEmitter2Gravity(INT ParticleEmitter2Id) = 0;
		virtual VOID SetAnimatedParticleEmitter2Visibility(INT ParticleEmitter2Id) = 0;
		virtual VOID SetAnimatedParticleEmitter2EmissionRate(INT ParticleEmitter2Id) = 0;
		virtual VOID SetAnimatedParticleEmitter2Width(INT ParticleEmitter2Id) = 0;
		virtual VOID SetAnimatedParticleEmitter2Length(INT ParticleEmitter2Id) = 0;

		virtual VOID SetAnimatedRibbonEmitterTranslation(INT RibbonEmitterId) = 0;
		virtual VOID SetAnimatedRibbonEmitterRotation(INT RibbonEmitterId) = 0;
		virtual VOID SetAnimatedRibbonEmitterScaling(INT RibbonEmitterId) = 0;
		virtual VOID SetAnimatedRibbonEmitterHeightAbove(INT RibbonEmitterId) = 0;
		virtual VOID SetAnimatedRibbonEmitterHeightBelow(INT RibbonEmitterId) = 0;
		virtual VOID SetAnimatedRibbonEmitterAlpha(INT RibbonEmitterId) = 0;
		virtual VOID SetAnimatedRibbonEmitterColor(INT RibbonEmitterId) = 0;
		virtual VOID SetAnimatedRibbonEmitterTextureSlot(INT RibbonEmitterId) = 0;
		virtual VOID SetAnimatedRibbonEmitterVisibility(INT RibbonEmitterId) = 0;

		virtual VOID SetAnimatedCameraSourceTranslation(INT CameraId) = 0;
		virtual VOID SetAnimatedCameraTargetTranslation(INT CameraId) = 0;
		virtual VOID SetAnimatedCameraRotation(INT CameraId) = 0;

		virtual VOID SetAnimatedGeosetAnimationColor(INT GeosetAnimationId) = 0;
		virtual VOID SetAnimatedGeosetAnimationAlpha(INT GeosetAnimationId) = 0;

		virtual VOID SetAnimatedMaterialLayerAlpha(INT MaterialId, INT MaterialLayerId) = 0;
		virtual VOID SetAnimatedMaterialLayerTextureId(INT MaterialId, INT MaterialLayerId) = 0;

		virtual VOID SetAnimatedTextureAnimationTranslation(INT TextureAnimationId) = 0;
		virtual VOID SetAnimatedTextureAnimationRotation(INT TextureAnimationId) = 0;
		virtual VOID SetAnimatedTextureAnimationScaling(INT TextureAnimationId) = 0;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
