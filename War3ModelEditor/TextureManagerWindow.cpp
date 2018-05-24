//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "TextureManagerWindow.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
TEXTURE_MANAGER_WINDOW TextureManagerWindow;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
TEXTURE_MANAGER_WINDOW::TEXTURE_MANAGER_WINDOW()
{
	ManagerMenuId1 = TextureMenu;
	ManagerMenuId2 = TextureMenu2;

	Title = "Texture Manager";
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
TEXTURE_MANAGER_WINDOW::~TEXTURE_MANAGER_WINDOW()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Creates a new window
//+-----------------------------------------------------------------------------
BOOL TEXTURE_MANAGER_WINDOW::Create()
{
	if(!MANAGER_WINDOW::Create()) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Destroys the window
//+-----------------------------------------------------------------------------
VOID TEXTURE_MANAGER_WINDOW::Destroy()
{
	MANAGER_WINDOW::Destroy();
}


//+-----------------------------------------------------------------------------
//| Handles the window messages
//+-----------------------------------------------------------------------------
LRESULT TEXTURE_MANAGER_WINDOW::MessageHandler(UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_CLOSE:
		{
			TextureWindow.Hide();
			Hide();
			return 0;
		}
	}

	return MANAGER_WINDOW::MessageHandler(Message, W, L);
}


//+-----------------------------------------------------------------------------
//| Handles the window menu messages
//+-----------------------------------------------------------------------------
LRESULT TEXTURE_MANAGER_WINDOW::MenuHandler(WORD MenuItem)
{
	switch(MenuItem)
	{
		case TextureFileCreateEmpty:
		case TextureFileCreateEmpty2:
		{
			if(!CreateEmpty())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case TextureFileImport:
		case TextureFileImport2:
		{
			if(!Import())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case TextureFileExport:
		case TextureFileExport2:
		{
			if(!Export())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case TextureFileRemove:
		case TextureFileRemove2:
		{
			if(!Remove(Window))
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case TextureFileView:
		case TextureFileView2:
		{
			if(!View())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case TextureFileEdit:
		case TextureFileEdit2:
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
LRESULT TEXTURE_MANAGER_WINDOW::ControlHandler(HWND Control, WORD Code)
{
	if(Control == ListBox.GetWindow())
	{
		if(Code == LBN_DBLCLK)
		{
			SendMessage(WM_COMMAND, TextureFileView, 0);
			return 0;
		}
	}

	return MANAGER_WINDOW::ControlHandler(Control, Code);
}


//+-----------------------------------------------------------------------------
//| Handles the window notify messages
//+-----------------------------------------------------------------------------
LRESULT TEXTURE_MANAGER_WINDOW::NotifyHandler(HWND Control, UINT Code, NMHDR* Header)
{
	return MANAGER_WINDOW::NotifyHandler(Control, Code, Header);
}


//+-----------------------------------------------------------------------------
//| Uses a texture from an mpq
//+-----------------------------------------------------------------------------
BOOL TEXTURE_MANAGER_WINDOW::UseTextureFromMpq(CONST std::string& FileName)
{
	MODEL_TEXTURE* Texture;

	if(ListBox.GetIndex(FileName) != INVALID_INDEX)
	{
		Error.SetMessage("Unable to use \"" + FileName + "\", a texture by this name already exists!");
		return FALSE;
	}

	if(!TextureManager.Load(FileName)) return FALSE;

	Texture = new MODEL_TEXTURE();
	if(Texture == NULL)
	{
		Error.SetMessage("Unable to create a new texture, memory allocation failed!");
		return FALSE;
	}

	Texture->Data().FileName = FileName;

	if(!Model.AddTexture(Texture))
	{
		delete Texture;
		return FALSE;
	}

	TextureWindow.Hide();

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Hides the texture window
//+-----------------------------------------------------------------------------
VOID TEXTURE_MANAGER_WINDOW::HideTextureWindow()
{
	TextureWindow.Hide();
	TextureWindow.SetFileName("");
}


//+-----------------------------------------------------------------------------
//| Creates a new empty texture
//+-----------------------------------------------------------------------------
BOOL TEXTURE_MANAGER_WINDOW::CreateEmpty()
{
	MODEL_TEXTURE* Texture;

	Texture = new MODEL_TEXTURE();
	if(Texture == NULL)
	{
		Error.SetMessage("Unable to create a new texture, memory allocation failed!");
		return FALSE;
	}

	Texture->Data().FileName = "";

	if(!Model.AddTexture(Texture))
	{
		delete Texture;
		return FALSE;
	}

	TextureWindow.Hide();

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Imports a texture
//+-----------------------------------------------------------------------------
BOOL TEXTURE_MANAGER_WINDOW::Import()
{
	std::string FileName;
	std::string ListFileName;
	WINDOW_FILE_DIALOG FileDialog;
	MODEL_TEXTURE* Texture;

	FileDialog.SetTitle("Import Texture");
	FileDialog.SetFilter(FILTER_TEXTURE);
	if(!FileDialog.OpenFile(Window, FileName)) return TRUE;

	ListFileName = Common.GetFileName(FileName);
	if(ListBox.GetIndex(ListFileName) != INVALID_INDEX)
	{
		Error.SetMessage("Unable to import \"" + FileName + "\", a texture by this name already exists!");
		return FALSE;
	}

	Texture = new MODEL_TEXTURE();
	if(Texture == NULL)
	{
		Error.SetMessage("Unable to create a new texture, memory allocation failed!");
		return FALSE;
	}

	Texture->Data().FileName = ListFileName;

	if(!TextureManager.Import(ListFileName, FileName))
	{
		delete Texture;
		return FALSE;
	}

	if(!Model.AddTexture(Texture))
	{
		delete Texture;
		return FALSE;
	}

	TextureWindow.Hide();

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Exports a texture
//+-----------------------------------------------------------------------------
BOOL TEXTURE_MANAGER_WINDOW::Export()
{
	INT Index;
	std::string FileName;
	std::string RealFileName;
	WINDOW_FILE_DIALOG FileDialog;

	Index = ListBox.GetCurrentSelection();
	if(Index == INVALID_INDEX)
	{
		Error.SetMessage("No texture was selected!");
		return FALSE;
	}

	FileName = ListBox.GetString(Index);
	RealFileName = Common.GetFileName(FileName);

	FileDialog.SetTitle("Export Texture");
	FileDialog.SetFilter(FILTER_TEXTURE);
	if(!FileDialog.SaveFile(Window, RealFileName)) return TRUE;

	if(!TextureManager.Export(FileName, RealFileName)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes a texture
//+-----------------------------------------------------------------------------
BOOL TEXTURE_MANAGER_WINDOW::Remove(HWND Window)
{
	INT Index;
	std::string FileName;
	MODEL_TEXTURE* Texture;

	Index = ListBox.GetCurrentSelection();
	if(Index == INVALID_INDEX) return TRUE;

	Texture = reinterpret_cast<MODEL_TEXTURE*>(ListBox.GetData(Index));
	if(Texture == NULL) return TRUE;

	FileName = Texture->Data().FileName;

	if(!Model.RemoveTexture(Texture, Window)) return FALSE;
	if(!TextureManager.Unload(FileName)) return FALSE;

	TextureWindow.Hide();
	TextureWindow.SetFileName("");

	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Views a texture
//+-----------------------------------------------------------------------------
BOOL TEXTURE_MANAGER_WINDOW::View()
{
	INT Index;
	INT Width;
	INT Height;
	std::string FileName;
	std::stringstream Stream;
	MODEL_TEXTURE* ModelTexture;
	TEXTURE* Texture;

	Index = ListBox.GetCurrentSelection();
	if(Index == INVALID_INDEX) return TRUE;

	ModelTexture = reinterpret_cast<MODEL_TEXTURE*>(ListBox.GetData(Index));
	if(ModelTexture == NULL) return TRUE;

	FileName = ModelTexture->Data().FileName;
	if(FileName == "")
	{
		Texture = TextureManager.GetReplaceableTexture(ModelTexture->Data().ReplaceableId);
	}
	else
	{
		Texture = TextureManager.GetTexture(FileName);
	}

	if(Texture == NULL)
	{
		Error.SetMessage("Unable to retrieve \"" + FileName + "\"!");
		return FALSE;
	}

	Width = Texture->GetWidth() * 2;
	Height = Texture->GetHeight() * 2;

	if(Width < MIN_TEXTURE_WINDOW_WIDTH) Width = MIN_TEXTURE_WINDOW_WIDTH;
	if(Width > MAX_TEXTURE_WINDOW_WIDTH) Width = MAX_TEXTURE_WINDOW_WIDTH;
	if(Height < MIN_TEXTURE_WINDOW_HEIGHT) Height = MIN_TEXTURE_WINDOW_HEIGHT;
	if(Height > MAX_TEXTURE_WINDOW_HEIGHT) Height = MAX_TEXTURE_WINDOW_HEIGHT;

	Stream << Common.GetFileName(FileName) << " (" << Texture->GetWidth() << "x" << Texture->GetHeight() << ")";

	TextureWindow.SetText(Stream.str());
	TextureWindow.SetFileName(FileName, ModelTexture->Data().ReplaceableId);
	TextureWindow.SetWindowDimension(Width, Height);
	TextureWindow.CenterPosition();
	TextureWindow.Show();
	TextureWindow.Restore();
	TextureWindow.Activate();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Edits a texture
//+-----------------------------------------------------------------------------
BOOL TEXTURE_MANAGER_WINDOW::Edit()
{
	INT Index;
	INT ReplaceableId;
	MODEL_TEXTURE_DATA Data;
	MODEL_TEXTURE* Texture;
	std::stringstream Stream;

	Index = ListBox.GetCurrentSelection();
	if(Index == INVALID_INDEX) return TRUE;

	Texture = reinterpret_cast<MODEL_TEXTURE*>(ListBox.GetData(Index));
	if(Texture == NULL) return TRUE;

	Data = Texture->Data();
	ReplaceableId = Data.ReplaceableId;

	if(!TextureDialog.Display(Window, Data)) return TRUE;

	Texture->Data() = Data;
	if((ReplaceableId != Data.ReplaceableId) && (Data.FileName == ""))
	{
		Stream << "Replaceable ID " << Data.ReplaceableId;

		ListBox.RemoveString(Index);
		Index = ListBox.AddString(Stream.str());
		ListBox.SetData(Index, reinterpret_cast<INT>(Texture));
	}

	MainWindow.MakeModelUnsaved();

	return TRUE;
}
