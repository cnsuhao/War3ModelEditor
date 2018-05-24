//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_PARTICLE_EMITTER_2_DIALOG_H
#define MAGOS_WINDOW_PARTICLE_EMITTER_2_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Particle emitter 2 dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_PARTICLE_EMITTER_2_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_PARTICLE_EMITTER_2_DIALOG();
		DESTRUCTOR ~WINDOW_PARTICLE_EMITTER_2_DIALOG();

		BOOL Display(HWND ParentWindow, MODEL_PARTICLE_EMITTER_2_DATA& Data, MODEL_TEXTURE*& Texture) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static VOID BuildTextureIdList(HWND Window);

		static MODEL_PARTICLE_EMITTER_2_DATA StaticData;
		static MODEL_TEXTURE* StaticTexture;
		static D3DCOLOR StaticColor1;
		static D3DCOLOR StaticColor2;
		static D3DCOLOR StaticColor3;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_PARTICLE_EMITTER_2_DIALOG ParticleEmitter2Dialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
