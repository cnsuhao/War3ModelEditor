//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "WindowModelInfoDialog.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
WINDOW_MODEL_INFO_DIALOG ModelInfoDialog;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
WINDOW_MODEL_INFO_DIALOG::WINDOW_MODEL_INFO_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
WINDOW_MODEL_INFO_DIALOG::~WINDOW_MODEL_INFO_DIALOG()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Displays the dialog
//+-----------------------------------------------------------------------------
VOID WINDOW_MODEL_INFO_DIALOG::Display(HWND ParentWindow) CONST
{
	DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(DialogModelInfo), ParentWindow, DialogMessageHandler, 0);
}


//+-----------------------------------------------------------------------------
//| Handles the dialog messages
//+-----------------------------------------------------------------------------
BOOL CALLBACK WINDOW_MODEL_INFO_DIALOG::DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L)
{
	switch(Message)
	{
		case WM_INITDIALOG:
		{
			INT i;
			FLOAT Temp;
			INT NrOfVertices = 0;
			INT NrOfFaces = 0;
			INT NrOfGroups = 0;
			INT NrOfTracks = 0;
			FLOAT TotalAnimationLength = 0.0f;
			MODEL_GEOSET* Geoset;
			MODEL_SEQUENCE* Sequence;
			REFERENCE<INTERPOLATOR*, INTERPOLATOR*>* CurrentReference;

			AddWindow(Window);
			CenterWindow(Window);

			for(i = 0; i < Model.Data().GeosetContainer.GetTotalSize(); i++)
			{
				if(Model.Data().GeosetContainer.ValidIndex(i))
				{
					Geoset = Model.Data().GeosetContainer[i];

					NrOfVertices += Geoset->Data().VertexContainer.GetSize();
					NrOfFaces += Geoset->Data().FaceContainer.GetSize();
					NrOfGroups += Geoset->Data().GroupContainer.GetSize();
				}
			}

			for(i = 0; i < Model.Data().SequenceContainer.GetTotalSize(); i++)
			{
				if(Model.Data().SequenceContainer.ValidIndex(i))
				{
					Sequence = Model.Data().SequenceContainer[i];

					Temp = Sequence->Data().Interval.y - Sequence->Data().Interval.x;
					if(Temp < 0.0f) Temp = 0.0f;

					TotalAnimationLength += Temp;
				}
			}

			TotalAnimationLength /= 1000.0f;

			CurrentReference = INTERPOLATOR::InterpolatorNodes.GetFirstReference();
			while(CurrentReference != NULL)
			{
				NrOfTracks += CurrentReference->GetData()->GetNrOfNodes();
				CurrentReference = INTERPOLATOR::InterpolatorNodes.GetNextReference(CurrentReference);
			}

			SetInt(GetDlgItem(Window, DialogModelInfoTextGeoset), Model.Data().GeosetContainer.GetSize());
			SetInt(GetDlgItem(Window, DialogModelInfoTextGeosetAnimation), Model.Data().GeosetAnimationContainer.GetSize());
			SetInt(GetDlgItem(Window, DialogModelInfoTextTexture), Model.Data().TextureContainer.GetSize());
			SetInt(GetDlgItem(Window, DialogModelInfoTextTextureAnimation), Model.Data().TextureAnimationContainer.GetSize());
			SetInt(GetDlgItem(Window, DialogModelInfoTextMaterial), Model.Data().MaterialContainer.GetSize());
			SetInt(GetDlgItem(Window, DialogModelInfoTextSequence), Model.Data().SequenceContainer.GetSize());
			SetInt(GetDlgItem(Window, DialogModelInfoTextGlobalSequence), Model.Data().GlobalSequenceContainer.GetSize());
			SetInt(GetDlgItem(Window, DialogModelInfoTextCamera), Model.Data().CameraContainer.GetSize());
			SetInt(GetDlgItem(Window, DialogModelInfoTextAttachment), Model.Data().AttachmentContainer.GetSize());
			SetInt(GetDlgItem(Window, DialogModelInfoTextBone), Model.Data().BoneContainer.GetSize());
			SetInt(GetDlgItem(Window, DialogModelInfoTextCollisionShape), Model.Data().CollisionShapeContainer.GetSize());
			SetInt(GetDlgItem(Window, DialogModelInfoTextEventObject), Model.Data().EventObjectContainer.GetSize());
			SetInt(GetDlgItem(Window, DialogModelInfoTextHelper), Model.Data().HelperContainer.GetSize());
			SetInt(GetDlgItem(Window, DialogModelInfoTextLight), Model.Data().LightContainer.GetSize());
			SetInt(GetDlgItem(Window, DialogModelInfoTextParticleEmitter), Model.Data().ParticleEmitterContainer.GetSize());
			SetInt(GetDlgItem(Window, DialogModelInfoTextParticleEmitter2), Model.Data().ParticleEmitter2Container.GetSize());
			SetInt(GetDlgItem(Window, DialogModelInfoTextRibbonEmitter), Model.Data().RibbonEmitterContainer.GetSize());
			SetInt(GetDlgItem(Window, DialogModelInfoTextVertex), NrOfVertices);
			SetInt(GetDlgItem(Window, DialogModelInfoTextFace), NrOfFaces);
			SetInt(GetDlgItem(Window, DialogModelInfoTextGroup), NrOfGroups);
			SetInt(GetDlgItem(Window, DialogModelInfoTextTrack), NrOfTracks);
			SetFloat(GetDlgItem(Window, DialogModelInfoTextAnimationLength), TotalAnimationLength);

			return TRUE;
		}

		case WM_COMMAND:
		{
			switch(LOWORD(W))
			{
				case DialogModelInfoButtonOk:
				{
					RemoveWindow(Window);
					EndDialog(Window, 1);
					return TRUE;
				}
			}

			return FALSE;
		}

		case WM_CLOSE:
		{
			::SendMessage(Window, WM_COMMAND, DialogModelInfoButtonOk, 0);
			return TRUE;
		}
	}

	return FALSE;
}
