//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_NODE_MANAGER_WINDOW_H
#define MAGOS_NODE_MANAGER_WINDOW_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Window.h"


//+-----------------------------------------------------------------------------
//| Node manager window class
//+-----------------------------------------------------------------------------
class NODE_MANAGER_WINDOW : public WINDOW_FRAME
{
	public:
		CONSTRUCTOR NODE_MANAGER_WINDOW();
		DESTRUCTOR ~NODE_MANAGER_WINDOW();

		virtual BOOL Create();
		virtual VOID Destroy();

		virtual LRESULT MessageHandler(UINT Message, WPARAM W, LPARAM L);
		virtual LRESULT MenuHandler(WORD MenuItem);
		virtual LRESULT ControlHandler(HWND Control, WORD Code);
		virtual LRESULT NotifyHandler(HWND Control, UINT Code, NMHDR* Header);
		virtual LRESULT TreeViewHandler(UINT Code, NM_TREEVIEW* TreeViewHeader);

		VOID AddNode(MODEL_BASE* Node, HTREEITEM InsertionPoint = TVI_LAST);
		VOID AddNodeAndChildren(MODEL_BASE* Node, HTREEITEM InsertionPoint = TVI_LAST);
		VOID RemoveNode(MODEL_BASE* Node);
		VOID ClearAllNodes();

	protected:
		MODEL_BASE* GetCurrentParent();

		BOOL CreateNewAttachment();
		BOOL CreateNewBone();
		BOOL CreateNewCollisionShape();
		BOOL CreateNewEventObject();
		BOOL CreateNewHelper();
		BOOL CreateNewLight();
		BOOL CreateNewParticleEmitter();
		BOOL CreateNewParticleEmitter2();
		BOOL CreateNewRibbonEmitter();

		BOOL EditAttachment(MODEL_ATTACHMENT* Attachment);
		BOOL EditBone(MODEL_BONE* Bone);
		BOOL EditCollisionShape(MODEL_COLLISION_SHAPE* CollisionShape);
		BOOL EditEventObject(MODEL_EVENT_OBJECT* EventObject);
		BOOL EditHelper(MODEL_HELPER* Helper);
		BOOL EditLight(MODEL_LIGHT* Light);
		BOOL EditParticleEmitter(MODEL_PARTICLE_EMITTER* ParticleEmitter);
		BOOL EditParticleEmitter2(MODEL_PARTICLE_EMITTER_2* ParticleEmitter2);
		BOOL EditRibbonEmitter(MODEL_RIBBON_EMITTER* RibbonEmitter);

		BOOL MoveUp();
		BOOL MoveDown();
		BOOL MoveLeft();
		BOOL MoveRight();

		BOOL Remove();
		BOOL Edit();
		BOOL EditNode();
		BOOL Duplicate();

		VOID ResizeTreeView();

		WINDOW_TREEVIEW TreeView;

		MODEL_BASE* CurrentlySelectedNode;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern NODE_MANAGER_WINDOW NodeManagerWindow;


//+-----------------------------------------------------------------------------
//| Post-included files
//+-----------------------------------------------------------------------------
#include "MainWindow.h"


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
