#include	"MyLog.h"

#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <time.h>
#include <math.h>

CMyLog::CMyLog()//long nIndexFileId, long nUnitID)
{
	m_nLogID	=	0;
	memset(m_szLogFilePath		, 0x00, sizeof(m_szLogFilePath));
	memset(m_szLogFilePrefix	, 0x00,	sizeof(m_szLogFilePrefix));
	memset(m_szLogFileExt		, 0x00, sizeof(m_szLogFileExt));
	memset(m_szLogIndexFileName	, 0x00, sizeof(m_szLogIndexFileName));
//	m_nKeepDay = MAX_LOG_KEEP_DAY;
}

CMyLog::~CMyLog()
{
}

long CMyLog::CreateDir(char* Path)
{
	char DirName[256];
	char* p = Path;
	char* q = DirName; 
	long	rtn =0;
	
	while(*p)
	{
		if (('\\' == *p) || ('/' == *p))
		{
			if (':' != *(p-1))
			{
				rtn = CreateDirectory(DirName, NULL);
			}
		}
		*q++ = *p++;
		*q = '\0';
	}
	rtn = CreateDirectory(DirName, NULL);
	
	return rtn;
}


void CMyLog::SetLogConfig(char *szLogPath, char *szPrefix, char *szFileExt, char *szLogIndexFile)
{
	long nLen	= 0;
	long nRval	= 0;

	// SonJaeWon
	memset(m_szLogFilePath		, 0x00, sizeof(m_szLogFilePath));
	memset(m_szLogFilePrefix	, 0x00,	sizeof(m_szLogFilePrefix));
	memset(m_szLogFileExt		, 0x00, sizeof(m_szLogFileExt));
	memset(m_szLogIndexFileName	, 0x00, sizeof(m_szLogIndexFileName));

	nLen = strlen(szLogPath);
	memcpy(m_szLogFilePath, szLogPath, nLen);

	nLen	=	strlen(szPrefix);
	memcpy(m_szLogFilePrefix, szPrefix, nLen);

	nLen	=	strlen(szFileExt);
	memcpy(m_szLogFileExt, szFileExt, nLen);

	nLen	=	strlen(szLogIndexFile);
	memcpy(m_szLogIndexFileName, szLogIndexFile, nLen);

// 	if ( nKeepDay <= MAX_LOG_KEEP_DAY )
// 		m_nKeepDay = MAX_LOG_KEEP_DAY;
// 	else
// 		m_nKeepDay = nKeepDay;

	// Index File Create
	sprintf( m_szIndexFileName, "%s%s", m_szLogFilePath, m_szLogIndexFileName);

	nRval = CreateDir(m_szLogFilePath);
	
	m_hIndexFile = open( m_szIndexFileName, O_RDONLY | O_BINARY, 0777 );
	memset( &m_staIndex, 0x00, sizeof( m_staIndex ) );

	if( m_hIndexFile != -1 ) 
	{
		read( m_hIndexFile, &m_staIndex, sizeof( m_staIndex ) );
		close( m_hIndexFile );
	}
}

void CMyLog::SetLogConfig(char *szLogPath, char *szPrefix, char *szFileExt, char *szLogIndexFile, long nKeepDay)
{
	long nLen	= 0;
	long nRval	= 0;

	nLen = strlen(szLogPath);
	memcpy(m_szLogFilePath, szLogPath, nLen);

	nLen	=	strlen(szPrefix);
	memcpy(m_szLogFilePrefix, szPrefix, nLen);

	nLen	=	strlen(szFileExt);
	memcpy(m_szLogFileExt, szFileExt, nLen);

	nLen	=	strlen(szLogIndexFile);
	memcpy(m_szLogIndexFileName, szLogIndexFile, nLen);

	if ( nKeepDay <= MAX_LOG_KEEP_DAY )
		m_nKeepDay = MAX_LOG_KEEP_DAY;
	else
		m_nKeepDay = nKeepDay;

	// Index File Create
	sprintf( m_szIndexFileName, "%s%s", m_szLogFilePath, m_szLogIndexFileName);

	nRval = CreateDir(m_szLogFilePath);
	
	m_hIndexFile = open( m_szIndexFileName, O_RDONLY | O_BINARY, 0777 );
	memset( &m_staIndex, 0, sizeof( m_staIndex ) );

	if( m_hIndexFile != -1 ) 
	{
		read( m_hIndexFile, &m_staIndex, sizeof( m_staIndex ) );
		close( m_hIndexFile );
	}
}

// System의 날짜를 얻는다.
char* CMyLog::GetSystemDate()
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);

	WORD	wYear	=	systime.wYear;
	WORD	wMonth	=	systime.wMonth;
	WORD	wDay	=	systime.wDay;
	
	sprintf(m_szDate, "%04d-%02d-%02d", wYear, wMonth, wDay);
	return m_szDate;
}

// System의 날짜를 얻는다.
char* CMyLog::GetSystemHour()
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	
	WORD	wHour	=	systime.wHour;

	sprintf(m_szHour, "%02d시", wHour);
	return m_szHour;
}

