#ifndef	__Queue_h__
#define	__Queue_h__

#include	"HostMsg.h"
#include	"ConstDefine.h"
#include	"IPCSma.h"

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

//	Indexer Signal Structure
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

//	Alarm 발생시 사용
struct	stAlarmOccuredDataType
{
	long	nModuleID;	
	long	nAlarmCode;
	long	nAlarmID;
	long	nAlarmPause;
	char	szUnitID[MAX_UNIT_ID_LEN+1];
	char	szAlarmText[MAX_ALARM_TEXT_LEN+1];
};

//	Alarm 해제시 사용
struct	stAlarmTreatedDataType
{
	long	nModuleID; 
	long	nAlarmCode;
	long	nAlarmID;
	char	szUnitID[MAX_UNIT_ID_LEN+1];
	char	szAlarmText[MAX_ALARM_TEXT_LEN+1];
};

struct stProcessRunType
{
	 long		nPort;		
	 long		nRecipe;
	 long		nGlassCount;
	 long		nSlotNo[MAX_SLOT_COUNT_PER_PORT];
};

struct stUIGlassIDInputType
{
	long nPortNo;
	long nSlotNo;
	char szGlassID[MAX_SLOT_COUNT_PER_PORT+1];
	char szVCRGlassID[MAX_SLOT_COUNT_PER_PORT+1];	//* 05-27
};

// Glass Send To PM(InConv Or HiPassInConv)
struct	stGlassProcessDataType				//* 05-27
{
	long	nRecipeNo;
	long	nPortNo;
	long	nSlotNo;
	long	nGlassNo;    // Unique ID
	long	nGlassSeq;	// 4:Noraml, 5:Start, 6:End
	
	long	nAckCode;	// for reply 
};

struct	stECOChangeType
{
	long	nModuleID;
	long	nEOMD;
	long	nEOV;
};


struct	stEOIDChangeType
{
	long	nEOCount;
	stEOIDDataType	stEOData[MAX_ONLINE_PARAM_COUNT];
};

struct	stECDataType	
{
	long	nECID;
	
	char	szECDefault[MAX_EQ_CONSTANT_VALUE_LEN+1];
	char	szECStopLowLimit[MAX_EQ_CONSTANT_VALUE_LEN+1];
	char	szECStopUpLimit[MAX_EQ_CONSTANT_VALUE_LEN+1];
	char	szECWarnLowLimit[MAX_EQ_CONSTANT_VALUE_LEN+1];
	char	szECWarnUpLimit[MAX_EQ_CONSTANT_VALUE_LEN+1];

};

struct	stECDataSingleType	
{
	long	nECID;
	char	szECDefault[MAX_EQ_CONSTANT_VALUE_LEN+1];
};

struct	stECIDChangeType
{
	long		nModuleID;
	long		nECCount;

	stECDataType	stECData[MAX_ECID_MULTI_COUNT];
	stECDataSingleType	stECData_Single[MAX_ECID_SINGLE_COUNT];
};

///////////////////////////////////////////////////////////////////
//*  Manual Operation Signal Define
struct	stTerminalMsgType
{
	long	nModuleNo;
	long	nMsgCnt;
	char	szMsg[MAX_TEXT_MESSAGE_LEN+1];
};

struct	stBuzzerCtrlType
{
	long	nModuleNo;
	BOOL	bOnOff;		// FALSE : Buzzer Off, TRUE : Buzzer On [6/13/2003]
};

struct	stMachineCmdType
{
	long	nModuleNo;
	long	nCmdData;
	char	szReasonCode[MAX_EQ_CMD_RCODE_LEN+1];
};

struct	stOperatorCallType
{
	long	nModuleNo;		//  [6/24/2003]
	BOOL	bOnOff;			// Operator Call On/Off [6/13/2003]
};

struct	stTowerLampType
{
	long	nModuleNo;
	long	nCmdData;		// enum 
};

struct	stAlarmClearReqType	// All Clear 기능 겸용.
{
	long	nAlarmID;		//Alarm ID가 9999 Alarm All Clear[6/13/2003]
	long	nModuleID; 	//설비군별 Module ID
};

