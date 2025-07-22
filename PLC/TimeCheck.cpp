//#include "stdafx.h"
#include "TimeCheck.h"
#include <time.h>


/////////////////////////////////////////////////
CTimerCheck::CTimerCheck()
{
	m_nTime = 0;
	m_bStart = FALSE;
}

CTimerCheck::~CTimerCheck()
{

}

/////////////////////////////////////////////////
BOOL CTimerCheck::IsStartedTimer()
{	
	if( m_bStart == TRUE ) 
		return TRUE;
	else
		return FALSE;
}

void CTimerCheck::StartTimer()
{	
	m_nTime = 0;
	m_bStart = TRUE;
}

void CTimerCheck::EndTimer()
{	
	m_nTime = 0;
	m_bStart = FALSE;
}

void CTimerCheck::Count()
{	
	m_nTime++;
}

void CTimerCheck::CountResest()
{	
	m_nTime = 0;
}

ULONG CTimerCheck::GetLimitedTime()
{
	return MAX_LIMIT_TIME_SET;

}


/////////////////////////////////////////////////
BOOL CTimerCheck::LessThan(ULONG ctime)
{
	if( m_nTime < (ctime/MAX_RUN_TIME_SET) ) 
		return TRUE;
	else
		return FALSE;
}

/////////////////////////////////////////////////
BOOL CTimerCheck::MoreThan(ULONG ctime)
{
	if( m_nTime > (ctime/MAX_RUN_TIME_SET) )
		return TRUE;
	else
		return FALSE;
}

ULONG CTimerCheck::GetTimerAfterStart()
{
	return (ULONG)(m_nTime*MAX_RUN_TIME_SET);
}

