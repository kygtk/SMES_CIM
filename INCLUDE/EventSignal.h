#ifndef	__EventSignal_H__
#define	__EventSignal_H__

//	MMI와 Host Task 간의 Signal Code
enum eSigHostTask
{
	sigNone					=	-1	,
	sigShutDown				=	9999,	//	MMI로 부터의 Host Task 종료 Signal
	sigWakeUpProcess		        =	9000,
	sigEnd					=	sigShutDown,
	
	//	설비 공통 Signal
	sigAlarmOccured			=	1	,
	sigAlarmTreated			=	2	,
	sigTerminalRequest		=	3	,	//	자동화 Terminal Service
	sigBuzzerStop			=	4	,	//	Buzzer Off
	sigOperatorCall			=	5	,	//	Buzzer On
	sigMachineCommand		=	6	,
	sigTimeSetRequest		=	7	,	//	20050818 하위 제어기 시간 동기화
	sigEQSpecCtrlEvent		=	8	,	// 07-04-11 H/S Status
	sigAlarmResetEvent		=	9	,
	sigTerminalMsg			=   10	,	

	//	Process Event Report ( SCH -> GUI )
	sigGlassStart					,
	sigGlassEnd						,
	sigCassetteEnd					,

	//	Process Command ( GUI -> SCH )
	sigProcessStart 				,
	sigProcessCancel 				,
	sigProcessAbort 				,
	sigProcessPause 				,	// 2004-07-09 추가 
	sigProcessResume 				,	// 2004-07-09 추가  
	sigProcessGlassCancel			,	// 20060207 추가
	sigCassetteReClamp 				,	//16

	sigEQProcessPause 				,
	sigEQProcessResume	 			,
	sigEQStateChangeToPM			,
	sigEQStateChangeToNormal		,
	sigEQSpoolProcessDataReq		,	//20
	sigEQCycleStopReq				,

	sigGlassProcessData				,	// Glass Loading 전 Glass Transfer Data 및 Recipe Data Write 요구
	sigPPIDPrepareReq				,	// 20060130 PPID에 의한 Job 예약 Signal ( 노광기 )
	sigRecipeDownloadReq			,
	sigJobDataClear					,

	//	Parameter Change
	sigModeChange					,	//MMI -> Host Task로의 Signal
	sigOnLineParamChange			,   //25 	//* 03-06-11(주석추가)Gui, PLC 변경
	sigEQConstantChange				,	//* 03-06-11(주석추가)Gui, PLC 변경.
	sigRecipeDataChange				,	//	UI -> Host
	sigFlowRecipeChange				,	//	Host -> UI [7/10/2003]
	sigCleanerRecipeChange			,	//	Host -> UI
	sigDBKRecipeChange				,	//	Host -> UI [7/10/2003]
	sigSBKRecipeChange				,	//	Host -> UI 
	sigBufferRecipeChange			,	//	Host -> UI 
	sigInterfaceRecipeChange		,	//	Host -> UI 
	sigPEBRecipeChange				,
	sigDeveloperRecipeChange		,	//	Host -> UI 
	sigPBKRecipeChange				,
	sigEtchRecipeChange				,	// 07-03-19 Add
	sigStripRecipeChange			,	// 07-03-19 Add


	//	PLC Event Report
	sigGlassDelete					,
	sigGlassMake					,
	sigJudgementEvent				,
	sigPPIDEvent					,
	sigProcessEndEvent				,	//	mschoi
	sigAllGlassDelete				,
	sigChemicalChange				,
	sigManualCellLoad				,   // 면취 세정기 Manual Cell Load
	sigPhotoBackModeStart			,   // Photo Crack 감지
	sigHostPanelValid				,
	sigMNCellCancel					,
	sigMNGlassRunStart				,
	sigVCRReadFail					,

	sigGlassSendFail				,  // 2010-08-24 add

	sigIndexerCommand				,
	sigIndexerEvent					,
	sigEcoModeRequest				,

	// RPC //
	sigRPCDataSet					,
	sigRPCDataDeletion				,
	sigRPCDataStateChange			,	
    sigRPCDataHistoryWrite			,
	sigRPCDeleteGUIToSCH			,
    sigRPCCreateGUIToSCH			,

	sigGECDCrackEvent				, 
	sigRunDataLog					,

	//	Host I/F 관련 Signal
	sigHsmsParamChange		=	1000,
	sigConnectionStatus				,

	sigS1F1ToHost					,
	sigS1F1FromHost					,
	sigS1F2ToHost					,
	sigS1F2FromHost					,
	sigS1F3							,
	sigS1F4							,
	sigS1F5							,
	sigS1F6							,
	sigS1F11						,
	sigS1F12						,
	sigS1F15						,
	sigS1F16						,
	sigS1F17						,
	sigS1F18						,

