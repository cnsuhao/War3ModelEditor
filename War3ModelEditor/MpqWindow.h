//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MPQ_WINDOW_H
#define MAGOS_MPQ_WINDOW_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "MainWindow.h"
#include "Mpq.h"


//+-----------------------------------------------------------------------------
//| Pre-declared classes
//+-----------------------------------------------------------------------------
struct FILE_NODE;


//+-----------------------------------------------------------------------------
//| File node connection structure
//+-----------------------------------------------------------------------------
struct FILE_NODE_CONNECTION
{
	FILE_NODE_CONNECTION()
	{
		FirstChildNode = NULL;
		LastChildNode = NULL;
	}

	FILE_NODE* FirstChildNode;
	FILE_NODE* LastChildNode;
};


//+-----------------------------------------------------------------------------
//| File node structure
//+-----------------------------------------------------------------------------
struct FILE_NODE
{
	FILE_NODE()
	{
		IsFolder = FALSE;
		IconCollectionIndex = INVALID_INDEX;

		FileNameAndPath = "";
		FileName = "";
		Extention = "";

		FilterData.FilterList.resize(Filter.GetNrOfFilters(), FALSE);

		NextNode = NULL;
		PreviousNode = NULL;
		ParentNode = NULL;

		TreeViewItem = NULL;
	}

	BOOL IsFolder;
	INT IconCollectionIndex;

	std::string FileNameAndPath;
	std::string FileName;
	std::string Extention;

	FILTER_DATA FilterData;

	FILE_NODE* NextNode;
	FILE_NODE* PreviousNode;
	FILE_NODE* ParentNode;

	FILE_NODE_CONNECTION FileConnection;
	FILE_NODE_CONNECTION FolderConnection;

	HTREEITEM TreeViewItem;
};


//+-----------------------------------------------------------------------------
//| Mpq window class
//+-----------------------------------------------------------------------------
class MPQ_WINDOW : public WINDOW_FRAME
{
	public:
		CONSTRUCTOR MPQ_WINDOW();
		DESTRUCTOR ~MPQ_WINDOW();

		virtual BOOL Create();
		virtual VOID Destroy();

		virtual LRESULT MessageHandler(UINT Message, WPARAM W, LPARAM L);
		virtual LRESULT MenuHandler(WORD MenuItem);
		virtual LRESULT ControlHandler(HWND Control, WORD Code);
		virtual LRESULT NotifyHandler(HWND Control, UINT Code, NMHDR* Header);
		virtual LRESULT TreeViewHandler(UINT Code, NM_TREEVIEW* TreeViewHeader);

		BOOL OpenAllMpqArchives();
		VOID CloseAllMpqArchives();

		BOOL IsMpqExtention(CONST std::string& Extention);

		BOOL OpenMpqByName(CONST std::string& FileName);

	protected:
		BOOL OpenMpq();
		BOOL CloseMpq();

		VOID UpdateWindowStatus();

		BOOL OpenMpqFile(CONST std::string& FileName);
		BOOL ExportMpqFile(CONST std::string& FileName, CONST std::string& RealFileName);

		VOID UpdateTreeView();
		VOID ResizeTreeView();

		BOOL LoadListFile(CONST std::string& FileName);
		BOOL AddFileToFileTree(CONST std::string& FileName);
		VOID RemoveFileTree(FILE_NODE* FileNode);

		FILE_NODE* FindChild(FILE_NODE* FileNode, FILE_NODE_CONNECTION* Connection, CONST std::string& Name);
		FILE_NODE* CreateFolder(FILE_NODE* FileNode, CONST std::string& Name);
		VOID AddChild(FILE_NODE* FileNode, FILE_NODE_CONNECTION* Connection, FILE_NODE* ChildNode);
		VOID UpdateFilterData(FILE_NODE* FileNode, FILTER_DATA& FilterData);

		VOID SplitPath(CONST std::string& Path, std::string& FileName, std::list<std::string>& PathList);

		BOOL InvalidExtention(CONST std::string& Extention);
		BOOL ModelExtention(CONST std::string& Extention);
		BOOL TextureExtention(CONST std::string& Extention);
		BOOL ScriptExtention(CONST std::string& Extention);

		WINDOW_TREEVIEW TreeView;

		FILE_NODE RootNode;
		FILE_NODE* CurrentlySelectedFile;
		FILE_NODE* CurrentlySelectedPopupFile;

		BOOL MpqLoaded;
		std::string MpqFileName;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern MPQ_WINDOW MpqWindow;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
