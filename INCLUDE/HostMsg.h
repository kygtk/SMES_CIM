/*****************************************************************************************************
		T8-1 Host Message Define Version Date : 2007-03-05
 *****************************************************************************************************/

#ifndef	_HostMsg_H_
#define	_HostMsg_H_

#pragma once
#include "windows.h"
#include "EventSignal.h"
#include "ConstDefine.h"

// EQ Layout Concept
// 1. EQ Level
// 2. 1'st Level	: Loader, Etcher, Strip, Unloader
// 3. 2'nd Level	: each unit

//	Hsms Message 
struct stHsmsMsgHeadType
{
	char	szName[20+1];
	long	nWaitBit;
	long	nStreamNo;
	long	nFunctionNo;
	long	nItemNo;
	long	nSystemByte;
	long	nSessionID;
};

struct stHsmsMsgDataType
{
	char	szName[20+1];
	char	cType;
	long	nSize;
	char*	Value;
};

struct	stHsmsMsgType
{
	stHsmsMsgHeadType		MsgHead;
	stHsmsMsgDataType		*pMsgBody;
};

//	Host Parameter
struct stHsmsParamType
{
	char szIPAddress[16+1];
	long nPort;
	long nT3;
	long nT5;
	long nT6;
	long nT7;
	long nT8;
	BOOL bPassive;

	long nDeviceID;
};
////////////////////////////////////////////
struct	stEQObjectDataType
{
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nMCMD;
	long	nEQState;
	long	nProcessState;
	long	nByWho;
	char	szOperID[MAX_OPERATOR_ID_LEN+1];
};

struct	stPortObjectDataType
{
	char	szPortID[MAX_PORT_ID_LEN+1];
	long	nEQState;
	long	nPortState;
	long	nPortType;
	char	szPortMode[MAX_PORT_MODE_LEN+1];
	long	nSortType;
	long	nCST_Demand;
};

struct	stCassetteObjectDataType
{
	char	szCassetteID[MAX_CASSETTE_ID_LEN+1];
	char	szCassetteType[4+1];
	char	szMapSlotInfo[MAX_SLOT_INFO_LEN+1];
	char	szCurSlotInfo[MAX_SLOT_INFO_LEN+1];
	long	nBatchOrder;						//	add 20040621
};

// SFCD = 3	: Glass Tracking
struct	stGlassMainDataType
{
	char	szHGlassID[MAX_PANEL_ID_LEN+1];
	char	szEGlassID[MAX_PANEL_ID_LEN+1];
	char	szSlotNo[MAX_SLOT_NUMBER_LEN+1];
	char	szProcessID[MAX_PROCESS_ID_LEN+1];
	char	szProductID[MAX_PRODUCT_ID_LEN+1];
	char	szStepID[MAX_STEP_ID_LEN+1];
	char	szBatchID[MAX_BATCH_ID_LEN+1];
	char	szProductType[MAX_PRODUCT_TYPE_LEN+1];
	char	szProductKind[MAX_PRODUCT_KIND_LEN+1];
	char	szPPID[MAX_PPID_LEN+1];
	char    szFlowID[MAX_FLOW_ID_LEN];
	long    nPanelSize[2]; 
	long	nThickness;
	long    nCompCount;
	char	szCellGrade[MAX_CELL_GRADE_LEN+1];
	char	szJudgement[MAX_JUDGEMENT_RESULT_LEN+1];
	char	szCode[MAX_JUDGEMENT_CODE_LEN+1];
	char	szCount1[MAX_GLASS_COUNT_LEN+1];
	char	szCount2[MAX_GLASS_COUNT_LEN+1];
	char	szPanelPosition[MAX_GLASS_POSITION_LEN+1];
	long	nPanelState;
	long    nFlowHistory[MAX_FLOW_HISTORY_LEN];
	long	nUniqueID[MAX_UNIQUE_ID_LEN];
	char	szReadingFlag[MAX_READING_FLAG_LEN+1];
	char    szMultiUse[MAX_MULTI_USE_LEN+1];
};

//@ Flow Group
struct	stGlassSub1DataType
{
	short   shFlowGroup[MAX_FLOW_GROUP_LEN]; 
};

//@ Usable Chamber
struct	stGlassSub2DataType
{
	short   shUsableChamber[MAX_USABLE_CHAMBER_LEN]; 
};

//@ Glass Pair Information
struct	stGlassSub3DataType
{
	char	szPairHGlassID[MAX_PANEL_ID_LEN+1];
	char	szPairEGlassID[MAX_PANEL_ID_LEN+1];
	char    szPairProcuctID[MAX_PRODUCT_ID_LEN+1];
	char	szPairGrade[MAX_CELL_GRADE_LEN+1];
};

//@ Glass Sub Information
struct	stGlassSub4DataType
{
	char	szSubPanelID[MAX_PANEL_ID_LEN+1];
	char	szSubJudgement[MAX_JUDGEMENT_RESULT_LEN+1];
	char    szSubJudgeCode[MAX_JUDGEMENT_CODE_LEN+1];
};

//@ Glass Specific Information
struct	stGlassSub5DataType
{
	char	szItemName[MAX_EQ_SPECIFIC_ITEM_NAME+1];
	char	szItemValue[MAX_EQ_SPECIFIC_ITEM_VALUE+1];
};

//@ Bit Signal Information
struct  stGlassSub6DataType
{
	char	szFlagName[MAX_BIT_SIGNAL_FLAG_NAME+1];
	//short	shBitSignal;
	long	nVaule;
};
struct	stGlassObjectType
{
	stGlassMainDataType	stMainData;
	long	nSub1Count;
	stGlassSub1DataType	stSub1Data[MAX_GLASS_SUB_DATA_COUNT];	//Flow Group
	long	nSub2Count;
	stGlassSub2DataType	stSub2Data[MAX_GLASS_SUB_DATA_COUNT];	//Usable Chamber
	long	nSub3Count;
	stGlassSub3DataType	stSub3Data[MAX_GLASS_SUB_DATA_COUNT];	//Pair Glass Inform
	long	nSub4Count;
	stGlassSub4DataType	stSub4Data[MAX_GLASS_SUB_DATA_COUNT];	//Sub Glass Inform
	long	nSub5Count;
	stGlassSub5DataType	stSub5Data[MAX_GLASS_SUB_DATA_COUNT];	//Specific Glass Inform
	long	nSub6Count;
	stGlassSub6DataType	stSub6Data[2];	//Bit Signal Inform
};

///////////////////////////////////////////
//	S1F1	: Only Siganl ( EQ <-> Host )
//	Are You There Request
struct	stS1F1Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
};

///////////////////////////////////////////
//	S1F2	:	EQ -> Host
//	Online Data
struct	stS1F2ToHostType
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nOnlineMode;	// 1: OFFLINE(Not Used) 2: LOCAL(Not Used), 3: REMOTE 
};

//	S1F2	:	Host -> EQ
struct	stS1F2FromHostType
{
	stHsmsMsgHeadType	MsgHead;

	long	nListCount;		//	통상 nListCount = 0로 Host가 Alive 상태임을 EQ에 알린다.
};

///////////////////////////////////////////
//	S1F3	: EQ <- Host의 경우
//	Selected Equipment Status Request Structure
struct	stS1F3Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nSVCount;
	long	nSVID[MAX_SVID_COUNT];
};

///////////////////////////////////////////
//	S1F4	: EQ -> Host의 경우
//	Selected Equipment Status Data Structure
struct	stSVDataType
{
	long	nSVID;							//	사용되는 nSVID 범위는 1 ~ 53, Spare(54~100)
	char	szSV[MAX_STATUS_VALUE_LEN+1];
	char	szSVNAME[MAX_STATUS_VALUE_NAME_LEN+1];
};

struct	stS1F4Type
{
	stHsmsMsgHeadType	MsgHead;

	char			szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long            nTMACK;
	char			szReason[MAX_MESSAGE_REASON_LEN+1];
	long			nSVCount;
	stSVDataType	stSVData[MAX_SVID_COUNT];
};

///////////////////////////////////////////
//	S1F5	: Host -> EQ인 경우
//	Formatted Status Request
struct	stS1F5Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nSFCD;	//	1 : Equipment Online Parameter
					//	2 : Port Status(Not Used)
					//	3 : Glass Tracking
					//	4 : Module(Unit) Status
};

///////////////////////////////////////////
//	S1F6	: EQ -> Host인 경우
//	Formatted Status Data
//	SFCD = 1인 경우	On Line Parameter
struct	stS1F6SFCD1DataType
{
	char	szEOMD[MAX_EOMD_LEN+1];
	long	nEOV;		//	OnLine Parameter Value
};

struct	stS1F6SFCD1EOIDType
{
	long	nEOID;		//	Online Parameter ID -	1:Component Trace, 
												//	2:Equipment State Trace
												//	3:Equipment Process State Trace
												//	4:Equipment Processing State Lapse Time Trace Mode
												//	7:Load Reject
												//	8:Robot Access Order
												//	10:Unload Port Glass Sort Type
												//	12:NG Port Operation Mode
												//	13:Judgement Mode
												//	30:Set of QTY count	
												//	31:Job End Spool for last 2 Job
	long	nEOModeCount;		//	OnLine Parameter Mode Count
	stS1F6SFCD1DataType	Data[MAX_ONLINE_PARAM_MODE_COUNT];
};

//	1 : Equipment Online Parameter
struct	stS1F6SFCD1Type
{
	stHsmsMsgHeadType	MsgHead;

	char		szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long		nSFCD;					//	1
	long        nTMACK;
	char		szReason[MAX_MESSAGE_REASON_LEN+1];
	long		nEOCount;				//	1
	stS1F6SFCD1EOIDType	stEOID[MAX_ONLINE_PARAM_COUNT];	
};

struct	stS1F6SFCD2PortType
{
	stPortObjectDataType		stPortData;
	stCassetteObjectDataType	stCassData;
};

