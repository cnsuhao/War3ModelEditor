//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_TEAM_COLOR_H
#define MAGOS_TEAM_COLOR_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Common.h"


//+-----------------------------------------------------------------------------
//| Enumeration of team colors
//+-----------------------------------------------------------------------------
enum TEAM_COLOR_TYPE
{
	TEAM_COLOR_TYPE_RED,
	TEAM_COLOR_TYPE_BLUE,
	TEAM_COLOR_TYPE_TEAL,
	TEAM_COLOR_TYPE_PURPLE,
	TEAM_COLOR_TYPE_YELLOW,
	TEAM_COLOR_TYPE_ORANGE,
	TEAM_COLOR_TYPE_GREEN,
	TEAM_COLOR_TYPE_PINK,
	TEAM_COLOR_TYPE_GRAY,
	TEAM_COLOR_TYPE_LIGHT_BLUE,
	TEAM_COLOR_TYPE_DARK_GREEN,
	TEAM_COLOR_TYPE_BROWN,
	TEAM_COLOR_TYPE_BLACK,
};


//+-----------------------------------------------------------------------------
//| Team color class
//+-----------------------------------------------------------------------------
class TEAM_COLOR
{
	public:
		CONSTRUCTOR TEAM_COLOR();
		DESTRUCTOR ~TEAM_COLOR();

		VOID SetTeamColorIcons(HMENU Menu);

		VOID SetCurrentTeamColor(TEAM_COLOR_TYPE NewCurrentTeamColor);
		TEAM_COLOR_TYPE GetCurrentTeamColor() CONST;

		VOID SetCurrentTeamColorId(INT NewCurrentTeamColorId);
		INT GetCurrentTeamColorId() CONST;

	protected:
		VOID RegisterTeamColor(TEAM_COLOR_TYPE TeamColor, INT TeamColorId);

		HBITMAP LoadBitmap(INT BitmapId);

		TEAM_COLOR_TYPE CurrentTeamColor;
		INT CurrentTeamColorId;

		std::map<INT, TEAM_COLOR_TYPE> IdToColorMap;
		std::map<TEAM_COLOR_TYPE, INT> ColorToIdMap;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern TEAM_COLOR TeamColor;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
