
//
//	Shared Memory Class Library
//
#pragma once

#include	"SW3FaLib.h"
#include	<stdio.h>
//-----------------------------------------------------------------------------
//	Constructor
//

CShMem::CShMem( )
{
	m_hShmem = HANDLE( 0 );
	m_hFile  = HANDLE( 0 );

	m_szShmemName = (char *)0;
	m_szPath	  = (char *)0;

	m_vpAddress = (void*)0;
}



//-----------------------------------------------------------------------------
//	Destructor
//
CShMem::~CShMem( )
{
	if( 0 != long( m_hShmem ) )
		CloseHandle( m_hShmem    );

	if( 0 != long( m_hFile ) )
		CloseHandle( m_hFile     );

	if( m_szShmemName )
		delete [] m_szShmemName;

	if( m_szPath )
		delete [] m_szPath;
}



//-----------------------------------------------------------------------------
//	Create Shared Memory
//
long CShMem::Create( char* szShmemName, long nSize, long* lpAddress, char* szPath )
{
	m_nSize = nSize;

	if( m_szShmemName )
		delete [] m_szShmemName;

	if( m_szPath )
		delete [] m_szPath;

	if( 0 != szShmemName )
	m_szShmemName	= new char [ strlen( szShmemName) + 1 ];
	else return dERROR;

	if( 0 != szPath )
	m_szPath		= new char [ strlen( szPath		) + 1 ];


	strcpy( m_szShmemName, szShmemName );

	if( 0 != szPath )
	strcpy( m_szPath, szPath );


	if( (char*)NULL == szPath ) {// Only Memory Require ?
		m_hFile = HANDLE(-1);
		goto gotoCreateFileMapping;
	}


	//
	//	Memory Mapped File
	//
	if( 0 == long( m_hFile ) ){
		char	szFileName[ 255 ];

		sprintf( szFileName, "%s\\%s", szPath, szShmemName );

		m_hFile = CreateFile(
			szFileName,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			CREATE_NEW | OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);
	}

	if( INVALID_HANDLE_VALUE == m_hFile )
		return dERROR;

	//
	//	Memory Map
	//
gotoCreateFileMapping:

	// Shared Memory Create or Mapping
	if( 0 == long( m_hShmem ) )
        m_hShmem = CreateFileMapping(	// Create Shared Memory
			m_hFile,
			NULL,
			PAGE_READWRITE,
            NULL,
            nSize,
            szShmemName
		);


	if( NULL == long( m_hShmem ) )
		m_hShmem = OpenFileMapping(
			FILE_MAP_READ | FILE_MAP_WRITE,
			FALSE,
			szShmemName
		);

	if( NULL == long( m_hShmem ) )
		return dERROR;

	if( 0 == long( m_vpAddress ) )
	m_vpAddress = (void*)MapViewOfFile(
			m_hShmem,
			FILE_MAP_WRITE | FILE_MAP_READ,
			NULL,
			NULL,
			NULL
		);

	if( 0 == long( m_vpAddress ) )
		return dERROR;

	*lpAddress = (long)m_vpAddress;

	return dSUCCESS;
}



//-----------------------------------------------------------------------------
//	Open Shared Memory
//
long CShMem::Open( char* szShmemName, long nSize, long* lpAddress )
{
	m_nSize = nSize;

	if( m_szShmemName )
		delete [] m_szShmemName;

	m_szShmemName = new char [ strlen( szShmemName ) + 1 ];

	strcpy( m_szShmemName, szShmemName );


	if( 0 == long( m_hShmem ) )
	m_hShmem = OpenFileMapping(
			FILE_MAP_READ | FILE_MAP_WRITE,
			FALSE,
			szShmemName
		);

	if( NULL == long( m_hShmem ) )
		return dERROR;



	if( 0 == long( m_vpAddress ) )
	m_vpAddress = MapViewOfFile(
			m_hShmem,
			FILE_MAP_WRITE | FILE_MAP_READ,
			NULL,
			NULL,
			NULL
		);


	if( 0 == long( m_vpAddress ) )
		return dERROR;

	*lpAddress = (long)m_vpAddress;

	return dSUCCESS;
}



//-----------------------------------------------------------------------------
//	Free Shared Memory
//
long CShMem::Free( )
{
	if( 0 != long( m_vpAddress ) && 0 == UnmapViewOfFile( m_vpAddress ) )
		return dERROR;

	if( 0 != long( m_hShmem    ) && 0 == CloseHandle    ( m_hShmem    ) )
		return dERROR;

	if( 0 != long( m_hFile     ) && 0 == CloseHandle    ( m_hFile     ) )
		return dERROR;

	return dSUCCESS;
}