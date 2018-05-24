//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ModelBuilder.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
MODEL_BUILDER ModelBuilder;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_BUILDER::MODEL_BUILDER()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL_BUILDER::~MODEL_BUILDER()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Sets an error message
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetErrorMessage(CONST CHAR* ErrorMessage)
{
	Error.SetMessage(ErrorMessage);
}


//+-----------------------------------------------------------------------------
//| Sets the model info
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetModelInfo(CONST WAR3_MODEL_INFO& ModelInfo)
{
	Model.Data().Info.Version = ModelInfo.Version;
	Model.Data().Info.BlendTime = ModelInfo.BlendTime;

	Model.Data().Info.Name = ModelInfo.Name;
	Model.Data().Info.AnimationFile = ModelInfo.AnimationFile;

	Model.Data().Info.Extent.Radius = ModelInfo.Extent.Radius;
	Model.Data().Info.Extent.Min.x = ModelInfo.Extent.Min.X;
	Model.Data().Info.Extent.Min.y = ModelInfo.Extent.Min.Y;
	Model.Data().Info.Extent.Min.z = ModelInfo.Extent.Min.Z;
	Model.Data().Info.Extent.Max.x = ModelInfo.Extent.Max.X;
	Model.Data().Info.Extent.Max.y = ModelInfo.Extent.Max.Y;
	Model.Data().Info.Extent.Max.z = ModelInfo.Extent.Max.Z;
}


