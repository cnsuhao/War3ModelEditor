//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "NodeIcon.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
NODE_ICON NodeIcon;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
NODE_ICON::NODE_ICON()
{
	NodeIconCollection = NULL;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
NODE_ICON::~NODE_ICON()
{
	ClearAllIcons();
}


//+-----------------------------------------------------------------------------
//| Clears all icons
//+-----------------------------------------------------------------------------
VOID NODE_ICON::ClearAllIcons()
{
	NodeIconInfoMap.clear();

	if(NodeIconCollection != NULL)
	{
		ImageList_Destroy(NodeIconCollection);
		NodeIconCollection = NULL;
	}
}


//+-----------------------------------------------------------------------------
//| Loads all icons
//+-----------------------------------------------------------------------------
BOOL NODE_ICON::LoadAllIcons()
{
	INT Index;
	std::map<NODE_TYPE, NODE_ICON_INFO>::iterator i;

	if(!LoadIconFromResource(NODE_TYPE_NONE, IconBone)) return FALSE;
	if(!LoadIconFromResource(NODE_TYPE_ATTACHMENT, IconAttachment)) return FALSE;
	if(!LoadIconFromResource(NODE_TYPE_BONE, IconBone)) return FALSE;
	if(!LoadIconFromResource(NODE_TYPE_EVENT_OBJECT, IconEventObject)) return FALSE;
	if(!LoadIconFromResource(NODE_TYPE_COLLISION_SHAPE, IconCollisionShape)) return FALSE;
	if(!LoadIconFromResource(NODE_TYPE_HELPER, IconHelper)) return FALSE;
	if(!LoadIconFromResource(NODE_TYPE_LIGHT, IconLight)) return FALSE;
	if(!LoadIconFromResource(NODE_TYPE_PARTICLE_EMITTER, IconParticleEmitter)) return FALSE;
	if(!LoadIconFromResource(NODE_TYPE_PARTICLE_EMITTER_2, IconParticleEmitter2)) return FALSE;
	if(!LoadIconFromResource(NODE_TYPE_RIBBON_EMITTER, IconRibbonEmitter)) return FALSE;
	
	NodeIconCollection = ImageList_Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), ILC_COLOR8 | ILC_MASK, 1, 1);
	if(NodeIconCollection == NULL)
	{
		Error.SetMessage("Unable to create a node icon list!");
		return FALSE;
	}

	i = NodeIconInfoMap.begin();
	while(i != NodeIconInfoMap.end())
	{
		Index = ImageList_AddIcon(NodeIconCollection, i->second.Icon);
		if(Index == -1)
		{
			Error.SetMessage("Unable to add icons to the icon list!");
			return FALSE;
		}

		i->second.IconCollectionIndex = Index;

		i++;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Returns the icon index of a specific node
//+-----------------------------------------------------------------------------
INT NODE_ICON::GetIconIndex(NODE_TYPE NodeType) CONST
{
	std::map<NODE_TYPE, NODE_ICON_INFO>::const_iterator i;
	
	i = NodeIconInfoMap.find(NodeType);
	if(i == NodeIconInfoMap.end()) return 0;

	return i->second.IconCollectionIndex;
}


//+-----------------------------------------------------------------------------
//| Assigns the icon list to a treeview
//+-----------------------------------------------------------------------------
VOID NODE_ICON::AssignIconListToTreeView(HWND TreeView)
{
	TreeView_SetImageList(TreeView, NodeIconCollection, TVSIL_NORMAL);
}


//+-----------------------------------------------------------------------------
//| Loads an icon from a resource
//+-----------------------------------------------------------------------------
BOOL NODE_ICON::LoadIconFromResource(NODE_TYPE NodeType, UINT ResourceId)
{
	NODE_ICON_INFO NodeIconInfo;

	NodeIconInfo.Icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(ResourceId));
	if(NodeIconInfo.Icon == NULL)
	{
		Error.SetMessage("Unable to load the node icon!");
		return FALSE;
	}

	NodeIconInfoMap.insert(std::make_pair(NodeType, NodeIconInfo));

	return TRUE;
}
