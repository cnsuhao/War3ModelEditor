//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Graphics.h"
#include "MainWindow.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
GRAPHICS Graphics;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
GRAPHICS::GRAPHICS()
{
	CurrentGraphicsWindow = NULL;

	Direct3D = NULL;
	Direct3DDevice = NULL;
	LineVertexBuffer = NULL;
	ParticleVertexBuffer = NULL;
	GroundVertexBuffer = NULL;

	Material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Material.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Material.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	Material.Power = 8.0f;

	Light.Type = D3DLIGHT_DIRECTIONAL;
	Light.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	Light.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	Light.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	Light.Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Light.Direction = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Light.Range = 0.0f;
	Light.Falloff = 0.0f;
	Light.Attenuation0 = 0.0f;
	Light.Attenuation1 = 0.0f;
	Light.Attenuation2 = 0.0f;
	Light.Theta = 0.0f;
	Light.Phi = 0.0f;

	Sprite = NULL;

	D3DXMatrixIdentity(&ProjectionMatrix);
	D3DXMatrixIdentity(&ViewMatrix);
	D3DXMatrixIdentity(&WorldMatrix);
	D3DXMatrixIdentity(&BillboardMatrix);
	D3DXMatrixIdentity(&WorldViewProjectionMatrix);

	CameraPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	CameraDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	ScreenWidth = 0;
	ScreenHeight = 0;
	ScreenAspect = 0.0f;
	FieldOfView = GRAPHICS_DEFAULT_FIELD_OF_VIEW;
	NearDistance = GRAPHICS_DEFAULT_NEAR_DISTANCE;
	FarDistance = GRAPHICS_DEFAULT_FAR_DISTANCE;

	Initialized = FALSE;
	DeviceLost = FALSE;
	UseShading = TRUE;
	UseCulling = TRUE;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
GRAPHICS::~GRAPHICS()
{
	Shutdown();
}


//+-----------------------------------------------------------------------------
//| Returns the Direct3D device pointer
//+-----------------------------------------------------------------------------
LPDIRECT3DDEVICE9 GRAPHICS::GetDevice() CONST
{
	return Direct3DDevice;
}


//+-----------------------------------------------------------------------------
//| Returns the graphics info pointer
//+-----------------------------------------------------------------------------
GRAPHICS_INFO* GRAPHICS::GetGraphicsInfo()
{
	return &GraphicsInfo;
}


//+-----------------------------------------------------------------------------
//| Returns the texture format
//+-----------------------------------------------------------------------------
D3DFORMAT GRAPHICS::GetTextureFormat() CONST
{
	return GraphicsInfo.TextureFormat;
}


//+-----------------------------------------------------------------------------
//| Returns the billboard matrix
//+-----------------------------------------------------------------------------
CONST D3DXMATRIX& GRAPHICS::GetBillboardMatrix() CONST
{
	return BillboardMatrix;
}


//+-----------------------------------------------------------------------------
//| Sets up the shaders
//+-----------------------------------------------------------------------------
BOOL GRAPHICS::SetupShaders()
{
	if(!Properties.ShadersAvailable()) return TRUE;

	if(!VertexShader.Setup(PATH_VERTEX_SHADER)) return FALSE;;
	if(!PixelShaderShaded.Setup(PATH_PIXEL_SHADER_SHADED)) return FALSE;
	if(!PixelShaderUnshaded.Setup(PATH_PIXEL_SHADER_UNSHADED)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Shuts down the shaders
//+-----------------------------------------------------------------------------
VOID GRAPHICS::ShutdownShaders()
{
	PixelShaderUnshaded.Shutdown();
	PixelShaderShaded.Shutdown();
	VertexShader.Shutdown();
}


//+-----------------------------------------------------------------------------
//| Enables or disables shading
//+-----------------------------------------------------------------------------
VOID GRAPHICS::SetShading(BOOL Shading)
{
	UseShading = Shading;
}


//+-----------------------------------------------------------------------------
//| Enables or disables backface culling
//+-----------------------------------------------------------------------------
VOID GRAPHICS::SetCulling(BOOL Culling)
{
	UseCulling = Culling;
}


//+-----------------------------------------------------------------------------
//| Sets the world camera
//+-----------------------------------------------------------------------------
VOID GRAPHICS::SetCamera(CONST CAMERA& Camera)
{
	SetCamera(Camera.GetSource(), Camera.GetTarget(), Camera.GetUp());
}


//+-----------------------------------------------------------------------------
//| Sets the world camera
//+-----------------------------------------------------------------------------
VOID GRAPHICS::SetCamera(CONST D3DXVECTOR3& Source, CONST D3DXVECTOR3& Target, CONST D3DXVECTOR3& Up)
{
	D3DXMatrixLookAtRH(&ViewMatrix, &Source, &Target, &Up);
	Direct3DDevice->SetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixTranspose(&BillboardMatrix, &ViewMatrix);

	BillboardMatrix._14 = 0.0f;
	BillboardMatrix._24 = 0.0f;
	BillboardMatrix._34 = 0.0f;
	BillboardMatrix._41 = 0.0f;
	BillboardMatrix._42 = 0.0f;
	BillboardMatrix._43 = 0.0f;

	CameraPosition = Source;
	CameraDirection = Target - Source;
	D3DXVec3Normalize(&CameraDirection, &CameraDirection);

	Light.Direction = CameraDirection;
}


//+-----------------------------------------------------------------------------
//| Sets a new field of view
//+-----------------------------------------------------------------------------
VOID GRAPHICS::SetFieldOfView(FLOAT NewFieldOfView)
{
	FieldOfView = NewFieldOfView;
}


//+-----------------------------------------------------------------------------
//| Sets a new clipping plane
//+-----------------------------------------------------------------------------
VOID GRAPHICS::SetClippingPlane(FLOAT NewNearDistance, FLOAT NewFarDistance)
{
	NearDistance = NewNearDistance;
	FarDistance = NewFarDistance;
}


//+-----------------------------------------------------------------------------
//| Sets up the graphics
//+-----------------------------------------------------------------------------
BOOL GRAPHICS::Setup()
{
	Shutdown();

	if(!CreateDummyWindow()) return FALSE;
	if(!CreateDirect3D()) return FALSE;
	if(!CheckCapabilities()) return FALSE;
	if(!CreateDirect3DDevice()) return FALSE;
	if(!CreateObjects()) return FALSE;

	SetRenderStates();
	SetProjection();

	GraphicsWindowReferenceObject.SetData(this);

	Initialized = TRUE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Shuts down the graphics
//+-----------------------------------------------------------------------------
VOID GRAPHICS::Shutdown()
{
	OnShutdown();

	SAFE_RELEASE(Sprite);

	SAFE_RELEASE(GroundVertexBuffer);
	SAFE_RELEASE(ParticleVertexBuffer);
	SAFE_RELEASE(LineVertexBuffer);
	SAFE_RELEASE(Direct3DDevice);
	SAFE_RELEASE(Direct3D);
	SAFE_DESTROY(Window);

	D3DXMatrixIdentity(&ProjectionMatrix);
	D3DXMatrixIdentity(&ViewMatrix);
	D3DXMatrixIdentity(&WorldMatrix);
	D3DXMatrixIdentity(&BillboardMatrix);

	ScreenWidth = 0;
	ScreenHeight = 0;
	ScreenAspect = 0.0f;
	FieldOfView = GRAPHICS_DEFAULT_FIELD_OF_VIEW;
	NearDistance = GRAPHICS_DEFAULT_NEAR_DISTANCE;
	FarDistance = GRAPHICS_DEFAULT_FAR_DISTANCE;

	Initialized = FALSE;
	DeviceLost = FALSE;
}


//+-----------------------------------------------------------------------------
//| Checks for a lost state
//+-----------------------------------------------------------------------------
BOOL GRAPHICS::CheckLostState()
{
	HRESULT Result;
	GRAPHICS_INFO TempGraphicsInfo;

	if(FAILED(Result = Direct3DDevice->TestCooperativeLevel()))
	{
		if(!DeviceLost) OnLostDevice();

		DeviceLost = TRUE;

		if(Result == D3DERR_DEVICELOST)
		{
			Error.SetMessage("The Direct3D device was lost!");
			return FALSE;
		}

		TempGraphicsInfo = GraphicsInfo;
		if(FAILED(Direct3DDevice->Reset(&TempGraphicsInfo.DeviceInfo)))
		{
			Error.SetMessage("Unable to reset the Direct3D device!");
			return FALSE;
		}

		SetRenderStates();
		OnResetDevice();

		Error.ClearMessage();

		DeviceLost = FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Begins rendering
//+-----------------------------------------------------------------------------
BOOL GRAPHICS::BeginRender(GRAPHICS_WINDOW& GraphicsWindow)
{
	LPDIRECT3DSWAPCHAIN9 SwapChain;

	if(!Initialized)
	{
		Error.SetMessage("The Direct3D device is not initialized!");
		return FALSE;
	}

	if(!CheckLostState()) return FALSE;

	CurrentGraphicsWindow = &GraphicsWindow;

	SwapChain = CurrentGraphicsWindow->GetSwapChain();
	if(SwapChain == NULL)
	{
		Error.SetMessage("Unable to retrieve the swap chain from the graphics window!");
		return FALSE;
	}

	if(FAILED(Direct3DDevice->SetRenderTarget(0, CurrentGraphicsWindow->GetBackBuffer())))
	{
		Error.SetMessage("Unable to set the swap chain back buffer as a render target!");
		return FALSE;
	}

	if(FAILED(Direct3DDevice->SetDepthStencilSurface(CurrentGraphicsWindow->GetZBuffer())))
	{
		Error.SetMessage("XXXXXXXXXXXX!");
		return FALSE;
	}

	ScreenWidth = CurrentGraphicsWindow->GetWidth();
	ScreenHeight = CurrentGraphicsWindow->GetHeight();
	ScreenAspect = CurrentGraphicsWindow->GetScreenAspect();

	SetProjection();

	Direct3DDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

	if(FAILED(Direct3DDevice->BeginScene()))
	{
		Error.SetMessage("Unable to begin a new scene!");
		return FALSE;
	}

	SetShader();
	SetShaderConstants();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Ends rendering
//+-----------------------------------------------------------------------------
VOID GRAPHICS::EndRender()
{
	Direct3DDevice->EndScene();

	CurrentGraphicsWindow->GetSwapChain()->Present(NULL, NULL, CurrentGraphicsWindow->GetWindow(), NULL, 0);
	CurrentGraphicsWindow = NULL;

	ScreenWidth = 0;
	ScreenHeight = 0;
	ScreenAspect = 0.0f;
}


//+-----------------------------------------------------------------------------
//| Renders some debug information
//+-----------------------------------------------------------------------------
VOID GRAPHICS::RenderDebug()
{
	D3DXVECTOR3 Position;
	std::stringstream Stream;

	if(!Properties().ShowDebug) return;

	Position = Camera.GetTarget();

	Stream << "Width:     " << MainWindow.GetWidth() << "\n";
	Stream << "Height:    " << MainWindow.GetHeight() << "\n";
	Stream << "Pitch:     " << Camera.GetPitch() << "\n";
	Stream << "Yaw:       " << Camera.GetYaw() << "\n";
	Stream << "Distance:  " << Camera.GetDistance() << "\n";
	Stream << "X:         " << Position.x << "\n";
	Stream << "Y:         " << Position.y << "\n";
	Stream << "Z:         " << Position.z << "\n";
	Stream << "Particles: " << ParticleManager.GetNrOfParticles() << "\n";

	RenderText(Stream.str(), NULL, GRAPHICS_DEBUG_COLOR);
}


//+-----------------------------------------------------------------------------
//| Renders the coordinate system axises
//+-----------------------------------------------------------------------------
VOID GRAPHICS::RenderAxises()
{
	BOOL XBeforeY;
	BOOL ZBeforeXY;
	FLOAT CameraPitch;
	FLOAT CameraYaw;
	FLOAT CameraDistance;
	D3DXVECTOR3 CameraPosition;
	D3DXVECTOR3 SourceVector;
	D3DXVECTOR3 TargetVectorX;
	D3DXVECTOR3 TargetVectorY;
	D3DXVECTOR3 TargetVectorZ;
	D3DXVECTOR3 ScreenVectorX;
	D3DXVECTOR3 ScreenVectorY;
	D3DXVECTOR3 ScreenVectorZ;
	D3DXVECTOR3 ScreenOffset;
	D3DXVECTOR3 FontOffset;
	D3DXVECTOR4 TempVector;
	D3DXMATRIX TempMatrix;
	D3DXMATRIX TempViewMatrix;
	D3DXMATRIX IdentityMatrix;
	D3DVIEWPORT9 ViewPort;

	if(!Properties().ShowAxises) return;

	D3DXMatrixIdentity(&WorldMatrix);
	D3DXMatrixIdentity(&IdentityMatrix);
	Direct3DDevice->GetViewport(&ViewPort);

	ScreenOffset = D3DXVECTOR3(-0.76f, -0.67f, 0.0f);
	FontOffset = D3DXVECTOR3(-5.0f, -5.0f, 0.0f);
	FontOffset.x += (ScreenOffset.x * ViewPort.Width / 2.0f);
	FontOffset.y -= (ScreenOffset.y * ViewPort.Height / 2.0f);

	CameraPitch = Camera.GetPitch();
	CameraYaw = Camera.GetYaw();
	CameraDistance = 10.0f;

	CameraPosition.x = CameraDistance * std::cos(CameraPitch) * std::cos(CameraYaw);
	CameraPosition.y = CameraDistance * std::cos(CameraPitch) * std::sin(CameraYaw);
	CameraPosition.z = CameraDistance * std::sin(CameraPitch);

	TempViewMatrix = ViewMatrix;
	SetCamera(CameraPosition, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	D3DXMatrixMultiply(&TempMatrix, &ViewMatrix, &ProjectionMatrix);

	D3DXVec3Transform(&TempVector, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &TempMatrix);
	SourceVector.x = ScreenOffset.x + TempVector.x;
	SourceVector.y = ScreenOffset.y + TempVector.y;
	SourceVector.z = 0.0f;

	D3DXVec3Transform(&TempVector, &D3DXVECTOR3(0.1f, 0.0f, 0.0f), &TempMatrix);
	TargetVectorX.x = ScreenOffset.x + TempVector.x;
	TargetVectorX.y = ScreenOffset.y + TempVector.y;
	TargetVectorX.z = 0.0f;

	D3DXVec3Transform(&TempVector, &D3DXVECTOR3(0.0f, 0.1f, 0.0f), &TempMatrix);
	TargetVectorY.x = ScreenOffset.x + TempVector.x;
	TargetVectorY.y = ScreenOffset.y + TempVector.y;
	TargetVectorY.z = 0.0f;

	D3DXVec3Transform(&TempVector, &D3DXVECTOR3(0.0f, 0.0f, 0.1f), &TempMatrix);
	TargetVectorZ.x = ScreenOffset.x + TempVector.x;
	TargetVectorZ.y = ScreenOffset.y + TempVector.y;
	TargetVectorZ.z = 0.0f;

	D3DXVec3Project(&ScreenVectorX, &D3DXVECTOR3(1.1f, 0.0f, 0.0f), &ViewPort, &ProjectionMatrix, &ViewMatrix, &WorldMatrix);
	D3DXVec3Project(&ScreenVectorY, &D3DXVECTOR3(0.0f, 1.1f, 0.0f), &ViewPort, &ProjectionMatrix, &ViewMatrix, &WorldMatrix);
	D3DXVec3Project(&ScreenVectorZ, &D3DXVECTOR3(0.0f, 0.0f, 1.1f), &ViewPort, &ProjectionMatrix, &ViewMatrix, &WorldMatrix);

	ScreenVectorX += FontOffset;
	ScreenVectorY += FontOffset;
	ScreenVectorZ += FontOffset;

	Direct3DDevice->SetTransform(D3DTS_WORLD, &IdentityMatrix);
	Direct3DDevice->SetTransform(D3DTS_VIEW, &IdentityMatrix);
	Direct3DDevice->SetTransform(D3DTS_PROJECTION, &IdentityMatrix);

	XBeforeY = ((CameraYaw > (D3DX_PI / 4.0f)) && (CameraYaw < (5.0f * D3DX_PI / 4.0f)));
	ZBeforeXY = (CameraPitch < 0.0f);

	PrepareForLines();

	if(ZBeforeXY)
	{
		RenderLine(SourceVector, TargetVectorZ, COLOR_AXIS_Z);
		RenderText("Z", static_cast<INT>(ScreenVectorZ.x), static_cast<INT>(ScreenVectorZ.y), COLOR_AXIS_Z);
	}

	if(XBeforeY)
	{
		RenderLine(SourceVector, TargetVectorX, COLOR_AXIS_X);
		RenderText("X", static_cast<INT>(ScreenVectorX.x), static_cast<INT>(ScreenVectorX.y), COLOR_AXIS_X);
	}

	RenderLine(SourceVector, TargetVectorY, COLOR_AXIS_Y);
	RenderText("Y", static_cast<INT>(ScreenVectorY.x), static_cast<INT>(ScreenVectorY.y), COLOR_AXIS_Y);

	if(!XBeforeY)
	{
		RenderLine(SourceVector, TargetVectorX, COLOR_AXIS_X);
		RenderText("X", static_cast<INT>(ScreenVectorX.x), static_cast<INT>(ScreenVectorX.y), COLOR_AXIS_X);
	}

	if(!ZBeforeXY)
	{
		RenderLine(SourceVector, TargetVectorZ, COLOR_AXIS_Z);
		RenderText("Z", static_cast<INT>(ScreenVectorZ.x), static_cast<INT>(ScreenVectorZ.y), COLOR_AXIS_Z);
	}

	ViewMatrix = TempViewMatrix;
	Direct3DDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);
	Direct3DDevice->SetTransform(D3DTS_VIEW, &ViewMatrix);
	Direct3DDevice->SetTransform(D3DTS_PROJECTION, &ProjectionMatrix);
}


//+-----------------------------------------------------------------------------
//| Fills the screen in one color
//+-----------------------------------------------------------------------------
VOID GRAPHICS::RenderFill(D3DCOLOR Color)
{
	Direct3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, Color, 0.0f, 0);
}


//+-----------------------------------------------------------------------------
//| Renders a line
//+-----------------------------------------------------------------------------
VOID GRAPHICS::RenderLine(CONST D3DXVECTOR3& From, CONST D3DXVECTOR3& To, D3DCOLOR Color)
{
	LINE_VERTEX* VertexPointer;

	if(FAILED(LineVertexBuffer->Lock(0, 0, reinterpret_cast<VOID**>(&VertexPointer), 0))) return;

	VertexPointer[0].Position = From;
	VertexPointer[0].Color = Color;
	VertexPointer[1].Position = To;
	VertexPointer[1].Color = Color;

	LineVertexBuffer->Unlock();

	Direct3DDevice->SetStreamSource(0, LineVertexBuffer, 0, sizeof(LINE_VERTEX));
	Direct3DDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1);
}


//+-----------------------------------------------------------------------------
//| Renders a box
//+-----------------------------------------------------------------------------
VOID GRAPHICS::RenderBox(CONST D3DXVECTOR3& Corner1, CONST D3DXVECTOR3& Corner2, D3DCOLOR Color)
{
	FLOAT Width;
	FLOAT Height;
	FLOAT Depth;
	LPD3DXMESH Mesh;
	D3DXVECTOR3 Center;
	DWORD OldCullMode;
	DWORD OldFillMode;

	Width = std::abs(Corner1.x - Corner2.x);
	Height = std::abs(Corner1.y - Corner2.y);
	Depth = std::abs(Corner1.z - Corner2.z);

	if(FAILED(D3DXCreateBox(Direct3DDevice, Width, Height, Depth, &Mesh, NULL))) return;

	Center = (Corner1 + Corner2) / 2;
	D3DXMatrixTranslation(&WorldMatrix, Center.x, Center.y, Center.z);
	SetWorldMatrix(WorldMatrix);

	Direct3DDevice->GetRenderState(D3DRS_CULLMODE, &OldCullMode);
	Direct3DDevice->GetRenderState(D3DRS_FILLMODE, &OldFillMode);

	Direct3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	Direct3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	Direct3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);
	Direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVBLENDFACTOR);
	Direct3DDevice->SetRenderState(D3DRS_BLENDFACTOR, Color);

	Mesh->DrawSubset(0);

	Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	Direct3DDevice->SetRenderState(D3DRS_CULLMODE, OldCullMode);
	Direct3DDevice->SetRenderState(D3DRS_FILLMODE, OldFillMode);

	SAFE_RELEASE(Mesh);
}


//+-----------------------------------------------------------------------------
//| Renders a sphere
//+-----------------------------------------------------------------------------
VOID GRAPHICS::RenderSphere(CONST D3DXVECTOR3& Center, FLOAT Radius, D3DCOLOR Color)
{
	INT Slices;
	INT Stacks;
	LPD3DXMESH Mesh;
	DWORD OldCullMode;
	DWORD OldFillMode;

	Slices = 8;
	Stacks = 6;

	if(FAILED(D3DXCreateSphere(Direct3DDevice, Radius, Slices, Stacks, &Mesh, NULL))) return;

	D3DXMatrixTranslation(&WorldMatrix, Center.x, Center.y, Center.z);
	SetWorldMatrix(WorldMatrix);

	Direct3DDevice->GetRenderState(D3DRS_CULLMODE, &OldCullMode);
	Direct3DDevice->GetRenderState(D3DRS_FILLMODE, &OldFillMode);

	Direct3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	Direct3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	Direct3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);
	Direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVBLENDFACTOR);
	Direct3DDevice->SetRenderState(D3DRS_BLENDFACTOR, Color);

	Mesh->DrawSubset(0);

	Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	Direct3DDevice->SetRenderState(D3DRS_CULLMODE, OldCullMode);
	Direct3DDevice->SetRenderState(D3DRS_FILLMODE, OldFillMode);

	SAFE_RELEASE(Mesh);
}


//+-----------------------------------------------------------------------------
//| Renders a particle
//+-----------------------------------------------------------------------------
VOID GRAPHICS::RenderParticle(D3DCOLOR Color, FLOAT Left, FLOAT Top, FLOAT Right, FLOAT Bottom)
{
	PARTICLE_VERTEX* VertexPointer;

	if(FAILED(ParticleVertexBuffer->Lock(0, 0, reinterpret_cast<VOID**>(&VertexPointer), 0))) return;

	VertexPointer[0].Color = Color;
	VertexPointer[0].TexturePosition = D3DXVECTOR2(Left, Top);
	VertexPointer[1].Color = Color;
	VertexPointer[1].TexturePosition = D3DXVECTOR2(Left, Bottom);
	VertexPointer[2].Color = Color;
	VertexPointer[2].TexturePosition = D3DXVECTOR2(Right, Top);
	VertexPointer[3].Color = Color;
	VertexPointer[3].TexturePosition = D3DXVECTOR2(Right, Bottom);

	ParticleVertexBuffer->Unlock();

	Direct3DDevice->SetFVF(PARTICLE_VERTEX::FORMAT);
	Direct3DDevice->SetStreamSource(0, ParticleVertexBuffer, 0, sizeof(PARTICLE_VERTEX));
	Direct3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}


//+-----------------------------------------------------------------------------
//| Renders the ground texture
//+-----------------------------------------------------------------------------
VOID GRAPHICS::RenderGroundTexture()
{
	FLOAT Scale;
	TEXTURE* Texture;

	if(!Properties().UseGroundTexture) return;

	TextureManager.Load(Properties().GroundTexture);
	Error.ClearMessage();

	Texture = TextureManager.GetTexture(Properties().GroundTexture);
	if(Texture == NULL) return;

	Scale = Properties().GroundTextureScale;
	D3DXMatrixScaling(&WorldMatrix, Scale, Scale, Scale);
	SetWorldMatrix(WorldMatrix);

	Graphics.SetShading(TRUE);
	Graphics.SetCulling(FALSE);

	Graphics.SetShader();
	Graphics.SetShaderConstants();

	Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	Direct3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	Direct3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	Direct3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	Direct3DDevice->SetTexture(0, Texture->GetTexture());
	Direct3DDevice->SetFVF(GROUND_VERTEX::FORMAT);
	Direct3DDevice->SetStreamSource(0, GroundVertexBuffer, 0, sizeof(GROUND_VERTEX));
	Direct3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}


//+-----------------------------------------------------------------------------
//| Renders a texture
//+-----------------------------------------------------------------------------
VOID GRAPHICS::RenderTexture(TEXTURE* Texture, RECT* SourceRect, INT X, INT Y, D3DCOLOR Color)
{
	D3DXVECTOR3 Position;

	if(Texture == NULL) return;

	Position.x = static_cast<FLOAT>(X);
	Position.y = static_cast<FLOAT>(Y);
	Position.z = 0.0f;

	D3DXMatrixIdentity(&WorldMatrix);
	Direct3DDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);

	Sprite->SetTransform(&WorldMatrix);
	Sprite->Begin(D3DXSPRITE_ALPHABLEND);
	Sprite->Draw(Texture->GetTexture(), SourceRect, NULL, &Position, Color);
	Sprite->End();
}


