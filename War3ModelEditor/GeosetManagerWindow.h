//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_GEOSET_MANAGER_WINDOW_H
#define MAGOS_GEOSET_MANAGER_WINDOW_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ManagerWindow.h"


//+-----------------------------------------------------------------------------
//| Geoset manager window class
//+-----------------------------------------------------------------------------
class GEOSET_MANAGER_WINDOW : public MANAGER_WINDOW
{
	public:
		CONSTRUCTOR GEOSET_MANAGER_WINDOW();
		DESTRUCTOR ~GEOSET_MANAGER_WINDOW();

		virtual BOOL Create();
		virtual VOID Destroy();

		virtual LRESULT MessageHandler(UINT Message, WPARAM W, LPARAM L);
		virtual LRESULT MenuHandler(WORD MenuItem);
		virtual LRESULT ControlHandler(HWND Control, WORD Code);
		virtual LRESULT NotifyHandler(HWND Control, UINT Code, NMHDR* Header);

		VOID SelectGeoset(MODEL_GEOSET* Geoset);

	protected:
		BOOL CreateNew();
		BOOL Remove(HWND Window);
		BOOL Import();
		BOOL Export();
		BOOL Edit();

		BOOL ImportGeoset(MODEL_GEOSET& Geoset, CONST std::string& FileName, MODEL_BASE* Node);
		BOOL ExportGeoset(MODEL_GEOSET& Geoset, CONST std::string& FileName);
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern GEOSET_MANAGER_WINDOW GeosetManagerWindow;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
