//-----------------------------------------------------------------------------
//
//	FA Class Library for Window-NT ver 4.0
//
//
//	Queue Class Library
//
//////////////////////////////////////////////////////////////////////

//#pragma once

#include	"SW3FaLib.h"
#include	<stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQueue::CQueue()
{
	m_hShmem = HANDLE( NULL );
	m_hEvent = HANDLE( NULL );
	m_hMutex = HANDLE( NULL );

	m_szQueueName = (char*)NULL;

	m_lpDataCount		= (long *)NULL;
	m_lpReadPoint		= (long *)NULL;
	m_lpWritePoint		= (long *)NULL;
	m_vpDataBasePoint	= (void*)NULL;
}

CQueue::~CQueue()
{
	if( NULL != long( m_hShmem ) )
		CloseHandle( m_hShmem );

	if( NULL != long( m_hMutex ) )
		CloseHandle( m_hMutex );

	if( NULL != long( m_hEvent ) )
		CloseHandle( m_hEvent );

	if( m_szQueueName )
		delete [] m_szQueueName;
}


//-----------------------------------------------------------------------------
//	Queue Constructor
//
CQueue::CQueue( char* szQueueName, long nCount, long nSize )
{
	Create( szQueueName, nCount, nSize );
}


//-----------------------------------------------------------------------------
//	Queue Create or Open
//
long CQueue::Create( char* szQueueName, long nCount, long nSize )
{

	// Member Variable Data Mapping
	if( m_szQueueName )
		delete [] m_szQueueName;

	m_szQueueName = new char [ strlen( szQueueName ) + 1 ];

	strcpy( m_szQueueName, szQueueName );
	m_nCount = nCount;
	m_nSize  = nSize ;

	char	szQShMemName[ 255 ],
			szQMutexName[ 255 ],
			szQEventName[ 255 ];


	sprintf( szQShMemName, "QS_%s", m_szQueueName ); // Shared Memory Name Setting
	sprintf( szQMutexName, "QM_%s", m_szQueueName ); // Mutex Name Setting
	sprintf( szQEventName, "QE_%s", m_szQueueName ); // Event Name Setting


	// Shared Memory Create or Mapping
	if( 0 == long( m_hShmem ) )
        m_hShmem = CreateFileMapping(	// Create Shared Memory
			(HANDLE)-1,
			NULL,
			PAGE_READWRITE,
            NULL,
            m_nCount * m_nSize + sizeof( long *) * 3,
            szQShMemName
		);

	if( 0 == long( m_hShmem ) ) {
			m_hShmem = OpenFileMapping(
			FILE_MAP_READ | FILE_MAP_WRITE,
			FALSE,
			szQShMemName
		);

	}

	if( 0 == long( m_hShmem ) )
		return dERROR;


	long* vpShmem = 0;

	if( 0 == long( m_lpDataCount ) )
	vpShmem = (long *)MapViewOfFile(	// Shared Memory Mapping
			m_hShmem,
            FILE_MAP_READ | FILE_MAP_WRITE,
            NULL,
			NULL,
			NULL
		);

	if( 0 == long( vpShmem ) )
		return dERROR;

	m_lpDataCount		= vpShmem;
	m_lpReadPoint		= (vpShmem + 1 );
	m_lpWritePoint		= (vpShmem + 2 );
	m_vpDataBasePoint	= (void*)(vpShmem + 3 );



	// Mutex Create or Mapping
	if( NULL == long( m_hMutex ) )
	m_hMutex = CreateMutex(
			NULL,
			FALSE,
			szQMutexName
		);

	if( NULL == long( m_hMutex ) ){		// When Create Fail
		m_hMutex = OpenMutex(
				NULL,
				FALSE,
				szQMutexName
			);
		if( NULL == long( m_hMutex ) )
			return dERROR;
	}

	

	// Event Create or Mapping
	SECURITY_ATTRIBUTES	stAtt;
	memset( &stAtt, 0, sizeof( stAtt ) );
	if( NULL == long( m_hEvent ) )
	m_hEvent = CreateEvent(
			&stAtt,
			FALSE,
			FALSE,
			szQEventName
		);

	if( NULL == long( m_hEvent ) ){		// When Create Fail
		m_hEvent = OpenEvent(
				EVENT_ALL_ACCESS | SYNCHRONIZE,
				FALSE,
				szQEventName
			);
		if( NULL == long( m_hEvent ) )
			return dERROR;
	}

	return dSUCCESS;
}



