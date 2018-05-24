//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_EVENT_OBJECT_DIALOG_H
#define MAGOS_WINDOW_EVENT_OBJECT_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Event object dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_EVENT_OBJECT_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_EVENT_OBJECT_DIALOG();
		DESTRUCTOR ~WINDOW_EVENT_OBJECT_DIALOG();

		BOOL LoadData();

		BOOL Display(HWND ParentWindow, MODEL_EVENT_OBJECT_DATA& Data, MODEL_GLOBAL_SEQUENCE*& GlobalSequence) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);
		static BOOL CALLBACK TrackDialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static VOID SortedInsert(HWND ListBox, INT Data);
		static VOID BuildGlobalSequenceIdList(HWND Window);

		static VOID BuildSplatList(HWND Window);
		static VOID BuildUberSplatList(HWND Window);
		static VOID BuildSoundList(HWND Window);
		static VOID BuildSpawnObjectList(HWND Window);

		static VOID RegisterSplat(CONST std::string& Data, CONST std::string& Name);
		static VOID RegisterUberSplat(CONST std::string& Data, CONST std::string& Name);
		static VOID RegisterSound(CONST std::string& Data, CONST std::string& Name);
		static VOID RegisterSpawnObject(CONST std::string& Data, CONST std::string& Name);

		static INT GetComboIndex(HWND Window, CONST std::string& Data, std::map<std::string, std::string>& Map);

		static MODEL_EVENT_OBJECT_DATA StaticData;
		static MODEL_GLOBAL_SEQUENCE* StaticGlobalSequence;
		static std::string StaticNameType;
		static std::string StaticNameIdentifier;
		static std::string StaticNameData;

		static std::map<std::string, std::string> SplatMap;
		static std::map<std::string, std::string> UberSplatMap;
		static std::map<std::string, std::string> SoundMap;
		static std::map<std::string, std::string> SpawnObjectMap;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_EVENT_OBJECT_DIALOG EventObjectDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