struct	stAllDataInitType
{
	long	nModuleNo;
};

// Event define 
struct	stGlassShiftType
{
	long	nModuleID;
	stPanelInfoType	stPanelInfo;
};

struct	stGlassUnscrapType
{
	short	shAckCode;
	//long	nPortNo;	// 2006/12/28 Delete
	//long	nSlotNo;	// 2006/12/28 Delete
	long	nModuleID;	//	2006/01/26 Add
	long	nUniqueID[4];	// 2006/12/28 Add
	long	nOwnGlassNo;

	stPanelInfoType	stPanelInfo;
};

//	Glass Delete Event & Judgement Event 공통 사용
struct	stGlassScrapType
{
	short	shAckCode;
	long	nModuleID;	//	2006/01/26 Add
	long	nUniqueID[4];	// 2006/12/28 Add

	char	szPreJudgement[MAX_JUDGEMENT_RESULT_LEN+1];

	stPanelInfoType	stPanelInfo;
};

struct	stRecipeDownloadType
{

	short	shAckCode;
	long	nModuleID;
	long	nPosNo;
	long	nSubPosNo;
	long	nRCPNo;				// Main(Flow) Recipe No [6/13/2003]
};

struct	stGlassEventFromPMType	//* 05-27 Glass Event 발생시 사용됨.
{								//* Struct Name Change(stGlassMoveFromPMType)
	long	nGlassInfo;
	long	nPortNo;
	long	nSlotNo;
	long	nRcpNo;			// Main Rcp No[6/23/2003]
	
	long	nModuleNo;		// Glass Event 발생 Module ID [6/13/2003]	
	long	nPosNo;			//	PM Unit Position Index
	long	nSubPosNo;		// Buffer Slot No.
	long	nEventId;		// for NI
	
	char	szGlassID[MAX_PANEL_ID_LEN + 1];	// NI Inpector Only* 05-27 PM Module 발생시 Empty
	char	szVCRGlassID[MAX_PANEL_ID_LEN + 1]; // NI Inpector Only* 05-27 PM Module 발생시 Empty
};

// Glass Tracking  [6/27/2003]
struct	stGlassTrackingDataType
{
	long	nPortNo;
	long	nSlotNo;
	long	nGlassSeq;		// 4:Start/5:Normal/6:End
	long	nFlowPattern;	// 1: Etch-Strip, 2:Strip Only 3:Etch Only
};

// UI -> Host [7/10/2003]
struct	stRecipeChangeType
{
	long	nChangeMode;	//	1 : Create, 2 : Delete, 3 : Modify
	long	nRcpType;		//	1 : Main Flow,	2 : Etcher, 3 : Strip
	long	nPPIDType;		//	1 : Normal,	2 : Mapping Table
	long	nByWho;
	long	nRevCount;		//	Revision 회수
	char	szChangedTime[MAX_DATE_TIME_LEN+1];
	long	nPPIDNo;		// PPID 번호
	char	szPPID[MAX_PPID_LEN+1];
	long	nRecipeNo[MAX_LAYER1_MODULE_COUNT];	//세부 Recipe 번호
	long	nTactTime;
};

// Host -> UI
struct  stModuleRecipeChangeType
{
    long    nEventType;  // Create/Change/Delete
    long    nPPIDType;   // 예비용 FlowRecipe = 1/SubRecipe = 2
    long    nModuleNo; 
    long    nRecipeNo;
};

// Host -> UI
struct	stFlowRecipeChangeType
{
	long  nEventType;  // Create/Delete/Modify
	long	nFlowRcpNo;
	long	nLineTact;
};

// Chemical Change [9/16/2003]
struct	stChemicalChangeCmdType
{
	long	nModuleNo;
	long	nTankType;		// 1-Strip, 2-Etch , 3-Hno3 

	bool	bOnOff;			// Chemical Change On/Off (Don't Used)
};

