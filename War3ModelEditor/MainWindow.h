//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MAIN_WINDOW_H
#define MAGOS_MAIN_WINDOW_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Window.h"
#include "ResourceLoader.h"


//+-----------------------------------------------------------------------------
//| Main window class
//+-----------------------------------------------------------------------------
class MAIN_WINDOW : public WINDOW_FRAME
{
	public:
		CONSTRUCTOR MAIN_WINDOW();
		DESTRUCTOR ~MAIN_WINDOW();

		virtual BOOL Create();
		virtual VOID Destroy();

		virtual LRESULT MessageHandler(UINT Message, WPARAM W, LPARAM L);
		virtual LRESULT MenuHandler(WORD MenuItem);
		virtual LRESULT ControlHandler(HWND Control, WORD Code);
		virtual LRESULT NotifyHandler(HWND Control, UINT Code, NMHDR* Header);

		HMENU GetHistoryMenu();

		VOID UpdateAndRender(INT TimeDifference);

		BOOL ModelIsLoaded();
		VOID MakeModelUnsaved();
		VOID MakeModelNeverSaved();

		BOOL LoadByForce(CONST std::string& FileName);

		BOOL LoadFileFromBuffer(CONST std::string& FileName, BUFFER& Buffer, BOOL SkipSaveTest = FALSE);
		BOOL LoadModelFromBuffer(CONST std::string& FileName, BUFFER& Buffer);
		BOOL LoadTextureFromBuffer(CONST std::string& FileName, BUFFER& Buffer);

	protected:
		BOOL RegisterExtentions(REGISTER_INFO& RegisterInfo);

		BOOL NewFile();
		BOOL OpenFile();
		BOOL ImportFile();
		BOOL SaveFile(BOOL& Cancel, BOOL ForceNewName);
		BOOL SaveFileIfNeccessary(BOOL& Cancel);
		BOOL CloseFile(BOOL NoWindowHide = FALSE);
		BOOL NewLoadingScreen(CONST std::string& TextureFileName);

		BOOL DisplayHelp();
		BOOL CalculateExtents();
		VOID UpdateWindowStatus(BOOL NoWindowHide = FALSE);

		VOID Render(INT TimeDifference);

		GRAPHICS_WINDOW GraphicsWindow;

		BOOL ModelLoaded;
		BOOL ModelSaved;
		BOOL ModelNeverSaved;

		std::string ModelFileName;

		BOOL WindowActive;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern MAIN_WINDOW MainWindow;


//+-----------------------------------------------------------------------------
//| Post-included files
//+-----------------------------------------------------------------------------
#include "AnimationWindow.h"
#include "TextureWindow.h"
#include "ModelWindow.h"
#include "MpqWindow.h"
#include "NodeManagerWindow.h"
#include "CameraManagerWindow.h"
#include "GeosetManagerWindow.h"
#include "GeosetAnimationManagerWindow.h"
#include "TextureManagerWindow.h"
#include "TextureAnimationManagerWindow.h"
#include "MaterialManagerWindow.h"
#include "SequenceManagerWindow.h"
#include "GlobalSequenceManagerWindow.h"


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
