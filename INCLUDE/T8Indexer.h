#ifndef		__T8Indexer_h__
#define		__T8Indexer_h__

/**************************************************************
				 Indexer 제어 관련 Structure : 미사용 Reserved
**************************************************************/

struct	stRobotWaitPosType
{
	BOOL	bGlassTakeMode1;
	BOOL	bGlassTakeMode2;
};

struct	stGlassStatusInArmIOType
{
	BOOL	bGlassContain;
	BOOL	bGlassWithout;
};

struct	stRobotModeStatusIOType
{
	BOOL	bInitializing;
	BOOL	bManualOperation;
	BOOL	bEmergency;
	BOOL	bAbnormal;
};

struct	stRobotStatusIOType
{
	BOOL	bIdle;
	BOOL	bWait;
	BOOL	bBusy;
	BOOL	bComplete;
	BOOL	bError;
};

struct	stPortStatusIOType
{
	BOOL	bMapping[MAX_PORT_COUNT];
	BOOL	bContained[MAX_PORT_COUNT];
	BOOL	bChuckin[MAX_PORT_COUNT];
	BOOL	bStkAgvComm[MAX_PORT_COUNT];
};

struct	stPortEventIOType
{
	BOOL	bEvent[MAX_PORT_COUNT];
};

struct	stMachineReplyIOType
{
	BOOL	bCommand;
	BOOL	bAlarmClear;
	BOOL	bTerminalMsg;
	BOOL	bOperatorCall;
};


struct	stRobotReplyIOType
{
	BOOL	bWaitPosChange;
	BOOL	bArmDataClear;
};

struct	stGlassHandlingReplyIOType
{
	BOOL	bReply;
	BOOL	bCancel;
};

struct	stPortCommandReplyIOType
{
	BOOL	bProcReserved;
	BOOL	bProcStart;
	BOOL	bProcComplete;
	BOOL	bProcCancel;
	BOOL	bProcAbort;
	BOOL	bSpecifyPortDisable;
	BOOL	bSTKAGVAbort;
	BOOL	bPortPause;	  
	BOOL	bPortModeChange;		//	Port Type Change
	BOOL	bReMapping;
	BOOL	bGlassSizeChange;
	BOOL	bPortStatusWait;
	BOOL	bChuck;
	BOOL	bUnChuck;
	BOOL	bCassetteIDRetry;
	BOOL	bBCRModeChagne;
};

struct	stSpecialCommandReplyIOType
{
	BOOL	bPortRobotRemapping;
	BOOL	bSpecialCommand2;
	BOOL	bSpecialCommand3;
	BOOL	bSpecialCommand4;
	BOOL	bSpecialCommand5;
};

struct	stETCEventIOType
{
	BOOL	bAlarmEvent;
};

/////////////////////////////////////////////////
//	Indexer Control Master 정보
struct	stMachineReqIOType
{
	BOOL	bCommand;
	BOOL	bAlarmClear;
	BOOL	bTerminalMsg;
	BOOL	bOperatorCall;
};

struct	stRobotReqIOType   // 구조체명 변경 stRobotReqIOType -> stEtcReqIOType  
{                        // kim.m.i [2004-06-06] 
	BOOL	bWaitPosChange;
	BOOL	bGlassTakeMode;
	BOOL	bArmDataClear;
};

struct	stGlassHandlingReqIOType
{
	BOOL	bCycleMove;
	BOOL	bStepMove;
	BOOL	bCancel;
};

struct	stPortCommandReqIOType
{
	BOOL	bProcReserved;
	BOOL	bProcStart;
	BOOL	bProcComplete;
	BOOL	bProcCancel;
	BOOL	bProcAbort;
	BOOL	bSpecifyPortDisable;
	BOOL	bSTKAGVAbort;
	BOOL	bPortPause;
	BOOL	bPortModeChange;		//	Port Type Change
	BOOL	bReMapping;
	BOOL	bGlassSizeChange;
	BOOL	bPortStatusWait;
	BOOL	bChuck;
	BOOL	bUnChuck;
	BOOL	bCassetteIDRetry;
	BOOL	bBCRModeChagne;
};

struct	stSpecialCommandReqIOType
{
	BOOL	bPortRobotRemapping;
	BOOL	bSpecialCommand2;
	BOOL	bSpecialCommand3;
	BOOL	bSpecialCommand4;
	BOOL	bSpecialCommand5;
};

struct	stPortEventReplyIOType
{
	BOOL	bEvent[MAX_PORT_COUNT];
};

