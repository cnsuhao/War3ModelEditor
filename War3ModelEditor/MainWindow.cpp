//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "MainWindow.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
MAIN_WINDOW MainWindow;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MAIN_WINDOW::MAIN_WINDOW()
{
	ModelLoaded = FALSE;
	ModelSaved = FALSE;
	ModelNeverSaved = FALSE;

	ModelFileName = "";

	WindowActive = TRUE;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MAIN_WINDOW::~MAIN_WINDOW()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Creates a new window
//+-----------------------------------------------------------------------------
BOOL MAIN_WINDOW::Create()
{
	FrameInfo.Title = EDITOR_NAME;
	FrameInfo.Width = DEFAULT_MAIN_WINDOW_WIDTH;
	FrameInfo.Height = DEFAULT_MAIN_WINDOW_HEIGHT;
	FrameInfo.Menu = ::LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(MainMenu));
	FrameInfo.Style |= (WS_MAXIMIZEBOX | WS_SIZEBOX);

	if(!WINDOW_FRAME::Create()) return FALSE;

	TeamColor.SetTeamColorIcons(GetMenu());
	CheckMenuItem(TeamColor.GetCurrentTeamColorId(), TRUE);

	if(!GraphicsWindow.Create(Window)) return FALSE;

	ModelLoaded = FALSE;
	ModelSaved = FALSE;
	ModelNeverSaved = FALSE;

	ModelFileName = "";

	UpdateWindowStatus(TRUE);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Destroys the window
//+-----------------------------------------------------------------------------
VOID MAIN_WINDOW::Destroy()
{
	GraphicsWindow.Destroy();

	WINDOW_FRAME::Destroy();
}


//+-----------------------------------------------------------------------------
//| Handles the window messages
//+-----------------------------------------------------------------------------
LRESULT MAIN_WINDOW::MessageHandler(UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_PAINT:
		{
			Graphics.SetCamera(Camera);
			UpdateAndRender(0);
			ValidateRect(Window, NULL);
			return 0;
		}

		case WM_SIZE:
		{
			WindowActive = (W != SIZE_MINIMIZED);

			Camera.DisableMouseMove();
			GraphicsWindow.Resize();
			return 0;
		}

		case WM_SIZING:
		{
			Camera.DisableMouseMove();
			GraphicsWindow.Resize();
			return 0;
		}

		case WM_SHOWWINDOW:
		{
			WindowActive = static_cast<BOOL>(W);
			return 0;
		}

		case WM_CLOSE:
		{
			SendMessage(WM_COMMAND, MainFileExit, 0);
			return 0;
		}

		case WM_DESTROY:
		{
			return 0;
		}
	}

	return DefWindowProc(Window, Message, W, L);
}


