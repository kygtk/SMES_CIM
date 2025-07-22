#ifndef __Enum_h__
#define __Enum_h__

//////////////////////////////////////////////////
//	Used Enum Definition
//	1.	Equipment Modeling ( Configuration )
//	2.	Equipment State (EQ State, Process State)
//	3.	Indexer Interface Related
//		- Port State
//		- Control Command
//	4.	HSMS(자동화) Related...

//	1.	Equipment Modeling ( Configuration )
enum eEQPhotoModuleType
{
	eModuleType_Indexer		=	1,
	eModuleType_Cleaner		=	2,
	eModuleType_DBK			=	3,
	eModuleType_Coater		=	4,
	eModuleType_VCD			=	5,
	eModuleType_SBK			=	6,
	eModuleType_Interface	=	7,
	eModuleType_Exposure	=	8,
	eModuleType_EdgeExp		=	9,
	eModuleType_PEB			=	10,
	eModuleType_Developer	=	11,
	eModuleType_PBK			=	12,
	eModuleType_Inspector	=	13,
	eModuleType_Macro		=	14,
};

enum eEQEtchStripModuleType
{
	eModuleType_Etcher		=	2,
	eModuleType_Stripper	=	3,
	eModuleType_Bypass		=	4,
	eModuleType_Buffer		=	5,
};

enum eEQPFCModuleType
{
	eModuleType_PFC			=	2,
	
	// 확장용으로 Module을 하나 추가
	eModuleType_EX		=	3,
};

enum eEQEDGEModuleType
{
	eModuleType_Edge		=	2,
};

enum eEQWRUModuleType
{
	eModuleType_WRU			=	2,
};

enum eEQLCPIModuleType
{
	eModuleType_LCPI			=	2,
};

enum eEQLCODFModuleType
{
	eModuleType_LCODF			=	2,
};

enum eEQLCRWModuleType
{
	eModuleType_LCRW			=	2,
};


//--------------- 설비 유형 ----------------
enum eEQPType
{
    eEQType_FULL			=	1,     // IDX + Process EQs
    eEQType_LOCAL			=	2,     // Local 설비 (Photo/Etch&strip/Etch/Strip/PFC/Edge)
};

//--------------- 설비군 타입 ----------------
enum eEQType
{
    eEQType_PHOTO			=	1,      //PHOTO INLINE
    eEQType_ETCHSTRIP		=	2,      //ECH/STRIP INLINE
    eEQType_ETCH			=	3,      //LOCAL ETCHER
    eEQType_STRIP			=	4,      //LOCAL STRIPPER
    eEQType_PFC				=	5,      //PFC 세정기
    eEQType_EDGE			=	6,      //면취 세정기
	eEQType_WRU				=	7,      //CGR2500
	eEQType_LCPI			=	8,      //LC PI 전 Cleaner
	eEQType_LCODF			=	9,      //LC ODF 전 Cleaner
	eEQType_LCRW			=	10,     //LC PI REWORK
};

//--------------- 공정별 설비 SUB 타입 ----------------
enum eEQPHSubType   //PHOTO
{
    eEQPHSubType_GATE		=	1,
    eEQPHSubType_SD			=	2,
    eEQPHSubType_PASSIVE	=	3,
    eEQPHSubType_CFITO		=	4,
};

enum eEQETCHSTRIPSubType //ETCH/STRIP INLINE
{
    eEQETCHSTRIPSubType_GATE		=	1,
    eEQETCHSTRIPSubType_PIXEL		=	2,
    eEQETCHSTRIPSubType_CFITO		=	3,
    eEQETCHSTRIPSubType_RW			=	4,	
	eEQETCHSTRIPSubType_PIXELCLN	=	5,	//Pixel Cleaner
};

enum eEQLocalETSubType       //LOCAL ETCH
{
    eEQLocalETSubType_SD1st_1 		=	1,
	eEQLocalETSubType_SD1st_2 		=	2,
	eEQLocalETSubType_SD2nd 		=	3,
};

enum eEQLocalSTSubType       //LOCAL STRIP
{
    eEQLocalSTSubType_SDSTRIP 		=	1,
};

