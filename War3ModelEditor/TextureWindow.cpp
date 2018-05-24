//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "TextureWindow.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
TEXTURE_WINDOW TextureWindow;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
TEXTURE_WINDOW::TEXTURE_WINDOW()
{
	FileName = "";
	ReplaceableId = INVALID_INDEX;

	WindowActive = FALSE;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
TEXTURE_WINDOW::~TEXTURE_WINDOW()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Creates a new window
//+-----------------------------------------------------------------------------
BOOL TEXTURE_WINDOW::Create()
{
	FrameInfo.Title = "Texture";
	FrameInfo.Width = DEFAULT_TEXTURE_WINDOW_WIDTH;
	FrameInfo.Height = DEFAULT_TEXTURE_WINDOW_HEIGHT;
	FrameInfo.Style &= (~WS_VISIBLE);
	FrameInfo.Style |= (WS_MAXIMIZEBOX | WS_SIZEBOX);

	if(!WINDOW_FRAME::Create()) return FALSE;
	if(!GraphicsWindow.Create(Window)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Destroys the window
//+-----------------------------------------------------------------------------
VOID TEXTURE_WINDOW::Destroy()
{
	GraphicsWindow.Destroy();

	WINDOW_FRAME::Destroy();
}


//+-----------------------------------------------------------------------------
//| Handles the window messages
//+-----------------------------------------------------------------------------
LRESULT TEXTURE_WINDOW::MessageHandler(UINT Message, WPARAM W, LPARAM L)
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
			GraphicsWindow.Resize();
			return 0;
		}

		case WM_SIZING:
		{
			GraphicsWindow.Resize();
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
LRESULT TEXTURE_WINDOW::MenuHandler(WORD MenuItem)
{
	return 0;
}


//+-----------------------------------------------------------------------------
//| Handles the window control messages
//+-----------------------------------------------------------------------------
LRESULT TEXTURE_WINDOW::ControlHandler(HWND Control, WORD Code)
{
	return 0;
}


//+-----------------------------------------------------------------------------
//| Handles the window notify messages
//+-----------------------------------------------------------------------------
LRESULT TEXTURE_WINDOW::NotifyHandler(HWND Control, UINT Code, NMHDR* Header)
{
	return 0;
}


//+-----------------------------------------------------------------------------
//| Sets a new filename
//+-----------------------------------------------------------------------------
VOID TEXTURE_WINDOW::SetFileName(CONST std::string& NewFileName, INT NewReplaceableId)
{
	FileName = NewFileName;
	ReplaceableId = NewReplaceableId;
}


//+-----------------------------------------------------------------------------
//| Updates and renders the window
//+-----------------------------------------------------------------------------
VOID TEXTURE_WINDOW::UpdateAndRender()
{
	if(!WindowActive) return;

	if(Graphics.BeginRender(GraphicsWindow))
	{
		Render();
		Graphics.EndRender();
	}
}


//+-----------------------------------------------------------------------------
//| Renders the window
//+-----------------------------------------------------------------------------
VOID TEXTURE_WINDOW::Render()
{
	TEXTURE* Texture;

	Graphics.RenderFill(PROPERTIES_NO_BACKGROUND);

	if(FileName == "")
	{
		Texture = TextureManager.GetReplaceableTexture(ReplaceableId);
	}
	else
	{
		Texture = TextureManager.GetTexture(FileName);
	}

	if(Texture != NULL) Graphics.RenderTexture(Texture, NULL, NULL, 0xFFFFFFFF);
}