struct	stGlassInfoForGUIType
{
	char	szSlotNo[MAX_SLOT_COUNT_PER_PORT+1];
	char	szGlassID[MAX_PANEL_ID_LEN+1];
	char	szBatchID[MAX_BATCH_ID_LEN+1];
	char	szProdType[MAX_PRODUCT_TYPE_LEN+1];	// Add ms
	char	szPPID[MAX_PPID_LEN+1];
	char	szProcessID[MAX_PROCESS_ID_LEN+1];
	char	szStepID[MAX_STEP_ID_LEN+1];
	char	szInsFlag[MAX_INSPECT_FLAG_LEN+1];
	long	nThick;
};

struct	stGlassRunInfoForGUIType
{
	char	szPortID[MAX_PORT_ID_LEN+1];
	long	nGlassCount;
	stGlassInfoForGUIType	stGlass[MAX_SLOT_COUNT_PER_PORT];
};

/*
 * 노광기 Job Reserve Data 전용 
 */
struct	stProcAvailCheckType
{
	long	nModuleType;	//	0 - All, != 0  Each Module

	//	Old
	char	szOldBatchID[MAX_BATCH_ID_LEN+1];
	char	szOldProcessID[MAX_PROCESS_ID_LEN+1];
	char	szOldStepID[MAX_STEP_ID_LEN+1];
	char	szOldPPID[MAX_PPID_LEN+1];
	long	nOldGlassSize[2];
	long	nOldGlassThick;
	long	nOldFlowRecipe;
	long	nRemainCount;
	long	nOldJobUniqueID;

	//	New
	char	szNewBatchID[MAX_BATCH_ID_LEN+1];
	char	szNewProcessID[MAX_PROCESS_ID_LEN+1];
	char	szNewStepID[MAX_STEP_ID_LEN+1];
	char	szNewPPID[MAX_PPID_LEN+1];
	long	nNewGlassSize[2];
	long	nNewGlassThick;
	long	nNewFlowRecipe;
	long	nBatchCount;
	long	nNewJobUniqueID;
	long	nRequestID;
};


//	Recipe Down Load Data : EQ -> CIM 보고용
struct	stPPIDTransferType
{
	long	nModuleType;
	char	szPPID[MAX_PPID_LEN+1];
};


struct	stEQSpecialCtrlEventType
{
	long	nModuleNo;
	long	nEventId;		// 자동화 Special Control Event ID로 사용

	long	nItemId;		// [H/S Valid Check Fail]
							// 1:Duplicate, 2:Omission, 3:Availability

	long	nItemVal;		// [H/s Valid Check Fail]
							// - Duplicate    : H_PANELID(1)+E_PANELID(2)+UNIQUEID(4)
							// - Omission     : UNIQUEID(1)+H_PANELID(2)+BATCHID(4)+DEVICEID(8)+STEPID(16)+PPID(32)+GLASS THICKNESS(64)
							// - Availability : PPID(1)+GLASS SIZE(2)+GLASS THICKNESS(4)+FLOW RECIPE(8)
							// [기타]
							// 0:OFf, 1:ON
	long	nHSNo;
	char	szItemValue[MAX_SPEC_CONTROL_DATA_VALUE_LEN];
};

struct stGECDCrackEventType
{
	long	nModuleNo;
	long	nEventID;

	long	nGECDEventID;
	long	nSemesUniqID;
};

////////////////////////////////////////////////////////////////////////////////////////////
struct	stQueueRootType
{
	long	nSignal;	
	long	nParam;	
	long	nFrom;	
	long	nTo;	
	long	nFlag;	

	union	unionDataType
	{
		//	EQ 공통 사용 Signal Struct
		float				fSetData;
		long				nValue;

		//	Alarm 발생/해제 관련
		stAlarmOccuredDataType		stAlarmOccuredData;		
		stAlarmTreatedDataType		stAlarmTreatedData;	

		//	Terminal Service 관련
		stTerminalMsgType			stTerminalMsg;

		/////////////////////////////////
		//	Indexer Signal Handling Items

		// 하기 4개 항목 수정 08-13
		stIndManualOpReqType		stManOpReqData;
		stIndManualOpRlyType		stManOpRlyData;


		// 추가 부문
		stProcessRunType			stProcessRun;

		/////////////////////////////////
		//	inline Plc Signal Handling Items