enum eEQPFCSubType  //PFC 세정기
{
    eEQPFCSubType_TwoWay			=	1,
	eEQPFCSubType_OneWay			=	2,
    eEQPFCSubType_PASSIVE_1			=	3,			// Roll Brush used		VC05호기
    eEQPFCSubType_PASSIVE_2			=	4,			// Roll Brush Not used	VC10호기
};

enum eEQEDGESubType //면취 세정기
{
    eEQEDGESubType_EDGE			=	1,
    eEQEDGESubType_SCRIBE		=	2,
};

enum eEQLCPISubType                //LC PI 전 세정기
{
    eEQLCPISubType_PICLN  			=	1,
};

enum eEQLCODFSubType               //LC ODF 전 세정기
{
    eEQLCODFSubType_ODFCLN  		=	1,
};

enum eEQLCRWSubType            //LC PI REWORK
{
    eEQLCRWSubType_PIRW  	=	1,
};

//	2.	Equipment State (EQ State, Process State)
//	Equipment State
enum	eEquipmentState
{
	eEQState_None		=	0,
	eEQState_Normal		=	1,
	eEQState_Fault		=	2,
	eEQState_PM			=	3,
};

//	Equipment Process State
enum	eEquipmentProcessState
{
	eProcState_None		=	0,
	eProcState_Init		=	1,
	eProcState_Idle		=	2,
	eProcState_Setup	=	3,
	eProcState_Ready	=	4,
	eProcState_Execute	=	5,
	eProcState_Pause	=	6,
};

//	3.	Indexer Interface Related...
//	Port Running State
enum ePortRunningState
{
	ePortRun_Empty		=	0,
	ePortRun_Idle		=	1,
	ePortRun_Ready		=	2,
	ePortRun_Waiting	=	3,
	ePortRun_Reserved	=	4,
	ePortRun_Busy		=	5,
	ePortRun_Completed	=	6,
	ePortRun_Aborted	=	7,
	ePortRun_Canceled	=	8,
	ePortRun_Paused		=	9,
	ePortRun_Disable	=	10,
	ePortRun_Error		=	11,
};

//	Machine Command ID
enum eMachineCommandParam
{
	eMachine_Start	=	1,	
	eMachine_Stop	=	2,	
	eMachine_Pause	=	3,	//	To PLC : Cycle Stop Set
	eMachine_Resume	=	4,	//	To PLC : Cycle Stop Reset
	eMachine_Auto	=	5,
	eMachine_Manual	=	6,
	eMachine_Reset	=	7,	//	Indexer Robot Initialize
	eMachine_PM		=	8,	//	EQ State Change To PM Request
	eMachine_Normal	=	9,	//	EQ State Change To Normal Request
	eMachine_CycleStop	=	10,
};

//	ECO MODE EOMD
enum eEcoModeEOMDParam
{
	eEOMD_All_Unit	=	1,			// EOMD 세부로 구별하지 않고 1번으로 전제 Sleep Mode를 구분한다. (DI, Brush, Spray)
	eEOMD_Reserved	=	2,	
};

//	ECO MODE EOV
enum eEcoModeEOVParam
{
	eEOV_WorkingMode	=	0,			// EOMD 세부로 구별하지 않고 1번으로 전제 Sleep Mode를 구분한다. (DI, Brush, Spray)
	eEOV_StandyByMode_1	=	1,	
	eEOV_StandyByMode_2	=	2,	
};

//		Step Move ID
enum eStepMotionType	//eMotionType
{
	eMotionGet		=	1,
	eMotionMove		=	2,
	eMotionPut		=	3,	// (Lay)
	eMotionRotate	=	4,
};

//		Glass Handling Parameter
enum ePositionType
{
	ePositionPort		=	1,
	ePositionStage		=	2,
	ePositionBuffer		=	3,
	ePositionNotUsed	=	4,
	ePositionWait		=	5,
	ePositionTestPort	=	6,
	ePositionTestStage	=	7,
	ePositionOther		=	8,
};

enum eCycleMoveType
{
	eFromPortToPort		=	1,
	eFromPortToStage	=	2,
	eFromPortToStgWait	=	3,

	eFromStageToStage	=	4,
	eFromStageToPort	=	5,
	eFromStageToWait	=	6,

	eFromGetStgOrPort	=	7,
	eFromPutStgOrPort	=	8,
};