// System의 시간을 얻는다.
char* CMyLog::GetSystemTime()
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	
	WORD	wHour	=	systime.wHour;
	WORD	wMin	=	systime.wMinute;
	WORD	wSec	=	systime.wSecond;
	WORD	wMMSec	=	systime.wMilliseconds;
	
	sprintf(m_szTime, "%02d:%02d:%02d", wHour, wMin, wSec);

	return m_szTime;
}

void CMyLog::AddLog(char *pLog, ...)
{
//	LogLock.Lock();	

	SYSTEMTIME systime;
	GetLocalTime(&systime);

	WORD	wYear	=	systime.wYear;
	WORD	wMonth	=	systime.wMonth;
	WORD	wDay	=	systime.wDay;
	WORD	wHour	=	systime.wHour;
	WORD	wMin	=	systime.wMinute;
	WORD	wSec	=	systime.wSecond;
	WORD	wMMSec	=	systime.wMilliseconds;

	char	szFileName[1024] = { NULL, };
	char	msg[2048];

	m_staIndex.sYear	= wYear;
	m_staIndex.sMonth	= wMonth;
	m_staIndex.sDay		= wDay;
	m_staIndex.sHour	= wHour;

	sprintf(szFileName, "%s%s_%04d%02d%02d.%s", 
		m_szLogFilePath,
		m_szLogFilePrefix, 
		m_staIndex.sYear,
		m_staIndex.sMonth,
		m_staIndex.sDay,
		m_szLogFileExt);

	if (m_nLogType == eLog_SVID)
	{
		sprintf(szFileName, "%s%s_%04d%02d%02d_%02d.%s", 
			m_szLogFilePath,
			m_szLogFilePrefix, 
			m_staIndex.sYear,
			m_staIndex.sMonth,
			m_staIndex.sDay,
			m_staIndex.sHour,
			m_szLogFileExt);
	}

	if (m_nLogType == eLog_RUNDATA)
	{
		sprintf(szFileName, "%s%s_%04d%02d.%s", 
			m_szLogFilePath,
			m_szLogFilePrefix, 
			m_staIndex.sYear,
			m_staIndex.sMonth,
			m_szLogFileExt);
	}

	WIN32_FIND_DATA FindFileData;
    HANDLE hFind;
	
    hFind = FindFirstFile(szFileName, &FindFileData);
	FindClose(hFind);

	m_hDataFile = open( szFileName, O_WRONLY | O_BINARY | O_CREAT, 0777 );
	lseek( m_hDataFile, 0L, SEEK_END );

	if(hFind == INVALID_HANDLE_VALUE && m_szLogTitle != NULL)
    {
		switch(m_nLogType)
		{
			case eLOG_EPDDATA:
			case eLOG_ENDDATA:
			case eLOG_ALARM:
			case eLog_SVID:
			case eLog_JUDGEMENT:
			case eLog_RUNDATA:
				write( m_hDataFile, m_szLogTitle, strlen(m_szLogTitle) );
				write( m_hDataFile, "\r\n", strlen("\r\n") );
				break;

			default:
				break;
		}
    }
	
	char	buff[128] = { NULL, };
	//sprintf(buff, "%04d/%02d/%02d - %02d:%02d:%02d.%03d ", wYear, wMonth, wDay, wHour, wMin, wSec, wMMSec);
	//write( m_hDataFile, buff, strlen(buff) );

	va_list stList;
	va_start(stList, pLog);
	vsprintf(msg, pLog, stList);
	va_end(stList);
	write( m_hDataFile, msg, strlen( msg ) );

	sprintf(buff, "\r\n");
	write( m_hDataFile, buff, strlen(buff) );

	close( m_hDataFile );

	//LogLock.Unlock();

	return;
} 

void CMyLog::SetLogTitle(long nLogType, char *szTitles)
{
	switch(nLogType)
	{
		case eLOG_EPDDATA:
		case eLOG_ENDDATA:
		case eLOG_ALARM:
		case eLog_SVID:
		case eLog_JUDGEMENT:
		case eLog_RUNDATA:
			strcpy(m_szLogTitle, szTitles);
			m_nLogType=nLogType;
			break;
		default:
			break;
	}
}


/*
int WINAPI WinMain(
  HINSTANCE hInstance,      // handle to current instance
  HINSTANCE hPrevInstance,  // handle to previous instance
  LPSTR lpCmdLine,          // command line
  int nCmdShow              // show state
)
{
	CMyLog	htcLog(1);
	CMyLog	cicLog(2);
	CMyLog	pscLog(3);

	htcLog.AddLog("Create htc log");
	cicLog.AddLog("Create cic log");
	pscLog.AddLog("Create psc log");
	return 0;
}
*/

char* CMyLog::GetLogTitle()
{
	return m_szLogTitle;
}

char* CMyLog::GetEPDLogTitle()
{
	return m_szEPDLogTitle;
}

char* CMyLog::GetEndDataLogTitle()
{
	return m_szEndDataLogTitle;
}
