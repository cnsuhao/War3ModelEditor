//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowMaterialDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_MATERIAL_DIALOG MaterialDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
MODEL_MATERIAL_DATA WINDOW_MATERIAL_DIALOG::StaticData;
std::list<MODEL_MATERIAL_LAYER*> WINDOW_MATERIAL_DIALOG::StaticLayerList;
std::list<MODEL_MATERIAL_LAYER*> WINDOW_MATERIAL_DIALOG::StaticAddedLayerList;
std::list<MODEL_MATERIAL_LAYER*> WINDOW_MATERIAL_DIALOG::StaticRemovedLayerList;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_MATERIAL_DIALOG::WINDOW_MATERIAL_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_MATERIAL_DIALOG::~WINDOW_MATERIAL_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_MATERIAL_DIALOG::Display(HWND ParentWindow, MODEL_MATERIAL_DATA& Data) CONST
{
	StaticData = Data;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogMaterial), ParentWindow, DialogMessageHandler, 0))
	{
		Data = StaticData;
		StaticData = MODEL_MATERIAL_DATA();
		return TRUE;
	}

	StaticData = MODEL_MATERIAL_DATA();
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_MATERIAL_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			INT i;
			INT Index;
			HWND ListBox;
			MODEL_MATERIAL_LAYER* MaterialLayer;

			AddWindow(Window);
			CenterWindow(Window);

			SetInt(GetDlgItem(Window, DialogMaterialEditPriorityPlane), StaticData.PriorityPlane);
			CheckDlgButton(Window, DialogMaterialButtonConstantColor, BoolToCheckState(StaticData.ConstantColor));
			CheckDlgButton(Window, DialogMaterialButtonSortPrimitivesFarZ, BoolToCheckState(StaticData.SortPrimitivesFarZ));
			CheckDlgButton(Window, DialogMaterialButtonFullResolution, BoolToCheckState(StaticData.FullResolution));

			ListBox = GetDlgItem(Window, DialogMaterialList);

			StaticLayerList.clear();
			StaticAddedLayerList.clear();
			StaticRemovedLayerList.clear();

			for(i = 0; i < StaticData.LayerContainer.GetTotalSize(); i++)
			{
				if(StaticData.LayerContainer.ValidIndex(i))
				{
					std::stringstream Stream;

					MaterialLayer = StaticData.LayerContainer[i];

					MaterialLayer->NewData() = MaterialLayer->Data();
					MaterialLayer->NewTextureNode = MaterialLayer->TextureNode;
					MaterialLayer->NewTextureAnimationNode = MaterialLayer->TextureAnimationNode;
					MaterialLayer->MarkAsUpdated();

					Stream << "MaterialLayer " << MaterialLayer;
					StaticLayerList.push_back(MaterialLayer);
					Index = static_cast<INT>(::SendMessage(ListBox, LB_INSERTSTRING, -1, reinterpret_cast<LPARAM>(Stream.str().c_str())));
					::SendMessage(ListBox, LB_SETITEMDATA, Index, reinterpret_cast<LPARAM>(MaterialLayer));
				}
			}

			return TRUE;
		}

		case WM_CONTEXTMENU:
		{
			INT Index;
			INT MenuId;
			INT NrOfItems;
			HWND ListBox;
			RECT TempRect;
			RECT ListRect;
			POINT MousePosition;
			POINT LocalMousePosition;

			ListBox = GetDlgItem(Window, DialogMaterialList);

			GetWindowRect(ListBox, &ListRect);
			GetCursorPos(&MousePosition);
			LocalMousePosition = MousePosition;
			ScreenToClient(ListBox, &LocalMousePosition);

			if(!Common.PointInsideRect(MousePosition, ListRect)) return TRUE;

			MenuId = LayerMenu2;
			NrOfItems = static_cast<INT>(::SendMessage(ListBox, LB_GETCOUNT, 0, 0));

			Index = 0;
			while(Index < NrOfItems)
			{
				::SendMessage(ListBox, LB_GETITEMRECT, Index, reinterpret_cast<LPARAM>(&TempRect));
				if(Common.PointInsideRect(LocalMousePosition, TempRect))
				{
					::SendMessage(ListBox, LB_SETCURSEL, Index, 0);
					MenuId = LayerMenu;
					break;
				}

				Index++;
			}

			TrackPopupMenuEx(GetSubMenu(LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(MenuId)), 0), TPM_LEFTALIGN | TPM_RIGHTBUTTON, MousePosition.x, MousePosition.y, Window, NULL);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogMaterialList:
				{
					switch(HIWORD(W))
					{
						case LBN_DBLCLK:
						{
							if(!Edit(Window))
							{
								Error.DisplayMessage(Window);
								Error.ClearMessage();
								return TRUE;
							}

							return TRUE;
						}
					}

					return TRUE;
				}

				case LayerFileCreateNew:
				case LayerFileCreateNew2:
				{
					if(!CreateNew(Window))
					{
						Error.DisplayMessage(Window);
						Error.ClearMessage();
						return TRUE;
					}

					return TRUE;
				}

				case LayerFileEdit:
				case LayerFileEdit2:
				{
					if(!Edit(Window))
					{
						Error.DisplayMessage(Window);
						Error.ClearMessage();
						return TRUE;
					}

					return TRUE;
				}

				case LayerFileRemove:
				case LayerFileRemove2:
				{
					if(!Remove(Window))
					{
						Error.DisplayMessage(Window);
						Error.ClearMessage();
						return TRUE;
					}

					return TRUE;
				}

				case LayerFileMoveUp:
				case LayerFileMoveUp2:
				{
					if(!MoveUp(Window))
					{
						Error.DisplayMessage(Window);
						Error.ClearMessage();
						return TRUE;
					}

					return TRUE;
				}

				case LayerFileMoveDown:
				case LayerFileMoveDown2:
				{
					if(!MoveDown(Window))
					{
						Error.DisplayMessage(Window);
						Error.ClearMessage();
						return TRUE;
					}

					return TRUE;
				}

				case DialogMaterialButtonOk:
				{
					std::list<MODEL_MATERIAL_LAYER*>::iterator i;

					StaticData.PriorityPlane = GetInt(GetDlgItem(Window, DialogMaterialEditPriorityPlane));
					StaticData.ConstantColor = CheckStateToBool(IsDlgButtonChecked(Window, DialogMaterialButtonConstantColor));
					StaticData.SortPrimitivesFarZ = CheckStateToBool(IsDlgButtonChecked(Window, DialogMaterialButtonSortPrimitivesFarZ));
					StaticData.FullResolution = CheckStateToBool(IsDlgButtonChecked(Window, DialogMaterialButtonFullResolution));

					StaticData.LayerContainer.Clear();

					i = StaticLayerList.begin();
					while(i != StaticLayerList.end())
					{
						(*i)->UpdateDataIfNeccessary();
						StaticData.LayerContainer.Add(*i);
						i++;
					}

					i = StaticRemovedLayerList.begin();
					while(i != StaticRemovedLayerList.end())
					{
						delete (*i);
						i++;
					}

					StaticLayerList.clear();
					StaticAddedLayerList.clear();
					StaticRemovedLayerList.clear();

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogMaterialButtonCancel:
				{
					std::list<MODEL_MATERIAL_LAYER*>::iterator i;

					i = StaticAddedLayerList.begin();
					while(i != StaticAddedLayerList.end())
					{
						delete (*i);
						i++;
					}

					StaticLayerList.clear();
					StaticAddedLayerList.clear();
					StaticRemovedLayerList.clear();

					RemoveWindow(Window);
					EndDialog(Window, 0);
					return TRUE;
				}
			}

			return FALSE;
		}

		case WM_CLOSE:
		{
			::SendMessage(Window, WM_COMMAND, DialogMaterialButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Creates a new layer
//+-----------------------------------------------------------------------------
BOOL WINDOW_MATERIAL_DIALOG::CreateNew(HWND Window)
{
	INT Index;
	HWND ListBox;
	MODEL_MATERIAL_LAYER* Layer;
	MODEL_TEXTURE* Texture;
	MODEL_TEXTURE_ANIMATION* TextureAnimation;
	std::stringstream Stream;

	ListBox = GetDlgItem(Window, DialogMaterialList);

	Layer = new MODEL_MATERIAL_LAYER();
	if(Layer == NULL)
	{
		Error.SetMessage("Unable to create a new material layer, memory allocation failed!");
		return FALSE;
	}

	Texture = NULL;
	TextureAnimation = NULL;

	if(!MaterialLayerDialog.Display(Window, Layer->NewData(), Texture, TextureAnimation))
	{
		delete Layer;
		return TRUE;
	}

	if(Texture != NULL)
	{
		Layer->NewTextureNode.Attach(Texture->MaterialLayerNodes);
	}
	else
	{
		Layer->NewTextureNode.Detach();
	}

	if(TextureAnimation != NULL)
	{
		Layer->NewTextureAnimationNode.Attach(TextureAnimation->MaterialLayerNodes);
	}
	else
	{
		Layer->NewTextureAnimationNode.Detach();
	}

	Layer->MarkAsUpdated();

	Stream << "MaterialLayer " << Layer;
	StaticLayerList.push_back(Layer);
	StaticAddedLayerList.push_back(Layer);
	Index = static_cast<INT>(::SendMessage(ListBox, LB_INSERTSTRING, -1, reinterpret_cast<LPARAM>(Stream.str().c_str())));
	::SendMessage(ListBox, LB_SETITEMDATA, Index, reinterpret_cast<LPARAM>(Layer));
	::SendMessage(ListBox, LB_SETCURSEL, Index, 0);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Edits an existing layer
//+-----------------------------------------------------------------------------
BOOL WINDOW_MATERIAL_DIALOG::Edit(HWND Window)
{
	INT Index;
	HWND ListBox;
	MODEL_MATERIAL_LAYER* Layer;
	MODEL_TEXTURE* Texture;
	MODEL_TEXTURE_ANIMATION* TextureAnimation;

	ListBox = GetDlgItem(Window, DialogMaterialList);

	Index = static_cast<INT>(::SendMessage(ListBox, LB_GETCURSEL, 0, 0));
	if(Index == LB_ERR) return TRUE;

	Layer = reinterpret_cast<MODEL_MATERIAL_LAYER*>(::SendMessage(ListBox, LB_GETITEMDATA, Index, 0));
	if(Layer == NULL) return TRUE;

	Texture = Layer->NewTextureNode.IsAttached() ? Layer->NewTextureNode.GetObjectData() : NULL;
	TextureAnimation = Layer->NewTextureAnimationNode.IsAttached() ? Layer->NewTextureAnimationNode.GetObjectData() : NULL;

	if(!MaterialLayerDialog.Display(Window, Layer->NewData(), Texture, TextureAnimation)) return TRUE;

	if(Texture != NULL)
	{
		Layer->NewTextureNode.Attach(Texture->MaterialLayerNodes);
	}
	else
	{
		Layer->NewTextureNode.Detach();
	}

	if(TextureAnimation != NULL)
	{
		Layer->NewTextureAnimationNode.Attach(TextureAnimation->MaterialLayerNodes);
	}
	else
	{
		Layer->NewTextureAnimationNode.Detach();
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Removes an existing layer
//+-----------------------------------------------------------------------------
BOOL WINDOW_MATERIAL_DIALOG::Remove(HWND Window)
{
	INT Index;
	HWND ListBox;
	MODEL_MATERIAL_LAYER* Layer;
	std::list<MODEL_MATERIAL_LAYER*>::iterator i;

	ListBox = GetDlgItem(Window, DialogMaterialList);

	Index = static_cast<INT>(::SendMessage(ListBox, LB_GETCURSEL, 0, 0));
	if(Index == LB_ERR) return TRUE;

	Layer = reinterpret_cast<MODEL_MATERIAL_LAYER*>(::SendMessage(ListBox, LB_GETITEMDATA, Index, 0));
	if(Layer == NULL) return TRUE;

	i = std::find(StaticAddedLayerList.begin(), StaticAddedLayerList.end(), Layer);
	if(i != StaticAddedLayerList.end())
	{
		StaticAddedLayerList.erase(i);
	}
	else
	{
		StaticRemovedLayerList.push_back(Layer);
	}

	i = std::find(StaticLayerList.begin(), StaticLayerList.end(), Layer);
	StaticLayerList.erase(i);

	::SendMessage(ListBox, LB_DELETESTRING, Index, 0);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Moves an existing layer up in the list
//+-----------------------------------------------------------------------------
BOOL WINDOW_MATERIAL_DIALOG::MoveUp(HWND Window)
{
	INT Index;
	HWND ListBox;
	std::stringstream Stream;
	MODEL_MATERIAL_LAYER* Layer;
	std::list<MODEL_MATERIAL_LAYER*>::iterator i;
	std::list<MODEL_MATERIAL_LAYER*>::iterator j;

	ListBox = GetDlgItem(Window, DialogMaterialList);

	Index = static_cast<INT>(::SendMessage(ListBox, LB_GETCURSEL, 0, 0));
	if(Index == LB_ERR) return TRUE;

	if(Index <= 0)
	{
		Error.SetMessage("The material layer is already at the top of the list!");
		return FALSE;
	}

	Layer = reinterpret_cast<MODEL_MATERIAL_LAYER*>(::SendMessage(ListBox, LB_GETITEMDATA, Index, 0));
	if(Layer == NULL) return TRUE;

	i = std::find(StaticLayerList.begin(), StaticLayerList.end(), Layer);
	j = i;
	j--;

	StaticLayerList.insert(j, Layer);
	StaticLayerList.erase(i);

	Stream << "MaterialLayer " << Layer;

	::SendMessage(ListBox, LB_DELETESTRING, Index, 0);
	Index = static_cast<INT>(::SendMessage(ListBox, LB_INSERTSTRING, Index - 1, reinterpret_cast<LPARAM>(Stream.str().c_str())));
	::SendMessage(ListBox, LB_SETITEMDATA, Index, reinterpret_cast<LPARAM>(Layer));

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Moves an existing layer down in the list
//+-----------------------------------------------------------------------------
BOOL WINDOW_MATERIAL_DIALOG::MoveDown(HWND Window)
{
	INT Index;
	HWND ListBox;
	std::stringstream Stream;
	MODEL_MATERIAL_LAYER* Layer;
	std::list<MODEL_MATERIAL_LAYER*>::iterator i;
	std::list<MODEL_MATERIAL_LAYER*>::iterator j;

	ListBox = GetDlgItem(Window, DialogMaterialList);

	Index = static_cast<INT>(::SendMessage(ListBox, LB_GETCURSEL, 0, 0));
	if(Index == LB_ERR) return TRUE;

	if(Index >= static_cast<INT>(::SendMessage(ListBox, LB_GETCOUNT, 0, 0)) - 1)
	{
		Error.SetMessage("The material layer is already at the bottom of the list!");
		return FALSE;
	}

	Layer = reinterpret_cast<MODEL_MATERIAL_LAYER*>(::SendMessage(ListBox, LB_GETITEMDATA, Index, 0));
	if(Layer == NULL) return TRUE;

	i = std::find(StaticLayerList.begin(), StaticLayerList.end(), Layer);
	j = i;
	j++;
	j++;

	StaticLayerList.insert(j, Layer);
	StaticLayerList.erase(i);

	Stream << "MaterialLayer " << Layer;

	::SendMessage(ListBox, LB_DELETESTRING, Index, 0);
	Index = static_cast<INT>(::SendMessage(ListBox, LB_INSERTSTRING, Index + 1, reinterpret_cast<LPARAM>(Stream.str().c_str())));
	::SendMessage(ListBox, LB_SETITEMDATA, Index, reinterpret_cast<LPARAM>(Layer));

	return TRUE;
}