enum eGlassHandlingParam
{
	eCycleMoveReq		=	1,
	eStepMoveReq		=	2,
	eGlassCancel		=	3,
};

//		Port Command Parameter
enum ePortCommandParam
{
	ePortReservedReq	=	1,
	ePortStartReq		=	2,
	ePortCompleteReq	=	3,
	ePortCancelReq		=	4,
	ePortAbortReq		=	5,
	ePortDisableReq		=	6,
	ePortSTKAGVAbortReq =	7,
	ePortPauseReq		=	8,
	ePortTypeChangeReq	=	9,
	ePortRemappingReq	=	10,	
	ePortGlsSizeChgReq	=	11,	
	ePortStateWaitReq	=	12,
	ePortChuckingReq	=	13,
	ePortUnchuckingReq	=	14,
	ePortCassIDReadReq	=	15,
	ePortBCRModeChgReq	=	16,
};

//	Indexer Manual Operation Code List
enum eIndexerManualOpCode
{
	eManOpMachineCommand	=	1,
	eManOpAlarmClearReq		=	2,
	eManOpTerminalMsgReq	=	3,
	eManOpOperatorCall		=	4,	//	Indexer Buzzer On Request
	eManOpWaitPosiChangeReq	=	5,
	eManOpArmDataClearReq	=	6,
	eManOpGlassHandling		=	7,
	eManOpPortCommand		=	8,
	eManOpSpecialCommand	=	9,
	eManOpPortEventReply	=	10,
	eManOpGlassThickChange	=	11,
};


//	4. HSMS(자동화) Related...
enum eByWho
{
	eByWhoHost		=	1,
	eByWhoOperator	=	2,
	eByWhoEquipment	=	3,
};

//	Glass State
enum eGlassState
{
	eGlass_Nothing				=	0,
	eGlass_Idle					=	1,
	eGlass_SelectedToProcess	=	2,
	eGlass_Processing			=	3,
	eGlass_Done					=	4,
	eGlass_Aborting				=	5,
	eGlass_Aborted				=	6,
	eGlass_Canceled				=	7,

};

//	Batch Job State
enum eJobState
{
	eJobState_None				=	0,
	eJobState_WaitForStart		=	1,
	eJobState_ReserveForStart	=	2,
	eJobState_Processing		=	3,
	eJobState_Done				=	4,
	eJobState_Pausing			=	5,
	eJobState_Paused			=	6,
	eJobState_Aborting			=	7,
	eJobState_Aborted			=	8,
	eJobState_Canceled			=	9,
};

//	Remote Command ID
enum eRemoteCommand
{
	eProcessJobStart				=	1,
	eProcessJobCancel				=	2,
	eProcessJobAbort				=	3,
	eGlassProcessCancel				=	12,
	eGlassProcessAbort				=	13,
	eClampTheCassette				=	36,
	eEQProcessPause					=	51,
	eEQProcessResume				=	52,
	eEQStateChangeToPM				=	53,
	eEQStateChangeToNormal			=	54,
	eSpoolProcEndDataRequest		=	55,
	eEQCycleStopRequest				=   57,
	eManualCellProcessStartCommand  =	62,
	eManualCellProcessCancelCommand =	63,
	eJudgementDownloadCommand		 =	161,	//T8Y
};

enum eSFCD
{
	eSFCD_OnlineParameter			=	1,
	eSFCD_PortStates				=	2,
	eSFCD_GlassTracking				=	3,
	eSFCD_ModuleStates				=	4,
	eSFCD_StandardTactTimeRequest   =	7, 
	eSFCD_EQCurrentInterlockRequest =	8,
	eSFCD_SoftWareVersionRequest	=	30,
};

enum eEOID
{
	eEOID_TraceComponet				=	1,
	eEOID_EQStateTrace				=	2,
	eEOID_EQProcStateTrace			=	3,
	eEOID_EQProcStateLapse			=	4,
	eEOID_OptionForStart			=	5,
	eEOID_PostponeCount				=	6,
	eEOID_LoadReject				=	7,
	eEOID_RobotAccessOrder			=	8,
	eEOID_PortType					=	9,
	eEOID_ULPortGlassSort			=	10,
	eEOID_CassetteDemand			=	11,
	eEOID_NGPortOpMode				=	12,
	eEOID_JudgmentMode				=	13,
	eEOID_VCRReadingMode			=   15,
	eEOID_WaitTimeForGlsIDKeyInput  =	16,
	eEOID_SetOfQtyCount				=	30,
	eEOID_JobEndSpoolMode			=	31,
	eEOID_AutoAbortMode				=	32,

