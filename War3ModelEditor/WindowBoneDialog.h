//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_BONE_DIALOG_H
#define MAGOS_WINDOW_BONE_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Bone dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_BONE_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_BONE_DIALOG();
		DESTRUCTOR ~WINDOW_BONE_DIALOG();

		BOOL Display(HWND ParentWindow, MODEL_BONE_DATA& Data, MODEL_GEOSET*& Geoset, MODEL_GEOSET_ANIMATION*& GeosetAnimation) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static VOID BuildGeosetIdList(HWND Window);
		static VOID BuildGeosetAnimationIdList(HWND Window);

		static MODEL_BONE_DATA StaticData;
		static MODEL_GEOSET* StaticGeoset;
		static MODEL_GEOSET_ANIMATION* StaticGeosetAnimation;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_BONE_DIALOG BoneDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