struct	stS1F6SFCD2Type
{
	stHsmsMsgHeadType	MsgHead;

	char		szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long        nTMACK;         // ?
	long		nSFCD;			//	2
	long		nPortCount;		//	1
	stS1F6SFCD2PortType	stPort[MAX_PORT_COUNT];
};

struct	stS1F6SFCD3ModuleType		
{
	char		szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long		nGlassCount;
	//stGlassObjectType	stGlass[MAX_LAYER2_MODULE_COUNT];
	stGlassObjectType	stGlass[10];
};

struct	stS1F6SFCD3Type
{
	stHsmsMsgHeadType	MsgHead;

	char		szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long		nSFCD;			//	3
	long        nTMACK;         
	char		szReason[MAX_MESSAGE_REASON_LEN+1];
	long		nModuleCount;
	//stS1F6SFCD3ModuleType	stModule[MAX_LAYER2_MODULE_COUNT];
	stS1F6SFCD3ModuleType	stModule[50];
};

struct	stS1F6SFCD4AlarmDataType
{
	long	nAlarmCode;
	long	nAlarmID;
	char	szAlarmText[MAX_ALARM_TEXT_LEN+1];
	char    szAlarmDateTime[MAX_DATE_TIME_LEN+1];
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
};

// SFCD = 4	: Module(Unit) Status
struct	stS1F6SFCD4Layer2ModuleType		//	Unit Level
{
	char		szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long		nEQState;
	long		nProcessState;
	char		szPMCode[MAX_PM_CODE_LEN+1];
	char		szPasueCode[MAX_PAUSE_CODE_LEN+1];
	
	long		nAlarmCnt;
	stS1F6SFCD4AlarmDataType	stLayer2LevelAlarm;
};

struct	stS1F6SFCD4Layer1ModuleType		//	Control Module Level
{
	char		szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long		nEQState;
	long		nProcessState;
	char		szPMCode[MAX_PM_CODE_LEN+1];
	char		szPasueCode[MAX_PAUSE_CODE_LEN+1];

	long		nModuleCount;

	long		nAlarmCnt;
	//stS1F6SFCD4AlarmDataType	stLayer1LevelAlarm[MAX_WAIT_ALARM_COUNT];
	stS1F6SFCD4AlarmDataType	stLayer1LevelAlarm;

	stS1F6SFCD4Layer2ModuleType	stModule[MAX_LAYER2_MODULE_COUNT];
};

struct	stS1F6SFCD4Type					//	EQ Level
{
	stHsmsMsgHeadType	MsgHead;

	long		nSFCD;		//	4
	long        nTMACK;
	char		szReason[MAX_MESSAGE_REASON_LEN+1];

	long		nEQCount;

	char		szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long		nEQState;
	long		nProcessState;
	char		szPMCode[MAX_PM_CODE_LEN+1];
	char		szPasueCode[MAX_PAUSE_CODE_LEN+1];
	
	long		nModuleCount;

	long		nAlarmCnt;
	//stS1F6SFCD4AlarmDataType	stEQLevelAlarm[MAX_WAIT_ALARM_COUNT];
	stS1F6SFCD4AlarmDataType	stEQLevelAlarm;

	stS1F6SFCD4Layer1ModuleType	stModule[MAX_LAYER2_MODULE_COUNT];
};

struct	stS1F6SFCD5ReportType
{
	long	nReportID;
	stEQObjectDataType	stEQData;
};

struct	stS1F6SFCD5Type
{
	stHsmsMsgHeadType	MsgHead;

	char		szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long		nSFCD;		//	5
	long        nTMACK;
	stS1F6SFCD5ReportType		stReport0;

	long	nCassetteCount;
	long	nReportID[MAX_PORT_COUNT];
	stCassetteObjectDataType	stCassData[MAX_PORT_COUNT];
};
struct	stS1F6SFCD6Type
{
	stHsmsMsgHeadType	MsgHead;

	char		szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long		nSFCD;		//	5
	long        nTMACK;

	long	nCassetteCount;
	long	nReportID[MAX_PORT_COUNT];
	stCassetteObjectDataType	stCassData[MAX_PORT_COUNT];
};

struct	stS1F6SFCD7Type					//	Standard Tact
{
	stHsmsMsgHeadType	MsgHead;

	char		szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long		nSFCD;		//	7
	long        nTMACK;

	long        nModuleCount;
	long		nEQState;
	long		nProcessState;
	long		nOnlineMode;  //MCMD
	long		nStandardTact;
};

struct	stModuleInterLockDataType
{
	char	szItemName[MAX_EQ_INTERLOCK_ITEM_NAME_LEN+1];
	char	szItemValue[MAX_EQ_INTERLOCK_ITEM_VALUE_LEN+1];
	char	szRelatedModuleID[MAX_EQ_INTERLOCK_RELATED_MODULEID+1];
};

struct	stS1F6SFCD8DataType
{
	char		szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long        nInterLockCount;

	stModuleInterLockDataType	stEQInterLock[MAX_EQ_CURRENT_INTERLOCK_COUNT];
};

struct	stS1F6SFCD8Type					//	Current InterLock
{
	stHsmsMsgHeadType	MsgHead;

	char		szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long		nSFCD;		//	8
	long        nTMACK;

	long		nEQState;
	long		nProcessState;
	long		nOnlineMode;  //MCMD
	long        nModuleCount;

	stS1F6SFCD8DataType   stS1F6SFCD8Data[MAX_LAYER1_MODULE_COUNT];
};

struct	stS1F6SFCD16DataType
{
	char	szProcessID[MAX_PROCESS_ID_LEN+1];
	char	szStepID[MAX_STEP_ID_LEN+1];
	char	szBatchID[MAX_BATCH_ID_LEN+1];
	long	nGlsCnt;
};

struct	stS1F6SFCD16Type
{
	stHsmsMsgHeadType	MsgHead;

	long		nSFCD;		//	21
	long        nTMACK;
	char		szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long		nBatchCount;
	stS1F6SFCD16DataType	stData[MAX_WORKING_STATE_BATCH_COUNT];
};
struct	stS1F6SFCD21DataType
{
	char	szProcessID[MAX_PROCESS_ID_LEN+1];
	char	szStepID[MAX_STEP_ID_LEN+1];
	char	szBatchID[MAX_BATCH_ID_LEN+1];
	long	nGlsCnt;
};

struct	stS1F6SFCD21Type
{
	stHsmsMsgHeadType	MsgHead;

	long		nSFCD;		//	21
	long        nTMACK;
	char		szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long		nBatchCount;
	stS1F6SFCD21DataType	stData[MAX_WORKING_STATE_BATCH_COUNT];
};
struct	stS1F6SFCD22DataType
{
	char	szProcessID[MAX_PROCESS_ID_LEN+1];
	char	szStepID[MAX_STEP_ID_LEN+1];
	char	szBatchID[MAX_BATCH_ID_LEN+1];
	long	nGlsCnt;
};

struct	stS1F6SFCD22Type
{
	stHsmsMsgHeadType	MsgHead;

	long		nSFCD;		//	21
	long        nTMACK;
	char		szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long		nBatchCount;
	stS1F6SFCD22DataType	stData[MAX_WORKING_STATE_BATCH_COUNT];
};
struct	stS1F6SFCD23DataType
{
	char	szProcessID[MAX_PROCESS_ID_LEN+1];
	char	szStepID[MAX_STEP_ID_LEN+1];
	char	szBatchID[MAX_BATCH_ID_LEN+1];
	long	nGlsCnt;
};

struct	stS1F6SFCD23Type
{
	stHsmsMsgHeadType	MsgHead;

	long		nSFCD;		//	21
	long        nTMACK;
	char		szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long		nBatchCount;
	stS1F6SFCD23DataType	stData[MAX_WORKING_STATE_BATCH_COUNT];
};

// For S1F5 Soft Version Request
// 2010-08-24 add
struct	stS1F6SFCD30DataType
{
	char	szMDLN[MAX_MDLN_LEN+1];					// Model number
	char	szDesc[MAX_DESCRIPTION_LEN+1];			// Description
	char	szSoftRev[MAX_SOFTWARE_VERSION_LEN+1];  // Version
	char	szReleaseTime[MAX_RELEASE_TIME_LEN+1];  // Last Modifity time
	char	szReleaseSize[MAX_RELEASE_SIZE_LEN+1];  // File Size, Ladder Step
};

struct	stS1F6SFCD30Type					//	Current InterLock
{
	stHsmsMsgHeadType	MsgHead;
	
	char		szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long		nSFCD;		//	30 : Fixed
	long        nTMACK;
	
	long		nItemCount; // 3 : Fixed ( CIM, PLC, PLC Touch )
	
	stS1F6SFCD30DataType   stS1F6SFCD30Data[MAX_SOFT_VERSION_COUNT]; // ( CIM, PLC, PLC Touch )
};

///////////////////////////////////////////
//	S1F4	: EQ -> Host의 경우
//	Selected Equipment Status Data Structure (for SEM)
struct	stS1F11Type
{
	stHsmsMsgHeadType	MsgHead;

	char		szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nSVCount;
	long		nSVID[MAX_SVID_COUNT];
};

///////////////////////////////////////////
//	S1F12	:	EQ -> Host
//		Selected Equipment Status Data Structure Reply(for SEM)
struct	stS1F12DataType
{
	stHsmsMsgHeadType	MsgHead;
	
	char			szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long            nTMACK;
	char			szReason[MAX_MESSAGE_REASON_LEN+1];
	long			nSVCount;
	stSVDataType	stSVData[MAX_SVID_COUNT];
};

struct	stS1F12Type
{
	stHsmsMsgHeadType	MsgHead;

	char			szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long            nTMACK;
	char			szReason[MAX_MESSAGE_REASON_LEN+1];
	long			nSVCount;
	stSVDataType	stSVData[MAX_SVID_COUNT];
};

