//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_FILE_LOADER_H
#define MAGOS_FILE_LOADER_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Mpq.h"


//+-----------------------------------------------------------------------------
//| File loader class
//+-----------------------------------------------------------------------------
class FILE_LOADER
{
	public:
		CONSTRUCTOR FILE_LOADER();
		DESTRUCTOR ~FILE_LOADER();

		BOOL Load(CONST std::string& FileName, BUFFER& Buffer);

		BOOL LoadFromFile(CONST std::string& FileName, BUFFER& Buffer);
		BOOL LoadFromMpq(MPQ& Mpq, CONST std::string& FileName, BUFFER& Buffer);

		BOOL SaveToFile(CONST std::string& FileName, BUFFER& Buffer);

	protected:
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern FILE_LOADER FileLoader;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
