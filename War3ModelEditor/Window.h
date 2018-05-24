//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_H
#define MAGOS_WINDOW_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowQualityDialog.h"
#include "WindowFileDialog.h"
#include "WindowRegisterDialog.h"
#include "WindowPropertiesDialog.h"
#include "WindowModelPropertiesDialog.h"
#include "WindowModelInfoDialog.h"
#include "WindowColoredTextDialog.h"
#include "WindowLoadingScreenDialog.h"
#include "WindowAttachmentDialog.h"
#include "WindowBoneDialog.h"
#include "WindowCameraDialog.h"
#include "WindowCollisionShapeDialog.h"
#include "WindowEventObjectDialog.h"
#include "WindowHelperDialog.h"
#include "WindowLightDialog.h"
#include "WindowParticleEmitterDialog.h"
#include "WindowParticleEmitter2Dialog.h"
#include "WindowRibbonEmitterDialog.h"
#include "WindowNodeDialog.h"
#include "WindowGeosetDialog.h"
#include "WindowGeosetAnimationDialog.h"
#include "WindowTextureDialog.h"
#include "WindowTextureAnimationDialog.h"
#include "WindowMaterialDialog.h"
#include "WindowMaterialLayerDialog.h"
#include "WindowSequenceDialog.h"
#include "WindowGlobalSequenceDialog.h"
#include "WindowImportGeosetDialog.h"
#include "WindowVerticesDialog.h"
#include "WindowFacesDialog.h"
#include "WindowGroupsDialog.h"
#include "WindowExtentsDialog.h"
#include "WindowTranslateDialog.h"
#include "WindowRotateDialog.h"
#include "WindowScaleDialog.h"


//+-----------------------------------------------------------------------------
//| Window class
//+-----------------------------------------------------------------------------
class WINDOW
{
	public:
		CONSTRUCTOR WINDOW();
		DESTRUCTOR ~WINDOW();

		virtual VOID Destroy();

		HWND GetWindow() CONST;

		LRESULT SendMessage(UINT Message, WPARAM W, LPARAM L);

		VOID SetParent(HWND NewParent);
		VOID SetText(CONST std::string& NewText);

		HWND GetParent() CONST;
		std::string GetText() CONST;

		VOID CenterPosition();

		VOID SetX(INT NewX);
		VOID SetY(INT NewY);
		VOID SetPosition(INT NewX, INT NewY);
		VOID SetWidth(INT NewWidth);
		VOID SetHeight(INT NewHeight);
		VOID SetDimension(INT NewWidth, INT NewHeight);

		INT GetX() CONST;
		INT GetY() CONST;
		INT GetWidth() CONST;
		INT GetHeight() CONST;

		VOID Show();
		VOID Hide();
		VOID Activate();

		VOID Minimize();
		VOID Maximize();
		VOID Restore();

		VOID Enable();
		VOID Disable();

	protected:
		HWND Window;
};


//+-----------------------------------------------------------------------------
//| Post-included files
//+-----------------------------------------------------------------------------
#include "WindowEdit.h"
#include "WindowFrame.h"
#include "WindowButton.h"
#include "WindowListbox.h"
#include "WindowCombobox.h"
#include "WindowTreeview.h"


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