//+-----------------------------------------------------------------------------
//| Renders a texture
//+-----------------------------------------------------------------------------
VOID GRAPHICS::RenderTexture(TEXTURE* Texture, RECT* SourceRect, RECT* TargetRect, D3DCOLOR Color)
{
	FLOAT Width;
	FLOAT Height;
	FLOAT ScaleX;
	FLOAT ScaleY;
	RECT TempRect;
	D3DXVECTOR3 Position;

	if(Texture == NULL) return;

	if(TargetRect == NULL)
	{
		TempRect.left = 0;
		TempRect.top = 0;
		TempRect.right = ScreenWidth;
		TempRect.bottom = ScreenHeight;

		TargetRect = &TempRect;
	}

	Position.x = static_cast<FLOAT>(TargetRect->left);
	Position.y = static_cast<FLOAT>(TargetRect->top);
	Position.z = 0.0f;

	Width = static_cast<FLOAT>(TargetRect->right - TargetRect->left);
	Height = static_cast<FLOAT>(TargetRect->bottom - TargetRect->top);

	ScaleX = (Width / Texture->GetWidth());
	ScaleY = (Height / Texture->GetHeight());

	D3DXMatrixScaling(&WorldMatrix, ScaleX, ScaleY, 1.0f);
	Direct3DDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);

	Sprite->SetTransform(&WorldMatrix);
	Sprite->Begin(D3DXSPRITE_ALPHABLEND);
	Sprite->Draw(Texture->GetTexture(), SourceRect, NULL, &Position, Color);
	Sprite->End();
}