//+-----------------------------------------------------------------------------
//| Creates an attachment
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreateAttachment(CONST WAR3_MODEL_ATTACHMENT& Attachment)
{
	MODEL_ATTACHMENT* ModelAttachment;

	ModelAttachment = new MODEL_ATTACHMENT();
	if(ModelAttachment == NULL) return FALSE;

	ModelAttachment->Data().Name = Attachment.Node.Name;
	ModelAttachment->Data().ObjectId = Attachment.Node.ObjectId;
	ModelAttachment->Data().ParentId = Attachment.Node.ParentId;
	ModelAttachment->Data().DontInheritTranslation = Attachment.Node.DontInheritTranslation;
	ModelAttachment->Data().DontInheritRotation = Attachment.Node.DontInheritRotation;
	ModelAttachment->Data().DontInheritScaling = Attachment.Node.DontInheritScaling;
	ModelAttachment->Data().Billboarded = Attachment.Node.Billboarded;
	ModelAttachment->Data().BillboardedLockX = Attachment.Node.BillboardedLockX;
	ModelAttachment->Data().BillboardedLockY = Attachment.Node.BillboardedLockY;
	ModelAttachment->Data().BillboardedLockZ = Attachment.Node.BillboardedLockZ;
	ModelAttachment->Data().CameraAnchored = Attachment.Node.CameraAnchored;

	if(!Model.AddAttachment(ModelAttachment)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a bone
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreateBone(CONST WAR3_MODEL_BONE& Bone)
{
	MODEL_BONE* ModelBone;

	ModelBone = new MODEL_BONE();
	if(ModelBone == NULL) return FALSE;

	ModelBone->Data().Name = Bone.Node.Name;
	ModelBone->Data().ObjectId = Bone.Node.ObjectId;
	ModelBone->Data().ParentId = Bone.Node.ParentId;
	ModelBone->Data().DontInheritTranslation = Bone.Node.DontInheritTranslation;
	ModelBone->Data().DontInheritRotation = Bone.Node.DontInheritRotation;
	ModelBone->Data().DontInheritScaling = Bone.Node.DontInheritScaling;
	ModelBone->Data().Billboarded = Bone.Node.Billboarded;
	ModelBone->Data().BillboardedLockX = Bone.Node.BillboardedLockX;
	ModelBone->Data().BillboardedLockY = Bone.Node.BillboardedLockY;
	ModelBone->Data().BillboardedLockZ = Bone.Node.BillboardedLockZ;
	ModelBone->Data().CameraAnchored = Bone.Node.CameraAnchored;

	ModelBone->Data().GeosetId = Bone.GeosetId;
	ModelBone->Data().GeosetAnimationId = Bone.GeosetAnimationId;

	if(!Model.AddBone(ModelBone)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a collision shape
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreateCollisionShape(CONST WAR3_MODEL_COLLISION_SHAPE& CollisionShape)
{
	MODEL_COLLISION_SHAPE* ModelCollisionShape;

	ModelCollisionShape = new MODEL_COLLISION_SHAPE();
	if(ModelCollisionShape == NULL) return FALSE;

	ModelCollisionShape->Data().Name = CollisionShape.Node.Name;
	ModelCollisionShape->Data().ObjectId = CollisionShape.Node.ObjectId;
	ModelCollisionShape->Data().ParentId = CollisionShape.Node.ParentId;
	ModelCollisionShape->Data().DontInheritTranslation = CollisionShape.Node.DontInheritTranslation;
	ModelCollisionShape->Data().DontInheritRotation = CollisionShape.Node.DontInheritRotation;
	ModelCollisionShape->Data().DontInheritScaling = CollisionShape.Node.DontInheritScaling;
	ModelCollisionShape->Data().Billboarded = CollisionShape.Node.Billboarded;
	ModelCollisionShape->Data().BillboardedLockX = CollisionShape.Node.BillboardedLockX;
	ModelCollisionShape->Data().BillboardedLockY = CollisionShape.Node.BillboardedLockY;
	ModelCollisionShape->Data().BillboardedLockZ = CollisionShape.Node.BillboardedLockZ;
	ModelCollisionShape->Data().CameraAnchored = CollisionShape.Node.CameraAnchored;

	ModelCollisionShape->Data().Type = static_cast<COLLISION_SHAPE_TYPE>(CollisionShape.Type);
	ModelCollisionShape->Data().BoundsRadius = CollisionShape.Radius;
	ModelCollisionShape->Data().VertexList.push_back(D3DXVECTOR3(CollisionShape.Vertex1.X, CollisionShape.Vertex1.Y, CollisionShape.Vertex1.Z));
	if(ModelCollisionShape->Data().Type == COLLISION_SHAPE_TYPE_BOX) ModelCollisionShape->Data().VertexList.push_back(D3DXVECTOR3(CollisionShape.Vertex2.X, CollisionShape.Vertex2.Y, CollisionShape.Vertex2.Z));

	if(!Model.AddCollisionShape(ModelCollisionShape)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates an event object
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreateEventObject(CONST WAR3_MODEL_EVENT_OBJECT& EventObject)
{
	MODEL_EVENT_OBJECT* ModelEventObject;

	ModelEventObject = new MODEL_EVENT_OBJECT();
	if(ModelEventObject == NULL) return FALSE;

	ModelEventObject->Data().Name = EventObject.Node.Name;
	ModelEventObject->Data().ObjectId = EventObject.Node.ObjectId;
	ModelEventObject->Data().ParentId = EventObject.Node.ParentId;
	ModelEventObject->Data().DontInheritTranslation = EventObject.Node.DontInheritTranslation;
	ModelEventObject->Data().DontInheritRotation = EventObject.Node.DontInheritRotation;
	ModelEventObject->Data().DontInheritScaling = EventObject.Node.DontInheritScaling;
	ModelEventObject->Data().Billboarded = EventObject.Node.Billboarded;
	ModelEventObject->Data().BillboardedLockX = EventObject.Node.BillboardedLockX;
	ModelEventObject->Data().BillboardedLockY = EventObject.Node.BillboardedLockY;
	ModelEventObject->Data().BillboardedLockZ = EventObject.Node.BillboardedLockZ;
	ModelEventObject->Data().CameraAnchored = EventObject.Node.CameraAnchored;

	ModelEventObject->Data().GlobalSequenceId = EventObject.GlobalSequenceId;

	if(!Model.AddEventObject(ModelEventObject)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a helper
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreateHelper(CONST WAR3_MODEL_HELPER& Helper)
{
	MODEL_HELPER* ModelHelper;

	ModelHelper = new MODEL_HELPER();
	if(ModelHelper == NULL) return FALSE;

	ModelHelper->Data().Name = Helper.Node.Name;
	ModelHelper->Data().ObjectId = Helper.Node.ObjectId;
	ModelHelper->Data().ParentId = Helper.Node.ParentId;
	ModelHelper->Data().DontInheritTranslation = Helper.Node.DontInheritTranslation;
	ModelHelper->Data().DontInheritRotation = Helper.Node.DontInheritRotation;
	ModelHelper->Data().DontInheritScaling = Helper.Node.DontInheritScaling;
	ModelHelper->Data().Billboarded = Helper.Node.Billboarded;
	ModelHelper->Data().BillboardedLockX = Helper.Node.BillboardedLockX;
	ModelHelper->Data().BillboardedLockY = Helper.Node.BillboardedLockY;
	ModelHelper->Data().BillboardedLockZ = Helper.Node.BillboardedLockZ;
	ModelHelper->Data().CameraAnchored = Helper.Node.CameraAnchored;

	if(!Model.AddHelper(ModelHelper)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a light
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreateLight(CONST WAR3_MODEL_LIGHT& Light)
{
	MODEL_LIGHT* ModelLight;

	ModelLight = new MODEL_LIGHT();
	if(ModelLight == NULL) return FALSE;

	ModelLight->Data().Name = Light.Node.Name;
	ModelLight->Data().ObjectId = Light.Node.ObjectId;
	ModelLight->Data().ParentId = Light.Node.ParentId;
	ModelLight->Data().DontInheritTranslation = Light.Node.DontInheritTranslation;
	ModelLight->Data().DontInheritRotation = Light.Node.DontInheritRotation;
	ModelLight->Data().DontInheritScaling = Light.Node.DontInheritScaling;
	ModelLight->Data().Billboarded = Light.Node.Billboarded;
	ModelLight->Data().BillboardedLockX = Light.Node.BillboardedLockX;
	ModelLight->Data().BillboardedLockY = Light.Node.BillboardedLockY;
	ModelLight->Data().BillboardedLockZ = Light.Node.BillboardedLockZ;
	ModelLight->Data().CameraAnchored = Light.Node.CameraAnchored;

	ModelLight->Data().Type = Light.Type;

	ModelLight->Data().Visibility.SetStaticScalar(Light.Visibility);
	ModelLight->Data().AttenuationStart.SetStaticScalar(Light.AttenuationStart);
	ModelLight->Data().AttenuationEnd.SetStaticScalar(Light.AttenuationEnd);
	ModelLight->Data().Intensity.SetStaticScalar(Light.Intensity);
	ModelLight->Data().AmbientIntensity.SetStaticScalar(Light.AmbientIntensity);
	ModelLight->Data().Color.SetStaticVector3(D3DXVECTOR3(Light.Color.B, Light.Color.G, Light.Color.R));
	ModelLight->Data().AmbientColor.SetStaticVector3(D3DXVECTOR3(Light.AmbientColor.B, Light.AmbientColor.G, Light.AmbientColor.R));

	if(!Model.AddLight(ModelLight)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a particle emitter
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreateParticleEmitter(CONST WAR3_MODEL_PARTICLE_EMITTER& ParticleEmitter)
{
	MODEL_PARTICLE_EMITTER* ModelParticleEmitter;

	ModelParticleEmitter = new MODEL_PARTICLE_EMITTER();
	if(ModelParticleEmitter == NULL) return FALSE;

	ModelParticleEmitter->Data().Name = ParticleEmitter.Node.Name;
	ModelParticleEmitter->Data().ObjectId = ParticleEmitter.Node.ObjectId;
	ModelParticleEmitter->Data().ParentId = ParticleEmitter.Node.ParentId;
	ModelParticleEmitter->Data().DontInheritTranslation = ParticleEmitter.Node.DontInheritTranslation;
	ModelParticleEmitter->Data().DontInheritRotation = ParticleEmitter.Node.DontInheritRotation;
	ModelParticleEmitter->Data().DontInheritScaling = ParticleEmitter.Node.DontInheritScaling;
	ModelParticleEmitter->Data().Billboarded = ParticleEmitter.Node.Billboarded;
	ModelParticleEmitter->Data().BillboardedLockX = ParticleEmitter.Node.BillboardedLockX;
	ModelParticleEmitter->Data().BillboardedLockY = ParticleEmitter.Node.BillboardedLockY;
	ModelParticleEmitter->Data().BillboardedLockZ = ParticleEmitter.Node.BillboardedLockZ;
	ModelParticleEmitter->Data().CameraAnchored = ParticleEmitter.Node.CameraAnchored;

	ModelParticleEmitter->Data().EmissionRate.SetStaticScalar(ParticleEmitter.EmissionRate);
	ModelParticleEmitter->Data().Gravity.SetStaticScalar(ParticleEmitter.Gravity);
	ModelParticleEmitter->Data().Longitude.SetStaticScalar(ParticleEmitter.Longitude);
	ModelParticleEmitter->Data().Latitude.SetStaticScalar(ParticleEmitter.Latitude);
	ModelParticleEmitter->Data().Visibility.SetStaticScalar(ParticleEmitter.Visibility);

	ModelParticleEmitter->Data().ParticleFileName = ParticleEmitter.ParticleFileName;
	ModelParticleEmitter->Data().ParticleLifeSpan.SetStaticScalar(ParticleEmitter.ParticleLifeSpan);
	ModelParticleEmitter->Data().ParticleInitialVelocity.SetStaticScalar(ParticleEmitter.ParticleInitialVelocity);

	ModelParticleEmitter->Data().EmitterUsesMdl = ParticleEmitter.EmitterUsesMdl;
	ModelParticleEmitter->Data().EmitterUsesTga = ParticleEmitter.EmitterUsesTga;

	if(!Model.AddParticleEmitter(ModelParticleEmitter)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a particle emitter 2
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreateParticleEmitter2(CONST WAR3_MODEL_PARTICLE_EMITTER_2& ParticleEmitter2)
{
	MODEL_PARTICLE_EMITTER_2* ModelParticleEmitter2;

	ModelParticleEmitter2 = new MODEL_PARTICLE_EMITTER_2();
	if(ModelParticleEmitter2 == NULL) return FALSE;

	ModelParticleEmitter2->Data().Name = ParticleEmitter2.Node.Name;
	ModelParticleEmitter2->Data().ObjectId = ParticleEmitter2.Node.ObjectId;
	ModelParticleEmitter2->Data().ParentId = ParticleEmitter2.Node.ParentId;
	ModelParticleEmitter2->Data().DontInheritTranslation = ParticleEmitter2.Node.DontInheritTranslation;
	ModelParticleEmitter2->Data().DontInheritRotation = ParticleEmitter2.Node.DontInheritRotation;
	ModelParticleEmitter2->Data().DontInheritScaling = ParticleEmitter2.Node.DontInheritScaling;
	ModelParticleEmitter2->Data().Billboarded = ParticleEmitter2.Node.Billboarded;
	ModelParticleEmitter2->Data().BillboardedLockX = ParticleEmitter2.Node.BillboardedLockX;
	ModelParticleEmitter2->Data().BillboardedLockY = ParticleEmitter2.Node.BillboardedLockY;
	ModelParticleEmitter2->Data().BillboardedLockZ = ParticleEmitter2.Node.BillboardedLockZ;
	ModelParticleEmitter2->Data().CameraAnchored = ParticleEmitter2.Node.CameraAnchored;

	ModelParticleEmitter2->Data().FilterMode = static_cast<FILTER_MODE>(ParticleEmitter2.FilterMode);

	ModelParticleEmitter2->Data().Speed.SetStaticScalar(ParticleEmitter2.Speed);
	ModelParticleEmitter2->Data().Variation.SetStaticScalar(ParticleEmitter2.Variation);
	ModelParticleEmitter2->Data().Latitude.SetStaticScalar(ParticleEmitter2.Latitude);
	ModelParticleEmitter2->Data().Gravity.SetStaticScalar(ParticleEmitter2.Gravity);
	ModelParticleEmitter2->Data().Visibility.SetStaticScalar(ParticleEmitter2.Visibility);
	ModelParticleEmitter2->Data().EmissionRate.SetStaticScalar(ParticleEmitter2.EmissionRate);
	ModelParticleEmitter2->Data().Width.SetStaticScalar(ParticleEmitter2.Width);
	ModelParticleEmitter2->Data().Length.SetStaticScalar(ParticleEmitter2.Length);

	ModelParticleEmitter2->Data().SegmentColor1.x = ParticleEmitter2.Segment1.Color.B;
	ModelParticleEmitter2->Data().SegmentColor1.y = ParticleEmitter2.Segment1.Color.G;
	ModelParticleEmitter2->Data().SegmentColor1.z = ParticleEmitter2.Segment1.Color.R;
	ModelParticleEmitter2->Data().Alpha.x = ParticleEmitter2.Segment1.Alpha * 255.0f;
	ModelParticleEmitter2->Data().ParticleScaling.x = ParticleEmitter2.Segment1.Scaling;

	ModelParticleEmitter2->Data().SegmentColor2.x = ParticleEmitter2.Segment2.Color.B;
	ModelParticleEmitter2->Data().SegmentColor2.y = ParticleEmitter2.Segment2.Color.G;
	ModelParticleEmitter2->Data().SegmentColor2.z = ParticleEmitter2.Segment2.Color.R;
	ModelParticleEmitter2->Data().Alpha.y = ParticleEmitter2.Segment2.Alpha * 255.0f;
	ModelParticleEmitter2->Data().ParticleScaling.y = ParticleEmitter2.Segment2.Scaling;

	ModelParticleEmitter2->Data().SegmentColor2.x = ParticleEmitter2.Segment2.Color.B;
	ModelParticleEmitter2->Data().SegmentColor2.y = ParticleEmitter2.Segment2.Color.G;
	ModelParticleEmitter2->Data().SegmentColor2.z = ParticleEmitter2.Segment2.Color.R;
	ModelParticleEmitter2->Data().Alpha.z = ParticleEmitter2.Segment3.Alpha * 255.0f;
	ModelParticleEmitter2->Data().ParticleScaling.z = ParticleEmitter2.Segment3.Scaling;

	ModelParticleEmitter2->Data().HeadLifeSpan.x = static_cast<FLOAT>(ParticleEmitter2.HeadLifeSpan.Start);
	ModelParticleEmitter2->Data().HeadLifeSpan.y = static_cast<FLOAT>(ParticleEmitter2.HeadLifeSpan.End);
	ModelParticleEmitter2->Data().HeadLifeSpan.z = static_cast<FLOAT>(ParticleEmitter2.HeadLifeSpan.Repeat);
	ModelParticleEmitter2->Data().HeadDecay.x = static_cast<FLOAT>(ParticleEmitter2.HeadDecay.Start);
	ModelParticleEmitter2->Data().HeadDecay.y = static_cast<FLOAT>(ParticleEmitter2.HeadDecay.End);
	ModelParticleEmitter2->Data().HeadDecay.z = static_cast<FLOAT>(ParticleEmitter2.HeadDecay.Repeat);
	ModelParticleEmitter2->Data().TailLifeSpan.x = static_cast<FLOAT>(ParticleEmitter2.TailLifeSpan.Start);
	ModelParticleEmitter2->Data().TailLifeSpan.y = static_cast<FLOAT>(ParticleEmitter2.TailLifeSpan.End);
	ModelParticleEmitter2->Data().TailLifeSpan.z = static_cast<FLOAT>(ParticleEmitter2.TailLifeSpan.Repeat);
	ModelParticleEmitter2->Data().TailDecay.x = static_cast<FLOAT>(ParticleEmitter2.TailDecay.Start);
	ModelParticleEmitter2->Data().TailDecay.y = static_cast<FLOAT>(ParticleEmitter2.TailDecay.End);
	ModelParticleEmitter2->Data().TailDecay.z = static_cast<FLOAT>(ParticleEmitter2.TailDecay.Repeat);

	ModelParticleEmitter2->Data().Rows = ParticleEmitter2.Rows;
	ModelParticleEmitter2->Data().Columns = ParticleEmitter2.Columns;
	ModelParticleEmitter2->Data().TextureId = ParticleEmitter2.TextureId;
	ModelParticleEmitter2->Data().PriorityPlane = ParticleEmitter2.PriorityPlane;
	ModelParticleEmitter2->Data().ReplaceableId = ParticleEmitter2.ReplaceableId;
	ModelParticleEmitter2->Data().Time = ParticleEmitter2.Time;
	ModelParticleEmitter2->Data().LifeSpan = ParticleEmitter2.LifeSpan;
	ModelParticleEmitter2->Data().TailLength = ParticleEmitter2.TailLength;

	ModelParticleEmitter2->Data().SortPrimitivesFarZ = ParticleEmitter2.SortPrimitivesFarZ;
	ModelParticleEmitter2->Data().LineEmitter = ParticleEmitter2.LineEmitter;
	ModelParticleEmitter2->Data().ModelSpace = ParticleEmitter2.ModelSpace;
	ModelParticleEmitter2->Data().AlphaKey = ParticleEmitter2.AlphaKey;
	ModelParticleEmitter2->Data().Unshaded = ParticleEmitter2.Unshaded;
	ModelParticleEmitter2->Data().Unfogged = ParticleEmitter2.Unfogged;
	ModelParticleEmitter2->Data().XYQuad = ParticleEmitter2.XYQuad;
	ModelParticleEmitter2->Data().Squirt = ParticleEmitter2.Squirt;
	ModelParticleEmitter2->Data().Head = ParticleEmitter2.Head;
	ModelParticleEmitter2->Data().Tail = ParticleEmitter2.Tail;

	if(!Model.AddParticleEmitter2(ModelParticleEmitter2)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a ribbon emitter
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreateRibbonEmitter(CONST WAR3_MODEL_RIBBON_EMITTER& RibbonEmitter)
{
	MODEL_RIBBON_EMITTER* ModelRibbonEmitter;

	ModelRibbonEmitter = new MODEL_RIBBON_EMITTER();
	if(ModelRibbonEmitter == NULL) return FALSE;

	ModelRibbonEmitter->Data().Name = RibbonEmitter.Node.Name;
	ModelRibbonEmitter->Data().ObjectId = RibbonEmitter.Node.ObjectId;
	ModelRibbonEmitter->Data().ParentId = RibbonEmitter.Node.ParentId;
	ModelRibbonEmitter->Data().DontInheritTranslation = RibbonEmitter.Node.DontInheritTranslation;
	ModelRibbonEmitter->Data().DontInheritRotation = RibbonEmitter.Node.DontInheritRotation;
	ModelRibbonEmitter->Data().DontInheritScaling = RibbonEmitter.Node.DontInheritScaling;
	ModelRibbonEmitter->Data().Billboarded = RibbonEmitter.Node.Billboarded;
	ModelRibbonEmitter->Data().BillboardedLockX = RibbonEmitter.Node.BillboardedLockX;
	ModelRibbonEmitter->Data().BillboardedLockY = RibbonEmitter.Node.BillboardedLockY;
	ModelRibbonEmitter->Data().BillboardedLockZ = RibbonEmitter.Node.BillboardedLockZ;
	ModelRibbonEmitter->Data().CameraAnchored = RibbonEmitter.Node.CameraAnchored;

	ModelRibbonEmitter->Data().Color.SetStaticVector3(D3DXVECTOR3(RibbonEmitter.Color.B, RibbonEmitter.Color.G, RibbonEmitter.Color.R));

	ModelRibbonEmitter->Data().HeightAbove.SetStaticScalar(RibbonEmitter.HeightAbove);
	ModelRibbonEmitter->Data().HeightBelow.SetStaticScalar(RibbonEmitter.HeightBelow);
	ModelRibbonEmitter->Data().Alpha.SetStaticScalar(RibbonEmitter.Alpha);
	ModelRibbonEmitter->Data().TextureSlot.SetStaticScalar(RibbonEmitter.TextureSlot);
	ModelRibbonEmitter->Data().Visibility.SetStaticScalar(RibbonEmitter.Visibility);

	ModelRibbonEmitter->Data().EmissionRate = RibbonEmitter.EmissionRate;
	ModelRibbonEmitter->Data().LifeSpan = RibbonEmitter.LifeSpan;
	ModelRibbonEmitter->Data().Gravity = RibbonEmitter.Gravity;
	ModelRibbonEmitter->Data().Rows = RibbonEmitter.Rows;
	ModelRibbonEmitter->Data().Columns = RibbonEmitter.Columns;
	ModelRibbonEmitter->Data().MaterialId = RibbonEmitter.MaterialId;

	if(!Model.AddRibbonEmitter(ModelRibbonEmitter)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a camera
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreateCamera(CONST WAR3_MODEL_CAMERA& Camera)
{
	MODEL_CAMERA* ModelCamera;

	ModelCamera = new MODEL_CAMERA();
	if(ModelCamera == NULL) return FALSE;

	ModelCamera->Data().Name = Camera.Name;

	ModelCamera->Data().Source.x = Camera.Position.X;
	ModelCamera->Data().Source.y = Camera.Position.Y;
	ModelCamera->Data().Source.z = Camera.Position.Z;
	ModelCamera->Data().Target.x = Camera.TargetPosition.X;
	ModelCamera->Data().Target.y = Camera.TargetPosition.Y;
	ModelCamera->Data().Target.z = Camera.TargetPosition.Z;

	ModelCamera->Data().FieldOfView = Camera.FieldOfView;
	ModelCamera->Data().NearDistance = Camera.NearDistance;
	ModelCamera->Data().FarDistance = Camera.FarDistance;

	if(!Model.AddCamera(ModelCamera)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a geoset
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreateGeoset(CONST WAR3_MODEL_GEOSET& Geoset)
{
	MODEL_GEOSET* ModelGeoset;

	ModelGeoset = new MODEL_GEOSET();
	if(ModelGeoset == NULL) return FALSE;

	ModelGeoset->Data().MaterialId = Geoset.MaterialId;
	ModelGeoset->Data().SelectionGroup = Geoset.SelectionGroup;
	ModelGeoset->Data().Unselectable = Geoset.Unselectable;

	ModelGeoset->Data().Extent.Radius = Geoset.Extent.Radius;
	ModelGeoset->Data().Extent.Min.x = Geoset.Extent.Min.X;
	ModelGeoset->Data().Extent.Min.y = Geoset.Extent.Min.Y;
	ModelGeoset->Data().Extent.Min.z = Geoset.Extent.Min.Z;
	ModelGeoset->Data().Extent.Max.x = Geoset.Extent.Max.X;
	ModelGeoset->Data().Extent.Max.y = Geoset.Extent.Max.Y;
	ModelGeoset->Data().Extent.Max.z = Geoset.Extent.Max.Z;

	if(!Model.AddGeoset(ModelGeoset)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a geoset animation
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreateGeosetAnimation(CONST WAR3_MODEL_GEOSET_ANIMATION& GeosetAnimation)
{
	MODEL_GEOSET_ANIMATION* ModelGeosetAnimation;

	ModelGeosetAnimation = new MODEL_GEOSET_ANIMATION();
	if(ModelGeosetAnimation == NULL) return FALSE;

	ModelGeosetAnimation->Data().GeosetId = GeosetAnimation.GeosetId;

	ModelGeosetAnimation->Data().Color.SetStaticVector3(D3DXVECTOR3(GeosetAnimation.Color.B, GeosetAnimation.Color.G, GeosetAnimation.Color.R));
	ModelGeosetAnimation->Data().Alpha.SetStaticScalar(GeosetAnimation.Alpha);

	ModelGeosetAnimation->Data().UseColor = GeosetAnimation.UseColor;
	ModelGeosetAnimation->Data().DropShadow = GeosetAnimation.DropShadow;

	if(!Model.AddGeosetAnimation(ModelGeosetAnimation)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a global sequence
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreateGlobalSequence(CONST WAR3_MODEL_GLOBAL_SEQUENCE& GlobalSequence)
{
	MODEL_GLOBAL_SEQUENCE* ModelGlobalSequence;

	ModelGlobalSequence = new MODEL_GLOBAL_SEQUENCE();
	if(ModelGlobalSequence == NULL) return FALSE;

	ModelGlobalSequence->Data().Duration = GlobalSequence.Duration;

	if(!Model.AddGlobalSequence(ModelGlobalSequence)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a material
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreateMaterial(CONST WAR3_MODEL_MATERIAL& Material)
{
	MODEL_MATERIAL* ModelMaterial;

	ModelMaterial = new MODEL_MATERIAL();
	if(ModelMaterial == NULL) return FALSE;

	ModelMaterial->Data().PriorityPlane = Material.PriorityPlane;

	ModelMaterial->Data().ConstantColor = Material.ConstantColor;
	ModelMaterial->Data().SortPrimitivesFarZ = Material.SortPrimitivesFarZ;
	ModelMaterial->Data().FullResolution = Material.FullResolution;

	if(!Model.AddMaterial(ModelMaterial)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a pivot point
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreatePivotPoint(CONST WAR3_MODEL_PIVOT_POINT& PivotPoint)
{
	D3DXVECTOR3* ModelPivotPoint;

	ModelPivotPoint = new D3DXVECTOR3();
	if(ModelPivotPoint == NULL) return FALSE;

	ModelPivotPoint->x = PivotPoint.Position.X;
	ModelPivotPoint->y = PivotPoint.Position.Y;
	ModelPivotPoint->z = PivotPoint.Position.Z;

	if(!Model.AddPivotPoint(ModelPivotPoint)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a sequence
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreateSequence(CONST WAR3_MODEL_SEQUENCE& Sequence)
{
	MODEL_SEQUENCE* ModelSequence;

	ModelSequence = new MODEL_SEQUENCE();
	if(ModelSequence == NULL) return FALSE;

	ModelSequence->Data().Name = Sequence.Name;

	ModelSequence->Data().Rarity = Sequence.Rarity;
	ModelSequence->Data().MoveSpeed = Sequence.MoveSpeed;
	ModelSequence->Data().NonLooping = Sequence.NonLooping;
	ModelSequence->Data().Interval.x = static_cast<FLOAT>(Sequence.IntervalStart);
	ModelSequence->Data().Interval.y = static_cast<FLOAT>(Sequence.IntervalEnd);

	ModelSequence->Data().Extent.Radius = Sequence.Extent.Radius;
	ModelSequence->Data().Extent.Min.x = Sequence.Extent.Min.X;
	ModelSequence->Data().Extent.Min.y = Sequence.Extent.Min.Y;
	ModelSequence->Data().Extent.Min.z = Sequence.Extent.Min.Z;
	ModelSequence->Data().Extent.Max.x = Sequence.Extent.Max.X;
	ModelSequence->Data().Extent.Max.y = Sequence.Extent.Max.Y;
	ModelSequence->Data().Extent.Max.z = Sequence.Extent.Max.Z;

	if(!Model.AddSequence(ModelSequence)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a texture
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreateTexture(CONST WAR3_MODEL_TEXTURE& Texture)
{
	std::string FileName;
	MODEL_TEXTURE* ModelTexture;

	FileName = Texture.FileName;
	if(FileName != "")
	{
		if(FileName.substr(0, 2) == ".\\") FileName.erase(0, 2);

		if(!TextureManager.Load(FileName))
		{
			FileName = Common.GetFileName(FileName);
			if(!TextureManager.Load(FileName))
			{
				return FALSE;
			}
		}
	}

	Error.ClearMessage();

	ModelTexture = new MODEL_TEXTURE();
	if(ModelTexture == NULL) return FALSE;

	ModelTexture->Data().FileName = FileName;

	ModelTexture->Data().ReplaceableId = Texture.ReplaceableId;

	ModelTexture->Data().WrapWidth = Texture.WrapWidth;
	ModelTexture->Data().WrapHeight = Texture.WrapHeight;

	if(!Model.AddTexture(ModelTexture)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a texture animation
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreateTextureAnimation(CONST WAR3_MODEL_TEXTURE_ANIMATION& TextureAnimation)
{
	MODEL_TEXTURE_ANIMATION* ModelTextureAnimation;

	ModelTextureAnimation = new MODEL_TEXTURE_ANIMATION();
	if(ModelTextureAnimation == NULL) return FALSE;

	if(!Model.AddTextureAnimation(ModelTextureAnimation)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates an event object track
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreateEventObjectTrack(INT EventObjectId, INT Time)
{
	MODEL_EVENT_OBJECT* ModelEventObject;

	if(!Model.Data().EventObjectContainer.ValidIndex(EventObjectId))
	{
		std::stringstream Stream;
		Stream << "Unable to create an event object track, event object ID " << EventObjectId << " does not exist!";
		Error.SetMessage(Stream.str());
		return FALSE;
	}

	ModelEventObject = Model.Data().EventObjectContainer[EventObjectId];

	ModelEventObject->Data().EventTrack.push_back(Time);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a geoset vertex
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreateGeosetVertex(INT GeosetId, CONST WAR3_MODEL_VERTEX& Vertex)
{
	MODEL_GEOSET* ModelGeoset;
	MODEL_GEOSET_VERTEX* ModelGeosetVertex;

	if(!Model.Data().GeosetContainer.ValidIndex(GeosetId))
	{
		std::stringstream Stream;
		Stream << "Unable to create a geoset vertex, geoset ID " << GeosetId << " does not exist!";
		Error.SetMessage(Stream.str());
		return FALSE;
	}

	ModelGeoset = Model.Data().GeosetContainer[GeosetId];

	ModelGeosetVertex = new MODEL_GEOSET_VERTEX();
	if(ModelGeosetVertex == NULL) return FALSE;

	ModelGeosetVertex->Position.x = Vertex.Position.X;
	ModelGeosetVertex->Position.y = Vertex.Position.Y;
	ModelGeosetVertex->Position.z = Vertex.Position.Z;
	ModelGeosetVertex->Normal.x = Vertex.Normal.X;
	ModelGeosetVertex->Normal.y = Vertex.Normal.Y;
	ModelGeosetVertex->Normal.z = Vertex.Normal.Z;
	ModelGeosetVertex->TexturePosition.x = Vertex.TexturePosition.X;
	ModelGeosetVertex->TexturePosition.y = Vertex.TexturePosition.Y;
	ModelGeosetVertex->VertexGroup = Vertex.Group;

	if(!ModelGeoset->AddVertex(ModelGeosetVertex)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a geoset face
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreateGeosetFace(INT GeosetId, CONST WAR3_MODEL_FACE& Face)
{
	MODEL_GEOSET* ModelGeoset;
	MODEL_GEOSET_FACE* ModelGeosetFace;

	if(!Model.Data().GeosetContainer.ValidIndex(GeosetId))
	{
		std::stringstream Stream;
		Stream << "Unable to create a geoset vertex, geoset ID " << GeosetId << " does not exist!";
		Error.SetMessage(Stream.str());
		return FALSE;
	}

	ModelGeoset = Model.Data().GeosetContainer[GeosetId];

	ModelGeosetFace = new MODEL_GEOSET_FACE();
	if(ModelGeosetFace == NULL) return FALSE;

	ModelGeosetFace->Index1 = Face.Index1;
	ModelGeosetFace->Index2 = Face.Index2;
	ModelGeosetFace->Index3 = Face.Index3;

	if(!ModelGeoset->AddFace(ModelGeosetFace)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a geoset extent
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreateGeosetExtent(INT GeosetId, CONST WAR3_MODEL_EXTENT& Extent)
{
	MODEL_GEOSET* ModelGeoset;
	EXTENT* ModelExtent;

	if(!Model.Data().GeosetContainer.ValidIndex(GeosetId))
	{
		std::stringstream Stream;
		Stream << "Unable to create a geoset vertex, geoset ID " << GeosetId << " does not exist!";
		Error.SetMessage(Stream.str());
		return FALSE;
	}

	ModelGeoset = Model.Data().GeosetContainer[GeosetId];

	ModelExtent = new EXTENT();
	if(ModelExtent == NULL) return FALSE;

	ModelExtent->Radius = Extent.Radius;
	ModelExtent->Min.x = Extent.Min.X;
	ModelExtent->Min.y = Extent.Min.Y;
	ModelExtent->Min.z = Extent.Min.Z;
	ModelExtent->Max.x = Extent.Max.X;
	ModelExtent->Max.y = Extent.Max.Y;
	ModelExtent->Max.z = Extent.Max.Z;

	if(!ModelGeoset->AddExtent(ModelExtent)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a geoset group
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreateGeosetGroup(INT GeosetId)
{
	MODEL_GEOSET* ModelGeoset;
	MODEL_GEOSET_GROUP* ModelGeosetGroup;

	if(!Model.Data().GeosetContainer.ValidIndex(GeosetId))
	{
		std::stringstream Stream;
		Stream << "Unable to create a geoset vertex, geoset ID " << GeosetId << " does not exist!";
		Error.SetMessage(Stream.str());
		return FALSE;
	}

	ModelGeoset = Model.Data().GeosetContainer[GeosetId];

	ModelGeosetGroup = new MODEL_GEOSET_GROUP();
	if(ModelGeosetGroup == NULL) return FALSE;

	if(!ModelGeoset->AddGroup(ModelGeosetGroup)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a geoset group bone
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreateGeosetGroupBone(INT GeosetId, INT GeosetGroupId, INT BoneId)
{
	MODEL_GEOSET* ModelGeoset;
	MODEL_GEOSET_GROUP* ModelGeosetGroup;
	MODEL_GEOSET_GROUP_NODE* ModelGeosetGroupNode;

	if(!Model.Data().GeosetContainer.ValidIndex(GeosetId))
	{
		std::stringstream Stream;
		Stream << "Unable to create a geoset group bone, geoset ID " << GeosetId << " does not exist!";
		Error.SetMessage(Stream.str());
		return FALSE;
	}

	ModelGeoset = Model.Data().GeosetContainer[GeosetId];

	if(!ModelGeoset->Data().GroupContainer.ValidIndex(GeosetGroupId))
	{
		std::stringstream Stream;
		Stream << "Unable to create a geoset group bone, geoset group ID " << GeosetGroupId << " in geoset ID " << GeosetId << " does not exist!";
		Error.SetMessage(Stream.str());
		return FALSE;
	}

	ModelGeosetGroup = ModelGeoset->Data().GroupContainer[GeosetGroupId];

	ModelGeosetGroupNode = new MODEL_GEOSET_GROUP_NODE();
	if(ModelGeosetGroupNode == NULL) return FALSE;

	ModelGeosetGroupNode->NodeId = BoneId;

	if(!ModelGeosetGroup->MatrixList.Add(ModelGeosetGroupNode)) return FALSE;

	ModelGeosetGroup->MatrixListSize++;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a material layer
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreateMaterialLayer(INT MaterialId, CONST WAR3_MODEL_MATERIAL_LAYER& MaterialLayer)
{
	MODEL_MATERIAL* ModelMaterial;
	MODEL_MATERIAL_LAYER* ModelMaterialLayer;

	if(!Model.Data().MaterialContainer.ValidIndex(MaterialId))
	{
		std::stringstream Stream;
		Stream << "Unable to create a material layer, material ID " << MaterialId << " does not exist!";
		Error.SetMessage(Stream.str());
		return FALSE;
	}

	ModelMaterial = Model.Data().MaterialContainer[MaterialId];

	ModelMaterialLayer = new MODEL_MATERIAL_LAYER();
	if(ModelMaterialLayer == NULL) return FALSE;

	ModelMaterialLayer->Data().FilterMode = static_cast<FILTER_MODE>(MaterialLayer.FilterMode);

	ModelMaterialLayer->Data().TextureId = MaterialLayer.TextureId;
	ModelMaterialLayer->Data().TextureAnimationId = MaterialLayer.TextureAnimationId;

	ModelMaterialLayer->Data().AnimatedTextureId.SetStaticScalarInt(MaterialLayer.TextureId);
	ModelMaterialLayer->Data().Alpha.SetStaticScalar(MaterialLayer.Alpha);

	ModelMaterialLayer->Data().Unshaded = MaterialLayer.Unshaded;
	ModelMaterialLayer->Data().Unfogged = MaterialLayer.Unfogged;
	ModelMaterialLayer->Data().TwoSided = MaterialLayer.TwoSided;
	ModelMaterialLayer->Data().SphereEnvironmentMap = MaterialLayer.SphereEnvironmentMap;
	ModelMaterialLayer->Data().NoDepthTest = MaterialLayer.NoDepthTest;
	ModelMaterialLayer->Data().NoDepthSet = MaterialLayer.NoDepthSet;

	if(!ModelMaterial->AddLayer(ModelMaterialLayer)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates an interpolator
//+-----------------------------------------------------------------------------
BOOL MODEL_BUILDER::CreateInterpolator()
{
	CurrentInterpolator = INTERPOLATOR();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Sets the interpolator type
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetInterpolatorType(WAR3_INTERPOLATOR_TYPE InterpolatorType)
{
	CurrentInterpolator.SetType(static_cast<INTERPOLATOR_TYPE>(InterpolatorType));
}


//+-----------------------------------------------------------------------------
//| Sets the interpolator interpolation type
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetInterpolatorInterpolationType(WAR3_INTERPOLATION_TYPE InterpolationType)
{
	CurrentInterpolator.SetInterpolationType(static_cast<INTERPOLATION_TYPE>(InterpolationType));
}


//+-----------------------------------------------------------------------------
//| Sets the interpolator global sequence ID
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetInterpolatorGlobalSequenceId(INT GlobalSequenceId)
{
	CurrentInterpolator.SetGlobalSequenceId(GlobalSequenceId);
}


//+-----------------------------------------------------------------------------
//| Adds a scalar to the interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::AddScalar(INT Time, FLOAT Scalar, FLOAT InTan, FLOAT OutTan)
{
	INTERPOLATOR_NODE Node;

	Node.Time = Time;
	Node.Vector.x = Scalar;
	Node.InTan.x = InTan;
	Node.OutTan.x = OutTan;

	CurrentInterpolator.AddNode(Node);
}


//+-----------------------------------------------------------------------------
//| Adds an integer scalar to the interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::AddScalarInt(INT Time, INT Scalar, INT InTan, INT OutTan)
{
	INTERPOLATOR_NODE Node;

	Node.Time = Time;
	Node.Vector.x = static_cast<FLOAT>(Scalar);
	Node.InTan.x = static_cast<FLOAT>(InTan);
	Node.OutTan.x = static_cast<FLOAT>(OutTan);

	CurrentInterpolator.AddNode(Node);
}


//+-----------------------------------------------------------------------------
//| Adds a 2-dimensional vector to the interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::AddVector2(INT Time, CONST WAR3_MODEL_VECTOR_2& Vector, CONST WAR3_MODEL_VECTOR_2& InTan, CONST WAR3_MODEL_VECTOR_2& OutTan)
{
	INTERPOLATOR_NODE Node;

	Node.Time = Time;
	Node.Vector.x = static_cast<FLOAT>(Vector.X);
	Node.Vector.y = static_cast<FLOAT>(Vector.Y);
	Node.InTan.x = static_cast<FLOAT>(InTan.X);
	Node.InTan.y = static_cast<FLOAT>(InTan.Y);
	Node.OutTan.x = static_cast<FLOAT>(OutTan.X);
	Node.OutTan.y = static_cast<FLOAT>(OutTan.Y);

	CurrentInterpolator.AddNode(Node);
}


//+-----------------------------------------------------------------------------
//| Adds a 3-dimensional vector to the interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::AddVector3(INT Time, CONST WAR3_MODEL_VECTOR_3& Vector, CONST WAR3_MODEL_VECTOR_3& InTan, CONST WAR3_MODEL_VECTOR_3& OutTan)
{
	INTERPOLATOR_NODE Node;

	Node.Time = Time;
	Node.Vector.x = static_cast<FLOAT>(Vector.X);
	Node.Vector.y = static_cast<FLOAT>(Vector.Y);
	Node.Vector.z = static_cast<FLOAT>(Vector.Z);
	Node.InTan.x = static_cast<FLOAT>(InTan.X);
	Node.InTan.y = static_cast<FLOAT>(InTan.Y);
	Node.InTan.z = static_cast<FLOAT>(InTan.Z);
	Node.OutTan.x = static_cast<FLOAT>(OutTan.X);
	Node.OutTan.y = static_cast<FLOAT>(OutTan.Y);
	Node.OutTan.z = static_cast<FLOAT>(OutTan.Z);

	CurrentInterpolator.AddNode(Node);
}


//+-----------------------------------------------------------------------------
//| Adds a 4-dimensional vector to the interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::AddVector4(INT Time, CONST WAR3_MODEL_VECTOR_4& Vector, CONST WAR3_MODEL_VECTOR_4& InTan, CONST WAR3_MODEL_VECTOR_4& OutTan)
{
	INTERPOLATOR_NODE Node;

	Node.Time = Time;
	Node.Vector.x = static_cast<FLOAT>(Vector.X);
	Node.Vector.y = static_cast<FLOAT>(Vector.Y);
	Node.Vector.z = static_cast<FLOAT>(Vector.Z);
	Node.Vector.w = static_cast<FLOAT>(Vector.W);
	Node.InTan.x = static_cast<FLOAT>(InTan.X);
	Node.InTan.y = static_cast<FLOAT>(InTan.Y);
	Node.InTan.z = static_cast<FLOAT>(InTan.Z);
	Node.InTan.w = static_cast<FLOAT>(InTan.W);
	Node.OutTan.x = static_cast<FLOAT>(OutTan.X);
	Node.OutTan.y = static_cast<FLOAT>(OutTan.Y);
	Node.OutTan.z = static_cast<FLOAT>(OutTan.Z);
	Node.OutTan.w = static_cast<FLOAT>(OutTan.W);

	CurrentInterpolator.AddNode(Node);
}


//+-----------------------------------------------------------------------------
//| Sets the translation to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedAttachmentTranslation(INT AttachmentId)
{
	MODEL_ATTACHMENT* ModelAttachment;

	if(!Model.Data().AttachmentContainer.ValidIndex(AttachmentId)) return;
	ModelAttachment = Model.Data().AttachmentContainer[AttachmentId];

	ModelAttachment->Data().Translation = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the rotation to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedAttachmentRotation(INT AttachmentId)
{
	MODEL_ATTACHMENT* ModelAttachment;

	if(!Model.Data().AttachmentContainer.ValidIndex(AttachmentId)) return;
	ModelAttachment = Model.Data().AttachmentContainer[AttachmentId];

	ModelAttachment->Data().Rotation = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the scaling to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedAttachmentScaling(INT AttachmentId)
{
	MODEL_ATTACHMENT* ModelAttachment;

	if(!Model.Data().AttachmentContainer.ValidIndex(AttachmentId)) return;
	ModelAttachment = Model.Data().AttachmentContainer[AttachmentId];

	ModelAttachment->Data().Scaling = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the visibility to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedAttachmentVisibility(INT AttachmentId)
{
	MODEL_ATTACHMENT* ModelAttachment;

	if(!Model.Data().AttachmentContainer.ValidIndex(AttachmentId)) return;
	ModelAttachment = Model.Data().AttachmentContainer[AttachmentId];

	ModelAttachment->Data().Visibility = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the translation to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedBoneTranslation(INT BoneId)
{
	MODEL_BONE* ModelBone;

	if(!Model.Data().BoneContainer.ValidIndex(BoneId)) return;
	ModelBone = Model.Data().BoneContainer[BoneId];

	ModelBone->Data().Translation = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the rotation to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedBoneRotation(INT BoneId)
{
	MODEL_BONE* ModelBone;

	if(!Model.Data().BoneContainer.ValidIndex(BoneId)) return;
	ModelBone = Model.Data().BoneContainer[BoneId];

	ModelBone->Data().Rotation = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the scaling to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedBoneScaling(INT BoneId)
{
	MODEL_BONE* ModelBone;

	if(!Model.Data().BoneContainer.ValidIndex(BoneId)) return;
	ModelBone = Model.Data().BoneContainer[BoneId];

	ModelBone->Data().Scaling = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the translation to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedCollisionShapeTranslation(INT CollisionShapeId)
{
	MODEL_COLLISION_SHAPE* ModelCollisionShape;

	if(!Model.Data().CollisionShapeContainer.ValidIndex(CollisionShapeId)) return;
	ModelCollisionShape = Model.Data().CollisionShapeContainer[CollisionShapeId];

	ModelCollisionShape->Data().Translation = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the rotation to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedCollisionShapeRotation(INT CollisionShapeId)
{
	MODEL_COLLISION_SHAPE* ModelCollisionShape;

	if(!Model.Data().CollisionShapeContainer.ValidIndex(CollisionShapeId)) return;
	ModelCollisionShape = Model.Data().CollisionShapeContainer[CollisionShapeId];

	ModelCollisionShape->Data().Rotation = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the scaling to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedCollisionShapeScaling(INT CollisionShapeId)
{
	MODEL_COLLISION_SHAPE* ModelCollisionShape;

	if(!Model.Data().CollisionShapeContainer.ValidIndex(CollisionShapeId)) return;
	ModelCollisionShape = Model.Data().CollisionShapeContainer[CollisionShapeId];

	ModelCollisionShape->Data().Scaling = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the translation to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedEventObjectTranslation(INT EventObjectId)
{
	MODEL_EVENT_OBJECT* ModelEventObject;

	if(!Model.Data().EventObjectContainer.ValidIndex(EventObjectId)) return;
	ModelEventObject = Model.Data().EventObjectContainer[EventObjectId];

	ModelEventObject->Data().Translation = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the rotation to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedEventObjectRotation(INT EventObjectId)
{
	MODEL_EVENT_OBJECT* ModelEventObject;

	if(!Model.Data().EventObjectContainer.ValidIndex(EventObjectId)) return;
	ModelEventObject = Model.Data().EventObjectContainer[EventObjectId];

	ModelEventObject->Data().Rotation = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the scaling to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedEventObjectScaling(INT EventObjectId)
{
	MODEL_EVENT_OBJECT* ModelEventObject;

	if(!Model.Data().EventObjectContainer.ValidIndex(EventObjectId)) return;
	ModelEventObject = Model.Data().EventObjectContainer[EventObjectId];

	ModelEventObject->Data().Scaling = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the translation to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedHelperTranslation(INT HelperId)
{
	MODEL_HELPER* ModelHelper;

	if(!Model.Data().HelperContainer.ValidIndex(HelperId)) return;
	ModelHelper = Model.Data().HelperContainer[HelperId];

	ModelHelper->Data().Translation = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the rotation to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedHelperRotation(INT HelperId)
{
	MODEL_HELPER* ModelHelper;

	if(!Model.Data().HelperContainer.ValidIndex(HelperId)) return;
	ModelHelper = Model.Data().HelperContainer[HelperId];

	ModelHelper->Data().Rotation = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the scaling to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedHelperScaling(INT HelperId)
{
	MODEL_HELPER* ModelHelper;

	if(!Model.Data().HelperContainer.ValidIndex(HelperId)) return;
	ModelHelper = Model.Data().HelperContainer[HelperId];

	ModelHelper->Data().Scaling = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the translation to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedLightTranslation(INT LightId)
{
	MODEL_LIGHT* ModelLight;

	if(!Model.Data().LightContainer.ValidIndex(LightId)) return;
	ModelLight = Model.Data().LightContainer[LightId];

	ModelLight->Data().Translation = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the rotation to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedLightRotation(INT LightId)
{
	MODEL_LIGHT* ModelLight;

	if(!Model.Data().LightContainer.ValidIndex(LightId)) return;
	ModelLight = Model.Data().LightContainer[LightId];

	ModelLight->Data().Rotation = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the scaling to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedLightScaling(INT LightId)
{
	MODEL_LIGHT* ModelLight;

	if(!Model.Data().LightContainer.ValidIndex(LightId)) return;
	ModelLight = Model.Data().LightContainer[LightId];

	ModelLight->Data().Scaling = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the attenuation start to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedLightAttenuationStart(INT LightId)
{
	MODEL_LIGHT* ModelLight;

	if(!Model.Data().LightContainer.ValidIndex(LightId)) return;
	ModelLight = Model.Data().LightContainer[LightId];

	ModelLight->Data().AttenuationStart = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the attenuation end to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedLightAttenuationEnd(INT LightId)
{
	MODEL_LIGHT* ModelLight;

	if(!Model.Data().LightContainer.ValidIndex(LightId)) return;
	ModelLight = Model.Data().LightContainer[LightId];

	ModelLight->Data().AttenuationEnd = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the color to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedLightColor(INT LightId)
{
	MODEL_LIGHT* ModelLight;

	if(!Model.Data().LightContainer.ValidIndex(LightId)) return;
	ModelLight = Model.Data().LightContainer[LightId];

	ModelLight->Data().Color = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the intensity to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedLightIntensity(INT LightId)
{
	MODEL_LIGHT* ModelLight;

	if(!Model.Data().LightContainer.ValidIndex(LightId)) return;
	ModelLight = Model.Data().LightContainer[LightId];

	ModelLight->Data().Intensity = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the ambient color to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedLightAmbientColor(INT LightId)
{
	MODEL_LIGHT* ModelLight;

	if(!Model.Data().LightContainer.ValidIndex(LightId)) return;
	ModelLight = Model.Data().LightContainer[LightId];

	ModelLight->Data().AmbientColor = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the ambient intensity to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedLightAmbientIntensity(INT LightId)
{
	MODEL_LIGHT* ModelLight;

	if(!Model.Data().LightContainer.ValidIndex(LightId)) return;
	ModelLight = Model.Data().LightContainer[LightId];

	ModelLight->Data().AmbientIntensity = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the visibility to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedLightVisibility(INT LightId)
{
	MODEL_LIGHT* ModelLight;

	if(!Model.Data().LightContainer.ValidIndex(LightId)) return;
	ModelLight = Model.Data().LightContainer[LightId];

	ModelLight->Data().Visibility = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the translation to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedParticleEmitterTranslation(INT ParticleEmitterId)
{
	MODEL_PARTICLE_EMITTER* ModelParticleEmitter;

	if(!Model.Data().ParticleEmitterContainer.ValidIndex(ParticleEmitterId)) return;
	ModelParticleEmitter = Model.Data().ParticleEmitterContainer[ParticleEmitterId];

	ModelParticleEmitter->Data().Translation = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the rotation to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedParticleEmitterRotation(INT ParticleEmitterId)
{
	MODEL_PARTICLE_EMITTER* ModelParticleEmitter;

	if(!Model.Data().ParticleEmitterContainer.ValidIndex(ParticleEmitterId)) return;
	ModelParticleEmitter = Model.Data().ParticleEmitterContainer[ParticleEmitterId];

	ModelParticleEmitter->Data().Rotation = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the scaling to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedParticleEmitterScaling(INT ParticleEmitterId)
{
	MODEL_PARTICLE_EMITTER* ModelParticleEmitter;

	if(!Model.Data().ParticleEmitterContainer.ValidIndex(ParticleEmitterId)) return;
	ModelParticleEmitter = Model.Data().ParticleEmitterContainer[ParticleEmitterId];

	ModelParticleEmitter->Data().Scaling = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the emission rate to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedParticleEmitterEmissionRate(INT ParticleEmitterId)
{
	MODEL_PARTICLE_EMITTER* ModelParticleEmitter;

	if(!Model.Data().ParticleEmitterContainer.ValidIndex(ParticleEmitterId)) return;
	ModelParticleEmitter = Model.Data().ParticleEmitterContainer[ParticleEmitterId];

	ModelParticleEmitter->Data().EmissionRate = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the gravity to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedParticleEmitterGravity(INT ParticleEmitterId)
{
	MODEL_PARTICLE_EMITTER* ModelParticleEmitter;

	if(!Model.Data().ParticleEmitterContainer.ValidIndex(ParticleEmitterId)) return;
	ModelParticleEmitter = Model.Data().ParticleEmitterContainer[ParticleEmitterId];

	ModelParticleEmitter->Data().Gravity = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the longitude to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedParticleEmitterLongitude(INT ParticleEmitterId)
{
	MODEL_PARTICLE_EMITTER* ModelParticleEmitter;

	if(!Model.Data().ParticleEmitterContainer.ValidIndex(ParticleEmitterId)) return;
	ModelParticleEmitter = Model.Data().ParticleEmitterContainer[ParticleEmitterId];

	ModelParticleEmitter->Data().Longitude = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the latitude to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedParticleEmitterLatitude(INT ParticleEmitterId)
{
	MODEL_PARTICLE_EMITTER* ModelParticleEmitter;

	if(!Model.Data().ParticleEmitterContainer.ValidIndex(ParticleEmitterId)) return;
	ModelParticleEmitter = Model.Data().ParticleEmitterContainer[ParticleEmitterId];

	ModelParticleEmitter->Data().Latitude = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the visibility to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedParticleEmitterVisibility(INT ParticleEmitterId)
{
	MODEL_PARTICLE_EMITTER* ModelParticleEmitter;

	if(!Model.Data().ParticleEmitterContainer.ValidIndex(ParticleEmitterId)) return;
	ModelParticleEmitter = Model.Data().ParticleEmitterContainer[ParticleEmitterId];

	ModelParticleEmitter->Data().Visibility = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the lifespan to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedParticleEmitterParticleLifeSpan(INT ParticleEmitterId)
{
	MODEL_PARTICLE_EMITTER* ModelParticleEmitter;

	if(!Model.Data().ParticleEmitterContainer.ValidIndex(ParticleEmitterId)) return;
	ModelParticleEmitter = Model.Data().ParticleEmitterContainer[ParticleEmitterId];

	ModelParticleEmitter->Data().ParticleLifeSpan = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the velocity to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedParticleEmitterParticleInitialVelocity(INT ParticleEmitterId)
{
	MODEL_PARTICLE_EMITTER* ModelParticleEmitter;

	if(!Model.Data().ParticleEmitterContainer.ValidIndex(ParticleEmitterId)) return;
	ModelParticleEmitter = Model.Data().ParticleEmitterContainer[ParticleEmitterId];

	ModelParticleEmitter->Data().ParticleInitialVelocity = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the translation to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedParticleEmitter2Translation(INT ParticleEmitter2Id)
{
	MODEL_PARTICLE_EMITTER_2* ModelParticleEmitter2;

	if(!Model.Data().ParticleEmitter2Container.ValidIndex(ParticleEmitter2Id)) return;
	ModelParticleEmitter2 = Model.Data().ParticleEmitter2Container[ParticleEmitter2Id];

	ModelParticleEmitter2->Data().Translation = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the rotation to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedParticleEmitter2Rotation(INT ParticleEmitter2Id)
{
	MODEL_PARTICLE_EMITTER_2* ModelParticleEmitter2;

	if(!Model.Data().ParticleEmitter2Container.ValidIndex(ParticleEmitter2Id)) return;
	ModelParticleEmitter2 = Model.Data().ParticleEmitter2Container[ParticleEmitter2Id];

	ModelParticleEmitter2->Data().Rotation = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the scaling to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedParticleEmitter2Scaling(INT ParticleEmitter2Id)
{
	MODEL_PARTICLE_EMITTER_2* ModelParticleEmitter2;

	if(!Model.Data().ParticleEmitter2Container.ValidIndex(ParticleEmitter2Id)) return;
	ModelParticleEmitter2 = Model.Data().ParticleEmitter2Container[ParticleEmitter2Id];

	ModelParticleEmitter2->Data().Scaling = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the speed to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedParticleEmitter2Speed(INT ParticleEmitter2Id)
{
	MODEL_PARTICLE_EMITTER_2* ModelParticleEmitter2;

	if(!Model.Data().ParticleEmitter2Container.ValidIndex(ParticleEmitter2Id)) return;
	ModelParticleEmitter2 = Model.Data().ParticleEmitter2Container[ParticleEmitter2Id];

	ModelParticleEmitter2->Data().Speed = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the variation to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedParticleEmitter2Variation(INT ParticleEmitter2Id)
{
	MODEL_PARTICLE_EMITTER_2* ModelParticleEmitter2;

	if(!Model.Data().ParticleEmitter2Container.ValidIndex(ParticleEmitter2Id)) return;
	ModelParticleEmitter2 = Model.Data().ParticleEmitter2Container[ParticleEmitter2Id];

	ModelParticleEmitter2->Data().Variation = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the latitude to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedParticleEmitter2Latitude(INT ParticleEmitter2Id)
{
	MODEL_PARTICLE_EMITTER_2* ModelParticleEmitter2;

	if(!Model.Data().ParticleEmitter2Container.ValidIndex(ParticleEmitter2Id)) return;
	ModelParticleEmitter2 = Model.Data().ParticleEmitter2Container[ParticleEmitter2Id];

	ModelParticleEmitter2->Data().Latitude = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the gravity to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedParticleEmitter2Gravity(INT ParticleEmitter2Id)
{
	MODEL_PARTICLE_EMITTER_2* ModelParticleEmitter2;

	if(!Model.Data().ParticleEmitter2Container.ValidIndex(ParticleEmitter2Id)) return;
	ModelParticleEmitter2 = Model.Data().ParticleEmitter2Container[ParticleEmitter2Id];

	ModelParticleEmitter2->Data().Gravity = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the visibility to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedParticleEmitter2Visibility(INT ParticleEmitter2Id)
{
	MODEL_PARTICLE_EMITTER_2* ModelParticleEmitter2;

	if(!Model.Data().ParticleEmitter2Container.ValidIndex(ParticleEmitter2Id)) return;
	ModelParticleEmitter2 = Model.Data().ParticleEmitter2Container[ParticleEmitter2Id];

	ModelParticleEmitter2->Data().Visibility = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the emission rate to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedParticleEmitter2EmissionRate(INT ParticleEmitter2Id)
{
	MODEL_PARTICLE_EMITTER_2* ModelParticleEmitter2;

	if(!Model.Data().ParticleEmitter2Container.ValidIndex(ParticleEmitter2Id)) return;
	ModelParticleEmitter2 = Model.Data().ParticleEmitter2Container[ParticleEmitter2Id];

	ModelParticleEmitter2->Data().EmissionRate = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the width to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedParticleEmitter2Width(INT ParticleEmitter2Id)
{
	MODEL_PARTICLE_EMITTER_2* ModelParticleEmitter2;

	if(!Model.Data().ParticleEmitter2Container.ValidIndex(ParticleEmitter2Id)) return;
	ModelParticleEmitter2 = Model.Data().ParticleEmitter2Container[ParticleEmitter2Id];

	ModelParticleEmitter2->Data().Width = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the length to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedParticleEmitter2Length(INT ParticleEmitter2Id)
{
	MODEL_PARTICLE_EMITTER_2* ModelParticleEmitter2;

	if(!Model.Data().ParticleEmitter2Container.ValidIndex(ParticleEmitter2Id)) return;
	ModelParticleEmitter2 = Model.Data().ParticleEmitter2Container[ParticleEmitter2Id];

	ModelParticleEmitter2->Data().Length = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the translation to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedRibbonEmitterTranslation(INT RibbonEmitterId)
{
	MODEL_RIBBON_EMITTER* ModelRibbonEmitter;

	if(!Model.Data().RibbonEmitterContainer.ValidIndex(RibbonEmitterId)) return;
	ModelRibbonEmitter = Model.Data().RibbonEmitterContainer[RibbonEmitterId];

	ModelRibbonEmitter->Data().Translation = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the rotation to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedRibbonEmitterRotation(INT RibbonEmitterId)
{
	MODEL_RIBBON_EMITTER* ModelRibbonEmitter;

	if(!Model.Data().RibbonEmitterContainer.ValidIndex(RibbonEmitterId)) return;
	ModelRibbonEmitter = Model.Data().RibbonEmitterContainer[RibbonEmitterId];

	ModelRibbonEmitter->Data().Rotation = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the scaling to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedRibbonEmitterScaling(INT RibbonEmitterId)
{
	MODEL_RIBBON_EMITTER* ModelRibbonEmitter;

	if(!Model.Data().RibbonEmitterContainer.ValidIndex(RibbonEmitterId)) return;
	ModelRibbonEmitter = Model.Data().RibbonEmitterContainer[RibbonEmitterId];

	ModelRibbonEmitter->Data().Scaling = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the height above to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedRibbonEmitterHeightAbove(INT RibbonEmitterId)
{
	MODEL_RIBBON_EMITTER* ModelRibbonEmitter;

	if(!Model.Data().RibbonEmitterContainer.ValidIndex(RibbonEmitterId)) return;
	ModelRibbonEmitter = Model.Data().RibbonEmitterContainer[RibbonEmitterId];

	ModelRibbonEmitter->Data().HeightAbove = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the height below to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedRibbonEmitterHeightBelow(INT RibbonEmitterId)
{
	MODEL_RIBBON_EMITTER* ModelRibbonEmitter;

	if(!Model.Data().RibbonEmitterContainer.ValidIndex(RibbonEmitterId)) return;
	ModelRibbonEmitter = Model.Data().RibbonEmitterContainer[RibbonEmitterId];

	ModelRibbonEmitter->Data().HeightBelow = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the alpha to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedRibbonEmitterAlpha(INT RibbonEmitterId)
{
	MODEL_RIBBON_EMITTER* ModelRibbonEmitter;

	if(!Model.Data().RibbonEmitterContainer.ValidIndex(RibbonEmitterId)) return;
	ModelRibbonEmitter = Model.Data().RibbonEmitterContainer[RibbonEmitterId];

	ModelRibbonEmitter->Data().Alpha = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the color to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedRibbonEmitterColor(INT RibbonEmitterId)
{
	MODEL_RIBBON_EMITTER* ModelRibbonEmitter;

	if(!Model.Data().RibbonEmitterContainer.ValidIndex(RibbonEmitterId)) return;
	ModelRibbonEmitter = Model.Data().RibbonEmitterContainer[RibbonEmitterId];

	ModelRibbonEmitter->Data().Color = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the texture slot to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedRibbonEmitterTextureSlot(INT RibbonEmitterId)
{
	MODEL_RIBBON_EMITTER* ModelRibbonEmitter;

	if(!Model.Data().RibbonEmitterContainer.ValidIndex(RibbonEmitterId)) return;
	ModelRibbonEmitter = Model.Data().RibbonEmitterContainer[RibbonEmitterId];

	ModelRibbonEmitter->Data().TextureSlot = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the visibility to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedRibbonEmitterVisibility(INT RibbonEmitterId)
{
	MODEL_RIBBON_EMITTER* ModelRibbonEmitter;

	if(!Model.Data().RibbonEmitterContainer.ValidIndex(RibbonEmitterId)) return;
	ModelRibbonEmitter = Model.Data().RibbonEmitterContainer[RibbonEmitterId];

	ModelRibbonEmitter->Data().Visibility = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the translation to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedCameraSourceTranslation(INT CameraId)
{
	MODEL_CAMERA* ModelCamera;

	if(!Model.Data().CameraContainer.ValidIndex(CameraId)) return;
	ModelCamera = Model.Data().CameraContainer[CameraId];

	ModelCamera->Data().SourceTranslation = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the translation to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedCameraTargetTranslation(INT CameraId)
{
	MODEL_CAMERA* ModelCamera;

	if(!Model.Data().CameraContainer.ValidIndex(CameraId)) return;
	ModelCamera = Model.Data().CameraContainer[CameraId];

	ModelCamera->Data().TargetTranslation = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the rotation to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedCameraRotation(INT CameraId)
{
	MODEL_CAMERA* ModelCamera;

	if(!Model.Data().CameraContainer.ValidIndex(CameraId)) return;
	ModelCamera = Model.Data().CameraContainer[CameraId];

	ModelCamera->Data().Rotation = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the color to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedGeosetAnimationColor(INT GeosetAnimationId)
{
	MODEL_GEOSET_ANIMATION* ModelGeosetAnimation;

	if(!Model.Data().GeosetAnimationContainer.ValidIndex(GeosetAnimationId)) return;
	ModelGeosetAnimation = Model.Data().GeosetAnimationContainer[GeosetAnimationId];

	ModelGeosetAnimation->Data().Color = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the alpha to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedGeosetAnimationAlpha(INT GeosetAnimationId)
{
	MODEL_GEOSET_ANIMATION* ModelGeosetAnimation;

	if(!Model.Data().GeosetAnimationContainer.ValidIndex(GeosetAnimationId)) return;
	ModelGeosetAnimation = Model.Data().GeosetAnimationContainer[GeosetAnimationId];

	ModelGeosetAnimation->Data().Alpha = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the alpha to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedMaterialLayerAlpha(INT MaterialId, INT MaterialLayerId)
{
	MODEL_MATERIAL* ModelMaterial;
	MODEL_MATERIAL_LAYER* ModelMaterialLayer;

	if(!Model.Data().MaterialContainer.ValidIndex(MaterialId)) return;
	ModelMaterial = Model.Data().MaterialContainer[MaterialId];

	if(!ModelMaterial->Data().LayerContainer.ValidIndex(MaterialLayerId)) return;
	ModelMaterialLayer = ModelMaterial->Data().LayerContainer[MaterialLayerId];

	ModelMaterialLayer->Data().Alpha = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the texture ID to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedMaterialLayerTextureId(INT MaterialId, INT MaterialLayerId)
{
	MODEL_MATERIAL* ModelMaterial;
	MODEL_MATERIAL_LAYER* ModelMaterialLayer;

	if(!Model.Data().MaterialContainer.ValidIndex(MaterialId)) return;
	ModelMaterial = Model.Data().MaterialContainer[MaterialId];

	if(!ModelMaterial->Data().LayerContainer.ValidIndex(MaterialLayerId)) return;
	ModelMaterialLayer = ModelMaterial->Data().LayerContainer[MaterialLayerId];

	ModelMaterialLayer->Data().AnimatedTextureId = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the translation to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedTextureAnimationTranslation(INT TextureAnimationId)
{
	MODEL_TEXTURE_ANIMATION* ModelTextureAnimation;

	if(!Model.Data().TextureAnimationContainer.ValidIndex(TextureAnimationId)) return;
	ModelTextureAnimation = Model.Data().TextureAnimationContainer[TextureAnimationId];

	ModelTextureAnimation->Data().Translation = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the rotation to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedTextureAnimationRotation(INT TextureAnimationId)
{
	MODEL_TEXTURE_ANIMATION* ModelTextureAnimation;

	if(!Model.Data().TextureAnimationContainer.ValidIndex(TextureAnimationId)) return;
	ModelTextureAnimation = Model.Data().TextureAnimationContainer[TextureAnimationId];

	ModelTextureAnimation->Data().Rotation = CurrentInterpolator;
}


//+-----------------------------------------------------------------------------
//| Sets the sclaing to the last created interpolator
//+-----------------------------------------------------------------------------
VOID MODEL_BUILDER::SetAnimatedTextureAnimationScaling(INT TextureAnimationId)
{
	MODEL_TEXTURE_ANIMATION* ModelTextureAnimation;

	if(!Model.Data().TextureAnimationContainer.ValidIndex(TextureAnimationId)) return;
	ModelTextureAnimation = Model.Data().TextureAnimationContainer[TextureAnimationId];

	ModelTextureAnimation->Data().Scaling = CurrentInterpolator;
}