///////////////////////////////////////////
//	S1F15	:	Host -> EQ
//	Request Off-Line (signal only)
struct	stS1F15Type
{
	stHsmsMsgHeadType	MsgHead;
};

///////////////////////////////////////////
//	S1F16	:	EQ -> Host
//	Off-Line Acknowledge
struct	stS1F16Type
{
	stHsmsMsgHeadType	MsgHead;

	long	nOFLAck;	//	0 : Offline Accepted
						//	1 : Offline Not Allowed
						//	2 : EQ Already Offline
};

///////////////////////////////////////////
//	S1F17	:	Host -> EQ
//	Request ON-Line
struct	stS1F17Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nMCMD;		//	1 : OFF LINE
						//	2 : OnLine Local
						//	3 : OnLine Remote
};

///////////////////////////////////////////
//	S1F18	:	EQ -> Host
//	On-Line Acknowledge
struct	stS1F18Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long    nTMACK;     //  0 : ACK
						//  2 : Data Mismatch
						//	3 : Data Out of range 
	                    //102 : Already Required State

	char	szReason[MAX_MESSAGE_REASON_LEN+1];
	               

	
};

///////////////////////////////////////////////////////
//	S2F15	:	Host -> EQ
//	New Equipment Send
struct	stS2F15DataType
{
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nECID;
	char	szECName[MAX_EQ_CONSTANT_NAME_LEN+1];
	char	szECDefault[MAX_EQ_CONSTANT_VALUE_LEN+1];
	char	szECStopLowLimit[MAX_EQ_CONSTANT_VALUE_LEN+1];
	char	szECStopUpLimit[MAX_EQ_CONSTANT_VALUE_LEN+1];
	char	szECWarnLowLimit[MAX_EQ_CONSTANT_VALUE_LEN+1];
	char	szECWarnUpLimit[MAX_EQ_CONSTANT_VALUE_LEN+1];
};

struct	stS2F15Type
{
	stHsmsMsgHeadType	MsgHead;

	char			szModuleID[MAX_EQ_MODULE_ID_LEN+1];

	long			nECCount;				//
	stS2F15DataType	stData[MAX_EQ_CONSTANT_COUNT];
};

///////////////////////////////////////////////////////
//	S2F16	: EQ -> Host
//	New Equipment Constant Acknowledge
struct	stS2F16Type
{
	stHsmsMsgHeadType	MsgHead;

	char		szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long        nTMACK;	

	char		szReason[MAX_MESSAGE_REASON_LEN+1];
};

///////////////////////////////////////////////////////
//	S2F23	:	Host -> EQ
//	Trace Initialize Send
struct	stS2F23Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nTraceID;
	char	szDataSamplePeriod[MAX_TIME_LEN+1];
	long	nTotalSample;						//	Default : 0
	long	nReportGlassSize;					//	Default : 0

	long	nSVIDCount;			//	0 이면 Trace Off
	long	nSVID[MAX_SVID_COUNT];
};

///////////////////////////////////////////////////////
//	S2F24	:	EQ -> Host
//	Trace Initialize Acknowledge
struct	stS2F24Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nTIACK; 	//	0	:	OK
						//	1	:	Error, Not Done
						//	2	:	Trace ID is out of range

	char	szReason[MAX_MESSAGE_REASON_LEN+1];
};

///////////////////////////////////////////////////////
//	S2F25	:	Host -> EQ
//	Trace Initialize Send (for SEM)
struct	stS2F25Type
{
	stHsmsMsgHeadType	MsgHead;
	
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nTraceID;
	char	szDataSamplePeriod[MAX_TIME_LEN+1];
	long	nTotalSample;						//	Default : 0
	long	nReportGlassSize;					//	Default : 0
	
	long	nSVIDCount;			//	0 이면 Trace Off
	long	nSVID[MAX_SVID_COUNT];
};

///////////////////////////////////////////////////////
//	S2F26	:	EQ -> Host
//	Trace Initialize Acknowledge (for SEM)
struct	stS2F26Type
{
	stHsmsMsgHeadType	MsgHead;
	
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nTIACK; 	//	0	:	OK
						//	1	:	Error, Not Done
						//	2	:	Trace ID is out of range
	
	char	szReason[MAX_MESSAGE_REASON_LEN+1];
};

///////////////////////////////////////////////////////
//	S2F29	:	Host -> EQ
//	Equipment Constant Namelist Request
struct	stS2F29Type
{
	stHsmsMsgHeadType	MsgHead;

	char		szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long		nEQConstCount;
	long		nECID[MAX_EQ_CONSTANT_COUNT];
};

///////////////////////////////////////////////////////
//	S2F30	:	EQ -> Host
//	Equipment Constant Namelist 
struct	stS2F30DataType
{
	long	nECID;
	char	szECName[MAX_EQ_CONSTANT_NAME_LEN+1];
	char	szECDefault[MAX_EQ_CONSTANT_VALUE_LEN+1];
	char	szECStopLowLimit[MAX_EQ_CONSTANT_VALUE_LEN+1];
	char	szECStopUpLimit[MAX_EQ_CONSTANT_VALUE_LEN+1];
	char	szECWarnLowLimit[MAX_EQ_CONSTANT_VALUE_LEN+1];
	char	szECWarnUpLimit[MAX_EQ_CONSTANT_VALUE_LEN+1];
};

struct	stS2F30Type
{
	stHsmsMsgHeadType	MsgHead;

	char			szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long            nMIACK ;   // 0 : ACK
							   // 2 : Data Mismatch
							   // 4 : Data Duplication

	char			szReason[MAX_MESSAGE_REASON_LEN+1];
	                          
	long			nEQConstCount;				
	stS2F30DataType	stData[MAX_TOTAL_EQ_CONSTANT_COUNT];
};

///////////////////////////////////////////////////////
//	S2F31	:	Host -> EQ
//	Equipment Constant Namelist 
struct	stS2F31Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szSetTime[MAX_DATE_TIME_LEN+1];
};

struct	stS2F32Type
{
	stHsmsMsgHeadType	MsgHead;

	long                nTMACK;   // 0 : ACK
	                              // 6 : Content Not UnderStood
	
	char				szReason[MAX_MESSAGE_REASON_LEN+1];
};

///////////////////////////////////////////////////////
//	S2F41	:	Host -> EQ
//	Host Command Send
//	1. Process Command Type
struct	stS2F41ProcCmdType
{
	stHsmsMsgHeadType	MsgHead;

	long	nRCmd;	//	Remote Command ID
	char	szIPID[MAX_RCMD_LABEL_LEN+1];		//	"IPID  "
	char	szIPIDValue[MAX_PORT_ID_LEN+1];
	char	szICID[MAX_RCMD_LABEL_LEN+1];	//	"ICID  "
	char	szICIDValue[MAX_CASSETTE_ID_LEN+1];
	char	szOPID[MAX_RCMD_LABEL_LEN+1];		//	"OPID  "
	char	szOPIDValue[MAX_PORT_ID_LEN+1];
	char	szOCID[MAX_RCMD_LABEL_LEN+1];	//	"OCID  "
	char	szOCIDValue[MAX_CASSETTE_ID_LEN+1];
	char	szSTIF[MAX_RCMD_LABEL_LEN+1];		//	"STIF "
	char	szSTIFValue[MAX_SLOT_INFO_LEN+1];
	char	szOrder[MAX_RCMD_LABEL_LEN+1];		//	"ORDER   "
	long	nSlotCount;							// Glass Process Cancel인 경우에만 사용함.
	long	nSlotNo[MAX_SLOT_COUNT_PER_PORT];	// Cancel 처리할 Glass의 Slot No임.
};

//	Port Command Type
struct	stS2F41PortCmdDataType
{
	char	szPortID[MAX_RCMD_LABEL_LEN+1];		//	"PTID  "
	char	szPortIDValue[MAX_PORT_ID_LEN+1];
};

struct	stS2F41PortCmdType
{
	stHsmsMsgHeadType	MsgHead;

	long	nRCmd;
	long	nPortCount;	//	MAX ( Port 개수 : 6 (LD : 2, UD : 4)
	stS2F41PortCmdDataType	stPortCmd[MAX_PORT_COUNT];
};

//	EQ Command Type
struct	stS2F41EQCmdDataType
{
	char	szModuleLabel[MAX_RCMD_LABEL_LEN+1];	//	"MODULEID"
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	char	szRCodeLabel[MAX_RCMD_LABEL_LEN+1];		//	T7-2 "RCODE   "
	char	szRCode[MAX_EQ_CMD_RCODE_LEN+1];		
};

struct	stS2F41EQCmdType
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nRCmd;
	char	szRCode[MAX_EQ_CMD_RCODE_LEN+1];
	
};

//	S2F42	: EQ -> Host
struct	stS2F42ProcCmdType
{
	stHsmsMsgHeadType	MsgHead;

	long	nRCMD;
	long	nHCAck;	//	0 : OK
					//	1 : Command Not Exist
					//	2 : Cannot Perform now(Hardware)
					//	3 : At least on parameter invalid
					//	4 : Ack,
					//	5 : Rejected
					//	6 : No such object exists
					//	7 : Control State is Online Local
					//	8 : Cannot find raw data
					//	9 : Cannot read raw data
					//	10: PPID is not selected

	char	szIPID[MAX_RCMD_LABEL_LEN+1];	//	"IPID  "
	char	szIPIDValue[MAX_PORT_ID_LEN+1];
	long	nIPIDAck;		//	0 : OK
							//	1 : Parameter Name does not exist
							//	2 : Illegal Value specified for CPVAL
							//	3 : Illegal Format specified for CPVAL
							//	4 : No Cassette (In Port)
							//	5 :	No Cassette (Out Port)
							//	6 : Cassette ID not match(In Port)
							//	7 : Cassette ID not match(Out Port)
							//	8 : Port Status error(In Port)
							//	9 : Port Status error(Out Port)
							//	10: Too many slots are selected.
							//	11: No Glass Error
							//	12: Glass already exist, cannot replace
							//	13: Glass State Error
							//	14: Denied, Module state error
							//	15: Denied, Tool State Error
							//	16: Denied, Unit State Error
	char	szICID[MAX_RCMD_LABEL_LEN+1];	//	"ICID  "
	char	szICIDValue[MAX_CASSETTE_ID_LEN+1];
	long	nICIDAck;

