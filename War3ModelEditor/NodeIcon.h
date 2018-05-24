//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_NODE_ICON_H
#define MAGOS_NODE_ICON_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Container.h"


//+-----------------------------------------------------------------------------
//| Node icon info structure
//+-----------------------------------------------------------------------------
struct NODE_ICON_INFO
{
	NODE_ICON_INFO()
	{
		Icon = NULL;
		IconCollectionIndex = INVALID_INDEX;
	}

	HICON Icon;
	INT IconCollectionIndex;
};


//+-----------------------------------------------------------------------------
//| Node icon class
//+-----------------------------------------------------------------------------
class NODE_ICON
{
	public:
		CONSTRUCTOR NODE_ICON();
		DESTRUCTOR ~NODE_ICON();

		VOID ClearAllIcons();
		BOOL LoadAllIcons();

		INT GetIconIndex(NODE_TYPE NodeType) CONST;

		VOID AssignIconListToTreeView(HWND TreeView);

	protected:
		BOOL LoadIconFromResource(NODE_TYPE NodeType, UINT ResourceId);

		HIMAGELIST NodeIconCollection;
		std::map<NODE_TYPE, NODE_ICON_INFO> NodeIconInfoMap;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern NODE_ICON NodeIcon;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
