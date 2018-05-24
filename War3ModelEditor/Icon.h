//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_ICON_H
#define MAGOS_ICON_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Container.h"


//+-----------------------------------------------------------------------------
//| Icon info structure
//+-----------------------------------------------------------------------------
struct ICON_INFO
{
	ICON_INFO()
	{
		Icon = NULL;
		IconCollectionIndex = INVALID_INDEX;
	}

	HICON Icon;
	INT IconCollectionIndex;
};


//+-----------------------------------------------------------------------------
//| Icon class
//+-----------------------------------------------------------------------------
class ICON
{
	public:
		CONSTRUCTOR ICON();
		DESTRUCTOR ~ICON();

		VOID ClearAllIcons();
		BOOL LoadAllIcons();

		INT GetFolderIconIndex() CONST;
		INT GetUnknownIconIndex() CONST;
		INT GetIconIndex(CONST std::string& Extention) CONST;

		VOID AssignIconListToTreeView(HWND TreeView);

	protected:
		BOOL LoadIconFromFile(CONST std::string& FileName);
		BOOL LoadIconFromResource(CONST std::string& Name, UINT ResourceId);

		INT FolderIconIndex;
		INT UnknownIconIndex;

		HIMAGELIST IconCollection;
		CONTAINER<ICON_INFO> IconInfoContainer;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern ICON Icon;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