	char	szOPID[MAX_RCMD_LABEL_LEN+1];		//	"OPID  "
	char	szOPIDValue[MAX_PORT_ID_LEN+1];
	long	nOPIDAck;

	char	szOCID[MAX_RCMD_LABEL_LEN+1];	//	"OCID  "
	char	szOCIDValue[MAX_CASSETTE_ID_LEN+1];
	long	nOCIDAck;

	char	szSTIF[MAX_RCMD_LABEL_LEN+1];		//	"STIF "
	char	szSTIFValue[MAX_SLOT_INFO_LEN+1];
	long	nSTIFAck;

	char	szORDER[MAX_RCMD_LABEL_LEN+1];		//	"ORDER  "
	long	nSlotCount;
	long	nSlotNo[MAX_SLOT_COUNT_PER_PORT];
	long	nORDERAck;
};

//	Port Command Type
struct	stS2F42PortCmdDataType
{
	char	szPortID[MAX_RCMD_LABEL_LEN+1];		//	"PTID  "
	char	szPortIDValue[MAX_PORT_ID_LEN+1];
	long	nAck;
};

struct	stS2F42PortCmdType
{
	stHsmsMsgHeadType	MsgHead;

	long	nRCMD;
	long	nHCAck;
	long	nPortCount;
	stS2F42PortCmdDataType	stPort[MAX_PORT_COUNT];
};

//	EQ Command Type
struct	stS2F42EQCmdDataType
{
	char	szModuleLabel[MAX_RCMD_LABEL_LEN+1];		//	"MODULEID"
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nModulePMACK;// = PMACK
					     // 0 : ACK
					     // 3 : Illeagal Value
	char	szRCodeLabel[MAX_RCMD_LABEL_LEN+1];		    
	char	szRCode[MAX_EQ_CMD_RCODE_LEN+1];		
	long	nRCodePMACK; // = PMACK
					     // 0 : ACK
					     // 3 : Illeagal Value
};

struct	stS2F42Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];

	long	nRCMD;
	long    nTMACK;
					   // 0 : ACK
					   // 2 : Can Not Perform Now[설비 H/W 문제로 수행할 수 없음]
					   //10 : At Least one Condition Doesn't Match[PortID/CSTID/STIF 등 적어도 하나이상이 Match 되지 않음]
					   // 25 : Command isn't Supported by Equipment
					   // 74 : Already Received[이미 수행 명령을 받았음]

	char	szReason[MAX_MESSAGE_REASON_LEN+1];
};

struct  stS2F41JudgementDataType
{
	char	szPanelID[MAX_PANEL_ID_LEN+1];
	char	szOldJudgement[MAX_JUDGEMENT_RESULT_LEN+1];
	char	szNewJudgement[MAX_JUDGEMENT_RESULT_LEN+1];
	char	szCode[MAX_JUDGEMENT_CODE_LEN+1];
};

struct  stS2F41JudgementCmdType 
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nRCMD;
	
	long	nGlassCnt;

	stS2F41JudgementDataType	stS2F41Judgement[MAX_LAYER2_MODULE_COUNT];
};

struct	stS2F42JudgementCmdType
{
	stHsmsMsgHeadType	MsgHead;
	long	nRCMD;
	long	nHCAck;

	char	szPanelIDLabel[MAX_RCMD_LABEL_LEN+1];
	char	szPanelID[MAX_PANEL_ID_LEN+1];
	long	nCPAck;

	char	szUniqueIDLabel[MAX_RCMD_LABEL_LEN+1];
	long	nUniqueID[4];
	long	nCPAck2;
	
	char	szJudgementLabel[9+1];
	char	szJudgement[MAX_JUDGEMENT_RESULT_LEN+1];
	long	nCPAck3;

	char	szCodeLabel[4+1];
	char	szCode[4+1];
	long	nCPAck4;
};


///////////////////////////////////////////////////////
//	S2F101	:	Host -> EQ
//	Operator Call
struct	stS2F101Type
{
	stHsmsMsgHeadType	MsgHead;

	long	nTerminalID;
	char	szMessage[MAX_TEXT_MESSAGE_LEN+1];
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
};

///////////////////////////////////////////////////////
//	S2F102	:	EQ -> Host
//	Operator Call Reply
struct	stS2F102Type
{
	stHsmsMsgHeadType	MsgHead;

	long	nAckCode;	//	0 : Accepted for display
						//	1 : Message will not be displayed
						//	2 : Terminal not available
};

///////////////////////////////////////////////////////
//	S2F103	:	Host -> EQ
//	Equipment OnLine Parameter Change
struct	stS2F103EOMDDataType
{
	char	szEOMD[MAX_EOMD_LEN+1];
	long	nEOV;
};

struct	stS2F103EOIDDataType
{
	long	nEOID;			//	Equipment Online Parameter ID
	long	nEOMDCount;
	stS2F103EOMDDataType	stData[MAX_ONLINE_PARAM_MODE_COUNT];	//	Equipment Online Parameter Mode
};

struct	stS2F103Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nEOIDCount;
	stS2F103EOIDDataType	stData[MAX_ONLINE_PARAM_COUNT];
};

///////////////////////////////////////////////////////
//	S2F104	:	EQ -> Host
//	Equipment Online Parameter Acknowledge
struct	stS2F104EOMDDataType
{
	char	szEOMD[MAX_EOMD_LEN+1];
	long    nPMACK;  // 0 : Ack
	                 // 1 : Parameter doesn't exist
	                 // 2 : Illegal value
};

struct	stS2F104EOIDDataType
{
	long	nEOID;
	long	nEOMDCount;
	stS2F104EOMDDataType	stData[MAX_ONLINE_PARAM_MODE_COUNT];	//	Equipment Online Parameter Mode
};

struct	stS2F104Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long    nTMACK;   // 0 : Ack
	                  // 2 : Can not Perform now
	                  // 4 : Aleady Required State
	                  // 7 : At least one parameter doew not exist
	                  // 9 : At least one value is out of renge
	                  // 31 : ModuleID doesn't exist
	
	char	szReason[MAX_MESSAGE_REASON_LEN+1];
	long	nEOIDCount;
	stS2F104EOIDDataType	stData[MAX_ONLINE_PARAM_COUNT];
};

struct stS3F1MaterialDataType
{
	long	nMaterialType;
	char	szMaterialKind[MAX_MATERIAL_KIND_LEN+1];
	char	szMaterialLayer[MAX_MATERIAL_LAYER_LEN+1];
};
struct stS3F1Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nMaterialCnt;

	stS3F1MaterialDataType	stS3F1MaterialData[MAX_MATERIAL_COUNT];
};

struct stMaterialSub1DataType 
{
	char	szM_ProcessID[MAX_MATERIAL_PROCESS_ID_LEN+1];
	char	szM_ProductID[MAX_MATERIAL_PRODUCT_ID_LEN+1];
	char	szM_BatchID[MAX_MATERIAL_BATCH_ID_LEN+1];
	char	szM_StepID[MAX_MATERIAL_STEP_ID_LEN+1];
	char	szM_PPID[MAX_MATERIAL_PPID_LEN+1];
};

struct stMaterialSub2DataType
{
	char	szM_TrayID[MAX_MATERIAL_TRAY_ID_LEN+1];
	char	szM_PanelID[MAX_MATERIAL_PANEL_ID_LEN+1];
	char	szM_AssembledLocation[MAX_MATERIAL_ASSEMBLED_LOC_LEN+1];
	char	szM_CancelCode[MAX_MATERIAL_CANCEL_CODE_LEN+1];
	char	szM_DefectCode[MAX_MATERIAL_DEFECT_CODE_LEN+1];
};

struct stMaterialSub3DataType
{
	char	szM_ItemName[MAX_EQ_SPECIFIC_ITEM_NAME];
	char	szM_ItemValue[MAX_EQ_SPECIFIC_ITEM_VALUE];
};

struct stMaterialDataType
{
	char	szMaterialID[MAX_MATERIAL_ID_LEN + 1];
	long	nMaterialType;
	char	szMaterialKind[MAX_MATERIAL_KIND_LEN+1];
	char	szMaterialLayer[MAX_MATERIAL_LAYER_LEN+1];
	char	szMaterialCode[MAX_MATERIAL_CODE_LEN+1];
	char	szMaterialSlotNo[MAX_MATERIAL_SLOTID_LEN+1];
	long	nMaterialState;
	char	szMaterialLocation[MAX_MATERIAL_LOC_LEN+1];
	char	szMaterialTotalQty[MAX_MATERIAL_TOTAL_QTY_LEN+1];
	char	szMaterialUsedCnt[MAX_MATERIAL_USED_CNT_LEN+1];
	char	szMaterialUsedQty[MAX_MATERIAL_USED_QTY_LEN+1];
	char	szMaterialRemainedQty[MAX_MATERIAL_REMAINED_QTY_LEN+1];
	char	szMaterialReqQty[MAX_MATERIAL_REQ_QTY_LEN+1];
	char	szMaterialNGQty[MAX_MATERIAL_NG_QTY_LEN+1];
	char	szMaterialAssembledQty[MAX_MATERIAL_ASSEMBLED_QTY_LEN+1];

	long	nMaterialSub1DataCnt;
	stMaterialSub1DataType	stMaterialSub1Data[MAX_MATERIAL_COUNT];

	long	nMaterialSub2DataCnt;
	stMaterialSub2DataType	stMaterialSub2Data[MAX_MATERIAL_COUNT];

