// ControlModule.h: interface for the ControlModule class.
//
//////////////////////////////////////////////////////////////////////

/* ==== [Sequence Function Name Rule] ==== */
/* 1. P2C_~ : [Request Flow] PLC => CIM	   */
/* 2. C2P_~ : [Request Flow] CIM => PLC    */
/* ======================================= */

#ifndef _CONTROLMODULE_H__
#define _CONTROLMODULE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Melsec.h"
#include "../lib/SysLib.h"
#include "Thread.h"	
#include "TimeCheck.h"
#include "Math.h"
#include "../include/RecipeOffset.h"


// PCJ
#define DEF_TIME_OVER		3000
#define MAX_SEQUENCE_EVENT	50
#define MAX_LOCAL_COUNT		20
#define MAX_MSC_ITEM		100

#define STEP_START			0
#define STEP_NG				1
#define STEP_OK				2

class CMainPlc;
class CShareCommonData;

enum eTimeSequenceName
{
	eDefault = 0,
//--------- Sequence Check -----------//
	eP2C_GlassScrap				= 1,
	eP2C_GlassUnscrap			= 2,
	eP2C_GlassJudgement			= 3,
	eP2C_PPIDCheck				= 4,
	eP2C_ECIDChange				= 5,
	eP2C_TEMPChange				= 6,
	eP2C_CHEMICAL				= 7,
	eP2C_RecipeDown				= 8,
	eP2C_ProcessEnd				= 9,
	eP2C_ManualCellRoad			= 10, // 면취만 사용
	eP2c_BackModeStart			= 11, // Photo만 사용 (Crack 감지)
	eP2C_HS_NRTimeOver			= 12,
	eP2C_HS_BCTimeOver			= 13,
//--------- Queue Check -----------//
	eC2P_AlarmTreated			= 14,
	eC2P_MachineCommand			= 15,
	eC2P_ECIDChange				= 16,
	eC2P_TEMPChange				= 17,
	eC2P_TankDataChange			= 18,
	eC2P_PPIDPrepareReq			= 19,
//--------- Event Check -----------//
	eP2C_AlarmOccured			= 20,
	eP2C_AlarmTreated			= 21,
//--------- CMD Req -----------//
	eC2P_RecoveryMode			= 22,
	eC2P_BuzzerStop				= 23,
	eC2P_OperatorCall			= 24,
	eC2P_TerminalMsg			= 25,
	eC2P_TimeSet				= 26,
//--------- Monitor ----------//
	eGetStatus					= 27,
	eGetStatusCode				= 28,
//----------- etc ------------//
	eC2P_AlarmResetEvent		= 29,
	eCheckAliveState			= 30,
	eP2C_VCRReadingNG			= 31,
	eHandShakeValid				= 32,
	eMNCellRead					= 33,
	eMNCellValid				= 34,
	eMNCellCancel				= 35,
	eMNCellJudge				= 36,
	eProcessMode				= 37,

	eP2C_GlassSendFail			= 38, // 2010-08-24 add

	eUsingTankNo				= 38,		//KWY
	eC2P_ECOModeChange			= 39,
	eP2C_ECOModeChange			= 40,
				
	eP2C_EQNetworkState			= 41,
	eP2C_RPC_GLASSID			= 42,

	eGECDCrackEvent				= 43,

	eModuleRunData				= 44,
};

enum eMSCItem
{
	eMSCGlassIn					 = 0,		// Glass 투입 시간
	eMSCGlassArrange1				,		// 정렬및 위치 이동 시간 1
	eMSCProcessWait1				,       // Tact 대기 시간 1
	eMSCChemicalPosBeforeMove1		,       // 약액 구간전 이동시간 1
	eMSCChemicalPosBeforeMove2		,		// 약액 구간전 이동시간 2
	eMSCProcessWait2				,		// Tact 대기 시간 2
	eMSCChemicalPosBeforeMove3		,		// 약액 구간전 이동시간 3
	eMSCChemicalPosBeforeMove4		,		// 약액 구간전 이동시간 4
	eMSCChemicalPosMove				,		// 약액부 처리 시간
	eMSCLinsePosMove				,       // 수세부 처리 시간
	eMSCGlassOutProcess1			,		// Glass 배출전 처리 시간 1
	eMSCGlassOutProcess2			,		// Glass 배출전 처리 시간 2
	eMSCGlassOutProcess3			,		// Glass 배출전 처리 시간 3
	eMSCGlassArrange2				,		// 정렬및 위치 이동 시간 2
	eMSCGlassOutEnd					,		// Glass 배출 시간

