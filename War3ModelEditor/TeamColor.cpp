//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "TeamColor.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
TEAM_COLOR TeamColor;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
TEAM_COLOR::TEAM_COLOR()
{
	CurrentTeamColor = TEAM_COLOR_TYPE_RED;
	CurrentTeamColorId = MainTeamColorRed;

	RegisterTeamColor(TEAM_COLOR_TYPE_RED,        MainTeamColorRed);
	RegisterTeamColor(TEAM_COLOR_TYPE_BLUE,       MainTeamColorBlue);
	RegisterTeamColor(TEAM_COLOR_TYPE_TEAL,       MainTeamColorTeal);
	RegisterTeamColor(TEAM_COLOR_TYPE_PURPLE,     MainTeamColorPurple);
	RegisterTeamColor(TEAM_COLOR_TYPE_YELLOW,     MainTeamColorYellow);
	RegisterTeamColor(TEAM_COLOR_TYPE_ORANGE,     MainTeamColorOrange);
	RegisterTeamColor(TEAM_COLOR_TYPE_GREEN,      MainTeamColorGreen);
	RegisterTeamColor(TEAM_COLOR_TYPE_PINK,       MainTeamColorPink);
	RegisterTeamColor(TEAM_COLOR_TYPE_GRAY,       MainTeamColorGray);
	RegisterTeamColor(TEAM_COLOR_TYPE_LIGHT_BLUE, MainTeamColorLightBlue);
	RegisterTeamColor(TEAM_COLOR_TYPE_DARK_GREEN, MainTeamColorDarkGreen);
	RegisterTeamColor(TEAM_COLOR_TYPE_BROWN,      MainTeamColorBrown);
	RegisterTeamColor(TEAM_COLOR_TYPE_BLACK,      MainTeamColorBlack);
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
TEAM_COLOR::~TEAM_COLOR()
{
	IdToColorMap.clear();
	ColorToIdMap.clear();
}


//+-----------------------------------------------------------------------------
//| Sets the team color icons for a menu
//+-----------------------------------------------------------------------------
VOID TEAM_COLOR::SetTeamColorIcons(HMENU Menu)
{
	::SetMenuItemBitmaps(Menu, MainTeamColorRed,       0, LoadBitmap(BitmapU00), LoadBitmap(BitmapC00));
	::SetMenuItemBitmaps(Menu, MainTeamColorBlue,      0, LoadBitmap(BitmapU01), LoadBitmap(BitmapC01));
	::SetMenuItemBitmaps(Menu, MainTeamColorTeal,      0, LoadBitmap(BitmapU02), LoadBitmap(BitmapC02));
	::SetMenuItemBitmaps(Menu, MainTeamColorPurple,    0, LoadBitmap(BitmapU03), LoadBitmap(BitmapC03));
	::SetMenuItemBitmaps(Menu, MainTeamColorYellow,    0, LoadBitmap(BitmapU04), LoadBitmap(BitmapC04));
	::SetMenuItemBitmaps(Menu, MainTeamColorOrange,    0, LoadBitmap(BitmapU05), LoadBitmap(BitmapC05));
	::SetMenuItemBitmaps(Menu, MainTeamColorGreen,     0, LoadBitmap(BitmapU06), LoadBitmap(BitmapC06));
	::SetMenuItemBitmaps(Menu, MainTeamColorPink,      0, LoadBitmap(BitmapU07), LoadBitmap(BitmapC07));
	::SetMenuItemBitmaps(Menu, MainTeamColorGray,      0, LoadBitmap(BitmapU08), LoadBitmap(BitmapC08));
	::SetMenuItemBitmaps(Menu, MainTeamColorLightBlue, 0, LoadBitmap(BitmapU09), LoadBitmap(BitmapC09));
	::SetMenuItemBitmaps(Menu, MainTeamColorDarkGreen, 0, LoadBitmap(BitmapU10), LoadBitmap(BitmapC10));
	::SetMenuItemBitmaps(Menu, MainTeamColorBrown,     0, LoadBitmap(BitmapU11), LoadBitmap(BitmapC11));
	::SetMenuItemBitmaps(Menu, MainTeamColorBlack,     0, LoadBitmap(BitmapU12), LoadBitmap(BitmapC12));
}


//+-----------------------------------------------------------------------------
//| Sets a new current team color
//+-----------------------------------------------------------------------------
VOID TEAM_COLOR::SetCurrentTeamColor(TEAM_COLOR_TYPE NewCurrentTeamColor)
{
	std::map<TEAM_COLOR_TYPE, INT>::iterator i;

	CurrentTeamColor = NewCurrentTeamColor;

	i = ColorToIdMap.find(CurrentTeamColor);
	if(i != ColorToIdMap.end()) CurrentTeamColorId = i->second;
}


//+-----------------------------------------------------------------------------
//| Returns the current team color
//+-----------------------------------------------------------------------------
TEAM_COLOR_TYPE TEAM_COLOR::GetCurrentTeamColor() CONST
{
	return CurrentTeamColor;
}


//+-----------------------------------------------------------------------------
//| Sets a new current team color id
//+-----------------------------------------------------------------------------
VOID TEAM_COLOR::SetCurrentTeamColorId(INT NewCurrentTeamColorId)
{
	std::map<INT, TEAM_COLOR_TYPE>::iterator i;

	CurrentTeamColorId = NewCurrentTeamColorId;

	i = IdToColorMap.find(CurrentTeamColorId);
	if(i != IdToColorMap.end()) CurrentTeamColor = i->second;
}


//+-----------------------------------------------------------------------------
//| Returns the current team color id
//+-----------------------------------------------------------------------------
INT TEAM_COLOR::GetCurrentTeamColorId() CONST
{
	return CurrentTeamColorId;
}


//+-----------------------------------------------------------------------------
//| Registers a team color and its associated menu id
//+-----------------------------------------------------------------------------
VOID TEAM_COLOR::RegisterTeamColor(TEAM_COLOR_TYPE TeamColor, INT TeamColorId)
{
	IdToColorMap.insert(std::make_pair(TeamColorId, TeamColor));
	ColorToIdMap.insert(std::make_pair(TeamColor, TeamColorId));
}


//+-----------------------------------------------------------------------------
//| Loads a resource bitmap
//+-----------------------------------------------------------------------------
HBITMAP TEAM_COLOR::LoadBitmap(INT BitmapId)
{
	return ::LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(BitmapId));
}