	long	nMaterialSub3DataCnt;
	stMaterialSub3DataType	stMaterialSub3Data[MAX_MATERIAL_COUNT];
};

struct stS3F2Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nTMACK;
	char	szReason[MAX_MESSAGE_REASON_LEN+1];

	long	nMaterialCount;
	stMaterialDataType		stMaterialData[MAX_MATERIAL_COUNT];
};
//	S3F101	:	Host -> EQ
struct	stS3F101Type
{
	stHsmsMsgHeadType	MsgHead;

	long	nHotDevice;
	stPortObjectDataType		stPortData;
	stCassetteObjectDataType	stCassData;

	long	nGlassCount;
	stGlassObjectType			stGlass[MAX_SLOT_COUNT_PER_PORT];
};

//	S3F102	: EQ -> Host
struct stS3F102Type
{
	stHsmsMsgHeadType	MsgHead;

	long	nAckCode;	//	0 : Accepted
						//	1 : Busy, Try again
						//	2 : Data Already Received
						//	3 : No Cassette
						//	4 : Cassette ID Mismatch
						//	5 : Mapping Information Mismatch ( Real Info <-> S3F101 Info )
};

struct  stCodeDataType
{
	char	szCode[MAX_CODE_LEN+1];
	char	szCodeDescription[MAX_CODE_DESCRIPTION_LEN+1];
};

struct	stS3F201Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long    nCDID;

	long    nCodeCount;
	stCodeDataType   stCodeData[MAX_CODE_COUNT];
};

struct	stS3F202Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long    nCDID;
	long    nTMACK;

	long    nCodeCount;
	stCodeDataType   stCodeData[MAX_CODE_COUNT];
};

//	S5F1	:	EQ -> Host
//	Alarm Report Send
struct	stS5F1Type
{
	stHsmsMsgHeadType	MsgHead;

	long	nAlarmCode;
	long	nAlarmID;
	char	szAlarmText[MAX_ALARM_TEXT_LEN+1];
	char    szAlarmDateTime[MAX_DATE_TIME_LEN+1];
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
};

///////////////////////////////////////////////////////
//	S5F2	:	Host -> EQ
//	Acknowledge of Alarm Report
struct	stS5F2Type
{
	stHsmsMsgHeadType	MsgHead;

	long    nTMACK;
};

///////////////////////////////////////////////////////
//	S5F5	:	Host -> EQ
//	Alarm List Request
struct  stS5F5Type 
{
	stHsmsMsgHeadType	MsgHead;
	
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nAlarmCount;
	
	long	nALID[MAX_ALARM_COUNT_IN_MODULE];
};
///////////////////////////////////////////////////////
//	S5F6	:	EQ -> HOST
//	Acknowledge to Alarm List Request

struct	stAlamrDataType 
{
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nAlarmCode;
	long	nAlarmID;
	char	szAlarmText[MAX_ALARM_TEXT_LEN+1];
};
struct	stS5F6Type 
{
	stHsmsMsgHeadType	MsgHead;
	
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nTMACK;
	char	szReason[MAX_MESSAGE_REASON_LEN+1];

	long	nAlarmCount;

	stAlamrDataType		stAlarmData[MAX_ALARM_COUNT_IN_MODULE];
};


///////////////////////////////////////////////////////
//	S5F101	:	Host -> EQ
//	Waiting Reset Alarm List
struct	stS5F101Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
};

///////////////////////////////////////////////////////
//	S5F102	:	EQ -> Host
//	Waiting Reset Alarm List Acknowledge
struct	stS5F102DataType
{
	long	nAlarmCode;
	long	nAlarmID;
	char	szAlarmText[MAX_ALARM_TEXT_LEN+1];
	char	szAlarmDateTime[MAX_DATE_TIME_LEN+1];
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
};

struct	stS5F102Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long    nTMACK;
	char	szReason[MAX_MESSAGE_REASON_LEN+1];

	long	nAlarmCount;	//	MAX = 20
	stS5F102DataType	Alarm[MAX_ALARM_HISTORY_COUNT * MAX_LAYER1_MODULE_COUNT];
};

struct  stS5F103DataType
{
	long	nAlarmID;
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
};

struct	stS5F103Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long    nAlarmCount;

	stS5F103DataType    stAlarmData[MAX_ALARM_DATA_COUNT];
};

struct  stS5F104DataType
{
	long	nAlarmID;
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long    nPMACK;
};

struct	stS5F104Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long    nTMACK;

	long	nAlarmCount;	
	stS5F104DataType	stAlarmData[MAX_ALARM_DATA_COUNT];
};

struct	stS5F105DataType
{
	long	nAlarmCode;
	long	nAlarmID;
	char	szAlarmText[MAX_ALARM_TEXT_LEN+1];
	char	szAlarmDateTime[MAX_DATE_TIME_LEN+1];
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
};

struct	stS5F105Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nAlarmCount;	//	MAX = 20
	stS5F105DataType	Alarm[MAX_ALARM_HISTORY_COUNT];
};
/////////////////////////////////////////////////////////////////
//	S6F1	:	EQ -> Host
//	Trace Data Send
struct	stS6F1PanelDataType
{
	char	szHGlassID[MAX_PANEL_ID_LEN+1];
	char	szEGlassID[MAX_PANEL_ID_LEN+1];
};

struct	stS6F1DataType
{
	long	nSVID;
	char	szSV[MAX_STATUS_VALUE_LEN+1];
	char	szSVNAME[MAX_STATUS_VALUE_NAME_LEN+1];
};

struct	stS6F1GroupDataType
{
	char	szDateTime[MAX_DATE_TIME_LEN+1];
	long    nPanelCount;
	stS6F1PanelDataType stPanelData[MAX_UNIT_COUNT];
	long	nSVCount;
	stS6F1DataType	stSV[MAX_SVID_COUNT];
};

struct	stS6F1Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nTraceID;
	long	nSampleCount;
	long	nRptGroupSize;	// 10
	stS6F1GroupDataType	stData[MAX_REPORT_GROUP_SIZE];
};

/////////////////////////////////////////////////////////////////
//	S6F2	:	Host -> EQ
//	Trace Data Acknowledge
struct	stS6F2Type
{
	stHsmsMsgHeadType	MsgHead;

	long	nAckCode;	//	0 : Accepted
						//	1 : Unknown Tool ID
};

/////////////////////////////////////////////////////////////////
//	S6F3	:	EQ -> Host
//	Trace Data Send (for SEM)
struct	stS6F3Type
{
	stHsmsMsgHeadType	MsgHead;
	
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nTraceID;
	long	nSampleCount;
	long	nRptGroupSize;	// 10
	stS6F1GroupDataType	stData[MAX_REPORT_GROUP_SIZE];
};

/////////////////////////////////////////////////////////////////
//	S6F4	:	Host -> EQ
//	Trace Data Acknowledge
struct	stS6F4Type
{
	stHsmsMsgHeadType	MsgHead;
	
	long	nAckCode;	//	0 : Accepted
	//	1 : Unknown Tool ID
};

/////////////////////////////////////////////////////////////////
//	S6F11	:	EQ -> Host
//	Job Event
struct	stS6F11Report0Type
{
	long	nDataID;
	long	nCEID;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nByWho;
	char	szOperID[MAX_OPERATOR_ID_LEN+1];
	
	long	nReportID;	//	0 : Fixed
	stEQObjectDataType	stEQData;
};

struct	stS6F11Report1Type
{
	long	nReportID;	//	1 : Fixed
	stPortObjectDataType	stPort;
};

struct	stS6F11Report2Type
{
	long	nReportID;	//	2 : Fixed
	stCassetteObjectDataType	stCassette;
};

struct	stS6F11Report3Type
{
	long	nReportID;						//	3 : Fixed
	long	nGlassCount;
	stGlassObjectType	stGlass[MAX_SLOT_COUNT_PER_PORT];
};

struct	stS6F11Report4Type
{
	long	nReportID;	//	4 : Fixed
	long	nOldState;
	long	nNewState;
	char	szLimitTime[MAX_TIME_LEN+1];
	char	szReasonCode[MAX_REASON_CODE_LEN+1];
};

struct	stS6F11EOMDDataType
{
	char	szEOMD[MAX_EOMD_LEN+1];
	long	EOV;

};

struct	stS6F11Report5DataType
{
	long	nEOID;
	long	nEOMDCount;

	stS6F11EOMDDataType	stEOMD[MAX_ONLINE_PARAM_MODE_COUNT];
};

struct	stS6F11Report5Type
{
	long	nReportID;						//	5 : Fixed
	long	nEOIDCount;
	stS6F11Report5DataType	stData[MAX_ONLINE_PARAM_COUNT];
};

struct	stS6F11Report20Type
{
	long	nReportID;						//	20 : Fixed
	char	szFromModuleID[MAX_EQ_MODULE_ID_LEN+1];
	char	szToModuleID[MAX_EQ_MODULE_ID_LEN+1];
};

struct	stS6F11Report6DataType
{
	long	nECID;
	char	szECName[MAX_EQ_CONSTANT_NAME_LEN+1];
	char	szECDefault[MAX_EQ_CONSTANT_VALUE_LEN+1];
	char	szECStopLowLimit[MAX_EQ_CONSTANT_VALUE_LEN+1];
	char	szECStopUpLimit[MAX_EQ_CONSTANT_VALUE_LEN+1];
	char	szECWarnLowLimit[MAX_EQ_CONSTANT_VALUE_LEN+1];
	char	szECWarnUpLimit[MAX_EQ_CONSTANT_VALUE_LEN+1];
};

struct	stS6F11Report6Type
{
	long	nReportID;						//	6 : Fixed
	long	nECIDCount;
	stS6F11Report6DataType	stData[MAX_EQ_CONSTANT_COUNT];
};