//+-----------------------------------------------------------------------------
//| Renders some text
//+-----------------------------------------------------------------------------
VOID GRAPHICS::RenderText(CONST std::string& Text, INT X, INT Y, D3DCOLOR Color)
{
	RECT Rect;

	Rect.left = X;
	Rect.top = Y;
	Rect.right = ScreenWidth;
	Rect.bottom = ScreenHeight;

	D3DXMatrixIdentity(&WorldMatrix);
	Direct3DDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);

	Sprite->SetTransform(&WorldMatrix);
	Sprite->Begin(D3DXSPRITE_ALPHABLEND);
	Font.GetFont()->DrawText(Sprite, Text.c_str(), static_cast<INT>(Text.size()), &Rect, DT_LEFT | DT_WORDBREAK, Color);
	Sprite->End();
}


//+-----------------------------------------------------------------------------
//| Renders some text
//+-----------------------------------------------------------------------------
VOID GRAPHICS::RenderText(CONST std::string& Text, RECT* Rect, D3DCOLOR Color)
{
	D3DXMatrixIdentity(&WorldMatrix);
	Direct3DDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);

	Sprite->SetTransform(&WorldMatrix);
	Sprite->Begin(D3DXSPRITE_ALPHABLEND);
	Font.GetFont()->DrawText(Sprite, Text.c_str(), static_cast<INT>(Text.size()), Rect, DT_LEFT | DT_WORDBREAK, Color);
	Sprite->End();
}