		stGlassProcessDataType		stGlassProcessData;		// Process All Data ( Low/High Glass Transfer Data )
		stRecipeDownloadType		stPMProcDataReq;		// Etcher/U-Turn/Strip/NI 제어기에서 CIM으로 Process Data 요구시 사용 [6/13/2003]
		
		stEQSpecialCtrlEventType	stEQSpecCtrlEventData;

		stGECDCrackEventType		stGECDCrackEvent;

		stGlassShiftType			stGlassShiftData;
		stGlassUnscrapType			stGlassUnscrapData;
		stGlassScrapType			stGlassScrapData;

		stECOChangeType				stECOChange;

		stEOIDChangeType			stEOIDChange;
		stECIDChangeType			stECIDChange;
		
		stBuzzerCtrlType			stBuzzerCtrl;	// Buzzer On/Off Control [6/13/2003]
		stMachineCmdType			stMachineCmd;	// Start/Stop/Pause/Resume/Auto/Manual/Reset [6/13/2003]
		stOperatorCallType			stOperatorCall;	// Tower Lamp 전체 On/Off 및 Buzzer On/Off  [6/13/2003]
		stAlarmClearReqType			stAlarmClearData;	// All Clear 기능 겸용.
		stAllDataInitType			stAllDataInit;


		stGlassTrackingDataType		stGlassTrackingData;

		stRecipeChangeType			stRcpChangeData;
		stMainRecipeTableType		stMainRcpTbl;
		stCleanerRecipeDataType		stClnData;
//1616		stInterfaceRecipeDataType	stINFData;
//1616		stDevelopRecipeDataType		stDevData;
		// 07-03-19: Add (Recipe Change by Host)
		stEtchRecipeDataType		stEtchData;
		stStripRecipeDataType		stStripData;
		
		// Chemical Change [9/16/2003]
		stChemicalChangeCmdType		stChemicalChange;

		stProcessEndDataType		stProcEndData;

		// H/S Status Report 
		stEQSpecialCtrlEventType	stEQSpecialCtrlEvent;

		// 2010-08-24 add
		stGlassSendFailType			stGlassSendFailEvent;
		
		stRPCDataType				stRPCData;

		//	Host Signal Handling Items
		stHsmsParamType		HostParam;
		stHsmsMsgHeadType	MsgHead;
		stS1F1Type			s1f1ToHost;
		stS1F1Type			s1f1FromHost;

		stS1F2ToHostType	s1f2ToHost;			//	EQ -> Host Task로 보낼 Data
		stS1F2FromHostType	s1f2FromHost;

		stS1F3Type			s1f3FromHost;		//	Host Task -> EQ로 보낼 Data
		stS1F4Type			s1f4ToHost;			//	EQ -> Host Task로 보낼 Data

		stS1F5Type			s1f5FromHost;		//	Host Task -> EQ로 보낼 Data
		stS1F6SFCD1Type		s1f6sfcd1ToHost;	//	Online Parameter
		stS1F6SFCD2Type		s1f6sfcd2ToHost;	//	Port Status
		stS1F6SFCD3Type		s1f6sfcd3ToHost;	//	Glass Tracking
		stS1F6SFCD4Type		s1f6sfcd4ToHost;	//	Module Status
		stS1F6SFCD5Type		s1f6sfcd5ToHost;	//	Cassette Tracking
		stS1F6SFCD7Type		s1f6sfcd7ToHost;	//	Standard Tact Time
		stS1F6SFCD8Type		s1f6sfcd8ToHost;	//	Current Interlock
		stS1F6SFCD21Type	s1f6sfcd21ToHost;	//	Working Status
		stS1F6SFCD30Type	s1f6sfcd30ToHost;	//	Software Request

		stS1F11Type			s1f11FromHost;
		stS1F12Type			s1f12ToHost;

		stS1F15Type			s1f15FromHost;
		stS1F16Type			s1f16ToHost;

		stS1F17Type			s1f17FromHost;
		stS1F18Type			s1f18ToHost;

		stS2F15Type			s2f15FromHost;
		stS2F16Type			s2f16ToHost;

		stS2F23Type			s2f23FromHost;
		stS2F24Type			s2f24ToHost;

		stS2F25Type			s2f25FromHost;
		stS2F26Type			s2f26ToHost;