	eEOID_New_GlassTrace_IN			=	101,
	eEOID_New_GlassTrace_OUT		=	102,
	eEOID_New_EQStateTrace			=	103,
	eEOID_New_EQProcStateTrace		=	104,

	eEOID_ECOMode					=	201,		//BoB	
	eEOID_ProcessControlPriority	=   202,
};

//	CEID & Remote Command ID
enum eCEID
{
	//	Process Event
	eJobProcessStart		=	1,
	eJobProcessCancel		=	2,
	eJobProcessAbort		=	3,
	eJobProcessEnd			=	4,
	eSpooledProcessEnd		=	5,
	ePanelStartForIdx		=	6,
	ePanelEndForIdx			=	7,
	//	Glass Event
	eProcessResultEvent		=	11,
	eGlassIsCanceled		=	12,
	eGlassIsAborted			=	13,		// Not Used
	eGlassScrap				=	14,		//	Glass Delete
	eGlassUnscrap			=	15,		//	Glass Undo(Make)
	ePanelStartForModule	=	16,
	ePanelEndForModule		=	17,
	eSortKeyValueMismatch	=	18,		//	NG Port Sorting Type : Device+Step, Batch + Step인 경우
	eGlassMoveOut           =   19,
	eGlassMoveIn            =   20,

	//	Port Event
	eCassettePreLoad		=	31,
	eCassetteClampOn		=	32,
	eCassetteLoad			=	33,
	eCassetteUnloadRequest	=	34,
	eCassetteUnload			=	35,
	eCassetteLoadRequest	=	36,
	eCassetteLoadRejected	=	37,
	ePortParamChange		=	38,		//	Port Type, Port Mode, Sort Type, Cassette Demand : NG <-> OK 변경되는 경우에 고려

	//	Equipment Event
	eEQProcStateChagned		=	51,
	eEQProcStateTimeover	=	52,
	eEQStateChanged			=	53,

	eStandardTactTimeChanged  = 56,

	eEQSpecificStep			=	59, 

	eChangeToOffline		=	71,		
	eChangeToOnlineLocal	=	72,
	eChangeToOnlineRemote	=	73,

	//	Equipment Parameter Event
	eEOIDChanged			=	101,
	eECIDChanged			=	102,

	//   PanelID Validation Event
	ePanelIDReadFail        =   111,
	ePanelIDMismatch        =   113,
	eKeyInTimeout           =   114,

	// Edge Cleaner Manual Cell Loading
	eSoftVersionChanged		=	130,
	ePortCarrierClampOn		=	132,
	ePortCarrierLoadComplete =  133,

	eCrackGlassDetect       =   141,    // T8 Added
	eCrackGlassRelease      =   142,    // T8 Added
	eCrackConfirmEvent      =   143,    // T8 Added

	//  Equipment Specific Control Event
	eUserLogin              =   161,
	eUserLogout             =   162,

	eMaterialStockIn		=   201,
	eMaterialStockOut		=   202,
	eMaterialConsumeStart	=	207,	//	T7-2 Not Used
	eMaterialConsumeEnd		=	208,	//	T7-2 Not Used

	eSpecCurPPIDChange		=	401,
	eSpecGlassHSTimeout		=	402,
	eSpecEmgPauseHappen		=	403,
	eSpecEmgStopHappen		=	404,
	eSpecPPIDIntLockHappen	=	405,
	eSpecMechaIntLockHappen	=	406,
	eSpecGlassTransferStop	=	407,
	eSpecGlassLoadingStop	=	408,
	eSpecSpecificIntLockOn	=	409,
	eSpecPPIDAvailableReply	=	410,
	eSpecPPIDPrepareReply	=	411,
	eSpecConditionChange	=	412,
	eSpecHSCheckFail		=	413,
	eSpecGlassSendFail		=	414,

	eSpecEQNetworkError		=	704,
};


// PLC I/F Acknowledge Code
enum	eAckCode
{
	eOK				=	1,
	eNG				=	2,	
	eACK			=	0x41,	// 'A'
	eNAK			=	0x4E,	// 'N'
};