//+-----------------------------------------------------------------------------
//| Sets the shaders
//+-----------------------------------------------------------------------------
VOID GRAPHICS::SetShader()
{
	BOOL EnableLights;

	EnableLights = (UseShading && Properties().UseLighting);
	Direct3DDevice->SetRenderState(D3DRS_CULLMODE, (UseCulling ? (Properties().ClockwiseCulling ? D3DCULL_CW : D3DCULL_CCW) : D3DCULL_NONE));
	Direct3DDevice->SetRenderState(D3DRS_FILLMODE, Properties().FillMode);
	//Direct3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	if(Properties().UseShaders)
	{
		VertexShader.Use();
		if(EnableLights) PixelShaderShaded.Use();
		else PixelShaderUnshaded.Use();
	}
	else
	{
		Direct3DDevice->SetVertexShader(NULL);
		Direct3DDevice->SetPixelShader(NULL);

		Light.Ambient = D3DXCOLOR(Properties().Ambient);
		Light.Diffuse = D3DXCOLOR(Properties().Diffuse);
		Light.Specular = D3DXCOLOR(Properties().Specular);

		Material.Power = Properties().Power;

		Direct3DDevice->SetLight(0, &Light);
		Direct3DDevice->SetMaterial(&Material);

		Direct3DDevice->SetRenderState(D3DRS_SPECULARENABLE, EnableLights);
		Direct3DDevice->SetRenderState(D3DRS_LIGHTING, EnableLights);
		Direct3DDevice->LightEnable(0, EnableLights);
	}
}