struct	stETCEventReplyIOType
{
	BOOL	bAlarmEvent;
};

struct	stIOInfoType 
{
	// 설비(Process Module) Preview I/O : Contact Point
	stConvPreViewIOType			ioInConvStatus;    // ioConvStatus -> ioInConvStatus 수정됨.[2004-06-06]
	stConvPreViewIOType			ioOutConvStatus;   // kim.m.i. 추가 [2004-06-06]

	// Indexer Handshake I/O
	stGlassSendIOType			ioGlassSend;	//입구	Loader
	stRobotPreViewIOType		ioSendPreView;
	stGlassRecvIOType			ioGlassRecv;	//출구	Unloader
	stRobotPreViewIOType		ioRecvPreView;

	// 설비(Process Module) Handshake I/O
	stGlassSendIOType			ioEQGlassSend;	 // [입구] kim.m.i. 추가 [2004-06-06]
	stGlassRecvIOType			ioEQGlassRecv;	 // [출구] kim.m.i. 추가 [2004-06-06]

	//	Indexer 자체 정보	
	stRobotWaitPosType			ioRobotWaitPos;
	stGlassStatusInArmIOType	ioGlassStatusInArm;
	stRobotModeStatusIOType		ioRobotModeStatus;		// ioMachineStatus;	// T7 Robot Status
	stRobotStatusIOType			ioRobotRunStatus;		// Robot In Machine Status
	stPortStatusIOType			ioPortStatus;
	stPortEventIOType			ioPortEvent;
	stMachineReplyIOType		ioMachineReply;
	stRobotReplyIOType			ioRobotReply;
	stGlassHandlingReplyIOType	ioGlassHandleReply;
	stPortCommandReplyIOType	ioPortCommandReply;
	stSpecialCommandReplyIOType	ioSpecialCommandReply;
	stETCEventIOType			ioETCEvent;

	//	Indexer Control Master 정보
	stMachineReqIOType			ioMachineReq;
	stRobotReqIOType			ioRobotReq;
	stGlassHandlingReqIOType	ioGlassHandleReq;
	stPortCommandReqIOType		ioPortCommandReq;
	stSpecialCommandReqIOType	ioSpecialCommandReq;
	stPortEventReplyIOType		ioPortEventReply;
	stETCEventReplyIOType		ioEtcReply;
};

struct	stPortInfoType
{
	//	Indexer 자체 Update Data
	long	nRunStatus;				//	Port Status
	long	nJobStatus;				//	ms 20051121 - Add. Job Status 
	long	nPortType;				//	Both(Uni), In, Out
	long	nPortMode;				//	OK, NG
	long	nGlassSize[2];
	long	nCSTType;

	long	nPanelCount;
	long	nPanelMapping[MAX_GLASS_COUNT_PER_PORT];
	char	szCassetteID[MAX_CASSETTE_ID_LEN+1];
	long	nBCRMode;

	//	Running Information Data
	char	szPortID[MAX_PORT_ID_LEN+1];
	long	nSortType;
	long	nCSTDemand;
	long	nHotDevice;				//	0 : Normal, 1 : Hot Device

	//	
	long	nPortEvent;				//	Port Event에 대한 Data

	stPanelInfoType	 PanelInfo[MAX_GLASS_COUNT_PER_PORT];
};

struct	stRobotInfoType
{
	//	Indexer 자체 Update Data
	long	nCurRobotPos;					//	Robot 현재 위치
	long	nArmStatus;						//	Robot Arm 현재 상태

	//	Running Information Data
	char	szUnitID[MAX_UNIT_ID_LEN+1];
	stPanelInfoType stRbtGlsInfo;
};

struct	stIdxReplyDataType
{
	long	nMachineCmd;
	long	nAlarmClear;
	long	nTerminalMsg;

	long	nGlassHandling;

	long	nProcReserve;
	long	nProcStart;
	long	nProcComplete;
	long	nProcCancel;
	long	nProcAbort;
	long	nPortDisable;
	long	nSTKAbort;
	long	nPortPause;
	long	nPortMode;
	long	nPortRemapping;
	long	nPortGlassSize;
	long	nPortStatusWait;
	long	nChuck;
	long	nUnchuck;
	long	nCassetteID;
	long	nBCRMode;
};

struct	stIdxEventDataType
{
	long	nPortEvent[MAX_PORT_COUNT];

	long	nAlarmID;
	long	nAlarmCode;
	char	szAlarmModule[20];
	char	szAlarmText[20];
};
	
