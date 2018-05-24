//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_COLLISION_SHAPE_DIALOG_H
#define MAGOS_WINDOW_COLLISION_SHAPE_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Collision shape dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_COLLISION_SHAPE_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_COLLISION_SHAPE_DIALOG();
		DESTRUCTOR ~WINDOW_COLLISION_SHAPE_DIALOG();

		BOOL Display(HWND ParentWindow, MODEL_COLLISION_SHAPE_DATA& Data) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static MODEL_COLLISION_SHAPE_DATA StaticData;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_COLLISION_SHAPE_DIALOG CollisionShapeDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
