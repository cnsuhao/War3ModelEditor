//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowCollisionShapeDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_COLLISION_SHAPE_DIALOG CollisionShapeDialog;


//+-----------------------------------------------------------------------------
//| Static member variables
//+-----------------------------------------------------------------------------
MODEL_COLLISION_SHAPE_DATA WINDOW_COLLISION_SHAPE_DIALOG::StaticData;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_COLLISION_SHAPE_DIALOG::WINDOW_COLLISION_SHAPE_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_COLLISION_SHAPE_DIALOG::~WINDOW_COLLISION_SHAPE_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
BOOL WINDOW_COLLISION_SHAPE_DIALOG::Display(HWND ParentWindow, MODEL_COLLISION_SHAPE_DATA& Data) CONST
{
	StaticData = Data;
	if(DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogCollisionShape), ParentWindow, DialogMessageHandler, 0))
	{
		Data = StaticData;
		StaticData = MODEL_COLLISION_SHAPE_DATA();
		return TRUE;
	}

	StaticData = MODEL_COLLISION_SHAPE_DATA();
	return FALSE;
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_COLLISION_SHAPE_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			D3DXVECTOR3 Vector1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 Vector2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			std::list<D3DXVECTOR3>::iterator i;

			AddWindow(Window);
			CenterWindow(Window);

			if(StaticData.Type == COLLISION_SHAPE_TYPE_SPHERE)
			{
				CheckDlgButton(Window, DialogCollisionShapeButtonSphere, TRUE);
			}
			else
			{
				CheckDlgButton(Window, DialogCollisionShapeButtonBox, TRUE);
			}

			i = StaticData.VertexList.begin();
			if(i != StaticData.VertexList.end())
			{
				Vector1 = (*i);

				i++;
				if(i != StaticData.VertexList.end())
				{
					Vector2 = (*i);
				}
			}

			SetFloat(GetDlgItem(Window, DialogCollisionShapeEditBoundsRadius), StaticData.BoundsRadius);
			SetFloat(GetDlgItem(Window, DialogCollisionShapeEditX1), Vector1.x);
			SetFloat(GetDlgItem(Window, DialogCollisionShapeEditY1), Vector1.y);
			SetFloat(GetDlgItem(Window, DialogCollisionShapeEditZ1), Vector1.z);
			SetFloat(GetDlgItem(Window, DialogCollisionShapeEditX2), Vector2.x);
			SetFloat(GetDlgItem(Window, DialogCollisionShapeEditY2), Vector2.y);
			SetFloat(GetDlgItem(Window, DialogCollisionShapeEditZ2), Vector2.z);

			EnableWindow(GetDlgItem(Window, DialogCollisionShapeEditBoundsRadius), (StaticData.Type == COLLISION_SHAPE_TYPE_SPHERE));
			EnableWindow(GetDlgItem(Window, DialogCollisionShapeEditX2), (StaticData.Type != COLLISION_SHAPE_TYPE_SPHERE));
			EnableWindow(GetDlgItem(Window, DialogCollisionShapeEditY2), (StaticData.Type != COLLISION_SHAPE_TYPE_SPHERE));
			EnableWindow(GetDlgItem(Window, DialogCollisionShapeEditZ2), (StaticData.Type != COLLISION_SHAPE_TYPE_SPHERE));

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogCollisionShapeButtonBox:
				{
					EnableWindow(GetDlgItem(Window, DialogCollisionShapeEditBoundsRadius), FALSE);
					EnableWindow(GetDlgItem(Window, DialogCollisionShapeEditX2), TRUE);
					EnableWindow(GetDlgItem(Window, DialogCollisionShapeEditY2), TRUE);
					EnableWindow(GetDlgItem(Window, DialogCollisionShapeEditZ2), TRUE);

					return TRUE;
				}

				case DialogCollisionShapeButtonSphere:
				{
					EnableWindow(GetDlgItem(Window, DialogCollisionShapeEditBoundsRadius), TRUE);
					EnableWindow(GetDlgItem(Window, DialogCollisionShapeEditX2), FALSE);
					EnableWindow(GetDlgItem(Window, DialogCollisionShapeEditY2), FALSE);
					EnableWindow(GetDlgItem(Window, DialogCollisionShapeEditZ2), FALSE);

					return TRUE;
				}

				case DialogCollisionShapeButtonOk:
				{
					D3DXVECTOR3 Vector;

					StaticData.Type = CheckStateToBool(IsDlgButtonChecked(Window, DialogCollisionShapeButtonBox)) ? COLLISION_SHAPE_TYPE_BOX : COLLISION_SHAPE_TYPE_SPHERE;

					StaticData.BoundsRadius = GetFloat(GetDlgItem(Window, DialogCollisionShapeEditBoundsRadius));

					Vector.x = GetFloat(GetDlgItem(Window, DialogCollisionShapeEditX1));
					Vector.y = GetFloat(GetDlgItem(Window, DialogCollisionShapeEditY1));
					Vector.z = GetFloat(GetDlgItem(Window, DialogCollisionShapeEditZ1));

					StaticData.VertexList.clear();
					StaticData.VertexList.push_back(Vector);

					if(StaticData.Type == COLLISION_SHAPE_TYPE_BOX)
					{
						Vector.x = GetFloat(GetDlgItem(Window, DialogCollisionShapeEditX2));
						Vector.y = GetFloat(GetDlgItem(Window, DialogCollisionShapeEditY2));
						Vector.z = GetFloat(GetDlgItem(Window, DialogCollisionShapeEditZ2));

						StaticData.VertexList.push_back(Vector);
					}

					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}

				case DialogCollisionShapeButtonCancel:
				{
					RemoveWindow(Window);
					EndDialog(Window, 0);
					return TRUE;
				}
			}

			return FALSE;
		}

		case WM_CLOSE:
		{
			::SendMessage(Window, WM_COMMAND, DialogCollisionShapeButtonCancel, 0);
			return TRUE;
		}
	}

	return FALSE;
}
