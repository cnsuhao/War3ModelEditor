//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_RANDOM_H
#define MAGOS_RANDOM_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Common.h"


//+-----------------------------------------------------------------------------
//| Random class
//+-----------------------------------------------------------------------------
class RANDOM
{
	public:
		explicit RANDOM();
		virtual ~RANDOM();

		VOID SetSeed();
		VOID SetSeed(UINT NewSeed);
		UINT GetSeed() CONST;

		BOOL Bool() CONST;
		INT Int(INT Min, INT Max) CONST;
		FLOAT Float(FLOAT Min, FLOAT Max) CONST;
		DOUBLE Double(DOUBLE Min, DOUBLE Max) CONST;
		CHAR Digit() CONST;
		CHAR Letter() CONST;
		CHAR LowerCaseLetter() CONST;
		CHAR UpperCaseLetter() CONST;

	protected:
		UINT Seed;
};


//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
extern RANDOM Random;


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
