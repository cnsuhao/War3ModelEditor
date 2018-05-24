//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_MODEL_IMPORTER_H
#define MAGOS_MODEL_IMPORTER_H


//+-----------------------------------------------------------------------------
//| Definitions
//+-----------------------------------------------------------------------------
#define DLL_IMPORT


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Container.h"
#include "Buffer.h"
#include "War3ModelImporter\\War3ModelImporter.h"


//+-----------------------------------------------------------------------------
//| Importer function
//+-----------------------------------------------------------------------------
typedef BOOL (*IMPORT_FUNCTION)(WAR3_MODEL_BUILDER*, CONST CHAR*, CONST CHAR*, INT);


//+-----------------------------------------------------------------------------
//| Model importer node structure
//+-----------------------------------------------------------------------------
struct MODEL_IMPORTER_NODE
{
	MODEL_IMPORTER_NODE()
	{
		Extention = "";

		Dll = NULL;
		ImportFunction = NULL;
	}

	std::string Extention;

	HINSTANCE Dll;
	IMPORT_FUNCTION ImportFunction;
};


//+-----------------------------------------------------------------------------
//| Model importer class
//+-----------------------------------------------------------------------------
class MODEL_IMPORTER
{
	public:
		CONSTRUCTOR MODEL_IMPORTER();
		DESTRUCTOR ~MODEL_IMPORTER();

		VOID ClearAllImporters();
		BOOL LoadAllImporters();

		BOOL ImportModel(CONST std::string& FileName, BUFFER& Buffer);

		CONST CHAR* GetFilter();

	protected:
		BOOL LoadImporter(CONST std::string& FileName);

		BOOL BuildFilter();

		VOID AppendStringToVector(std::vector<CHAR>& Vector, CONST std::string& String);
		VOID AppendNullToVector(std::vector<CHAR>& Vector);

		HINSTANCE LoadDll(CONST std::string& FileName);
		VOID UnloadDll(HINSTANCE Dll);

		std::vector<CHAR> Filter;
		CONTAINER<MODEL_IMPORTER_NODE*> ImporterContainer;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern MODEL_IMPORTER ModelImporter;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