//	자동화 Port Object 관련 상수 Define
enum	eIndexerPortType
{
	ePortType_Both		=	1,
	ePortType_Loader	=	2,
	ePortType_Unloader	=	3,
};

enum	ePortModeType
{
	ePortMode_OK	=	1,
	ePortMode_NG	=	2,
	ePortMode_RJ	=	3,
	ePortMode_RP	=	4,
	ePortMode_RW	=	5,
};

enum	ePortSortType
{
	eSort_Quantity		=	1,
	eSort_ReverseFill	=	2,
	eSort_Reverse		=	3,
	eSort_Size_Thick	=	4,
	eSort_Process_Step	=	5,
	eSort_Batch_Step	=	6,
	eSort_Process		=	7,
	eSort_Batch			=	8,
};

enum	eCassetteDemandType
{
	eNormal_For_Robot	=	1,
	eEmpty_For_Robot	=	2,
	eNormal_For_Lifter	=	3,
	eEmpty_For_Lifter	=	4,
};

enum	eCassetteType
{
	eNormal_Cassette	=	1,
};

//	20060127
enum	eNGPortOpModeType
{
	eNGOp_Off		=	0,
	eNGOp_Absolute	=	1,
	eNGOp_Flexible	=	2,
};

enum	eRecipeChangeModeType
{
	eRecipe_Create	=	1,
	eRecipe_Delete	=	2,
	eRecipe_Modify	=	3,
};

enum	eRecipeType
{
	eRecipeType_FlowRcp		=	1,
	eRecipeType_Cleaner		=	2,
	eRecipeType_DBK			=	3,
	eRecipeType_COT			=	4,
	eRecipeType_VCD			=	5,
	eRecipeType_SBK			=	6,
	eRecipeType_Interface	=	7,
	eRecipeType_Develop		=	8,
	eRecipeType_PEB		    = 	9,	
    eRecipeType_PBK			=	10,	

    eRecipeType_ETC			=	11,   //	LOCAL ETCHER
    eRecipeType_STP			=	21,   //	LOCAL STRIPPER
    eRecipeType_PFC			=	31,   //	PFC 세정기
    eRecipeType_EDG			=	41,   //	면취 세정기
};

enum	eCoaterPumpStepType
{
	ePump_Step1	=	1,
	ePump_Step2	=	2,
	ePump_Step3	=	3,
};



/////////////////
//	EQ Constant 관련 Define
enum	eEquipmentConstantType
{
	eConstant_Flow   		=	1,	//	유량/압력
	eConstant_Press			=	2,
	eConstant_Temperature	=	3,	//	온도
	eConstant_TankInfo		=	4,	//	Tank 정보 : Life Time/Count....
	eConstant_EPD			=	5,
	eConstant_AP1			=	6,
	eConstant_AP2			=	7,
	eConstant_AP3			=	8,
	eConstant_HP1			=	9,
	eConstant_HP2			=	10,
	eConstant_HP3			=	11,
	eConstant_CP1			=	12,
	eConstant_CP2			=	13,
	eConstant_CP3			=	14,
	eConstant_TactTime		=	15,
	eConstant_Common		=	16,		
	eConstant_Barcode		=	17,

	eConstant_TotalConsum      =   20,
};

// Word Size Define
enum	eWordType
{
	eWordSize_1Word		= 1,
	eWordSize_2Word		= 2,
	eWordSize_3Word		= 3,
	eWordSize_4Word		= 4,
	eWordSize_5Word		= 5,
};

enum	eFlowParamType	
{
	eFlow_CurVal				=	1,
	eFlow_UpperStopLimitVal		=	3,
	eFlow_UpperWarningLimitVal	=	4,
	eFlow_LowerStopLimitVal		=	5,
	eFlow_LowerWarningLimitVal	=	6,
};

enum	ePressParamType	
{
	ePress_CurVal				=	1,
	ePress_UpperStopLimitVal	=	3,
	ePress_UpperWarningLimitVal	=	4,
	ePress_LowerStopLimitVal	=	5,
	ePress_LowerWarningLimitVal	=	6,
};