	eMSC_BUf_ARM_STRAIGHT	   =  50,       // Buffer ARM Straight
	eMSC_BUf_ARM_FOLD				,		// Buffer ARM Fold
	eMSC_BUf_ARM_UP					,		// Buffer ARM Up
	eMSC_BUf_ARM_DOWN				,		// Buffer ARM Down
	eMSC_BUF_BODY_MOVE				,		// Buffer ARM Move
	eMSC_ITEM_END					,
};

enum eInterlockRelatedID
{
	eInterlock_Upper			= 0,
	eInterlock_SEMES			= 1,
	eInterlock_Lower			= 2,
};

enum eAlarmDataOrder
{
	eAlarmID		= 0,		
	eAlarmCode		= 1,		
	eAlarmText		= 2,		
};

enum eActionLogDataOrder
{
	eActionLogData1			= 0,	//0x06356	// ActionID (Unit 동작 정의) 
	eActionLogData2			= 1,    //0x06357   // ActionID (HandShake No.1 & No.2 상태 정의)
	eActionLogData3			= 2,	//0x06358   // ActionID (HandShake No.3 상태 정의)
	eActionLogData_FromPos	= 3,    //0x06359   // From Position
	eActionLogData_ToPos	= 5,    //0x0635B	// To	Position
};

struct stMSCDatatype
{
	short shMSCStartTime;
	ULONG nMSCTimeItem[MAX_MSC_ITEM];
};

class CShareCommonData
{
public:

	// Normal 공유 Member
	long	m_nCmdData;
	long	m_nReqID;
	short	m_shBitAddr;	
	short	m_shDataAddr;
	short	m_shSetData;
	short	m_shTempAddr;
	short	m_shCmdPacket[4];
	BOOL	m_bWaitTime;
	long	m_nElaspedTime;

	CTimerCheck m_TimeCheck;

	// Queue 전송용 공유 Member
	long	m_nUniquID[4];
	long	m_nPPID;
	long	m_nAlarmID;
	long	m_nAlarmCode;
	char	m_szAlarmText[MAX_ALARM_TEXT_LEN+1];
	long	m_nModuleID;
	char	m_szHPanelID[MAX_PANEL_ID_LEN+1];
	char	m_szRCode[MAX_REASON_CODE_LEN+1];
	long	m_nOwnGlassNo;

	stECIDChangeType	m_pData;
	stProcessEndDataType m_stpProcEndData;

	stECOChangeType		m_stECOData;

	CShareCommonData();
	~CShareCommonData();
};

//	제어기 단위의 Layer1 Module에 대한 Base Class
class CBaseModule : public CMelsec , public CThread ,public SysLib
{
public:

	CQueue				m_cMyQue;

	long				m_nAlarmStatData[32];
	long				m_nAlarmEvtFlag[32];

	long				m_nLastAlmOccurID;
	long				m_nLastAlmTreatID;
	BOOL				m_bSetRecoveryCmd;

	CMainPlc				*m_pParent;
	stModuleCfgType			*m_stpModCfg;
	stModuleDataInfoType	*m_stpModData;
	stModuleRunInfoType		*m_stpModRunInfo;

	long				m_nMaxCount;
	long				m_nAlarmCount;
	long				m_nOffset;

	bool*				m_pbAlarmHappen;
	BOOL				m_bBaseThread;

	long				m_EtchGCnt;
	long				m_BypassGCnt;
	long				m_BufferGCnt;

	short				m_shPLCMAlive;

	long				m_nVCRMode;
	long				m_nKeyInWaitTime;

	// PCJ
	CShareCommonData *m_ShCommData_Master[MAX_SEQUENCE_EVENT];
	CShareCommonData *m_ShCommData_Local[MAX_SEQUENCE_EVENT];
	stMSCDatatype	m_stMSCData;
	char	m_szMSCHPanelID[MAX_PANEL_ID_LEN+1];
	BOOL	m_bMDStartforFlowPress[MAX_LAYER1_MODULE_COUNT];
	BOOL	m_bMDStartforTemp[MAX_LAYER1_MODULE_COUNT];

