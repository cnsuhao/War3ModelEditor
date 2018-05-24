//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_WINDOW_H
#define MAGOS_MODEL_WINDOW_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "MainWindow.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| View index enumeration
//+-----------------------------------------------------------------------------
enum VIEW_INDEX
{
	VIEW_INDEX_TOP_LEFT,
	VIEW_INDEX_TOP_RIGHT,
	VIEW_INDEX_BOTTOM_LEFT,
	VIEW_INDEX_BOTTOM_RIGHT,
};


//+-----------------------------------------------------------------------------
//| View type enumeration
//+-----------------------------------------------------------------------------
enum VIEW_TYPE
{
	VIEW_TYPE_NONE,
	VIEW_TYPE_XY_FRONT,
	VIEW_TYPE_XY_BACK,
	VIEW_TYPE_XZ_FRONT,
	VIEW_TYPE_XZ_BACK,
	VIEW_TYPE_YZ_FRONT,
	VIEW_TYPE_YZ_BACK,
};


//+-----------------------------------------------------------------------------
//| View name list
//+-----------------------------------------------------------------------------
CONST CHAR* CONST VIEW_NAME[] =
{
	"None",
	"+X/Y",
	"-X/Y",
	"+X/Z",
	"-X/Z",
	"+Y/Z",
	"-Y/Z",
};


//+-----------------------------------------------------------------------------
//| Model view structure
//+-----------------------------------------------------------------------------
struct MODEL_VIEW
{
	MODEL_VIEW()
	{
		Window = NULL;
		ViewType = VIEW_TYPE_NONE;

		Scale = 1.0f;
		Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		X = 0;
		Y = 0;
		Width = 0;
		Height = 0;
	}

	HWND Window;
	VIEW_TYPE ViewType;
	GRAPHICS_WINDOW GraphicsWindow;

	FLOAT Scale;
	D3DXVECTOR3 Position;

	INT X;
	INT Y;
	INT Width;
	INT Height;
};


//+-----------------------------------------------------------------------------
//| Model window class
//+-----------------------------------------------------------------------------
class MODEL_WINDOW : public WINDOW_FRAME
{
	public:
		CONSTRUCTOR MODEL_WINDOW();
		DESTRUCTOR ~MODEL_WINDOW();

		virtual BOOL Create();
		virtual VOID Destroy();

		virtual LRESULT MessageHandler(UINT Message, WPARAM W, LPARAM L);
		virtual LRESULT MenuHandler(WORD MenuItem);
		virtual LRESULT ControlHandler(HWND Control, WORD Code);
		virtual LRESULT NotifyHandler(HWND Control, UINT Code, NMHDR* Header);

		VOID UpdateAndRender();

	protected:
		BOOL Translate();
		BOOL Rotate();
		BOOL Scale();

		VOID Select();
		VOID SelectAllVertices();
		VOID SelectAllFaces();
		VOID SelectNoVertices();
		VOID SelectNoFaces();

		VOID RenderView(INT Index, VIEW_TYPE ViewType);
		VOID RenderCross(INT ViewIndex, CONST D3DXVECTOR3& Position, FLOAT Scale, D3DCOLOR Color);
		VOID RenderSelectionBox();
		VOID RenderVertices(INT ViewIndex);
		VOID BuildViewMatrices(INT Index, VIEW_TYPE ViewType);

		D3DXPLANE BuildProjectionPlane(INT ViewIndex, CONST D3DXVECTOR3& Point);
		D3DXVECTOR3 ScreenPositionToWorldPosition(INT ViewIndex, CONST D3DXPLANE& ProjectionPlane, INT X, INT Y);

		HWND CreateView();
		VOID ResizeViews();
		VOID SelectView(INT View);
		INT GetView(INT X, INT Y);

		D3DXVECTOR3 CalculateMassCentre();

		INT CurrentView;
		INT ActiveLeftButtonView;
		INT ActiveRightButtonView;
		MODEL_VIEW ViewList[4];

		D3DXVECTOR3 PivotPoint;

		BOOL Selecting;
		BOOL DeselectMode;
		INT SelectingView;
		D3DXVECTOR3 SelectionStart;
		D3DXVECTOR3 SelectionEnd;

		BOOL WindowActive;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern MODEL_WINDOW ModelWindow;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
