//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_HISTORY_H
#define MAGOS_MODEL_HISTORY_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Common.h"


//+-----------------------------------------------------------------------------
//| Constants
//+-----------------------------------------------------------------------------
CONST INT MAX_NR_OF_HISTORY_ITEMS = 8;


//+-----------------------------------------------------------------------------
//| Model history node structure
//+-----------------------------------------------------------------------------
struct MODEL_HISTORY_NODE
{
	MODEL_HISTORY_NODE()
	{
		Id = 0;
		FileName = "";
		ShortFileName = "";
	}

	UINT Id;
	std::string FileName;
	std::string ShortFileName;
};


//+-----------------------------------------------------------------------------
//| Model history class
//+-----------------------------------------------------------------------------
class MODEL_HISTORY
{
	public:
		CONSTRUCTOR MODEL_HISTORY();
		DESTRUCTOR ~MODEL_HISTORY();

		VOID Clear();

		VOID AddFileName(CONST std::string& FileName);
		std::string GetFileName(UINT MenuItem);

		BOOL Save();
		BOOL Load();

		UINT GetMinMenuItemId() CONST;
		UINT GetMaxMenuItemId() CONST;

	protected:
		VOID RedrawMenu();

		std::list<MODEL_HISTORY_NODE> FileNameList;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern MODEL_HISTORY ModelHistory;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