	BOOL m_bProgramFirstStart1[3];
	BOOL m_bProgramFirstStart2[3];
	BOOL m_bHSLogUpper1[3][100];
	BOOL m_bHSLogUpper2[3][100];
	BOOL m_bHSLogUpperOld1[3][100];
	BOOL m_bHSLogUpperOld2[3][100];
	BOOL m_bHSLogLower1[3][100];
	BOOL m_bHSLogLower2[3][100];
	BOOL m_bHSLogLowerOld1[3][100];
	BOOL m_bHSLogLowerOld2[3][100];
	BOOL m_bHSLogUpperState[3];
	BOOL m_bHSLogLowerState[3];

	/*============================ MSC LOG 영역 =================================================*/
	char m_chModuleID[27+1];		
	/*
	char m_chITEM[4+1];
	char m_chTime[18+1];				
	char m_chH_PANELID[12+1];				
	char m_chCOMM_TYPE[12+1]; // CMD_SEND, ACK_RECEIVE, ACT_EXECUTE, ACT_END
	char m_chUNIT_TYPE[17+1];				
	char m_chUNITID[4+1];					
	char m_chACTION[MAX_MSC_ITEM][12+1];	
	char m_chDIRECTION[4+1];				
	char m_chDESTINATION[27+1];			
	ULONG m_nPROCESSING_TIME[MAX_MSC_ITEM];
	*/
	/*==========================================================================================*/

	BOOL m_bOldActionState[MAX_LAYER2_MODULE_COUNT][MAX_ACTION_ID_COUNT];	//Max Action ID Count	// 50, 48 

	bool bPMCheck[MAX_LAYER1_MODULE_COUNT];
	bool bPMLog_Set[MAX_LAYER1_MODULE_COUNT];

	BOOL m_bHSValid_Upper1[MAX_HS_VALID_ID_COUNT][16];
	BOOL m_bHSValid_Upper2[MAX_HS_VALID_ID_COUNT][16];
	BOOL m_bHSValid_Upper3[MAX_HS_VALID_ID_COUNT][16];
	long m_nHSValid_SemesUniqID[MAX_HANDSHAKE_COUNT];

	long m_nGECDEventID[MAX_GECD_EVENT_COUNT];

	BOOL m_bEQNetworkState[6];

	short m_shHS_TO;

public:
	void SetInterlockRelatedID(long nModuleNo, long nHSNo, long nInterlockNo, long nUpLow);
	void SetNotifyEvent(long nECode);
	void FillSpace(char *pGlassDataItem, size_t nSize);
	void GetGlassDataFromRUNDATA(stSMAUniqueIDTableType *pUniqueIDData, long OwnGlassNo);

	void CheckHandShakeValidData();		//@ HandShake Valid Check
	void CheckEQNetworkStateCheck();	//@ EQ Network State Check
	void CheckGECDCrackEvent();			//@ GECD Crack Event Check

	stSystemRunInfoType* GetSysRunInfo();
	//	각 Local에 대한 SMA Point를 Member로 할당한다.
	void SetConfig(void *pParent, stModuleCfgType *stpModCfg, stModuleDataInfoType *stpModDataInfo, stModuleRunInfoType *stpModRunInfo);

	////////////////////////////////////////////////////
	//	Inline Net Related
	//	각 Local의 Handshake IO상태를 SMA에 Update : Bit Data
	void UpdateHandShakeInfoToUpper(short *pshData, short shAddress);
	void UpdateHandShakeInfoToLower(short *pshData, short shAddress);
	void GetHandShakeIO();								//	Read each station state from Melsec Network

	//	각 Local의 Status를 SMA에 Update : Word Data
	void UpdateEquipmentDataToSMA(short *pshData);
	void GetEquipmentData();			//	Read each station state from Melsec Network
	void GetStatusData();
	void GetProcessMode();		//RW 설비의 Process Mode Check.
	void GetUsingTankNo();		
	void GetECOModeState();		

