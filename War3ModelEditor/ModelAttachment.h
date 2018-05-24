//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_ATTACHMENT_H
#define MAGOS_MODEL_ATTACHMENT_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "ModelBase.h"


//+-----------------------------------------------------------------------------
//| Model attachment data structure
//+-----------------------------------------------------------------------------
struct MODEL_ATTACHMENT_DATA : public MODEL_BASE_DATA
{
	MODEL_ATTACHMENT_DATA()
	{
		Path = "";
		Visibility.SetStaticScalar(1.0f, "Visibility");

		AttachmentId = INVALID_INDEX;
	}

	std::string Path;
	INTERPOLATOR Visibility;

	INT AttachmentId;
};


//+-----------------------------------------------------------------------------
//| Model attachment class
//+-----------------------------------------------------------------------------
class MODEL_ATTACHMENT : public MODEL_BASE
{
	public:
		CONSTRUCTOR MODEL_ATTACHMENT();
		DESTRUCTOR ~MODEL_ATTACHMENT();

		VOID Clear();
		INT GetSize();

		MODEL_ATTACHMENT_DATA& Data();

	protected:
		MODEL_ATTACHMENT_DATA AttachmentData;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