	sigS2F15						,
	sigS2F16						,
	sigS2F17						,	//	Not Used, But Common Spec
	sigS2F18						,	//	Not Used, But Common Spec
	sigS2F23						,
	sigS2F24						,
	sigS2F25						,
	sigS2F26						,
	sigS2F29						,
	sigS2F30						,
	sigS2F31						,	
	sigS2F32						,	
	sigS2F41						,
	sigS2F42						,
	sigS2F101						,
	sigS2F102						,
	sigS2F103						,
	sigS2F104						,

	sigS3F1							,
	sigS3F2							,
	sigS3F101						,
	sigS3F102						,
	sigS3F201                       ,
	sigS3F202                       ,
		
	sigS5F1							,
	sigS5F2							,
	sigS5F5							,
	sigS5F6							,
	sigS5F101						,
	sigS5F102						,
	sigS5F103						,
	sigS5F104						,
	sigS5F105						,

	sigS6F1							,
	sigS6F2							,
	sigS6F3							,
	sigS6F4							,
	sigS6F11						,
	sigS6F12						,
	sigS6F13						,
	sigS6F14						,

	sigS7F1                         ,
	sigS7F2                         ,
	sigS7F9                         ,
	sigS7F10                        ,
	sigS7F23						,
	sigS7F24						,
	sigS7F25						,
	sigS7F26						,
	sigS7F33						, 
	sigS7F34						,	 
	sigS7F101						,
	sigS7F102						,
	sigS7F103						,
	sigS7F104						,
	sigS7F105						,
	sigS7F106						,
	sigS7F107						,
	sigS7F108						,
	sigS7F109						,
	sigS7F110						,	
	sigS7F111						,
	sigS7F112						,
	sigS9F1							,
	sigS9F3							,
	sigS9F5							,
	sigS9F7							,
	sigS9F9							,
	sigS9F11						,
	
	sigS10F1						,
	sigS10F2						,
	sigS10F3						,
	sigS10F4						,
	sigS10F9						,
	sigS10F10						,
	sigS10F101						,
	sigS10F102						,

	sigS64F1						,
	sigS64F2						,
	sigS64F3						,
	sigS64F4						,
	sigS64F5						,

	sigS16F101						,
	sigS16F102						,
	sigS16F103						,
	sigS16F104						,
	sigS16F105						,
	sigS16F106						,
	sigS16F107						,
	sigS16F108						,
};

///////////////////////////////////
//	PLC Ctrl Main Signal List

enum	eSigPLCTask
{

	eSigPMNone				=	-1	,
	eSigPMShutDown			=	0	,
	eSigPMManualOp			=	200	,
	eSigPMManualRly			=	201	,
	eSigSendGlassToPM				,
	eSigMelsecInitialized			,

	eSigECIDChange					,
	eSigECOChange					,		//PLC -> SCH ECO MODE Change 후 보고
//	eSigEOIDChange					,
	
	//eSigPMGlassRemove				,	// ManualRly 처리함.
	//eSigPMAllGlassRemove			,
	//eSigPMGlassGenerate			,
	//eSigPMGlassShift				,
	//eSigPMGlassEvent				,
	//eSigPMEQCtrlEvent				,
	
	eSigPMAlarmOccured				,
	eSigPMAlarmTreated				,
	
	sigPMCommandReply			=	10036	,	// Data Change 및 Command에 대한 응답.
};


///////////////////////////////////
//	PLC Ctrl Main Signal List
enum	eSigTempTask
{
	eSigNone				=	-1,
	eSigSetOpRun			=	0,
	eSigSetOpStop			=	1,
	eSigSetPBand			=	2,
	eSigSetITime			=	3,
	eSigSetDTime			=	4,
	eSigSetTemp				=	5,
	eSigSetUpLimit			=	6,
	eSigSetLoLimit			=	7,
	eSigGetPBand			=	8,
	eSigGetITime			=	9,
	eSigGetDTime			=	10,
	eSigGetTemp				=	11,
	eSigGetUpLimit			=	12,
	eSigGetLoLimit			=	13,
	eSigGetMeasTemp			=	14,
	eSigGetMeasUpAlarm		=	15,
	eSigGetMeasLoAlarm		=	16,
	eSigGetMeasCtrlOut		=	17,
	eSigGetMeasSetTemp		=	18,
	eSigGetOpMode			=	19,
	eSigGetAllSetData		=	20,
	eSigGetAllMeasData		=	21,
};

#endif	//
