//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "NodeManagerWindow.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
NODE_MANAGER_WINDOW NodeManagerWindow;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
NODE_MANAGER_WINDOW::NODE_MANAGER_WINDOW()
{
	CurrentlySelectedNode = NULL;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
NODE_MANAGER_WINDOW::~NODE_MANAGER_WINDOW()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Creates a new window
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::Create()
{
	WINDOW_TREEVIEW_INFO TreeviewInfo;

	FrameInfo.Title = "Node Manager";
	FrameInfo.Width = DEFAULT_NODE_MANAGER_WINDOW_WIDTH;
	FrameInfo.Height = DEFAULT_NODE_MANAGER_WINDOW_HEIGHT;
	FrameInfo.Style &= (~WS_VISIBLE);
	FrameInfo.Style |= (WS_MAXIMIZEBOX | WS_SIZEBOX);

	if(!WINDOW_FRAME::Create()) return FALSE;

	TreeviewInfo.Parent = Window;

	if(!TreeView.Create(TreeviewInfo)) return FALSE;

	NodeIcon.AssignIconListToTreeView(TreeView.GetWindow());

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Destroys the window
//+-----------------------------------------------------------------------------
VOID NODE_MANAGER_WINDOW::Destroy()
{
	TreeView.Destroy();

	WINDOW_FRAME::Destroy();
}


//+-----------------------------------------------------------------------------
//| Handles the window messages
//+-----------------------------------------------------------------------------
LRESULT NODE_MANAGER_WINDOW::MessageHandler(UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_SIZE:
		case WM_SIZING:
		{
			ResizeTreeView();
			return 0;
		}

		case WM_CLOSE:
		{
			Hide();
			return 0;
		}

		case WM_DESTROY:
		{
			return 0;
		}
	}

	return DefWindowProc(Window, Message, W, L);
}


//+-----------------------------------------------------------------------------
//| Handles the window menu messages
//+-----------------------------------------------------------------------------
LRESULT NODE_MANAGER_WINDOW::MenuHandler(WORD MenuItem)
{
	switch(MenuItem)
	{
		case NodeFileCreateAttachment:
		case NodeFileCreateAttachment2:
		{
			if(!CreateNewAttachment())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case NodeFileCreateBone:
		case NodeFileCreateBone2:
		{
			if(!CreateNewBone())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case NodeFileCreateCollisionShape:
		case NodeFileCreateCollisionShape2:
		{
			if(!CreateNewCollisionShape())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case NodeFileCreateEventObject:
		case NodeFileCreateEventObject2:
		{
			if(!CreateNewEventObject())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case NodeFileCreateHelper:
		case NodeFileCreateHelper2:
		{
			if(!CreateNewHelper())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case NodeFileCreateLight:
		case NodeFileCreateLight2:
		{
			if(!CreateNewLight())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case NodeFileCreateParticleEmitter:
		case NodeFileCreateParticleEmitter2:
		{
			if(!CreateNewParticleEmitter())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case NodeFileCreateParticleEmitter2_1:
		case NodeFileCreateParticleEmitter2_2:
		{
			if(!CreateNewParticleEmitter2())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case NodeFileCreateRibbonEmitter:
		case NodeFileCreateRibbonEmitter2:
		{
			if(!CreateNewRibbonEmitter())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case NodeFileDuplicate:
		case NodeFileDuplicate2:
		{
			if(!Duplicate())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case NodeFileMoveUp:
		case NodeFileMoveUp2:
		{
			if(!MoveUp())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case NodeFileMoveDown:
		case NodeFileMoveDown2:
		{
			if(!MoveDown())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case NodeFileMoveLeft:
		case NodeFileMoveLeft2:
		{
			if(!MoveLeft())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case NodeFileMoveRight:
		case NodeFileMoveRight2:
		{
			if(!MoveRight())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case NodeFileEdit:
		case NodeFileEdit2:
		{
			if(!Edit())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case NodeFileEditNode:
		case NodeFileEditNode2:
		{
			if(!EditNode())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case NodeFileRemove:
		case NodeFileRemove2:
		{
			if(!Remove())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}
	}

	return 0;
}


//+-----------------------------------------------------------------------------
//| Handles the window control messages
//+-----------------------------------------------------------------------------
LRESULT NODE_MANAGER_WINDOW::ControlHandler(HWND Control, WORD Code)
{
	return 0;
}


//+-----------------------------------------------------------------------------
//| Handles the window notify messages
//+-----------------------------------------------------------------------------
LRESULT NODE_MANAGER_WINDOW::NotifyHandler(HWND Control, UINT Code, NMHDR* Header)
{
	if(Control == TreeView.GetWindow())
	{
		NM_TREEVIEW* TreeViewHeader;

		TreeViewHeader = reinterpret_cast<NM_TREEVIEW*>(Header);
		if(TreeViewHeader == NULL) return 0;

		return TreeViewHandler(Code, TreeViewHeader);
	}

	return 0;
}


//+-----------------------------------------------------------------------------
//| Handles the treeview messages
//+-----------------------------------------------------------------------------
LRESULT NODE_MANAGER_WINDOW::TreeViewHandler(UINT Code, NM_TREEVIEW* TreeViewHeader)
{
	TVITEM ItemData;
	POINT MousePosition;
	POINT LocalMousePosition;
	TVHITTESTINFO HitTestInfo;

	GetCursorPos(&MousePosition);
	LocalMousePosition = MousePosition;
	ScreenToClient(TreeView.GetWindow(), &LocalMousePosition);

	switch(Code)
	{
		case NM_DBLCLK:
		{
			if(CurrentlySelectedNode == NULL) return 0;

			ZeroMemory(&HitTestInfo, sizeof(TVHITTESTINFO));
			HitTestInfo.pt = LocalMousePosition;
			if(TreeView_HitTest(TreeView.GetWindow(), &HitTestInfo) == NULL) return 0;
			if(!(HitTestInfo.flags & TVHT_ONITEM)) return 0;

			if(!Edit())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			TreeView_Expand(TreeView.GetWindow(), HitTestInfo.hItem, TVE_TOGGLE);

			return 0;
		}

		case NM_RCLICK:
		{
			INT MenuId;
			HTREEITEM TreeViewItem;

			MenuId = NodeMenu2;
			TreeViewItem = NULL;

			TreeView_SelectItem(TreeView.GetWindow(), NULL);
			CurrentlySelectedNode = NULL;

			while(TRUE)
			{
				ZeroMemory(&HitTestInfo, sizeof(TVHITTESTINFO));
				HitTestInfo.pt = LocalMousePosition;
				if(TreeView_HitTest(TreeView.GetWindow(), &HitTestInfo) == NULL) break;
				if(!(HitTestInfo.flags & TVHT_ONITEM)) break;

				TreeViewItem = HitTestInfo.hItem;
				if(TreeViewItem == NULL) break;

				ZeroMemory(&ItemData, sizeof(TVITEM));
				ItemData.mask = TVIF_PARAM;
				ItemData.hItem = TreeViewItem;
				if(!TreeView_GetItem(TreeView.GetWindow(), &ItemData)) return 0;

				TreeView_SelectItem(TreeView.GetWindow(), TreeViewItem);
				CurrentlySelectedNode = reinterpret_cast<MODEL_BASE*>(ItemData.lParam);
				if(CurrentlySelectedNode == NULL) return 0;

				MenuId = NodeMenu;
				break;
			}

			if(!TrackPopupMenuEx(GetSubMenu(LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(MenuId)), 0),
								 TPM_LEFTALIGN | TPM_RIGHTBUTTON, MousePosition.x, MousePosition.y,
								 Window, NULL))
			{
				Error.SetMessage("Unable to create popup menu!");
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case TVN_SELCHANGED:
		{
			CurrentlySelectedNode = reinterpret_cast<MODEL_BASE*>(TreeViewHeader->itemNew.lParam);
			if(CurrentlySelectedNode == NULL) return 0;

			return 0;
		}
	}

	return 0;
}


//+-----------------------------------------------------------------------------
//| Adds a node
//+-----------------------------------------------------------------------------
VOID NODE_MANAGER_WINDOW::AddNode(MODEL_BASE* Node, HTREEITEM InsertionPoint)
{
	TVINSERTSTRUCT InsertItem;
	HTREEITEM Parent;

	if(Node->ParentNode.IsAttached())
	{
		Parent = Node->ParentNode.GetObjectData()->BaseData()->TreeViewItem;
		if(Parent == NULL) Parent = TVI_ROOT;
	}
	else
	{
		Parent = TVI_ROOT;
	}

	InsertItem.hParent = Parent;
	InsertItem.hInsertAfter = InsertionPoint;

	InsertItem.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	InsertItem.item.hItem = NULL;
	InsertItem.item.state = 0;
	InsertItem.item.stateMask = 0;
	InsertItem.item.pszText = const_cast<CHAR*>(Node->BaseData()->Name.c_str());
	InsertItem.item.cchTextMax = static_cast<INT>(Node->BaseData()->Name.size());
	InsertItem.item.iImage = NodeIcon.GetIconIndex(Node->BaseData()->Type);
	InsertItem.item.iSelectedImage = InsertItem.item.iImage;
	InsertItem.item.cChildren = 0;
	InsertItem.item.lParam = reinterpret_cast<LPARAM>(Node);

	Node->BaseData()->TreeViewItem = TreeView_InsertItem(TreeView.GetWindow(), &InsertItem);

	TreeView_SelectItem(TreeView.GetWindow(), Node->BaseData()->TreeViewItem);
	CurrentlySelectedNode = Node;
}


//+-----------------------------------------------------------------------------
//| Adds a node and all of its children (and grandchildren etc...)
//+-----------------------------------------------------------------------------
VOID NODE_MANAGER_WINDOW::AddNodeAndChildren(MODEL_BASE* Node, HTREEITEM InsertionPoint)
{
	REFERENCE<MODEL_BASE*, MODEL_BASE*>* CurrentReference;

	AddNode(Node, InsertionPoint);

	CurrentReference = Node->ChildNodes.GetFirstReference();
	while(CurrentReference != NULL)
	{
		AddNodeAndChildren(CurrentReference->GetData(), TVI_LAST);
		CurrentReference = Node->ChildNodes.GetNextReference(CurrentReference);
	}
}


//+-----------------------------------------------------------------------------
//| Removes a node
//+-----------------------------------------------------------------------------
VOID NODE_MANAGER_WINDOW::RemoveNode(MODEL_BASE* Node)
{
	TreeView_DeleteItem(TreeView.GetWindow(), Node->BaseData()->TreeViewItem);
	Node->BaseData()->TreeViewItem = NULL;
}


//+-----------------------------------------------------------------------------
//| Clears all nodes
//+-----------------------------------------------------------------------------
VOID NODE_MANAGER_WINDOW::ClearAllNodes()
{
	TreeView.Clear();
}


//+-----------------------------------------------------------------------------
//| Returns the current parent node
//+-----------------------------------------------------------------------------
MODEL_BASE* NODE_MANAGER_WINDOW::GetCurrentParent()
{
	if(CurrentlySelectedNode == NULL) return NULL;

	return CurrentlySelectedNode;
}


//+-----------------------------------------------------------------------------
//| Creates a new attachment
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::CreateNewAttachment()
{
	MODEL_ATTACHMENT* Attachment;

	Attachment = new MODEL_ATTACHMENT();
	if(Attachment == NULL)
	{
		Error.SetMessage("Unable to create a new attachment, memory allocation failed!");
		return FALSE;
	}

	if(!Model.AddAttachment(Attachment, GetCurrentParent()))
	{
		delete Attachment;
		return FALSE;
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a new bone
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::CreateNewBone()
{
	MODEL_BONE* Bone;

	Bone = new MODEL_BONE();
	if(Bone == NULL)
	{
		Error.SetMessage("Unable to create a new bone, memory allocation failed!");
		return FALSE;
	}

	if(!Model.AddBone(Bone, GetCurrentParent()))
	{
		delete Bone;
		return FALSE;
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a new collision shape
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::CreateNewCollisionShape()
{
	MODEL_COLLISION_SHAPE* CollisionShape;

	CollisionShape = new MODEL_COLLISION_SHAPE();
	if(CollisionShape == NULL)
	{
		Error.SetMessage("Unable to create a new collision shape, memory allocation failed!");
		return FALSE;
	}

	if(!Model.AddCollisionShape(CollisionShape, GetCurrentParent()))
	{
		delete CollisionShape;
		return FALSE;
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a new event object
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::CreateNewEventObject()
{
	MODEL_EVENT_OBJECT* EventObject;

	EventObject = new MODEL_EVENT_OBJECT();
	if(EventObject == NULL)
	{
		Error.SetMessage("Unable to create a new event object, memory allocation failed!");
		return FALSE;
	}

	if(!Model.AddEventObject(EventObject, GetCurrentParent()))
	{
		delete EventObject;
		return FALSE;
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a new helper
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::CreateNewHelper()
{
	MODEL_HELPER* Helper;

	Helper = new MODEL_HELPER();
	if(Helper == NULL)
	{
		Error.SetMessage("Unable to create a new helper, memory allocation failed!");
		return FALSE;
	}

	if(!Model.AddHelper(Helper, GetCurrentParent()))
	{
		delete Helper;
		return FALSE;
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a new light
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::CreateNewLight()
{
	MODEL_LIGHT* Light;

	Light = new MODEL_LIGHT();
	if(Light == NULL)
	{
		Error.SetMessage("Unable to create a new light, memory allocation failed!");
		return FALSE;
	}

	if(!Model.AddLight(Light, GetCurrentParent()))
	{
		delete Light;
		return FALSE;
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a new particle emitter
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::CreateNewParticleEmitter()
{
	MODEL_PARTICLE_EMITTER* ParticleEmitter;

	ParticleEmitter = new MODEL_PARTICLE_EMITTER();
	if(ParticleEmitter == NULL)
	{
		Error.SetMessage("Unable to create a new particle emitter, memory allocation failed!");
		return FALSE;
	}

	if(!Model.AddParticleEmitter(ParticleEmitter, GetCurrentParent()))
	{
		delete ParticleEmitter;
		return FALSE;
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a new particle emitter 2
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::CreateNewParticleEmitter2()
{
	MODEL_PARTICLE_EMITTER_2* ParticleEmitter2;

	ParticleEmitter2 = new MODEL_PARTICLE_EMITTER_2();
	if(ParticleEmitter2 == NULL)
	{
		Error.SetMessage("Unable to create a new particle emitter 2, memory allocation failed!");
		return FALSE;
	}

	if(!Model.AddParticleEmitter2(ParticleEmitter2, GetCurrentParent()))
	{
		delete ParticleEmitter2;
		return FALSE;
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a new ribbon emitter
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::CreateNewRibbonEmitter()
{
	MODEL_RIBBON_EMITTER* RibbonEmitter;

	RibbonEmitter = new MODEL_RIBBON_EMITTER();
	if(RibbonEmitter == NULL)
	{
		Error.SetMessage("Unable to create a new ribbon emitter, memory allocation failed!");
		return FALSE;
	}

	if(!Model.AddRibbonEmitter(RibbonEmitter, GetCurrentParent()))
	{
		delete RibbonEmitter;
		return FALSE;
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Edits an existing attachment
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::EditAttachment(MODEL_ATTACHMENT* Attachment)
{
	if(!AttachmentDialog.Display(Window, Attachment->Data())) return TRUE;

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Edits an existing bone
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::EditBone(MODEL_BONE* Bone)
{
	MODEL_GEOSET* Geoset;
	MODEL_GEOSET_ANIMATION* GeosetAnimation;

	Geoset = Bone->GeosetNode.IsAttached() ? Bone->GeosetNode.GetObjectData() : NULL;
	GeosetAnimation = Bone->GeosetAnimationNode.IsAttached() ? Bone->GeosetAnimationNode.GetObjectData() : NULL;

	if(!BoneDialog.Display(Window, Bone->Data(), Geoset, GeosetAnimation)) return TRUE;

	if(Geoset != NULL)
	{
		Bone->GeosetNode.Attach(Geoset->BoneNodes);
	}
	else
	{
		Bone->GeosetNode.Detach();
	}

	if(GeosetAnimation != NULL)
	{
		Bone->GeosetAnimationNode.Attach(GeosetAnimation->BoneNodes);
	}
	else
	{
		Bone->GeosetAnimationNode.Detach();
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Edits an existing collision shape
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::EditCollisionShape(MODEL_COLLISION_SHAPE* CollisionShape)
{
	if(!CollisionShapeDialog.Display(Window, CollisionShape->Data())) return TRUE;

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Edits an existing event object
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::EditEventObject(MODEL_EVENT_OBJECT* EventObject)
{
	std::string Name;
	MODEL_EVENT_OBJECT_DATA Data;
	MODEL_GLOBAL_SEQUENCE* GlobalSequence;

	Data = EventObject->Data();
	GlobalSequence = EventObject->GlobalSequenceNode.IsAttached() ? EventObject->GlobalSequenceNode.GetObjectData() : NULL;
	Name = Data.Name;

	if(!EventObjectDialog.Display(Window, Data, GlobalSequence)) return TRUE;

	if(Data.Name != Name)
	{
		TVITEM ItemInfo;

		ItemInfo.mask = TVIF_TEXT;
		ItemInfo.hItem = Data.TreeViewItem;
		ItemInfo.state = 0;
		ItemInfo.stateMask = 0;
		ItemInfo.pszText = const_cast<CHAR*>(Data.Name.c_str());
		ItemInfo.cchTextMax = static_cast<INT>(Data.Name.size());
		ItemInfo.iImage = 0;
		ItemInfo.iSelectedImage = 0;
		ItemInfo.cChildren = 0;
		ItemInfo.lParam = 0;

		TreeView_SetItem(TreeView.GetWindow(), &ItemInfo);
	}

	EventObject->Data() = Data;
	if(GlobalSequence != NULL)
	{
		EventObject->GlobalSequenceNode.Attach(GlobalSequence->EventObjectNodes);
	}
	else
	{
		EventObject->GlobalSequenceNode.Detach();
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Edits an existing helper
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::EditHelper(MODEL_HELPER* Helper)
{
	if(!HelperDialog.Display(Window, Helper->Data())) return TRUE;

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Edits an existing light
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::EditLight(MODEL_LIGHT* Light)
{
	if(!LightDialog.Display(Window, Light->Data())) return TRUE;

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Edits an existing particle emitter
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::EditParticleEmitter(MODEL_PARTICLE_EMITTER* ParticleEmitter)
{
	if(!ParticleEmitterDialog.Display(Window, ParticleEmitter->Data())) return TRUE;

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Edits an existing particle emitter 2
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::EditParticleEmitter2(MODEL_PARTICLE_EMITTER_2* ParticleEmitter2)
{
	MODEL_TEXTURE* Texture;

	Texture = ParticleEmitter2->TextureNode.IsAttached() ? ParticleEmitter2->TextureNode.GetObjectData() : NULL;

	if(!ParticleEmitter2Dialog.Display(Window, ParticleEmitter2->Data(), Texture)) return TRUE;

	if(Texture != NULL)
	{
		ParticleEmitter2->TextureNode.Attach(Texture->ParticleEmitter2Nodes);
	}
	else
	{
		ParticleEmitter2->TextureNode.Detach();
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Edits an existing ribbon emitter
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::EditRibbonEmitter(MODEL_RIBBON_EMITTER* RibbonEmitter)
{
	MODEL_MATERIAL* Material;

	Material = RibbonEmitter->MaterialNode.IsAttached() ? RibbonEmitter->MaterialNode.GetObjectData() : NULL;

	if(!RibbonEmitterDialog.Display(Window, RibbonEmitter->Data(), Material)) return TRUE;

	if(Material != NULL)
	{
		RibbonEmitter->MaterialNode.Attach(Material->RibbonEmitterNodes);
	}
	else
	{
		RibbonEmitter->MaterialNode.Detach();
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Moves a node up
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::MoveUp()
{
	MODEL_BASE* Node;
	HTREEITEM Item;
	HTREEITEM InsertionPoint;

	Node = CurrentlySelectedNode;
	if(Node == NULL) return FALSE;

	Item = TreeView_GetPrevSibling(TreeView.GetWindow(), Node->BaseData()->TreeViewItem);
	if(Item == NULL)
	{
		Error.SetMessage("Cannot move the node upwards!");
		return FALSE;
	}

	InsertionPoint = TreeView_GetPrevSibling(TreeView.GetWindow(), Item);
	if(InsertionPoint == NULL) InsertionPoint = TVI_FIRST;

	RemoveNode(Node);
	AddNodeAndChildren(Node, InsertionPoint);

	TreeView_SelectItem(TreeView.GetWindow(), Node->BaseData()->TreeViewItem);
	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Moves a node down
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::MoveDown()
{
	MODEL_BASE* Node;
	HTREEITEM Item;
	HTREEITEM InsertionPoint;

	Node = CurrentlySelectedNode;
	if(Node == NULL) return FALSE;

	Item = TreeView_GetNextSibling(TreeView.GetWindow(), Node->BaseData()->TreeViewItem);
	if(Item == NULL)
	{
		Error.SetMessage("Cannot move the node downwards!");
		return FALSE;
	}

	InsertionPoint = Item;

	RemoveNode(Node);
	AddNodeAndChildren(Node, InsertionPoint);

	TreeView_SelectItem(TreeView.GetWindow(), Node->BaseData()->TreeViewItem);
	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Moves a node left
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::MoveLeft()
{
	MODEL_BASE* Node;
	MODEL_BASE* TempNode;
	HTREEITEM Item;
	HTREEITEM InsertionPoint;

	Node = CurrentlySelectedNode;
	if(Node == NULL) return FALSE;

	Item = TreeView_GetParent(TreeView.GetWindow(), Node->BaseData()->TreeViewItem);
	if(Item == NULL)
	{
		Error.SetMessage("Cannot move the node to the left!");
		return FALSE;
	}

	TempNode = Node->ParentNode.GetObjectData();
	Node->ParentNode.Attach(TempNode->ParentNode.GetObjectData()->ChildNodes);

	InsertionPoint = TempNode->BaseData()->TreeViewItem;

	RemoveNode(Node);
	AddNodeAndChildren(Node, InsertionPoint);

	TreeView_SelectItem(TreeView.GetWindow(), Node->BaseData()->TreeViewItem);
	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Moves a node right
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::MoveRight()
{
	MODEL_BASE* Node;
	MODEL_BASE* TempNode;
	HTREEITEM Item;
	TVITEM TreeViewItem;

	Node = CurrentlySelectedNode;
	if(Node == NULL) return FALSE;

	Item = TreeView_GetPrevSibling(TreeView.GetWindow(), Node->BaseData()->TreeViewItem);
	if(Item == NULL)
	{
		Error.SetMessage("Cannot move the node to the right!");
		return FALSE;
	}

	ZeroMemory(&TreeViewItem, sizeof(TVITEM));
	TreeViewItem.mask = TVIF_PARAM;
	TreeViewItem.hItem = Item;

	TreeView_GetItem(TreeView.GetWindow(), &TreeViewItem);

	TempNode = reinterpret_cast<MODEL_BASE*>(TreeViewItem.lParam);
	Node->ParentNode.Attach(TempNode->ChildNodes);

	RemoveNode(Node);
	AddNodeAndChildren(Node, TVI_LAST);

	TreeView_SelectItem(TreeView.GetWindow(), Node->BaseData()->TreeViewItem);
	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes an existing node
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::Remove()
{
	MODEL_BASE* Node;

	Node = CurrentlySelectedNode;
	if(Node == NULL) return FALSE;

	switch(Node->BaseData()->Type)
	{
		case NODE_TYPE_ATTACHMENT:
		{
			if(!Model.RemoveAttachment(reinterpret_cast<MODEL_ATTACHMENT*>(Node), Window)) return FALSE;
			MainWindow.MakeModelUnsaved();
			break;
		}

		case NODE_TYPE_BONE:
		{
			if(!Model.RemoveBone(reinterpret_cast<MODEL_BONE*>(Node), Window)) return FALSE;
			MainWindow.MakeModelUnsaved();
			break;
		}

		case NODE_TYPE_EVENT_OBJECT:
		{
			if(!Model.RemoveEventObject(reinterpret_cast<MODEL_EVENT_OBJECT*>(Node), Window)) return FALSE;
			MainWindow.MakeModelUnsaved();
			break;
		}

		case NODE_TYPE_COLLISION_SHAPE:
		{
			if(!Model.RemoveCollisionShape(reinterpret_cast<MODEL_COLLISION_SHAPE*>(Node), Window)) return FALSE;
			MainWindow.MakeModelUnsaved();
			break;
		}

		case NODE_TYPE_HELPER:
		{
			if(!Model.RemoveHelper(reinterpret_cast<MODEL_HELPER*>(Node), Window)) return FALSE;
			MainWindow.MakeModelUnsaved();
			break;
		}

		case NODE_TYPE_LIGHT:
		{
			if(!Model.RemoveLight(reinterpret_cast<MODEL_LIGHT*>(Node), Window)) return FALSE;
			MainWindow.MakeModelUnsaved();
			break;
		}

		case NODE_TYPE_PARTICLE_EMITTER:
		{
			if(!Model.RemoveParticleEmitter(reinterpret_cast<MODEL_PARTICLE_EMITTER*>(Node), Window)) return FALSE;
			MainWindow.MakeModelUnsaved();
			break;
		}

		case NODE_TYPE_PARTICLE_EMITTER_2:
		{
			if(!Model.RemoveParticleEmitter2(reinterpret_cast<MODEL_PARTICLE_EMITTER_2*>(Node), Window)) return FALSE;
			MainWindow.MakeModelUnsaved();
			break;
		}

		case NODE_TYPE_RIBBON_EMITTER:
		{
			if(!Model.RemoveRibbonEmitter(reinterpret_cast<MODEL_RIBBON_EMITTER*>(Node), Window)) return FALSE;
			MainWindow.MakeModelUnsaved();
			break;
		}
	}

	CurrentlySelectedNode = NULL;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Edits an existing node
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::Edit()
{
	MODEL_BASE* Node;

	Node = CurrentlySelectedNode;
	if(Node == NULL) return FALSE;

	switch(Node->BaseData()->Type)
	{
		case NODE_TYPE_ATTACHMENT:
		{
			if(!EditAttachment(reinterpret_cast<MODEL_ATTACHMENT*>(Node))) return FALSE;
			break;
		}

		case NODE_TYPE_BONE:
		{
			if(!EditBone(reinterpret_cast<MODEL_BONE*>(Node))) return FALSE;
			break;
		}

		case NODE_TYPE_EVENT_OBJECT:
		{
			if(!EditEventObject(reinterpret_cast<MODEL_EVENT_OBJECT*>(Node))) return FALSE;
			break;
		}

		case NODE_TYPE_COLLISION_SHAPE:
		{
			if(!EditCollisionShape(reinterpret_cast<MODEL_COLLISION_SHAPE*>(Node))) return FALSE;
			break;
		}

		case NODE_TYPE_HELPER:
		{
			if(!EditHelper(reinterpret_cast<MODEL_HELPER*>(Node))) return FALSE;
			break;
		}

		case NODE_TYPE_LIGHT:
		{
			if(!EditLight(reinterpret_cast<MODEL_LIGHT*>(Node))) return FALSE;
			break;
		}

		case NODE_TYPE_PARTICLE_EMITTER:
		{
			if(!EditParticleEmitter(reinterpret_cast<MODEL_PARTICLE_EMITTER*>(Node))) return FALSE;
			break;
		}

		case NODE_TYPE_PARTICLE_EMITTER_2:
		{
			if(!EditParticleEmitter2(reinterpret_cast<MODEL_PARTICLE_EMITTER_2*>(Node))) return FALSE;
			break;
		}

		case NODE_TYPE_RIBBON_EMITTER:
		{
			if(!EditRibbonEmitter(reinterpret_cast<MODEL_RIBBON_EMITTER*>(Node))) return FALSE;
			break;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Edits an existing base node
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::EditNode()
{
	std::string Name;
	MODEL_BASE_DATA Data;

	if(CurrentlySelectedNode == NULL) return FALSE;

	Data = *(CurrentlySelectedNode->BaseData());
	Name = Data.Name;

	if(!NodeDialog.Display(Window, Data, (Data.Type == NODE_TYPE_EVENT_OBJECT))) return TRUE;

	if(Data.Name != Name)
	{
		TVITEM ItemInfo;

		if(Data.Type == NODE_TYPE_EVENT_OBJECT)
		{
			Error.SetMessage("You cannot explicitly change the name of Event Objects!");
			return FALSE;
		}

		ItemInfo.mask = TVIF_TEXT;
		ItemInfo.hItem = Data.TreeViewItem;
		ItemInfo.state = 0;
		ItemInfo.stateMask = 0;
		ItemInfo.pszText = const_cast<CHAR*>(Data.Name.c_str());
		ItemInfo.cchTextMax = static_cast<INT>(Data.Name.size());
		ItemInfo.iImage = 0;
		ItemInfo.iSelectedImage = 0;
		ItemInfo.cChildren = 0;
		ItemInfo.lParam = 0;

		TreeView_SetItem(TreeView.GetWindow(), &ItemInfo);
	}

	*(CurrentlySelectedNode->BaseData()) = Data;
	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Duplicates an existing node
//+-----------------------------------------------------------------------------
BOOL NODE_MANAGER_WINDOW::Duplicate()
{
	MODEL_BASE* Node;

	Node = GetCurrentParent();
	if(Node == NULL) return FALSE;

	switch(Node->BaseData()->Type)
	{
		case NODE_TYPE_ATTACHMENT:
		{
			MODEL_ATTACHMENT* Attachment;

			Attachment = new MODEL_ATTACHMENT();
			if(Attachment == NULL)
			{
				Error.SetMessage("Unable to create a new attachment, memory allocation failed!");
				return FALSE;
			}

			Attachment->Data() = reinterpret_cast<MODEL_ATTACHMENT*>(Node)->Data();

			if(!Model.AddAttachment(Attachment, Node))
			{
				delete Attachment;
				return FALSE;
			}

			break;
		}

		case NODE_TYPE_BONE:
		{
			MODEL_BONE* Bone;

			Bone = new MODEL_BONE();
			if(Bone == NULL)
			{
				Error.SetMessage("Unable to create a new bone, memory allocation failed!");
				return FALSE;
			}

			Bone->Data() = reinterpret_cast<MODEL_BONE*>(Node)->Data();

			if(!Model.AddBone(Bone, Node))
			{
				delete Bone;
				return FALSE;
			}

			break;
		}

		case NODE_TYPE_COLLISION_SHAPE:
		{
			MODEL_COLLISION_SHAPE* CollisionShape;

			CollisionShape = new MODEL_COLLISION_SHAPE();
			if(CollisionShape == NULL)
			{
				Error.SetMessage("Unable to create a new collision shape, memory allocation failed!");
				return FALSE;
			}

			CollisionShape->Data() = reinterpret_cast<MODEL_COLLISION_SHAPE*>(Node)->Data();

			if(!Model.AddCollisionShape(CollisionShape, Node))
			{
				delete CollisionShape;
				return FALSE;
			}

			break;
		}

		case NODE_TYPE_EVENT_OBJECT:
		{
			MODEL_EVENT_OBJECT* EventObject;

			EventObject = new MODEL_EVENT_OBJECT();
			if(EventObject == NULL)
			{
				Error.SetMessage("Unable to create a new event object, memory allocation failed!");
				return FALSE;
			}

			EventObject->Data() = reinterpret_cast<MODEL_EVENT_OBJECT*>(Node)->Data();

			if(!Model.AddEventObject(EventObject, Node))
			{
				delete EventObject;
				return FALSE;
			}

			break;
		}

		case NODE_TYPE_HELPER:
		{
			MODEL_HELPER* Helper;

			Helper = new MODEL_HELPER();
			if(Helper == NULL)
			{
				Error.SetMessage("Unable to create a new helper, memory allocation failed!");
				return FALSE;
			}

			Helper->Data() = reinterpret_cast<MODEL_HELPER*>(Node)->Data();

			if(!Model.AddHelper(Helper, Node))
			{
				delete Helper;
				return FALSE;
			}

			break;
		}

		case NODE_TYPE_LIGHT:
		{
			MODEL_LIGHT* Light;

			Light = new MODEL_LIGHT();
			if(Light == NULL)
			{
				Error.SetMessage("Unable to create a new light, memory allocation failed!");
				return FALSE;
			}

			Light->Data() = reinterpret_cast<MODEL_LIGHT*>(Node)->Data();

			if(!Model.AddLight(Light, Node))
			{
				delete Light;
				return FALSE;
			}

			break;
		}

		case NODE_TYPE_PARTICLE_EMITTER:
		{
			MODEL_PARTICLE_EMITTER* ParticleEmitter;

			ParticleEmitter = new MODEL_PARTICLE_EMITTER();
			if(ParticleEmitter == NULL)
			{
				Error.SetMessage("Unable to create a new particle emitter, memory allocation failed!");
				return FALSE;
			}

			ParticleEmitter->Data() = reinterpret_cast<MODEL_PARTICLE_EMITTER*>(Node)->Data();

			if(!Model.AddParticleEmitter(ParticleEmitter, Node))
			{
				delete ParticleEmitter;
				return FALSE;
			}

			break;
		}

		case NODE_TYPE_PARTICLE_EMITTER_2:
		{
			MODEL_PARTICLE_EMITTER_2* ParticleEmitter2;

			ParticleEmitter2 = new MODEL_PARTICLE_EMITTER_2();
			if(ParticleEmitter2 == NULL)
			{
				Error.SetMessage("Unable to create a new particle emitter 2, memory allocation failed!");
				return FALSE;
			}

			ParticleEmitter2->Data() = reinterpret_cast<MODEL_PARTICLE_EMITTER_2*>(Node)->Data();

			if(!Model.AddParticleEmitter2(ParticleEmitter2, Node))
			{
				delete ParticleEmitter2;
				return FALSE;
			}

			break;
		}

		case NODE_TYPE_RIBBON_EMITTER:
		{
			MODEL_RIBBON_EMITTER* RibbonEmitter;

			RibbonEmitter = new MODEL_RIBBON_EMITTER();
			if(RibbonEmitter == NULL)
			{
				Error.SetMessage("Unable to create a new ribbon emitter, memory allocation failed!");
				return FALSE;
			}

			RibbonEmitter->Data() = reinterpret_cast<MODEL_RIBBON_EMITTER*>(Node)->Data();

			if(!Model.AddRibbonEmitter(RibbonEmitter, Node))
			{
				delete RibbonEmitter;
				return FALSE;
			}

			break;
		}
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Resizes the treeview
//+-----------------------------------------------------------------------------
VOID NODE_MANAGER_WINDOW::ResizeTreeView()
{
	TreeView.SetDimension(GetWidth(), GetHeight());
}