enum	eTempParamType
{
	eTemp_CurVal				=	1,
	eTemp_SetVal				=	2,
	eTemp_UpperStopLimitVal		=	3,
	eTemp_UpperWarningLimitVal	=	4,
	eTemp_LowerStopLimitVal		=	5,
	eTemp_LowerWarningLimitVal	=	6,

};

enum	eTankParamType
{
	eTank_LifeCount			=	1,
	eTank_LifeTime			=	2,
};

enum	eOnLineMode
{
	eOnLine_Offline			=	1,
	eOnLine_Local			=	2,
	eOnLine_Remote			=	3,
};

enum	eHostAlive
{
	eHost_Connected			=	1,
	eHost_NotSelected		=	2,
	eHost_Selected			=	3,
};

// 2004-07-06 추가 kim.m.i
enum	eGlassType
{
 	eGlassType_Normal	=	4,
 	eGlassType_First	=	5,
 	eGlassType_End		=	6,
};

//	System Config관련 신규 추가
enum	eUnitType
{
	eUnitType_Robot		=	1,
	eUnitType_Port		=	2,
	eUnitType_Conveyor	=	3,	//	Netural C/V 
	eUnitType_EUV		=	4,	//	4 : Eximer UV
	eUnitType_Rinse		=	5,	//	5 : Rinse
	eUnitType_AAJet		=	6,	//	6 : AA-Jet
	eUnitType_AirKnife	=	7,	//	7 : Air Knife
	eUnitType_SlopeCV	=	8,	//	8 : Slope C/V
	eUnitType_TurnCV	=	9,	//	9 : Turn C/V
	eUnitType_Buffer	=	10,	//	10: Buffer
	eUnitType_UpDown	=	11,	//	10: Buffer

	eUnitType_AP		=	15,	//	11 : AP
	eUnitType_HP		=	16,	//	12 : HP
	eUnitType_CP		=	17,	//	13 : CP

	eUnitType_Develop	=	21,	//	21 : Developper
	eUnitType_IUV		=	22,	//	22 : I-Line UV

	eUnitType_BypassConv=31,		//Etcher 상공 Conveyor
	eUnitType_Etcher=32,
	eUnitType_Stripper=33,
	eUnitType_Epd=34,
	eUnitType_Plazma=35,
	eUnitType_HF=36,
};


//Bake Plate Type
enum	eBakePlateType
{
	eAP=1,		//Adhesive Plate	
	eHP=2,		//Hot Plate
	eCP=3,		//Cool Plate
};

//	20050727
enum	eIndexerMakerType
{
	eIndexerMaker_SFA	=	1,
	eIndexerMaker_RORZE	=	2,
	eIndexerMaker_MECHA	=	3,
};

//	20050804
enum	eIdxRobotCurrentPositionType
{
	eCurPos_Port1		=	1,
	eCurPos_Port2		=	2,
	eCurPos_Port3		=	3,
	eCurPos_Port4		=	4,
	eCurPos_Port5		=	5,
	eCurPos_Port6		=	6,

	eCurPos_Stage1		=	11,	//	Entrance
	eCurPos_Stage2		=	12,	//	Exit

	eCurPos_Moving		=	21,
};

//	Robot Arm State
enum	eIdxRobotArmCurrentStateType
{
	eArmState_Stretch_UP_Move	=	1,
	eArmState_Stretch_UP_Cmpl	=	2,
	eArmState_Stretch_DN_Move	=	3,
	eArmState_Stretch_DN_Cmpl	=	4,
	eArmState_Stretching_Move	=	5,
	eArmState_Stretching_Cmpl	=	6,
	eArmState_Folding_Move		=	7,
	eArmState_Folding_Cmpl		=	8,
};

enum eHandshakeType
{
	eRobotToConveyor	=	1,
	eConveyorToRobot	=	2,
	eConveyorToConveyor =	3,
};

//Unique ID 순서
enum eUniqueID
{
	eUniqueID_GlassNo	=	0,
	eUniqueID_JobOrder	=	1,
	eUniqueID_SlotNo	=	2,
	eUniqueID_PortNo	=	3,
};

//	Host Acknowledge code Define
enum eTMAckType
{
	eTMAck_Acknowledge			=	0,

