//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "GeosetManagerWindow.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
GEOSET_MANAGER_WINDOW GeosetManagerWindow;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
GEOSET_MANAGER_WINDOW::GEOSET_MANAGER_WINDOW()
{
	ManagerMenuId1 = GeosetMenu;
	ManagerMenuId2 = GeosetMenu2;

	Title = "Geoset Manager";
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
GEOSET_MANAGER_WINDOW::~GEOSET_MANAGER_WINDOW()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Creates a new window
//+-----------------------------------------------------------------------------
BOOL GEOSET_MANAGER_WINDOW::Create()
{
	if(!MANAGER_WINDOW::Create()) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Destroys the window
//+-----------------------------------------------------------------------------
VOID GEOSET_MANAGER_WINDOW::Destroy()
{
	MANAGER_WINDOW::Destroy();
}


//+-----------------------------------------------------------------------------
//| Handles the window messages
//+-----------------------------------------------------------------------------
LRESULT GEOSET_MANAGER_WINDOW::MessageHandler(UINT Message, WPARAM W, LPARAM L)
{
	return MANAGER_WINDOW::MessageHandler(Message, W, L);
}


//+-----------------------------------------------------------------------------
//| Handles the window menu messages
//+-----------------------------------------------------------------------------
LRESULT GEOSET_MANAGER_WINDOW::MenuHandler(WORD MenuItem)
{
	switch(MenuItem)
	{
		case GeosetFileCreate:
		case GeosetFileCreate2:
		{
			if(!CreateNew())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case GeosetFileRemove:
		case GeosetFileRemove2:
		{
			if(!Remove(Window))
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case GeosetFileImport:
		case GeosetFileImport2:
		{
			if(!Import())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case GeosetFileExport:
		case GeosetFileExport2:
		{
			if(!Export())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case GeosetFileEdit:
		case GeosetFileEdit2:
		{
			if(!Edit())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}
	}

	return MANAGER_WINDOW::MenuHandler(MenuItem);
}


//+-----------------------------------------------------------------------------
//| Handles the window control messages
//+-----------------------------------------------------------------------------
LRESULT GEOSET_MANAGER_WINDOW::ControlHandler(HWND Control, WORD Code)
{
	if(Control == ListBox.GetWindow())
	{
		if(Code == LBN_DBLCLK)
		{
			SendMessage(WM_COMMAND, GeosetFileEdit, 0);
			return 0;
		}
	}

	return MANAGER_WINDOW::ControlHandler(Control, Code);
}


//+-----------------------------------------------------------------------------
//| Handles the window notify messages
//+-----------------------------------------------------------------------------
LRESULT GEOSET_MANAGER_WINDOW::NotifyHandler(HWND Control, UINT Code, NMHDR* Header)
{
	return MANAGER_WINDOW::NotifyHandler(Control, Code, Header);
}


//+-----------------------------------------------------------------------------
//| Selects a geoset
//+-----------------------------------------------------------------------------
VOID GEOSET_MANAGER_WINDOW::SelectGeoset(MODEL_GEOSET* Geoset)
{
	INT Size;
	INT Index;

	Size = ListBox.GetNrOfItems();
	Index = 0;

	while(Index < Size)
	{
		if(reinterpret_cast<MODEL_GEOSET*>(ListBox.GetData(Index)) == Geoset)
		{
			ListBox.SetCurrentSelection(Index);
			return;
		}

		Index++;
	}

	ListBox.SetCurrentSelection(-1);
}


//+-----------------------------------------------------------------------------
//| Creates a new geoset
//+-----------------------------------------------------------------------------
BOOL GEOSET_MANAGER_WINDOW::CreateNew()
{
	MODEL_GEOSET_DATA Data;
	MODEL_GEOSET* Geoset;
	MODEL_MATERIAL* Material;

	Material = NULL;

	Geoset = new MODEL_GEOSET();
	if(Geoset == NULL)
	{
		Error.SetMessage("Unable to create a new geoset, memory allocation failed!");
		return FALSE;
	}

	if(!GeosetDialog.Display(Window, Geoset, Data, Material))
	{
		delete Geoset;
		return TRUE;
	}

	Geoset->Data() = Data;
	if(Material != NULL)
	{
		Geoset->MaterialNode.Attach(Material->GeosetNodes);
	}
	else
	{
		Geoset->MaterialNode.Detach();
	}

	if(!Model.AddGeoset(Geoset))
	{
		delete Geoset;
		return FALSE;
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes an existing geoset
//+-----------------------------------------------------------------------------
BOOL GEOSET_MANAGER_WINDOW::Remove(HWND Window)
{
	INT Index;
	MODEL_GEOSET* Geoset;

	Index = ListBox.GetCurrentSelection();
	if(Index == INVALID_INDEX) return TRUE;

	Geoset = reinterpret_cast<MODEL_GEOSET*>(ListBox.GetData(Index));
	if(Geoset == NULL) return TRUE;

	if(!Model.RemoveGeoset(Geoset, Window)) return FALSE;

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Imports an existing geoset from a geo file
//+-----------------------------------------------------------------------------
BOOL GEOSET_MANAGER_WINDOW::Import()
{
	MODEL_BASE* Node;
	MODEL_GEOSET* Geoset;
	std::string FileName;
	WINDOW_FILE_DIALOG FileDialog;

	FileDialog.SetTitle("Import Geoset");
	FileDialog.SetFilter(FILTER_GEOSET);
	if(!FileDialog.OpenFile(Window, FileName)) return TRUE;

	Node = NULL;

	if(!ImportGeosetDialog.Display(Window, Node)) return TRUE;

	Geoset = new MODEL_GEOSET();
	if(Geoset == NULL)
	{
		Error.SetMessage("Unable to create a new geoset, memory allocation failed!");
		return FALSE;
	}

	if(!ImportGeoset(*Geoset, FileName, Node))
	{
		delete Geoset;
		return FALSE;
	}

	if(!Model.AddGeoset(Geoset))
	{
		delete Geoset;
		return FALSE;
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Exports an existing geoset to a geo file
//+-----------------------------------------------------------------------------
BOOL GEOSET_MANAGER_WINDOW::Export()
{
	INT Index;
	MODEL_GEOSET* Geoset;
	std::string FileName;
	std::stringstream Stream;
	WINDOW_FILE_DIALOG FileDialog;

	Index = ListBox.GetCurrentSelection();
	if(Index == INVALID_INDEX) return TRUE;

	Geoset = reinterpret_cast<MODEL_GEOSET*>(ListBox.GetData(Index));
	if(Geoset == NULL) return TRUE;

	Stream << "Geoset" << Geoset << ".geo";
	FileName = Stream.str();

	FileDialog.SetTitle("Export Geoset");
	FileDialog.SetFilter(FILTER_GEOSET);
	if(!FileDialog.SaveFile(Window, FileName)) return TRUE;

	if(!ExportGeoset(*Geoset, FileName)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Edits an existing geoset
//+-----------------------------------------------------------------------------
BOOL GEOSET_MANAGER_WINDOW::Edit()
{
	INT Index;
	MODEL_GEOSET_DATA Data;
	MODEL_GEOSET* Geoset;
	MODEL_MATERIAL* Material;

	Index = ListBox.GetCurrentSelection();
	if(Index == INVALID_INDEX) return TRUE;

	Geoset = reinterpret_cast<MODEL_GEOSET*>(ListBox.GetData(Index));
	if(Geoset == NULL) return TRUE;

	Data = Geoset->Data();
	Material = Geoset->MaterialNode.IsAttached() ? Geoset->MaterialNode.GetObjectData() : NULL;

	if(!GeosetDialog.Display(Window, Geoset, Data, Material)) return TRUE;

	Geoset->Data() = Data;
	if(Material != NULL)
	{
		Geoset->MaterialNode.Attach(Material->GeosetNodes);
	}
	else
	{
		Geoset->MaterialNode.Detach();
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Imports a geoset from a geo file
//+-----------------------------------------------------------------------------
BOOL GEOSET_MANAGER_WINDOW::ImportGeoset(MODEL_GEOSET& Geoset, CONST std::string& FileName, MODEL_BASE* Node)
{
	INT i;
	DWORD TempDWord;
	DWORD TempDWord2;
	MODEL_GEOSET_VERTEX* Vertex;
	MODEL_GEOSET_FACE* Face;
	MODEL_GEOSET_GROUP* Group;
	MODEL_GEOSET_GROUP_NODE* GroupNode;
	EXTENT* Extent;
	std::ifstream File;

	File.open(FileName.c_str(), std::ios::in | std::ios::binary);
	if(File.fail())
	{
		Error.SetMessage("Unable to open \"" + FileName + "\"!");
		return FALSE;
	}

	TempDWord = 0;
	File.read(reinterpret_cast<CHAR*>(&TempDWord), sizeof(DWORD));
	if(TempDWord != 'XOEG')
	{
		Error.SetMessage("The file is not a geoset file!");
		return FALSE;
	}

	TempDWord = 0;
	File.read(reinterpret_cast<CHAR*>(&TempDWord), sizeof(DWORD));
	Geoset.Data().SelectionGroup = TempDWord;

	TempDWord = 0;
	File.read(reinterpret_cast<CHAR*>(&TempDWord), sizeof(DWORD));
	Geoset.Data().Unselectable = (TempDWord == 1);

	File.read(reinterpret_cast<CHAR*>(&Geoset.Data().Extent.Radius), sizeof(FLOAT));
	File.read(reinterpret_cast<CHAR*>(&Geoset.Data().Extent.Min.x), sizeof(FLOAT));
	File.read(reinterpret_cast<CHAR*>(&Geoset.Data().Extent.Min.y), sizeof(FLOAT));
	File.read(reinterpret_cast<CHAR*>(&Geoset.Data().Extent.Min.z), sizeof(FLOAT));
	File.read(reinterpret_cast<CHAR*>(&Geoset.Data().Extent.Max.x), sizeof(FLOAT));
	File.read(reinterpret_cast<CHAR*>(&Geoset.Data().Extent.Max.y), sizeof(FLOAT));
	File.read(reinterpret_cast<CHAR*>(&Geoset.Data().Extent.Max.z), sizeof(FLOAT));

	TempDWord = 0;
	File.read(reinterpret_cast<CHAR*>(&TempDWord), sizeof(DWORD));

	Geoset.Data().VertexContainer.Reserve(TempDWord);
	for(i = 0; i < static_cast<INT>(TempDWord); i++)
	{
		Vertex = new MODEL_GEOSET_VERTEX();
		if(Vertex == NULL)
		{
			Error.SetMessage("Unable to create a new vertex!");
			return FALSE;
		}

		File.read(reinterpret_cast<CHAR*>(&Vertex->Position.x), sizeof(FLOAT));
		File.read(reinterpret_cast<CHAR*>(&Vertex->Position.y), sizeof(FLOAT));
		File.read(reinterpret_cast<CHAR*>(&Vertex->Position.z), sizeof(FLOAT));
		File.read(reinterpret_cast<CHAR*>(&Vertex->Normal.x), sizeof(FLOAT));
		File.read(reinterpret_cast<CHAR*>(&Vertex->Normal.y), sizeof(FLOAT));
		File.read(reinterpret_cast<CHAR*>(&Vertex->Normal.z), sizeof(FLOAT));
		File.read(reinterpret_cast<CHAR*>(&Vertex->TexturePosition.x), sizeof(FLOAT));
		File.read(reinterpret_cast<CHAR*>(&Vertex->TexturePosition.y), sizeof(FLOAT));

		Vertex->VertexGroup = 0;

		if(!Geoset.AddVertex(Vertex)) return FALSE;
	}

	TempDWord = 0;
	File.read(reinterpret_cast<CHAR*>(&TempDWord), sizeof(DWORD));

	Geoset.Data().FaceContainer.Reserve(TempDWord);
	for(i = 0; i < static_cast<INT>(TempDWord); i++)
	{
		Face = new MODEL_GEOSET_FACE();
		if(Face == NULL)
		{
			Error.SetMessage("Unable to create a new face!");
			return FALSE;
		}

		File.read(reinterpret_cast<CHAR*>(&TempDWord2), sizeof(DWORD));
		Face->Index1 = TempDWord2;
		File.read(reinterpret_cast<CHAR*>(&TempDWord2), sizeof(DWORD));
		Face->Index2 = TempDWord2;
		File.read(reinterpret_cast<CHAR*>(&TempDWord2), sizeof(DWORD));
		Face->Index3 = TempDWord2;

		if(!Geoset.AddFace(Face)) return FALSE;
	}

	Group = new MODEL_GEOSET_GROUP();
	if(Group == NULL)
	{
		Error.SetMessage("Unable to create a new matrix group!");
		return FALSE;
	}

	GroupNode = new MODEL_GEOSET_GROUP_NODE();
	if(GroupNode == NULL)
	{
		Error.SetMessage("Unable to create a new matrix group node!");
		return FALSE;
	}

	if(Node != NULL)
	{
		GroupNode->Node.Attach(Node->MatrixListNodes);
	}
	else
	{
		GroupNode->Node.Detach();
	}

	Group->MatrixListSize = 1;
	Group->MatrixList.Add(GroupNode);

	if(!Geoset.AddGroup(Group)) return FALSE;

	for(i = 0; i < Model.Data().SequenceContainer.GetSize(); i++)
	{
		Extent = new EXTENT(Geoset.Data().Extent);
		if(Extent == NULL)
		{
			Error.SetMessage("Unable to create a new extent!");
			return FALSE;
		}

		if(!Geoset.AddExtent(Extent)) return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Exports a geoset to a geo file
//+-----------------------------------------------------------------------------
BOOL GEOSET_MANAGER_WINDOW::ExportGeoset(MODEL_GEOSET& Geoset, CONST std::string& FileName)
{
	INT i;
	DWORD TempDWord;
	MODEL_GEOSET_VERTEX* Vertex;
	MODEL_GEOSET_FACE* Face;
	std::ofstream File;

	File.open(FileName.c_str(), std::ios::out | std::ios::binary);
	if(File.fail())
	{
		Error.SetMessage("Unable to open \"" + FileName + "\"!");
		return FALSE;
	}

	TempDWord = 'XOEG';
	File.write(reinterpret_cast<CHAR*>(&TempDWord), sizeof(DWORD));

	TempDWord = Geoset.Data().SelectionGroup;
	File.write(reinterpret_cast<CHAR*>(&TempDWord), sizeof(DWORD));

	TempDWord = Geoset.Data().Unselectable ? 1 : 0;
	File.write(reinterpret_cast<CHAR*>(&TempDWord), sizeof(DWORD));

	File.write(reinterpret_cast<CHAR*>(&Geoset.Data().Extent.Radius), sizeof(FLOAT));
	File.write(reinterpret_cast<CHAR*>(&Geoset.Data().Extent.Min.x), sizeof(FLOAT));
	File.write(reinterpret_cast<CHAR*>(&Geoset.Data().Extent.Min.y), sizeof(FLOAT));
	File.write(reinterpret_cast<CHAR*>(&Geoset.Data().Extent.Min.z), sizeof(FLOAT));
	File.write(reinterpret_cast<CHAR*>(&Geoset.Data().Extent.Max.x), sizeof(FLOAT));
	File.write(reinterpret_cast<CHAR*>(&Geoset.Data().Extent.Max.y), sizeof(FLOAT));
	File.write(reinterpret_cast<CHAR*>(&Geoset.Data().Extent.Max.z), sizeof(FLOAT));

	TempDWord = Geoset.Data().VertexContainer.GetSize();
	File.write(reinterpret_cast<CHAR*>(&TempDWord), sizeof(DWORD));

	for(i = 0; i < Geoset.Data().VertexContainer.GetTotalSize(); i++)
	{
		if(Geoset.Data().VertexContainer.ValidIndex(i))
		{
			Vertex = Geoset.Data().VertexContainer[i];

			File.write(reinterpret_cast<CHAR*>(&Vertex->Position.x), sizeof(FLOAT));
			File.write(reinterpret_cast<CHAR*>(&Vertex->Position.y), sizeof(FLOAT));
			File.write(reinterpret_cast<CHAR*>(&Vertex->Position.z), sizeof(FLOAT));
			File.write(reinterpret_cast<CHAR*>(&Vertex->Normal.x), sizeof(FLOAT));
			File.write(reinterpret_cast<CHAR*>(&Vertex->Normal.y), sizeof(FLOAT));
			File.write(reinterpret_cast<CHAR*>(&Vertex->Normal.z), sizeof(FLOAT));
			File.write(reinterpret_cast<CHAR*>(&Vertex->TexturePosition.x), sizeof(FLOAT));
			File.write(reinterpret_cast<CHAR*>(&Vertex->TexturePosition.y), sizeof(FLOAT));
		}
	}

	TempDWord = Geoset.Data().FaceContainer.GetSize();
	File.write(reinterpret_cast<CHAR*>(&TempDWord), sizeof(DWORD));

	for(i = 0; i < Geoset.Data().FaceContainer.GetTotalSize(); i++)
	{
		if(Geoset.Data().FaceContainer.ValidIndex(i))
		{
			Face = Geoset.Data().FaceContainer[i];

			TempDWord = Face->Index1;
			File.write(reinterpret_cast<CHAR*>(&TempDWord), sizeof(DWORD));
			TempDWord = Face->Index2;
			File.write(reinterpret_cast<CHAR*>(&TempDWord), sizeof(DWORD));
			TempDWord = Face->Index3;
			File.write(reinterpret_cast<CHAR*>(&TempDWord), sizeof(DWORD));
		}
	}

	return TRUE;
}