//-----------------------------------------------------------------------------
//	Queue Delete
//
long CQueue::Delete( )
{
	if( 0 != long( m_lpDataCount ) )
	if( NULL == UnmapViewOfFile( m_lpDataCount ) )
		return dERROR;

	if( NULL != long( m_hShmem ) && NULL == CloseHandle( m_hShmem ) )
		return dERROR;

	if( NULL != long( m_hMutex ) && NULL == CloseHandle( m_hMutex ) )
		return dERROR;

	if( NULL != long( m_hEvent ) && NULL == CloseHandle( m_hEvent ) )
		return dERROR;


	m_lpDataCount = (long*) NULL;
	m_hShmem      = HANDLE( NULL );
	m_hMutex      = HANDLE( NULL );
	m_hEvent      = HANDLE( NULL );

	return dSUCCESS;
}



//-----------------------------------------------------------------------------
//	Queue Data Read Sync
//
long CQueue::Read( void* vpRead )
{
gotoRead:
	if( 0 == *m_lpDataCount ) {// Event Wait when Queue Data Not Ready
		if( 0 == long( m_hEvent ) )
			return dERROR;

		WaitForSingleObject( m_hEvent, INFINITE );
	}

	if( 0 == long( m_hMutex ) )
		return dERROR;

	WaitForSingleObject( m_hMutex, INFINITE ); // Mutex Wait

	if( 0 == long( m_lpDataCount ) )
		return dERROR;
	
	if( 0 == *m_lpDataCount ) {		// Queue Data Not Ready ?
		ReleaseMutex( m_hMutex );	// Mutex Release
		goto gotoRead;
	}

//	m_hCritical.Lock( );			// Inter Processor Lock

	// Queue Data Read
	memcpy( ( char* )( vpRead            ) ,
		( char* )( m_vpDataBasePoint ) + *m_lpReadPoint * m_nSize,
		m_nSize
	);
	( *m_lpReadPoint )++;			// Read Point Move
	( *m_lpDataCount )--;			// Data Count Decrease

	if( m_nCount == *m_lpReadPoint )	// Circle Queue Read Point Move
		*m_lpReadPoint = 0;

//	m_hCritical.Release( );

	long nState = ReleaseMutex( m_hMutex );	// Mutex Release

	if( 0 == nState )			// Mutex Release Error
		return dERROR;

	return m_nSize;				// Read Size Return;
}



//-----------------------------------------------------------------------------
//	Queue Data Write Sync
//
long CQueue::Write( void* vpWrite )
{
	long nState;

	if( 0 == long( m_lpDataCount ) )
		return dERROR;

	if( m_nCount == *m_lpDataCount )	// Queue Overflow
		return dERROR;

	if( 0 == long( m_hMutex ) )
		return dERROR;

	WaitForSingleObject( m_hMutex, INFINITE ); // Mutex Wait

//	m_hCritical.Lock( );

	if( m_nCount == *m_lpDataCount ){	// Queue Overflow
		nState = ReleaseMutex( m_hMutex );
		if( 0 == nState )
			return dERROR;

		return dERROR;
	}


	// Queue Data Write
	memcpy( ( char* )( m_vpDataBasePoint ) + *m_lpWritePoint * m_nSize,
		( char* )( vpWrite           ) ,
		m_nSize
	);
	( *m_lpWritePoint ) ++;			// Write Point Move
	( *m_lpDataCount  ) ++;			// Data Count Increase

	if( m_nCount == *m_lpWritePoint )	// Circle Queue Write Point Move
		*m_lpWritePoint = 0;

//	m_hCritical.Release( );

	nState = ReleaseMutex( m_hMutex );	// Mutex Release

	if( 0 == nState )
		return dERROR;

	nState = SetEvent( m_hEvent );		// Queue Read Request

	if( 0 == nState )
		return dERROR;

	return m_nSize;				// Read Size Return;
}



//-----------------------------------------------------------------------------
//	Get Queue Data Count
//
long CQueue::GetState( void )
{
	return  *m_lpDataCount;
}