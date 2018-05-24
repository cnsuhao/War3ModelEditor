//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_MATERIAL_LAYER_DIALOG_H
#define MAGOS_WINDOW_MATERIAL_LAYER_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Material layer dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_MATERIAL_LAYER_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_MATERIAL_LAYER_DIALOG();
		DESTRUCTOR ~WINDOW_MATERIAL_LAYER_DIALOG();

		BOOL Display(HWND ParentWindow, MODEL_MATERIAL_LAYER_DATA& Data, MODEL_TEXTURE*& Texture, MODEL_TEXTURE_ANIMATION*& TextureAnimation) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static VOID BuildTextureIdList(HWND Window);
		static VOID BuildTextureAnimationIdList(HWND Window);

		static MODEL_MATERIAL_LAYER_DATA StaticData;
		static MODEL_TEXTURE* StaticTexture;
		static MODEL_TEXTURE_ANIMATION* StaticTextureAnimation;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_MATERIAL_LAYER_DIALOG MaterialLayerDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