//+-----------------------------------------------------------------------------
//| Sets the shader constants
//+-----------------------------------------------------------------------------
VOID GRAPHICS::SetShaderConstants()
{
	D3DXMATRIX ViewProjectionMatrix;

	if(Properties().UseShaders)
	{
		D3DXMatrixMultiply(&ViewProjectionMatrix, &ViewMatrix, &ProjectionMatrix);
		D3DXMatrixMultiply(&WorldViewProjectionMatrix, &WorldMatrix, &ViewProjectionMatrix);

		SetConstantTableConstants(VertexShader.GetConstantTable());
		SetConstantTableConstants(PixelShaderShaded.GetConstantTable());
		SetConstantTableConstants(PixelShaderUnshaded.GetConstantTable());
	}
}


//+-----------------------------------------------------------------------------
//| Prepares for line drawing
//+-----------------------------------------------------------------------------
VOID GRAPHICS::PrepareForLines()
{
	Direct3DDevice->SetVertexShader(NULL);
	Direct3DDevice->SetPixelShader(NULL);
	Direct3DDevice->SetTexture(0, NULL);
	Direct3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	Direct3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	Direct3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	Direct3DDevice->SetFVF(LINE_VERTEX::FORMAT);
}


//+-----------------------------------------------------------------------------
//| Prepares for view drawing
//+-----------------------------------------------------------------------------
VOID GRAPHICS::PrepareForViews()
{
	Direct3DDevice->SetVertexShader(NULL);
	Direct3DDevice->SetPixelShader(NULL);
	Direct3DDevice->SetTexture(0, NULL);
	Direct3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	Direct3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	Direct3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	Direct3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);
	Direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	Direct3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	Direct3DDevice->SetRenderState(D3DRS_CLIPPING, FALSE);
}


//+-----------------------------------------------------------------------------
//| Sets a new world matrix
//+-----------------------------------------------------------------------------
VOID GRAPHICS::SetWorldMatrix(CONST D3DXMATRIX& NewWorldMatrix)
{
	WorldMatrix = NewWorldMatrix;
	Direct3DDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);
}