		stS2F29Type			s2f29FromHost;
		stS2F30Type			s2f30ToHost;

		stS2F31Type			s2f31FromHost;
		stS2F32Type			s2f32ToHost;

		//	Remote Command
		stS2F41EQCmdType		s2f41EQCmdFromHost;
		stS2F41JudgementCmdType	s2f41JudgeCmdFromHost;
		
		stS2F42Type			s2f42ToHost;
		//stS2F41ProcCmdType		s2f41ProcCmdFromHost;
		//stS2F41PortCmdType		s2f41PortCmdFromHost;
		//stS2F41EQCmdType		s2f41EQCmdFromHost;
		//stS2F41JudgementCmdType	s2f41JudgeCmdFromHost;
		//stS2F41JudgeDownCmdType	s2f41JudgeDownCmdFromHost;
		
		//stS2F42ProcCmdType		s2f42ProcCmdToHost;
		//stS2F42PortCmdType		s2f42PortCmdToHost;
		//stS2F42EQCmdType		s2f42EQCmdToHost;
		//stS2F42JudgementCmdType s2f42JudgeToHost;
		//stS2F42JudgeDownCmdType s2f42JudgeDownToHost;

		stS2F101Type			s2f101FromHost;
		stS2F102Type			s2f102ToHost;

		stS2F103Type			s2f103FromHost;
		stS2F104Type			s2f104ToHost;

		stS3F1Type				s3f1FromHost;
		stS3F2Type				s3f2ToHost;

		stS3F101Type			s3f101FromHost;
		stS3F102Type			s3f102ToHost;

		stS3F201Type			s3f201FromHost;
		stS3F202Type			s3f202ToHost;

		stS5F1Type				s5f1ToHost;
		stS5F2Type				s5f2FromHost;

		stS5F5Type				s5f5FromHost;
		stS5F6Type				s5f6ToHost;

		stS5F101Type			s5f101FromHost;
		stS5F102Type			s5f102ToHost;
		stS5F103Type			s5f103FromHost;
		stS5F104Type			s5f104ToHost;
		stS5F105Type			s5f105ToHost;

		stS6F1Type				s6f1ToHost;
		stS6F2Type				s6f2FromHost;

		stS6F3Type				s6f3ToHost;
		stS6F4Type				s6f4FromHost;

		stS6F11ProcessType		s6f11ProcessToHost;
		stS6F11GlassType		s6f11GlassToHost;
		stS6F11PortType			s6f11PortToHost;
		stS6F11EQType			s6f11EQToHost;
		stS6F11EQParamType		s6f11EQParamToHost;
		stS6F11SpecCtrlType		s6f11SpecCtrlToHost;
		stS6F11MaterialType		s6f11MaterialToHost;
		stS6F11StandardDataType s6f11StandardDataToHost;
		stS6F11PanelIDValidationType s6f11PanelIDValidationToHost;

		stS6F11SpecificStepType	s6f11SpecificStep;

		stS6F11SoftVersionType	s6f11SoftversionToHost;  // 2010-08-24 add

		stS6F12Type				s6f12FromHost;

		stS6F13GlassType		s6f13GlassToHost;
		stS6F13CassetteType		s6f13CassetteToHost;
		stS6F14Type				s6f14FromHost;

		stS7F1Type              s7f1FromHost;
		stS7F2Type              s7f2ToHost;
		stS7F9Type              s7f9FromHost;
		stS7F10Type             s7f10ToHost;
		stS7F23Type				s7f23FromHost;
		stS7F24Type				s7f24ToHost;

		stS7F25Type				s7f25FromHost;
		stS7F26Type				s7f26ToHost;

		stS7F33Type				s7f33FromHost;	//	20041208
		stS7F34Type				s7f34ToHost;	//	20041208

		stS7F101Type			s7f101FromHost;
		stS7F102Type			s7f102ToHost;

		stS7F103Type			s7f103FromHost;
		stS7F104Type			s7f104ToHost;

		stS7F105Type			s7f105FromHost;
		stS7F106Type			s7f106ToHost;

		stS7F107Type			s7f107ToHost;
		stS7F108Type			s7f108FromHost;

