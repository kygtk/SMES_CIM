//-----------------------------------------------------------------------------
//	File	: FPDCIMPM.h
//
//	Purpose : Processor Management
//
//
#include "../include/SysLib.h"

#define MAX_PROCESSOR_COUNT	10
#define PROCESS_CONFIG_FILENAME		"D:\\FPDCIM\\Data\\Config\\Process.cfg"

// Processor Information Type define
struct stProcessListType {
	int iProcessCount;
	struct stProcessType {
		PROCESS_INFORMATION	stProcessInfo;
		char	szImageName  [ 50 ];
		char	szProcessName[ 50 ];
		int		iUnitID;
		BOOL	bUsed;
		int		iUnitType;
		int		iTrace;				// 1: Trace ON, 2 : Trace Off
		int		iWakeCount;			// 0: No Wake
		int		iProcessState;		// 0: Down, 1 : Run
	} stProcessor[ MAX_PROCESSOR_COUNT ];
};

//-----------------------------------------------------------------------------
//	PMC Class
class FPDCIMPM : public SysLib
{
protected : 

protected : // Member variable
	stProcessListType	m_stProcessInfo;
	stQueueRootType		m_stQueue;
	CQueue				m_Queue;

protected : // Member function
	int		CreateProcess( char* szImageName, char* szProcessName, int iUnitID, PROCESS_INFORMATION* stProcessInfo );
	int		TerminateProcess( char* szImageName, HANDLE& hProcess );
	void	RemoveAllProcessor( );

protected :	// PMC TP Information
	void	StartAllProcessor( );
	void	StartProcessor( int iLocation );
	void	StartDemonProcessor( );

	void	KillAllProcessor( );
	void	KillProcessor( int iLocation );
	void	KillDemonProcessor( );

protected : // Receive from CTC I/F
	void	InitProcessor( );

public :
	void	LoadProcessConfig();
	void	Init( int iUnitID );
	void	Run( );
};