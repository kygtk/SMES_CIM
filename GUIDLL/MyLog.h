//-----------------------------------------------------------------------------
// Log File Class
//
#include <atlbase.h>
#pragma once
#define	MAX_LOG_KEEP_DAY	30
#define TITLE_BUFFER_SIZE	32768

#define GUI_LOG_FILE_EXTENSION "csv"
#define GUI_LOG_INDEX_FILENAME "GUILOG.INX"
#define ALARM_LOG_FILE_PATH "D:\\FPDCIM\\LOG\\COURSE\\GUI\\ALARM\\"
#define GLASSDATA_LOG_FILE_PATH "D:\\FPDCIM\\LOG\\COURSE\\GUI\\STARTGLASSDATA\\"
#define ENDDATA_LOG_FILE_PATH "D:\\FPDCIM\\LOG\\COURSE\\GUI\\ENDDATA\\"
#define TACTTIME_LOG_FILE_PATH "D:\\FPDCIM\\LOG\\COURSE\\GUI\\TACTTIME\\"
#define SCRAP_UNSCRAP_LOG_FILE_PATH "D:\\FPDCIM\\LOG\\COURSE\\GUI\\SCRAP_UNSCRAP\\"
#define EVENT_MESSAGE_LOG_FILE_PATH "D:\\FPDCIM\\LOG\\COURSE\\GUI\\EVENTMSG\\"
#define TERMINAL_MESSAGE_LOG_FILE_PATH "D:\\FPDCIM\\LOG\\COURSE\\GUI\\TERMMSG\\"
#define RECIPEEDITHISTORY_LOG_FILE_PATH "D:\\FPDCIM\\LOG\\COURSE\\GUI\\RECIPEHISTORY\\"
#define EPDDATA_LOG_FILE_PATH "D:\\FPDCIM\\LOG\\EPDLOG\\"
#define JUDGEMENT_LOG_FILE_PATH	"D:\\FPDCIM\\LOG\\COURSE\\GUI\\JUDGEMENT\\"

#define CHEMICAL_LOG_FILE_PATH "D:/FPDCIM/LOG/COURSE/GUI/CHEMICAL_CHANGE/"

#define SVID_LOG_FILE_PATH "D:\\FPDCIM\\LOG\\COURSE\\GUI\\SVID\\"
#define RUNDATA_LOG_FILE_PATH "D:\\FPDCIM\\LOG\\RUNDATA\\"
#define TEMPDATA_LOG_FILE_PATH "D:/FPDCIM/LOG/TEMP/"

#define RPC_LOG_FILE_PATH "D:\\FPDCIM\\LOG\\COURSE\\GUI\\RPC_EVENT\\"

enum eLogType
{
	eLOG_EPDDATA	=1,
	eLOG_ENDDATA	=2,
	eLOG_ALARM		=3,
	eLog_SVID		=4,
	eLog_RPC		=5,
	eLog_JUDGEMENT  = 6,
	eLog_RUNDATA	= 7,
};

class CMyLog 
{
protected:

	long	m_nLogID;
	long	m_nIndexFileId;

	long	m_hIndexFile;
	long	m_hDataFile;

	long	m_nKeepDay;
	char	m_szLogFilePath[255];
	char	m_szLogFilePrefix[32];
	char	m_szLogFileExt[32];
	char	m_szLogIndexFileName[32];

	char	m_szIndexFileName[1024];

	struct stIndexTableType 
	{
		short	sYear;
		short	sMonth;
		short	sDay;
		short	sHour;
		short	sMin;
	} m_staIndex;
	
	char m_szEPDLogTitle[TITLE_BUFFER_SIZE];
	char m_szEndDataLogTitle[TITLE_BUFFER_SIZE];
	char m_szLogTitle[TITLE_BUFFER_SIZE];

	long m_nLogType;
	
public :
	char* GetEndDataLogTitle();
	char* GetEPDLogTitle();
	char* GetLogTitle();

	void SetLogTitle(long nLogType, char* szTitles);
	void SetLogConfig(char *szLogPath, char *szPrefix, char *szFileExt, char *szLogIndexFile);
	void SetLogConfig(char *szLogPath, char *szPrefix, char *szFileExt, char *szLogIndexFile, long nKeepDay);
	void AddLog(char *pLog, ...);
	long CMyLog::CreateDir(char* Path);
	
	char m_szDate[20];
	char m_szTime[10];
	char m_szHour[5];

	char* CMyLog::GetSystemDate();
	char* CMyLog::GetSystemTime();
	char* CMyLog::GetSystemHour();

	CComAutoCriticalSection LogLock;


	CMyLog();//(long nIndexFileId, long nUnitID);
	~CMyLog( );	
};
