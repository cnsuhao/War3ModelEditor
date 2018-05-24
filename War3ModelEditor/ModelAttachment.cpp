//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ModelAttachment.h"


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_ATTACHMENT::MODEL_ATTACHMENT()
{
	ModelBaseData = &AttachmentData;
	ModelBaseData->Type = NODE_TYPE_ATTACHMENT;
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL_ATTACHMENT::~MODEL_ATTACHMENT()
{
	Clear();
}


//+-----------------------------------------------------------------------------
//| Clears the attachment
//+-----------------------------------------------------------------------------
VOID MODEL_ATTACHMENT::Clear()
{
	AttachmentData = MODEL_ATTACHMENT_DATA();
}


//+-----------------------------------------------------------------------------
//| Returns the mdx size of the attachment
//+-----------------------------------------------------------------------------
INT MODEL_ATTACHMENT::GetSize()
{
	return GetBaseSize() + AttachmentData.Visibility.GetSize() + 12 + 256;
}


//+-----------------------------------------------------------------------------
//| Returns a reference to the data
//+-----------------------------------------------------------------------------
MODEL_ATTACHMENT_DATA& MODEL_ATTACHMENT::Data()
{
	return AttachmentData;
}
