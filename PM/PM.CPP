#include "PM.h"
#include <Windows.h>
#include <stdio.h>
#include <process.h>

int FPDCIMPM::CreateProcess( char* szImageName, char* szProcessName, int iUnitID, PROCESS_INFORMATION* stpProcessInformation )
{
	char		szCommandLine[ 255 ],
				szImagePath[ 255 ],
				szProcessPath[ 255 ];

	STARTUPINFO	stStartupInfo;
	memset( &stStartupInfo, 0, sizeof( STARTUPINFO ) );

	sprintf( szImagePath	, "D:\\FPDCIM\\DOWN\\%s.EXE"	, szImageName	);
	sprintf( szProcessPath	, "D:\\FPDCIM\\Bin\\%s.EXE"	, szProcessName	);

	DeleteFile( szProcessPath );
	Sleep( 100 );
	int iState = CopyFile( szImagePath, szProcessPath, FALSE	);
	if( 0 == iState ) {
		Sleep( 100 );
		iState = CopyFile( szImagePath, szProcessPath, FALSE	);
	}
	if( 0 == iState) {
//		Trace( "PMM-W-[CreateProcess] Make Image Fail : %s->%s", szImagePath, szProcessPath );
		return dERROR;
	}
	Sleep( 100 );

	sprintf( szCommandLine, "%s %d", szProcessPath, iUnitID );
//	Trace( "PMM-I-[CreateProcess] Proc[%s], Img[%s]", szImagePath, szCommandLine );
	iState = ::CreateProcess(
		0,						// name of executable module
		szCommandLine,			// command line string
		0,						// SD
		0,						// SD
		FALSE,					// handle inheritance option
		0,						// creation flags
		0,						// new environment block
		0,						// current directory name
		&stStartupInfo,			// startup information
		stpProcessInformation	// process information
	);
	if( 0 == iState ){
		Sleep( 100 );
		iState = ::CreateProcess(
			0,						// name of executable module
			szCommandLine,			// command line string
			0,						// SD
			0,						// SD
			FALSE,					// handle inheritance option
			0,						// creation flags
			0,						// new environment block
			0,						// current directory name
			&stStartupInfo,			// startup information
			stpProcessInformation	// process information
		);
	}

	// 081113
	// GUI Processor만 실행 완료를 기다린다.
	if(iUnitID == 1)
	{
		char szTask[50] = {0x00,};
		long nError1 = WaitForInputIdle(stpProcessInformation->hProcess , INFINITE );
		long nError2 = GetLastError();
		
		if(nError1 == WAIT_TIMEOUT )
		{
			sprintf(szTask,"PM TASK[%s] Starting Error : WAIT TIME OUT]", szProcessName, nError1);			
			MessageBox(NULL,szTask,	"PM ERROR",MB_OK);
		}
		if(nError2 != 0)
		{
			sprintf(szTask,"PM TASK[%s] Starting Error[%d]",szProcessName, nError2);
			MessageBox(NULL,szTask,	"PM ERROR",MB_OK);
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////


	if( iState ) return dSUCCESS;
	
//	Trace( "PMM-W-[CreateProcess] Process Creation Fail : %s", szCommandLine );
	
	return dERROR;
}

int FPDCIMPM::TerminateProcess( char* szProcessName, HANDLE& hProcess )
{
	char szProcessPath[ 255 ];

	sprintf( szProcessPath,	"D:\\FPDCIM\\BIN\\%s.EXE", szProcessName );

//	Trace( "PMM-I-[TerminateProcess] %s", szProcessName );
	int iState = ::TerminateProcess( hProcess, 0 );
	if( 0 == iState ) {
		Sleep( 100 );
		iState = ::TerminateProcess( hProcess, 0 );
	}
	if( 0 == iState ){
//		Trace( "PMM-W-[TerminateProcess] Process Terminate Fail : %s",	szProcessPath );
		return dERROR;
	}
	Sleep( 100 );
	iState = CloseHandle( hProcess );
	if( 0 == iState ) {
		Sleep( 100 );
		iState = CloseHandle( hProcess );
	}
//	if( 0 == iState )
//		Trace( "PMM-W-[TerminateProcess] %s Handle Close Fail", szProcessName );

	hProcess = HANDLE( 0 );

	iState = DeleteFile( szProcessPath );
	if( 0 == iState ) {
		Sleep( 100 );
		iState = DeleteFile( szProcessPath );
	}
	if( 0 == iState ) {
//		Trace( "PMM-W-[TerminateProcess] Process Remove Fail : %s",	szProcessPath );
		return dERROR;
	}

	return dSUCCESS;
}


void FPDCIMPM::InitProcessor( )
{
	// Processor Wake-up
	StartAllProcessor( );
}


void FPDCIMPM::StartProcessor( int iLocation )
{
	this->CreateProcess( 
		m_stProcessInfo.stProcessor[ iLocation ].szImageName,
		m_stProcessInfo.stProcessor[ iLocation ].szProcessName,
		m_stProcessInfo.stProcessor[ iLocation ].iUnitID,
		&m_stProcessInfo.stProcessor[ iLocation ].stProcessInfo
	);

	m_stProcessInfo.stProcessor[ iLocation ].iWakeCount = 1;
	m_stProcessInfo.stProcessor[ iLocation ].iTrace = 1;

	if (iLocation == GUI_TASK_ID-1)
		Sleep(10000);
	else
		Sleep(500);
}



void FPDCIMPM::StartAllProcessor()
{
//	Trace( "PMM-I-[StartAllProcessor]" );
	for( int iLoop = 0; iLoop < m_stProcessInfo.iProcessCount; iLoop ++ )
	{
		if(m_stProcessInfo.stProcessor[iLoop].bUsed)
		{
			StartProcessor( iLoop );
			
			if( iLoop == 0 )
				Sleep(1000);
			else
				Sleep(500);
		}
	}
}



void FPDCIMPM::KillProcessor( int iLocation )
{
	int iState;
	
	iState =  TerminateProcess(
		m_stProcessInfo.stProcessor[ iLocation ].szProcessName,
		m_stProcessInfo.stProcessor[ iLocation ].stProcessInfo.hProcess
	);

	m_stProcessInfo.stProcessor[ iLocation ].iWakeCount = 0;
	m_stProcessInfo.stProcessor[ iLocation ].iProcessState = 0;
}


void FPDCIMPM::KillAllProcessor()
{
//	Trace( "PMM-I-[KillAllProcessor]" );
	for( int iLoop = 0; iLoop < m_stProcessInfo.iProcessCount; iLoop ++ ) {
		KillProcessor( iLoop );
	} // End For
}

void FPDCIMPM::StartDemonProcessor( )
{
	int iState;

	PROCESS_INFORMATION	stProcInfo[2];

	//---- Trace Start-Up
	iState = CreateProcess("Log.exe", "Log.exe", 0, &stProcInfo[0]);
	if( dERROR == iState){
//		m_stpDemonProcessInfo->stProcessor[ 2 ].iProcessState = 0;
	} else {
//		m_stpDemonProcessInfo->stProcessor[ 2 ].iProcessState = 1;
	}
	Sleep( 300 );

	iState = CreateProcess("Tmp.exe", "Tmp.exe", 0, &stProcInfo[1]);
	if( dERROR == iState){
//		m_stpDemonProcessInfo->stProcessor[ 2 ].iProcessState = 0;
	} else {
//		m_stpDemonProcessInfo->stProcessor[ 2 ].iProcessState = 1;
	}

	Sleep( 300 );
}


void FPDCIMPM::KillDemonProcessor( )
{

}



void FPDCIMPM::Init( int iUnitID )
{
	// Set Privilege Up
	HANDLE hToken; 
	TOKEN_PRIVILEGES tkp; 

//	Trace( "PMM-I-@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" );
//	Trace( "PMM-I->>>>>>>>>>>>>>> Processor Manager Start <<<<<<<<<<<<<<<" );
//	Trace( "PMM-I-@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" );
 	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid); 
 
	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

	// Remove All Processor
	BOOL bRval = FALSE; //DeleteFile( "D:\\FPDCIM\\Bin\\**.EXE" );
	long nError = GetLastError();
	char szFileName[64];

	long iState = m_Queue.Create( "PM", 20, sizeof( stQueueRootType ) );
	if( dERROR == iState ) {
//		Trace( "PMM-E-[Init]Create PM Queue Fail, Exit Process" );
		ExitProcess( __LINE__ );
	}

	memset( &m_stProcessInfo,		0, sizeof( stProcessListType ) );

//	StartDemonProcessor( ); // Demon Processor Wake-up

	LoadProcessConfig();
	for( long i = 0; i < m_stProcessInfo.iProcessCount; i++ )
	{
		sprintf(szFileName, "D:\\FPDCIM\\Bin\\%s.EXE", m_stProcessInfo.stProcessor[i].szImageName);
		bRval = DeleteFile(szFileName);
		nError = GetLastError();
	}

	InitProcessor();
}


void FPDCIMPM::Run( )
{
	int	iLoop = 0;
	int	iState = 0;
gotoRecvQueue:
	if( 0 == m_Queue.GetState( ) ) goto gotoCheckProcess;

	m_Queue.Read( &m_stQueue	);
	switch( m_stQueue.nSignal ) 
	{
		case sigWakeUpProcess:
			switch ( m_stQueue.nParam )
			{
			case 1:	// Processor Start
				if( m_stQueue.nFlag )	StartProcessor( m_stQueue.nFlag -1 );
				else					InitProcessor( );
				break;
			case 2:
				if( m_stQueue.nFlag )	KillProcessor( m_stQueue.nFlag -1 );
				else
				{
					Sleep( 4100 );
					KillAllProcessor( );
				}
				break;
			}
			break;

		case 3 : // Demon Start
//			StartDemonProcessor(); 
			break;

		case 4 : // Demon Kill
			KillDemonProcessor(); 
			break;

		case sigShutDown:
			Sleep( 4100 );
			KillAllProcessor( );
			return ;

		default :		
			break;
	}

gotoCheckProcess:
	Sleep( 1000 );
	//---- Check Processor ----------------------------------------------------
	for( iLoop = 0; iLoop < m_stProcessInfo.iProcessCount; iLoop ++ )
	{
		if( WAIT_TIMEOUT ==
			WaitForSingleObject( m_stProcessInfo.stProcessor[ iLoop ].stProcessInfo.hProcess, 10 ) ) 
		{
			m_stProcessInfo.stProcessor[ iLoop ].iProcessState = 1;
			continue;
		} else
			m_stProcessInfo.stProcessor[ iLoop ].iProcessState = 0;

		if( 0 >= m_stProcessInfo.stProcessor[ iLoop ].iWakeCount )
			continue;

		if( 1 == m_stProcessInfo.stProcessor[ iLoop ].iTrace )
//			Trace( "PMM-I-[ProcessorDown] Process Name %s, Image %s, Unit ID %d, Remain Wake Up %d",
//				m_stProcessInfo.stProcessor[ iLoop ].szProcessName,
//				m_stProcessInfo.stProcessor[ iLoop ].szImageName,
//				m_stProcessInfo.stProcessor[ iLoop ].iUnitID,
//				m_stProcessInfo.stProcessor[ iLoop ].iWakeCount
//			);

		if( 1 <= m_stProcessInfo.stProcessor[ iLoop ].iWakeCount ) {
			iState = ::TerminateProcess( 
				m_stProcessInfo.stProcessor[ iLoop ].stProcessInfo.hProcess,
				0
			);
			Sleep( 100 );
			iState = CloseHandle( m_stProcessInfo.stProcessor[ iLoop ].stProcessInfo.hProcess );
			if( 0 == iState ) {
				Sleep( 300 );
				iState = CloseHandle( m_stProcessInfo.stProcessor[ iLoop ].stProcessInfo.hProcess );
			}
//			if( 0 == iState )
//				Trace( "PMM-W-[ProcessorDown] %s Handle Close Fail", m_stProcessInfo.stProcessor[ iLoop ].szProcessName );

			CreateProcess(
				m_stProcessInfo.stProcessor[ iLoop ].szImageName,
				m_stProcessInfo.stProcessor[ iLoop ].szProcessName,
				m_stProcessInfo.stProcessor[ iLoop ].iUnitID,
				&m_stProcessInfo.stProcessor[ iLoop ].stProcessInfo
			);

			(m_stProcessInfo.stProcessor[ iLoop ].iWakeCount) --;
		}
	}
	goto gotoRecvQueue;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	long uId = -1;	
	char* pszUnitName = (char *)lpCmdLine;
	if (pszUnitName != NULL) sscanf(pszUnitName, "%d", &uId);

	//...... check for previous instance of this application
	HANDLE hMutexInstance = CreateMutex(NULL, FALSE, "___ProcessManager___"); 
	DWORD rval = WaitForSingleObject(hMutexInstance, 0L);
	if (rval==WAIT_FAILED || rval==WAIT_TIMEOUT)
	{
		MessageBox(0, "ProcessManager is Already running !!", "ProcessManager", MB_OK);
		return FALSE;
	}

	FPDCIMPM	cProcManager;

	cProcManager.Init(0);
	cProcManager.Run();

	return 1;
}

void FPDCIMPM::LoadProcessConfig()
{
	long	i = 0;
	char	szSectionName[30];

	SetScriptFileName(PROCESS_CONFIG_FILENAME);
	GetInt("COUNT", "ProcessCount", (long &)m_stProcessInfo.iProcessCount);
	for( i = 0; i < m_stProcessInfo.iProcessCount; i++ )
	{
		sprintf(szSectionName,"PROCESS_CONFIG_%d", i+1);
		GetString(szSectionName, "ImageName"	, m_stProcessInfo.stProcessor[i].szImageName);
		GetString(szSectionName, "ProcessName"	, m_stProcessInfo.stProcessor[i].szProcessName);
		GetInt	 (szSectionName, "ProcessID"	, (long &)m_stProcessInfo.stProcessor[i].iUnitID);
		GetInt	 (szSectionName, "Used"	, (long &)m_stProcessInfo.stProcessor[i].bUsed);
	}
}
