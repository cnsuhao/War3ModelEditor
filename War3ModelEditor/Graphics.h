//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_GRAPHICS_H
#define MAGOS_GRAPHICS_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "GraphicsWindow.h"
#include "TextureManager.h"
#include "ConstantTable.h"
#include "GraphicsInfo.h"
#include "Vertex.h"
#include "Extent.h"


//+-----------------------------------------------------------------------------
//| Graphics class
//+-----------------------------------------------------------------------------
class GRAPHICS
{
	public:
		CONSTRUCTOR GRAPHICS();
		DESTRUCTOR ~GRAPHICS();

		LPDIRECT3DDEVICE9 GetDevice() CONST;
		GRAPHICS_INFO* GetGraphicsInfo();
		D3DFORMAT GetTextureFormat() CONST;
		CONST D3DXMATRIX& GetBillboardMatrix() CONST;

		BOOL SetupShaders();
		VOID ShutdownShaders();

		VOID SetShading(BOOL Shading);
		VOID SetCulling(BOOL Culling);

		VOID SetCamera(CONST CAMERA& Camera);
		VOID SetCamera(CONST D3DXVECTOR3& Source, CONST D3DXVECTOR3& Target, CONST D3DXVECTOR3& Up = D3DXVECTOR3(0.0f, 0.0f, 1.0f));
		VOID SetFieldOfView(FLOAT NewFieldOfView);
		VOID SetClippingPlane(FLOAT NewNearDistance, FLOAT NewFarDistance);

		BOOL Setup();
		VOID Shutdown();

		BOOL CheckLostState();
		BOOL BeginRender(GRAPHICS_WINDOW& GraphicsWindow);
		VOID EndRender();

		VOID RenderDebug();
		VOID RenderAxises();

		VOID RenderFill(D3DCOLOR Color);
		VOID RenderLine(CONST D3DXVECTOR3& From, CONST D3DXVECTOR3& To, D3DCOLOR Color);
		VOID RenderBox(CONST D3DXVECTOR3& Corner1, CONST D3DXVECTOR3& Corner2, D3DCOLOR Color);
		VOID RenderSphere(CONST D3DXVECTOR3& Center, FLOAT Radius, D3DCOLOR Color);
		VOID RenderParticle(D3DCOLOR Color, FLOAT Left, FLOAT Top, FLOAT Right, FLOAT Bottom);
		VOID RenderGroundTexture();

		VOID RenderTexture(TEXTURE* Texture, RECT* SourceRect, INT X, INT Y, D3DCOLOR Color);
		VOID RenderTexture(TEXTURE* Texture, RECT* SourceRect, RECT* TargetRect, D3DCOLOR Color);

		VOID RenderText(CONST std::string& Text, INT X, INT Y, D3DCOLOR Color);
		VOID RenderText(CONST std::string& Text, RECT* Rect, D3DCOLOR Color);

		VOID SetShader();
		VOID SetShaderConstants();
		VOID PrepareForLines();
		VOID PrepareForViews();

		VOID SetWorldMatrix(CONST D3DXMATRIX& NewWorldMatrix);
		VOID SetViewMatrix(CONST D3DXMATRIX& NewViewMatrix);
		VOID SetProjectionMatrix(CONST D3DXMATRIX& NewProjectionMatrix);

		CONST D3DXMATRIX& GetProjectionMatrix() CONST;
		CONST D3DXMATRIX& GetViewMatrix() CONST;
		CONST D3DXMATRIX& GetWorldMatrix() CONST;

		VOID BuildRay(CONST POINT& ScreenPosition, INT Width, INT Height, D3DXVECTOR3& RayPosition, D3DXVECTOR3& RayDirection);

	protected:
		VOID SetConstantTableConstants(CONSTANT_TABLE& ConstantTable);

		BOOL OnShutdown();
		BOOL OnLostDevice();
		BOOL OnResetDevice();

		BOOL CreateDummyWindow();
		BOOL CreateDirect3D();
		BOOL CheckCapabilities();
		BOOL CreateDirect3DDevice();
		BOOL CreateObjects();

		VOID SetRenderStates();
		VOID SetProjection();

		GRAPHICS_INFO GraphicsInfo;
		GRAPHICS_WINDOW* CurrentGraphicsWindow;

		HWND Window;
		LPDIRECT3D9 Direct3D;
		LPDIRECT3DDEVICE9 Direct3DDevice;
		LPDIRECT3DVERTEXBUFFER9 LineVertexBuffer;
		LPDIRECT3DVERTEXBUFFER9 ParticleVertexBuffer;
		LPDIRECT3DVERTEXBUFFER9 GroundVertexBuffer;
		D3DMATERIAL9 Material;
		D3DLIGHT9 Light;

		LPD3DXSPRITE Sprite;

		D3DXMATRIX ProjectionMatrix;
		D3DXMATRIX ViewMatrix;
		D3DXMATRIX WorldMatrix;
		D3DXMATRIX BillboardMatrix;
		D3DXMATRIX WorldViewProjectionMatrix;

		D3DXVECTOR3 CameraPosition;
		D3DXVECTOR3 CameraDirection;

		INT ScreenWidth;
		INT ScreenHeight;
		FLOAT ScreenAspect;
		FLOAT FieldOfView;
		FLOAT NearDistance;
		FLOAT FarDistance;

		BOOL Initialized;
		BOOL DeviceLost;
		BOOL UseShading;
		BOOL UseCulling;

		REFERENCE_OBJECT<GRAPHICS_WINDOW*, GRAPHICS*> GraphicsWindowReferenceObject;
		REFERENCE_OBJECT<TEXTURE*, GRAPHICS*> TextureReferenceObject;

		friend class GRAPHICS_WINDOW;
		friend class TEXTURE;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern GRAPHICS Graphics;


//+-----------------------------------------------------------------------------
//| Post-included files
//+-----------------------------------------------------------------------------
#include "Font.h"
#include "Mesh.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "ParticleManager.h"
#include "RibbonManager.h"


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
