//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "War3ModelEditor.h"


//+-----------------------------------------------------------------------------
//| Function prototypes
//+-----------------------------------------------------------------------------
BOOL Setup()
{
	InitCommonControls();

	if(!Register.FindWarcraftDirectory()) return FALSE;
	if(!Properties.LoadPropertiesInfo(PATH_PROPERTIES)) return FALSE;
	if(!EventObjectDialog.LoadData()) return FALSE;

	if(!Icon.LoadAllIcons()) return FALSE;
	if(!NodeIcon.LoadAllIcons()) return FALSE;
	if(!Filter.LoadAllFilters()) return FALSE;
	if(!ModelImporter.LoadAllImporters()) return FALSE;

	if(!Graphics.Setup()) return FALSE;
	if(!Graphics.SetupShaders()) return FALSE;
	if(!Font.Setup()) return FALSE;

	if(!MainWindow.Create()) return FALSE;
	if(!ModelWindow.Create()) return FALSE;
	if(!AnimationWindow.Create()) return FALSE;
	if(!TextureWindow.Create()) return FALSE;
	if(!NodeManagerWindow.Create()) return FALSE;
	if(!CameraManagerWindow.Create()) return FALSE;
	if(!GeosetManagerWindow.Create()) return FALSE;
	if(!GeosetAnimationManagerWindow.Create()) return FALSE;
	if(!TextureManagerWindow.Create()) return FALSE;
	if(!TextureAnimationManagerWindow.Create()) return FALSE;
	if(!MaterialManagerWindow.Create()) return FALSE;
	if(!SequenceManagerWindow.Create()) return FALSE;
	if(!GlobalSequenceManagerWindow.Create()) return FALSE;
	if(!MpqWindow.Create()) return FALSE;

	if(!Keyboard.Setup(MainWindow.GetWindow(), FALSE)) return FALSE;
	if(!ModelKeyboard.Setup(ModelWindow.GetWindow(), FALSE)) return FALSE;
	if(!Mouse.Setup(MainWindow.GetWindow(), FALSE)) return FALSE;
	if(!ModelMouse.Setup(ModelWindow.GetWindow(), FALSE)) return FALSE;

	if(!ModelHistory.Load()) return FALSE;

	Camera.Reset(MainWindow.GetWindow());
	ResourceLoader.RegisterAllLoaders();

	if(!TextureManager.LoadAllReplaceableTextures()) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Function prototypes
//+-----------------------------------------------------------------------------
VOID Shutdown()
{
	ResourceLoader.ClearAllLoaders();

	ModelMouse.Shutdown();
	Mouse.Shutdown();
	ModelKeyboard.Shutdown();
	Keyboard.Shutdown();

	MpqWindow.Destroy();
	GlobalSequenceManagerWindow.Destroy();
	SequenceManagerWindow.Destroy();
	MaterialManagerWindow.Destroy();
	TextureAnimationManagerWindow.Destroy();
	TextureManagerWindow.Destroy();
	GeosetAnimationManagerWindow.Destroy();
	GeosetManagerWindow.Destroy();
	CameraManagerWindow.Destroy();
	NodeManagerWindow.Destroy();
	TextureWindow.Destroy();
	AnimationWindow.Destroy();
	ModelWindow.Destroy();
	MainWindow.Destroy();

	ModelImporter.ClearAllImporters();
	Filter.ClearAllFilters();
	NodeIcon.ClearAllIcons();
	Icon.ClearAllIcons();

	TemporaryFile.DestroyAllTemporaryFiles();
	TextureManager.Clear();
	TextureManager.UnloadAllReplaceableTextures();

	Font.Shutdown();
	Graphics.ShutdownShaders();
	Graphics.Shutdown();
}


//+-----------------------------------------------------------------------------
//| Handles the windows messages
//+-----------------------------------------------------------------------------
BOOL HandleWindowsMessages()
{
	MSG Message;

	while(PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
		if(Message.message == WM_QUIT) return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Windows main function
//+-----------------------------------------------------------------------------
INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR Arguments, INT)
{
	std::string FileName;

	if(!Setup())
	{
		Shutdown();
		Error.DisplayMessage();
		return 0;
	}

	FileName = Arguments;
	Common.RemoveWhiteSpace(FileName);

	if(FileName != "")
	{
		if(!MainWindow.LoadByForce(FileName))
		{
			Shutdown();
			Error.DisplayMessage();
			return 0;
		}

		ModelHistory.AddFileName(FileName);
	}

	while(TRUE)
	{
		if(!HandleWindowsMessages())
		{
			break;
		}
		else if(Timer.Check())
		{
			Timer.Start(EDITOR_TIMER_PERIOD);

			Graphics.SetClippingPlane(Properties().NearDistance, Properties().FarDistance);

			FlashingGeoset.Update(EDITOR_TIMER_PERIOD);
			MainWindow.UpdateAndRender(EDITOR_TIMER_PERIOD);
			TextureWindow.UpdateAndRender();
			ModelWindow.UpdateAndRender();

			Timer.Sleep();
		}
	}

	Shutdown();
	Error.DisplayMessageIfSet();
	return 0;
}