	////////////////////////////////////////////////////
	//	Data Net Related : Read Data
	//	Each Module Bit State Related
	void UpdateDataLink_OpModeIOToSMA(short shData);
	void UpdateDataLink_OpStatusIOToSMA(short shData);
	void UpdateDataLink_GlassTrackingIOToSMA(short *shData);
	void UpdateDataLink_ReplyAndEventIOToSMA(short shData);
	void UpdateDataLink_DataChangeReplyAndEventIOToSMA(short shData);
	void UpdateDataLink_OpModeIOToSMA_FOR_BYP_BUF(short shData); // Etcher/Strip Type의 Bypass & Buffer 전용
	void UpdateDataLink_OpStatusIOToSMA_FOR_BYP_BUF(short shData); // Etcher/Strip Type의 Bypass & Buffer 전용
	void GetDataLink_BitState();

	//	Each Module Word Data Related
	void UpdateDataLink_GlassTrackingDataToSMA(short *shData);
	void GetDataLink_GlassTrackingData();

	void UpdateDataLink_CurrentProcessDataToSMA(short *shData);
	void GetDataLink_CurProcessingData();

	void GetDataLink_ActionLogData();	
	void OnActionStatusCheck(short *shData);

	void UpdateDataLink_ProcessEndDataToSMA(short *shCommonData, short *shData);
	void UpdateDataLink_ProcessEndDataToSMA_Inline_ETCH(short *shCommonData, short *shData, char *szHPanelID);
	void GetDataLink_ProcessEndData();

	////////////////////////////////////////////////////
	//	Data Net Related : Write Data

	//	Inner Queue Signal Handle
	//	Master Request
	void C2P_MachineCmdSequence(stMachineCmdType stCmd);
	void C2P_AlarmTreatSequence(stAlarmClearReqType stClearAlarmData);
	void C2P_TerminalMsgSequence(stTerminalMsgType stTerminalMsg);
	void C2P_OperatorCallSequence(stTerminalMsgType stTerminalMsg);
	void C2P_BuzzerStopSequence(stBuzzerCtrlType	stBuzzStop);
	void C2P_TimeSetSequence();
	void C2P_ECOModeChangeSequence(stECOChangeType stECO);
	void C2P_AlarmResetSequenceEnd();
//	void OnOperatorCallSequence(stBuzzerCtrlType	stBuzzStop);
	//	EQ Event Report
//	void P2C_ECOModeChangeSequence();

	void P2C_AlarmOccuredSequence();
	void P2C_AlarmTreatedSequence();
	void P2C_GlassScrapSequence();
	void P2C_GlassUnscrapSequence();
	void P2C_GlassJudgementSequence();
	void P2C_RecipeDownloadSequence();
	void P2C_PPIDValidationSequence();
	void P2C_ManualCellLoadSequence();
	void P2C_HSTimeOverSequence();
	void P2C_VCRReadingFailSequence();
	void P2C_GlassSendFailSequence(); // 2010-08-24 add
	
	//	EQ Constance Change Report
	void P2C_ECIDChangeSequence();
	void P2C_TEMPChangeSequence();
	void P2C_ProcessEndChangeSequence();

	void GetMasterDevBWAddr(short eContent, long nModuleSel, short shBitAddr, short shDataAddr);
	void GetLocalDevBWAddr(short eContent, long nModuleSel, short shBitAddr, short shDataAddr);
	long GetPosStartAddr(long nModuleSel);

	//	Host -> EQ Constance Chagne Request
	void C2P_ECIDChangeSequence(stECIDChangeType stFlowPress);
	void C2P_TEMPChangeSequence(stECIDChangeType stTemp);
	void C2P_DataChangeSequence(stECIDChangeType stData);
	void C2P_RecoveryModeSequence();	//	20060206
	void OnQueueCheck();

	//	Recipe Data Set Function
	void SetCleanerProcessData(long nPPID);

	void SetEtchProcessData(long nPPID);
	void SetStripProcessData(long nPPID);

	//	Utility Function
	void ConvertTransferDataToPanelData(stPanelInfoType *pPanelInfo, short *shData);
	void ConvertPanelDataToTransferData(stPanelInfoType *pPanelInfo, short *shData);
 
	stECIDConfigType *GetECIDConfig(long nECID);

	// Thread 관련.
	virtual BOOL OnTerminate();

	//	Main Process Tread
	void OnAlarmEventHandle();
	void OnSequenceAlarmEvent();

	void P2C_GlassSendFailSequenceEnd();  // 2010-08-24 add
	void P2C_GlassScrapSequenceEnd();
	void P2C_GlassUnscrapSequenceEnd();
	void P2C_GlassJudgementSequenceEnd();
	void P2C_RecipeDownloadSequenceEnd();

//	void P2C_ECOModeChangeSequenceEnd();

