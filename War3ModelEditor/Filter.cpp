//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Filter.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
FILTER Filter;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
FILTER::FILTER()
{
	OtherFilterActive = TRUE;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
FILTER::~FILTER()
{
	ClearAllFilters();
}


//+-----------------------------------------------------------------------------
//| Clears all filters
//+-----------------------------------------------------------------------------
VOID FILTER::ClearAllFilters()
{
	FilterInfoContainer.Clear();
}


//+-----------------------------------------------------------------------------
//| Loads all filters
//+-----------------------------------------------------------------------------
BOOL FILTER::LoadAllFilters()
{
	BUFFER Buffer;
	CHAR Char;
	std::string Token;
	std::string FileName;
	TOKEN_IN_STREAM TokenStream;
	FILTER_INFO FilterInfo;

	ClearAllFilters();

	FileName = Common.GetProgramDirectory() + "\\" + PATH_FILTERS;
	if(!FileLoader.LoadFromFile(FileName, Buffer)) return TRUE;

	TokenStream.SetFileName(PATH_FILTERS);

	if(!TokenStream.Load(Buffer)) return FALSE;

	while(!TokenStream.Eof())
	{
		FilterInfo = FILTER_INFO();

		Token = TokenStream.ReadWord();
		if(Token == "") break;
		if(Token != "Filter")
		{
			Error.SetMessage("Expected \"Filter\", got \"" + Token + "\" in \"" + PATH_FILTERS + "\"!");
			return FALSE;
		}

		FilterInfo.Name = TokenStream.ReadString();
		if(!TokenStream.ExpectChar('{')) return FALSE;

		while(TRUE)
		{
			if(TokenStream.Eof())
			{
				Error.SetMessage("Unexpected EOF reached in \"" + PATH_FILTERS + "\"!");
				return FALSE;
			}

			Token = Common.LowerCase(TokenStream.ReadString());
			FilterInfo.ExtentionList.insert(Token);

			Char = TokenStream.ReadChar();
			if(Char == '}') break;
			if(Char != ',')
			{
				Error.SetMessage(std::string("Expected \",\", got \"") + Char + "\" in \"" + PATH_FILTERS + "\"!");
				return FALSE;
			}
		}

		FilterInfo.MenuItemId = MENU_FILTER_BASE + FilterInfoContainer.GetSize();

		if(!FilterInfoContainer.Add(FilterInfo.Name, FilterInfo))
		{
			Error.SetMessage("Unable to add the filter \"" + FilterInfo.Name + "\"!");
			return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Selects the all-filter
//+-----------------------------------------------------------------------------
VOID FILTER::SelectAll()
{
	INT i;

	OtherFilterActive = TRUE;

	for(i = 0; i < FilterInfoContainer.GetSize(); i++)
	{
		FilterInfoContainer[i].Active = TRUE;
	}
}


//+-----------------------------------------------------------------------------
//| Selects the none-filter
//+-----------------------------------------------------------------------------
VOID FILTER::SelectNone()
{
	INT i;

	OtherFilterActive = FALSE;

	for(i = 0; i < FilterInfoContainer.GetSize(); i++)
	{
		FilterInfoContainer[i].Active = FALSE;
	}
}


//+-----------------------------------------------------------------------------
//| Selects the other-filter
//+-----------------------------------------------------------------------------
VOID FILTER::SelectOther()
{
	OtherFilterActive = !OtherFilterActive;
}


//+-----------------------------------------------------------------------------
//| Selects a generic menu item
//+-----------------------------------------------------------------------------
VOID FILTER::Select(UINT MenuItem)
{
	INT i;

	for(i = 0; i < FilterInfoContainer.GetSize(); i++)
	{
		if(MenuItem == FilterInfoContainer[i].MenuItemId)
		{
			FilterInfoContainer[i].Active = !FilterInfoContainer[i].Active;
			break;
		}
	}
}


//+-----------------------------------------------------------------------------
//| Returns the nr of filters
//+-----------------------------------------------------------------------------
INT FILTER::GetNrOfFilters()
{
	return FilterInfoContainer.GetSize();
}


//+-----------------------------------------------------------------------------
//| Returns the minimum menu item id
//+-----------------------------------------------------------------------------
UINT FILTER::GetMinMenuItemId() CONST
{
	return MENU_FILTER_BASE;
}


//+-----------------------------------------------------------------------------
//| Returns the maximum menu item id
//+-----------------------------------------------------------------------------
UINT FILTER::GetMaxMenuItemId() CONST
{
	return MENU_FILTER_BASE + FilterInfoContainer.GetSize() - 1;
}


//+-----------------------------------------------------------------------------
//| Updates the checked/unchecked items in the given menu
//+-----------------------------------------------------------------------------
VOID FILTER::UpdateFilterMenu(HMENU Menu)
{
	INT i;

	CheckMenuItem(Menu, MpqFiltersOther, BoolToCheckType(OtherFilterActive));

	for(i = 0; i < FilterInfoContainer.GetSize(); i++)
	{
		CheckMenuItem(Menu, FilterInfoContainer[i].MenuItemId, BoolToCheckType(FilterInfoContainer[i].Active));
	}
}


//+-----------------------------------------------------------------------------
//| Adds the filter items to the given menu
//+-----------------------------------------------------------------------------
BOOL FILTER::AddFilterItemsToMenu(HMENU Menu)
{
	INT i;

	for(i = 0; i < FilterInfoContainer.GetSize(); i++)
	{
		if(!InsertMenu(Menu, MpqFiltersOther, MF_BYCOMMAND | MF_STRING, FilterInfoContainer[i].MenuItemId, FilterInfoContainer[i].Name.c_str()))
		{
			Error.SetMessage("Unable to add the menu item \"" + FilterInfoContainer[i].Name + "\"!");
			return FALSE;
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Builds a filter
//+-----------------------------------------------------------------------------
VOID FILTER::BuildFilter(FILTER_DATA& FilterData, CONST std::string& Extention) CONST
{
	INT i;
	BOOL ExtentionFound = FALSE;
	std::set<std::string>::const_iterator j;

	if(Extention == "") return;

	for(i = 0; i < FilterInfoContainer.GetSize(); i++)
	{
		j = FilterInfoContainer[i].ExtentionList.find(Extention);
		if(j != FilterInfoContainer[i].ExtentionList.end())
		{
			FilterData.FilterList[i] = TRUE;
			ExtentionFound = TRUE;
		}
	}

	FilterData.OtherFilter = !ExtentionFound;
}


//+-----------------------------------------------------------------------------
//| Extends a filter with more info
//+-----------------------------------------------------------------------------
VOID FILTER::ExtendFilter(FILTER_DATA& FilterData, CONST FILTER_DATA& NewFilterData) CONST
{
	INT i;

	for(i = 0; i < FilterInfoContainer.GetSize(); i++)
	{
		if(NewFilterData.FilterList[i]) FilterData.FilterList[i] = TRUE;
	}

	if(NewFilterData.OtherFilter) FilterData.OtherFilter = TRUE;
}


//+-----------------------------------------------------------------------------
//| Checks if a filter is valid (should be displayed)
//+-----------------------------------------------------------------------------
BOOL FILTER::ValidFilter(CONST FILTER_DATA& FilterData) CONST
{
	INT i;

	if(OtherFilterActive && FilterData.OtherFilter) return TRUE;

	for(i = 0; i < FilterInfoContainer.GetSize(); i++)
	{
		if(FilterInfoContainer[i].Active && FilterData.FilterList[i]) return TRUE;
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Converts a bool to a menu item check type
//+-----------------------------------------------------------------------------
UINT FILTER::BoolToCheckType(BOOL Bool)
{
	return (Bool ? MF_CHECKED : MF_UNCHECKED);
}