// T7-2 Add
struct	stS6F11Report7DataType
{
	char	szSpecCtrlDataName[MAX_SPEC_CONTROL_DATA_NAME_LEN+1];
	char	szSpecCtrlDataValue[MAX_SPEC_CONTROL_DATA_VALUE_LEN+1];
	char	szRelatedModuleID[MAX_SPEC_CONTROL_RELATED_MODULEID+1];
};

struct	stS6F11Report7Type
{
	long	nReportID;						//	7 : Fixed
	long	nDataCount;
	stS6F11Report7DataType	stSpecCtrlData[MAX_SPEC_CONTROL_EVENT_COUNT];
};

struct	stS6F11Report8SubDataType
{
	char	szMProductID[MAX_MATERIAL_PRODUCT_ID_LEN+1];
	char	szMStepID[MAX_MATERIAL_STEP_ID_LEN+1];
	char	szMPPID[MAX_MATERIAL_PPID_LEN+1];
};

struct	stS6F11Report8DataType
{
	char	szMID[MAX_MATERIAL_ID_LEN+1];
	long	nMType;
	char	szMTYPE[MAX_MATERIAL_TYPE_LEN+1];
	char	szMSlotID[MAX_LIBRARY_ID_LEN+1];
	long	nMPState;
	long	nMState;
	char	szMLoc[MAX_MATERIAL_LOC_LEN+1];
	long	nMSize;
	long	nMCount;
	stS6F11Report8SubDataType	stMData[MAX_MATERIAL_COUNT];
};

struct	stS6F11Report8Type
{
	long	nReportID;						//	8 : Fixed

	stS6F11Report8DataType     stS6F11Report8Data[MAX_MATERIAL_COUNT];
};

struct  stS6F11PanelDataType
{
	char	szHGlassID[MAX_PANEL_ID_LEN+1];
	char    szSlotID[MAX_SLOT_ID_LEN+1];
};

struct stS6F11ReportPanelIDValidationType
{
	char	szBatchID[MAX_BATCH_ID_LEN+1];
	char	szEGlassID[MAX_PANEL_ID_LEN+1];
	char    szPortID[MAX_PORT_ID_LEN+1];
	char	szCassetteID[MAX_CASSETTE_ID_LEN+1];
	char    szSlotID[MAX_SLOT_ID_LEN+1];
	long    nPanelSize[2]; 

	long    nGlassCount;
	stS6F11PanelDataType          stS6F11PanelData[MAX_SLOT_COUNT_PER_PORT];
};

struct	stS6F11Report9Type
{
	long	nReportID;						//	9 : Fixed

	stS6F11ReportPanelIDValidationType	PanelIDValidation;
};

struct	stS6F11ReportSEQStandardDataType
{
	char	szProcessID[MAX_PROCESS_ID_LEN+1];
	char	szProductID[MAX_PRODUCT_ID_LEN+1];
	char	szPPID[MAX_PPID_LEN+1];
	long    nStandardTact;
};

struct stS6F11Report11DataType
{
	long	nAlarmCode;
	long	nAlarmID;
	char	szAlarmText[MAX_ALARM_TEXT_LEN+1];
	char    szAlarmDateTime[MAX_DATE_TIME_LEN+1];
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
};

struct	stS6F11Report11Type
{
	long	nReportID;						//	11 : Fixed

	long	nAlarmCount;	//	MAX = 20
	stS6F11Report11DataType  Alarm[MAX_ALARM_HISTORY_COUNT];
};

// For Edge Cleaner
struct	stS6F11Report16Type
{
	long	nReportID;						//	16 : Fixed

	char	szPortID[MAX_PORT_ID_LEN+1];
	long	nEQState;
	long	nPortState;
};

struct stPanelIDType
{
	char szPanelID[MAX_PANEL_ID_LEN+1];
};

// For Edge Cleaner
struct	stS6F11Report17Type
{
	long	nReportID;						//	17 : Fixed

	char	szCarrierID[MAX_CASSETTE_ID_LEN+1];
	long	nCstType;
	long	nReqQTY;

	long nPanelCount;
	stPanelIDType stPanelID[50];
};

struct	stS6F11Report26Type
{
	long	nReportID;						//	26 : Fixed

	stS6F11ReportSEQStandardDataType	StandardData;
};

struct	stS6F11ProcessType
{
	stHsmsMsgHeadType	MsgHead;

	stS6F11Report0Type	Report0;	// EQ
	stS6F11Report1Type	Report1;	// Port 
	stS6F11Report2Type	Report2;	// Cassette
	stS6F11Report3Type	Report3;	// Glass

};

struct	stS6F11Layer2ModuleType		//	Layer 2
{
	char		szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long		nEQState;
	long		nProcessState;
	long		nLayer3Count;   // 0 : Fix
};

struct	stS6F11Layer1ModuleType		//	Layer 1
{
	char		szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long		nEQState;
	long		nProcessState;
	long		nLayer2Count;
	stS6F11Layer2ModuleType	stLayer2[MAX_LAYER2_MODULE_COUNT];
};

struct	stS6F11Sub1Type
{
	long	nMCMD;
	long	nByWho;
	char	szOperID[MAX_OPERATOR_ID_LEN+1];
};

struct	stS6F11Sub2Type
{
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nEQState;
	long	nProcessState;
	long	nLayer1Count;
	stS6F11Layer1ModuleType	stLayer1[MAX_LAYER2_MODULE_COUNT];
};

struct	stS6F11Sub3Type
{
	char	szLimitTime[MAX_TIME_LEN+1];
	char	szReasonCode[MAX_REASON_CODE_LEN+1];
};
//////////////////////////////////////////////////////////////////////////////
//	Glass Event
struct	stS6F11GlassType
{
	stHsmsMsgHeadType	MsgHead;

	long	nCEID;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nByWho;
	char	szOperID[MAX_OPERATOR_ID_LEN+1];

	char	szFromModuleID[MAX_EQ_MODULE_ID_LEN+1];
	char	szToModuleID[MAX_EQ_MODULE_ID_LEN+1];

	long	nGlsCnt;

	stGlassObjectType	stGlass[MAX_LAYER2_MODULE_COUNT];
};

//	Port Event
struct	stS6F11PortType
{
	stHsmsMsgHeadType	MsgHead;

	stS6F11Report0Type	Report0;
	stS6F11Report1Type	Report1;
	stS6F11Report2Type	Report2;

	// For Edge Cleaner
	stS6F11Report16Type Report16;
	stS6F11Report17Type Report17;
};

//	Equipment Event
struct	stS6F11EQType
{
	stHsmsMsgHeadType	MsgHead;

	long	nDataID;
	long	nCEID;	

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nEQState;
	long	nProcessState;
	
	char	szPauseCode[MAX_PAUSE_CODE_LEN+1];
	char	szPMCode[MAX_PM_CODE_LEN+1];

	stS6F11Sub1Type	    Sub1;
	stS6F11Sub2Type	    Sub2;
	stS6F11Sub3Type	    Sub3;
	stS6F11Report11Type	Report11;	// EQ State가 Fault로 변경되는 경우의 Alarm Data
};

//	EQ Parameter Event
struct	stS6F11EQParamType
{
	stHsmsMsgHeadType	MsgHead;

	stS6F11Report0Type	Report0;
	stS6F11Report5Type	Report5;	//	EOID
	stS6F11Report6Type	Report6;	//	ECID
};

//	Equipment Specified Control Event
struct	stS6F11SpecCtrlType
{
	stHsmsMsgHeadType	MsgHead;

	stS6F11Report0Type	Report0;
	stS6F11Report7Type	Report7;
};

//	Equipment Material Event
struct	stS6F11MaterialType
{
	stHsmsMsgHeadType	MsgHead;

	stS6F11Report0Type	Report0;

	long	nMaterialCnt;

	stMaterialDataType	stMaterialData[MAX_MATERIAL_COUNT];
};

struct	stS6F11StandardDataType
{
	stHsmsMsgHeadType	MsgHead;

	stS6F11Report0Type	Report0;
	stS6F11Report26Type	Report26;
};

struct	stS6F11PanelIDValidationType
{
	stHsmsMsgHeadType	MsgHead;

	stS6F11Report0Type	Report0;
	stS6F11Report9Type	Report9;
};

struct	stS6F11SpecPortInfoType
{
	char	szPortID[MAX_PORT_ID_LEN+1];
	char	szCSTID[MAX_CASSETTE_ID_LEN+1];
	char	szTrayID[MAX_TRAY_ID_LEN+1]; 
};

struct	stS6F11SpecGlassInfoType
{
	char	szPanelID[MAX_PANEL_ID_LEN+1];
	char	szProcessID[MAX_PROCESS_ID_LEN+1];
	char	szProductID[MAX_PRODUCT_ID_LEN+1];
	char	szBatchID[MAX_BATCH_ID_LEN+1];
	char	szPPID[MAX_PPID_LEN+1];
	char	szSlotID[MAX_SLOT_ID_LEN+1];
};

struct	stS6F11SpecItemInfoType
{
	char	szItemName[MAX_EQ_SPECIFIC_ITEM_NAME+1];
	char	szItemValue[MAX_EQ_SPECIFIC_ITEM_VALUE+1];
};

struct	stS6F11SpecificStepType
{
	stHsmsMsgHeadType	MsgHead;

	long	nCEID;
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	char	szSpecStep[MAX_EQ_SPECIFIC_STEP_LEN+1];
	
	long	nPortInfoCnt;

	stS6F11SpecPortInfoType		stSpecPortInfo;
	
	long	nGlassCnt;
	stS6F11SpecGlassInfoType	stSpecGlassInfo;

	long	nItemCnt;
	stS6F11SpecItemInfoType		stSpecItemInfo;

};

// 2010-08-24 add
struct stS6F11SoftVersionType
{
	stHsmsMsgHeadType	MsgHead;
	
	long	nDataID;
	long	nCEID;
	
	long		nItemCount; // 3 : Fixed ( CIM, PLC, PLC Touch )
	