struct	stIndexerInfoType
{
	long	nIndexerType;       //  1:Indexer(Uni-Type), 2:Loader, 3:Unloader 
	long	nEQState;				 //Indexer EQ State
	long	nProcState;			//	Indexer Process State
	long	nPortCount;			//	Used Indexer Cassette Stage(Port) Count
	long	nRobotCount;		//	Used Robot Count

//	char	szIndexerName[MAX_LAYER_MODULE_ID_LEN+1];//[MAX_UNIT_NAME_LEN+1];

	long	nTimeOut3;		// Index <-> PC Timeout Check
	long	nTimeOut4;
	long	nTimeOutRetry;	// Time Out Retry Count Limit

	long	nCompany;
	long	nDockingType;
	long	nMelChannelNo;

	stIOInfoType	IoInfo;
	stRobotInfoType	RobotInfo;
	stPortInfoType	PortInfo[MAX_PORT_COUNT];	//	Each Port Information

	stIdxReplyDataType	stReplyData;
	stIdxEventDataType	stEventData;
};

///////////////////////////////////////////////////////////////////////////////////////////
//	Indexer Control를 위한 IPC Queue Structure
//	Glass Cycle Move Data
struct	stGlassCycleMoveDataType
{
	long	nCycleMoveType;	//	1	:	
							//	2	:	
							//	3	:	
							//	4	:	
							//	5	:	

	long	nFrom;		//	Port No, Stage No, etc...
	long	nFromSlot;	//	Slot No, 

	long	nTo;		//	Port No, Stage No, etc...
	long	nToSlot;	//	Slot No, 

	long	nThick;		//	Glass Thickness
	long	nSize[2];	//	Glass Size
};

struct	stGlassStepMoveDataType
{
	long	nStepMoveType;	//	Port, Stage, NotUsed, etc...
	long	nFrom;			//	Port No, Stage No, etc...
	long	nFromSlot;		//	Slot No, 

	long	nMotionType;	//	Port, Stage, NotUsed, etc...
};

struct	stPortCmdDataType
{
	long	nPortNo;
	long	nData;
};

struct	stPortEventReplyDataType
{
	long	nPortNo;
	long	nAckCode;
};

struct	stWaitPositionChangeDataType
{
	long	nPortNo;
};

struct	stArmDataClearDataType
{
	long	nArmNo;
};

struct	stGlassThickDataType
{
	long	nPortNo;
	long	nGlassThick;
};

struct	stGlassSizeDataType
{
	long	nPortNo;
	long	nGlassSize[2];
};

/*************************************************************************************
	QUEUE Data - Indexer Signal Structure
/*************************************************************************************/
//	1. Manual Operation Structure
struct	stIndManualOpReqType
{
	long	nCmdCode;	//	1. Machine Command Code
						//	2. Alarm Clear Command Code
	long	nCmdParam;	//	1. Machine Command Code Parameter
						//	2. Alarm Code
	union unionReqManOpType
	{
		long	nAckCode;
		char	szMsg[80+1];

		stWaitPositionChangeDataType	stWaitPositionData;
		stArmDataClearDataType			stArmDataClearData;

		stGlassCycleMoveDataType		stCycleMoveData;
		stGlassStepMoveDataType			stStepMoveData;
		stPortCmdDataType				stPortCmdData;
		stGlassThickDataType			stGlassThickData;
		stGlassSizeDataType				stGlassSizeData;
		stPortEventReplyDataType		stPortEventReplyData;
	}unionReqData;
};

struct	stIndManualOpRlyType
{
	long	nCmdCode;	//
	long	nCmdParam;	//
	union unionRlyManOpType
	{
		long nAckCode;
		// 하기 1개 항목 추가  08- 13
		stPortEventReplyDataType	stPortReply;

	}unionRlyData;
};

//	Indexer IPC용 전용 Queue Define
struct	stT8IDXQueueType
{
	long	nSignal;	//
	long	nParam;		//
	long	nFrom;		//	
	long	nTo;		//	
	long	nFlag;		//

	union	unionDataType
	{
		//	Alarm 발생/해제 관련
		stAlarmOccuredDataType		stAlarmOccuredData;		
		stAlarmTreatedDataType		stAlarmTreatedData;	
		stAlarmClearReqType			stAlarmClearData;	// All Clear 기능 겸용.

		/////////////////////////////////
		//	Indexer Signal Handling Items
		stIndManualOpReqType		stManOpReqData;
		stIndManualOpRlyType		stManOpRlyData;
		
	}unionData;
};
#endif	//T8INDEXER.H