	eTMAck_DataOmission			=	1,
	eTMAck_DataMismatch			=	2,
	eTMAck_DataOutOfRange		=	3,
	eTMAck_DataDuplication		=	4,
	eTMAck_ConditionNotMatch	=	101,
	eTMAck_AlreadyRequiredState	=	102,
	eTMAck_HWConditionError		=	103,
 	eTMAck_MessageNotSupport    =	51,

	eTMAck_ParamNameError		=   500,
	eTMAck_ParamValueError		=   501,

};

enum eAPC_RPC_State
{
	eWaitingState	= 1,
	eRunningState   = 2,
	eDoneState      = 3,
};

enum eRPCDataChange
{
	eRPC_CREATE			= 1,
	eRPC_DELETE			= 2,
	eRPC_EXPIRATION		= 3,
	eRPC_MODIFY			= 4,
	eRPC_STATE_CHANGE	= 5,
};

//T8-2 Ph2 Action Log 관련 Unit Type
enum eActionLogUnitType
{
	eActionLog_CV		= 0,
	eActionLog_Robot	= 1,
	eActionLog_Stage	= 2,
};

enum eECIDType
{
	eECIDMultiUse		= 1,
	eECIDSingleUse		= 2,
};


//	T8Y Data Type 재정의 
enum eDataDivideType
{
	eDataDivide_None			=	0,		//'정수
	eDataDivide_Float1			=	1,		//'나누기 10   = 소수 첫째자리
	eDataDivide_Float2			=	2,		//'나누기 100  = 소수 둘째자리
	eDataDivide_Float3			=	3,		//'나누기 1000 = 소수 셋째자리
};

// 2010-08-24 add
enum eGlassSendFailCodeType
{
	eFailCode_Duplication	= 1,
	eFailCode_Omission		= 2,
	eFailCode_Availability	= 3,
};

enum eEvtType
{
	eEvtType_EQState		= 1,
	eEvtType_EQProcessState = 2,
	eEvtType_Glass_IN		= 3,
	eEvtType_Glass_OUT		= 4,
};

enum eHandShakeFail_Type
{
	eHSFail_Duplication = 1,
	eHSFail_Omission	= 2,
	eHSFail_Availablity	= 3,
};

enum eHandShakeFail_Dupl_Type
{
	eHSFail_Dupl_H_PanelID	= 0,
	eHSFail_Dupl_E_PanelID	= 1,
	eHSFail_Dupl_UniqueID	= 2,
};

enum eHandShakeFail_Omis_Type
{
	eHSFail_Omis_H_PanelID	= 0,
	eHSFail_Omis_BatchID	= 1,
	eHSFail_Omis_ProcessID	= 2,
	eHSFail_Omis_ProductID	= 3,
	eHSFail_Omis_StepID		= 4,
	eHSFail_Omis_PPID		= 5,
	eHSFail_Omis_Thickness	= 6,
	eHSFail_Omis_FlowID		= 7,
	eHSFail_Omis_CompCount	= 8,
	eHSFail_Omis_PanelSize	= 9,
};

enum eHandHakeFail_Avai_Type
{
	eHSFail_Avai_PPID		= 0,
	eHSFail_Avai_PanelSize	= 1,
	eHSFail_Avai_Thickness	= 2,
	eHSFail_Avai_FlowID		= 3,
};

enum ePPIDType
{
	ePPIDType_Recipe		= 1,
	ePPIDType_PPID			= 2,
};

enum eSoftVersionType
{
	eSoftVerType_CIM			= 0,
	eSoftVerType_MD1st_PLC		= 1,
	eSoftVerType_MD1st_TOUCH	= 2,
	eSoftVerType_MD2nd_PLC		= 3,
	eSoftVerType_MD2nd_TOUCH	= 4,
};

enum eEndDataIndex		// link Map  상의 Map Index No-1
{
	eDcoll_SEMES_UNIQUE_NO		= 19,
};

enum eRPCMode
{
	eRPC_Host	= 0,
	eRPC_Inline = 1,
};

enum eModuleRunData
{
	eRunData_RunningTime	= 0,
	eRunData_ErrorCount		= 1,
	eRunData_ErrorTime		= 2,
	eRunData_MTBF			= 3,
	eRunData_MTTR			= 4,
	eRunData_RunningRate	= 5,
	eRunData_TotalGlassCnt  = 6,
	eRunData_RWGlassCnt  =7,
};


#endif // __Enum_h__