	stS1F6SFCD30DataType   stS6F11SofrVer[MAX_SOFT_VERSION_COUNT]; // ( CIM, PLC, PLC Touch )
};

/////////////////////////////////////////////////////////////////
//	S6F12	:	Host -> EQ
//	Event Report Acknowledge
struct	stS6F12Type
{
	stHsmsMsgHeadType	MsgHead;

	long    nTMACK;
};

/////////////////////////////////////////////////////////////////
//	S6F13	:	EQ -> Host
//	Namelist Variable Data Send
struct	stS6F13Report0Type
{
	long	nReportID;						//	0 : Fixed
	stEQObjectDataType	stEQData;
};

struct	stS6F13Report1Type
{
	long	nReportID;						//	1 : Fixed
	long	nGlassCount;
	stGlassObjectType	stGlass[MAX_SLOT_COUNT_PER_PORT];
};

struct	stS6F13Report10DataType
{
	char	szItemName[MAX_DATA_COLLECT_ITEM_NAME_LEN+1];
	char	szItemValue[MAX_DATA_COLLECT_ITEM_VAULE_LEN+1];
};

struct	stS6F13Report10ModuleType
{
	char	szModuleID[MAX_EQ_MODULE_ID_LEN];
	long	nDataCount;
	stS6F13Report10DataType	stData[MAX_DATA_COLLECT_ITEM_COUNT];

};

//	C6 Project 용도 변경
struct	stS6F13Report10Type
{
	long	nReportID;

	long	nModuleCount;	// Local / Inline - Etcher에서 사용
	stS6F13Report10ModuleType	stModuleData[MAX_LAYER1_MODULE_COUNT];
};

struct	stS6F13GlassType
{
	stHsmsMsgHeadType	MsgHead;

	long	nDataID;
	long	nCEID;		//	Fixed : 2(Glass Data)
	long	nOwnGlassNo;
	long	nHSNo;

	stS6F13Report0Type	Report0;
	stS6F13Report1Type	Report1;
	stS6F13Report10Type	Report10;

	long	nModuleID;
};

struct	stS6F13CassetteType
{
	stHsmsMsgHeadType	MsgHead;

	long	nDataID;
	long	nCEID;		//	Fixed : 1(Cassette Data)

	stS6F11Report0Type	Report0;
	stS6F11Report2Type	Report1;
	stS6F13Report10Type	Report10;
};
/////////////////////////////////////////////////////////////////
//	S6F14	:	Host -> EQ
//	Annotated Event Report Acknowledge
struct	stS6F14Type
{
	stHsmsMsgHeadType	MsgHead;

	long	nTMACK;	//	0 : Accepted
};

/////////////////////////////////////////////////////
struct	stS7F1Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	char	szPPID[MAX_PPID_LEN+1];
	long	nPPIDType;	
};

/////////////////////////////////////////////////////
//	S7F34	:	EQ -> Host
//	Process Program Availability Data (PAD)
struct	stS7F2Type
{
	stHsmsMsgHeadType	MsgHead;

	long    nTMACK;     // 0 : ACK;
	                    // 2 : can not perform not [설비 H/W문제로 수행할 수 없음]
	                    // 3 : Permission not garanted.
	                    // 26 : PPID doesn't exist
	                    // 27 : PPID Type doesn't exist
	                    // 30 : PPID isn't avaliable
	                    // 31 : Module ID doesn't exist
	                    // 74 : Aleady Received [이미 수행 명령을 받았음]
};

struct	stS7F9Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nPPIDType;	
	char	szPPID[MAX_PPID_LEN+1];
};

/////////////////////////////////////////////////////

struct	stS7F10Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long    nTMACK;     // 0 : ACK;
	                    // 2 : can not perform not [설비 H/W문제로 수행할 수 없음]
	                    // 3 : Permission not garanted.
	                    // 26 : PPID doesn't exist
	                    // 27 : PPID Type doesn't exist
	                    // 30 : PPID isn't avaliable
	                    // 31 : Module ID doesn't exist
	                    // 74 : Aleady Received [이미 수행 명령을 받았음]
};


//	S7F23	:	Host -> EQ
//	Formatted Process Program Send
struct	stS7F23DataType
{
	char	szProcParamName[MAX_PROCESS_PARAM_NAME_LEN+1];
	char	szProcParamValue[MAX_PROCESS_PARAM_VALUE_LEN+1];
};

struct	stS7F23CmdDataType
{
	long	nCmdCode;		//	1 (Fixed)
	long	nParamCount;	//	4
	stS7F23DataType	stParamData[MAX_PROCESS_PARAM_COUNT];
};

struct	stS7F23Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	char	szPPID[MAX_PPID_LEN+1];
	char	szSOFTREV[MAX_SOFT_REVISION_LEN+1];
	long	nPPIDType;	// 1 : Normal, 2 : Mapping Table

	long	nCmdCodeCount;
	stS7F23CmdDataType	stCmdData[MAX_COMMAND_CODE_COUNT];
};

/////////////////////////////////////////////////////
//	S7F24	:	EQ -> Host
//	Formatted Process Program Acknowledge
struct	stS7F24Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long    nTMACK;    
	
	char	szReason[MAX_MESSAGE_REASON_LEN+1];
};

/////////////////////////////////////////////////////
//	S7F25	:	Host -> EQ
//	Formatted Process Program Request
struct	stS7F25Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	char	szPPID[MAX_PPID_LEN+1];
	long	nPPIDType;	// 1 : Normal, 2 : Mapping Table
};

/////////////////////////////////////////////////////
//	S7F26	:	EQ -> Host
//	Formatted Process Program Data
struct	stS7F26DataType
{
	char	szProcParamName[MAX_PROCESS_PARAM_NAME_LEN+1];
	char	szProcParamValue[MAX_PROCESS_PARAM_VALUE_LEN+1];
};

struct	stS7F26CmdDataType
{
	long	nCmdCode;		//	1 (Fixed)
	long	nParamCount;	//	
	stS7F26DataType	ParamData[MAX_PROCESS_PARAM_COUNT];
};

struct	stS7F26Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	char	szPPID[MAX_PPID_LEN+1];
	char	szSOFTREV[MAX_SOFT_REVISION_LEN+1];
	char	szChangeDateTime[MAX_DATE_TIME_LEN+1];
	long	nPPIDType;	// 1 : Normal, 2 : Mapping Table
	long    nTMACK;     

	long	nPPIDCount;	// Pass의 경우 1, Fail의 경우 0

	char	szReason[MAX_MESSAGE_HEADER_LEN+1];
	long	nCmdCodeCount;	//	S7F23, 26에서만 Local Strip = 1, Inline = 2
	stS7F26CmdDataType	stCmdData[MAX_COMMAND_CODE_COUNT];
};

/////////////////////////////////////////////////////
//	S7F33	:	Host -> EQ
//	Process Program Available Request (PAR)
struct	stS7F33Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	char	szPPID[MAX_PPID_LEN+1];
	long	nPPIDType;	// 1 : Normal, 2 : Mapping Table
};

/////////////////////////////////////////////////////
//	S7F34	:	EQ -> Host
//	Process Program Availability Data (PAD)
struct	stS7F34Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szPPID[MAX_PPID_LEN+1];
	long	nUnflen;	//	0 - Fix
	long    nTMACK;     // 0 : ACK;
	                    // 26 : PPID doesn't exist
	                    // 27 : PPID Type doesn't exist
	                    // 30 : PPID isn't avaliable
	                    // 31 : Module ID doesn't exist
};

/////////////////////////////////////////////////////
//	S7F101	:	Host -> EQ
//	Current Equipment PPID Request
struct	stS7F101Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nPPIDType;	// 1 : Normal, 2 : Mapping Table
};

/////////////////////////////////////////////////////
//	S7F102	:	EQ -> Host
//	Current Equipment PPID Data
struct	stS7F102DataType
{
	char	szPPID[MAX_PPID_LEN+1];
};

struct	stS7F102Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nPPIDType;	// 1 : Normal, 2 : Mapping Table
	long    nTMACK;     

	char	szReason[MAX_MESSAGE_REASON_LEN+1];

	long	nPPIDCount;
	stS7F102DataType	stPPID[MAX_PPID_COUNT];
};

/////////////////////////////////////////////////////
//	S7F103	:	Host -> EQ
//	PPID Existence Check
struct	stS7F103Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	char	szPPID[MAX_PPID_LEN+1];
	long	nPPIDType;	// 1 : Normal, 2 : Mapping Table
};

/////////////////////////////////////////////////////
//	S7F104	:	EQ -> Host
//	PPID Existence Check Acknowledge
struct	stS7F104Type
{
	stHsmsMsgHeadType	MsgHead;

//	long	nAckCode;
	long    nTMACK;
};

/////////////////////////////////////////////////////
//	S7F105	:	Host -> EQ
//	PPID Change Time Check
struct	stS7F105Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	char	szPPID[MAX_PPID_LEN+1];
	long	nPPIDType;	// 1 : Normal, 2 : Mapping Table
};

/////////////////////////////////////////////////////
//	S7F106	:	EQ -> Host
//	PPID Change Time Check Acknowledge
struct	stS7F106Type
{
	stHsmsMsgHeadType	MsgHead;

	long    nTMACK;
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	char	szPPID[MAX_PPID_LEN+1];
	char	szSoftRev[MAX_SOFT_REVISION_LEN+1];
	long	nPPIDType;	// 1 : Normal, 2 : Mapping Table
	char	szChangeDateTime[MAX_DATE_TIME_LEN+1];
};

// PPID Create/Delete/Modify
struct	stS7F107DataType
{
	char	szProcParamName[MAX_PROCESS_PARAM_NAME_LEN+1];
	char	szProcParamValue[MAX_PROCESS_PARAM_VALUE_LEN+1];
};

struct	stS7F107CmdDataType
{
	long	nCmdCode;		//	1 (Fixed)
	long	nParamCount;	//	4
	stS7F107DataType	stParamData[MAX_PROCESS_PARAM_COUNT];
};

