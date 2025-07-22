// CMainPlc.h: interface for the CCMainPlc class.
//
//////////////////////////////////////////////////////////////////////

#ifndef	_CMainPlc_H__
#define _CMainPlc_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	"Melsec.h"
#include	"../include/mdfunc.h"
#include	"../include/ipcsma.h"
#include	"../include/ipcqueue.h"
#include	"Thread.h"
#include	"../lib/SW3FaLib.h"

#include	"../include/EventSignal.h"
#include	"../include/syslib.h"
#include	"MyLog.h"
#include	"BaseModule.h"

#include	"Map_Common.h"
#include	"Map_PhotoInline.h"
#include	"TimeCheck.h"
#include	"PLC_Version.h"


// Local Module Count Define
#define		MAX_PHOTO_MODULE_CNT				13
#define		MAX_ETCHSTRIP_MODULE_CNT			3
#define		MAX_ETCHSTRIP_SUB_2_MODULE_CNT		2
#define		MAX_ETCH_MODULE_CNT					1
#define		MAX_STRIP_MODULE_CNT				2
#define		MAX_EDGE_MODULE_CNT					1
#define		MAX_PFC_MODULE_CNT					1
#define		MAX_WRU_MODULE_CNT					1
#define		MAX_LCPI_MODULE_CNT					1
#define		MAX_LCODF_MODULE_CNT				1
#define		MAX_LCRW_MODULE_CNT					1

class CMainPlc : public CMelsec, public CThread, public SysLib
{
public:
	CQueue		m_cMyQueue;
	CQueue		m_cSendQue;
	CQueue		m_cMMIQue;
	CQueue		m_cInternelQue[MAX_LOCAL_COUNT];

	
	CMyLog		m_GUIlog;
	CMyLog		m_LogMaster;
	CMyLog		m_ActionLog[MAX_LOCAL_COUNT];		// 20 
	CMyLog		m_Log[MAX_LOCAL_COUNT];				// 20 
	CMyLog		m_Log_HandShake[MAX_LOG_HANDSHAKE_NO];
	CInfoLog		m_InfoLog; 

	CMelsec m_MelLinkMemIF;	//	Link Memory Handling Interface
	CMelsec m_MelERMemIF;	//	Board ER(0~31) Memory Handling Interface

	CBaseModule m_BaseModule;
	CBaseModule m_LocalUnit[MAX_LOCAL_COUNT];
/* =================================== m_LocalUnit Mapping ===================================*/
//	* m_LocalUnit[]  1 = eModuleType_Indexer
//					 2 = eModuleType_Cleaner	, eModuleType_Etcher(단동 Stripper포함)	, eModuleType_PFC	, eModuleType_Edge,  eModuleType_WRU
//				     3 = eModuleType_DBK		, eModuleType_Stripper
//					 4 = eModuleType_Coater
//					 5 = eModuleType_VCD
//					 6 = eModuleType_SBK
//					 7 = eModuleType_Interface
//					 8 = eModuleType_PEB
//					 9 = eModuleType_Developer
//				    10 = eModuleType_PBK
/* ========================================================================================*/

	short				m_shCIMAlive;
	long				m_nTimesetDay;

public:
	void SetCIMAlive();
	void OnTowerLampControl();
	void UpdateUnloadGlassData(stPanelInfoType *pGlsInfo);
	void CreateInternalQueue();
	void CreateExternalQueue();
	void SendSignalToSCH(stQueueRootType *sig);
	void SendSignalToMMI(stQueueRootType *sig);

	//	Data Link Update
	void OnSetCIM_BitStateToEQ();					//	CIM Data Update : Bit Area
	void OnSetCIM_ReqRlyBitToEQ(long nModuleID);	//	CIM Req&Rly Data Update : Bit Area

	void OnSetCIM_WordDataToEQ();	//	CIM Data Update : Word Area

	//	IPC Signal Function
	void C2P_TimeSetSequence(stPlcQueueType sig);			//	Master PC <-> PLC Time Synchronization
	void C2P_ECOModeChangeSequence(stPlcQueueType sig);		//	ECO Mode Change BoB 2011.11
	
	void C2P_AlarmTreatSequence(stPlcQueueType sig);
	void C2P_MachineCmdSequence(stPlcQueueType sig);
	void C2P_TerminalMsgSequence(stPlcQueueType sig);
	void C2P_OperatorCallSequence(stPlcQueueType sig);
	
	void C2P_BuzzerStopSequence(stPlcQueueType sig);
//	void OnOperatorCallSequence(stPlcQueueType sig);

	void C2P_DataChangeSequence(stPlcQueueType sig);
	
	void SetGlassTransferData(stPlcQueueType *sig);
	void GetGlassTransferData(stPanelInfoType* pGlsInfo);

	//PCJ
	void SetLayOutCfg();

	BOOL Run();
	BOOL ExitInstance();
	BOOL InitInstance();

	void CheckSyncTimeset();

	// Thread (Timer) 관련.
	virtual BOOL OnInit();
	virtual BOOL OnTerminate();

	CMainPlc();
	virtual ~CMainPlc();

};

#endif // !defined(AFX_CMainPlc_H__E04600DB_2C0C_4EAD_B37C_F3EC980E6CD3__INCLUDED_)
