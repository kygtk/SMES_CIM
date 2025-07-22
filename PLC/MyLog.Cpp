
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <time.h>

#include "../include/Enum.h"
#include "MyLog.h"
#include "MainPlc.h"

CComAutoCriticalSection LogLock;

CMyLog::CMyLog()//long nIndexFileId, long nUnitID)
{
	m_nLogID	=	0;

	m_LogFolderNo = 0;

	memset(m_szLogFilePath		, 0x00, sizeof(m_szLogFilePath));
	memset(m_szLogFilePrefix	, 0x00,	sizeof(m_szLogFilePrefix));
	memset(m_szLogFileExt		, 0x00, sizeof(m_szLogFileExt));
	memset(m_szLogIndexFileName	, 0x00, sizeof(m_szLogIndexFileName));
	memset(m_szLogTitleName		, 0x00, sizeof(m_szLogTitleName));
	memset(m_szOldFileName		, 0x00, sizeof(m_szOldFileName));

	memset(m_szEQPID			, 0x00, sizeof(m_szEQPID));
	memset(m_szModuleName		, 0x00, sizeof(m_szModuleName));

	memset(m_szFileName		, 0x00, sizeof(char)*1024); // shseo 2010_1024 add 
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


int CMyLog::GetFileSize(char *path)
{
	struct stat file_stats; 

	if( stat(path, &file_stats) )
		return -1; 

	return file_stats.st_size; 

}

void CMyLog::AddLog(char *pLog, ...)
{
	LogLock.Lock();

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
	char	msg[5000]	=	{NULL,};
	char	szDefTime[5] = {NULL,};
	int		sz; 


	m_staIndex.sYear		= wYear;
	m_staIndex.sMonth		= wMonth;
	m_staIndex.sDay			= wDay;
	m_staIndex.sHour		= wHour;

	/*
	if(strcmp(m_szLogFilePrefix,"MSC") == 0)
	{
		if(wHour <12) strcpy(szDefTime,"AM");
		else strcpy(szDefTime,"PM");
	}
	else
	{
		_itoa(wHour,szDefTime,10);
	}
	*/
	_itoa(wHour,szDefTime,10);

        // shseo 2010_1007
	if(strcmp(m_szLogTitleName,"LOG_TITLE_ACTION") == 0)
	{
//		sprintf(szFileName, "%s%s_%s_%04d%02d%02d.%s", 
//			m_szLogFilePath, m_szEQPID, m_szModuleName,
		sprintf(szFileName, "%s%s_%04d%02d%02d.%s", 
			m_szLogFilePath, m_szEQPID,
			m_staIndex.sYear,
			m_staIndex.sMonth,
			m_staIndex.sDay,		
			m_szLogFileExt					);

		// shseo 2010_1024 add 
		sz = GetFileSize(szFileName);
		if( sz > LIMIT_FILE_SIZE) 
		{
			if( strcmp(m_szFileName, "") == 0 ) 
			{
				sprintf(m_szFileName, "%s%s_%04d%02d%02d_%02d.%s", 
					m_szLogFilePath, m_szEQPID,
					m_staIndex.sYear,
					m_staIndex.sMonth,
					m_staIndex.sDay,		
					m_staIndex.sHour, 
					m_szLogFileExt);
				SetLogTitle(); 
			}

			sz = GetFileSize(m_szFileName);
			if( sz > LIMIT_FILE_SIZE) 
			{
				sprintf(m_szFileName, "%s%s_%04d%02d%02d_%02d.%s", 
					m_szLogFilePath, m_szEQPID,
					m_staIndex.sYear,
					m_staIndex.sMonth,
					m_staIndex.sDay,		
					m_staIndex.sHour, 
					m_szLogFileExt);
				SetLogTitle(); 
			}
			strcpy(szFileName, m_szFileName); 
		}
		else
		{
			if( strcmp(m_szFileName, "") != 0 ) 
			{
				SetLogTitle(); 
			}
			memset(m_szFileName , 0x00, sizeof(char)*1024);
		}


	}
	else
	{
	sprintf(szFileName, "%s%s_%04d%02d%02d.%s", 
			m_szLogFilePath, m_szLogFilePrefix,
			m_staIndex.sYear,
			m_staIndex.sMonth,
			m_staIndex.sDay,		
			m_szLogFileExt					);
	}

	WIN32_FIND_DATA FindFileData;
    HANDLE hFind;
	
    hFind = FindFirstFile(szFileName, &FindFileData);
    FindClose(hFind);
	
	m_hDataFile = open( szFileName, O_WRONLY | O_BINARY | O_CREAT, 0777 );
	lseek( m_hDataFile, 0L, SEEK_END );

	if(hFind == INVALID_HANDLE_VALUE && m_szLogTitle != NULL)
    {
		write( m_hDataFile, m_szLogTitle, strlen(m_szLogTitle) );
    }

	char	buff[128] = { NULL, };
 	
    // shseo 2010_1007
 	if(strcmp(m_szLogTitleName,"LOG_TITLE_ALL_HANDSHAKE") != 0 && strcmp(m_szLogTitleName, "LOG_TITLE_ACTION") !=0)
	//if(strcmp(m_szLogTitleName, "LOG_TITLE_ALL_HANDSHAKE") != 0)
	{
		sprintf(buff, "[%02d:%02d:%02d.%03d]", wHour, wMin, wSec, wMMSec);
 		write( m_hDataFile, buff, strlen(buff) );
	}


	va_list stList;
	va_start(stList, pLog);
	vsprintf(msg, pLog, stList);
	va_end(stList);
	write( m_hDataFile, msg, strlen( msg ) );

    // shseo 2010_1007
	if(strcmp(m_szLogTitleName,"LOG_TITLE_ALL_HANDSHAKE") != 0 && strcmp(m_szLogTitleName, "LOG_TITLE_ACTION") !=0)
	{		
	sprintf(buff, "\r\n");
	write( m_hDataFile, buff, strlen(buff) );
	}

//	sprintf(buff, "\r\n");
//	write( m_hDataFile, buff, strlen(buff) );

	close( m_hDataFile );

	LogLock.Unlock();

	return;
}

void CMyLog::SetLogTitle()
{
	char LogTitle1[1000] = {NULL,};
	char LogTitle2[1000] = {NULL,};

	char *LogContent[MAX_TITLE_NO][MAX_TITLE_NAME] =
	{
		// Action Log Title, 9개
		{
			"Date",		 "Time",	"Module",	"Log_Type",
			"Action_ID", "Departure",	 "Destination",	  "PPID",	"GlassID"
		},
		
		// HandShake Title
		{	// SEND						// RECEIVE
			"[TITLE][SEND]"			,	"[TITLE][RECV]"			,
			"HeartBeat"				,	"HeartBeat"			,			
			"Pause"					,	"Pause"					,
			"Abnormal"				,	"Abnormal"					,
			"Reserved"					,	"Reserved"					,
			"SendAble"				,	"RecvAble"				,
			"SendStart"				,	"RecvStart"				,
			"SendComplete"			,	"RecvComplete"			,
			"ImmPauseReq"			,	"ImmPauseReq"			,
			"ReturnRecvStart"		,	"ReturnSendStart"		,
			"ReturnRecvComplete"	,	"ReturnSendComplete"	,
			"ExchangeFlag"			,	"ExchangeFlag"			,
			"MultiCarryFlag"		,	"MultiCarryFlag"		,
			"Reserved"				,	"Reserved"				,
			"Reserved"				,	"Reserved"				,
			"Reserved"				,	"LoadingStop"			,
			"Reserved"				,	"TransferStop"			,
			"PreAction1"			,	"PreAction1"			,
			"PreAction2"			,	"PreAction2"			,
			"MidAction1"			,	"MidAction1"			,
			"MidAction2"			,	"MidAction2"			,
			"PostAction1"			,	"PostAction1"			,
			"Reserved"				,	"Reserved"				,
			"Reserved"				,	"ReceiveRefuse"			,
			"Reserved"			,	"Reserved"	,
			"Reserved"				,	"Reserved"			,
			"Reserved"				,	"Reserved"			,
			"HSResumeReq"			,	"HSResumeReq"			,
			"HSRecoveryAck"			,	"HSRecoveryAck"			,
			"HSRecoveryNak"			,	"HSRecoveryNak"			,
			"Reserved"				,	"Reserved"				,
			"Initial"				,	"Initial"				,
			"Error"					,	"Error"					,

			"Abnormal"				,	"Abnormal"				,
			"TypeofArm"				,	"TypeofArm"				,
			"TypeofStage"			,	"TypeofStage"			,
			"ManualOp"				,	"ManualOp"				,
			"Safety"				,	"Safety"				,
			"Empty"					,	"Empty"					,
			"Wait"					,	"Wait"					,
			"Busy"					,	"Busy"					,
			"Pause"					,	"Pause"					,
			"Reserved"				,	"Reserved"				,
			"Arm1Violate"			,	"Arm1Violate"			,
			"Arm2Violate"			,	"Arm2Violate"			,
			"Arm1FoldComplete"		,	"Arm1FoldComplete"		,
			"Arm2FoldComplete"		,	"Arm2FoldComplete"		,
			"Arm1GlassCheck"		,	"Arm1GlassCheck"		,
			"Arm2GlassCheck"		,	"Arm2GlassCheck"		,
			"RobotDirection"		,	"RobotDirection"		,
			"Reserved"				,	"Reserved"				,
			"Reserved"				,	"Reserved"				,
			"Reserved"				,	"Reserved"				,
			"LiftUp"				,	"LiftUp"				,
			"LiftDown"				,	"LiftDown"				,
			"StopperUp"				,	"StopperUp"				,
			"StopperDown"			,	"StopperDown"			,
			"DoorOpen"				,	"DoorOpen"				,
			"DoorClose"				,	"DoorClose"				,
			"GlassDetect"			,	"GlassDetect"			,
			"BodyMoving"			,	"BodyMoving"			,
			"BodyOP"				,	"BodyOP"				,
			"Reserved"				,	"Reserved"				,
			"Reserved"				,	"Reserved"				,
			"Reserved"				,	"Reserved"				
		},
	};

	memset(m_szLogTitle,0x20,sizeof(m_szLogTitle));

	if(strcmp(m_szLogTitleName,"LOG_TITLE_ACTION") == 0)
	{
		for(int i=0;i<9;i++)
		{
			strcat(LogTitle1,LogContent[0][i]);
			if(i != 8) strcat(LogTitle1,",");
		}
		
		strcpy(m_szLogTitle,LogTitle1);
		strcat(m_szLogTitle,"\r\n");
	}

	else if(strcmp(m_szLogTitleName,"LOG_TITLE_ALL_HANDSHAKE") == 0)
	{
		
		for(int i=0;i<MAX_TITLE_NAME/2;i++)
		{
			strcat(LogTitle1,LogContent[1][i*2]);
			if(i != 0 || i != 64) strcat(LogTitle1,",");
			strcat(LogTitle2,LogContent[1][i*2+1]);
			if(i != 0 || i != 64) strcat(LogTitle2,",");
		}
		
		strcpy(m_szLogTitle,LogTitle1);
		strcat(m_szLogTitle,"\r\n");
		strcat(m_szLogTitle,LogTitle2);		
		strcat(m_szLogTitle,"\r\n");
	}

	else
	{
		strcpy(m_szLogTitle,"");
	}
}

void CMyLog::SetLogConfig(long nEQType, long nModule, long nLogKind)
{
	long nLen		= 0;
	long nRval		= 0;
	long nLogTitle	= 0;
//	long nKeepDay	= 0;
	char LogName[MAX_LOG_SUB_NAME][255];	// 5 

	char *LogDefaultName[MAX_LOG_DEFAULT_NO][MAX_LOG_SUB_NAME] =
	{																												
		// Log Path											// Index Log				// Log Name			//Ext.			// Log Title
		{"D:/FPDCIM/LOG/COURSE/PLC/00 MASTER/"			,"PLC_MasterLog.INX"		,"PLC_Master"		,"LOG"	,		"LOG_TITLE_PLC"		 }, // 00 PLC LOG
		{"D:/FPDCIM/LOG/COURSE/GUI/"					,""							,""					,""		,		"LOG_TITLE_GUI"		 }, // 01 GUI LOG
		{"D:/FPDCIM/LOG/HOST/"							,""							,""					,""		,		"LOG_TITLE_HOST"	 }, // 02 HOST LOG
		{"D:/FPDCIM/LOG/MELSEC/"						,"MELSECLog.INX"			,"MELSEC"			,"LOG"	,		"LOG_TITLE_MELSEC"   }, // 03 MELSEC LOG
		{"D:/DATA/EQP/ActionLog/"						,"ActionLog.INX"			,"MSC"				,"log"	,		"LOG_TITLE_ACTION"	 }  // 04 ACTION(MSC) LOG =>T8-2 Ph2 추가사양
	};

	char *LogHS_EtrStripName[MAX_ETCHSTRIP_MODULE_CNT * MAX_LOG_HS_KIND][MAX_LOG_SUB_NAME] =
	{
		// ETCHER & STRIPER LINE
		{"D:/FPDCIM/LOG/HANDSHAKE/02 ETCHER/ALL VIEW/UPPER/"		,"ALL_ETCHER_UPPERLog.INX"		,"ALL_ETCHER_UPPER"		,"CSV"	,		"LOG_TITLE_ALL_HANDSHAKE"}, // 00 ETCHER UPPER LOG
		{"D:/FPDCIM/LOG/HANDSHAKE/02 ETCHER/ALL VIEW/LOWER/"		,"ALL_ETCHER_LOWERLog.INX"		,"ALL_ETCHER_LOWER"		,"CSV"	,		"LOG_TITLE_ALL_HANDSHAKE"}, // 01 ETCHER LOWER LOG

		{"D:/FPDCIM/LOG/HANDSHAKE/03 STRIPER/ALL VIEW/UPPER/"		,"ALL_STRIPER_UPPERLog.INX"		,"ALL_STRIPER_UPPER"	,"CSV"	,		"LOG_TITLE_ALL_HANDSHAKE"}, // 02 STRIPER UPPER LOG
		{"D:/FPDCIM/LOG/HANDSHAKE/03 STRIPER/ALL VIEW/LOWER/"		,"ALL_STRIPER_LOWERLog.INX"		,"ALL_STRIPER_LOWER"	,"CSV"	,		"LOG_TITLE_ALL_HANDSHAKE"}, // 03 STRIPER LOWER LOG

		{"D:/FPDCIM/LOG/HANDSHAKE/04 BYPASS/ALL VIEW/UPPER/"		,"ALL_BYPASS_UPPERLog.INX"		,"ALL_BYPASS_UPPER"		,"CSV"	,		"LOG_TITLE_ALL_HANDSHAKE"}, // 04 BYPASS UPPER LOG
		{"D:/FPDCIM/LOG/HANDSHAKE/04 BYPASS/ALL VIEW/LOWER/"		,"ALL_BYPASS_LOWERLog.INX"		,"ALL_BYPASS_LOWER"		,"CSV"	,		"LOG_TITLE_ALL_HANDSHAKE"}, // 05 BYPASS LOWER LOG
	};

	char *LogPLC_EtrStripName[MAX_ETCHSTRIP_MODULE_CNT][MAX_LOG_SUB_NAME] =
	{
		// ETCHER & STRIPER LINE
		{"D:/FPDCIM/LOG/COURSE/PLC/02 ETCHER/"		,"PLC_ETCHER_Log.INX"		,"PLC_ETCHER"		,"LOG"	,		"LOG_TITLE_PLC"}, // 00 ETCHER UPPER LOG
		{"D:/FPDCIM/LOG/COURSE/PLC/03 STRIPER/"		,"PLC_STRIPER_Log.INX"		,"PLC_STRIPER"		,"LOG"	,		"LOG_TITLE_PLC"}, // 02 STRIPER UPPER LOG
		{"D:/FPDCIM/LOG/COURSE/PLC/04 BYPASS/"		,"PLC_BYPASS_Log.INX"		,"PLC_BYPASS"		,"LOG"	,		"LOG_TITLE_PLC"}, // 04 BYPASS UPPER LOG
	};

	// shseo 2010_1007 - add 
	char *LogAction_EtrStripName[MAX_ETCHSTRIP_MODULE_CNT][MAX_LOG_SUB_NAME] =
	{
		// Action Long
		// ETCHER & STRIPER LINE
		{"D:/DATA/EQP/ActionLog/"				,"ACTION_ETCHER_Log.INX"		,"ACTION_ETCHER"		,"LOG"	,		"LOG_TITLE_ACTION"},	//00 ETCHER UPPER LOG
		{"D:/DATA/EQP/ActionLog/"			,"ACTION_STRIPER_Log.INX"		,"ACTION_STRIPER"		,"LOG"	,		"LOG_TITLE_ACTION"},	//02 STRIPER UPPER LOG
		{"D:/DATA/EQP/ActionLog/"				,"ACTION_BYPASS_Log.INX"		,"ACTION_BYPASS"		,"LOG"	,		"LOG_TITLE_ACTION"},	//04 BYPASS UPPER LOG
		// "D:/DATA/EQP/ActionLog/ETCHERSTRIP/"
//		{"D:/DATA/EQP/ActionLog/ETCHER/"				,"ACTION_ETCHER_Log.INX"		,"ACTION_ETCHER"		,"LOG"	,		"LOG_TITLE_ACTION"},	//00 ETCHER UPPER LOG
//		{"D:/DATA/EQP/ActionLog/STRIPER/"			,"ACTION_STRIPER_Log.INX"		,"ACTION_STRIPER"		,"LOG"	,		"LOG_TITLE_ACTION"},	//02 STRIPER UPPER LOG
//		{"D:/DATA/EQP/ActionLog/BYPASS/"				,"ACTION_BYPASS_Log.INX"		,"ACTION_BYPASS"		,"LOG"	,		"LOG_TITLE_ACTION"},	//04 BYPASS UPPER LOG
	};


	
	char *LogHS_ETCHERName[MAX_ETCH_MODULE_CNT * MAX_LOG_HS_KIND][MAX_LOG_SUB_NAME] =
	{
		// ETCHER LINE
		{"D:/FPDCIM/LOG/HANDSHAKE/02 ETCHER/ALL VIEW/UPPER/"		,"ALL_ETCHER_UPPERLog.INX"		,"ALL_ETCHER_UPPER"		,"CSV"	,		"LOG_TITLE_ALL_HANDSHAKE"}, // 00 ETCHER UPPER LOG
		{"D:/FPDCIM/LOG/HANDSHAKE/02 ETCHER/ALL VIEW/LOWER/"		,"ALL_ETCHER_LOWERLog.INX"		,"ALL_ETCHER_LOWER"		,"CSV"	,		"LOG_TITLE_ALL_HANDSHAKE"}, // 01 ETCHER LOWER LOG
	};

	char *LogPLC_ETCHERName[MAX_ETCH_MODULE_CNT][MAX_LOG_SUB_NAME] =
	{
		// ETCHER LINE
		{"D:/FPDCIM/LOG/COURSE/PLC/02 ETCHER/"			,"PLC_ETCHER_Log.INX"		,"PLC_ETCHER"		,"LOG"	,		"LOG_TITLE_PLC"}, // 00 ETCHER UPPER LOG
	};

	// shseo 2010_1007 - add 
	char *LogAction_ETCHERName[MAX_ETCH_MODULE_CNT][MAX_LOG_SUB_NAME] =
	{
		// Action Long
		// ETCHER LINE
		{"D:/DATA/EQP/ActionLog/"				,"ACTION_ETCHER_Log.INX"		,"ACTION_ETCHER"		,"LOG"	,		"LOG_TITLE_ACTION"},	//00 ETCHER UPPER LOG
		//D:/DATA/EQP/ActionLog/ETCHER/
	};

	char *LogHS_STRIPName[MAX_STRIP_MODULE_CNT * MAX_LOG_HS_KIND][MAX_LOG_SUB_NAME] =
	{
		// STRIPER LINE
		{"D:/FPDCIM/LOG/HANDSHAKE/03 STRIPER/ALL VIEW/UPPER/"	,"ALL_STRIPER_UPPERLog.INX"		,"ALL_STRIPER_UPPER"	,"CSV"	,		"LOG_TITLE_ALL_HANDSHAKE"}, // 02 STRIPER UPPER LOG
		{"D:/FPDCIM/LOG/HANDSHAKE/03 STRIPER/ALL VIEW/LOWER/"	,"ALL_STRIPER_LOWERLog.INX"		,"ALL_STRIPER_LOWER"	,"CSV"	,		"LOG_TITLE_ALL_HANDSHAKE"}, // 03 STRIPER LOWER LOG
	};

	char *LogPLC_STRIPName[MAX_STRIP_MODULE_CNT][MAX_LOG_SUB_NAME] =
	{
		// STRIPER LINE
		{"D:/FPDCIM/LOG/COURSE/PLC/03 STRIPER/"		,"PLC_STRIPER_Log.INX"		,"PLC_STRIPER"		,"LOG"	,		"LOG_TITLE_PLC"}, // 02 STRIPER UPPER LOG
	};
	// shseo 2010_1007 - add 
	char *LogAction_STRIPName[MAX_STRIP_MODULE_CNT][MAX_LOG_SUB_NAME] =
	{
		// Action Long
		// STRIP LINE
		{"D:/DATA/EQP/ActionLog/"				,"ACTION_STRIPER_Log.INX"		,"ACTION_STRIPER"		,"LOG"	,		"LOG_TITLE_ACTION"},	//00 ETCHER UPPER LOG
		//D:/DATA/EQP/ActionLog/STRIPER/
	};
	// PSK 080406 SD type Log 폴더 수정

	char *LogHS_PFCName[(MAX_PFC_MODULE_CNT + 1) * MAX_LOG_HS_KIND][MAX_LOG_SUB_NAME] =
	{
		// PFC LINE
		{"D:/FPDCIM/LOG/HANDSHAKE/02 PFC/ALL VIEW/UPPER/"			,"ALL_PFC_UPPERLog.INX"			,"ALL_PFC_UPPER"		,"CSV"	,		"LOG_TITLE_ALL_HANDSHAKE"}, // 00 PFC UPPER LOG
		{"D:/FPDCIM/LOG/HANDSHAKE/02 PFC/ALL VIEW/LOWER/"			,"ALL_PFC_LOWERLog.INX"			,"ALL_PFC_LOWER"		,"CSV"	,		"LOG_TITLE_ALL_HANDSHAKE"},  // 01 PFC LOWER LOG

		{"D:/FPDCIM/LOG/HANDSHAKE/03 SD1/ALL VIEW/UPPER/"			,"ALL_PFC_UPPERLog.INX"			,"ALL_PFC_UPPER"		,"CSV"	,		"LOG_TITLE_ALL_HANDSHAKE"}, // 00 PFC UPPER LOG
		{"D:/FPDCIM/LOG/HANDSHAKE/03 SD1/ALL VIEW/LOWER/"			,"ALL_PFC_LOWERLog.INX"			,"ALL_PFC_LOWER"		,"CSV"	,		"LOG_TITLE_ALL_HANDSHAKE"},  // 01 PFC LOWER LOG
	};

	char *LogPLC_PFCName[MAX_PFC_MODULE_CNT + 1][MAX_LOG_SUB_NAME] =
	{
		// PFC LINE
		{"D:/FPDCIM/LOG/COURSE/PLC/02 PFC/"				,"PLC_PFC_Log.INX"			,"PLC_PFC"			,"LOG"	,		"LOG_TITLE_PLC"}, // 00 PFC UPPER LOG
		{"D:/FPDCIM/LOG/COURSE/PLC/03 SD1/"				,"PLC_PFC_Log.INX"			,"PLC_PFC"			,"LOG"	,		"LOG_TITLE_PLC"}, // 00 PFC UPPER LOG
	};
	// shseo 2010_1007 - add 
	char *LogAction_PFCName[MAX_PFC_MODULE_CNT + 1][MAX_LOG_SUB_NAME] =
	{
		// Action Long
		// PFC LINE
		{"D:/DATA/EQP/ActionLog/"				,"ACTION_PFC_Log.INX"		,"ACTION_PFC"		,"LOG"	,		"LOG_TITLE_ACTION"},	//00 PFC LOG
		{"D:/DATA/EQP/ActionLog/"				,"ACTION_PFC_Log.INX"		,"ACTION_PFC"		,"LOG"	,		"LOG_TITLE_ACTION"}, // 00 PFC UPPER LOG
		//D:/DATA/EQP/ActionLog/PFC/
		//D:/DATA/EQP/ActionLog/SD1/
	};

	char *LogHS_EDGEName[MAX_STRIP_MODULE_CNT * MAX_LOG_HS_KIND][MAX_LOG_SUB_NAME] =
	{
		// EDGE LINE
		{"D:/FPDCIM/LOG/HANDSHAKE/02 EDGE/ALL VIEW/UPPER/"			,"ALL_EDGE_UPPERLog.INX"		,"ALL_EDGE_UPPER"		,"CSV"	,		"LOG_TITLE_ALL_HANDSHAKE"}, // 00 EDGE UPPER LOG
		{"D:/FPDCIM/LOG/HANDSHAKE/02 EDGE/ALL VIEW/LOWER/"			,"ALL_EDGE_LOWERLog.INX"		,"ALL_EDGE_LOWER"		,"CSV"	,		"LOG_TITLE_ALL_HANDSHAKE"},  // 01 EDGE LOWER LOG
	};
	char *LogPLC_EDGEName[MAX_EDGE_MODULE_CNT][MAX_LOG_SUB_NAME] =
	{
		// EDGE LINE
		{"D:/FPDCIM/LOG/COURSE/PLC/02 EDGE/"				,"PLC_EDGE_Log.INX"			,"PLC_EDGE"		,"LOG"	,		"LOG_TITLE_PLC"}, // 00 EDGE UPPER LOG
	};
	// shseo 2010_1007 - add 
	char *LogAction_EDGEName[MAX_EDGE_MODULE_CNT][MAX_LOG_SUB_NAME] =
	{
		// Action Long
		// EDGE LINE
		{"D:/DATA/EQP/ActionLog/"				,"ACTION_EDGE_Log.INX"		,"ACTION_EDGE"		,"LOG"	,		"LOG_TITLE_ACTION"},	//00 PFC LOG
		//D:/DATA/EQP/ActionLog/EDGE/
	};

	char *LogHS_WRUName[MAX_WRU_MODULE_CNT * MAX_LOG_HS_KIND][MAX_LOG_SUB_NAME] =
	{
		// EDGE LINE
		{"D:/FPDCIM/LOG/HANDSHAKE/02 WRU/ALL VIEW/UPPER/"			,"ALL_WRU_UPPER.INX"	,"ALL_WRU_UPPER"		,"CSV"	,		"LOG_TITLE_ALL_HANDSHAKE"}, // 00 WRU UPPER LOG
		{"D:/FPDCIM/LOG/HANDSHAKE/02 WRU/ALL VIEW/LOWER/"			,"ALL_WRU_LOWER.INX"	,"ALL_WRU_LOWER"		,"CSV"	,		"LOG_TITLE_ALL_HANDSHAKE"}, // 00 WRU UPPER LOG
	};
	char *LogPLC_WRUName[MAX_WRU_MODULE_CNT][MAX_LOG_SUB_NAME] =
	{	// EDGE LINE
		{"D:/FPDCIM/LOG/COURSE/PLC/02 WRU/"				,"PLC_WRU_Log.INX"			,"PLC_WRU"		,"LOG"	,	"LOG_TITLE_PLC"}, // 00 WRU UPPER LOG
	};
	// shseo 2010_1007 - add 
	//char *LogAction_WRUName[MAX_WRU_MODULE_CNT + 1][MAX_LOG_SUB_NAME] =
	char *LogAction_WRUName[MAX_WRU_MODULE_CNT][MAX_LOG_SUB_NAME] =
	{
		// Action Long
		// WRU LINE
		{"D:/DATA/EQP/ActionLog/"				,"ACTION_WRU_Log.INX"		,"ACTION_WRU"		,"LOG"	,		"LOG_TITLE_ACTION"},	//00 WRU LOG
		//D:/DATA/EQP/ActionLog/WRU/
	};


	char *LogHS_LCPIName[(MAX_LCPI_MODULE_CNT + 1) * MAX_LOG_HS_KIND][MAX_LOG_SUB_NAME] =
	{
		// LCPI LINE
		{"D:/FPDCIM/LOG/HANDSHAKE/02 LCPI/ALL VIEW/UPPER/"			,"ALL_LCPI_UPPERLog.INX"			,"ALL_LCPI_UPPER"		,"CSV"	,		"LOG_TITLE_ALL_HANDSHAKE"},  // 00 LCPI UPPER LOG
		{"D:/FPDCIM/LOG/HANDSHAKE/02 LCPI/ALL VIEW/LOWER/"			,"ALL_LCPI_LOWERLog.INX"			,"ALL_LCPI_LOWER"		,"CSV"	,		"LOG_TITLE_ALL_HANDSHAKE"},  // 01 LCPI LOWER LOG
	};
	
	char *LogPLC_LCPIName[MAX_LCPI_MODULE_CNT + 1][MAX_LOG_SUB_NAME] =
	{
		// LCPI LINE
		{"D:/FPDCIM/LOG/COURSE/PLC/02 LCPI/"				,"PLC_LCPI_Log.INX"			,"PLC_LCPI"			,"LOG"	,		"LOG_TITLE_PLC"}, // 00 LCPI UPPER LOG
	};
	// shseo 2010_1007 - add 
	char *LogAction_LCPIName[MAX_LCPI_MODULE_CNT][MAX_LOG_SUB_NAME] =
	{
		// Action Long
		// LCPI LINE
		{"D:/DATA/EQP/ActionLog/"				,"ACTION_LCPI_Log.INX"		,"ACTION_LCPI"		,"LOG"	,		"LOG_TITLE_ACTION"},	//00 LCPI LOG
		//D:/DATA/EQP/ActionLog/LCPI/
	};

	char *LogHS_LCODFName[(MAX_LCODF_MODULE_CNT + 1) * MAX_LOG_HS_KIND][MAX_LOG_SUB_NAME] =
	{
		// LCODF LINE
		{"D:/FPDCIM/LOG/HANDSHAKE/02 LCODF/ALL VIEW/UPPER/"			,"ALL_LCODF_UPPERLog.INX"			,"ALL_LCODF_UPPER"		,"CSV"	,		"LOG_TITLE_ALL_HANDSHAKE"},  // 00 LCODF UPPER LOG
		{"D:/FPDCIM/LOG/HANDSHAKE/02 LCODF/ALL VIEW/LOWER/"			,"ALL_LCODF_LOWERLog.INX"			,"ALL_LCODF_LOWER"		,"CSV"	,		"LOG_TITLE_ALL_HANDSHAKE"},  // 01 LCODF LOWER LOG		
	};
	
	char *LogPLC_LCODFName[MAX_LCODF_MODULE_CNT + 1][MAX_LOG_SUB_NAME] =
	{
		// LCODF LINE
		{"D:/FPDCIM/LOG/COURSE/PLC/02 LCODF/"				,"PLC_LCODF_Log.INX"			,"PLC_LCODF"			,"LOG"	,		"LOG_TITLE_PLC"}, // 00 LCODF UPPER LOG
	};
	// shseo 2010_1007 - add 
	char *LogAction_LCODFName[MAX_LCODF_MODULE_CNT][MAX_LOG_SUB_NAME] =
	{
		// Action Long
		// LCODF LINE
		{"D:/DATA/EQP/ActionLog/"				,"ACTION_LCODF_Log.INX"		,"ACTION_LCODF"		,"LOG"	,		"LOG_TITLE_ACTION"},	//00 LCODF LOG
		// D:/DATA/EQP/ActionLog/LCODF/
	};

	char *LogHS_LCRWName[(MAX_LCRW_MODULE_CNT + 1) * MAX_LOG_HS_KIND][MAX_LOG_SUB_NAME] =
	{
		// LCRW LINE
		{"D:/FPDCIM/LOG/HANDSHAKE/02 LCRW/ALL VIEW/UPPER/"			,"ALL_LCRW_UPPERLog.INX"			,"ALL_LCRW_UPPER"		,"CSV"	,		"LOG_TITLE_ALL_HANDSHAKE"},  // 00 LCRW UPPER LOG
		{"D:/FPDCIM/LOG/HANDSHAKE/02 LCRW/ALL VIEW/LOWER/"			,"ALL_LCRW_LOWERLog.INX"			,"ALL_LCRW_LOWER"		,"CSV"	,		"LOG_TITLE_ALL_HANDSHAKE"},  // 01 LCRW LOWER LOG		
	};
	
	char *LogPLC_LCRWName[MAX_LCRW_MODULE_CNT + 1][MAX_LOG_SUB_NAME] =
	{
		// LCRW LINE
		{"D:/FPDCIM/LOG/COURSE/PLC/02 LCRW/"				,"PLC_LCRW_Log.INX"			,"PLC_LCRW"			,"LOG"	,		"LOG_TITLE_PLC"}, // 00 LCRW UPPER LOG
	};
	// shseo 2010_1007 - add 
	char *LogAction_LCRWName[MAX_LCRW_MODULE_CNT][MAX_LOG_SUB_NAME] =
	{
		// Action Long
		// LCRW LINE
		{"D:/DATA/EQP/ActionLog/"				,"ACTION_LCRW_Log.INX"		,"ACTION_LCRW"		,"LOG"	,		"LOG_TITLE_ACTION"},	//00 LCRW LOG
		//D:/DATA/EQP/ActionLog/LCRW/
	};


	for(int i= 0; i<MAX_LOG_SUB_NAME; i++)
	{
		if(nEQType == -1) // Default Log
			strcpy(LogName[i], LogDefaultName[nModule][i]);
		else if(nEQType == eEQType_ETCHSTRIP)
		{
			if(nLogKind == eLogPLC)
				strcpy(LogName[i], LogPLC_EtrStripName[nModule][i]);
			else if(nLogKind == eLogHS)
				strcpy(LogName[i], LogHS_EtrStripName[nModule][i]);
			else if (nLogKind == eLogACTION)
				strcpy(LogName[i], LogAction_EtrStripName[nModule][i]);
		}
		else if(nEQType == eEQType_ETCH)
		{
			if(nLogKind == eLogPLC)
				strcpy(LogName[i], LogPLC_ETCHERName[nModule][i]);
			else if(nLogKind == eLogHS)
				strcpy(LogName[i], LogHS_ETCHERName[nModule][i]);
			else if (nLogKind == eLogACTION)
				strcpy(LogName[i], LogAction_ETCHERName[nModule][i]);
		}
		else if(nEQType == eEQType_STRIP)
		{
			if(nLogKind == eLogPLC)
				strcpy(LogName[i], LogPLC_STRIPName[nModule][i]);
			else if(nLogKind == eLogHS)
				strcpy(LogName[i], LogHS_STRIPName[nModule][i]);
			else if (nLogKind == eLogACTION)
				strcpy(LogName[i], LogAction_STRIPName[nModule][i]);
		}

		// PSK 080406 SD type Log 폴더 수정
		else if(nEQType == eEQType_PFC)
		{
			if(nLogKind == eLogPLC)
				strcpy(LogName[i], LogPLC_PFCName[nModule][i]);
			else if(nLogKind == eLogHS)
				strcpy(LogName[i], LogHS_PFCName[nModule][i]);
			else if (nLogKind == eLogACTION)
				strcpy(LogName[i], LogAction_PFCName[nModule][i]);
		}
		else if(nEQType == eEQType_EDGE)
		{
			if(nLogKind == eLogPLC)
				strcpy(LogName[i], LogPLC_EDGEName[nModule][i]);
			else if(nLogKind == eLogHS)
				strcpy(LogName[i], LogHS_EDGEName[nModule][i]);
			else if (nLogKind == eLogACTION)
				strcpy(LogName[i], LogAction_EDGEName[nModule][i]);
		}
		else if(nEQType == eEQType_WRU)
		{
			if(nLogKind == eLogPLC)
				strcpy(LogName[i], LogPLC_WRUName[nModule][i]);
			else if(nLogKind == eLogHS)
				strcpy(LogName[i], LogHS_WRUName[nModule][i]);
			else if (nLogKind == eLogACTION)
				strcpy(LogName[i], LogAction_WRUName[nModule][i]); // 1, 0 
				//strcpy(LogName[i], LogAction_WRUName[0][i]); // 1, 0 
		}

		else if(nEQType == eEQType_LCPI)
		{
			if(nLogKind == eLogPLC)
				strcpy(LogName[i], LogPLC_LCPIName[nModule][i]);
			else if(nLogKind == eLogHS)
				strcpy(LogName[i], LogHS_LCPIName[nModule][i]);
			else if (nLogKind == eLogACTION)
				strcpy(LogName[i], LogAction_LCPIName[nModule][i]);
		}

		else if(nEQType == eEQType_LCODF)
		{
			if(nLogKind == eLogPLC)
				strcpy(LogName[i], LogPLC_LCODFName[nModule][i]);
			else if(nLogKind == eLogHS)
				strcpy(LogName[i], LogHS_LCODFName[nModule][i]);
			else if (nLogKind == eLogACTION)
				strcpy(LogName[i], LogAction_LCODFName[nModule][i]);
		}

		else if(nEQType == eEQType_LCRW)
		{
			if(nLogKind == eLogPLC)
				strcpy(LogName[i], LogPLC_LCRWName[nModule][i]);
			else if(nLogKind == eLogHS)
				strcpy(LogName[i], LogHS_LCRWName[nModule][i]);
			else if (nLogKind == eLogACTION)
				strcpy(LogName[i], LogAction_LCRWName[nModule][i]);
		}
		else 
			MessageBox(NULL,"Log Path Failed","Log Make Error",MB_OK);
	}

	//	{"D:/DATA/EQP/ActionLog/WRU/"				,"ACTION_WRU_Log.INX"		,"ACTION_WRU"		,"LOG"	,		"LOG_TITLE_ACTION"},
	strcpy(m_szLogFilePath,LogName[0]);
	strcpy(m_szLogIndexFileName,LogName[1]);
	strcpy(m_szLogFilePrefix,LogName[2]);
	strcpy(m_szLogFileExt,LogName[3]);
//	nKeepDay = atoi(LogName[4]);
	strcpy(m_szLogTitleName,LogName[4]);

// 	if ( (nKeepDay != 7) && (nKeepDay <= MAX_LOG_KEEP_DAY) )
// 		m_nKeepDay = MAX_LOG_KEEP_DAY;
// 	else
// 		m_nKeepDay = nKeepDay;

	// -----------------------------------------------
	CreateDir(m_szLogFilePath);
	/*
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
	*/
	// -----------------------------------------------

	SetLogTitle();
}

void CMyLog::SetMSCPrefixName(char *szEQPID, char *szModuleName)
{
	strcpy(m_szEQPID, szEQPID);
	strcpy(m_szModuleName, szModuleName);
}




//================================================================
// shseo 2010_1011 add 

CInfoLog::CInfoLog()
{
	memset(m_szFilePath		, 0x00, sizeof(m_szFilePath));
	memset(m_szFilePrefix	, 0x00,	sizeof(m_szFilePrefix));
	memset(m_szFileExt		, 0x00, sizeof(m_szFileExt));

	memset(m_szFilePathName , 0x00, sizeof(char)*1024);
	memset(m_szLogTitle, 0x00, sizeof(char)*500);

	SetTitle();
}

CInfoLog::~CInfoLog()
{
	
}

long CInfoLog::CreateDir(char* Path)
{
	char DirName[256];
	char* p = Path;
	char* q = DirName; 
	LONG rtn =0;
	
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
	rtn = CreateDirectory(DirName, NULL); // zero: fail, non-zero: success
	
	return rtn;
	
	
}

void CInfoLog::SetConfig(char *szPath, char *szPrefix, char *szFileExt)
{
	long nLen	= 0;
	long nRval	= 0;
	
	// SonJaeWon
	memset(m_szFilePath		, 0x00, sizeof(m_szFilePath));
	memset(m_szFilePrefix	, 0x00,	sizeof(m_szFilePrefix));
	memset(m_szFileExt		, 0x00, sizeof(m_szFileExt));
	
	nLen = strlen(szPath);
	memcpy(m_szFilePath, szPath, nLen);
	
	nLen	=	strlen(szPrefix);
	memcpy(m_szFilePrefix, szPrefix, nLen);

	nLen	=	strlen(szFileExt);
	memcpy(m_szFileExt, szFileExt, nLen);
	
	nRval = CreateDir(m_szFilePath);
	
	//TRACE("nRval = %d \r\n", nRval); 
}


void CInfoLog::SetTitle()
{
	//char szLogTitle[255] = {NULL, };			// use 
	
	sprintf(m_szLogTitle, "%s,%s,%s,%s,%s,%s,%s,%s \r\n", "Date", "Time", "Module", "LogType", "Step",	// file title		// ,%s,%s 
		"PPID", "GlassID", "Information");			// "Departure", "Destination",  
	
	//Add(szLogTitle, true);
}


int CInfoLog::GetFileSize(char *path)
{
	struct stat file_stats; 

	if( stat(path, &file_stats) )
		return -1; 

	return file_stats.st_size; 

}

void CInfoLog::Add(char *szLog)
{	
	LogLock.Lock();	
	
	char		buff[4] = { NULL, };
	char		szFilePathName[1024] = { NULL, };
	SYSTEMTIME	systime;
	int			sz; // shseo 2010_1024 add 
	
	GetLocalTime(&systime);
	
	sprintf(szFilePathName, "%s%s_%04d%02d%02d.%s", 
		m_szFilePath,
		m_szFilePrefix, 
		systime.wYear,
		systime.wMonth,
		systime.wDay,
		m_szFileExt);
	
	// shseo 2010_1024 add 
	sz = GetFileSize(szFilePathName);
	if( sz > LIMIT_FILE_SIZE) 
	{
		if( strcmp(m_szFilePathName, "") == 0 ) 
		{
			sprintf(m_szFilePathName, "%s%s_%04d%02d%02d_%02d.%s", 
				m_szFilePath,
				m_szFilePrefix, 
				systime.wYear,
				systime.wMonth,
				systime.wDay,
				systime.wHour,
				m_szFileExt);
			//SetTitle(); 
		}

		sz = GetFileSize(m_szFilePathName);
		if( sz > LIMIT_FILE_SIZE) 
		{
			sprintf(m_szFilePathName, "%s%s_%04d%02d%02d_%02d.%s", 
				m_szFilePath,
				m_szFilePrefix, 
				systime.wYear,
				systime.wMonth,
				systime.wDay,
				systime.wHour,
				m_szFileExt);
			//SetTitle(); 
		}
		strcpy(szFilePathName, m_szFilePathName); 
	}
	else
	{
		if( strcmp(m_szFilePathName, "") != 0 ) 
		{
			//SetTitle(); 
		}
		memset(m_szFilePathName , 0x00, sizeof(char)*1024);
	}

	WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

    hFind = FindFirstFile(szFilePathName, &FindFileData);
	FindClose(hFind);
	
	m_hDataFile = open( szFilePathName, _O_WRONLY | _O_BINARY | _O_CREAT, 0777 );

	// shseo 2010-11-02 add 
	if(hFind == INVALID_HANDLE_VALUE && m_szLogTitle != NULL)
    {
		write( m_hDataFile, m_szLogTitle, strlen(m_szLogTitle) );
    }

	lseek( m_hDataFile, 0L, SEEK_END );
	
	write( m_hDataFile, szLog, strlen( szLog ) );
	sprintf(buff, "\r\n");
	write( m_hDataFile, buff, strlen(buff) );
	
	close( m_hDataFile );
	
	LogLock.Unlock();
}


void CInfoLog::AddInfo(stInfomationLogType *pInfoLogType)
{
	char szInformDescription[255] = {NULL, };	// use 
	char szDate[MAX_DATE_LEN+1] = {NULL,};		// use
	char szTime[MAX_OCCUR_TIME_LEN+1] = {NULL,}; // use 
	//char szModuleID[MAX_MODULE_LEN+1] = {NULL,};
	char szModuleID[MAX_INFO_SUB_UNITID_LEN+1] = {NULL,};
	char buff[4] = { NULL, };
	
	SYSTEMTIME			sm;
	GetLocalTime(&sm);
	
	sprintf(szDate, "%04d%02d%02d", sm.wYear, sm.wMonth, sm.wDay);
	sprintf(szTime, "%02d:%02d:%02d.%03d",  sm.wHour, sm.wMinute, sm.wSecond, sm.wMilliseconds);
	//sprintf(szModuleID, "%s%s",  pInfoLogType->szModuleName,  pInfoLogType->szUnitName);//Information Log의 경우 Module항목에 " ModuleName + UnitName "으로 기록한다.
	sprintf(szModuleID, "%s", pInfoLogType->szUnitName);//Information Log의 경우 Module항목에 " ModuleName + UnitName "으로 기록한다.
	
	sprintf(szInformDescription, "%s,%s,%s,%s,%s,%s,%s,%s", // %s,%s, 
		szDate, 
		szTime,
		szModuleID,
		pInfoLogType->szLogType,
		pInfoLogType->szStepID,
		//pInfoLogType->szFromPosUnitName,
		//pInfoLogType->szToPosUnitName,
		pInfoLogType->szPPID,
		pInfoLogType->szHPanelID,
		pInfoLogType->szInformation);
	
	Add(szInformDescription); 
}