//+-----------------------------------------------------------------------------
//| Sets a new view matrix
//+-----------------------------------------------------------------------------
VOID GRAPHICS::SetViewMatrix(CONST D3DXMATRIX& NewViewMatrix)
{
	ViewMatrix = NewViewMatrix;
	Direct3DDevice->SetTransform(D3DTS_VIEW, &ViewMatrix);
}


//+-----------------------------------------------------------------------------
//| Sets a new projection matrix
//+-----------------------------------------------------------------------------
VOID GRAPHICS::SetProjectionMatrix(CONST D3DXMATRIX& NewProjectionMatrix)
{
	ProjectionMatrix = NewProjectionMatrix;
	Direct3DDevice->SetTransform(D3DTS_PROJECTION, &ProjectionMatrix);
}


//+-----------------------------------------------------------------------------
//| Returns the world matrix
//+-----------------------------------------------------------------------------
CONST D3DXMATRIX& GRAPHICS::GetWorldMatrix() CONST
{
	return WorldMatrix;
}

//+-----------------------------------------------------------------------------
//| Returns the view matrix
//+-----------------------------------------------------------------------------
CONST D3DXMATRIX& GRAPHICS::GetViewMatrix() CONST
{
	return ViewMatrix;
}


//+-----------------------------------------------------------------------------
//| Returns the projection matrix
//+-----------------------------------------------------------------------------
CONST D3DXMATRIX& GRAPHICS::GetProjectionMatrix() CONST
{
	return ProjectionMatrix;
}


//+-----------------------------------------------------------------------------
//| Builds a ray from a screen position
//+-----------------------------------------------------------------------------
VOID GRAPHICS::BuildRay(CONST POINT& ScreenPosition, INT Width, INT Height, D3DXVECTOR3& RayPosition, D3DXVECTOR3& RayDirection)
{
	FLOAT TempScreenAspect;
	D3DXVECTOR3 RayTarget;
	D3DVIEWPORT9 ViewPort;
	D3DXMATRIX IdentityMatrix;
	D3DXMATRIX TempProjectionMatrix;
	D3DXVECTOR3 TempScreenPosition;

	TempScreenPosition.x = static_cast<FLOAT>(ScreenPosition.x);
	TempScreenPosition.y = static_cast<FLOAT>(ScreenPosition.y);

	D3DXMatrixIdentity(&IdentityMatrix);

	TempScreenAspect = (Height == 0) ? 0.0f : (static_cast<FLOAT>(Width) / static_cast<FLOAT>(Height));
	D3DXMatrixPerspectiveFovRH(&TempProjectionMatrix, FieldOfView, TempScreenAspect, NearDistance, FarDistance);

	ViewPort.X = 0;
	ViewPort.Y = 0;
	ViewPort.Width = Width;
	ViewPort.Height = Height;
	ViewPort.MinZ = 0.0f;
	ViewPort.MaxZ = 1.0f;

	TempScreenPosition.z = 0.0f;
	D3DXVec3Unproject(&RayPosition, &TempScreenPosition, &ViewPort, &TempProjectionMatrix, &ViewMatrix, &IdentityMatrix);

	TempScreenPosition.z = 1.0f;
	D3DXVec3Unproject(&RayTarget, &TempScreenPosition, &ViewPort, &TempProjectionMatrix, &ViewMatrix, &IdentityMatrix);

	RayDirection = RayTarget - RayPosition;
	D3DXVec3Normalize(&RayDirection, &RayDirection);
}


//+-----------------------------------------------------------------------------
//| Sets the constant table constants
//+-----------------------------------------------------------------------------
VOID GRAPHICS::SetConstantTableConstants(CONSTANT_TABLE& ConstantTable)
{
	D3DXCOLOR TempColor;

	TempColor = Properties().Ambient;
	ConstantTable.SetVector4(SHADER_AMBIENT_COLOR, D3DXVECTOR4(TempColor.r, TempColor.g, TempColor.b, TempColor.a));
	TempColor = Properties().Diffuse;
	ConstantTable.SetVector4(SHADER_DIFFUSE_COLOR, D3DXVECTOR4(TempColor.r, TempColor.g, TempColor.b, TempColor.a));
	TempColor = Properties().Specular;
	ConstantTable.SetVector4(SHADER_SPECULAR_COLOR, D3DXVECTOR4(TempColor.r, TempColor.g, TempColor.b, TempColor.a));
	ConstantTable.SetFloat(SHADER_SPECULAR_POWER, Properties().Power);

	ConstantTable.SetMatrix(SHADER_WORLD_MATRIX, WorldMatrix);
	ConstantTable.SetMatrix(SHADER_VIEW_MATRIX, ViewMatrix);
	ConstantTable.SetMatrix(SHADER_PROJECTION_MATRIX, ProjectionMatrix);
	ConstantTable.SetMatrix(SHADER_WORLD_VIEW_PROJECTION_MATRIX, WorldViewProjectionMatrix);

	ConstantTable.SetVector3(SHADER_CAMERA_POSITION, CameraPosition);
	ConstantTable.SetVector3(SHADER_CAMERA_DIRECTION, CameraDirection);
}