//+-----------------------------------------------------------------------------
//| Handles the window menu messages
//+-----------------------------------------------------------------------------
LRESULT MAIN_WINDOW::MenuHandler(WORD MenuItem)
{
	BOOL Cancel;
	std::string FileName;

	switch(MenuItem)
	{
		case MainFileNew:
		{
			if(!SaveFileIfNeccessary(Cancel))
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			if(Cancel) return 0;

			if(!NewFile())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case MainFileOpen:
		{
			if(!SaveFileIfNeccessary(Cancel))
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			if(Cancel) return 0;

			if(!OpenFile())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case MainFileSave:
		{
			if(!SaveFile(Cancel, FALSE))
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case MainFileSaveAs:
		{
			if(!SaveFile(Cancel, TRUE))
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case MainFileClose:
		{
			if(!SaveFileIfNeccessary(Cancel))
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			if(Cancel) return 0;

			if(!CloseFile())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case MainFileImport:
		{
			if(!SaveFileIfNeccessary(Cancel))
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			if(Cancel) return 0;

			if(!ImportFile())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case MainFileExit:
		{
			if(!SaveFileIfNeccessary(Cancel))
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			if(Cancel) return 0;

			if(!CloseFile())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			PostQuitMessage(0);

			return 0;
		}

		case MainHistoryClear:
		{
			ModelHistory.Clear();
			return 0;
		}

		case MainEditRegisterExtentions:
		{
			REGISTER_INFO RegisterInfo;

			if(RegisterDialog.Display(Window, RegisterInfo))
			{
				if(!RegisterExtentions(RegisterInfo))
				{
					Error.DisplayMessage(Window);
					Error.ClearMessage();
					return 0;
				}

				return 0;
			}

			return 0;
		}

		case MainEditCalculateExtents:
		{
			if(!CalculateExtents())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case MainEditProperties:
		{
			PROPERTIES_INFO PropertiesInfo;

			PropertiesInfo = Properties.GetPropertiesInfo();
			if(PropertiesDialog.Display(Window, PropertiesInfo))
			{
				Properties.SetPropertiesInfo(PropertiesInfo);
				if(!Properties.SavePropertiesInfo(PATH_PROPERTIES))
				{
					Error.DisplayMessage(Window);
					Error.ClearMessage();
					return 0;
				}

				return 0;
			}

			return 0;
		}

		case MainEditModelProperties:
		{
			MODEL_INFO_DATA Data;

			Data = Model.Data().Info;
			if(!ModelPropertiesDialog.Display(Window, Data)) return 0;
			Model.Data().Info = Data;

			MakeModelUnsaved();

			return 0;
		}

		case MainEditModelInfo:
		{
			ModelInfoDialog.Display(Window);
			return 0;
		}

		case MainViewResize:
		{
			SetWindowDimension(DEFAULT_MAIN_WINDOW_WIDTH, DEFAULT_MAIN_WINDOW_HEIGHT);
			GraphicsWindow.Resize();
			return 0;
		}

		case MainViewResetCamera:
		{
			Camera.Reset(Window);
			return 0;
		}

		case MainWindowsModelEditor:
		{
			ModelWindow.Show();
			ModelWindow.Restore();
			ModelWindow.Activate();
			return 0;
		}

		case MainWindowsAnimationController:
		{
			AnimationWindow.Show();
			AnimationWindow.Restore();
			AnimationWindow.Activate();
			return 0;
		}

		case MainWindowsNodeManager:
		{
			NodeManagerWindow.Show();
			NodeManagerWindow.Restore();
			NodeManagerWindow.Activate();
			return 0;
		}

		case MainWindowsCameraManager:
		{
			CameraManagerWindow.Show();
			CameraManagerWindow.Restore();
			CameraManagerWindow.Activate();
			return 0;
		}

		case MainWindowsGeosetManager:
		{
			GeosetManagerWindow.Show();
			GeosetManagerWindow.Restore();
			GeosetManagerWindow.Activate();
			return 0;
		}

		case MainWindowsGeosetAnimationManager:
		{
			GeosetAnimationManagerWindow.Show();
			GeosetAnimationManagerWindow.Restore();
			GeosetAnimationManagerWindow.Activate();
			return 0;
		}

		case MainWindowsTextureManager:
		{
			TextureManagerWindow.Show();
			TextureManagerWindow.Restore();
			TextureManagerWindow.Activate();
			return 0;
		}

		case MainWindowsTextureAnimationManager:
		{
			TextureAnimationManagerWindow.Show();
			TextureAnimationManagerWindow.Restore();
			TextureAnimationManagerWindow.Activate();
			return 0;
		}

		case MainWindowsMaterialManager:
		{
			MaterialManagerWindow.Show();
			MaterialManagerWindow.Restore();
			MaterialManagerWindow.Activate();
			return 0;
		}

		case MainWindowsSequenceManager:
		{
			SequenceManagerWindow.Show();
			SequenceManagerWindow.Restore();
			SequenceManagerWindow.Activate();
			return 0;
		}

		case MainWindowsGlobalSequenceManager:
		{
			GlobalSequenceManagerWindow.Show();
			GlobalSequenceManagerWindow.Restore();
			GlobalSequenceManagerWindow.Activate();
			return 0;
		}

		case MainWindowsMpqBrowser:
		{
			MpqWindow.Show();
			MpqWindow.Restore();
			MpqWindow.Activate();
			return 0;
		}

		case MainExtrasColoredText:
		{
			ColoredTextDialog.Display(Window);
			return 0;
		}

		case MainExtrasLoadingScreen:
		{
			std::string TextureFileName;

			if(!LoadingScreenDialog.Display(Window, TextureFileName)) return 0;

			TextureFileName = Common.GetFileName(TextureFileName);

			if(!SaveFileIfNeccessary(Cancel))
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			if(Cancel) return 0;

			if(!NewLoadingScreen(TextureFileName))
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case MainHelpHelp:
		{
			if(!DisplayHelp())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case MainHelpAbout:
		{
			MessageBoxEx(Window, EDITOR_HELP_MESSAGE.c_str(), EDITOR_HELP_TITLE.c_str(), MB_ICONINFORMATION, CURRENT_LANGUAGE);
			return 0;
		}

		case MainTeamColorRed:
		case MainTeamColorBlue:
		case MainTeamColorTeal:
		case MainTeamColorPurple:
		case MainTeamColorYellow:
		case MainTeamColorOrange:
		case MainTeamColorGreen:
		case MainTeamColorPink:
		case MainTeamColorGray:
		case MainTeamColorLightBlue:
		case MainTeamColorDarkGreen:
		case MainTeamColorBrown:
		case MainTeamColorBlack:
		{
			CheckMenuItem(TeamColor.GetCurrentTeamColorId(), FALSE);
			TeamColor.SetCurrentTeamColorId(MenuItem);
			CheckMenuItem(TeamColor.GetCurrentTeamColorId(), TRUE);
			return 0;
		}
	}

	if(MenuItem < ModelHistory.GetMinMenuItemId()) return 0;
	if(MenuItem > ModelHistory.GetMaxMenuItemId()) return 0;

	if(!SaveFileIfNeccessary(Cancel))
	{
		Error.DisplayMessage(Window);
		Error.ClearMessage();
		return 0;
	}

	if(Cancel) return 0;

	if(!CloseFile(TRUE)) return FALSE;

	FileName = ModelHistory.GetFileName(MenuItem);
	Common.SetCurrentDirectory(Common.GetPath(FileName));

	if(!LoadByForce(FileName))
	{
		Error.DisplayMessage(Window);
		Error.ClearMessage();
		return 0;
	}

	return 0;
}


//+-----------------------------------------------------------------------------
//| Handles the window control messages
//+-----------------------------------------------------------------------------
LRESULT MAIN_WINDOW::ControlHandler(HWND Control, WORD Code)
{
	return 0;
}


//+-----------------------------------------------------------------------------
//| Handles the window notify messages
//+-----------------------------------------------------------------------------
LRESULT MAIN_WINDOW::NotifyHandler(HWND Control, UINT Code, NMHDR* Header)
{
	return 0;
}


//+-----------------------------------------------------------------------------
//| Returns the history menu
//+-----------------------------------------------------------------------------
HMENU MAIN_WINDOW::GetHistoryMenu()
{
	HMENU Menu;
	HMENU SubMenu;

	Menu = ::GetMenu(Window);
	if(Menu == NULL) return NULL;

	SubMenu = ::GetSubMenu(Menu, 1);
	if(SubMenu == NULL) return NULL;

	return SubMenu;
}


//+-----------------------------------------------------------------------------
//| Updates and renders the window
//+-----------------------------------------------------------------------------
VOID MAIN_WINDOW::UpdateAndRender(INT TimeDifference)
{
	if(TimeDifference > 0)
	{
		Keyboard.Update();
		Mouse.Update();

		Camera.Update(Window);
		Graphics.SetCamera(Camera);

		if(Keyboard.KeyDown(KEY_LEFTCTRL))
		{
			if(Mouse.ButtonPressed(BUTTON_LEFT))
			{
				POINT Point;
				BOOL InsideScreen;
				MODEL_GEOSET* Geoset;
				MODEL_MATERIAL* Material;
				D3DXVECTOR3 RayPosition;
				D3DXVECTOR3 RayDirection;

				GetCursorPos(&Point);
				ScreenToClient(Window, &Point);

				InsideScreen = FALSE;
				while(TRUE)
				{
					if(Point.x < 1) break;
					if(Point.y < 1) break;
					if(Point.x >= (GetWidth() - 1)) break;
					if(Point.y >= (GetHeight() - 1)) break;

					InsideScreen = TRUE;
					break;
				}

				if(InsideScreen)
				{
					Graphics.BuildRay(Point, GetWidth(), GetHeight(), RayPosition, RayDirection);

					Geoset = Model.GetIntersectedGeoset(RayPosition, RayDirection);
					if(Geoset != NULL)
					{
						GeosetManagerWindow.SelectGeoset(Geoset);
						FlashingGeoset.SetGeoset(Geoset);

						if(Geoset->MaterialNode.IsAttached())
						{
							Material = Geoset->MaterialNode.GetObjectData();
							MaterialManagerWindow.SelectMaterial(Material);
						}
						else
						{
							MaterialManagerWindow.SelectMaterial(NULL);
						}
					}
					else
					{
						GeosetManagerWindow.SelectGeoset(NULL);
						MaterialManagerWindow.SelectMaterial(NULL);
					}
				}
			}
		}
	}

	if(!WindowActive) return;

	if(Graphics.BeginRender(GraphicsWindow))
	{
		Render(TimeDifference);
		Graphics.EndRender();
	}
}


//+-----------------------------------------------------------------------------
//| Checks if a model is loaded
//+-----------------------------------------------------------------------------
BOOL MAIN_WINDOW::ModelIsLoaded()
{
	return ModelLoaded;
}


//+-----------------------------------------------------------------------------
//| Makes the model unsaved
//+-----------------------------------------------------------------------------
VOID MAIN_WINDOW::MakeModelUnsaved()
{
	ModelSaved = FALSE;

	UpdateWindowStatus(TRUE);
}


//+-----------------------------------------------------------------------------
//| Makes the model never saved
//+-----------------------------------------------------------------------------
VOID MAIN_WINDOW::MakeModelNeverSaved()
{
	ModelNeverSaved = TRUE;

	UpdateWindowStatus(TRUE);
}


//+-----------------------------------------------------------------------------
//| Loads a model, texture or MPQ archive by force
//+-----------------------------------------------------------------------------
BOOL MAIN_WINDOW::LoadByForce(CONST std::string& FileName)
{
	std::string Extention;
	std::string RealFileName;

	RealFileName = Common.GetFileName(FileName);

	Extention = Common.LowerCase(Common.GetExtention(FileName));
	if(MpqWindow.IsMpqExtention(Extention))
	{
		if(!MpqWindow.OpenMpqByName(RealFileName)) return FALSE;

		MpqWindow.Show();
		MpqWindow.Restore();
		MpqWindow.Activate();

		return TRUE;
	}
	else if(ResourceLoader.IsModelExtention(Extention))
	{
		BUFFER Buffer;

		if(!FileLoader.LoadFromFile(RealFileName, Buffer))
		{
			Error.SetMessage("Unable to load \"" + FileName + "\", file does not exist!");
			return FALSE;
		}

		if(!LoadModelFromBuffer(RealFileName, Buffer)) return FALSE;

		return TRUE;
	}
	else if(ResourceLoader.IsTextureExtention(Extention))
	{
		BUFFER Buffer;

		if(!FileLoader.LoadFromFile(RealFileName, Buffer))
		{
			Error.SetMessage("Unable to load \"" + FileName + "\", file does not exist!");
			return FALSE;
		}

		if(!LoadTextureFromBuffer(RealFileName, Buffer)) return FALSE;

		return TRUE;
	}

	Error.SetMessage("Unable to load \"" + FileName + "\", unknown extention!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Loads a file from a buffer
//+-----------------------------------------------------------------------------
BOOL MAIN_WINDOW::LoadFileFromBuffer(CONST std::string& FileName, BUFFER& Buffer, BOOL SkipSaveTest)
{
	BOOL Cancel;
	std::string Extention;

	if(!SkipSaveTest)
	{
		if(!SaveFileIfNeccessary(Cancel)) return FALSE;
		if(Cancel) return TRUE;
	}

	if(!CloseFile(TRUE)) return FALSE;

	Extention = Common.LowerCase(Common.GetExtention(FileName));
	if(ResourceLoader.IsModelExtention(Extention))
	{
		if(!LoadModelFromBuffer(FileName, Buffer))
		{
			CloseFile();
			return FALSE;
		}

		return TRUE;
	}
	else if(ResourceLoader.IsTextureExtention(Extention))
	{
		if(!LoadTextureFromBuffer(FileName, Buffer))
		{
			CloseFile();
			return FALSE;
		}

		return TRUE;
	}

	Error.SetMessage("Unable to load \"" + FileName + "\", unknown extention!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Loads a model from a buffer
//+-----------------------------------------------------------------------------
BOOL MAIN_WINDOW::LoadModelFromBuffer(CONST std::string& FileName, BUFFER& Buffer)
{
	FLOAT Distance;

	if(!ResourceLoader.LoadModel(Model, FileName, Buffer)) return FALSE;

	Model.CalculateBoundsRadius();
	Distance = max(Model.GetBoundsRadius(), Model.Data().Info.Extent.Radius);
	Camera.SetDefaultTarget(Model.GetBoundsCenter());
	Camera.SetDefaultDistance(Distance * CAMERA_FACTOR_MODEL);

	ModelLoaded = TRUE;
	ModelSaved = TRUE;
	ModelNeverSaved = FALSE;

	ModelFileName = Common.GetFileName(FileName);

	Camera.Reset(Window);
	UpdateWindowStatus(TRUE);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Loads a texture from a buffer
//+-----------------------------------------------------------------------------
BOOL MAIN_WINDOW::LoadTextureFromBuffer(CONST std::string& FileName, BUFFER& Buffer)
{
	TEXTURE* Texture;

	if(!TextureManager.Load(FileName)) return FALSE;

	Texture = TextureManager.GetTexture(FileName);
	if(Texture == NULL) return FALSE;

	if(!Model.CreateTextureModel(FileName, Texture->GetWidth(), Texture->GetHeight(), Texture->GetRealWidth(), Texture->GetRealHeight())) return FALSE;

	Model.CalculateBoundsRadius();
	Camera.SetDefaultPitch(0);
	Camera.SetDefaultTarget(Model.GetBoundsCenter());
	Camera.SetDefaultDistance(Model.GetBoundsRadius() * CAMERA_FACTOR_MODEL);

	ModelLoaded = TRUE;
	ModelSaved = TRUE;
	ModelNeverSaved = TRUE;

	ModelFileName = FileName + ".mdl";

	Camera.Reset(Window);
	UpdateWindowStatus(TRUE);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Registers the desired extentions
//+-----------------------------------------------------------------------------
BOOL MAIN_WINDOW::RegisterExtentions(REGISTER_INFO& RegisterInfo)
{
	std::string Program;
	std::string ImageIconPath;
	std::string ModelIconPath;
	std::string ArchiveIconPath;

	Program = Common.GetProgramFileName();
	ImageIconPath = Program + ",4";
	ModelIconPath = Program + ",5";
	ArchiveIconPath = Program + ",3";

	if(RegisterInfo.Mdl)
	{
		if(!Register.RegisterExtention("mdl", "Warcraft 3 Model", ModelIconPath.c_str())) return FALSE;
	}

	if(RegisterInfo.Mdx)
	{
		if(!Register.RegisterExtention("mdx", "Warcraft 3 Model", ModelIconPath.c_str())) return FALSE;
	}

	if(RegisterInfo.Mpq)
	{
		if(!Register.RegisterExtention("mpq", "MPQ Archive", ArchiveIconPath.c_str())) return FALSE;
	}

	if(RegisterInfo.W3m)
	{
		if(!Register.RegisterExtention("w3m", "MPQ Archive", ArchiveIconPath.c_str())) return FALSE;
	}

	if(RegisterInfo.W3x)
	{
		if(!Register.RegisterExtention("w3x", "MPQ Archive", ArchiveIconPath.c_str())) return FALSE;
	}

	if(RegisterInfo.W3n)
	{
		if(!Register.RegisterExtention("w3n", "MPQ Archive", ArchiveIconPath.c_str())) return FALSE;
	}

	if(RegisterInfo.Bmp)
	{
		if(!Register.RegisterExtention("bmp", "Warcraft 3 Texture", ImageIconPath.c_str())) return FALSE;
	}

	if(RegisterInfo.Tga)
	{
		if(!Register.RegisterExtention("tga", "Warcraft 3 Texture", ImageIconPath.c_str())) return FALSE;
	}

	if(RegisterInfo.Png)
	{
		if(!Register.RegisterExtention("png", "Warcraft 3 Texture", ImageIconPath.c_str())) return FALSE;
	}

	if(RegisterInfo.Jpg)
	{
		if(!Register.RegisterExtention("jpg", "Warcraft 3 Texture", ImageIconPath.c_str())) return FALSE;
	}

	if(RegisterInfo.Jpeg)
	{
		if(!Register.RegisterExtention("jpeg", "Warcraft 3 Texture", ImageIconPath.c_str())) return FALSE;
	}

	if(RegisterInfo.Pcx)
	{
		if(!Register.RegisterExtention("pcx", "Warcraft 3 Texture", ImageIconPath.c_str())) return FALSE;
	}

	if(RegisterInfo.Blp)
	{
		if(!Register.RegisterExtention("blp", "Warcraft 3 Texture", ImageIconPath.c_str())) return FALSE;
	}

	if(RegisterInfo.OpenWith)
	{
		if(!Register.RegisterOpenWith("Open with War3ModelEditor")) return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a new file (model)
//+-----------------------------------------------------------------------------
BOOL MAIN_WINDOW::NewFile()
{
	CloseFile(TRUE);

	ModelLoaded = TRUE;
	ModelSaved = FALSE;
	ModelNeverSaved = TRUE;

	ModelFileName = DEFAULT_MODEL_NAME;

	Camera.Reset(Window);
	UpdateWindowStatus(TRUE);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Opens an existing file
//+-----------------------------------------------------------------------------
BOOL MAIN_WINDOW::OpenFile()
{
	BUFFER Buffer;
	std::string FileName;
	WINDOW_FILE_DIALOG FileDialog;

	FileDialog.SetTitle("Open Model or Texture");
	FileDialog.SetFilter(FILTER_BOTH);
	if(!FileDialog.OpenFile(Window, FileName)) return TRUE;

	if(!FileLoader.LoadFromFile(FileName, Buffer)) return FALSE;
	if(!LoadFileFromBuffer(FileName, Buffer, TRUE)) return FALSE;

	ModelHistory.AddFileName(FileName);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Imports an existing file
//+-----------------------------------------------------------------------------
BOOL MAIN_WINDOW::ImportFile()
{
	BUFFER Buffer;
	FLOAT Distance;
	std::string FileName;
	WINDOW_FILE_DIALOG FileDialog;

	FileDialog.SetTitle("Import Model");
	FileDialog.SetFilter(ModelImporter.GetFilter());
	if(!FileDialog.OpenFile(Window, FileName)) return TRUE;

	if(!FileLoader.LoadFromFile(FileName, Buffer)) return FALSE;
	if(!CloseFile(TRUE)) return FALSE;

	if(!ModelImporter.ImportModel(FileName, Buffer)) return FALSE;

	Model.CalculateBoundsRadius();
	Distance = max(Model.GetBoundsRadius(), Model.Data().Info.Extent.Radius);

	Camera.SetDefaultTarget(Model.GetBoundsCenter());
	Camera.SetDefaultDistance(Distance * CAMERA_FACTOR_MODEL);

	ModelLoaded = TRUE;
	ModelSaved = TRUE;
	ModelNeverSaved = TRUE;

	ModelFileName = Common.GetFileName(FileName);

	Camera.Reset(Window);
	UpdateWindowStatus(TRUE);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the file
//+-----------------------------------------------------------------------------
BOOL MAIN_WINDOW::SaveFile(BOOL& Cancel, BOOL ForceNewName)
{
	BUFFER Buffer;
	std::string FileName;
	WINDOW_FILE_DIALOG FileDialog;

	Cancel = FALSE;

	if(ForceNewName || ModelNeverSaved)
	{
		FileName = (ModelFileName == "") ? DEFAULT_MODEL_NAME : ModelFileName;

		FileDialog.SetTitle("Save Model");
		FileDialog.SetFilter(FILTER_MODEL);
		if(!FileDialog.SaveFile(Window, FileName))
		{
			Cancel = TRUE;
			return TRUE;
		}

		ModelFileName = FileName;
	}

	if(!ResourceLoader.SaveModel(Model, ModelFileName, Buffer)) return FALSE;
	if(!FileLoader.SaveToFile(ModelFileName, Buffer)) return FALSE;

	ModelLoaded = TRUE;
	ModelSaved = TRUE;
	ModelNeverSaved = FALSE;

	UpdateWindowStatus(TRUE);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Saves the file if neccessary
//+-----------------------------------------------------------------------------
BOOL MAIN_WINDOW::SaveFileIfNeccessary(BOOL& Cancel)
{
	Cancel = FALSE;

	if(!ModelLoaded) return TRUE;
	if(ModelSaved) return TRUE;

	switch(MessageBoxEx(Window, "Model has not been saved!\nSave now?", "Message", MB_YESNOCANCEL | MB_ICONINFORMATION, CURRENT_LANGUAGE))
	{
		case IDYES:
		{
			return SaveFile(Cancel, FALSE);
		}

		case IDNO:
		{
			return TRUE;
		}

		case IDCANCEL:
		{
			Cancel = TRUE;
			return TRUE;
		}
	}

	Error.SetMessage("Unexpected code reached!");
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Closes the file
//+-----------------------------------------------------------------------------
BOOL MAIN_WINDOW::CloseFile(BOOL NoWindowHide)
{
	Model.Clear();

	ModelLoaded = FALSE;
	ModelSaved = FALSE;
	ModelNeverSaved = FALSE;

	ModelFileName = "";

	WINDOW_DIALOG::CloseAllOpenDialogs();

	AnimationWindow.ClearAllAnimations();
	NodeManagerWindow.ClearAllNodes();
	CameraManagerWindow.ClearAllNames();
	GeosetManagerWindow.ClearAllNames();
	GeosetAnimationManagerWindow.ClearAllNames();
	TextureManagerWindow.ClearAllNames();
	TextureAnimationManagerWindow.ClearAllNames();
	MaterialManagerWindow.ClearAllNames();
	SequenceManagerWindow.ClearAllNames();
	GlobalSequenceManagerWindow.ClearAllNames();

	Camera.SetDefaultDefaults();
	TextureManager.Clear();
	UpdateWindowStatus(NoWindowHide);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a new loading screen model
//+-----------------------------------------------------------------------------
BOOL MAIN_WINDOW::NewLoadingScreen(CONST std::string& TextureFileName)
{
	CloseFile(TRUE);

	if(!TextureManager.Load(TextureFileName)) return FALSE;
	if(!Model.CreateLoadingScreenModel(TextureFileName)) return FALSE;

	ModelLoaded = TRUE;
	ModelSaved = FALSE;
	ModelNeverSaved = TRUE;

	ModelFileName = "LoadingScreen.mdl";

	Model.CalculateBoundsRadius();
	Camera.Reset(Window);
	UpdateWindowStatus(TRUE);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Displays the help file
//+-----------------------------------------------------------------------------
BOOL MAIN_WINDOW::DisplayHelp()
{
	std::string FileName;
	std::string FileNameAndPath;
	SHELLEXECUTEINFO Info;

	FileName = "War3ModelEditor.chm";
	FileNameAndPath = Common.GetProgramDirectory() + "\\" + FileName;

	ZeroMemory(&Info, sizeof(SHELLEXECUTEINFO));
	Info.cbSize = sizeof(SHELLEXECUTEINFO);
	Info.fMask = SEE_MASK_NOCLOSEPROCESS;
	Info.hwnd = Window;
	Info.lpVerb = "open";
	Info.lpFile = FileNameAndPath.c_str();
	Info.hInstApp = GetModuleHandle(NULL);
	Info.nShow = SW_SHOW;

	if(!ShellExecuteEx(&Info))
	{
		Error.SetMessage("Unable to open \"" + FileName + "\"!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Calculates the extents of all model components
//+-----------------------------------------------------------------------------
BOOL MAIN_WINDOW::CalculateExtents()
{
	INT i;
	FLOAT Min;
	FLOAT Max;
	EXTENT Extent;
	EXTENT TempExtent;

	Max = (std::numeric_limits<FLOAT>::max)();
	Min = (std::numeric_limits<FLOAT>::min)();

	Extent.Radius = 0.0f;
	Extent.Min = D3DXVECTOR3(Max, Max, Max);
	Extent.Max = D3DXVECTOR3(Min, Min, Min);

	if(MessageBox(Window, "This will calculate the extents of all model components. Proceed?\n(It may destroy existing extents)", "Message", MB_YESNO | MB_ICONQUESTION) == IDNO) return TRUE;

	for(i = 0; i < Model.Data().GeosetContainer.GetTotalSize(); i++)
	{
		if(Model.Data().GeosetContainer.ValidIndex(i))
		{
			Model.Data().GeosetContainer[i]->CalculateExtent(TempExtent);

			if(TempExtent.Radius > Extent.Radius) Extent.Radius = TempExtent.Radius;
			if(TempExtent.Min.x < Extent.Min.x) Extent.Min.x = TempExtent.Min.x;
			if(TempExtent.Min.y < Extent.Min.y) Extent.Min.y = TempExtent.Min.y;
			if(TempExtent.Min.z < Extent.Min.z) Extent.Min.z = TempExtent.Min.z;
			if(TempExtent.Max.x > Extent.Max.x) Extent.Max.x = TempExtent.Max.x;
			if(TempExtent.Max.y > Extent.Max.y) Extent.Max.y = TempExtent.Max.y;
			if(TempExtent.Max.z > Extent.Max.z) Extent.Max.z = TempExtent.Max.z;
		}
	}

	for(i = 0; i < Model.Data().SequenceContainer.GetTotalSize(); i++)
	{
		if(Model.Data().SequenceContainer.ValidIndex(i))
		{
			Model.Data().SequenceContainer[i]->Data().Extent = Extent;
		}
	}

	Model.Data().Info.Extent = Extent;

	MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Updates the window status
//+-----------------------------------------------------------------------------
VOID MAIN_WINDOW::UpdateWindowStatus(BOOL NoWindowHide)
{
	BOOL Gray;
	std::string Title;

	Gray = !ModelLoaded;

	GrayMenuItem(MainFileSave, Gray);
	GrayMenuItem(MainFileSaveAs, Gray);
	GrayMenuItem(MainFileClose, Gray);
	GrayMenuItem(MainViewResetCamera, Gray);
	GrayMenuItem(MainEditCalculateExtents, Gray);
	GrayMenuItem(MainEditModelProperties, Gray);
	GrayMenuItem(MainEditModelInfo, Gray);
	GrayMenuItem(MainWindowsModelEditor, Gray);
	GrayMenuItem(MainWindowsAnimationController, Gray);
	GrayMenuItem(MainWindowsNodeManager, Gray);
	GrayMenuItem(MainWindowsCameraManager, Gray);
	GrayMenuItem(MainWindowsGeosetManager, Gray);
	GrayMenuItem(MainWindowsGeosetAnimationManager, Gray);
	GrayMenuItem(MainWindowsTextureManager, Gray);
	GrayMenuItem(MainWindowsTextureAnimationManager, Gray);
	GrayMenuItem(MainWindowsMaterialManager, Gray);
	GrayMenuItem(MainWindowsSequenceManager, Gray);
	GrayMenuItem(MainWindowsGlobalSequenceManager, Gray);

	//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	//GrayMenuItem(MainWindowsModelEditor, TRUE);

	if(Gray && !NoWindowHide)
	{
		ModelWindow.Hide();
		AnimationWindow.Hide();
		NodeManagerWindow.Hide();
		CameraManagerWindow.Hide();
		GeosetManagerWindow.Hide();
		GeosetAnimationManagerWindow.Hide();
		TextureWindow.Hide();
		TextureManagerWindow.Hide();
		TextureAnimationManagerWindow.Hide();
		MaterialManagerWindow.Hide();
		SequenceManagerWindow.Hide();
		GlobalSequenceManagerWindow.Hide();
	}

	Title = EDITOR_NAME;
	if(ModelLoaded)
	{
		Title += (" (" + Common.GetFileName(ModelFileName) + ")");
		if(!ModelSaved) Title += " *";
	}

	SetText(Title);
}


//+-----------------------------------------------------------------------------
//| Renders the window
//+-----------------------------------------------------------------------------
VOID MAIN_WINDOW::Render(INT TimeDifference)
{
	if(!ModelLoaded)
	{
		Graphics.RenderFill(PROPERTIES_NO_BACKGROUND);
		return;
	}

	Graphics.RenderFill(Properties().Background);
	Graphics.RenderGroundTexture();

	Model.Render(TimeDifference);
	ParticleManager.Render();

	Graphics.RenderAxises();
	Graphics.RenderDebug();
}
