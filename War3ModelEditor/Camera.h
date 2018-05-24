//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_CAMERA_H
#define MAGOS_CAMERA_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Properties.h"
#include "Keyboard.h"
#include "Mouse.h"


//+-----------------------------------------------------------------------------
//| Camera class
//+-----------------------------------------------------------------------------
class CAMERA
{
	public:
		CONSTRUCTOR CAMERA();
		DESTRUCTOR ~CAMERA();

		VOID Reset(HWND Window);
		VOID Update(HWND Window);

		VOID SetPosition(CONST D3DXVECTOR3& SourcePosition, CONST D3DXVECTOR3& TargetPosition);

		FLOAT GetPitch();
		FLOAT GetYaw();
		FLOAT GetDistance();

		VOID SetPitch(FLOAT NewPitch);
		VOID SetYaw(FLOAT NewYaw);
		VOID SetDistance(FLOAT NewDistance);

		D3DXVECTOR3 GetSource() CONST;
		D3DXVECTOR3 GetTarget() CONST;
		D3DXVECTOR3 GetUp() CONST;

		VOID SetDefaultDefaults();
		VOID SetDefaultTarget(CONST D3DXVECTOR3& NewDefaultTarget);
		VOID SetDefaultPitch(FLOAT NewDefaultPitch);
		VOID SetDefaultYaw(FLOAT NewDefaultYaw);
		VOID SetDefaultDistance(FLOAT NewDefaultDistance);

		VOID DisableMouseMove();

	protected:
		VOID UpdateSourcePosition();

		D3DXVECTOR3 DefaultTarget;
		FLOAT DefaultPitch;
		FLOAT DefaultYaw;
		FLOAT DefaultDistance;

		D3DXVECTOR3 Source;
		D3DXVECTOR3 Target;
		D3DXVECTOR3 Up;

		FLOAT Pitch;
		FLOAT Yaw;
		FLOAT Distance;

		BOOL ValidMouseMove;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern CAMERA Camera;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