//+-----------------------------------------------------------------------------
//| Actions performed when the device is shut down
//+-----------------------------------------------------------------------------
BOOL GRAPHICS::OnShutdown()
{
	REFERENCE<GRAPHICS_WINDOW*, GRAPHICS*>* CurrentReference;
	REFERENCE<GRAPHICS_WINDOW*, GRAPHICS*>* NextReference;
	REFERENCE<TEXTURE*, GRAPHICS*>* CurrentTexture;
	REFERENCE<TEXTURE*, GRAPHICS*>* NextTexture;

	CurrentReference = GraphicsWindowReferenceObject.GetFirstReference();
	while(CurrentReference != NULL)
	{
		NextReference = GraphicsWindowReferenceObject.GetNextReference(CurrentReference);
		CurrentReference->GetData()->Destroy();
		CurrentReference = NextReference;
	}

	GraphicsWindowReferenceObject.Clear();

	CurrentTexture = TextureReferenceObject.GetFirstReference();
	while(CurrentTexture != NULL)
	{
		NextTexture = TextureReferenceObject.GetNextReference(CurrentTexture);
		CurrentTexture->GetData()->Clear();
		CurrentTexture = NextTexture;
	}

	TextureReferenceObject.Clear();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Actions performed when the device is lost
//+-----------------------------------------------------------------------------
BOOL GRAPHICS::OnLostDevice()
{
	REFERENCE<GRAPHICS_WINDOW*, GRAPHICS*>* CurrentReference;
	REFERENCE<GRAPHICS_WINDOW*, GRAPHICS*>* NextReference;

	Font.OnLostDevice();
	if(Sprite) Sprite->OnLostDevice();

	CurrentReference = GraphicsWindowReferenceObject.GetFirstReference();
	while(CurrentReference != NULL)
	{
		NextReference = GraphicsWindowReferenceObject.GetNextReference(CurrentReference);
		CurrentReference->GetData()->Release();
		CurrentReference = NextReference;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Actions performed when the device is reset
//+-----------------------------------------------------------------------------
BOOL GRAPHICS::OnResetDevice()
{
	REFERENCE<GRAPHICS_WINDOW*, GRAPHICS*>* CurrentReference;
	REFERENCE<GRAPHICS_WINDOW*, GRAPHICS*>* NextReference;

	Font.OnResetDevice();
	if(Sprite) Sprite->OnResetDevice();

	CurrentReference = GraphicsWindowReferenceObject.GetFirstReference();
	while(CurrentReference != NULL)
	{
		NextReference = GraphicsWindowReferenceObject.GetNextReference(CurrentReference);
		CurrentReference->GetData()->Acquire();
		CurrentReference = NextReference;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a dummy window
//+-----------------------------------------------------------------------------
BOOL GRAPHICS::CreateDummyWindow()
{
	Window = CreateWindowEx(0, "BUTTON", "", WS_POPUP,
		0, 0, 1, 1,
		NULL, NULL, GetModuleHandle(NULL), NULL);
	if(Window == NULL)
	{
		Error.SetMessage("Unable to create a dummy window!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a Direct3D object
//+-----------------------------------------------------------------------------
BOOL GRAPHICS::CreateDirect3D()
{
	Direct3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(Direct3D == NULL)
	{
		Error.SetMessage("Unable to create a Direct3D object!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Checks the device capabilities
//+-----------------------------------------------------------------------------
BOOL GRAPHICS::CheckCapabilities()
{
	D3DCAPS9 Capabilities;
	D3DDISPLAYMODE DisplayMode;
	std::stringstream Stream;

	if(!D3DXCheckVersion(D3D_SDK_VERSION, D3DX_SDK_VERSION))
	{
		Error.SetMessage("Your system does not have the latest DirectX version (DX9 required)!");
		return FALSE;
	}

	if(FAILED(Direct3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &Capabilities)))
	{
		Error.SetMessage("Unable to retrieve the Direct3D capabilities!");
		return FALSE;
	}

	if(FAILED(Direct3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &DisplayMode)))
	{
		Error.SetMessage("Unable to retrieve the Direct3D display mode!");
		return FALSE;
	}

	if(!(Capabilities.DeviceType & D3DDEVTYPE_HAL))
	{
		Error.SetMessage("Your system does not support HAL (Hardware Abstraction Layer)!");
		return FALSE;
	}

	Properties.MakeShadersAvailable(TRUE);

	if(Capabilities.VertexShaderVersion < D3DVS_VERSION(VERTEX_SHADER_MAJOR_VERSION, VERTEX_SHADER_MINOR_VERSION))
	{
		Properties.MakeShadersAvailable(FALSE);
	}

	if(Capabilities.PixelShaderVersion < D3DPS_VERSION(PIXEL_SHADER_MAJOR_VERSION, PIXEL_SHADER_MINOR_VERSION))
	{
		Properties.MakeShadersAvailable(FALSE);
	}

	if(!(Capabilities.TextureFilterCaps & D3DPTFILTERCAPS_MAGFLINEAR))
	{
		Error.SetMessage("Your system does not support linear magnifying filters!");
		return FALSE;
	}

	if(!(Capabilities.TextureFilterCaps & D3DPTFILTERCAPS_MINFLINEAR))
	{
		Error.SetMessage("Your system does not support linear minifying filters!");
		return FALSE;
	}

	if(!(Capabilities.TextureFilterCaps & D3DPTFILTERCAPS_MIPFLINEAR))
	{
		Error.SetMessage("Your system does not support linear mip-map filters!");
		return FALSE;
	}

	if(!(Capabilities.PrimitiveMiscCaps & D3DPMISCCAPS_CULLNONE))
	{
		Error.SetMessage("Your system does not support no culling!");
		return FALSE;
	}

	if(!(Capabilities.PrimitiveMiscCaps & D3DPMISCCAPS_CULLCW))
	{
		Error.SetMessage("Your system does not support clockwise culling!");
		return FALSE;
	}

	if(!(Capabilities.PrimitiveMiscCaps & D3DPMISCCAPS_CULLCCW))
	{
		Error.SetMessage("Your system does not support counter clockwise culling!");
		return FALSE;
	}

	GraphicsInfo.DeviceInfo.BackBufferFormat = D3DFMT_X8R8G8B8;
	if(FAILED(Direct3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, DisplayMode.Format, GraphicsInfo.DeviceInfo.BackBufferFormat, TRUE)))
	{
		GraphicsInfo.DeviceInfo.BackBufferFormat = D3DFMT_X1R5G5B5;
		if(FAILED(Direct3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, DisplayMode.Format, GraphicsInfo.DeviceInfo.BackBufferFormat, TRUE)))
		{
			GraphicsInfo.DeviceInfo.BackBufferFormat = D3DFMT_R5G6B5;
			if(FAILED(Direct3D->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, DisplayMode.Format, GraphicsInfo.DeviceInfo.BackBufferFormat, TRUE)))
			{
				Error.SetMessage("Your system does not support back buffers in the X8R8G8B8 format!");
				return FALSE;
			}
		}
	}

	GraphicsInfo.TextureFormat = D3DFMT_A8R8G8B8;
	if(FAILED(Direct3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, DisplayMode.Format, 0, D3DRTYPE_TEXTURE, GraphicsInfo.TextureFormat)))
	{
		GraphicsInfo.TextureFormat = D3DFMT_A4R4G4B4;
		if(FAILED(Direct3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, DisplayMode.Format, 0, D3DRTYPE_TEXTURE, GraphicsInfo.TextureFormat)))
		{
			Error.SetMessage("Your system does not support textures in the A8R8G8B8 format!");
			return FALSE;
		}
	}

	GraphicsInfo.DeviceInfo.AutoDepthStencilFormat = D3DFMT_D32;
	if(FAILED(Direct3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, DisplayMode.Format, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, GraphicsInfo.DeviceInfo.AutoDepthStencilFormat)))
	{
		GraphicsInfo.DeviceInfo.AutoDepthStencilFormat = D3DFMT_D24X8;
		if(FAILED(Direct3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, DisplayMode.Format, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, GraphicsInfo.DeviceInfo.AutoDepthStencilFormat)))
		{
			GraphicsInfo.DeviceInfo.AutoDepthStencilFormat = D3DFMT_D16;
			if(FAILED(Direct3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, DisplayMode.Format, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, GraphicsInfo.DeviceInfo.AutoDepthStencilFormat)))
			{
				Error.SetMessage("Your system does not support 16-bit depth buffers!");
				return FALSE;
			}
		}
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates a Direct3D device object
//+-----------------------------------------------------------------------------
BOOL GRAPHICS::CreateDirect3DDevice()
{
	GraphicsInfo.DeviceInfo.BackBufferWidth = 0;
	GraphicsInfo.DeviceInfo.BackBufferHeight = 0;
	GraphicsInfo.DeviceInfo.hDeviceWindow = NULL;
	GraphicsInfo.DeviceInfo.Windowed = TRUE;

	if(FAILED(Direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&GraphicsInfo.DeviceInfo, &Direct3DDevice)))
	{
		Error.SetMessage("Unable to create a Direct3D device!");
		return FALSE;
	}

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Creates all objects
//+-----------------------------------------------------------------------------
BOOL GRAPHICS::CreateObjects()
{
	PARTICLE_VERTEX* ParticleVertexPointer;
	GROUND_VERTEX* GroundVertexPointer;

	if(FAILED(D3DXCreateSprite(Direct3DDevice, &Sprite)))
	{
		Error.SetMessage("Unable to create a Direct3D sprite object!");
		return FALSE;
	}

	if(FAILED(Direct3DDevice->CreateVertexBuffer(sizeof(LINE_VERTEX) * 2, 0, LINE_VERTEX::FORMAT, D3DPOOL_MANAGED, &LineVertexBuffer, NULL)))
	{
		Error.SetMessage("Unable to create a line vertex buffer!");
		return FALSE;
	}

	if(FAILED(Direct3DDevice->CreateVertexBuffer(sizeof(PARTICLE_VERTEX) * 4, 0, PARTICLE_VERTEX::FORMAT, D3DPOOL_MANAGED, &ParticleVertexBuffer, NULL)))
	{
		Error.SetMessage("Unable to create a particle vertex buffer!");
		return FALSE;
	}

	if(FAILED(ParticleVertexBuffer->Lock(0, 0, reinterpret_cast<VOID**>(&ParticleVertexPointer), 0))) return FALSE;

	ParticleVertexPointer[0].Position = D3DXVECTOR3(0.0f, -1.0f, 1.0f);
	ParticleVertexPointer[0].Normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	ParticleVertexPointer[0].Color = 0xFFFFFFFF;
	ParticleVertexPointer[0].TexturePosition = D3DXVECTOR2(0.0f, 0.0f);

	ParticleVertexPointer[1].Position = D3DXVECTOR3(0.0f, -1.0f, -1.0f);
	ParticleVertexPointer[1].Normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	ParticleVertexPointer[1].Color = 0xFFFFFFFF;
	ParticleVertexPointer[1].TexturePosition = D3DXVECTOR2(0.0f, 1.0f);

	ParticleVertexPointer[2].Position = D3DXVECTOR3(0.0f, 1.0f, 1.0f);
	ParticleVertexPointer[2].Normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	ParticleVertexPointer[2].Color = 0xFFFFFFFF;
	ParticleVertexPointer[2].TexturePosition = D3DXVECTOR2(1.0f, 0.0f);

	ParticleVertexPointer[3].Position = D3DXVECTOR3(0.0f, 1.0f, -1.0f);
	ParticleVertexPointer[3].Normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	ParticleVertexPointer[3].Color = 0xFFFFFFFF;
	ParticleVertexPointer[3].TexturePosition = D3DXVECTOR2(1.0f, 1.0f);

	ParticleVertexBuffer->Unlock();

	if(FAILED(Direct3DDevice->CreateVertexBuffer(sizeof(GROUND_VERTEX) * 4, 0, GROUND_VERTEX::FORMAT, D3DPOOL_MANAGED, &GroundVertexBuffer, NULL)))
	{
		Error.SetMessage("Unable to create a ground vertex buffer!");
		return FALSE;
	}

	if(FAILED(GroundVertexBuffer->Lock(0, 0, reinterpret_cast<VOID**>(&GroundVertexPointer), 0))) return FALSE;

	GroundVertexPointer[0].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	GroundVertexPointer[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	GroundVertexPointer[0].Color = 0xFFFFFFFF;
	GroundVertexPointer[0].TexturePosition = D3DXVECTOR2(0.0f, 0.0f);

	GroundVertexPointer[1].Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	GroundVertexPointer[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	GroundVertexPointer[1].Color = 0xFFFFFFFF;
	GroundVertexPointer[1].TexturePosition = D3DXVECTOR2(0.0f, 1.0f);

	GroundVertexPointer[2].Position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	GroundVertexPointer[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	GroundVertexPointer[2].Color = 0xFFFFFFFF;
	GroundVertexPointer[2].TexturePosition = D3DXVECTOR2(1.0f, 0.0f);

	GroundVertexPointer[3].Position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	GroundVertexPointer[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	GroundVertexPointer[3].Color = 0xFFFFFFFF;
	GroundVertexPointer[3].TexturePosition = D3DXVECTOR2(1.0f, 1.0f);

	GroundVertexBuffer->Unlock();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Sets the render states
//+-----------------------------------------------------------------------------
VOID GRAPHICS::SetRenderStates()
{
	Direct3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	Direct3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	Direct3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	Direct3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	Direct3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	Direct3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	Direct3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	Direct3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Direct3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	Direct3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
}


//+-----------------------------------------------------------------------------
//| Sets the projection matrix
//+-----------------------------------------------------------------------------
VOID GRAPHICS::SetProjection()
{
	D3DXMatrixPerspectiveFovRH(&ProjectionMatrix, FieldOfView, ScreenAspect, NearDistance, FarDistance);
	Direct3DDevice->SetTransform(D3DTS_PROJECTION, &ProjectionMatrix);
}
