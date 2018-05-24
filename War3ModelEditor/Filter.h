//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_FILTER_H
#define MAGOS_FILTER_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "TokenInStream.h"
#include "FileLoader.h"
#include "Container.h"


//+-----------------------------------------------------------------------------
//| Filter data structure
//+-----------------------------------------------------------------------------
struct FILTER_DATA
{
	FILTER_DATA()
	{
		OtherFilter = FALSE;
	}

	BOOL OtherFilter;
	std::vector<BOOL> FilterList;
};


//+-----------------------------------------------------------------------------
//| Filter info structure
//+-----------------------------------------------------------------------------
struct FILTER_INFO
{
	FILTER_INFO()
	{
		Active = TRUE;
		MenuItemId = 0;
		Name = "";
	}

	BOOL Active;
	INT MenuItemId;
	std::string Name;
	std::set<std::string> ExtentionList;
};


//+-----------------------------------------------------------------------------
//| Filter class
//+-----------------------------------------------------------------------------
class FILTER
{
	public:
		CONSTRUCTOR FILTER();
		DESTRUCTOR ~FILTER();

		VOID ClearAllFilters();
		BOOL LoadAllFilters();

		VOID SelectAll();
		VOID SelectNone();
		VOID SelectOther();
		VOID Select(UINT MenuItem);

		INT GetNrOfFilters();

		UINT GetMinMenuItemId() CONST;
		UINT GetMaxMenuItemId() CONST;

		VOID UpdateFilterMenu(HMENU Menu);
		BOOL AddFilterItemsToMenu(HMENU Menu);

		VOID BuildFilter(FILTER_DATA& FilterData, CONST std::string& Extention) CONST;
		VOID ExtendFilter(FILTER_DATA& FilterData, CONST FILTER_DATA& NewFilterData) CONST;
		BOOL ValidFilter(CONST FILTER_DATA& FilterData) CONST;

	protected:
		UINT BoolToCheckType(BOOL Bool);

		BOOL OtherFilterActive;

		CONTAINER<FILTER_INFO> FilterInfoContainer;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern FILTER Filter;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
