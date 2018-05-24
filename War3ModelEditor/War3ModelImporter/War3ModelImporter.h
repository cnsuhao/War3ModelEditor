//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_WAR3_MODEL_IMPORTER_H
#define MAGOS_WAR3_MODEL_IMPORTER_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "War3ModelBuilder.h"


//+-----------------------------------------------------------------------------
//| Manages imports and exports. Do NOT define DLL_IMPORT yourself!!!
//+-----------------------------------------------------------------------------
#ifndef DLL
#ifdef DLL_IMPORT
#define DLL __declspec(dllimport)
#else
#define DLL __declspec(dllexport)
#endif
#endif


//+-----------------------------------------------------------------------------
//| Function prototypes
//+-----------------------------------------------------------------------------
extern "C" DLL BOOL DllImport(WAR3_MODEL_BUILDER* ModelBuilder, CONST CHAR* FileName, CONST CHAR* Data, INT DataSize);


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
