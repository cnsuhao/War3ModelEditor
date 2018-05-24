//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ModelWindow.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
MODEL_WINDOW ModelWindow;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_WINDOW::MODEL_WINDOW()
{
	CurrentView = INVALID_INDEX;
	ActiveLeftButtonView = INVALID_INDEX;
	ActiveRightButtonView = INVALID_INDEX;

	ViewList[VIEW_INDEX_TOP_LEFT].ViewType = VIEW_TYPE_XZ_FRONT;
	ViewList[VIEW_INDEX_TOP_RIGHT].ViewType = VIEW_TYPE_YZ_BACK;
	ViewList[VIEW_INDEX_BOTTOM_LEFT].ViewType = VIEW_TYPE_XY_FRONT;
	ViewList[VIEW_INDEX_BOTTOM_RIGHT].ViewType = VIEW_TYPE_NONE;

	PivotPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	Selecting = FALSE;
	DeselectMode = FALSE;
	SelectingView = INVALID_INDEX;
	SelectionStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	SelectionEnd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	WindowActive = FALSE;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL_WINDOW::~MODEL_WINDOW()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Creates a new window
//+-----------------------------------------------------------------------------
BOOL MODEL_WINDOW::Create()
{
	INT i;

	FrameInfo.Title = "Model Editor";
	FrameInfo.Width = DEFAULT_MODEL_WINDOW_WIDTH;
	FrameInfo.Height = DEFAULT_MODEL_WINDOW_HEIGHT;
	FrameInfo.Menu = ::LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(ModelMenu));
	FrameInfo.Style &= (~WS_VISIBLE);
	FrameInfo.Style |= (WS_MAXIMIZEBOX | WS_SIZEBOX);

	if(!WINDOW_FRAME::Create()) return FALSE;

	for(i = 0; i < 4; i++)
	{
		ViewList[i].Window = CreateView();
		if(ViewList[i].Window == NULL) return FALSE;
	}

	ResizeViews();

	for(i = 0; i < 4; i++)
	{
		if(!ViewList[i].GraphicsWindow.Create(ViewList[i].Window)) return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Destroys the window
//+-----------------------------------------------------------------------------
VOID MODEL_WINDOW::Destroy()
{
	INT i;

	CurrentView = INVALID_INDEX;

	for(i = 0; i < 4; i++)
	{
		ViewList[i].GraphicsWindow.Destroy();
		SAFE_DESTROY(ViewList[i].Window);
	}

	WINDOW_FRAME::Destroy();
}


//+-----------------------------------------------------------------------------
//| Handles the window messages
//+-----------------------------------------------------------------------------
LRESULT MODEL_WINDOW::MessageHandler(UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_PAINT:
		{
			UpdateAndRender();
			ValidateRect(Window, NULL);
			return 0;
		}

		case WM_SIZE:
		{
			WindowActive = (W != SIZE_MINIMIZED);
			ResizeViews();
			return 0;
		}

		case WM_SIZING:
		{
			ResizeViews();
			return 0;
		}

		case WM_SHOWWINDOW:
		{
			WindowActive = static_cast<BOOL>(W);
			return 0;
		}

		case WM_CLOSE:
		{
			Hide();
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
LRESULT MODEL_WINDOW::MenuHandler(WORD MenuItem)
{
	switch(MenuItem)
	{
		case ModelViewNone:
		{
			if(CurrentView == INVALID_INDEX)
			{
				Error.SetMessage("No view is selected!");
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			ViewList[CurrentView].ViewType = VIEW_TYPE_NONE;

			return 0;
		}

		case ModelViewXYFront:
		{
			if(CurrentView == INVALID_INDEX)
			{
				Error.SetMessage("No view is selected!");
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			ViewList[CurrentView].ViewType = VIEW_TYPE_XY_FRONT;

			return 0;
		}

		case ModelViewXYBack:
		{
			if(CurrentView == INVALID_INDEX)
			{
				Error.SetMessage("No view is selected!");
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			ViewList[CurrentView].ViewType = VIEW_TYPE_XY_BACK;

			return 0;
		}

		case ModelViewXZFront:
		{
			if(CurrentView == INVALID_INDEX)
			{
				Error.SetMessage("No view is selected!");
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			ViewList[CurrentView].ViewType = VIEW_TYPE_XZ_FRONT;

			return 0;
		}

		case ModelViewXZBack:
		{
			if(CurrentView == INVALID_INDEX)
			{
				Error.SetMessage("No view is selected!");
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			ViewList[CurrentView].ViewType = VIEW_TYPE_XZ_BACK;

			return 0;
		}

		case ModelViewYZFront:
		{
			if(CurrentView == INVALID_INDEX)
			{
				Error.SetMessage("No view is selected!");
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			ViewList[CurrentView].ViewType = VIEW_TYPE_YZ_FRONT;

			return 0;
		}

		case ModelViewYZBack:
		{
			if(CurrentView == INVALID_INDEX)
			{
				Error.SetMessage("No view is selected!");
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			ViewList[CurrentView].ViewType = VIEW_TYPE_YZ_BACK;

			return 0;
		}

		case ModelViewResize:
		{
			SetWindowDimension(DEFAULT_MODEL_WINDOW_WIDTH, DEFAULT_MODEL_WINDOW_HEIGHT);
			ResizeViews();
			return 0;
		}

		case ModelViewResetPivotPoint:
		{
			PivotPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			return 0;
		}

		case ModelViewResetCamera:
		{
			INT i;

			for(i = 0; i < 4; i++)
			{
				ViewList[i].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				ViewList[i].Scale = 1.0f;
			}

			return 0;
		}

		case ModelSelectionAll:
		{
			SelectAllVertices();
			SelectAllFaces();
			return 0;
		}

		case ModelSelectionNone:
		{
			SelectNoVertices();
			SelectNoFaces();
			return 0;
		}

		case ModelTransformationsTranslate:
		{
			if(!Translate())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case ModelTransformationsRotate:
		{
			if(!Rotate())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}

		case ModelTransformationsScale:
		{
			if(!Scale())
			{
				Error.DisplayMessage(Window);
				Error.ClearMessage();
				return 0;
			}

			return 0;
		}
	}

	return 0;
}


//+-----------------------------------------------------------------------------
//| Handles the window control messages
//+-----------------------------------------------------------------------------
LRESULT MODEL_WINDOW::ControlHandler(HWND Control, WORD Code)
{
	return 0;
}


//+-----------------------------------------------------------------------------
//| Handles the window notify messages
//+-----------------------------------------------------------------------------
LRESULT MODEL_WINDOW::NotifyHandler(HWND Control, UINT Code, NMHDR* Header)
{
	return 0;
}


//+-----------------------------------------------------------------------------
//| Updates and renders the window
//+-----------------------------------------------------------------------------
VOID MODEL_WINDOW::UpdateAndRender()
{
	INT i;
	INT View;
	POINT MousePosition;
	POINT TempMousePosition;
	POINT LocalMousePosition;

	ModelKeyboard.Update();
	ModelMouse.Update();

	GetCursorPos(&MousePosition);
	TempMousePosition = MousePosition;
	ScreenToClient(Window, &MousePosition);
	View = GetView(MousePosition.x, MousePosition.y);

	DeselectMode = ModelKeyboard.KeyDown(KEY_LEFTSHIFT);

	if(ModelMouse.ButtonPressed(BUTTON_LEFT))
	{
		SelectView(GetView(MousePosition.x, MousePosition.y));
		ActiveLeftButtonView = View;
	}

	if(ModelMouse.ButtonReleased(BUTTON_LEFT))
	{
		ActiveLeftButtonView = INVALID_INDEX;

		if(Selecting)
		{
			Select();
			Selecting = FALSE;
			SelectingView = INVALID_INDEX;
		}
	}

	if(ModelMouse.ButtonPressed(BUTTON_RIGHT))
	{
		SelectView(GetView(MousePosition.x, MousePosition.y));
		ActiveRightButtonView = View;
	}

	if(ModelMouse.ButtonReleased(BUTTON_RIGHT))
	{
		ActiveRightButtonView = INVALID_INDEX;
	}

	if(ActiveLeftButtonView != INVALID_INDEX)
	{
		if(ModelMouse.ButtonPressed(BUTTON_LEFT))
		{
			if(ModelKeyboard.KeyDown(KEY_LEFTCTRL))
			{
				LocalMousePosition = TempMousePosition;
				ScreenToClient(ViewList[ActiveLeftButtonView].Window, &LocalMousePosition);
				PivotPoint = ScreenPositionToWorldPosition(ActiveLeftButtonView, BuildProjectionPlane(ActiveLeftButtonView, PivotPoint), LocalMousePosition.x, LocalMousePosition.y);
			}
			else if(!ModelKeyboard.KeyDown(KEY_LEFTALT))
			{
				Selecting = TRUE;
				SelectingView = ActiveLeftButtonView;
				LocalMousePosition = TempMousePosition;
				ScreenToClient(ViewList[ActiveLeftButtonView].Window, &LocalMousePosition);
				SelectionStart = ScreenPositionToWorldPosition(ActiveLeftButtonView, BuildProjectionPlane(ActiveLeftButtonView, D3DXVECTOR3(0.0f, 0.0f, 0.0f)), LocalMousePosition.x, LocalMousePosition.y);
				SelectionEnd = SelectionStart;
			}
		}
	}

	if(Selecting)
	{
		if((ActiveLeftButtonView == SelectingView) && (CurrentView == SelectingView) && !ModelKeyboard.KeyDown(KEY_LEFTALT))
		{
			LocalMousePosition = TempMousePosition;
			ScreenToClient(ViewList[ActiveLeftButtonView].Window, &LocalMousePosition);
			SelectionEnd = ScreenPositionToWorldPosition(ActiveLeftButtonView, BuildProjectionPlane(ActiveLeftButtonView, D3DXVECTOR3(0.0f, 0.0f, 0.0f)), LocalMousePosition.x, LocalMousePosition.y);
		}
		else
		{
			Selecting = FALSE;
			SelectingView = INVALID_INDEX;
		}
	}

	if(ActiveLeftButtonView != INVALID_INDEX)
	{
		if(ModelKeyboard.KeyDown(KEY_LEFTALT) && ModelMouse.ButtonDown(BUTTON_LEFT))
		{
			ViewList[ActiveLeftButtonView].Scale -= (static_cast<FLOAT>(ModelMouse.GetDY()) * CAMERA_FACTOR_DISTANCE * 10.0f * ViewList[ActiveLeftButtonView].Scale);
		}

		if(ViewList[ActiveLeftButtonView].Scale < 0.01f) ViewList[ActiveLeftButtonView].Scale = 0.01f;
		if(ViewList[ActiveLeftButtonView].Scale > 100.0f) ViewList[ActiveLeftButtonView].Scale = 100.0f;
	}

	if(CurrentView != INVALID_INDEX)
	{
		ViewList[CurrentView].Scale += (static_cast<FLOAT>(ModelMouse.GetDZ()) * CAMERA_FACTOR_DISTANCE * ViewList[CurrentView].Scale);

		if(ViewList[CurrentView].Scale < 0.01f) ViewList[CurrentView].Scale = 0.01f;
		if(ViewList[CurrentView].Scale > 100.0f) ViewList[CurrentView].Scale = 100.0f;
	}

	if(ModelMouse.ButtonDown(BUTTON_RIGHT) && (ActiveRightButtonView != INVALID_INDEX))
	{
		FLOAT TempX;
		FLOAT TempY;

		TempX = static_cast<FLOAT>(ModelMouse.GetDX()) * 2.0f / ViewList[ActiveRightButtonView].Scale;
		TempY = static_cast<FLOAT>(-ModelMouse.GetDY()) * 2.0f / ViewList[ActiveRightButtonView].Scale;

		switch(ViewList[ActiveRightButtonView].ViewType)
		{
			case VIEW_TYPE_NONE:
			{
				break;
			}

			case VIEW_TYPE_XY_FRONT:
			{
				ViewList[ActiveRightButtonView].Position.x += TempX;
				ViewList[ActiveRightButtonView].Position.y += TempY;
				break;
			}

			case VIEW_TYPE_XY_BACK:
			{
				ViewList[ActiveRightButtonView].Position.x -= TempX;
				ViewList[ActiveRightButtonView].Position.y += TempY;
				break;
			}

			case VIEW_TYPE_XZ_FRONT:
			{
				ViewList[ActiveRightButtonView].Position.x += TempX;
				ViewList[ActiveRightButtonView].Position.z += TempY;
				break;
			}

			case VIEW_TYPE_XZ_BACK:
			{
				ViewList[ActiveRightButtonView].Position.x -= TempX;
				ViewList[ActiveRightButtonView].Position.z += TempY;
				break;
			}

			case VIEW_TYPE_YZ_FRONT:
			{
				ViewList[ActiveRightButtonView].Position.y += TempX;
				ViewList[ActiveRightButtonView].Position.z += TempY;
				break;
			}

			case VIEW_TYPE_YZ_BACK:
			{
				ViewList[ActiveRightButtonView].Position.y -= TempX;
				ViewList[ActiveRightButtonView].Position.z += TempY;
				break;
			}
		}
	}

	if(!WindowActive) return;

	for(i = 0; i < 4; i++)
	{
		if(Graphics.BeginRender(ViewList[i].GraphicsWindow))
		{
			RenderView(i, ViewList[i].ViewType);
			Graphics.EndRender();
		}
	}
}


//+-----------------------------------------------------------------------------
//| Translates the selected vertices
//+-----------------------------------------------------------------------------
BOOL MODEL_WINDOW::Translate()
{
	INT i;
	MODEL_GEOSET* Geoset;
	MODEL_GEOSET_VERTEX* GeosetVertex;
	TRANSLATE_INFO TranslateInfo;
	std::set<INT>::iterator j;

	if(!TranslateDialog.Display(Window, TranslateInfo)) return TRUE;

	for(i = 0; i < Model.Data().GeosetContainer.GetTotalSize(); i++)
	{
		if(Model.Data().GeosetContainer.ValidIndex(i))
		{
			Geoset = Model.Data().GeosetContainer[i];
			j = Geoset->SelectedVertices.begin();
			while(j != Geoset->SelectedVertices.end())
			{
				GeosetVertex = Geoset->Data().VertexContainer[*j];
				GeosetVertex->Position += TranslateInfo.Translation;
				j++;
			}
		}
	}

	Model.Rebuild();
	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Rotates the selected vertices
//+-----------------------------------------------------------------------------
BOOL MODEL_WINDOW::Rotate()
{
	INT i;
	FLOAT Angle;
	D3DXMATRIX Matrix;
	D3DXMATRIX TempMatrix;
	D3DXVECTOR3 Centre;
	D3DXVECTOR4 TempVector;
	MODEL_GEOSET* Geoset;
	MODEL_GEOSET_VERTEX* GeosetVertex;
	ROTATE_INFO RotateInfo;
	std::set<INT>::iterator j;

	RotateInfo.PivotPoint = PivotPoint;

	if(!RotateDialog.Display(Window, RotateInfo)) return TRUE;

	if(RotateInfo.AroundPivotPoint)
	{
		PivotPoint = RotateInfo.PivotPoint;
		Centre = PivotPoint;
	}
	else
	{
		Centre = CalculateMassCentre();
	}

	Angle = RotateInfo.Angle;
	if(!RotateInfo.Radians) Angle *= (D3DX_PI / 180.0f);

	switch(RotateInfo.Type)
	{
		case ROTATION_TYPE_X:
		{
			D3DXMatrixRotationX(&Matrix, Angle);
			break;
		}

		case ROTATION_TYPE_Y:
		{
			D3DXMatrixRotationY(&Matrix, Angle);
			break;
		}

		case ROTATION_TYPE_Z:
		{
			D3DXMatrixRotationZ(&Matrix, Angle);
			break;
		}

		case ROTATION_TYPE_AXIS:
		{
			D3DXMatrixRotationAxis(&Matrix, &RotateInfo.Axis, Angle);
			break;
		}

		case ROTATION_TYPE_QUATERNION:
		{
			D3DXMatrixRotationQuaternion(&Matrix, &RotateInfo.Quaternion);
			break;
		}

		default:
		{
			D3DXMatrixIdentity(&Matrix);
			break;
		}
	}

	D3DXMatrixTranslation(&TempMatrix, -Centre.x, -Centre.y, -Centre.z);
	D3DXMatrixMultiply(&Matrix, &TempMatrix, &Matrix);
	D3DXMatrixTranslation(&TempMatrix, Centre.x, Centre.y, Centre.z);
	D3DXMatrixMultiply(&Matrix, &Matrix, &TempMatrix);

	for(i = 0; i < Model.Data().GeosetContainer.GetTotalSize(); i++)
	{
		if(Model.Data().GeosetContainer.ValidIndex(i))
		{
			Geoset = Model.Data().GeosetContainer[i];
			j = Geoset->SelectedVertices.begin();
			while(j != Geoset->SelectedVertices.end())
			{
				GeosetVertex = Geoset->Data().VertexContainer[*j];
				D3DXVec3Transform(&TempVector, &GeosetVertex->Position, &Matrix);
				GeosetVertex->Position.x = TempVector.x;
				GeosetVertex->Position.y = TempVector.y;
				GeosetVertex->Position.z = TempVector.z;
				j++;
			}
		}
	}

	Model.Rebuild();
	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Scales the selected vertices
//+-----------------------------------------------------------------------------
BOOL MODEL_WINDOW::Scale()
{
	INT i;
	D3DXVECTOR3 Centre;
	MODEL_GEOSET* Geoset;
	MODEL_GEOSET_VERTEX* GeosetVertex;
	SCALE_INFO ScaleInfo;
	std::set<INT>::iterator j;

	ScaleInfo.PivotPoint = PivotPoint;

	if(!ScaleDialog.Display(Window, ScaleInfo)) return TRUE;

	if(ScaleInfo.AroundPivotPoint)
	{
		PivotPoint = ScaleInfo.PivotPoint;
		Centre = PivotPoint;
	}
	else
	{
		Centre = CalculateMassCentre();
	}

	for(i = 0; i < Model.Data().GeosetContainer.GetTotalSize(); i++)
	{
		if(Model.Data().GeosetContainer.ValidIndex(i))
		{
			Geoset = Model.Data().GeosetContainer[i];
			j = Geoset->SelectedVertices.begin();
			while(j != Geoset->SelectedVertices.end())
			{
				GeosetVertex = Geoset->Data().VertexContainer[*j];
				GeosetVertex->Position -= Centre;
				GeosetVertex->Position.x *= ScaleInfo.Scaling.x;
				GeosetVertex->Position.y *= ScaleInfo.Scaling.y;
				GeosetVertex->Position.z *= ScaleInfo.Scaling.z;
				GeosetVertex->Position += Centre;
				j++;
			}
		}
	}

	Model.Rebuild();
	MainWindow.MakeModelUnsaved();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Selects vertices and faces
//+-----------------------------------------------------------------------------
VOID MODEL_WINDOW::Select()
{
	INT i;
	INT j;
	D3DXVECTOR3 Min;
	D3DXVECTOR3 Max;
	MODEL_GEOSET* Geoset;
	MODEL_GEOSET_VERTEX* GeosetVertex;
	std::set<INT>::iterator Iterator;

	if(SelectingView == INVALID_INDEX) return;

	Min.x = min(SelectionStart.x, SelectionEnd.x);
	Min.y = min(SelectionStart.y, SelectionEnd.y);
	Min.z = min(SelectionStart.z, SelectionEnd.z);
	Max.x = max(SelectionStart.x, SelectionEnd.x);
	Max.y = max(SelectionStart.y, SelectionEnd.y);
	Max.z = max(SelectionStart.z, SelectionEnd.z);

	switch(ViewList[SelectingView].ViewType)
	{
		case VIEW_TYPE_XY_FRONT:
		case VIEW_TYPE_XY_BACK:
		{
			//Min.z = -std::numeric_limits<FLOAT>::max();
			//Max.z = std::numeric_limits<FLOAT>::max();
			break;
		}

		case VIEW_TYPE_XZ_FRONT:
		case VIEW_TYPE_XZ_BACK:
		{
			//Min.y = -std::numeric_limits<FLOAT>::max();
			//Max.y = std::numeric_limits<FLOAT>::max();
			break;
		}

		case VIEW_TYPE_YZ_FRONT:
		case VIEW_TYPE_YZ_BACK:
		{
			//Min.x = -std::numeric_limits<FLOAT>::max();
			//Max.x = std::numeric_limits<FLOAT>::max();
			break;
		}
	}

	for(i = 0; i < Model.Data().GeosetContainer.GetTotalSize(); i++)
	{
		if(Model.Data().GeosetContainer.ValidIndex(i))
		{
			Geoset = Model.Data().GeosetContainer[i];
			for(j = 0; j < Geoset->Data().VertexContainer.GetTotalSize(); j++)
			{
				if(Geoset->Data().VertexContainer.ValidIndex(j))
				{
					GeosetVertex = Geoset->Data().VertexContainer[j];
					while(TRUE)
					{
						if(GeosetVertex->Position.x < Min.x) break;
						if(GeosetVertex->Position.y < Min.y) break;
						if(GeosetVertex->Position.z < Min.z) break;
						if(GeosetVertex->Position.x > Max.x) break;
						if(GeosetVertex->Position.y > Max.y) break;
						if(GeosetVertex->Position.z > Max.z) break;

						if(DeselectMode)
						{
							Iterator = Geoset->SelectedVertices.find(j);
							if(Iterator != Geoset->SelectedVertices.end())
							{
								Geoset->SelectedVertices.erase(Iterator);
							}
						}
						else
						{
							Geoset->SelectedVertices.insert(j);
						}

						break;
					}
				}
			}
		}
	}
}


//+-----------------------------------------------------------------------------
//| Selects all vertices
//+-----------------------------------------------------------------------------
VOID MODEL_WINDOW::SelectAllVertices()
{
	INT i;
	INT j;
	MODEL_GEOSET* Geoset;

	for(i = 0; i < Model.Data().GeosetContainer.GetTotalSize(); i++)
	{
		if(Model.Data().GeosetContainer.ValidIndex(i))
		{
			Geoset = Model.Data().GeosetContainer[i];
			for(j = 0; j < Geoset->Data().VertexContainer.GetTotalSize(); j++)
			{
				if(Geoset->Data().VertexContainer.ValidIndex(j))
				{
					Geoset->SelectedVertices.insert(j);
				}
			}
		}
	}
}


//+-----------------------------------------------------------------------------
//| Selects all faces
//+-----------------------------------------------------------------------------
VOID MODEL_WINDOW::SelectAllFaces()
{
	INT i;
	INT j;
	MODEL_GEOSET* Geoset;

	for(i = 0; i < Model.Data().GeosetContainer.GetTotalSize(); i++)
	{
		if(Model.Data().GeosetContainer.ValidIndex(i))
		{
			Geoset = Model.Data().GeosetContainer[i];
			for(j = 0; j < Geoset->Data().FaceContainer.GetTotalSize(); j++)
			{
				if(Geoset->Data().VertexContainer.ValidIndex(j))
				{
					Geoset->SelectedFaces.insert(j);
				}
			}
		}
	}
}


//+-----------------------------------------------------------------------------
//| Selects no vertices
//+-----------------------------------------------------------------------------
VOID MODEL_WINDOW::SelectNoVertices()
{
	INT i;

	for(i = 0; i < Model.Data().GeosetContainer.GetTotalSize(); i++)
	{
		if(Model.Data().GeosetContainer.ValidIndex(i))
		{
			Model.Data().GeosetContainer[i]->SelectedVertices.clear();
		}
	}
}


//+-----------------------------------------------------------------------------
//| Selects no faces
//+-----------------------------------------------------------------------------
VOID MODEL_WINDOW::SelectNoFaces()
{
	INT i;

	for(i = 0; i < Model.Data().GeosetContainer.GetTotalSize(); i++)
	{
		if(Model.Data().GeosetContainer.ValidIndex(i))
		{
			Model.Data().GeosetContainer[i]->SelectedFaces.clear();
		}
	}
}


//+-----------------------------------------------------------------------------
//| Renders a view
//+-----------------------------------------------------------------------------
VOID MODEL_WINDOW::RenderView(INT Index, VIEW_TYPE ViewType)
{
	INT i;
	D3DXMATRIX Matrix;
	D3DXMATRIX TempMatrix;
	LINE_VERTEX Line_Vertex;

	if(Index == CurrentView) Graphics.RenderFill(COLOR_BACKGROUND_SELECTED);
	else Graphics.RenderFill(COLOR_BACKGROUND_UNSELECTED);

	Graphics.PrepareForViews();
	BuildViewMatrices(Index, ViewType);

	if(ViewList[Index].ViewType != VIEW_TYPE_NONE)
	{
		Graphics.GetDevice()->SetRenderState(D3DRS_BLENDFACTOR, COLOR_FACE);
		Graphics.GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		for(i = 0; i < Model.Data().GeosetContainer.GetTotalSize(); i++)
		{
			if(Model.Data().GeosetContainer.ValidIndex(i))
			{
				Model.Data().GeosetContainer[i]->RenderInView();
			}
		}

		Graphics.GetDevice()->SetFVF(Line_Vertex.FORMAT);
		Graphics.GetDevice()->SetRenderState(D3DRS_BLENDFACTOR, 0xFFFFFFFF);

		RenderVertices(Index);
		RenderCross(Index, PivotPoint, 10.0f, COLOR_PIVOT_POINT);

		if(Selecting && (Index == SelectingView)) RenderSelectionBox();
	}

	Graphics.RenderText(VIEW_NAME[ViewType], 0, 0, COLOR_TEXT);
}


//+-----------------------------------------------------------------------------
//| Renders a cross
//+-----------------------------------------------------------------------------
VOID MODEL_WINDOW::RenderCross(INT ViewIndex, CONST D3DXVECTOR3& Position, FLOAT Scale, D3DCOLOR Color)
{
	FLOAT Size;

	Size = (ViewList[ViewIndex].Scale == 0.0f) ? Scale : Scale / ViewList[ViewIndex].Scale;

	Graphics.RenderLine(Position - D3DXVECTOR3(Size, 0.0f, 0.0f), Position + D3DXVECTOR3(Size, 0.0f, 0.0f), Color);
	Graphics.RenderLine(Position - D3DXVECTOR3(0.0f, Size, 0.0f), Position + D3DXVECTOR3(0.0f, Size, 0.0f), Color);
	Graphics.RenderLine(Position - D3DXVECTOR3(0.0f, 0.0f, Size), Position + D3DXVECTOR3(0.0f, 0.0f, Size), Color);
}


//+-----------------------------------------------------------------------------
//| Renders the selection box
//+-----------------------------------------------------------------------------
VOID MODEL_WINDOW::RenderSelectionBox()
{
	D3DCOLOR Color;
	D3DXVECTOR3 Corner1;
	D3DXVECTOR3 Corner2;
	D3DXVECTOR3 Corner3;
	D3DXVECTOR3 Corner4;
	D3DXVECTOR3 Corner5;
	D3DXVECTOR3 Corner6;
	D3DXVECTOR3 Corner7;
	D3DXVECTOR3 Corner8;
	LINE_VERTEX Line_Vertex;

	Color = DeselectMode ? COLOR_FRAME_DESELECT : COLOR_FRAME_SELECT;

	Corner1 = D3DXVECTOR3(SelectionStart.x, SelectionStart.y, SelectionStart.z);
	Corner2 = D3DXVECTOR3(SelectionStart.x, SelectionStart.y, SelectionEnd.z);
	Corner3 = D3DXVECTOR3(SelectionStart.x, SelectionEnd.y, SelectionStart.z);
	Corner4 = D3DXVECTOR3(SelectionStart.x, SelectionEnd.y, SelectionEnd.z);
	Corner5 = D3DXVECTOR3(SelectionEnd.x, SelectionStart.y, SelectionStart.z);
	Corner6 = D3DXVECTOR3(SelectionEnd.x, SelectionStart.y, SelectionEnd.z);
	Corner7 = D3DXVECTOR3(SelectionEnd.x, SelectionEnd.y, SelectionStart.z);
	Corner8 = D3DXVECTOR3(SelectionEnd.x, SelectionEnd.y, SelectionEnd.z);

	Graphics.GetDevice()->SetFVF(Line_Vertex.FORMAT);
	Graphics.GetDevice()->SetRenderState(D3DRS_BLENDFACTOR, 0xFFFFFFFF);

	Graphics.RenderLine(Corner1, Corner2, Color);
	Graphics.RenderLine(Corner1, Corner3, Color);
	Graphics.RenderLine(Corner1, Corner5, Color);
	Graphics.RenderLine(Corner2, Corner4, Color);
	Graphics.RenderLine(Corner2, Corner6, Color);
	Graphics.RenderLine(Corner3, Corner4, Color);
	Graphics.RenderLine(Corner3, Corner7, Color);
	Graphics.RenderLine(Corner4, Corner8, Color);
	Graphics.RenderLine(Corner5, Corner6, Color);
	Graphics.RenderLine(Corner5, Corner7, Color);
	Graphics.RenderLine(Corner6, Corner8, Color);
	Graphics.RenderLine(Corner7, Corner8, Color);
}


//+-----------------------------------------------------------------------------
//| Renders the vertices
//+-----------------------------------------------------------------------------
VOID MODEL_WINDOW::RenderVertices(INT ViewIndex)
{
	INT i;
	INT j;
	MODEL_GEOSET* Geoset;
	MODEL_GEOSET_VERTEX* GeosetVertex;

	for(i = 0; i < Model.Data().GeosetContainer.GetTotalSize(); i++)
	{
		if(Model.Data().GeosetContainer.ValidIndex(i))
		{
			Geoset = Model.Data().GeosetContainer[i];
			for(j = 0; j < Geoset->Data().VertexContainer.GetTotalSize(); j++)
			{
				if(Geoset->Data().VertexContainer.ValidIndex(j))
				{
					GeosetVertex = Geoset->Data().VertexContainer[j];
					if(Geoset->SelectedVertices.find(j) != Geoset->SelectedVertices.end())
					{
						RenderCross(ViewIndex, GeosetVertex->Position, 2.5f, COLOR_VERTEX_SELECTED);
					}
					else
					{
						RenderCross(ViewIndex, GeosetVertex->Position, 2.5f, COLOR_VERTEX_DESELECTED);
					}
				}
			}
		}
	}
}


//+-----------------------------------------------------------------------------
//| Builds the matrices for a view
//+-----------------------------------------------------------------------------
VOID MODEL_WINDOW::BuildViewMatrices(INT Index, VIEW_TYPE ViewType)
{
	D3DXMATRIX Matrix;
	D3DXMATRIX TempMatrix;

	D3DXMatrixTranslation(&Matrix, ViewList[Index].Position.x, ViewList[Index].Position.y, ViewList[Index].Position.z);
	D3DXMatrixScaling(&TempMatrix, ViewList[Index].Scale, ViewList[Index].Scale, ViewList[Index].Scale);
	D3DXMatrixMultiply(&Matrix, &Matrix, &TempMatrix);
	Graphics.SetWorldMatrix(Matrix);

	switch(ViewList[Index].ViewType)
	{
		case VIEW_TYPE_NONE:
		{
			D3DXMatrixIdentity(&Matrix);
			break;
		}

		case VIEW_TYPE_XY_FRONT:
		{
			D3DXMatrixIdentity(&Matrix);
			break;
		}

		case VIEW_TYPE_XY_BACK:
		{
			D3DXMatrixRotationY(&Matrix, D3DX_PI);
			break;
		}

		case VIEW_TYPE_XZ_FRONT:
		{
			D3DXMatrixRotationX(&Matrix, (1.5f * D3DX_PI));
			break;
		}

		case VIEW_TYPE_XZ_BACK:
		{
			D3DXMatrixRotationX(&Matrix, (1.5f * D3DX_PI));
			D3DXMatrixRotationY(&TempMatrix, D3DX_PI);
			D3DXMatrixMultiply(&Matrix, &Matrix, &TempMatrix);
			break;
		}

		case VIEW_TYPE_YZ_FRONT:
		{
			D3DXMatrixRotationX(&Matrix, (1.5f * D3DX_PI));
			D3DXMatrixRotationY(&TempMatrix, (1.5f * D3DX_PI));
			D3DXMatrixMultiply(&Matrix, &Matrix, &TempMatrix);
			break;
		}

		case VIEW_TYPE_YZ_BACK:
		{
			D3DXMatrixRotationX(&Matrix, (1.5f * D3DX_PI));
			D3DXMatrixRotationY(&TempMatrix, (0.5f * D3DX_PI));
			D3DXMatrixMultiply(&Matrix, &Matrix, &TempMatrix);
			break;
		}
	}

	Graphics.SetViewMatrix(Matrix);

	D3DXMatrixOrthoRH(&Matrix, static_cast<FLOAT>(ViewList[Index].Width), static_cast<FLOAT>(ViewList[Index].Height), Properties().NearDistance, Properties().FarDistance);
	Graphics.SetProjectionMatrix(Matrix);
}


//+-----------------------------------------------------------------------------
//| Builds a projection plane from the view
//+-----------------------------------------------------------------------------
D3DXPLANE MODEL_WINDOW::BuildProjectionPlane(INT ViewIndex, CONST D3DXVECTOR3& Point)
{
	D3DXPLANE Plane;
	D3DXVECTOR3 PlaneNormal;

	switch(ViewList[ViewIndex].ViewType)
	{
		case VIEW_TYPE_XY_FRONT:
		{
			PlaneNormal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			break;
		}

		case VIEW_TYPE_XY_BACK:
		{
			PlaneNormal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			break;
		}

		case VIEW_TYPE_XZ_FRONT:
		{
			PlaneNormal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			break;
		}

		case VIEW_TYPE_XZ_BACK:
		{
			PlaneNormal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			break;
		}

		case VIEW_TYPE_YZ_FRONT:
		{
			PlaneNormal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			break;
		}

		case VIEW_TYPE_YZ_BACK:
		{
			PlaneNormal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			break;
		}

		default:
		{
			PlaneNormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			break;
		}
	}

	D3DXPlaneFromPointNormal(&Plane, &Point, &PlaneNormal);

	return Plane;
}


//+-----------------------------------------------------------------------------
//| Converts a screen position to a world position
//+-----------------------------------------------------------------------------
D3DXVECTOR3 MODEL_WINDOW::ScreenPositionToWorldPosition(INT ViewIndex, CONST D3DXPLANE& ProjectionPlane, INT X, INT Y)
{
	D3DVIEWPORT9 ViewPort;
	D3DXVECTOR3 TempVector;
	D3DXVECTOR3 TempVector1;
	D3DXVECTOR3 TempVector2;

	if(ViewIndex == INVALID_INDEX) return D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	BuildViewMatrices(ViewIndex, ViewList[ViewIndex].ViewType);

	ViewPort.X = 0;
	ViewPort.Y = 0;
	ViewPort.Width = ViewList[ViewIndex].GraphicsWindow.GetWidth();
	ViewPort.Height = ViewList[ViewIndex].GraphicsWindow.GetHeight();
	ViewPort.MinZ = 0.0f;
	ViewPort.MaxZ = 1.0f;

	D3DXVec3Unproject(&TempVector1, &D3DXVECTOR3(static_cast<FLOAT>(X), static_cast<FLOAT>(Y), 0.0f), &ViewPort, &Graphics.GetProjectionMatrix(), &Graphics.GetViewMatrix(), &Graphics.GetWorldMatrix());
	D3DXVec3Unproject(&TempVector2, &D3DXVECTOR3(static_cast<FLOAT>(X), static_cast<FLOAT>(Y), 1.0f), &ViewPort, &Graphics.GetProjectionMatrix(), &Graphics.GetViewMatrix(), &Graphics.GetWorldMatrix());
	D3DXPlaneIntersectLine(&TempVector, &ProjectionPlane, &TempVector1, &TempVector2);

	return TempVector;
}


//+-----------------------------------------------------------------------------
//| Creates a view
//+-----------------------------------------------------------------------------
HWND MODEL_WINDOW::CreateView()
{
	HWND View;
	DWORD Style;

	Style = WS_VISIBLE | WS_CHILD | WS_THICKFRAME;

	View = CreateWindowEx(0, "STATIC", "", Style, 0, 0, 1, 1, Window, NULL, GetModuleHandle(NULL), NULL);
	if(View == NULL)
	{
		Error.SetMessage("Unable to create a new view!");
		return FALSE;
	}

	return View;
}


//+-----------------------------------------------------------------------------
//| Resizes the views
//+-----------------------------------------------------------------------------
VOID MODEL_WINDOW::ResizeViews()
{
	INT i;
	INT Width;
	INT Height;
	INT HalfWidth;
	INT HalfHeight;

	Width = GetWidth();
	Height = GetHeight();
	HalfWidth = Width / 2;
	HalfHeight = Height / 2;

	ViewList[VIEW_INDEX_TOP_LEFT].X = 0;
	ViewList[VIEW_INDEX_TOP_LEFT].Y = 0;
	ViewList[VIEW_INDEX_TOP_LEFT].Width = HalfWidth;
	ViewList[VIEW_INDEX_TOP_LEFT].Height = HalfHeight;

	ViewList[VIEW_INDEX_TOP_RIGHT].X = HalfWidth;
	ViewList[VIEW_INDEX_TOP_RIGHT].Y = 0;
	ViewList[VIEW_INDEX_TOP_RIGHT].Width = (Width - HalfWidth);
	ViewList[VIEW_INDEX_TOP_RIGHT].Height = HalfHeight;

	ViewList[VIEW_INDEX_BOTTOM_LEFT].X = 0;
	ViewList[VIEW_INDEX_BOTTOM_LEFT].Y = HalfHeight;
	ViewList[VIEW_INDEX_BOTTOM_LEFT].Width = HalfWidth;
	ViewList[VIEW_INDEX_BOTTOM_LEFT].Height = (Height - HalfHeight);

	ViewList[VIEW_INDEX_BOTTOM_RIGHT].X = HalfWidth;
	ViewList[VIEW_INDEX_BOTTOM_RIGHT].Y = HalfHeight;
	ViewList[VIEW_INDEX_BOTTOM_RIGHT].Width = (Width - HalfWidth);
	ViewList[VIEW_INDEX_BOTTOM_RIGHT].Height = (Height - HalfHeight);

	for(i = 0; i < 4; i++)
	{
		::SetWindowPos(ViewList[i].Window, NULL, ViewList[i].X, ViewList[i].Y, ViewList[i].Width, ViewList[i].Height, SWP_NOACTIVATE | SWP_NOZORDER);
		ViewList[i].GraphicsWindow.Resize();
	}
}


//+-----------------------------------------------------------------------------
//| Selects a specific view
//+-----------------------------------------------------------------------------
VOID MODEL_WINDOW::SelectView(INT View)
{
	DWORD Style;

	if(CurrentView != INVALID_INDEX)
	{
		Style = GetWindowLong(ViewList[CurrentView].Window, GWL_STYLE);
		//Style &= ~WS_BORDER;
		SetWindowLong(ViewList[CurrentView].Window, GWL_STYLE, Style);
		CurrentView = INVALID_INDEX;
	}

	CurrentView = View;

	if(CurrentView != INVALID_INDEX)
	{
		Style = GetWindowLong(ViewList[CurrentView].Window, GWL_STYLE);
		//Style |= WS_BORDER;
		SetWindowLong(ViewList[CurrentView].Window, GWL_STYLE, Style);
	}

	InvalidateRect(Window, NULL, FALSE);
}


//+-----------------------------------------------------------------------------
//| Selects a specific view
//+-----------------------------------------------------------------------------
INT MODEL_WINDOW::GetView(INT X, INT Y)
{
	INT View;
	INT Width;
	INT Height;
	INT HalfWidth;
	INT HalfHeight;

	Width = GetWidth();
	Height = GetHeight();
	HalfWidth = Width / 2;
	HalfHeight = Height / 2;

	if(X < 1) return INVALID_INDEX;
	if(Y < 1) return INVALID_INDEX;
	if(X >= (Width - 1)) return INVALID_INDEX;
	if(Y >= (Height - 1)) return INVALID_INDEX;

	if(X < HalfWidth)
	{
		if(Y < HalfHeight)
		{
			View = VIEW_INDEX_TOP_LEFT;
		}
		else
		{
			View = VIEW_INDEX_BOTTOM_LEFT;
		}
	}
	else
	{
		if(Y < HalfHeight)
		{
			View = VIEW_INDEX_TOP_RIGHT;
		}
		else
		{
			View = VIEW_INDEX_BOTTOM_RIGHT;
		}
	}

	return View;
}


//+-----------------------------------------------------------------------------
//| Calculates the mass centre of all selected vertices
//+-----------------------------------------------------------------------------
D3DXVECTOR3 MODEL_WINDOW::CalculateMassCentre()
{
	INT i;
	INT Amount;
	FLOAT Scale;
	D3DXVECTOR3 Centre;
	MODEL_GEOSET* Geoset;
	MODEL_GEOSET_VERTEX* GeosetVertex;
	std::set<INT>::iterator j;

	Amount = 0;
	Centre = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for(i = 0; i < Model.Data().GeosetContainer.GetTotalSize(); i++)
	{
		if(Model.Data().GeosetContainer.ValidIndex(i))
		{
			Geoset = Model.Data().GeosetContainer[i];
			j = Geoset->SelectedVertices.begin();
			while(j != Geoset->SelectedVertices.end())
			{
				GeosetVertex = Geoset->Data().VertexContainer[*j];
				Centre += GeosetVertex->Position;
				Amount++;
				j++;
			}
		}
	}

	Scale = (Amount == 0) ? 1.0f : (1.0f / static_cast<FLOAT>(Amount));
	Centre.x *= Scale;
	Centre.y *= Scale;
	Centre.z *= Scale;

	return Centre;
}
