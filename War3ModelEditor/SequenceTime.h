//+-----------------------------------------------------------------------------
//| Inclusion guard
//+-----------------------------------------------------------------------------
#ifndef MAGOS_SEQUENCE_TIME_H
#define MAGOS_SEQUENCE_TIME_H


//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "Common.h"


//+-----------------------------------------------------------------------------
//| Sequence time structure
//+-----------------------------------------------------------------------------
struct SEQUENCE_TIME
{
	explicit SEQUENCE_TIME()
	{
		Time = 0;
		//IntervalStart = std::numeric_limits<INT>::min();
		//IntervalEnd = std::numeric_limits<INT>::max();
	}

	explicit SEQUENCE_TIME(INT NewTime)
	{
		Time = NewTime;
		//IntervalStart = std::numeric_limits<INT>::min();
		//IntervalEnd = std::numeric_limits<INT>::max();
	}

	explicit SEQUENCE_TIME(INT NewTime, INT NewIntervalStart, INT NewIntervalEnd)
	{
		Time = NewTime;
		IntervalStart = NewIntervalStart;
		IntervalEnd = NewIntervalEnd;
	}

	INT Time;
	INT IntervalStart;
	INT IntervalEnd;
};


//+-----------------------------------------------------------------------------
//| End of inclusion guard
//+-----------------------------------------------------------------------------
#endif
