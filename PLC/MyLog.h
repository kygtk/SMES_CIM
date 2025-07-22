//-----------------------------------------------------------------------------
// Log File Class
//
#pragma once
//#define	MAX_LOG_KEEP_DAY					 15

#define MAX_LOG_HS_KIND						 2
#define MAX_LOG_SUB_NAME					 5
#define MAX_LOG_DEFAULT_NO					 5
#define HS_LOG_CFG_START_POS				 4
#define MAX_LOG_HANDSHAKE_NO				(MAX_PHOTO_MODULE_CNT +1) * MAX_LOG_HS_KIND

#define MAX_TITLE_NO		2 // Action Log, HandShake
#define MAX_TITLE_NAME		130 // 65*2 

#define HS_LOG_PATH	"D:/FPDCIM/LOG/HANDSHAKE/"

#define LIMIT_FILE_SIZE		(10485760)		// 10 MB // shseo 2010_1011 add 

enum eLogDefaultModule
{
	eLog_DEFAULT_PLC		= 0	,
	eLog_DEFAULT_GUI		= 1	,
	eLog_DEFAULT_MELSEC		= 2	,
	eLog_DEFAULT_ACTION		= 3	,
	eLog_DEFAULT_END		= 4 ,
};

enum eLogKind
{
	eLogPLC = 0,
	eLogHS  = 1,
	eLogACTION =2,
};

#include <atlbase.h>
#include	"../include/ConstDefine.h"

class CMyLog
{
public:

	long	m_nLogID;
	long	m_nIndexFileId;

	long	m_hIndexFile;
	long	m_hDataFile;
	long	m_LogFolderNo;

//	long	m_nKeepDay;
	char	m_szLogFilePath[255];
	char	m_szLogFilePrefix[32];
	char	m_szLogFileExt[32];
	char	m_szLogIndexFileName[32];
	char	m_szIndexFileName[1024];
	char	m_szLogTitleName[32];
	char	m_szOldFileName[1024];		//
	char	m_szLogTitle[500];			// shseo chg

	char	m_szEQPID[13];				//
	char	m_szModuleName[5];			// 

	char	m_szFileName[1024];			// shseo 2010_1024 add

	struct stIndexTableType 
	{
		short	sYear;
		short	sMonth;
		short	sDay;
		short	sHour;
		short	sMin;
	} m_staIndex;

public :
	void AddLog(char *pLog, ...);

	long CreateDir(char* Path);
	void SetLogTitle();
	void SetLogConfig(long nEQType, long nModule, long nLogKind);

	void SetMSCPrefixName(char *szEQPID, char *szModuleName);

	int GetFileSize(char *path);// shseo 2010_1024 add 

	CMyLog();//(long nIndexFileId, long nUnitID);
	~CMyLog( );	
};


//================================================================
// shseo 2010_1011 add 
/*
struct stInfomationLogType	
{
	char szModuleName[MAX_LAYER_MODULE_ID_LEN+1];
	char szUnitName[MAX_UNIT_ID_LEN+1];
	char szLogType[MAX_LOG_TYPE_LEN+1];
	char szStepID[MAX_LOG_STEP_ID_LEN+1];
	char szFromPosUnitName[MAX_FROM_POSITION_LEN+1];
	char szToPosUnitName[MAX_TO_POSITION_LEN+1];
	char szPPID[MAX_PPID_LEN+1];
	char szHPanelID[MAX_PANEL_ID_LEN+1];
	char szInformation[255];
};
*/



struct stInfomationLogType	// shseo 2010_1007 - add
{
	//char szModuleName[MAX_LAYER_MODULE_ID_LEN+1];
	char szUnitName[MAX_UNIT_ID_LEN+1]; // 4
	char szLogType[MAX_LOG_TYPE_LEN+1]; // 4
	char szStepID[MAX_LOG_STEP_ID_LEN+1]; //8
	//char szFromPosUnitName[MAX_FROM_POSITION_LEN+1];
	//char szToPosUnitName[MAX_TO_POSITION_LEN+1];
	char szPPID[MAX_PPID_LEN+1]; // 16
	char szHPanelID[MAX_PANEL_ID_LEN+1]; // 12 
	char szInformation[255];
};

#define	INFORMATION_LOG_FILE_PATH		"D:\\DATA\\EQP\\EventLog\\"		// use
#define INFORMATION_LOG_FILE_EXTENSION	"log"							// use 

class CInfoLog  
{
public:
	CInfoLog();
	virtual ~CInfoLog();
	
	void SetConfig(char *szPath, char *szPrefix, char *szFileExt);
	void SetTitle();
	void Add(char *szLog);
	void AddInfo(stInfomationLogType *pInfoLogType);

	int GetFileSize(char *path);// shseo 2010_1024 add 
protected: 
	long CreateDir(char* Path);
	
	long	m_hDataFile;
	
	char	m_szFilePath[255];
	char	m_szFilePrefix[32];
	char	m_szFileExt[32];
	
	char	m_szFilePathName[1024];// shseo 2010_1024 add 
	char	m_szLogTitle[500];			// shseo chg
	
	CComAutoCriticalSection LogLock;
	
};