		stS7F109Type			s7f109FromHost;
		stS7F110Type			s7f110ToHost;

		stS7F111Type			s7f111FromHost;
		stS7F112Type			s7f112ToHost;

		stS9F1Type				s9f1ToHost;
		stS9F3Type				s9f3ToHost;
		stS9F5Type				s9f5ToHost;
		stS9F7Type				s9f7ToHost;
		stS9F9Type				s9f9ToHost;

		stS9F11Type				s9f11FromHost;
		stS9F11Type				s9f11ToHost;

		stS10F3Type				s10f3FromHost;
		stS10F4Type				s10f4ToHost;

		stS10F9Type				s10f9FromHost;
		stS10F10Type			s10f10ToHost;

		stS10F101Type			s10f101FromHost;
		stS10F102Type			s10f102ToHost;

		stS64F1Type				s64f1FromHost;
		stS64F2Type				s64f2ToHost;

		stS16F101Type			s16f101FromHost;
		stS16F102Type			s16f102ToHost;

		stS16F103Type			s16f103FromHost;
		stS16F104Type			s16f104ToHost;
		
		stS16F105Type			s16f105ToHost;
		stS16F106Type			s16f106FromHost;
		
		stS16F107Type			s16f107ToHost;
		stS16F108Type			s16f108FromHost;

		//	20050817
		stProcAvailCheckType	stProcAvailCheck;

		//	20051018
		stPPIDTransferType		stEvtPPIDData;
		
		stFlowRecipeChangeType		stFlowRecipeChange;
		stModuleRecipeChangeType	stModuleRecipeChange;
	}unionData;
};
/*

//	Indexer IPC용 전용 Queue Define
struct	stIndexerQueueType
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
*/
//	20050818
struct	stPlcQueueType
{
	long	nSignal;		//Event Signal
	long	nParam;
	long	nFrom;		
	long	nTo;
	long	nFlag;

	union	unionPlcDataType
	{
		stGlassShiftType			stGlassShiftData;
		stGlassUnscrapType		stGlassUnscrapData;
		stGlassScrapType			stGlassScrapData;

		stPPIDTransferType			stEvtPPIDData;

		stGlassProcessDataType		stGlassProcessData;		// Process All Data ( Low/High Glass Transfer Data )

		stRecipeDownloadType		stPMProcDataReq;		// Etcher/U-Turn/Strip/NI 제어기에서 CIM으로 Process Data 요구시 사용 [6/13/2003]
		
		//PLC Signal Handling Items
		stGlassEventFromPMType		stGlassEventData;		// Each Position Glass Event
		stEQSpecialCtrlEventType	stEQSpecCtrlEventData;
		
		stECOChangeType				stECOChange;

		stEOIDChangeType			stEOIDChange;
		stECIDChangeType			stECIDChange;
		
		stTerminalMsgType			stTerminalMsg;
		stBuzzerCtrlType			stBuzzerCtrl;		// Buzzer On/Off Control [6/13/2003]
		stMachineCmdType			stMachineCmd;		// Start/Stop/Pause/Resume/Auto/Manual/Reset [6/13/2003]
		stOperatorCallType			stOperatorCall;		// Tower Lamp 전체 On/Off 및 Buzzer On/Off  [6/13/2003]
		stAlarmClearReqType			stAlarmClearData;	// All Clear 기능 겸용.
		stAllDataInitType			stAllDataInit;

		stAlarmOccuredDataType	stAlarmOccuredData;
		stAlarmTreatedDataType		stAlarmTreatedData;

		stRecipeChangeType			stRcpChangeData;	//GUI -> HOST
		stChemicalChangeCmdType		stChemicalChange;
		
		stProcAvailCheckType		stProcAvailCheck;
		
		stGlassSendFailType			stGlassSendFail;    // 2010-08-24 add

	}unionData;
};
/*
struct	stTempCtrlQueue
{
	long	lngSignal;
	long	lngCtrlModule;		//	2 : Etch, 4 : Strip
	long	lngNode;			//	channel no
	long	lngData;
	float	fltData;
};
*/
#endif	//	__Queue_h__
