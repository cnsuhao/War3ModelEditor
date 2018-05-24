//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ModelHistory.h"
#include "MainWindow.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
MODEL_HISTORY ModelHistory;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_HISTORY::MODEL_HISTORY()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL_HISTORY::~MODEL_HISTORY()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Clears the model history
//+-----------------------------------------------------------------------------
VOID MODEL_HISTORY::Clear()
{
	FileNameList.clear();

	Save();
	RedrawMenu();
}


//+-----------------------------------------------------------------------------
//| Adds a new filename to the history
//+-----------------------------------------------------------------------------
VOID MODEL_HISTORY::AddFileName(CONST std::string& FileName)
{
	INT Id;
	MODEL_HISTORY_NODE Node;
	std::list<MODEL_HISTORY_NODE>::iterator i;

	Id = MENU_HISTORY_BASE + 1;

	i = FileNameList.begin();
	while(i != FileNameList.end())
	{
		if(i->FileName == FileName)
		{
			i = FileNameList.erase(i);
		}
		else
		{
			i->Id = Id;
			Id++;
			i++;
		}
	}

	Node.Id = MENU_HISTORY_BASE;
	Node.FileName = FileName;
	Node.ShortFileName = Common.GetFileName(FileName);

	if(FileNameList.size() >= MAX_NR_OF_HISTORY_ITEMS) FileNameList.pop_back();
	FileNameList.push_front(Node);

	Save();
	RedrawMenu();
}


//+-----------------------------------------------------------------------------
//| Returns the filename of a specific menu item
//+-----------------------------------------------------------------------------
std::string MODEL_HISTORY::GetFileName(UINT MenuItem)
{
	std::list<MODEL_HISTORY_NODE>::iterator i;

	i = FileNameList.begin();
	while(i != FileNameList.end())
	{
		if(i->Id == MenuItem) return i->FileName;
		i++;
	}

	return "";
}


//+-----------------------------------------------------------------------------
//| Saves the model history
//+-----------------------------------------------------------------------------
BOOL MODEL_HISTORY::Save()
{
	std::ofstream File;
	std::list<MODEL_HISTORY_NODE>::iterator i;

	File.open((Common.GetProgramDirectory() + "\\" + PATH_MODEL_HISTORY).c_str(), std::ios::out);
	if(File.fail()) return FALSE;

	i = FileNameList.begin();
	while(i != FileNameList.end())
	{
		File << i->FileName << std::endl;
		i++;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads the model history
//+-----------------------------------------------------------------------------
BOOL MODEL_HISTORY::Load()
{
	INT Id;
	MODEL_HISTORY_NODE Node;
	std::string FileName;
	std::ifstream File;

	File.open((Common.GetProgramDirectory() + "\\" + PATH_MODEL_HISTORY).c_str(), std::ios::in);
	if(File.fail()) return TRUE;

	Id = MENU_HISTORY_BASE;

	while(!File.eof())
	{
		FileName = "";
		std::getline(File, FileName);
		Common.RemoveWhiteSpace(FileName);
		if(FileName == "") break;

		Node.Id = Id;
		Node.FileName = FileName;
		Node.ShortFileName = Common.GetFileName(FileName);

		FileNameList.push_back(Node);
		if(FileNameList.size() >= MAX_NR_OF_HISTORY_ITEMS) break;

		Id++;
	}

	RedrawMenu();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Returns the minimum menu item ID
//+-----------------------------------------------------------------------------
UINT MODEL_HISTORY::GetMinMenuItemId() CONST
{
	return MENU_HISTORY_BASE;
}


//+-----------------------------------------------------------------------------
//| Returns the maximum menu item ID
//+-----------------------------------------------------------------------------
UINT MODEL_HISTORY::GetMaxMenuItemId() CONST
{
	return static_cast<UINT>(MENU_HISTORY_BASE + FileNameList.size() - 1);
}


//+-----------------------------------------------------------------------------
//| Redraws the history menu
//+-----------------------------------------------------------------------------
VOID MODEL_HISTORY::RedrawMenu()
{
	INT Id;
	HMENU Menu;
	std::list<MODEL_HISTORY_NODE>::iterator i;

	Menu = MainWindow.GetHistoryMenu();
	if(Menu == NULL) return;

	while(GetMenuItemCount(Menu) > 2)
	{
		DeleteMenu(Menu, 2, MF_BYPOSITION);
	}

	i = FileNameList.begin();
	Id = MENU_HISTORY_BASE;
	while(i != FileNameList.end())
	{
		AppendMenu(Menu, MF_STRING, Id, i->ShortFileName.c_str());
		i->Id = Id;
		Id++;
		i++;
	}

	DrawMenuBar(MainWindow.GetWindow());
}
