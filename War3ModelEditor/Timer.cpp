//+-----------------------------------------------------------------------------
//| Included files
//+-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Timer.h"



//+-----------------------------------------------------------------------------
//| Global objects
//+-----------------------------------------------------------------------------
TIMER Timer;


//+-----------------------------------------------------------------------------
//| Constructor
//+-----------------------------------------------------------------------------
TIMER::TIMER()
{
	HighResolutionSet = FALSE;
	NextTimerValue = 0;

	if(::timeBeginPeriod(EDITOR_TIMER_RESOLUTION) == TIMERR_NOERROR)
	{
		HighResolutionSet = TRUE;
	}
}


//+-----------------------------------------------------------------------------
//| Destructor
//+-----------------------------------------------------------------------------
TIMER::~TIMER()
{
	if(HighResolutionSet)
	{
		::timeEndPeriod(EDITOR_TIMER_RESOLUTION);
		HighResolutionSet = FALSE;
	}
}


//+-----------------------------------------------------------------------------
//| Starts a new timeout
//+-----------------------------------------------------------------------------
VOID TIMER::Start(DWORD Timeout)
{
	NextTimerValue = ::timeGetTime() + Timeout;
}


//+-----------------------------------------------------------------------------
//| Checks if the new timeout has occured
//+-----------------------------------------------------------------------------
BOOL TIMER::Check() CONST
{
	return ::timeGetTime() >= NextTimerValue;
}


//+-----------------------------------------------------------------------------
//| Sleeps if neccessary (not longer than the timeout)
//+-----------------------------------------------------------------------------
VOID TIMER::Sleep() CONST
{
	DWORD SleepTime;
	DWORD CurrentTime;

	CurrentTime = ::timeGetTime();

	if(NextTimerValue <= CurrentTime) return;

	SleepTime = NextTimerValue - CurrentTime;

	if(SleepTime <= EDITOR_TIMER_MIN_SLEEP_TIME) return;

	if(SleepTime > EDITOR_TIMER_MAX_SLEEP_TIME)
	{
		SleepTime = EDITOR_TIMER_MAX_SLEEP_TIME + EDITOR_TIMER_MIN_SLEEP_TIME;
	}

	::Sleep(SleepTime - EDITOR_TIMER_MIN_SLEEP_TIME);
}