struct	stS7F107Type
{
	stHsmsMsgHeadType	MsgHead;

	long	nMode;
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	char	szPPID[MAX_PPID_LEN+1];
	char	szSoftRev[MAX_SOFT_REVISION_LEN+1];
	char	szChangeDateTime[MAX_DATE_TIME_LEN+1];
	long	nPPIDType;	// 1 : Normal, 2 : Mapping Table
	long	nByWho;

	long	nCmdCodeCount;
	stS7F107CmdDataType	stCmdData[MAX_COMMAND_CODE_COUNT];
};

/////////////////////////////////////////////////////
//	S7F24	:	EQ -> Host
//	Formatted Process Program Acknowledge
struct	stS7F108Type
{
	stHsmsMsgHeadType	MsgHead;

	long    nTMACK;     //  0 : OK, Unstood
};

//	Current Running PPID Request
struct	stS7F109Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nPPIDType;	// 1 : Normal, 2 : Mapping Table
};

struct	stS7F110PPIDDataType
{
	long	nPPIDType;
	char	szPPID[MAX_PPID_LEN+1];
	char	szPPIDVer[MAX_SOFT_REVISION_LEN+1];
	char	szRevTime[MAX_DATE_TIME_LEN+1];
};
//	Current Running PPID Data
struct	stS7F110Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long    nTMACK;     
	char	szReason[MAX_MESSAGE_REASON_LEN+1];
	
	long	nPPIDCount;

	stS7F110PPIDDataType	stS7F110PPIDData;
};

struct	stS7F111Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	char    szProcessID[MAX_PROCESS_ID_LEN+1];
	char    szProductID[MAX_PRODUCT_ID_LEN+1];
	char    szStepID[MAX_STEP_ID_LEN+1];
	char    szPPID[MAX_PPID_LEN+1];
};

//	Current Running PPID Data
struct	stS7F112Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	char	szPPID[MAX_PPID_LEN+1];
	long    nStandardTact;
	long    nTMACK;     
};

////////////////////////////////////////////////
//	S9F1	: EQ -> Host
//	Unrecognized Device ID
struct	stS9F1Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szMessageHead[MAX_MESSAGE_HEADER_LEN+1];
};

/////////////////////////////////////////////////////
//	S9F3	: EQ -> Host
//	Unrecognized Stream Type
struct	stS9F3Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szMessageHead[MAX_MESSAGE_HEADER_LEN+1];
};

/////////////////////////////////////////////////////
//	S9F5	: EQ -> Host
//	Unrecognized Function Type
struct	stS9F5Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szMessageHead[MAX_MESSAGE_HEADER_LEN+1];
};

/////////////////////////////////////////////////////
//	S9F7	: EQ -> Host
//	Illegal Data
struct	stS9F7Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szMessageHead[MAX_MESSAGE_HEADER_LEN+1];
};

/////////////////////////////////////////////////////
//	S9F9	: EQ -> Host
//	Transaction Timer timeout
struct	stS9F9Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szMessageHead[MAX_MESSAGE_HEADER_LEN+1];
};

/////////////////////////////////////////////////////
//	S9F1	: EQ <-> Host
//	Data too long
struct	stS9F11Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szMessageHead[MAX_MESSAGE_HEADER_LEN+1];
};

/////////////////////////////////////////////////////
//	S10F3	:	Host -> EQ
//	Terminal Display single
struct	stS10F3Type
{
	stHsmsMsgHeadType	MsgHead;

	long	nTerminalID;
	char	szText[MAX_TEXT_MESSAGE_LEN+1];
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
};

/////////////////////////////////////////////////////
//	S10F4	:	EQ -> Host
//	Terminal Display single Acknowledge
struct	stS10F4Type
{
	stHsmsMsgHeadType	MsgHead;

	long	nAckCode;	//	0 : Accepted
						//	1 : Message will not be displayed
						//	2 : Terminal Not available
};

/////////////////////////////////////////////////////
//	S10F9	:	Host -> EQ
//	BroadCast
struct	stS10F9Type
{
	stHsmsMsgHeadType	MsgHead;

	char	szText[MAX_TEXT_MESSAGE_LEN+1];
};

/////////////////////////////////////////////////////
//	S10F10	:	EQ -> Host
struct	stS10F10Type
{
	stHsmsMsgHeadType	MsgHead;

	long	nAckCode;
};

struct  stATTRDataType 
{
	char	szATTRData[MAX_ATTR_DATA_LEN+1];
};

struct	stATTRIDType
{
	char	szATTRID[MAX_ATTR_ID_LEN+1];
	
	long	nATTRDataCnt;	
	stATTRDataType stATTRData[MAX_ATTR_DATA_COUNT];
};

/////////////////////////////////////////////////////
//	S10F101	:	Host -> EQ
//	Multi Block Send
struct	stS10F101Type
{
	stHsmsMsgHeadType	MsgHead;
	
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nTID;

	long	nATTRIDCnt;

	stATTRIDType stATTRID[MAX_ATTR_DATA_COUNT];

};
/////////////////////////////////////////////////////
//	S10F102	:	EQ -> Host
struct	stS10F102Type
{
	stHsmsMsgHeadType	MsgHead;
	
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nAckCode;
	char	szReason[MAX_MESSAGE_REASON_LEN+1];
};


struct stGlassHistoryDataType
{
	char	szH_PanelID[MAX_PANEL_ID_LEN+1];
	char	szBatchID[MAX_BATCH_ID_LEN+1];
	char	szStepID[MAX_STEP_ID_LEN+1];
	char	szProcessID[MAX_PROCESS_ID_LEN+1];
	char	szProductID[MAX_PRODUCT_ID_LEN+1];
};

struct	stS64F1Type
{
	stHsmsMsgHeadType	MsgHead;

	long	nNumberOfPanel;	// Max = 60
	
	stGlassHistoryDataType	stGlassHistoryData[MAX_GLASSHISTORYDATA_COUNT];
};

struct	stS64F2Type
{
	stHsmsMsgHeadType	MsgHead;
	
	long	nTMACK;
	long	nNumberOfPanel;	// Max = 60

	stGlassHistoryDataType	stGlassHistoryData[MAX_GLASSHISTORYDATA_COUNT];
};

///////////////////////////////////////////////////////////////////////////////
struct  stProcCtrlItemDataType
{
	char	szItemName[MAX_EQ_SPECIFIC_ITEM_NAME];
	char	szItemValue[MAX_EQ_SPECIFIC_ITEM_VALUE];
};

struct	stProcCtrlParamDataType
{
	char	szParamName[MAX_PROCESS_PARAM_NAME_LEN];
	char	szParamValue[MAX_PROCESS_PARAM_VALUE_LEN];
};

struct	stProcCtrlCmdDataType
{
	long	nCCode;
	long	nParamCount;

	stProcCtrlParamDataType		stProcCtrlParamData[MAX_PROCESS_PARAM_COUNT];
};	

struct  stProcCtrlModuleDataType 
{
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	char	szPPID[MAX_PPID_LEN+1];
	long	nPPIDType;

	//long	nProcCmdCount;
	//stProcCtrlCmdDataType	stProcCtrlCmdData[MAX_PROCESS_PARAM_COUNT];

	//long	nItemCount;
	//stProcCtrlItemDataType	stProcCtrlItemData[MAX_EQ_SPECIFIC_ITEM_COUNT];
};

struct	stProcCtrlDataType 
{
	char	szHPanelID[MAX_PANEL_ID_LEN+1];
	long	nSEQ_No;

	long	nModuleCount;

	stProcCtrlModuleDataType	stProcCtrlModuleData[MAX_LAYER1_MODULE_COUNT];	
};
/////////////////////////////////////////////////////
//	S16F101	:	Host -> EQ
//	Current Process Control Data Request
struct	stS16F101Type
{
	stHsmsMsgHeadType	MsgHead;
	
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
};

struct	stS16F102Type
{
	stHsmsMsgHeadType	MsgHead;
	
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nTACK;
	char	szReason[MAX_MESSAGE_REASON_LEN+1];

	long	nGlsCnt;

	stProcCtrlDataType	stProcCtrlData[MAX_PROCESS_CTRL_DATA_QUEUE_COUNT];
};
/////////////////////////////////////////////////////
//	S16F103	:	Host -> EQ
//	Process Control Information Send
struct	stS16F103Type
{
	stHsmsMsgHeadType	MsgHead;
	
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nMode;		// 1:Creation, 2:Deletion

	long	nGlsCnt;

	char	szSetTime[MAX_DATE_TIME_LEN];

	stProcCtrlDataType	stProcCtrlData[MAX_PROCESS_CTRL_DATA_QUEUE_COUNT];
};

struct	stS16F104Type
{
	stHsmsMsgHeadType	MsgHead;
	
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nTMACK;
	char	szReason[MAX_MESSAGE_REASON_LEN+1];
};
/////////////////////////////////////////////////////
//	S16F105	:	Host -> EQ
//	Process Control Data Create/Delete Report
struct	stS16F105Type
{	
	stHsmsMsgHeadType	MsgHead;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nMode;		// 1:Creation,  2:Deletion
	long	nByWho;

	long	nGlsCnt;
	stProcCtrlDataType	stProcCtrlData[MAX_PROCESS_CTRL_DATA_QUEUE_COUNT];
};

struct	stS16F106Type
{
	stHsmsMsgHeadType	MsgHead;

	long	nTACK;	// ACK = 0 (Fixed)
};

struct	stS16F107Type
{
	stHsmsMsgHeadType	MsgHead;
	
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	long	nResult;

	long	nGlsCnt;
	stProcCtrlDataType	stProcCtrlData;
};

struct  stS16F108Type 
{
	stHsmsMsgHeadType	MsgHead;

	long	nTACK;	// ACK = 0 (Fixed)
};

#endif // _HostMsg_H_
