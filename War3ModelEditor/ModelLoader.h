//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_LOADER_H
#define MAGOS_MODEL_LOADER_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Model.h"


//+-----------------------------------------------------------------------------
//| Model loader class
//+-----------------------------------------------------------------------------
class MODEL_LOADER
{
	public:
		CONSTRUCTOR MODEL_LOADER();
		DESTRUCTOR ~MODEL_LOADER();

		virtual BOOL Save(MODEL& Model, CONST std::string& FileName, BUFFER& Buffer) = 0;
		virtual BOOL Load(MODEL& Model, CONST std::string& FileName, BUFFER& Buffer) = 0;

	protected:
		static std::string CurrentFileName;
};


//+-----------------------------------------------------------------------------
//| Post-included files
//+-----------------------------------------------------------------------------
#include "ModelLoaderMdl.h"
#include "ModelLoaderMdx.h"


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
