//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Model.h"
#include "AnimationWindow.h"
#include "NodeManagerWindow.h"
#include "CameraManagerWindow.h"
#include "GeosetManagerWindow.h"
#include "GeosetAnimationManagerWindow.h"
#include "TextureManagerWindow.h"
#include "TextureAnimationManagerWindow.h"
#include "MaterialManagerWindow.h"
#include "SequenceManagerWindow.h"
#include "GlobalSequenceManagerWindow.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
MODEL Model;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL::MODEL()
{
	BoundsRadius = 0.0f;
	BoundsCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	SequenceNode.SetData(this);

	Root = new MODEL_HELPER();
	Root->BaseData()->TreeViewItem = TVI_ROOT;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL::~MODEL()
{
	delete Root;
}


//+-----------------------------------------------------------------------------
//| Clears the model
//+-----------------------------------------------------------------------------
VOID MODEL::Clear()
{
	ParticleManager.Clear();
	RibbonManager.Clear();

	ObjectIdManager.ClearAllObjectIds();

	ModelData.BaseContainer.Clear();

	SAFE_CLEAR(ModelData.AttachmentContainer);

	SAFE_CLEAR(ModelData.BoneContainer);
	SAFE_CLEAR(ModelData.CollisionShapeContainer);
	SAFE_CLEAR(ModelData.EventObjectContainer);
	SAFE_CLEAR(ModelData.HelperContainer);
	SAFE_CLEAR(ModelData.LightContainer);
	SAFE_CLEAR(ModelData.ParticleEmitterContainer);
	SAFE_CLEAR(ModelData.ParticleEmitter2Container);
	SAFE_CLEAR(ModelData.RibbonEmitterContainer);

	SAFE_CLEAR(ModelData.CameraContainer);
	SAFE_CLEAR(ModelData.GeosetContainer);
	SAFE_CLEAR(ModelData.GeosetAnimationContainer);
	SAFE_CLEAR(ModelData.GlobalSequenceContainer);
	SAFE_CLEAR(ModelData.MaterialContainer);
	SAFE_CLEAR(ModelData.SequenceContainer);
	SAFE_CLEAR(ModelData.TextureContainer);
	SAFE_CLEAR(ModelData.PivotPointContainer);
	SAFE_CLEAR(ModelData.TextureAnimationContainer);


	BoundsRadius = 0.0f;
	BoundsCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	AnimationTime = SEQUENCE_TIME();

	ModelData = MODEL_DATA();
}


//+-----------------------------------------------------------------------------
//| Returns a reference to the data
//+-----------------------------------------------------------------------------
MODEL_DATA& MODEL::Data()
{
	return ModelData;
}


//+-----------------------------------------------------------------------------
//| Rebuilds the meshes next time it's rendered
//+-----------------------------------------------------------------------------
VOID MODEL::Rebuild()
{
	INT i;

	for(i = 0; i < ModelData.GeosetContainer.GetTotalSize(); i++)
	{
		if(ModelData.GeosetContainer.ValidIndex(i))
		{
			ModelData.GeosetContainer[i]->Rebuild();
		}
	}
}


//+-----------------------------------------------------------------------------
//| Renders the model
//+-----------------------------------------------------------------------------
VOID MODEL::Render(INT TimeDifference)
{
	INT i;
	INT Interval;
	INT RealTimeDifference;
	BOOL Animated;
	MODEL_SEQUENCE* Sequence;
	MODEL_GLOBAL_SEQUENCE* GlobalSequence;
	std::list<INT> RenderOrderList1;
	std::list<INT> RenderOrderList2;
	std::list<INT> RenderOrderList3;
	std::list<INT> RenderOrderList4;
	std::list<INT>::iterator j;

	RealTimeDifference = TimeDifference;

	while(SequenceNode.IsAttached() && (TimeDifference != 0))
	{
		Sequence = SequenceNode.GetObjectData();
		Interval = AnimationTime.IntervalEnd - AnimationTime.IntervalStart;

		if(Interval < 0)
		{
			SequenceNode.Detach();
			break;
		}

		AnimationTime.Time += TimeDifference;
		if(AnimationTime.Time > AnimationTime.IntervalEnd)
		{
			if(Properties().Minor.AlwaysLoop)
			{
				AnimationTime.Time = AnimationTime.IntervalStart;
			}
			else if(Properties().Minor.NeverLoop)
			{
				AnimationTime.Time = AnimationTime.IntervalEnd;
				//TimeDifference = 0;
			}
			else if(Sequence->Data().NonLooping)
			{
				AnimationTime.Time = AnimationTime.IntervalEnd;
				//TimeDifference = 0;
			}
			else
			{
				AnimationTime.Time = AnimationTime.IntervalStart;
			}
		}

		for(i = 0; i < ModelData.GlobalSequenceContainer.GetTotalSize(); i++)
		{
			if(ModelData.GlobalSequenceContainer.ValidIndex(i))
			{
				GlobalSequence = ModelData.GlobalSequenceContainer[i];

				GlobalSequence->Data().Time += TimeDifference;
				if(GlobalSequence->Data().Time >= GlobalSequence->Data().Duration)
				{
					GlobalSequence->Data().Time = 0;
				}
			}
		}

		ParticleManager.Update(AnimationTime, RealTimeDifference, (TimeDifference > 0));
		RibbonManager.Update(AnimationTime, RealTimeDifference, (TimeDifference > 0));

		Root->BuildMatrix(AnimationTime);

		break;
	}

	for(i = 0; i < ModelData.GeosetContainer.GetTotalSize(); i++)
	{
		if(ModelData.GeosetContainer.ValidIndex(i))
		{
			switch(ModelData.GeosetContainer[i]->GetRenderOrder())
			{
				case 1:
				{
					RenderOrderList1.push_back(i);
					break;
				}

				case 2:
				{
					RenderOrderList2.push_back(i);
					break;
				}

				case 3:
				{
					RenderOrderList3.push_back(i);
					break;
				}

				case 4:
				{
					RenderOrderList4.push_back(i);
					break;
				}
			}
		}
	}

	Animated = SequenceNode.IsAttached();

	j = RenderOrderList1.begin();
	while(j != RenderOrderList1.end())
	{
		ModelData.GeosetContainer[*j]->Render(AnimationTime, Animated);
		j++;
	}

	j = RenderOrderList2.begin();
	while(j != RenderOrderList2.end())
	{
		ModelData.GeosetContainer[*j]->Render(AnimationTime, Animated);
		j++;
	}

	j = RenderOrderList3.begin();
	while(j != RenderOrderList3.end())
	{
		ModelData.GeosetContainer[*j]->Render(AnimationTime, Animated);
		j++;
	}

	j = RenderOrderList4.begin();
	while(j != RenderOrderList4.end())
	{
		ModelData.GeosetContainer[*j]->Render(AnimationTime, Animated);
		j++;
	}

	Graphics.PrepareForLines();

	if(Properties().ShowBones) RenderBones();
	if(Properties().ShowLights) RenderLights();
	if(Properties().ShowCameras) RenderCameras();
	if(Properties().ShowAttachments) RenderAttachments();
	if(Properties().ShowCollisionShapes) RenderCollisionShapes();
}


//+-----------------------------------------------------------------------------
//| Plays an animation
//+-----------------------------------------------------------------------------
VOID MODEL::PlayAnimation(MODEL_SEQUENCE* Sequence)
{
	INT i;

	AnimationTime = SEQUENCE_TIME();

	for(i = 0; i < ModelData.GlobalSequenceContainer.GetTotalSize(); i++)
	{
		if(ModelData.GlobalSequenceContainer.ValidIndex(i))
		{
			ModelData.GlobalSequenceContainer[i]->Data().Time = 0;
		}
	}

	SequenceNode.Detach();

	ParticleManager.Clear();
	RibbonManager.Clear();

	if(Sequence == NULL) return;

	SequenceNode.Attach(Sequence->ModelNodes);
	AnimationTime.IntervalStart = static_cast<INT>(Sequence->Data().Interval.x);
	AnimationTime.IntervalEnd = static_cast<INT>(Sequence->Data().Interval.y);
	AnimationTime.Time = AnimationTime.IntervalStart;
}


//+-----------------------------------------------------------------------------
//| Calculates the bounds radius
//+-----------------------------------------------------------------------------
VOID MODEL::CalculateBoundsRadius()
{
	INT i;
	INT j;
	MODEL_GEOSET* Geoset;
	std::vector<D3DXVECTOR3> VertexList;

	for(i = 0; i < ModelData.GeosetContainer.GetTotalSize(); i++)
	{
		if(ModelData.GeosetContainer.ValidIndex(i))
		{
			Geoset = ModelData.GeosetContainer[i];
			VertexList.reserve(static_cast<INT>(VertexList.size()) + Geoset->Data().VertexContainer.GetSize());

			for(j = 0; j < Geoset->Data().VertexContainer.GetTotalSize(); j++)
			{
				if(Geoset->Data().VertexContainer.ValidIndex(j))
				{
					VertexList.push_back(Geoset->Data().VertexContainer[j]->Position);
				}
			}
		}
	}

	if(static_cast<INT>(VertexList.size()) > 0)
	{
		if(FAILED(D3DXComputeBoundingSphere(&VertexList[0], static_cast<INT>(VertexList.size()), sizeof(D3DXVECTOR3), &BoundsCenter, &BoundsRadius)))
		{
			BoundsRadius = 0.0f;
			BoundsCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}
	else
	{
		BoundsRadius = 0.0f;
		BoundsCenter = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}


//+-----------------------------------------------------------------------------
//| Returns the radius of the bounding sphere
//+-----------------------------------------------------------------------------
FLOAT MODEL::GetBoundsRadius()
{
	return BoundsRadius;
}


//+-----------------------------------------------------------------------------
//| Returns the center of the bounding sphere
//+-----------------------------------------------------------------------------
D3DXVECTOR3 MODEL::GetBoundsCenter()
{
	return BoundsCenter;
}


//+-----------------------------------------------------------------------------
//| Adds an attachment
//+-----------------------------------------------------------------------------
MODEL_GEOSET* MODEL::GetIntersectedGeoset(CONST D3DXVECTOR3& RayPosition, CONST D3DXVECTOR3& RayDirection)
{
	INT i;
	BOOL Animated;
	FLOAT CurrentDistance;
	FLOAT ClosestDistance;
	MODEL_GEOSET* CurrentGeoset;
	MODEL_GEOSET* ClosestGeoset;

	ClosestGeoset = NULL;
	//ClosestDistance = std::numeric_limits<FLOAT>::max();
	Animated = SequenceNode.IsAttached();

	for(i = 0; i < ModelData.GeosetContainer.GetTotalSize(); i++)
	{
		if(ModelData.GeosetContainer.ValidIndex(i))
		{
			CurrentGeoset = ModelData.GeosetContainer[i];

			if(CurrentGeoset->RayIntersects(RayPosition, RayDirection, Animated, &CurrentDistance))
			{
				if(CurrentDistance < ClosestDistance)
				{
					ClosestGeoset = CurrentGeoset;
					ClosestDistance = CurrentDistance;
				}
			}
		}
	}

	return ClosestGeoset;
}


//+-----------------------------------------------------------------------------
//| Adds an attachment
//+-----------------------------------------------------------------------------
BOOL MODEL::AddAttachment(MODEL_ATTACHMENT* Attachment, MODEL_BASE* ParentNode)
{
	if(!AddBaseNode(Attachment, ParentNode)) return FALSE;

	if(!ModelData.AttachmentContainer.Add(Attachment))
	{
		Error.SetMessage("Unable to add a new attachment!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a bone
//+-----------------------------------------------------------------------------
BOOL MODEL::AddBone(MODEL_BONE* Bone, MODEL_BASE* ParentNode)
{
	if(!AddBaseNode(Bone, ParentNode)) return FALSE;

	if(!ModelData.BoneContainer.Add(Bone))
	{
		Error.SetMessage("Unable to add a new bone!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a collision shape
//+-----------------------------------------------------------------------------
BOOL MODEL::AddCollisionShape(MODEL_COLLISION_SHAPE* CollisionShape, MODEL_BASE* ParentNode)
{
	if(!AddBaseNode(CollisionShape, ParentNode)) return FALSE;

	if(!ModelData.CollisionShapeContainer.Add(CollisionShape))
	{
		Error.SetMessage("Unable to add a new collision shape!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds an event object
//+-----------------------------------------------------------------------------
BOOL MODEL::AddEventObject(MODEL_EVENT_OBJECT* EventObject, MODEL_BASE* ParentNode)
{
	if(!AddBaseNode(EventObject, ParentNode)) return FALSE;

	if(!ModelData.EventObjectContainer.Add(EventObject))
	{
		Error.SetMessage("Unable to add a new event object!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a helper
//+-----------------------------------------------------------------------------
BOOL MODEL::AddHelper(MODEL_HELPER* Helper, MODEL_BASE* ParentNode)
{
	if(!AddBaseNode(Helper, ParentNode)) return FALSE;

	if(!ModelData.HelperContainer.Add(Helper))
	{
		Error.SetMessage("Unable to add a new helper!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a light
//+-----------------------------------------------------------------------------
BOOL MODEL::AddLight(MODEL_LIGHT* Light, MODEL_BASE* ParentNode)
{
	if(!AddBaseNode(Light, ParentNode)) return FALSE;

	if(!ModelData.LightContainer.Add(Light))
	{
		Error.SetMessage("Unable to add a new light!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a particle emitter
//+-----------------------------------------------------------------------------
BOOL MODEL::AddParticleEmitter(MODEL_PARTICLE_EMITTER* ParticleEmitter, MODEL_BASE* ParentNode)
{
	if(!AddBaseNode(ParticleEmitter, ParentNode)) return FALSE;

	if(!ModelData.ParticleEmitterContainer.Add(ParticleEmitter))
	{
		Error.SetMessage("Unable to add a new particle emitter!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a particle emitter 2
//+-----------------------------------------------------------------------------
BOOL MODEL::AddParticleEmitter2(MODEL_PARTICLE_EMITTER_2* ParticleEmitter2, MODEL_BASE* ParentNode)
{
	if(!AddBaseNode(ParticleEmitter2, ParentNode)) return FALSE;

	if(!ModelData.ParticleEmitter2Container.Add(ParticleEmitter2))
	{
		Error.SetMessage("Unable to add a new particle emitter 2!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a ribbon emitter
//+-----------------------------------------------------------------------------
BOOL MODEL::AddRibbonEmitter(MODEL_RIBBON_EMITTER* RibbonEmitter, MODEL_BASE* ParentNode)
{
	if(!AddBaseNode(RibbonEmitter, ParentNode)) return FALSE;

	if(!ModelData.RibbonEmitterContainer.Add(RibbonEmitter))
	{
		Error.SetMessage("Unable to add a new ribbon emitter!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a camera
//+-----------------------------------------------------------------------------
BOOL MODEL::AddCamera(MODEL_CAMERA* Camera)
{
	if(!ModelData.CameraContainer.Add(Camera))
	{
		Error.SetMessage("Unable to add a new camera!");
		return FALSE;
	}

	CameraManagerWindow.AddName(reinterpret_cast<INT>(Camera), Camera->Data().Name);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a geoset
//+-----------------------------------------------------------------------------
BOOL MODEL::AddGeoset(MODEL_GEOSET* Geoset, BOOL Imported)
{
	std::stringstream Stream;

	if(!ModelData.GeosetContainer.Add(Geoset))
	{
		Error.SetMessage("Unable to add a new geoset!");
		return FALSE;
	}

	if(Imported) Stream << "Imported";
	Stream << "Geoset " << Geoset;
	GeosetManagerWindow.AddName(reinterpret_cast<INT>(Geoset), Stream.str());

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a geoset animation
//+-----------------------------------------------------------------------------
BOOL MODEL::AddGeosetAnimation(MODEL_GEOSET_ANIMATION* GeosetAnimation)
{
	std::stringstream Stream;

	if(!ModelData.GeosetAnimationContainer.Add(GeosetAnimation))
	{
		Error.SetMessage("Unable to add a new geoset animation!");
		return FALSE;
	}

	Stream << "GeosetAnimation " << GeosetAnimation;
	GeosetAnimationManagerWindow.AddName(reinterpret_cast<INT>(GeosetAnimation), Stream.str());

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a global sequence
//+-----------------------------------------------------------------------------
BOOL MODEL::AddGlobalSequence(MODEL_GLOBAL_SEQUENCE* GlobalSequence)
{
	std::stringstream Stream;

	if(!ModelData.GlobalSequenceContainer.Add(GlobalSequence))
	{
		Error.SetMessage("Unable to add a new global sequence!");
		return FALSE;
	}

	Stream << "GlobalSequence " << GlobalSequence;
	GlobalSequenceManagerWindow.AddName(reinterpret_cast<INT>(GlobalSequence), Stream.str());

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a material
//+-----------------------------------------------------------------------------
BOOL MODEL::AddMaterial(MODEL_MATERIAL* Material)
{
	std::stringstream Stream;

	if(!ModelData.MaterialContainer.Add(Material))
	{
		Error.SetMessage("Unable to add a new material!");
		return FALSE;
	}

	Stream << "Material " << Material;
	MaterialManagerWindow.AddName(reinterpret_cast<INT>(Material), Stream.str());

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a pivot point
//+-----------------------------------------------------------------------------
BOOL MODEL::AddPivotPoint(D3DXVECTOR3* PivotPoint)
{
	if(!ModelData.PivotPointContainer.Add(PivotPoint))
	{
		Error.SetMessage("Unable to add a new PivotPoint!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a sequence
//+-----------------------------------------------------------------------------
BOOL MODEL::AddSequence(MODEL_SEQUENCE* Sequence)
{
	if(!ModelData.SequenceContainer.Add(Sequence))
	{
		Error.SetMessage("Unable to add a new sequence!");
		return FALSE;
	}

	AnimationWindow.AddAnimation(reinterpret_cast<INT>(Sequence), Sequence->Data().Name);
	SequenceManagerWindow.AddName(reinterpret_cast<INT>(Sequence), Sequence->Data().Name);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a texture
//+-----------------------------------------------------------------------------
BOOL MODEL::AddTexture(MODEL_TEXTURE* Texture)
{
	std::stringstream Stream;

	if(!ModelData.TextureContainer.Add(Texture))
	{
		Error.SetMessage("Unable to add a new texture!");
		return FALSE;
	}

	if(Texture->Data().FileName == "")
	{
		Stream << "Replaceable ID " << Texture->Data().ReplaceableId;
	}
	else
	{
		Stream << Texture->Data().FileName;
	}

	TextureManagerWindow.AddName(reinterpret_cast<INT>(Texture), Stream.str());

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Adds a texture animation
//+-----------------------------------------------------------------------------
BOOL MODEL::AddTextureAnimation(MODEL_TEXTURE_ANIMATION* TextureAnimation)
{
	std::stringstream Stream;

	if(!ModelData.TextureAnimationContainer.Add(TextureAnimation))
	{
		Error.SetMessage("Unable to add a new texture animation!");
		return FALSE;
	}

	Stream << "TextureAnimation " << TextureAnimation;
	TextureAnimationManagerWindow.AddName(reinterpret_cast<INT>(TextureAnimation), Stream.str());

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes an attachment
//+-----------------------------------------------------------------------------
BOOL MODEL::RemoveAttachment(MODEL_ATTACHMENT* Attachment, HWND Window)
{
	INT i;

	if(!RemoveBaseNode(Attachment, Window)) return FALSE;

	for(i = 0; i < ModelData.AttachmentContainer.GetTotalSize(); i++)
	{
		if(ModelData.AttachmentContainer.ValidIndex(i))
		{
			if(ModelData.AttachmentContainer[i] == Attachment)
			{
				ModelData.AttachmentContainer.Remove(i);
				break;
			}
		}
	}

	delete Attachment;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes a bone
//+-----------------------------------------------------------------------------
BOOL MODEL::RemoveBone(MODEL_BONE* Bone, HWND Window)
{
	INT i;

	if(!RemoveBaseNode(Bone, Window)) return FALSE;

	for(i = 0; i < ModelData.BoneContainer.GetTotalSize(); i++)
	{
		if(ModelData.BoneContainer.ValidIndex(i))
		{
			if(ModelData.BoneContainer[i] == Bone)
			{
				ModelData.BoneContainer.Remove(i);
				break;
			}
		}
	}

	delete Bone;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes a collision shape
//+-----------------------------------------------------------------------------
BOOL MODEL::RemoveCollisionShape(MODEL_COLLISION_SHAPE* CollisionShape, HWND Window)
{
	INT i;

	if(!RemoveBaseNode(CollisionShape, Window)) return FALSE;

	for(i = 0; i < ModelData.CollisionShapeContainer.GetTotalSize(); i++)
	{
		if(ModelData.CollisionShapeContainer.ValidIndex(i))
		{
			if(ModelData.CollisionShapeContainer[i] == CollisionShape)
			{
				ModelData.CollisionShapeContainer.Remove(i);
				break;
			}
		}
	}

	delete CollisionShape;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes an event object
//+-----------------------------------------------------------------------------
BOOL MODEL::RemoveEventObject(MODEL_EVENT_OBJECT* EventObject, HWND Window)
{
	INT i;

	if(!RemoveBaseNode(EventObject, Window)) return FALSE;

	for(i = 0; i < ModelData.EventObjectContainer.GetTotalSize(); i++)
	{
		if(ModelData.EventObjectContainer.ValidIndex(i))
		{
			if(ModelData.EventObjectContainer[i] == EventObject)
			{
				ModelData.EventObjectContainer.Remove(i);
				break;
			}
		}
	}

	delete EventObject;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes a helper
//+-----------------------------------------------------------------------------
BOOL MODEL::RemoveHelper(MODEL_HELPER* Helper, HWND Window)
{
	INT i;

	if(!RemoveBaseNode(Helper, Window)) return FALSE;

	for(i = 0; i < ModelData.HelperContainer.GetTotalSize(); i++)
	{
		if(ModelData.HelperContainer.ValidIndex(i))
		{
			if(ModelData.HelperContainer[i] == Helper)
			{
				ModelData.HelperContainer.Remove(i);
				break;
			}
		}
	}

	delete Helper;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes a light
//+-----------------------------------------------------------------------------
BOOL MODEL::RemoveLight(MODEL_LIGHT* Light, HWND Window)
{
	INT i;

	if(!RemoveBaseNode(Light, Window)) return FALSE;

	for(i = 0; i < ModelData.LightContainer.GetTotalSize(); i++)
	{
		if(ModelData.LightContainer.ValidIndex(i))
		{
			if(ModelData.LightContainer[i] == Light)
			{
				ModelData.LightContainer.Remove(i);
				break;
			}
		}
	}

	delete Light;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes a particle emitter
//+-----------------------------------------------------------------------------
BOOL MODEL::RemoveParticleEmitter(MODEL_PARTICLE_EMITTER* ParticleEmitter, HWND Window)
{
	INT i;

	if(!RemoveBaseNode(ParticleEmitter, Window)) return FALSE;

	for(i = 0; i < ModelData.ParticleEmitterContainer.GetTotalSize(); i++)
	{
		if(ModelData.ParticleEmitterContainer.ValidIndex(i))
		{
			if(ModelData.ParticleEmitterContainer[i] == ParticleEmitter)
			{
				ModelData.ParticleEmitterContainer.Remove(i);
				break;
			}
		}
	}

	delete ParticleEmitter;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes a particle emitter 2
//+-----------------------------------------------------------------------------
BOOL MODEL::RemoveParticleEmitter2(MODEL_PARTICLE_EMITTER_2* ParticleEmitter2, HWND Window)
{
	INT i;

	if(!RemoveBaseNode(ParticleEmitter2, Window)) return FALSE;

	for(i = 0; i < ModelData.ParticleEmitter2Container.GetTotalSize(); i++)
	{
		if(ModelData.ParticleEmitter2Container.ValidIndex(i))
		{
			if(ModelData.ParticleEmitter2Container[i] == ParticleEmitter2)
			{
				ModelData.ParticleEmitter2Container.Remove(i);
				break;
			}
		}
	}

	delete ParticleEmitter2;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes a ribbon emitter
//+-----------------------------------------------------------------------------
BOOL MODEL::RemoveRibbonEmitter(MODEL_RIBBON_EMITTER* RibbonEmitter, HWND Window)
{
	INT i;

	if(!RemoveBaseNode(RibbonEmitter, Window)) return FALSE;

	for(i = 0; i < ModelData.RibbonEmitterContainer.GetTotalSize(); i++)
	{
		if(ModelData.RibbonEmitterContainer.ValidIndex(i))
		{
			if(ModelData.RibbonEmitterContainer[i] == RibbonEmitter)
			{
				ModelData.RibbonEmitterContainer.Remove(i);
				break;
			}
		}
	}

	delete RibbonEmitter;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes a camera
//+-----------------------------------------------------------------------------
BOOL MODEL::RemoveCamera(MODEL_CAMERA* Camera, HWND Window)
{
	INT i;

	CameraManagerWindow.RemoveName(reinterpret_cast<INT>(Camera));

	for(i = 0; i < ModelData.CameraContainer.GetTotalSize(); i++)
	{
		if(ModelData.CameraContainer.ValidIndex(i))
		{
			if(ModelData.CameraContainer[i] == Camera)
			{
				ModelData.CameraContainer.Remove(i);
				break;
			}
		}
	}

	delete Camera;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes a geoset
//+-----------------------------------------------------------------------------
BOOL MODEL::RemoveGeoset(MODEL_GEOSET* Geoset, HWND Window)
{
	INT i;

	if(Geoset->BoneNodes.GetReferenceCount() > 0)
	{
		if(MessageBox(Window, "Some bones still references this geoset! Detach?", "Message", MB_YESNO | MB_ICONQUESTION) == IDNO)
		{
			Error.SetMessage("Unable to remove the geoset!");
			return FALSE;
		}

		Geoset->BoneNodes.Clear();
		MainWindow.MakeModelUnsaved();
	}

	if(Geoset->GeosetAnimationNodes.GetReferenceCount() > 0)
	{
		if(MessageBox(Window, "Some geoset animations still references this geoset! Detach?", "Message", MB_YESNO | MB_ICONQUESTION) == IDNO)
		{
			Error.SetMessage("Unable to remove the geoset!");
			return FALSE;
		}

		Geoset->GeosetAnimationNodes.Clear();
	}

	GeosetManagerWindow.RemoveName(reinterpret_cast<INT>(Geoset));

	for(i = 0; i < ModelData.GeosetContainer.GetTotalSize(); i++)
	{
		if(ModelData.GeosetContainer.ValidIndex(i))
		{
			if(ModelData.GeosetContainer[i] == Geoset)
			{
				ModelData.GeosetContainer.Remove(i);
				break;
			}
		}
	}

	delete Geoset;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes a geoset animation
//+-----------------------------------------------------------------------------
BOOL MODEL::RemoveGeosetAnimation(MODEL_GEOSET_ANIMATION* GeosetAnimation, HWND Window)
{
	INT i;

	if(GeosetAnimation->BoneNodes.GetReferenceCount() > 0)
	{
		if(MessageBox(Window, "Some bones still references this geoset animation! Detach?", "Message", MB_YESNO | MB_ICONQUESTION) == IDNO)
		{
			Error.SetMessage("Unable to remove the geoset animation!");
			return FALSE;
		}

		GeosetAnimation->BoneNodes.Clear();
	}

	GeosetAnimationManagerWindow.RemoveName(reinterpret_cast<INT>(GeosetAnimation));

	for(i = 0; i < ModelData.GeosetAnimationContainer.GetTotalSize(); i++)
	{
		if(ModelData.GeosetAnimationContainer.ValidIndex(i))
		{
			if(ModelData.GeosetAnimationContainer[i] == GeosetAnimation)
			{
				ModelData.GeosetAnimationContainer.Remove(i);
				break;
			}
		}
	}

	delete GeosetAnimation;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes a global sequence
//+-----------------------------------------------------------------------------
BOOL MODEL::RemoveGlobalSequence(MODEL_GLOBAL_SEQUENCE* GlobalSequence, HWND Window)
{
	INT i;

	if(GlobalSequence->EventObjectNodes.GetReferenceCount() > 0)
	{
		if(MessageBox(Window, "Some event objects still references this global sequence! Detach?", "Message", MB_YESNO | MB_ICONQUESTION) == IDNO)
		{
			Error.SetMessage("Unable to remove the global sequence!");
			return FALSE;
		}

		GlobalSequence->EventObjectNodes.Clear();
		MainWindow.MakeModelUnsaved();
	}

	if(GlobalSequence->InterpolatorNodes.GetReferenceCount() > 0)
	{
		if(MessageBox(Window, "Some interpolators still references this global sequence! Detach?", "Message", MB_YESNO | MB_ICONQUESTION) == IDNO)
		{
			Error.SetMessage("Unable to remove the global sequence!");
			return FALSE;
		}

		GlobalSequence->InterpolatorNodes.Clear();
	}

	GlobalSequenceManagerWindow.RemoveName(reinterpret_cast<INT>(GlobalSequence));

	for(i = 0; i < ModelData.GlobalSequenceContainer.GetTotalSize(); i++)
	{
		if(ModelData.GlobalSequenceContainer.ValidIndex(i))
		{
			if(ModelData.GlobalSequenceContainer[i] == GlobalSequence)
			{
				ModelData.GlobalSequenceContainer.Remove(i);
				break;
			}
		}
	}

	delete GlobalSequence;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes a material
//+-----------------------------------------------------------------------------
BOOL MODEL::RemoveMaterial(MODEL_MATERIAL* Material, HWND Window)
{
	INT i;

	if(Material->GeosetNodes.GetReferenceCount() > 0)
	{
		if(MessageBox(Window, "Some geosets still references this material! Detach?", "Message", MB_YESNO | MB_ICONQUESTION) == IDNO)
		{
			Error.SetMessage("Unable to remove the material!");
			return FALSE;
		}

		Material->GeosetNodes.Clear();
		MainWindow.MakeModelUnsaved();
	}

	if(Material->RibbonEmitterNodes.GetReferenceCount() > 0)
	{
		if(MessageBox(Window, "Some ribbon emitters still references this material! Detach?", "Message", MB_YESNO | MB_ICONQUESTION) == IDNO)
		{
			Error.SetMessage("Unable to remove the material!");
			return FALSE;
		}

		Material->RibbonEmitterNodes.Clear();
	}

	MaterialManagerWindow.RemoveName(reinterpret_cast<INT>(Material));

	for(i = 0; i < ModelData.MaterialContainer.GetTotalSize(); i++)
	{
		if(ModelData.MaterialContainer.ValidIndex(i))
		{
			if(ModelData.MaterialContainer[i] == Material)
			{
				ModelData.MaterialContainer.Remove(i);
				break;
			}
		}
	}

	delete Material;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes a pivot point
//+-----------------------------------------------------------------------------
BOOL MODEL::RemovePivotPoint(D3DXVECTOR3* PivotPoint, HWND Window)
{
	INT i;

	for(i = 0; i < ModelData.PivotPointContainer.GetTotalSize(); i++)
	{
		if(ModelData.PivotPointContainer.ValidIndex(i))
		{
			if(ModelData.PivotPointContainer[i] == PivotPoint)
			{
				ModelData.PivotPointContainer.Remove(i);
				break;
			}
		}
	}

	delete PivotPoint;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes a sequence
//+-----------------------------------------------------------------------------
BOOL MODEL::RemoveSequence(MODEL_SEQUENCE* Sequence, HWND Window)
{
	INT i;

	if(Sequence->ModelNodes.GetReferenceCount() > 0)
	{
		Error.SetMessage("Unable to remove the sequence, this sequence is currently being played!");
		return FALSE;
	}

	AnimationWindow.RemoveAnimation(reinterpret_cast<INT>(Sequence));
	SequenceManagerWindow.RemoveName(reinterpret_cast<INT>(Sequence));

	for(i = 0; i < ModelData.SequenceContainer.GetTotalSize(); i++)
	{
		if(ModelData.SequenceContainer.ValidIndex(i))
		{
			if(ModelData.SequenceContainer[i] == Sequence)
			{
				ModelData.SequenceContainer.Remove(i);
				break;
			}
		}
	}

	delete Sequence;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes a texture
//+-----------------------------------------------------------------------------
BOOL MODEL::RemoveTexture(MODEL_TEXTURE* Texture, HWND Window)
{
	INT i;

	if(Texture->MaterialLayerNodes.GetReferenceCount() > 0)
	{
		if(MessageBox(Window, "Some material layers still references this texture! Detach?", "Message", MB_YESNO | MB_ICONQUESTION) == IDNO)
		{
			Error.SetMessage("Unable to remove the texture!");
			return FALSE;
		}

		Texture->MaterialLayerNodes.Clear();
		MainWindow.MakeModelUnsaved();
	}

	if(Texture->ParticleEmitter2Nodes.GetReferenceCount() > 0)
	{
		if(MessageBox(Window, "Some particle emitters 2 still references this texture! Detach?", "Message", MB_YESNO | MB_ICONQUESTION) == IDNO)
		{
			Error.SetMessage("Unable to remove the texture!");
			return FALSE;
		}

		Texture->ParticleEmitter2Nodes.Clear();
	}

	TextureManagerWindow.RemoveName(reinterpret_cast<INT>(Texture));

	for(i = 0; i < ModelData.TextureContainer.GetTotalSize(); i++)
	{
		if(ModelData.TextureContainer.ValidIndex(i))
		{
			if(ModelData.TextureContainer[i] == Texture)
			{
				ModelData.TextureContainer.Remove(i);
				break;
			}
		}
	}

	delete Texture;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes a texture animation
//+-----------------------------------------------------------------------------
BOOL MODEL::RemoveTextureAnimation(MODEL_TEXTURE_ANIMATION* TextureAnimation, HWND Window)
{
	INT i;

	if(TextureAnimation->MaterialLayerNodes.GetReferenceCount() > 0)
	{
		if(MessageBox(Window, "Some material layers still references this texture animation! Detach?", "Message", MB_YESNO | MB_ICONQUESTION) == IDNO)
		{
			Error.SetMessage("Unable to remove the texture animation!");
			return FALSE;
		}

		TextureAnimation->MaterialLayerNodes.Clear();
	}

	TextureAnimationManagerWindow.RemoveName(reinterpret_cast<INT>(TextureAnimation));

	for(i = 0; i < ModelData.TextureAnimationContainer.GetTotalSize(); i++)
	{
		if(ModelData.TextureAnimationContainer.ValidIndex(i))
		{
			if(ModelData.TextureAnimationContainer[i] == TextureAnimation)
			{
				ModelData.TextureAnimationContainer.Remove(i);
				break;
			}
		}
	}

	delete TextureAnimation;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Returns the node with a specific object ID
//+-----------------------------------------------------------------------------
MODEL_BASE* MODEL::GetNode(INT ObjectId)
{
	INT i;

	for(i = 0; i < ModelData.BaseContainer.GetTotalSize(); i++)
	{
		if(ModelData.BaseContainer.ValidIndex(i))
		{
			if(ModelData.BaseContainer[i]->BaseData()->ObjectId == ObjectId)
			{
				return ModelData.BaseContainer[i];
			}
		}
	}

	return NULL;
}


//+-----------------------------------------------------------------------------
//| Returns the texture with a specific texture ID
//+-----------------------------------------------------------------------------
MODEL_TEXTURE* MODEL::GetTexture(INT TextureId)
{
	INT i;
	INT CurrentTextureId = 0;

	for(i = 0; i < ModelData.TextureContainer.GetTotalSize(); i++)
	{
		if(ModelData.TextureContainer.ValidIndex(i))
		{
			if(CurrentTextureId == TextureId)
			{
				return ModelData.TextureContainer[i];
			}

			CurrentTextureId++;
		}
	}

	return NULL;
}


//+-----------------------------------------------------------------------------
//| Creates a default model for texture viewing
//+-----------------------------------------------------------------------------
BOOL MODEL::CreateTextureModel(CONST std::string& TextureFileName, INT Width, INT Height, INT RealWidth, INT RealHeight)
{
	FLOAT X;
	FLOAT Y;
	FLOAT TextureFactorX;
	FLOAT TextureFactorY;
	MODEL_BONE* Bone;
	MODEL_GEOSET* Geoset;
	MODEL_TEXTURE* Texture;
	MODEL_MATERIAL* Material;
	MODEL_MATERIAL_LAYER* Layer;
	MODEL_GEOSET_VERTEX* Vertex;
	MODEL_GEOSET_FACE* Face;
	MODEL_GEOSET_GROUP* Group;
	std::vector<INT> MatrixList;

	ModelData.Info.Name = "Texture";
	ModelData.Info.BlendTime = 150;

	Texture = new MODEL_TEXTURE();
	if(Texture == NULL)
	{
		Error.SetMessage("Unable to load \"" + TextureFileName + "\", memory allocation failed!");
		return FALSE;
	}

	Texture->Data().FileName = TextureFileName;

	if(!AddTexture(Texture)) return FALSE;

	Material = new MODEL_MATERIAL();
	if(Material == NULL)
	{
		Error.SetMessage("Unable to load \"" + TextureFileName + "\", memory allocation failed!");
		return FALSE;
	}

	if(!AddMaterial(Material)) return FALSE;

	Layer = new MODEL_MATERIAL_LAYER();
	if(Layer == NULL)
	{
		Error.SetMessage("Unable to load \"" + TextureFileName + "\", memory allocation failed!");
		return FALSE;
	}

	Layer->Data().FilterMode = FILTER_MODE_NONE;
	Layer->Data().TextureId = ModelData.TextureContainer.GetLastAddedIndex();
	Layer->Data().AnimatedTextureId.SetStaticScalarInt(Layer->Data().TextureId);
	Layer->Data().Alpha.SetStaticScalar(1.0f);
	Layer->Data().Unshaded = TRUE;
	Layer->Data().Unfogged = TRUE;
	Layer->Data().TwoSided = TRUE;

	if(!Material->AddLayer(Layer)) return FALSE;

	Geoset = new MODEL_GEOSET();
	if(Geoset == NULL)
	{
		Error.SetMessage("Unable to load \"" + TextureFileName + "\", memory allocation failed!");
		return FALSE;
	}

	if(!AddGeoset(Geoset)) return FALSE;

	Bone = new MODEL_BONE();
	if(Bone == NULL)
	{
		Error.SetMessage("Unable to load \"" + TextureFileName + "\", memory allocation failed!");
		return FALSE;
	}

	Bone->Data().Name = "TextureBone";

	if(!AddBone(Bone)) return FALSE;

	MatrixList.push_back(0);

	Group = CreateGroup(MatrixList);
	if(Group == NULL) return FALSE;
	if(!Geoset->AddGroup(Group)) return FALSE;

	X = static_cast<FLOAT>(Width / 2);
	Y = static_cast<FLOAT>(Height / 2);

	TextureFactorX = (RealWidth == 0) ? 1.0f : (static_cast<FLOAT>(Width) / static_cast<FLOAT>(RealWidth));
	TextureFactorY = (RealHeight == 0) ? 1.0f : (static_cast<FLOAT>(Height) / static_cast<FLOAT>(RealHeight));

	Vertex = CreateVertex(D3DXVECTOR3(0.0f, -X, Y), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f), 0);
	if(Vertex == NULL) return FALSE;
	if(!Geoset->AddVertex(Vertex)) return FALSE;

	Vertex = CreateVertex(D3DXVECTOR3(0.0f, X, Y), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR2(TextureFactorX, 0.0f), 0);
	if(Vertex == NULL) return FALSE;
	if(!Geoset->AddVertex(Vertex)) return FALSE;

	Vertex = CreateVertex(D3DXVECTOR3(0.0f, -X, -Y), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, TextureFactorY), 0);
	if(Vertex == NULL) return FALSE;
	if(!Geoset->AddVertex(Vertex)) return FALSE;

	Vertex = CreateVertex(D3DXVECTOR3(0.0f, X, -Y), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR2(TextureFactorX, TextureFactorY), 0);
	if(Vertex == NULL) return FALSE;
	if(!Geoset->AddVertex(Vertex)) return FALSE;

	Face = CreateFace(0, 2, 3);
	if(Face == NULL) return FALSE;
	if(!Geoset->AddFace(Face)) return FALSE;

	Face = CreateFace(0, 3, 1);
	if(Face == NULL) return FALSE;
	if(!Geoset->AddFace(Face)) return FALSE;

	Geoset->MaterialNode.Attach(Material->GeosetNodes);
	Layer->TextureNode.Attach(Texture->MaterialLayerNodes);
	Bone->GeosetNode.Attach(Geoset->BoneNodes);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a default loading screen model
//+-----------------------------------------------------------------------------
BOOL MODEL::CreateLoadingScreenModel(CONST std::string& TextureFileName)
{
	MODEL_BONE* Bone;
	MODEL_GEOSET* Geoset;
	MODEL_TEXTURE* Texture;
	MODEL_MATERIAL* Material;
	MODEL_MATERIAL_LAYER* Layer;
	MODEL_GEOSET_VERTEX* Vertex;
	MODEL_GEOSET_FACE* Face;
	MODEL_GEOSET_GROUP* Group;
	MODEL_SEQUENCE* Sequence;
	std::vector<INT> MatrixList;

	ModelData.Info.Name = "LoadingScreen";
	ModelData.Info.BlendTime = 150;

	Texture = new MODEL_TEXTURE();
	if(Texture == NULL)
	{
		Error.SetMessage("Unable to load \"" + TextureFileName + "\", memory allocation failed!");
		return FALSE;
	}

	Texture->Data().FileName = TextureFileName;

	if(!AddTexture(Texture)) return FALSE;

	Material = new MODEL_MATERIAL();
	if(Material == NULL)
	{
		Error.SetMessage("Unable to load \"" + TextureFileName + "\", memory allocation failed!");
		return FALSE;
	}

	Material->Data().FullResolution = TRUE;

	if(!AddMaterial(Material)) return FALSE;

	Layer = new MODEL_MATERIAL_LAYER();
	if(Layer == NULL)
	{
		Error.SetMessage("Unable to load \"" + TextureFileName + "\", memory allocation failed!");
		return FALSE;
	}

	Layer->Data().FilterMode = FILTER_MODE_NONE;
	Layer->Data().TextureId = ModelData.TextureContainer.GetLastAddedIndex();
	Layer->Data().AnimatedTextureId.SetStaticScalarInt(Layer->Data().TextureId);
	Layer->Data().Alpha.SetStaticScalar(1.0f);
	Layer->Data().Unshaded = TRUE;
	Layer->Data().Unfogged = TRUE;
	Layer->Data().TwoSided = TRUE;

	if(!Material->AddLayer(Layer)) return FALSE;

	Geoset = new MODEL_GEOSET();
	if(Geoset == NULL)
	{
		Error.SetMessage("Unable to load \"" + TextureFileName + "\", memory allocation failed!");
		return FALSE;
	}

	if(!AddGeoset(Geoset)) return FALSE;

	Bone = new MODEL_BONE();
	if(Bone == NULL)
	{
		Error.SetMessage("Unable to load \"" + TextureFileName + "\", memory allocation failed!");
		return FALSE;
	}

	Bone->Data().Name = "LoadingScreenBone";

	if(!AddBone(Bone)) return FALSE;

	Sequence = new MODEL_SEQUENCE();
	if(Sequence == NULL)
	{
		Error.SetMessage("Unable to load \"" + TextureFileName + "\", memory allocation failed!");
		return FALSE;
	}

	Sequence->Data().Name = "Birth";
	Sequence->Data().NonLooping = TRUE;
	Sequence->Data().Interval = D3DXVECTOR2(1000.0f, 2000.0f);

	if(!AddSequence(Sequence)) return FALSE;

	MatrixList.push_back(0);

	Group = CreateGroup(MatrixList);
	if(Group == NULL) return FALSE;
	if(!Geoset->AddGroup(Group)) return FALSE;

	Vertex = CreateVertex(D3DXVECTOR3(0.8f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), 0);
	if(Vertex == NULL) return FALSE;
	if(!Geoset->AddVertex(Vertex)) return FALSE;

	Vertex = CreateVertex(D3DXVECTOR3(0.8f, 0.59999f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f), 0);
	if(Vertex == NULL) return FALSE;
	if(!Geoset->AddVertex(Vertex)) return FALSE;

	Vertex = CreateVertex(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f), 0);
	if(Vertex == NULL) return FALSE;
	if(!Geoset->AddVertex(Vertex)) return FALSE;

	Vertex = CreateVertex(D3DXVECTOR3(0.0f, 0.59999f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f), 0);
	if(Vertex == NULL) return FALSE;
	if(!Geoset->AddVertex(Vertex)) return FALSE;

	Face = CreateFace(3, 2, 1);
	if(Face == NULL) return FALSE;
	if(!Geoset->AddFace(Face)) return FALSE;

	Face = CreateFace(0, 1, 2);
	if(Face == NULL) return FALSE;
	if(!Geoset->AddFace(Face)) return FALSE;

	Geoset->MaterialNode.Attach(Material->GeosetNodes);
	Layer->TextureNode.Attach(Texture->MaterialLayerNodes);
	Bone->GeosetNode.Attach(Geoset->BoneNodes);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Generates real object IDs for all components
//+-----------------------------------------------------------------------------
VOID MODEL::GenerateObjectIds()
{
	INT i;
	INT j;
	INT k;
	INT Index;
	MODEL_BASE* Node;
	MODEL_GEOSET* Geoset;
	MODEL_GEOSET_GROUP* GeosetGroup;
	MODEL_GEOSET_GROUP_NODE* GeosetGroupNode;
	MODEL_MATERIAL* Material;
	MODEL_MATERIAL_LAYER* Layer;
	INTERPOLATOR* Interpolator;
	REFERENCE<INTERPOLATOR*, INTERPOLATOR*>* CurrentReference;

	ObjectIdManager.ClearAllObjectIds();

	CONTAINER_GENERATE_ID(ModelData.BoneContainer);
	CONTAINER_GENERATE_ID(ModelData.BoneContainer);
	CONTAINER_GENERATE_ID(ModelData.LightContainer);
	CONTAINER_GENERATE_ID(ModelData.HelperContainer);
	CONTAINER_GENERATE_ID(ModelData.AttachmentContainer);
	CONTAINER_GENERATE_ID(ModelData.ParticleEmitterContainer);
	CONTAINER_GENERATE_ID(ModelData.ParticleEmitter2Container);
	CONTAINER_GENERATE_ID(ModelData.RibbonEmitterContainer);
	CONTAINER_GENERATE_ID(ModelData.EventObjectContainer);
	CONTAINER_GENERATE_ID(ModelData.CollisionShapeContainer);

	for(i = 0; i < ModelData.BaseContainer.GetTotalSize(); i++)
	{
		if(ModelData.BaseContainer.ValidIndex(i))
		{
			Node = ModelData.BaseContainer[i];
			if(Node->ParentNode.GetObjectData() == Root)
			{
				Node->BaseData()->ParentId = INVALID_INDEX;
			}
			else
			{
				Node->BaseData()->ParentId = Node->ParentNode.GetObjectData()->BaseData()->ObjectId;
			}
		}
	}

	CONTAINER_ASSIGN_ID(ModelData.MaterialContainer, InternalMaterialId);
	CONTAINER_ASSIGN_ID(ModelData.GlobalSequenceContainer, InternalGlobalSequenceId);
	CONTAINER_ASSIGN_ID(ModelData.GeosetContainer, InternalGeosetId);
	CONTAINER_ASSIGN_ID(ModelData.GeosetAnimationContainer, InternalGeosetAnimationId);
	CONTAINER_ASSIGN_ID(ModelData.TextureContainer, InternalTextureId);
	CONTAINER_ASSIGN_ID(ModelData.TextureAnimationContainer, InternalTextureAnimationId);

	CONTAINER_RETRIEVE_ID(ModelData.GeosetContainer, MaterialNode, MaterialId, InternalMaterialId);
	CONTAINER_RETRIEVE_ID(ModelData.EventObjectContainer, GlobalSequenceNode, GlobalSequenceId, InternalGlobalSequenceId);
	CONTAINER_RETRIEVE_ID(ModelData.BoneContainer, GeosetNode, GeosetId, InternalGeosetId);
	CONTAINER_RETRIEVE_ID(ModelData.BoneContainer, GeosetAnimationNode, GeosetAnimationId, InternalGeosetAnimationId);
	CONTAINER_RETRIEVE_ID(ModelData.GeosetAnimationContainer, GeosetNode, GeosetId, InternalGeosetId);
	CONTAINER_RETRIEVE_ID(ModelData.ParticleEmitter2Container, TextureNode, TextureId, InternalTextureId);
	CONTAINER_RETRIEVE_ID(ModelData.RibbonEmitterContainer, MaterialNode, MaterialId, InternalMaterialId);

	for(i = 0; i < ModelData.GeosetContainer.GetTotalSize(); i++)
	{
		if(ModelData.GeosetContainer.ValidIndex(i))
		{
			Geoset = ModelData.GeosetContainer[i];
			for(j = 0; j < Geoset->Data().GroupContainer.GetTotalSize(); j++)
			{
				if(Geoset->Data().GroupContainer.ValidIndex(j))
				{
					GeosetGroup = Geoset->Data().GroupContainer[j];
					for(k = 0; k < GeosetGroup->MatrixList.GetTotalSize(); k++)
					{
						if(GeosetGroup->MatrixList.ValidIndex(k))
						{
							GeosetGroupNode = GeosetGroup->MatrixList[k];
							if(GeosetGroupNode->Node.IsAttached())
							{
								GeosetGroupNode->NodeId = GeosetGroupNode->Node.GetObjectData()->BaseData()->ObjectId;
							}
							else
							{
								GeosetGroupNode->NodeId = 0;
							}
						}
					}
				}
			}
		}
	}

	for(i = 0; i < ModelData.MaterialContainer.GetTotalSize(); i++)
	{
		if(ModelData.MaterialContainer.ValidIndex(i))
		{
			Material = ModelData.MaterialContainer[i];
			for(j = 0; j < Material->Data().LayerContainer.GetTotalSize(); j++)
			{
				if(Material->Data().LayerContainer.ValidIndex(j))
				{
					Layer = Material->Data().LayerContainer[j];
					if(Layer->TextureNode.IsAttached())
					{
						Layer->Data().TextureId = Layer->TextureNode.GetObjectData()->Data().InternalTextureId;
						Layer->Data().AnimatedTextureId.SetStaticScalarInt(Layer->Data().TextureId);
					}
					else
					{
						Layer->Data().TextureId = INVALID_INDEX;
					}
				}
			}
		}
	}

	Index = 0;
	for(i = 0; i < ModelData.AttachmentContainer.GetTotalSize(); i++)
	{
		if(ModelData.AttachmentContainer.ValidIndex(i))
		{
			ModelData.AttachmentContainer[i]->Data().AttachmentId = Index;
			Index++;
		}
	}

	CurrentReference = INTERPOLATOR::InterpolatorNodes.GetFirstReference();
	while(CurrentReference != NULL)
	{
		Interpolator = CurrentReference->GetData();
		if(Interpolator->GlobalSequenceNode.IsAttached())
		{
			Interpolator->SetGlobalSequenceId(Interpolator->GlobalSequenceNode.GetObjectData()->Data().InternalGlobalSequenceId);
		}

		CurrentReference = INTERPOLATOR::InterpolatorNodes.GetNextReference(CurrentReference);
	}
}


//+-----------------------------------------------------------------------------
//| Connects the nodes with its parents etc...
//+-----------------------------------------------------------------------------
VOID MODEL::ConnectNodes()
{
	INT i;
	INT j;
	INT k;
	INT Id;
	MODEL_BASE* Node;
	MODEL_BASE* ParentNode;
	MODEL_GEOSET* Geoset;
	MODEL_GEOSET_GROUP* GeosetGroup;
	MODEL_GEOSET_GROUP_NODE* GeosetGroupNode;
	MODEL_MATERIAL* Material;
	INTERPOLATOR* Interpolator;
	REFERENCE<INTERPOLATOR*, INTERPOLATOR*>* CurrentReference;

	for(i = 0; i < ModelData.BaseContainer.GetTotalSize(); i++)
	{
		if(ModelData.BaseContainer.ValidIndex(i))
		{
			Node = ModelData.BaseContainer[i];

			Id = Node->BaseData()->ParentId;
			if(Id != INVALID_INDEX)
			{
				ParentNode = GetNode(Id);
				if(ParentNode != NULL)
				{
					Node->ParentNode.Attach(ParentNode->ChildNodes);
				}
			}
		}
	}

	for(i = 0; i < ModelData.GeosetContainer.GetTotalSize(); i++)
	{
		if(!ModelData.GeosetContainer.ValidIndex(i)) continue;

		Geoset = ModelData.GeosetContainer[i];
		for(j = 0; j < Geoset->Data().GroupContainer.GetTotalSize(); j++)
		{
			if(!Geoset->Data().GroupContainer.ValidIndex(j)) continue;

			GeosetGroup = Geoset->Data().GroupContainer[j];
			for(k = 0; k < GeosetGroup->MatrixList.GetTotalSize(); k++)
			{
				if(!GeosetGroup->MatrixList.ValidIndex(k)) continue;

				GeosetGroupNode = GeosetGroup->MatrixList[k];
				Node = GetNode(GeosetGroupNode->NodeId);
				if(Node == NULL) continue;

				GeosetGroupNode->Node.Attach(Node->MatrixListNodes);
			}
		}
	}

	for(i = 0; i < ModelData.MaterialContainer.GetTotalSize(); i++)
	{
		if(ModelData.MaterialContainer.ValidIndex(i))
		{
			Material = ModelData.MaterialContainer[i];

			CONTAINER_CONNECT_NODES(Material->Data().LayerContainer, ModelData.TextureContainer, TextureNode, MaterialLayerNodes, TextureId);
			CONTAINER_CONNECT_NODES(Material->Data().LayerContainer, ModelData.TextureAnimationContainer, TextureAnimationNode, MaterialLayerNodes, TextureAnimationId);
		}
	}

	CurrentReference = INTERPOLATOR::InterpolatorNodes.GetFirstReference();
	while(CurrentReference != NULL)
	{
		Interpolator = CurrentReference->GetData();
		Id = Interpolator->GetGlobalSequenceId();

		if(ModelData.GlobalSequenceContainer.ValidIndex(Id))
		{
			Interpolator->GlobalSequenceNode.Attach(ModelData.GlobalSequenceContainer[Id]->InterpolatorNodes);
		}

		CurrentReference = INTERPOLATOR::InterpolatorNodes.GetNextReference(CurrentReference);
	}

	CONTAINER_CONNECT_NODES(ModelData.GeosetContainer, ModelData.MaterialContainer, MaterialNode, GeosetNodes, MaterialId);

	CONTAINER_CONNECT_NODES(ModelData.EventObjectContainer, ModelData.GlobalSequenceContainer, GlobalSequenceNode, EventObjectNodes, GlobalSequenceId);
	CONTAINER_CONNECT_NODES(ModelData.BoneContainer, ModelData.GeosetContainer, GeosetNode, BoneNodes, GeosetId);
	CONTAINER_CONNECT_NODES(ModelData.BoneContainer, ModelData.GeosetAnimationContainer, GeosetAnimationNode, BoneNodes, GeosetAnimationId);
	CONTAINER_CONNECT_NODES(ModelData.GeosetAnimationContainer, ModelData.GeosetContainer, GeosetNode, GeosetAnimationNodes, GeosetId);
	CONTAINER_CONNECT_NODES(ModelData.ParticleEmitter2Container, ModelData.TextureContainer, TextureNode, ParticleEmitter2Nodes, TextureId);
	CONTAINER_CONNECT_NODES(ModelData.RibbonEmitterContainer, ModelData.MaterialContainer, MaterialNode, RibbonEmitterNodes, MaterialId);


	NodeManagerWindow.ClearAllNodes();
	InsertNode(Root);
}


//+-----------------------------------------------------------------------------
//| Wraps the pivot points into its container
//+-----------------------------------------------------------------------------
VOID MODEL::WrapPivotPoints()
{
	INT ObjectId;
	MODEL_BASE* Node;

	SAFE_CLEAR(ModelData.PivotPointContainer);

	ObjectId = 0;
	while(TRUE)
	{
		Node = GetNode(ObjectId);
		if(Node == NULL) break;

		ModelData.PivotPointContainer.Add(new D3DXVECTOR3(Node->BaseData()->PivotPoint));

		ObjectId++;
	}
}


//+-----------------------------------------------------------------------------
//| Unwraps the pivot points from its container
//+-----------------------------------------------------------------------------
VOID MODEL::UnwrapPivotPoints()
{
	INT i;
	MODEL_BASE* Node;

	for(i = 0; i < ModelData.PivotPointContainer.GetTotalSize(); i++)
	{
		if(ModelData.PivotPointContainer.ValidIndex(i))
		{
			Node = GetNode(i);
			if(Node != NULL)
			{
				Node->BaseData()->PivotPoint = (*ModelData.PivotPointContainer[i]);
			}
		}
	}
}


//+-----------------------------------------------------------------------------
//| Adds a base node
//+-----------------------------------------------------------------------------
BOOL MODEL::AddBaseNode(MODEL_BASE* Node, MODEL_BASE* ParentNode)
{
	if(Node->BaseData()->ObjectId == INVALID_INDEX)
	{
		Node->BaseData()->ObjectId = ObjectIdManager.GetNewObjectId();
	}

	if(ParentNode != NULL)
	{
		Node->ParentNode.Attach(ParentNode->ChildNodes);
	}
	else
	{
		Node->ParentNode.Attach(Root->ChildNodes);
	}

	if(!ModelData.BaseContainer.Add(Node))
	{
		Error.SetMessage("Unable to add a new node!");
		return FALSE;
	}

	NodeManagerWindow.AddNode(Node);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes a base node
//+-----------------------------------------------------------------------------
BOOL MODEL::RemoveBaseNode(MODEL_BASE* Node, HWND Window)
{
	INT i;

	if(Node->ChildNodes.GetReferenceCount() > 0)
	{
		Error.SetMessage("Unable to remove node, it is still in use by a child node!");
		return FALSE;
	}

	if(Node->MatrixListNodes.GetReferenceCount() > 0)
	{
		if(MessageBox(Window, "Some geoset matrix list groups still references this bone! Detach?", "Message", MB_YESNO | MB_ICONQUESTION) == IDNO)
		{
			Error.SetMessage("Unable to remove the bone!");
			return FALSE;
		}

		Node->MatrixListNodes.Clear();
	}

	ObjectIdManager.RemoveObjectId(Node->BaseData()->ObjectId);
	NodeManagerWindow.RemoveNode(Node);

	for(i = 0; i < ModelData.BaseContainer.GetTotalSize(); i++)
	{
		if(ModelData.BaseContainer.ValidIndex(i))
		{
			if(ModelData.BaseContainer[i] == Node)
			{
				ModelData.BaseContainer.Remove(i);
				break;
			}
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Inserts a node and all its child nodes into the node manager
//+-----------------------------------------------------------------------------
VOID MODEL::InsertNode(MODEL_BASE* Node)
{
	REFERENCE<MODEL_BASE*, MODEL_BASE*>* CurrentNode;

	CurrentNode = Node->ChildNodes.GetFirstReference();
	while(CurrentNode != NULL)
	{
		NodeManagerWindow.AddNode(CurrentNode->GetData());
		InsertNode(CurrentNode->GetData());
		CurrentNode = Node->ChildNodes.GetNextReference(CurrentNode);
	}
}


//+-----------------------------------------------------------------------------
//| Creates a new vertex
//+-----------------------------------------------------------------------------
MODEL_GEOSET_VERTEX* MODEL::CreateVertex(CONST D3DXVECTOR3& Position, CONST D3DXVECTOR3& Normal, CONST D3DXVECTOR2& TexturePosition, INT VertexGroup)
{
	MODEL_GEOSET_VERTEX* Vertex;

	Vertex = new MODEL_GEOSET_VERTEX();
	if(Vertex == NULL)
	{
		Error.SetMessage("Unable to create a new vertex!");
		return FALSE;
	}

	Vertex->Position = Position;
	Vertex->Normal = Normal;
	Vertex->TexturePosition = TexturePosition;
	Vertex->VertexGroup = VertexGroup;

	return Vertex;
}


//+-----------------------------------------------------------------------------
//| Creates a new face
//+-----------------------------------------------------------------------------
MODEL_GEOSET_FACE* MODEL::CreateFace(INT Index1, INT Index2, INT Index3)
{
	MODEL_GEOSET_FACE* Face;

	Face = new MODEL_GEOSET_FACE();
	if(Face == NULL)
	{
		Error.SetMessage("Unable to create a new face!");
		return FALSE;
	}

	Face->Index1 = Index1;
	Face->Index2 = Index2;
	Face->Index3 = Index3;

	return Face;
}


//+-----------------------------------------------------------------------------
//| Creates a new matrix group
//+-----------------------------------------------------------------------------
MODEL_GEOSET_GROUP* MODEL::CreateGroup(CONST std::vector<INT>& MatrixList)
{
	INT i;
	MODEL_BASE* Node;
	MODEL_GEOSET_GROUP* Group;
	MODEL_GEOSET_GROUP_NODE* GroupNode;

	Group = new MODEL_GEOSET_GROUP();
	if(Group == NULL)
	{
		Error.SetMessage("Unable to create a new matrix group!");
		return FALSE;
	}

	Group->MatrixListSize = static_cast<INT>(MatrixList.size());

	for(i = 0; i < Group->MatrixListSize; i++)
	{
		GroupNode = new MODEL_GEOSET_GROUP_NODE();
		if(GroupNode == NULL)
		{
			Error.SetMessage("Unable to create a new matrix group node!");
			return FALSE;
		}

		GroupNode->NodeId = MatrixList[i];
		Group->MatrixList.Add(GroupNode);

		Node = GetNode(GroupNode->NodeId);
		if(Node != NULL) GroupNode->Node.Attach(Node->MatrixListNodes);
	}

	return Group;
}


//+-----------------------------------------------------------------------------
//| Renders all bones
//+-----------------------------------------------------------------------------
VOID MODEL::RenderBones()
{
	INT i;
	BOOL Render;
	MODEL_BASE* Node;
	MODEL_BASE* ParentNode;
	D3DXVECTOR3 From;
	D3DXVECTOR3 To;

	for(i = 0; i < ModelData.BaseContainer.GetTotalSize(); i++)
	{
		if(ModelData.BaseContainer.ValidIndex(i))
		{
			Render = TRUE;
			Node = ModelData.BaseContainer[i];

			while(TRUE)
			{
				if(Node->BaseData()->Type == NODE_TYPE_BONE) break;
				if(Node->BaseData()->Type == NODE_TYPE_HELPER) break;

				Render = FALSE;
				break;
			}

			if(!Render) continue;

			if(Node->ParentNode.IsAttached())
			{
				ParentNode = Node->ParentNode.GetObjectData();
				if(ParentNode == NULL) ParentNode = Root;
			}
			else
			{
				ParentNode = Root;
			}

			From = ParentNode->BaseData()->PivotPoint;
			To = Node->BaseData()->PivotPoint;

			if(SequenceNode.IsAttached())
			{
				D3DXVECTOR4 TempFrom;
				D3DXVECTOR4 TempTo;

				D3DXVec3Transform(&TempFrom, &From, &(ParentNode->Matrix()));
				D3DXVec3Transform(&TempTo, &To, &(Node->Matrix()));

				From.x = TempFrom.x;
				From.y = TempFrom.y;
				From.z = TempFrom.z;

				To.x = TempTo.x;
				To.y = TempTo.y;
				To.z = TempTo.z;
			}

			Graphics.RenderLine(From, To, COLOR_BONE);
		}
	}
}


//+-----------------------------------------------------------------------------
//| Renders all lights
//+-----------------------------------------------------------------------------
VOID MODEL::RenderLights()
{
	//XXXXXXXXXXXXXXXXXXXXX
}


//+-----------------------------------------------------------------------------
//| Renders all cameras
//+-----------------------------------------------------------------------------
VOID MODEL::RenderCameras()
{
	//XXXXXXXXXXXXXXXXXXXXX
}


//+-----------------------------------------------------------------------------
//| Renders all attachments
//+-----------------------------------------------------------------------------
VOID MODEL::RenderAttachments()
{
	//XXXXXXXXXXXXXXXXXXXXX
}


//+-----------------------------------------------------------------------------
//| Renders all collision shapes
//+-----------------------------------------------------------------------------
VOID MODEL::RenderCollisionShapes()
{
	INT i;
	MODEL_COLLISION_SHAPE* CollisionShape;
	std::list<D3DXVECTOR3>::iterator j;

	for(i = 0; i < ModelData.CollisionShapeContainer.GetTotalSize(); i++)
	{
		if(ModelData.CollisionShapeContainer.ValidIndex(i))
		{
			CollisionShape = ModelData.CollisionShapeContainer[i];
			switch(CollisionShape->Data().Type)
			{
				case COLLISION_SHAPE_TYPE_BOX:
				{
					D3DXVECTOR3 Corner1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					D3DXVECTOR3 Corner2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					j = CollisionShape->Data().VertexList.begin();
					if(j != CollisionShape->Data().VertexList.end())
					{
						Corner1 = (*j);

						j++;
						if(j != CollisionShape->Data().VertexList.end())
						{
							Corner2 = (*j);
						}
					}

					Graphics.RenderBox(Corner1, Corner2, COLOR_COLLISION_SHAPE);
					break;
				}

				case COLLISION_SHAPE_TYPE_SPHERE:
				{
					D3DXVECTOR3 Center = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					FLOAT Radius;

					j = CollisionShape->Data().VertexList.begin();
					if(j != CollisionShape->Data().VertexList.end())
					{
						Center = (*j);
					}

					Radius = CollisionShape->Data().BoundsRadius;

					Graphics.RenderSphere(Center, Radius, COLOR_COLLISION_SHAPE);
					break;
				}
			}
		}
	}
}
