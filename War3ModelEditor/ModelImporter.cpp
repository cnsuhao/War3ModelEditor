//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ModelImporter.h"
#include "ModelBuilder.h"


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
MODEL_IMPORTER ModelImporter;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
MODEL_IMPORTER::MODEL_IMPORTER()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
MODEL_IMPORTER::~MODEL_IMPORTER()
{
	//Empty
}


//+-----------------------------------------------------------------------------
//| Clears all importers
//+-----------------------------------------------------------------------------
VOID MODEL_IMPORTER::ClearAllImporters()
{
	INT i;

	for(i = 0; i < ImporterContainer.GetTotalSize(); i++)
	{
		if(ImporterContainer.ValidIndex(i))
		{
			UnloadDll(ImporterContainer[i]->Dll);
			delete ImporterContainer[i];
		}
	}
}


//+-----------------------------------------------------------------------------
//| Loads all importers
//+-----------------------------------------------------------------------------
BOOL MODEL_IMPORTER::LoadAllImporters()
{
	BOOL Looping;
	HANDLE SearchHandle;
	WIN32_FIND_DATA FileData;
	std::string Path;
	std::string FileName;

	ClearAllImporters();

	Path = Common.GetProgramDirectory() + "\\" + PATH_IMPORTER_FILES;

	SearchHandle = FindFirstFile(Path.c_str(), &FileData);
	Looping = (SearchHandle != INVALID_HANDLE_VALUE);

	while(Looping)
	{
		FileName = Common.GetProgramDirectory() + "\\" + PATH_IMPORTERS + FileData.cFileName;
		if(!LoadImporter(FileName))
		{
			FindClose(SearchHandle);
			return FALSE;
		}

		if(!FindNextFile(SearchHandle, &FileData))
		{
			FindClose(SearchHandle);
			Looping = FALSE;
		}
	}

	if(!BuildFilter()) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Imports a model
//+-----------------------------------------------------------------------------
BOOL MODEL_IMPORTER::ImportModel(CONST std::string& FileName, BUFFER& Buffer)
{
	INT Index;
	std::string Extention;

	Extention = Common.LowerCase(Common.GetExtention(FileName));

	Index = ImporterContainer.GetIndex(Extention);
	if(Index == INVALID_INDEX)
	{
		Error.SetMessage("Unable to import \"" + FileName + "\", no importer for this extention exists!");
		return FALSE;
	}

	if(!ImporterContainer[Index]->ImportFunction(&ModelBuilder, FileName.c_str(), Buffer.GetData(), Buffer.GetSize())) return FALSE;

	Model.ConnectNodes();
	Model.UnwrapPivotPoints();

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Returns the filter
//+-----------------------------------------------------------------------------
CONST CHAR* MODEL_IMPORTER::GetFilter()
{
	if(Filter.size() <= 0) return NULL;

	return &Filter[0];
}


//+-----------------------------------------------------------------------------
//| Loads an importer
//+-----------------------------------------------------------------------------
BOOL MODEL_IMPORTER::LoadImporter(CONST std::string& FileName)
{
	MODEL_IMPORTER_NODE* Node;

	Node = new MODEL_IMPORTER_NODE();
	if(Node == NULL)
	{
		Error.SetMessage("Unable to load \"" + FileName + "\", memory allocation failed!");
		return FALSE;
	}


	Node->Extention = Common.LowerCase(Common.GetName(FileName));

	Node->Dll = LoadDll(FileName);
	if(Node->Dll == NULL)
	{
		delete Node;
		return FALSE;
	}

	Node->ImportFunction = reinterpret_cast<IMPORT_FUNCTION>(GetProcAddress(Node->Dll, "DllImport"));
	if(Node->ImportFunction == NULL)
	{
		Error.SetMessage("Unable to load \"" + FileName + "\", no DllImport function found!");
		delete Node;
		return FALSE;
	}

	if(!ImporterContainer.Add(Node->Extention, Node)) return FALSE;

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Builds all filters
//+-----------------------------------------------------------------------------
BOOL MODEL_IMPORTER::BuildFilter()
{
	INT i;
	std::string Extentions;

	Filter.clear();

	for(i = 0; i < ImporterContainer.GetTotalSize(); i++)
	{
		if(ImporterContainer.ValidIndex(i))
		{
			if(Extentions.size() > 0) Extentions += ";";
			Extentions += "*.";
			Extentions += ImporterContainer[i]->Extention;
		}
	}

	if(ImporterContainer.GetSize() > 0)
	{
		AppendStringToVector(Filter, "Model Files (" + Extentions + ")");
		AppendNullToVector(Filter);
		AppendStringToVector(Filter, Extentions);
		AppendNullToVector(Filter);
	}

	AppendStringToVector(Filter, "All Files (*.*)");
	AppendNullToVector(Filter);
	AppendStringToVector(Filter, "*.*");
	AppendNullToVector(Filter);
	AppendNullToVector(Filter);

	return TRUE;
}


//+-----------------------------------------------------------------------------
//| Appends a string to a vector
//+-----------------------------------------------------------------------------
VOID MODEL_IMPORTER::AppendStringToVector(std::vector<CHAR>& Vector, CONST std::string& String)
{
	INT i;
	INT Size;

	Size = static_cast<INT>(String.size());

	for(i = 0; i < Size; i++)
	{
		Vector.push_back(String[i]);
	}
}


//+-----------------------------------------------------------------------------
//| Appends a NULL-character to a vector
//+-----------------------------------------------------------------------------
VOID MODEL_IMPORTER::AppendNullToVector(std::vector<CHAR>& Vector)
{
	Vector.push_back('\0');
}


//+-----------------------------------------------------------------------------
//| Loads a dll
//+-----------------------------------------------------------------------------
HINSTANCE MODEL_IMPORTER::LoadDll(CONST std::string& FileName)
{
	HINSTANCE Dll;

	Dll = ::LoadLibrary(FileName.c_str());
	if(Dll == NULL)
	{
		Error.SetMessage("Unable to load \"" + FileName + "\"!");
		return FALSE;
	}

	return Dll;
}


//+-----------------------------------------------------------------------------
//| Unloads a dll
//+-----------------------------------------------------------------------------
VOID MODEL_IMPORTER::UnloadDll(HINSTANCE Dll)
{
	if(Dll != NULL) ::FreeLibrary(Dll);
}
