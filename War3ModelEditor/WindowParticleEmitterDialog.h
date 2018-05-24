//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WINDOW_PARTICLE_EMITTER_DIALOG_H
#define MAGOS_WINDOW_PARTICLE_EMITTER_DIALOG_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "WindowDialog.h"
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Particle emitter dialog window class
//+-----------------------------------------------------------------------------
class WINDOW_PARTICLE_EMITTER_DIALOG : public WINDOW_DIALOG
{
	public:
		CONSTRUCTOR WINDOW_PARTICLE_EMITTER_DIALOG();
		DESTRUCTOR ~WINDOW_PARTICLE_EMITTER_DIALOG();

		BOOL Display(HWND ParentWindow, MODEL_PARTICLE_EMITTER_DATA& Data) CONST;

	protected:
		static BOOL CALLBACK DialogMessageHandler(HWND Window, UINT Message, WPARAM W, LPARAM L);

		static MODEL_PARTICLE_EMITTER_DATA StaticData;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern WINDOW_PARTICLE_EMITTER_DIALOG ParticleEmitterDialog;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