	void P2C_AlarmOccuredSequenceEnd();
	void P2C_AlarmTreatedSequenceEnd();
	void P2C_PPIDValidationSequenceEnd();
	void P2C_ManualCellLoadSequenceEnd();
	void P2C_HSTimeOverSequenceEnd();
	void P2C_VCRReadingFailSequenceEnd();
	void P2C_MNCellValidCheckSequence();
	void P2C_MNCellValidCheckSequenceEnd();
	void P2C_MNCellCancelSequence();
	void P2C_MNCellCancelSequenceEnd();

	void C2P_RecoveryModeSequenceEnd();
	void C2P_BuzzerStopSequenceEnd();

	void C2P_MNCellReadSequence();
	void C2P_MNCellReadSequenceEnd();

	void C2P_MNCellJudgeFromHSTSequence(long nAckCode);
	void C2P_MNCellJudgeFromHSTSequenceEnd();

	void P2C_ECIDChangeSequenceEnd();
	void P2C_TEMPChangeSequenceEnd();
	void P2C_ProcessEndChangeSequenceEnd();

	void C2P_AlarmTreatSequenceEnd();
	void C2P_MachineCmdSequenceEnd();
	void C2P_TimeSetSequenceEnd();
	void C2P_ECOModeChangeSequenceEnd();
	void C2P_ECIDChangeSequenceEnd();
	void C2P_TEMPChangeSequenceEnd();
	void C2P_TerminalMsgSequenceEnd();
	void C2P_OperatorCallSequenceEnd();

	BOOL ThreadAllRun();
	void OnUpdateBitArea();
	void OnUpdateWordArea();
	void OnUpdateERArea();
	void TrsUpdateMelSecArea();
	void TrsSequenceProcessing();
	void TrsSequenceProcessEnd();
	void TrsQueueCheckProcessing();
	void TrsBaseTimer();
	
	void AnalizeHSLog(short shAddr);

	void WriteActionLog(long nModuleID, long nUnitID, BOOL bHS, BOOL bBitState, long nBitNo, long nFromPos, long nToPos, long nSemesUniqID);

	void OnAlarmOffset(long nOffset);
	void AlarmHappenDataInit();
	void SendAlarmEvent(long nAlarmID, long nSig, long nModuleID);
	void OnEventAnalysis(long *pVal, long nModuleID);
	long IsAlarmEvent( long nIndex, bool bVal, long nModuleID );

	char* GetHandShakeItem(long nSide, long nItem);
	char* GetActionID(long nUnitType, long nActionID);
	char* GetDirectionUnitName(long nUnitPos);

	void OnECIDDataUpdateforStart();

	void CheckAliveState();
	void GetEpdData();
    void GetEpdTime();                                  // EPD Event Time ohanaya 2011.05.22

	BOOL SetRPCState(long nModuleNo, char *szH_PanelID, char *szJobID = NULL );
	BOOL OnCheckRPCData(long nModuleNo, char *szH_PanelID,  stRPCDataType *stRPCData = NULL);

	void _Trim(char* szString);

/*	CThread				m_ThreadUpdateBitArea;
	friend UINT			ThreadUpdateBitArea(LPVOID pParam);
	
	CThread				m_ThreadUpdateWordArea;
	friend UINT			ThreadUpdateWordArea(LPVOID pParam);

	CThread				m_ThreadUpdateERArea;
	friend UINT			ThreadUpdateERArea(LPVOID pParam);

*/
        void GetEQModuleRunData();
	CThread				m_ThreadUpdateMelSecArea;
	friend UINT			ThreadUpdateMelSecArea(LPVOID pParam);

	CThread				m_ThreadSequenceProcess;
	friend UINT			ThreadSequenceProcess(LPVOID pParam);

	CThread				m_ThreadSequenceProcessEnd;
	friend UINT			ThreadSequenceProcessEnd(LPVOID pParam);

	CThread				m_ThreadQueueProcess;
	friend UINT			ThreadQueueProcess(LPVOID pParam);

	CThread				m_ThreadUpdateTimer;
	friend UINT			ThreadUpdateTimer(LPVOID pParam);

	CBaseModule();
	virtual ~CBaseModule();

};

#endif // _CONTROLMODULE_H__
