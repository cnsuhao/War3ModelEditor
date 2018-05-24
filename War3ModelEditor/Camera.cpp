//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Camera.h"
#include "Graphics.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
CAMERA Camera;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
CAMERA::CAMERA()
{
	SetDefaultDefaults();

	ValidMouseMove = FALSE;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
CAMERA::~CAMERA()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Resets the camera
//+-----------------------------------------------------------------------------
VOID CAMERA::Reset(HWND Window)
{
	Source = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Target = DefaultTarget;
	Up = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	Pitch = DefaultPitch;
	Yaw = DefaultYaw;
	Distance = DefaultDistance;

	UpdateSourcePosition();
	Graphics.SetCamera(*this);
}


//+-----------------------------------------------------------------------------
//| Updates the camera vectors to fit its state
//+-----------------------------------------------------------------------------
VOID CAMERA::Update(HWND Window)
{
	BOOL AltDown;
	POINT MousePosition;

	if(Keyboard.KeyDown(KEY_LEFTCTRL)) return;

	AltDown = Keyboard.KeyDown(KEY_LEFTALT);

	if(Mouse.ButtonPressed(BUTTON_LEFT) || Mouse.ButtonPressed(BUTTON_RIGHT))
	{
		GetCursorPos(&MousePosition);
		ScreenToClient(Window, &MousePosition);

		ValidMouseMove = (MousePosition.y >= 0);
	}

	if(AltDown)
	{
		if(ValidMouseMove && Mouse.ButtonDown(BUTTON_LEFT))
		{
			Distance += Mouse.GetDY() * 10.0f * CAMERA_FACTOR_DISTANCE * (Properties().RelativeZoom ? Distance : CAMERA_FACTOR_RELATIVE_DISTANCE);
		}
	}
	else
	{
		if(ValidMouseMove && Mouse.ButtonDown(BUTTON_LEFT))
		{
			Pitch += Mouse.GetDY() * CAMERA_FACTOR_PITCH;
			Yaw -= Mouse.GetDX() * CAMERA_FACTOR_YAW;
		}
	}

	if(Pitch < CAMERA_MIN_PITCH) Pitch = CAMERA_MIN_PITCH;
	if(Pitch > CAMERA_MAX_PITCH) Pitch = CAMERA_MAX_PITCH;

	while(Yaw < 0.0) Yaw += (D3DX_PI * 2);
	while(Yaw >= (D3DX_PI * 2)) Yaw -= (D3DX_PI * 2);

	Distance -= Mouse.GetDZ() * CAMERA_FACTOR_DISTANCE * (Properties().RelativeZoom ? Distance : CAMERA_FACTOR_RELATIVE_DISTANCE);

	if(Distance < CAMERA_MIN_DISTANCE) Distance = CAMERA_MIN_DISTANCE;
	if(Distance > CAMERA_MAX_DISTANCE) Distance = CAMERA_MAX_DISTANCE;

	if(ValidMouseMove && Mouse.ButtonDown(BUTTON_RIGHT))
	{
		FLOAT X_X = Mouse.GetDX() * std::sin(Yaw);
		FLOAT X_Y = -Mouse.GetDY() * std::sin(Pitch) * std::cos(Yaw);
		FLOAT Y_X = -Mouse.GetDX() * std::cos(Yaw);
		FLOAT Y_Y = -Mouse.GetDY() * std::sin(Pitch) * std::sin(Yaw);
		FLOAT Z_X = 0.0;
		FLOAT Z_Y = Mouse.GetDY() * std::cos(Pitch);
		FLOAT Factor;

		Factor = CAMERA_FACTOR_MOVE * (Properties().RelativeMove ? Distance : CAMERA_FACTOR_RELATIVE_MOVE);

		Target.x += Factor * (X_X + X_Y);
		Target.y += Factor * (Y_X + Y_Y);
		Target.z += Factor * (Z_X + Z_Y);
	}

	UpdateSourcePosition();
}


//+-----------------------------------------------------------------------------
//| Forces the camera to a new position
//+-----------------------------------------------------------------------------
VOID CAMERA::SetPosition(CONST D3DXVECTOR3& SourcePosition, CONST D3DXVECTOR3& TargetPosition)
{
	D3DXVECTOR3 CameraView;

	Target = TargetPosition;
	D3DXVec3Subtract(&CameraView, &TargetPosition, &SourcePosition);

	Distance = D3DXVec3Length(&CameraView);
	Pitch = (Distance == 0.0f) ? 0.0f : std::asin(-CameraView.z / Distance);
	Yaw = std::atan2((SourcePosition.y - TargetPosition.y), (SourcePosition.x - TargetPosition.x));

	if(Distance < CAMERA_MIN_DISTANCE) Distance = CAMERA_MIN_DISTANCE;
	if(Distance > CAMERA_MAX_DISTANCE) Distance = CAMERA_MAX_DISTANCE;
}


//+-----------------------------------------------------------------------------
//| Returns the pitch
//+-----------------------------------------------------------------------------
FLOAT CAMERA::GetPitch()
{
	return Pitch;
}


//+-----------------------------------------------------------------------------
//| Returns the yaw
//+-----------------------------------------------------------------------------
FLOAT CAMERA::GetYaw()
{
	return Yaw;
}


//+-----------------------------------------------------------------------------
//| Returns the distance
//+-----------------------------------------------------------------------------
FLOAT CAMERA::GetDistance()
{
	return Distance;
}


//+-----------------------------------------------------------------------------
//| Sets a new pitch
//+-----------------------------------------------------------------------------
VOID CAMERA::SetPitch(FLOAT NewPitch)
{
	Pitch = NewPitch;

	UpdateSourcePosition();
}


//+-----------------------------------------------------------------------------
//| Sets a new yaw
//+-----------------------------------------------------------------------------
VOID CAMERA::SetYaw(FLOAT NewYaw)
{
	Yaw = NewYaw;

	UpdateSourcePosition();
}


//+-----------------------------------------------------------------------------
//| Sets a new distance
//+-----------------------------------------------------------------------------
VOID CAMERA::SetDistance(FLOAT NewDistance)
{
	Distance = NewDistance;

	UpdateSourcePosition();
}


//+-----------------------------------------------------------------------------
//| Returns the source vector
//+-----------------------------------------------------------------------------
D3DXVECTOR3 CAMERA::GetSource() CONST
{
	return Source;
}


//+-----------------------------------------------------------------------------
//| Returns the target vector
//+-----------------------------------------------------------------------------
D3DXVECTOR3 CAMERA::GetTarget() CONST
{
	return Target;
}


//+-----------------------------------------------------------------------------
//| Returns the up vector
//+-----------------------------------------------------------------------------
D3DXVECTOR3 CAMERA::GetUp() CONST
{
	return Up;
}


//+-----------------------------------------------------------------------------
//| Resets all default values
//+-----------------------------------------------------------------------------
VOID CAMERA::SetDefaultDefaults()
{
	DefaultTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	DefaultPitch = CAMERA_DEFAULT_PITCH;
	DefaultYaw = CAMERA_DEFAULT_YAW;
	DefaultDistance = CAMERA_DEFAULT_DISTANCE;
}


//+-----------------------------------------------------------------------------
//| Sets a new default target
//+-----------------------------------------------------------------------------
VOID CAMERA::SetDefaultTarget(CONST D3DXVECTOR3& NewDefaultTarget)
{
	DefaultTarget = NewDefaultTarget;
}


//+-----------------------------------------------------------------------------
//| Sets a new default pitch
//+-----------------------------------------------------------------------------
VOID CAMERA::SetDefaultPitch(FLOAT NewDefaultPitch)
{
	DefaultPitch = NewDefaultPitch;

	if(DefaultPitch < CAMERA_MIN_PITCH) DefaultPitch = CAMERA_MIN_PITCH;
	if(DefaultPitch > CAMERA_MAX_PITCH) DefaultPitch = CAMERA_MAX_PITCH;
}


//+-----------------------------------------------------------------------------
//| Sets a new default yaw
//+-----------------------------------------------------------------------------
VOID CAMERA::SetDefaultYaw(FLOAT NewDefaultYaw)
{
	DefaultYaw = NewDefaultYaw;
}


//+-----------------------------------------------------------------------------
//| Sets a new default distance
//+-----------------------------------------------------------------------------
VOID CAMERA::SetDefaultDistance(FLOAT NewDefaultDistance)
{
	DefaultDistance = NewDefaultDistance;

	if(DefaultDistance < CAMERA_DEFAULT_MIN_DISTANCE) DefaultDistance = CAMERA_DEFAULT_MIN_DISTANCE;
	if(DefaultDistance > CAMERA_MAX_DISTANCE) DefaultDistance = CAMERA_MAX_DISTANCE;
}


//+-----------------------------------------------------------------------------
//| Disables the valid mouse movement
//+-----------------------------------------------------------------------------
VOID CAMERA::DisableMouseMove()
{
	ValidMouseMove = FALSE;
}


//+-----------------------------------------------------------------------------
//| Updates the camera source position
//+-----------------------------------------------------------------------------
VOID CAMERA::UpdateSourcePosition()
{
	Source.x = Target.x + Distance * std::cos(Pitch) * std::cos(Yaw);
	Source.y = Target.y + Distance * std::cos(Pitch) * std::sin(Yaw);
	Source.z = Target.z + Distance * std::sin(Pitch);
}
