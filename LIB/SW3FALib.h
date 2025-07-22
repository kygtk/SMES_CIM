#ifndef		_SW3FALIB_H_
#define		_SW3FALIB_H_

#pragma once
#include	<windows.h>

//-----------------------------------------------------------------------------
//	Shared Memory Class Type Define
//
#define DLL_SET 1
#define DLL_GET 2

#define dERROR		-1
#define dSUCCESS	1
#define dGUIERROR	0

class CShMem 
{
protected :
	HANDLE	m_hFile      ;
	HANDLE	m_hShmem     ;
	long 	m_nSize      ;
	char*	m_szShmemName;
	char*	m_szPath     ;
	LPVOID	m_vpAddress  ;

public    :
	CShMem ( void );
	~CShMem( void );

	long	Create( char* szShmemName, long nSize, long* vpAddress, char* szPath = (char *)NULL );
	long	Open  ( char* szShmemName, long nSize, long* vpAddress  );
	long	Free  ( void );
};

//-----------------------------------------------------------------------------
//	Queue Class Type Define
//
class CQueue  
{
public:
	CQueue();
	CQueue    ( char* szQueueName, long nCount, long nSize );
	virtual ~CQueue();

protected :
	HANDLE	m_hShmem;
	HANDLE	m_hEvent;
	HANDLE	m_hMutex;
//	CfaCriticalSection	m_hCritical;
	char*	m_szQueueName;
	long	m_nCount;
	long	m_nSize;
	long*	m_lpDataCount;
	long*	m_lpReadPoint;
	long*	m_lpWritePoint;
	void*	m_vpDataBasePoint;

public:
	long  Create  ( char* szQueueName, long nCount, long nSize );
	long  Delete  ( void );
	long  Write   ( void* vpWrite   );
	long  Read    ( void* vpRead    );
	long  GetState( void );
};

#endif // _SW3FALIB_H_