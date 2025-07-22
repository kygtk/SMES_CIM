/*****************************************************************************************************
		T8-1 SMA Version Date : 2007-05-10
 *****************************************************************************************************/

#ifndef		__IPCSma_h__
#define		__IPCSma_h__

#include	"Enum.h"
#include	"ConstDefine.h"

/************************************************/
//	온도 통신 관련 구성 정보(Temp.cfg file 참조 )
/************************************************/
/*
struct stSerialType
{
	long	nPortNo;
	long	nCommType;
	long	nBaudRate;
	long	nStartBit;
	long	nStopBit;
	long	nDataBit;
	long	nParityBit;
};


struct stSerialCommCfgType
{
	long			nCtrlType;		// 1 - Temp(CB100), 2 - 적산 유량, 3 - 농도계
	long			nUsedNode;		// 통신 Port 하나당 연결되는 Multi Drop Temp Controller 갯수
	stSerialType	stSerial;		// 통신 Port Parameter 
};
*/

/*******************************************/
//	System Layout Configuration Data 정의
/*******************************************/
//	3. Host Event Control Parameter Structure
struct	stEventCtrlDataType
{
	long	nSelectEQState;
	long	nSelectProcState;
	long	nSelect_GlassIN;
	long	nSelect_GlassOUT;
};

// Unit 단위 구성 요소(최하위 Glass 관리 기준임.) 
struct	stUnitCfgType
{
	long	nModuleID;		//	상위 Module ID
	long	nUnitType;		//	해당 Unit의 종류
							//	eUnitType_Robot		=	1,
							//	eUnitType_Port		=	2,
							//	eUnitType_Conveyor	=	3,	//	Netural C/V 
							//	eUnitType_EUV		=	4,	//	4 : Eximer UV
							//	eUnitType_Rinse		=	5,	//	5 : Rinse
							//	eUnitType_AAJet		=	6,	//	6 : AA-Jet
							//	eUnitType_AirKnife	=	7,	//	7 : Air Knife
							//	eUnitType_SlopeCV	=	8,	//	8 : Slope C/V
							//	eUnitType_TurnCV	=	9,	//	9 : Turn C/V
							//	eUnitType_Buffer	=	10,	//	10: Buffer
							//	eUnitType_UpDown	=	11,	//	10: Buffer
							//	
							//	eUnitType_AP		=	15,	//	11 : AP
							//	eUnitType_HP		=	16,	//	12 : HP
							//	eUnitType_CP		=	17,	//	13 : CP
							//	
							//	eUnitType_Develop	=	21,	//	21 : Developper
							//	eUnitType_IUV		=	22,	//	22 : I-Line UV
							//	
							//	eUnitType_BypassConv=31,		//Etcher 상공 Conveyor
							//	eUnitType_Etcher=32,
							//	eUnitType_Stripper=33,
							//	eUnitType_Epd=34,
							//	eUnitType_Plazma=35,
							//	eUnitType_HF=36,									
	long	nUnitID;		//	해당 Module의 Unit 식별 ID ( 1 ~ n )
	long	nPosiNo;		//	Layout에 따른 순서 번호
	long	nLiftPosiNo;	// Up/Down Lift의 경우 상/중/하를 표시한다.
	long	nUpDownPos;	
	char	szUnitName[MAX_LAYER_MODULE_ID_LEN+1];
	char	szUnitDesc[MAX_UNIT_DESCRIPT_LEN+1];

	long	nProcStart;		// 설비 공정 Start Zone이면 "1"
	long	nProcEnd;		// 설비 공정 End Zone이면 "1"
};

// 단위 제어기 기준의 Module 단위 구성 요소(제어기별 관리 단위임)
struct	stMelsecConfigForModuleType
{
	long	nMelChannelNo;
	long	nMelStationNo;
};
/*
struct	stSerialConfigForModuleType
{
	long	nUsedNodeCount;
};
*/
struct	stHandShakeConfigForModuleType
{
	long	nSendType;			//	To Lower Type
								//	1 : Robot	  -> Conveyer,
								//	2 : Conveyer  -> Robot
								//	3 : Conveyer <-> Conveyer
	long	nRecvType;			//	To Upper Type
};

//ECID, SVID, DVID 공통 Parameter
struct  stParamConfigType
{
	long	nModuleID;			//	1 - Index, 2 -Cleaner.....

	long	nConstantType;		//	1 - Flow 2 - Pressure, 3 - Temperature, 4 - Tank , 5 - EPD
								//  6 -  AP1,  7 - AP2,  8 - AP3
	                            //  9 -  HP1, 10 - HP2, 11 - HP3
								//  12 - CP1, 13 - CP2, 14 - CP3
								//  15 - TactTime, 16 - Common

	long	nIndex;				//	Item별 개수에 따른 Index 번호
	long	nDataType;			//	1 - Int, 2이상이면 소숫점 이하 자리수(2 - 1)
	long	nMapIndex;			//  Link Mapping Index
	long	nSVECIndex;			//  SVID-ECID 매칭을 위한 Index
	long	nWordType;			//  Item 할당 Word Size
	long	nECIDType;	
	BOOL	bRemoteChange;
   
	union unionDefValue
	{
		float	fDefValue;		//	Current Float Set Value
		long	nDefValue;		//	Current Integer Set Value
	}stDefValue;
};

//	ECID 관련 Structure
struct	stECIDConfigType
{
	long	nECID;				//	Equipment Constant ID : Config File에서 Reading
	char	szName[MAX_EQ_CONSTANT_NAME_LEN+1];
	stParamConfigType	stParamCfg[MAX_ECID_ITEM_COUNT];	// 0: Define Value
	                                                        // 1: Lower Stop Limit
															// 2: Upper Stop Limit
															// 3: Lower Warning Limit
															// 4: Upper Warning Limit
};

struct	stECIDTableType
{	
	long	nECIDCount;		//	System 전체 ECID 수량
	long	nECIDCount_Single;
	
	stECIDConfigType	stECIDCfg_Multi[MAX_ECID_MULTI_COUNT];
	stECIDConfigType	stECIDCfg_Single[MAX_ECID_SINGLE_COUNT];
};

//	FDC 관련 Structure
struct	stSVIDConfigType
{
	long	nSVID;				//	Item별 Link Map상의 Constant 값에 대한 위치 번호
	char	szName[MAX_STATUS_VALUE_NAME_LEN+1];
	stParamConfigType	stParamCfg;
};

struct	stSVIDTableType
{	
	long	nSVIDCount;			//	System 전체 SVID 수량
	stSVIDConfigType	stSVIDCfg[MAX_SVID_COUNT];
};

//	Data Collect(S6F13) 관련 Structure
struct	stDVIDConfigType
{
	long	nDVID;				//	Equipment Data COLLECT ID : Config File에서 Reading
	char	szName[MAX_DATA_COLLECT_ITEM_NAME_LEN+1];
	stParamConfigType	stParamCfg;
};

struct	stDVIDTableType
{
	long	nDVIDCount;		//	System 전체 Rundata 수량
	stDVIDConfigType	stDVIDCfg[MAX_DATA_COLLECT_ITEM_COUNT];
};

struct stOtherEQPIDType
{
	char	szUpperEQPID[MAX_EQ_MODULE_ID_LEN+1];
	char	szLowerEQPID[MAX_EQ_MODULE_ID_LEN+1];
};

struct stTrackingInfoType		////T8Y Tracking Module Info
{
	long	nUpperUnitCount;
	long	nUpperProcessDirection;		
	long	nLowerUnitCount;
	long	nLowerProcessDirection;
	
	BOOL	bUpperLowerUsed;
};

struct	stModuleCfgType
{
	BOOL bModuleUsed;
	long	nModuleID;		
	long	nModelType;			//  Module Type에 대한 Sub Type( Maker 구분 등 )
								//	Indexer Case : 1 - SFA, 2 - RORZE, 3 - MECHA
	long	nMakerType;			//	내부 제어 업체 구분 및 타사 설비 업체 구분
	long	nUnitCount;		//	각 Module별 구성 Unit 개수
	long	nRobotCount;		//	TM Unit이 있는 경우에 사용(Indexer, Bake, Buffer, Interface etc...)
	long	nPortCount;			//	Indexer의 Port(Cassette Stage) Count임.
	
	long	nUsedMelCount;		//	각 제어기에서 사용할 Melsec Board 수량(물류, Data)
	stMelsecConfigForModuleType	stMelsecCfg[2];

	long	nHandShakeCount;	//	Send/Recv를 한쌍으로 처리한다.
	stHandShakeConfigForModuleType	stHandShakeCfg[2];

	long	nUsedSerialComm;	//	용도 : 해당 Module에서 Serial 통신하다는 것과 Index를 의미한다.
//	stSerialConfigForModuleType		stSerialCfg;

	long	nTerminalID;		//	Each Module Touch Screen 지정 ID
	long	nAlarmCount;		//	제어기별 사용 Alarm Count

	long	nRobotArmCount;		//	Module의 Robot Arm Count
	long	nUsedBuff;			//	해당 Module에 Buffer 유무 1 = 있음.
	long	nUsedBuffSlotCount;	//	사용할 Buffer Slot Count;
	char	szModuleName[MAX_LAYER_MODULE_ID_LEN+1];
	char	szModuleDesc[MAX_MODULE_DESCRIPT_LEN+1];

	stUnitCfgType		stUnitCfg[MAX_LAYER2_MODULE_COUNT];

	char szPLCVersion[MAX_PLC_VERSION_LEN+1];		//PLC Versoin 표기

	stECIDTableType	stECIDTable;
	stSVIDTableType	stSVIDTable;
	stDVIDTableType	stDVIDTable;

	//상/하류 Module ID 추가
	stOtherEQPIDType stOtherEQPID[MAX_SUMMARY_DATA_COUNT];
	
	//T8Y Tracking Module Info
	stTrackingInfoType  stTrackingInfo;
};

//	CIM PC 장착 Melsec Board Config
//	Fix Board Channel과 응용 Channel 별도 지정
struct	stMelsecBoardConfigType
{
	long	nInterfaceType;		//	1 = Indexer Interface,
								//	2 = EQ Glass Transfer Handling Interface
								//	3 = EQ Internal Data Handling Interface
	long	nChannelNo;			//	사용 Melsec Board Channel No. ( 51 ~ 54 )
	long	nShareMemChNo;		//	Board 내부 Memory Access용 Channel No (15 : PC Device ... (R - Memory)
};


struct	stEPDConfigType	
{	
	BOOL	bEPDUsed;
	long	nEPDCount;
};


//	Online Parameter 관련 Structure
struct	stEOMDDataType
{
	char	szEOMD[MAX_EOMD_LEN+1]; // EQPID + Unit
	long	nEOV;
};


struct	stEOIDDataType
{
	long nEOID;
	long nEOMDCount;
	stEOMDDataType	stEOMDData[MAX_ONLINE_PARAM_MODE_COUNT]; // Unit
};


struct	stEOIDTableType
{
	long nEOIDCount;
	stEOIDDataType	stEOIDData[MAX_ONLINE_PARAM_COUNT]; // Module
};

struct stSoftTaskRevType
{
	char	szCIMRevCIM[MAX_SOFT_REVISION_LEN+1];	//	CIM Software Version
	char	szCIMRevPLC[MAX_SOFT_REVISION_LEN+1];	//	PLC Software Version
	char	szCIMRevGOT[MAX_SOFT_REVISION_LEN+1];	//	GOT Software Version
};

struct	stLayOutCfgType
{
	long	nEQPType;		//1: Full Inline
									//2: Local Inline
	long	nEQType;			//1:Photo Inline
									//2:Etcher/Stripper Inline
									//3:Etcher
									//4:Stripper
									//5:PFC 세정기
									//6:Edge Scriber (면취세정기)

	long	nEQSubType;			// 각 설비군별에 따른 공정별 분류
										// Photo Inline - 1:GATE, 2:S/D , 3:PASSIVE, 4:C/F-ITO
										// Etcher/Stripper Inline - 1: Mo,  2: Pixel,  3: C/F

	long	nEQNumber;			//호기 번호 - UI용 추가
	long	nProcessDirection; //진행방향  - UI용 추가

	char	szModelName[MAX_MODEL_NUMBER_LEN+1];
	char	szEQPID[MAX_EQ_MODULE_ID_LEN+1];			//	EQPID 으로 사용

	char	szOperatorID[MAX_OPERATOR_ID_LEN+1];	//Operator ID

	long	nModuleCount;		// 전체 Module개수	
	stModuleCfgType 	stModCfg[MAX_LAYER1_MODULE_COUNT];

	long	nMelsecBoardCount;
	stMelsecBoardConfigType	stMelsecConfig[MAX_MELSEC_BOARD_COUNT];

	//	Serial 통신 관련 Config : Rocket Port 통신 Port 관리용
//	long	nSerialCommCount;	// 통신 Port 사용 갯수
//	stSerialCommCfgType		stSerialCommCfg[MAX_SERIAL_COMM_PORT_COUNT];
	stEOIDTableType	stEOIDTable;

	stEPDConfigType	stEPDCfg;

	// TASK별 Soft Version 등록
	stSoftTaskRevType	stSoftTaskRev;

	BOOL	bWaterjetUse;	// GUI에서 처음 시작할 때 사용 유무 체크하여 값을 변경. (2011년 개조)
};

//----------------------------------------------------------------------------------


/*****************************************************************************************************
					           System Data Information 
*****************************************************************************************************/

/*---------------------------------------------
 === Recipe의 구분 ===
 1. Recipe Header 정보는 모든 Recipe에서 동일하게 사용하므로 공통처리 (Recipe 번호, 기록시간, Soft Revision, 사용유무)
 2. Photo Inline, Etcher/Stripper Inline은 2개 설비군만 Flow Recipe Table 유지
 3. Photo Module별 Recipe, Etcher, Stripper Recipe로 나누어 사용
 4. Cleaner Recipe의 공통항목은 Photo Cleaner, PFC Cleaner, Edge Cleaner 모두 동일하게 사용하고
     PFC, EDGE의 개별 항목은 하부 Data로 분리
---------------------------------------------*/

struct	stRecipeDataHeaderType
{
	char	szPPID[MAX_PPID_LEN+1];
	long	nRecipeNo;
	char	szWriteTime[MAX_DATE_TIME_LEN+1];
	long	nSoftRev;
	BOOL	bUsed;	// Recipe 사용여부
};

//	1. Main Recipe Structure
//------------ Etcher/Stripper Inline용 Recipe -------------------
struct	stEtchStripRecipeDataItemType	
{
	char	szPPID[MAX_PPID_LEN+1];
	long	nEtchRecipeNo;					// Etch Recipe No
	long	nStripRecipeNo;					// Strip Recipe No;

	long	nLineTact;	//	Line Tact
};

struct	stMainRecipeEtchStripDataType
{
	stRecipeDataHeaderType		stRecipeHead;
	stEtchStripRecipeDataItemType	stMainRecipe;
};


//------------ PFC Cleaner용 Recipe -------------------
struct	stPFCRecipeDataItemType
{
	char	szPPID[MAX_PPID_LEN+1];
	long	nCLNRecipeNo;	//	Cleaner Recipe No
	long	nLineTact;	//	Line Tact
};

struct	stMainRecipePFCDataType
{
	stRecipeDataHeaderType		stRecipeHead;
	stPFCRecipeDataItemType		stMainRecipe;
};


//------------Edge Cleaner용 Recipe -------------------
struct	stEDGERecipeDataItemType
{
	char	szPPID[MAX_PPID_LEN+1];
	long	nCLNRecipeNo;	//	Cleaner Recipe No
	long	nLineTact;	//	Line Tact
};

//------------ LC Cleaner용 Recipe -------------------
struct	stLCRecipeDataItemType
{
	char	szPPID[MAX_PPID_LEN+1];
	long	nCLNRecipeNo;	//	Cleaner Recipe No
	long	nLineTact;	//	Line Tact
};

struct	stMainRecipeLCDataType
{
	stRecipeDataHeaderType		stRecipeHead;
	stLCRecipeDataItemType		stMainRecipe;
};



struct	stMainRecipeEDGEDataType
{
	stRecipeDataHeaderType		stRecipeHead;
	stEDGERecipeDataItemType	stMainRecipe;
};


struct	stMainRecipeTableType
{
	long	nFlowRecipeCount;
	BOOL	bEditRecipe;

	union stRecipeType
	{
		//1. Etcher / Stripper Flow Recipe
		stMainRecipeEtchStripDataType	stWetRcp;

		//3. PFC Cleaner Flow Recipe
		stMainRecipePFCDataType			stPFCRcp;

		//4. Edge Cleaner Flow Recipe
		stMainRecipeEDGEDataType		stEdgeRcp;

		//5. LC Cleaner Flow Recipe
		stMainRecipeLCDataType			stLCRcp;

	} stRcp[MAX_PPID_COUNT];
};


//	2. Module Recipe Structure
//------------------------------------------------------------------------------------------------------------------
struct	stEtchingDataType
{
	long	nProcessMode;				//처리 모드  0:처리 없음 , 1:Time 처리, 2:EPD처리, 3:추가처리1,  4:추가처리2
	long	nProcessSetTime;			//처리 설정 시간
	long	nProcessAvgTime;		//처리 예상 시간
	long	nEPDUpperError1;	//EPD 상한 불량(경)
	long	nEPDLowerError1;  //EPD 하한 불량(경)
	long	nEPDUpperError2;	//EPD 상한 불량(중)
	long	nEPDLowerError2;  //EPD 하한 불량(중)
	long	nAdditionalEtchPrecent;	//Etching 추가 처리율
	long	nAdditionalEtchTime;		//Etching 추가 처리시간
	long	nAdditionalEPDUpperTime;	//EPD 상한 추가 처리시간
	long	nAdditionalEPDLowerTime;	//EPD 하한 추가 처리시간
};

//Etcher/Stripper Rinse Data Type
struct stWetRinseDataType
{
	BOOL	bSprayUse[MAX_ETCHING_STEP_COUNT];			//Spray 사용/미사용 : Etching 1 ~ Etching 5
	long	nSpraySelect;		//순환수/직수
	BOOL	bKnifeShowerUse;	//입구 Knife Shower 사용/미사용
	BOOL	bInSprayUse;		//반입 Spray 사용/미상요
	BOOL	bOutSprayUse;		//반출 Spray 사용/미사용
	BOOL	bGlassOscUse;		//기판요동 사용/미사용
	BOOL	bNozzleOscUse;		//노즐요동 사용/미사용
	BOOL	bHighPressSpray;	//고압 Spray 사용/미사용
	BOOL	bAAJetUsed;			//AA-JET 사용/미사용
	BOOL	bDirectRinse;		//직수 사용/미사용

	long	nProcSetTime;
	long	nProcSpeed;
	long	nEvenProcess;				// 07-04-01 : Add
};

struct stUVDataType
{
	long	nUVUsed;
	long	nUVProcSetTime;
	long	nUVProcSpeed;  
	long	nUVLampControl;				// 0 bit: UV Lamp1 On/Off
	                                                // 1 bit: UV Lamp2 On/Off
													// 2 bit: UV Lamp3 On/Off
													// 3 bit: UV Lamp4 On/Off
													// 4 bit: UV Lamp5 On/Off
													// 5 bit: UV Lamp6 On/Off
	long	nUVPower;								// UV 조도(1 ~ 100%)
	long	nUVReserved1;
	long	nUVReserved2;
};

struct stERDataType		//LC ODF Cleaner
{
	BOOL	bERRegUsed;									//E/R Regulator 사용/미사용
	long	nERSetValue[MAX_ER_REGULATOR_COUNT];		//E/R Regulator 설정 값 1~10 (0 ~ 1.000)		
};


struct stUsedDataType		//LC PI Cleaner
{
	long	nCB01_CDAFlowUsed;									//0=미사용, 1=사용
	long	nCB02_CDAFlowUsed;									//0=미사용, 1=사용
	long	nTilt_CDAFlowUsed;									//0=미사용, 1=사용
	long	nTurn_CDAFlowUsed;									//0=미사용, 1=사용

};


struct stPlasmaDataType
{
	BOOL	bPlasmaUsed;							// 0:Not Used, 1:Used
	long	nPlasmaSetTime;
	long	nPlasmaSpeed;
	long	nPlasmaPower;							// 1 ~ 100(%)
	long    nPlasmaN2;
	long    nPlasmaCDA;
};

struct stWaterJetDataType
{
	BOOL	bWaterJetUsed;							// 0:Not Used, 1:Used
};

struct	stWetEtchDataType
{
	stEtchingDataType stEtchingStepData[MAX_ETCHING_STEP_COUNT];	//Etching Step 1~5
	stWetRinseDataType stEtchRinseData;								//Etching Spray 처리
};

struct stModuleRecipeEtchType
{
	long	nRecipeValue[MAX_RECIPE_PARAM_COUNT];
	//stWetEtchDataType		stEtchingBathData[MAX_ETCHING_UNIT_COUNT];	//Etching Bath 1~6
	//stWetRinseDataType		stRinsingData[MAX_RINSING_UNIT_COUNT];			//Rinsing
	//stUVDataType			stExcimerUVData;								//Excimer UV 사용시 Data
	//stPlasmaDataType		stPlasmaData;								// Plasma
};

struct	stEtchRecipeDataType
{
	stRecipeDataHeaderType	stRecipeHead;
	stModuleRecipeEtchType	stProcData;
};

struct	stEtchRecipeTableType
{
	long	nRecipeCount;
	BOOL	bEditRecipe;    
	stEtchRecipeDataType	stRecipeData[MAX_MODULE_RECIPE_COUNT];
};

struct	stStrippingDataType
{
	long	nProcessMode;			// 처리 Mode 설정
	long	nProcessSetTime;		// 처리 설정 시간
	long	nProcessAvgTime;		// 처리 예상 시간
	BOOL	bSprayMode;
	BOOL	bKnifeShowerMode;
	BOOL	bInSprayMode;
	BOOL	bOutSprayMode;
	BOOL	bGlassOscMode;
	BOOL	bNozzleOscMode;					
	BOOL	bHighPressSpray;
	BOOL	bAAJetUsed;
	BOOL	bDirectRinse;			// 0: 순환수세, 1: 직수세
};

struct stModuleRecipeStripType
{
	long	nRecipeValue[MAX_RECIPE_PARAM_COUNT];
	//stStrippingDataType	stStrippingData[MAX_STRIPPING_UNIT_COUNT];		//Stripping
	//stWetRinseDataType	stRinsingData[MAX_RINSING_UNIT_COUNT];			//Rinsing
	//stWaterJetDataType		stWaterJetData;	
};

struct	stStripRecipeDataType
{
	stRecipeDataHeaderType	stRecipeHead;
	stModuleRecipeStripType stProcData;
};

struct	stStripRecipeTableType
{
	long	nRecipeCount;
	BOOL	bEditRecipe;    
	stStripRecipeDataType		stRecipeData[MAX_MODULE_RECIPE_COUNT];
};
//------------------------------------------------------------------------------------------------------------------

//	---------------- Cleaner Process Data ------------------
//Brush Control 값을 설정 (Photo Cleaner, PFC Cleaner, EDGE Cleaner)
struct stBrushDataType
{
	long	nBrushSpeed[MAX_BRUSH_COUNT];		//Brush Speed	
	long	nBrushControl;	 					//0 - Not Use ,  1:Use	
												//면취후 세정기는 6bit로 처리할 것
												//0 bit: Brush1 상부
												//1 bit: Brush1 하부
												//2 bit: Brush2 상부
												//3 bit: Brush2 하부
												//4 bit: Brush3 상부
												//5 bit: Brush3 하부
	long	nBrushSpinDir;						//Brush 회전 방향 0:CW , 1:CCW
												//면취후 세정기는 6bit로 처리할 것
												//0 bit: Brush1 상부
												//1 bit: Brush1 하부
												//2 bit: Brush2 상부
												//3 bit: Brush2 하부
												//4 bit: Brush3 상부
												//5 bit: Brush3 하부
	long	nBrushUpPress[MAX_BRUSH_COUNT];		//Brush Upper Press Gap 0~3 (-4.0 ~ 4.0 mm)		
	long	nBrushLowPress[MAX_BRUSH_COUNT];	//Brush Lower Press Gap 0~3 (-4.0 ~ 4.0 mm)	
};

struct stPFCCleanerRecipeType
{
	long	nRecipeValue[MAX_RECIPE_PARAM_COUNT];
// 	long	nRecipeNo;
// 	long	nLineTact;
// 	long	nProcSpeed;
// 	long	nGlassSize;
// 	long	nGlassLength;
// 	long	nGlassWidth;
// 	long	nProcSetTime;	//처리 설정 시간
// 	long	nProcAvgTime;	//처리 예상 시간
// 
// 	long	nThroughMode;							// 0: Normal,		1:Through
// 	long	nChemicalMode;							// 0: DIW, 1: Chemical
// 
// 
// 	stBrushDataType		stRBrushData;				// Roll Brush
// 	stBrushDataType		stDBrushData;				// Disk Brush
// 	stBrushDataType		stEBrushData;				// Edge Brush
// 	stWetRinseDataType	stRinse1Data;				// Rinse1
// 	stWetRinseDataType	stRinse2Data;				// Rinse2
// 	stWetRinseDataType	stRinse3Data;				// Rinse3
// 	stWetRinseDataType	stRBRinseData;				// R/B Rinse
// 	stWetRinseDataType	stHFRinseData;				// HF 
// 	stPlasmaDataType	stPlasmaData;				// Plasma
// 	stUVDataType		stUVData;					// UV
// 	stWaterJetDataType	stWaterJetData;				// Waterjet (2011 개조)
};

struct stEdgeCleanerRecipeType
{
	long	nRecipeNo;
	long	nLineTact;
	long	nProcSpeed;
	long	nGlassSize;
	long	nGlassLength;
	long	nGlassWidth;
	long	nProcSetTime;	//처리 설정 시간
	long	nProcAvgTime;	//처리 예상 시간

	long	nThroughMode;							// 0: Normal,		1:Through
	long	nChemicalMode;							// 0: DIW, 1: Chemical

	stBrushDataType		stRBrushData;				// Roll Brush
};


struct stLCCleanerRecipeType		
{
	long	nRecipeValue[MAX_RECIPE_PARAM_COUNT];
// 	long	nRecipeNo;
// 	long	nLineTact;
// 	long	nProcSpeed;
// 	long	nGlassSize;
// 	long	nGlassLength;
// 	long	nGlassWidth;
// 	long	nProcSetTime;	//처리 설정 시간
// 	long	nProcAvgTime;	//처리 예상 시간
// 	
// 	long	nThroughMode;							// 0: Normal,		1:Through
// 	long	nChemicalSelectMode;					// 0: ?, 1: ?, 2:?						LC PI Cleaner
// 	long	nChemicalTankMode;						// 0: 약액탱크, 1: Rinse Tank			LC PI Cleaner
// 	long	nTankTempSetting;						// LC PI, LC PI Rework 추가
// 	long	nTurnStageDir;							// 1:0도, 2:90도, 3:180도, 4:-90도		LC PI Cleaner	
// 	
// 	stBrushDataType		stRBrushData;				// Roll Brush
// 	stWetRinseDataType	stCBData;					// CB	//LC REWORK Cleaner
// 	stWetRinseDataType	stIPAData;					// IPA	//LCODF Cleaner
// 	stWetRinseDataType	stRinse1Data;				// Rinse1
// 	stWetRinseDataType	stRinse2Data;				// Rinse2
// 	stWetRinseDataType	stRinse3Data;				// Rinse3
// 	stWetRinseDataType	stRBRinseData;				// R/B Rinse
// 
// 	stPlasmaDataType	stPlasma1Data;				// LC PI Cleaner
// 	stPlasmaDataType	stPlasma2Data;				// LC PI Cleaner
// 
// 	stUVDataType		stUVData;					// LC PI Cleaner
// 	stERDataType		stERData;					// LCODF Cleaner
// 
// 	stUsedDataType		stUsedData;					// LC PI Used Data
	
};


struct	stCleanerRecipeDataType
{
	stRecipeDataHeaderType		stRecipeHead;
	stPFCCleanerRecipeType		stPFCProcData;
	stEdgeCleanerRecipeType		stEdgProcData;
	stLCCleanerRecipeType		stLCProcData;
};

struct	stCleanerRecipeTableType
{
	long	nRecipeCount;
	BOOL	bEditRecipe;
	stCleanerRecipeDataType	stRecipeData[MAX_MODULE_RECIPE_COUNT];
};
//------------------------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------------------------------------
struct	stAlarmRecordType
{
	long nALCD;
	long nALID;
	char szALTX[MAX_ALARM_TEXT_LEN+1];
	char szALTM[MAX_DATE_TIME_LEN+1];
	long nModuleID;			// 제어기 Module Type
	long nUnitID;		// 각 Module별로 position
	char szUnitID[MAX_UNIT_ID_LEN+1];	//Unit ID

	//T8 추가 Alarm Item
	long	nFault;			//Fault/Warning 구분
	long	nAutoReset;		//AutoReset 가능 여부
	long	nRemoteClear;	//Remote Clear 가능 여부
	long	nLoadingStop; 
	long	nTransferStop;
	long	nAlarmUsed;		//Alarm 사용 유무
};

//	6. Alarm Data Define [6/25/2003]
struct	stAlarmDBTableType
{
	long nRecordCount;
	stAlarmRecordType		stAlarmRecord[MAX_ALARM_COUNT_IN_MODULE];
};

struct	stECIDDataType	
{
	long	nECDefault;
	long	nECStopLowLimit;
	long	nECStopUpLimit;
	long	nECWarnLowLimit;
	long	nECWarnUpLimit;
};

struct	stTempDataType	
{
	float	fTempDefault;
	float	fTempStopLowLimit;
	float	fTempStopUpLimit;
	float	fTempWarnLowLimit;
	float	fTempWarnUpLimit;
};

struct	stTankDataType	
{
	long	nTKLifeTime;		// Chemical Change Count
	long	nTKLifeCount;		// Chemical Change Time
	long	nDrainTime;			// Drain Time
	long	nActCount;			// Actual Process Glass Count
	long	nMode;				// 0: None, 1: Count, 2: Time, 3: Count/Time
	long	nPattern;			// 0: None, 1: Mix (Etch) / Cascade (Strip), 2: CCSS
};

struct	stECIDSingleDataType
{
	long	nECDefault;
};

struct	stSetProcDataInfoType				// Process Module Data
{
	stECIDDataType	stECIDData[MAX_ECID_MULTI_COUNT];
	stECIDSingleDataType	stECIDData_Single[MAX_ECID_SINGLE_COUNT];
};

struct	stUnitDataInfoType
{
	stEventCtrlDataType		stEvtData;
};


// Recipe Parameter Information 설정
struct  stRecipeParamDataType
{
	BOOL bUsed;
	BOOL bRangeCheckUsed;
	
	long nParamIndex;
	long nModuleNo;
	long nMapIndex;
	long nDecimalPoint;
	
	char szParamName[MAX_PROCESS_PARAM_NAME_LEN + 1];
	char szRangeMin[MAX_RANGE_LEN + 1];
	char szRangeMax[MAX_RANGE_LEN + 1];
	char szSymbol[MAX_SYMBOL_LEN + 1];
};

struct stRecipeParamTableType 
{
	long nParamCount;
	long nUsedParamCount;
	
	stRecipeParamDataType stRecipeParam[MAX_RECIPE_PARAM_COUNT];
};


struct	stModuleDataInfoType
{
	long	nModuleID;	//	[20060112]

	//	1. Event Control Data 설정
	stEventCtrlDataType		stEvtData;

	//	2. Equipment Alarm Data 설정
	stAlarmDBTableType		stAlarmDB;

	//	4. Process 관련 Data 설정 및 Monitoring
	stSetProcDataInfoType	stProcData;
	
	//	5.	Unit관련 Data 설정
	stUnitDataInfoType		stUnitData[MAX_UNIT_COUNT];
};

struct	stSystemDataInfoType
{
	// Event Control Data 설정 On, Off
	BOOL	bEventControlOnOff; // add

	// 2. Event Control Data 설정(EQ Level) : GUI에서 설정한다.	
	stEventCtrlDataType		stEvtData;
	
	// 3. Recipe Table - Inline용 Flow Recipe Table
	stMainRecipeTableType	stMainRecipeTbl;

	//	4. Module Levle Data 설정
	stModuleDataInfoType	stMODDataInfo[MAX_LAYER1_MODULE_COUNT];

	//	5. Module Recipe Data 설정
	stCleanerRecipeTableType	stRecipeTblCLN;		//Photo Cleaner, PFC Cleaner, Edge Cleaner 공통 사용


	// Etcher/Strip 용 Recipe Data
	stStripRecipeTableType		stRecipeTblStrip;
	stEtchRecipeTableType		stRecipeTblEtch;

	stRecipeParamTableType		stRecipeParamTbl[MAX_LAYER1_MODULE_COUNT];
};

//-------------------------------------------------------------------------------

/*****************************************************************************************************
												System Running Information 
*****************************************************************************************************/

struct stTowerLampControlType
{
	BOOL	bControlMode;
	BOOL	bRed;
	BOOL	bYellow;
	BOOL	bGreen;
	BOOL	bBuzzer;
};

// Module 단위 Run Data ContactPoint Info
struct	stEQStateBitsignalType
{
	long	nEQState_Normal;
	long	nEQState_Fault;
	long	nEQState_PM;
	long	nEQState_Reserved[5];

	//	Process State
	long	nProcState_Init;
	long	nProcState_Idle;
	long	nProcState_Setup;
	long	nProcState_Ready;
	long	nProcState_Execute;
	long	nProcState_Pause;
	long	nProcState_Reserved[2];

	long	nERC_Mode_Auto;		// Equipment Recipe Change Mode Auto
	long	nERC_Mode_Man;		// Equipment Recipe Change Mode Manual
	long	nGMC_Mode_Auto;		// Glass Mechanical condition Change Mode Auto
	long	nGMC_Mode_Man;		// Glass Mechanical condition Change Mode Manual
	long	nOther_Reserved[12];
};

struct	stPreViewRunDataType
{
	char	szPPID[MAX_PPID_LEN+1];
	long	nGlassSize[2];		
	long	nGlassThickness;
	long	nReserve1;
	long	nSetTactTime;
	long	nCurTactTime;
	long	nReqGlassType;
	long	nReqGlassCount;

	char	szEOMode[2+1];				//	Equipment Operation Mode
	char	szOPMode[2+1];				//	Equipment Operation Mode

	stEQStateBitsignalType	stEQState;
};

struct	stPreViewRunGlassSummaryDataType
{
	char	szProcessID[MAX_PROCESS_ID_LEN+1];
	char	szProductID[MAX_PROCESS_ID_LEN+1];
	char	szBatchID[MAX_BATCH_ID_LEN+1];
	char	szStepID[MAX_STEP_ID_LEN+1];
	long	nGlassCount;
	long	nReserved;
};

struct	stPreViewRunGlassSummaryType
{
	long	nTotalGlassCount;	//	Count of product glass which is located in equipment
	long	nTFTCount;			//	Count of TFT product glass which is located in equipment
	long	nCFCount;			//	Count of CF  product glass which is located in equipment
	long	nPMDTCount;			//	Count of NON-product glass which is located in equipment
	long	nIdleCount;			//	Count of product glass which is before process
	long	nBusyCount;			//	Count of product glass which is before process
	long	nFinishCount;		//	Count of product glass which is before process
	long	nBFCount;			//	Count of product glass which is before process
	long	nReserved[6];		//	Count of product glass which is before process
	
	stPreViewRunGlassSummaryDataType	stSummaryData[MAX_SUMMARY_DATA_COUNT];
};

//	하기 제어기 조작 Switch 상태 정보 표시
struct	stOPModeIOInfoType
{
	BOOL	bAlive;
	BOOL	bInLine;		// 0: OffLine,	1: Inline(Machine	Mode)
	BOOL	bManual;		// 0: Manual,	1: Auto
	BOOL	bStandBy;		// 0: Stanbying,1: StandBy
	BOOL	bStart;			// 0: None,		1: Start
	BOOL	bStop;			// 0: None,		1: Stop
	BOOL	bCycleStop;		// 0: None,		1: CycleStop(Pause)
	BOOL	bPM;			// 0: Normal,	1: PM
	BOOL	bBuzzer;		// 0: Buzz Off,	1: Buzz On
	BOOL	bDataEditing;	// 0: None,		1: Editing...

	BOOL	bNetworkError[6];	// 배열 [0] : 상류, 배열 [1] : 하류 
								// 0: 통신 에러 해제, 1: 통신 에러 발생
	
	BOOL	bSleepModeReady;	//On - Sleep Mode 진입 可, Off - Sleep Mode 진입 不可

};

//	하위 제어기 동작 State 정보 표시
struct	stOPStateIOInfoType
{
	BOOL	bWarnAlarm;									// 0: None	,	1: Alarmed
	BOOL	bHeavyAlarm;								// 0: None	,	1: Alarmed
	BOOL	bChemChgWarning;							// 0: None	,	1: Chemical Change 대기중
	BOOL	bChemChanging;								// 0: None	,	1: Chemical Changing....
	BOOL	bChemicalSupply[MAX_TANK_TOTAL_COUNT-1];	// 0: None	,	1: 액 보충 상태.(-1 : 액교환)
														// Etcher : [0~1] : A/B, Stripper : [0~3] : HP01/HP02/SS02/조합조

//	BOOL	bMTBFStart;			//	중알람 이상 발생으로 설비 가동이 불가능할 때 ON : CIM에서 Log 관리용
};

//	하위 제어기의 Event 보고 IO
struct	stSEMESEQCmdRly_EvtRptIOType
{
	//	EQ Cmd Reply
	BOOL	bMachineCmdReply;
	BOOL	bAlarmClearReply;
	BOOL	bTimeSetReply;
	BOOL	bTerminalMsgReply;
	BOOL	bBuzzerStopReply;
	BOOL	bReserve1Reply;
	BOOL	bReserve2Reply;
	BOOL	bReserve3Reply;

	BOOL	bOperatorCallReply;

	//	EQ Event Report
	BOOL	bAlarmOccured;
	BOOL	bAlarmTreated;
	BOOL	bGlassScrap;
	BOOL	bGlassUnscrap;
	BOOL	bGlassJudgement;
	BOOL	bRecipeDownReq;
	BOOL	bManualCellLoad;
	BOOL	bBackModeStart;
	BOOL	bGlassSendFail; // 2010-08-24 add.
	

	// H/S Status Report
	BOOL	bHS_NR_TimeOver;
	BOOL	bHS_BC_TimeOver;
};

//	하위 제어기의 응답(Reply) IO
struct	stSEMESEQDataRly_EvtRptIOType
{
	BOOL	bECIDRly;
 	BOOL	bTemperatureRly;
	BOOL	bECORly;
	BOOL	bMNCellReadRly;
	BOOL	bMNValidChkReq;
	BOOL	bMNCancelReq;
	BOOL	bVCRReadFail;
	BOOL	bMNCellJudgeRly;
	BOOL	bGlassSendFailRly; // 2010-08-24 add.

	BOOL	bECIDEvtReq;	
	BOOL	bTemperatureEvtReq;
	BOOL	bECOEvtReq;
	BOOL	bPPIDValidationReq;
	BOOL	bProcessEndEvtReq;
	BOOL	bParam6EvtReq;
	BOOL	bParam7EvtReq;
	BOOL	bParam8EvtReq;
};

//	Conveyer/Robot ContactPoint IO
struct	stContactPointIOType
{
	BOOL	bAbnormal;
	BOOL	bTypeofArm;
	BOOL	bTypeofStage;
	BOOL	bManualOp;
	BOOL	bSafety;
	BOOL	bEmpty;
	BOOL	bWait;
	BOOL	bBusy;
	BOOL	bPause;
	BOOL	bReserved9;
	BOOL	bArm1Violate;		//	On - Violate
	BOOL	bArm2Violate;		//	On - Violate
	BOOL	bArm1FoldComplete;	//	On - Folded
	BOOL	bArm2FoldComplete;	//	On - Folded
	BOOL	bArm1GlassCheck;	//	On - Glass Detected
	BOOL	bArm2GlassCheck;	//	On - Glass Detected
	BOOL	bRobotDirection;	//	On - See Opponent
	BOOL	bReserved17;
	BOOL	bReserved18;
	BOOL	bReserved19;
	BOOL	bLiftUp;			//	= Pin Up	
	BOOL	bLiftDown;			//	= Pin Down
	BOOL	bStopperUp;			// = Centering Close
	BOOL	bStopperDown;		//	= Centering Open
	BOOL	bDoorOpen;
	BOOL	bDoorClose;
	BOOL	bGlassDetect;
	BOOL	bBodyMoving;
	BOOL	bBodyOP;
	BOOL	bReserved29;
	BOOL	bReserved30;
	BOOL	bReserved31;
};

//	Glass Send Handshake I/O ( To Lower H/S )
struct	stGlassSendIOType
{
	BOOL	bHeartBeat;
	BOOL	bPause;
	BOOL	bAbnormal;
	BOOL	bReserved3;
	BOOL	bSendAble;
	BOOL	bSendStart;
	BOOL	bSendComplete;
	BOOL	bImmPauseReq;
	BOOL	bReturnRecvStart;
	BOOL	bReturnRecvComplete;
	BOOL	bExchangeFlag;
	BOOL	bMultiCarryFlag;
	BOOL	bReserved12;
	BOOL	bReserved13;
	BOOL	bReserved14;
	BOOL	bReserved15;
	BOOL	bPreAction1;
	BOOL	bPreAction2;
	BOOL	bMidAction1;
	BOOL	bMidAction2;
	BOOL	bPostAction1;
	BOOL	bReserved21;
	BOOL	bReserved22;
	BOOL	bReserved23;
	BOOL	bReserved24;
	BOOL	bReserved25;
	BOOL	bHSResumeReq;
	BOOL	bHSRecoveryAck;
	BOOL	bHSRecoveryNak;
	BOOL	bReserved29;
	BOOL	bHSInitial;
	BOOL	bHSError;
};

//	Glass Receive Handshake I/O ( To Upper H/S )
struct	stGlassRecvIOType
{
	BOOL	bHeartBeat;
	BOOL	bPause;
	BOOL	bAbnormal;
	BOOL	bReserved3;
	BOOL	bRecvAble;
	BOOL	bRecvStart;
	BOOL	bRecvComplete;
	BOOL	bImmPauseReq;
	BOOL	bReturnSendStart;
	BOOL	bReturnSendComplete;
	BOOL	bExchangeFlag;
	BOOL	bMultiCarryFlag;
	BOOL	bReserved12;
	BOOL	bReserved13;
	BOOL	bLoadingStop;
	BOOL	bTransferStop;
	BOOL	bPreAction1;
	BOOL	bPreAction2;
	BOOL	bMidAction1;
	BOOL	bMidAction2;
	BOOL	bPostAction1;
	BOOL	bReserved21;
	BOOL	bReceiveRefuse;
	BOOL	bReserved23;
	BOOL	bReserved24;
	BOOL	bReserved25;
	BOOL	bHSResumeReq;
	BOOL	bHSRecoveryAck;
	BOOL	bHSRecoveryNak;
	BOOL	bReserved29;
	BOOL	bHSInitial;
	BOOL	bHSError;	
};

/*-------------------------
 	Panel Data 
------------------------- */

//Pair Property
struct  stPairPanelInfoType
{
	char	szPairHPanelID[MAX_PANEL_ID_LEN+1];
	char	szPairEPanelID[MAX_PANEL_ID_LEN+1];
	char	szPairProductID[MAX_PRODUCT_ID_LEN+1];			//T8 추가
	char	szPairGrade[MAX_CELL_GRADE_LEN+1];				//T8 ASCII Length 변경
};

//Cell Property
struct	stCellPanelInfoType
{	
	char	szSubPanelID[MAX_PANEL_ID_LEN+1];
	char	szSubJudgement[MAX_JUDGEMENT_RESULT_LEN+1];
	char	szSubCode[MAX_JUDGEMENT_CODE_LEN+1];
};

//SCH 사용 추가 Glass Data
struct  stExtraPanelInfoType
{
		long	nCurPosId;		
		long	nJobOrder;		//	PIO Bit Signal에 반영되어야함.	노광기 Interface에 활용예정
								//	1-Batch(Cassette) Start, 2-Batch(Cassette) End
		long	nGlassType;		//	PIO Bit Signal에 반영되어야함.
								//	4-Normal, 5-Start, 6-End
};

//	Bit Signal ( Total Item : 32 )
struct  stGlassBitSignalType
{
	long	nWorkSkipBit;	//	Work Skip
	long	nBatchStartBit;	//	Job End
	long	nBatchEndBit;	//	Batch End
	long	nJobStartBit;	//	Job Start
	long	nJobEndBit;		//	Job End
	long	nHotFlowFlag;	
	long	nPhysicalFlag;	
	long	nMStartFlag;
	long	nAlignFlag;
	long	nReserved[23];	//	Reserved 
};

//Original Spec Glass Data	- FIC SPEC 반영
struct	stPanelInfoType
{
	char	szHPanelID[MAX_PANEL_ID_LEN+1];
	char	szEPanelID[MAX_PANEL_ID_LEN+1];
	char	szSlotNo[MAX_SLOT_NUMBER_LEN+1];
	char	szProcessID[MAX_PROCESS_ID_LEN+1];
	char	szProductID[MAX_PRODUCT_ID_LEN+1];	
	char	szStepID[MAX_STEP_ID_LEN+1];	
	char	szBatchID[MAX_BATCH_ID_LEN+1];
	char	szProdType[MAX_PRODUCT_TYPE_LEN+1];
	char	szProdKind[MAX_PRODUCT_KIND_LEN+1];
	char	szPPID[MAX_PPID_LEN+1];	
	char	szFlowID[MAX_FLOW_ID_LEN+1];
	short	shFlowGroup[MAX_FLOW_GROUP_LEN/2];
	short	shUsableChamber[MAX_USABLE_CHAMBER_LEN/2];
	long	nPanelSize[2];
	long	nThickness;	
	long	nCompCount;
	char	szGrade[MAX_CELL_GRADE_LEN+1];
	char	szJudgement[MAX_JUDGEMENT_RESULT_LEN+1];
	char	szCode[MAX_JUDGEMENT_CODE_LEN+1];
	char	szCount1[MAX_GLASS_COUNT_LEN+1];
	char	szCount2[MAX_GLASS_COUNT_LEN+1];
	char	szPanelPosition[MAX_GLASS_POSITION_LEN+1];	
	long	nFlowHistory[MAX_FLOW_HISTORY_LEN];
	long	nUniqueID[MAX_UNIQUE_ID_LEN];
	char	szReadingFlag[MAX_READING_FLAG_LEN+1];
	char	szMultiUse[MAX_MULTI_USE_LEN+1];

	stGlassBitSignalType	stBitSignal;
	stPairPanelInfoType		stPairPanelInfo;
	stCellPanelInfoType		stCellPanelInfo;

	short	shReferData[2];

	long	nOwnGlassNo;

	long	nPanelState;
	short	shReserved[16];
	
}; 

//	2005.08.15 
struct	stHandshakeToLowerType
{
	stGlassSendIOType		stSendIO;
	stContactPointIOType	stSendContactPoint;	//	Conveyer/Robot ContactPoint IO
	stGlassRecvIOType		stRecvIO;
	stContactPointIOType	stRecvContactPoint;	//	Conveyer/Robot ContactPoint IO

};

struct	stHandshakeToUpperType
{
	stGlassSendIOType		stSendIO;
	stContactPointIOType	stSendContactPoint;	//	Conveyer/Robot ContactPoint IO
	stGlassRecvIOType		stRecvIO;	
	stContactPointIOType	stRecvContactPoint;	//	Conveyer/Robot ContactPoint IO
};

//	20050816
struct	stContactPointToUpperType
{
	//	H/S Inform
	char	szPostActData[MAX_POST_ACT_DATA_LEN+1];
	char	szRefuseCode[MAX_REFUSE_CODE_LEN+1];	//	Refuse Code
	long	nReserved1[2];

	//	Contact State
	long	nCSIF;
	long	nReserved2[2];
};

//	20060131
struct	stSecondReplyDataType
{
	BOOL	bProcAvailCheck;
	long	nProcAvailCheck;

	BOOL	bProcCondChange;
	long	nProcCondChange;

	BOOL	bCollectMode;
	long	nCollectMode;
};

//	20050819
struct	stMasterReplyDataType
{
	long	nReplyData;
};

struct	stCurTempMeasDataType
{
	float	fSettemp;
	float	fCurtemp;
	float	fCtrlout;		// control output (0 - 100%)
	BOOL	bULmtalarm;		// upper limit alarm
	BOOL	bLLmtalarm;		// lower limit alarm	
};

struct	stCurTankDataType
{
	long	nLifeTime;		// Chemical Change Time
	long	nLifeCount;		// Chemical Change Count
	long	nSupplyTime;	// 약액 보충 시간
	long	nSupplyCount;	// 약액 보충 Count
	long	nUsedTankNo;	// 사용 Tank No
	long	nActCount;		// Actual Process Glass Count
	long	nReserved1;		// 
	long	nReserved2;		// 
};

/*
struct stCurEpdDataType
{
	long	nCurEpdValue;
	char	szPanelID[MAX_PANEL_ID_LEN+1];
};
*/


struct stCurEpdDataType
{
	long	nCurEpdValue;
	long	nCurEpdTime[4];                     // EPD Event Time ohanaya 2011.06
	BOOL	bGlassSet;							// Glass Set
	char	szPanelID[MAX_PANEL_ID_LEN+1];
};

struct	stCurProcessingDataType
{
	long nUsedTankNo;		// 현재 사용중인 Tank No : 다수 Tank를 사용하지만, 선택적으로 사용하는 경우에 사용
	
	long	nCurSVIDData[MAX_SVID_COUNT];	//전설비공통
	stCurTempMeasDataType	stCurTemperature[MAX_TEMP_CTRL_COUNT];	//전설비공통
	stCurTankDataType		stCurTankData[MAX_MODULE_TANK_COUNT];	//전설비공통
	stCurEpdDataType		stCurEpdData[MAX_EPD_CTRL_COUNT];		//Etcher용
};

struct stUnitTactDataType
{
	char    szSet_UnitName[MAX_TACT_UNIT_LEN+1];
	char	szMes_UnitName[MAX_TACT_UNIT_LEN+1];
	long	nSetTactTime;
	long	nMesTactTime;
};

struct	stBakeProcEndDataType		
{
//	stBakeProcEndDataItemType  stPlateEndData[MAX_PLATE_COUNT];
	long	nProcMode;		//	Process Skip 여부 설정					
	long	nProcPlateNo;	//	Bake Plate No ( HP1, HP2, CP1, CP2, AP1, AP2 etc...)			
	long	nProcSetTime;	//	Process 설정 시간			
	long	nProcTime;		//	Process 실측 시간
	long	nProcSetTemp;	//	Process 설정 온도
	long	nProcTemp;		//	Process 실측 온도
};


// 20051214
struct stConvProcEndDataType
{
	long	nDVIDMaxValue[MAX_DATA_COLLECT_ITEM_COUNT];
// 	long	nSVIDMaxValue[MAX_SVID_COUNT];
// 	long	nTempValue[MAX_TEMP_CTRL_COUNT];
// 	long	nEpdValue[MAX_EPD_CTRL_COUNT];			//Etcher용
// 
// 	// shmagic 2011.06.20 접액시간 추가.
// 	long	nContactTime[MAX_CONTTACT_TIME_COUNT];	//Etcher용
// 
// 	stUnitTactDataType	stUnitTactData[MAX_LAYER2_MODULE_COUNT];	// Unit Tact Time
};

struct	stProcessEndDataType
{
	long	nModuleID;
	long	nUniqueID[MAX_UNIQUE_ID_LEN];
	long	nTotalProcTime;				//	설비 In -> Out 시간
	long	nProcessTime[MAX_ETCHING_UNIT_COUNT];				//	Main Process Module의 처리시간
	long	nProcGlassCount;			//	누적 처리 매수
	long	nProcRecipeNo;
	long	nUsedTankNo;
	long	nTankUsedTime;
	long	nUVUsedTime[6];
	long	nOwnGlassNo;

	// for 면취후 세정기
	char	szPanelID[MAX_PANEL_ID_LEN+1];
	long	nUpperTactTime;
	long	nLowerTactTime;

	stBakeProcEndDataType stBakeProcEndData[MAX_PLATE_TYPE_COUNT];	//0-HP, 1-AP, 2-CP
	stConvProcEndDataType stConvProcEndData;
};

// Up/Down Lift의 Position 표기 (PFC 세정기, Etcher 사용)
struct stUpDownLiftPosType 
{	
	BOOL bUpPosition;	//상
	BOOL bMidPosition;	//중	
	BOOL bDownPosition;	//하
};

/*
 *	Unit별 Glass Tracking 정보를 저장
 */
struct stUnitRunInfoType
{
	char	szUnitName[MAX_LAYER_MODULE_ID_LEN+1];
	
	long	nEQState;			//	Current EQ State	: 1 - Normal, 2 - FAULT, 3 - PM
	long	nProcState;		//	Current Proc State	: 1 - Init, 2 - Idle, 3 - Setup, 4 - Ready, 5 - Exec, 6 - Pause
	long	nUniqueID[MAX_UNIQUE_ID_LEN];
	char	szJudgement[MAX_JUDGEMENT_RESULT_LEN+1];
	long	nGlassState;
	BOOL	bGlassSet;		//Current Glass Yes/No
	long	nRecipeNo;		//사용 Recipe번호 추가
	long	nOwnGlassNo;
	
	stPanelInfoType		stPanelData;

	stUpDownLiftPosType stUDLiftData;	// Up/Down Lift Glass Set 표기용  0: 첫번째, 1: 두번째, 2: 세번째
};

//하위 제어기로 부터 진행 매수를 받는다.
struct  stRunGlassCountType
{
	long nRunGlassCount;		// 현재 Module내의 진행중인 Glass 매수
	long nDayGlassCount;		// Day Glass 매수
	long nTotalGlassCount;	// Total Glass 매수
};

struct stEQInterlockListType  
{
	long nItemNo;
	char szItemName[MAX_EQ_INTERLOCK_ITEM_NAME_LEN+1];		//ASCII[40]
	char szItemValue[MAX_EQ_INTERLOCK_ITEM_VALUE_LEN+1];	//ASCII[40]
	char szRelatedModuleID[MAX_EQ_INTERLOCK_RELATED_MODULEID+1];	//ASCII[40]
	BOOL bUsed;
};

struct stEQInterlockTableType
{
	long nModuleNo;
	long nInterlockCount;
	stEQInterlockListType stEQInterlock[3][MAX_EQ_CURRENT_INTERLOCK_COUNT];
};

struct stModuleRunDataType
{
	long	nRuningTime;
	long	nErrorCount;
	long	nErrorTime;
	long	nMTBF;
	long	nMTTR;
	long	nRunningRate;
	
	long	nTotalProductCnt;
	long	nRWGlassCnt;
};

// 2010-08-24 add
struct stGlassSendFailType
{
	long	nModuleID;
	long	nGlassSendFailCode; // 1.Duplication, 2.Omission, 3.Availability
	char	szH_PanelID[MAX_PANEL_ID_LEN+1];
};

/*
*	Module별 Running Information
*/

struct stProcEndData_Inline_ET
{
	BOOL	bUsed;
	long	nOwnGlassNo;
	char	szHPanelID[MAX_PANEL_ID_LEN+1];
	stProcessEndDataType	stProcEndData;
};


struct stECO_EOMDDataType
{
	long	nECO_EOV;
};

struct stECOModeType
{
	stECO_EOMDDataType	stECO_EOMD[MAX_ECO_EOMD_COUNT];
};


struct	stModuleRunInfoType
{
	long	nModuleID;						//	Interlock Check용	

	stEventCtrlDataType	 stModEvtCtrlData;		//0:Disable, 1:Enable

	//////////////////////////////////////////
	//	Inline Net Used Item
	//	Inline Etcher 기준 : 상공 / 하부 PIO 분리 운영의 경우
	stHandshakeToUpperType	stToUpperIO[3];	//	0 - Normal, 1 - Option(상공), 3 - Reserved 
	stHandshakeToLowerType	stToLowerIO[3];	//	0 - Normal, 1 - Option(상공), 3 - Reserved 
	
	//	20050816
	stContactPointToUpperType	stToUpperData[3];	//	0 - Normal, 1 - Option(상공), 3 - Reserved 
	stPanelInfoType				stInGlassData[3];		//	0 - Normal, 1 - Option(상공), 3 - Reserved 
	stPanelInfoType				stOutGlassData[3];		//	0 - Normal, 1 - Option(상공), 3 - Reserved 

	long	nEQState;			//	Equipment State	: 1 - Normal, 2 - FAULT, 3 - PM
	long	nProcState;			//	Process State	: 1 - Init, 2 - Idle, 3 - Setup, 4 - Ready, 5 - Execute, 6 - Pause
											
	long	nBypassEQState;		// * Etcher의 상공 
	long	nBypassProcState;

	long	nBufferEQState;		// * Buffer
	long	nBufferProcState;

	long	nUsingTankNo;		//KWY
	
	stRunGlassCountType   stRunGlass[3];	//Module의 Glass 매수 정보를 얻는다. * Etcher의 상공 처럼 분리하여 관리되는 Unit을 위해 배열 사용

	stPreViewRunDataType			stPreViewRunData;
	stPreViewRunGlassSummaryType	stPreViewGlassSummaryData;

	//	20050817
	stSecondReplyDataType			stSecondReply;

	//	Data Net Used Item
	stOPModeIOInfoType				stOPModeIO;				//	하위 제어기의 조작 Switch 상태 정보

	//	20050819
	stOPStateIOInfoType				stOPStateIO;			//	하위 제어기의 동작 상태 정보
	stSEMESEQCmdRly_EvtRptIOType	stEQCmdRlyEvtIO;		//	하위 제어기의 물류관련 Reply & Event IO
	stSEMESEQDataRly_EvtRptIOType	stDataChangeRlyEvtIO;	//	하위 제어기의 Data 변경관련 Reply & Event IO

	bool	bAlarmed[MAX_ALARM_COUNT_IN_MODULE];

	//Unit단위 Glass Tracking Data
	stUnitRunInfoType		stRobotArmInfo[MAX_ROBOT_ARM_COUNT];	//Robot Arm
	stUnitRunInfoType		stUnitInfo[MAX_LAYER2_MODULE_COUNT];	//Conveyor
	stUnitRunInfoType		stBuffInfo[MAX_BUFFER_SLOT_COUNT];		//Buffer

	//	20050819
	//	각 Local의 Event Report 및 Master Request에 Reply Data를 SMA로 Update토록함.
	stMasterReplyDataType	stMasterReplyData;
	stPanelInfoType			stGlassJudgeScrapData;
	stPanelInfoType			stGlassShiftData;

	long					nUnscrapUniqueID[MAX_UNIQUE_ID_LEN];
	char					szPPIDTransferData[MAX_PPID_LEN+1];	//	Recipe Download 요구용
	long					nOccuredAlarmID;
	long					nTreatedAlarmID;

	long					nCurAlarmState[32];	//	Alarm Word(short) 64 -> long 32로 

	stCurProcessingDataType	stCurProcData;
	stProcessEndDataType	stProcEndData;

	// 07-02-14 : Add
	char					szPauseCode[MAX_PAUSE_CODE_LEN+1];
	char					szPMCode[MAX_PM_CODE_LEN+1];

	//EQ Interlock List
	stEQInterlockTableType	stEQInterlockTable;

	stGlassSendFailType		stGlassSendFail;

	stECOModeType			stECOMode;

	stModuleRunDataType		stModuleRunData;
};

struct	stTraceRecordType
{
	long	nTRID;
	long	nSampleNo;
	long	nTimeInterval;
	long	nMeasureTime;
	long	nRepGroupSize;	//mschoi
	long	nSVIDCount;
	long	nSVID[MAX_SVID_COUNT];

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1];
};

struct	stTraceTableType
{
	long	nTraceCount;
	stTraceRecordType	stTraceData[MAX_SVID_COUNT];
};

struct	stWaitAlarmTableType
{
	long				nAlarmCount;
	stAlarmRecordType	stWaitAlarm[MAX_WAIT_ALARM_COUNT];
};

//	Temperature Monitoring Data Type
struct stCurTempDataFromCIMType
{
	long	nUpdateSeqNo;					//	온도 Data 변경 유무 확인용.
	float	fTemperature[MAX_NODE_COUNT];
};

//	Temperature Monitoring Data Table Type
//	통신 Port 사용 개수 만큼 확장되어야 함.
struct stTempMonitoringDataTableType
{
	stCurTempDataFromCIMType	stData[MAX_SERIAL_COMM_PORT_COUNT];
};

// Unique ID Table 유지 - Unit내의 Run Glass Data 체크용
struct stSMAUniqueIDTableType
{
	long nOwnGlassNo;
	stPanelInfoType		stUniqueIDPanelInfo;
};

struct	stGlassMakeType
{
	short	shAckCode;
	long	nPortNo;
	long	nSlotNo;
	long	nModuleID;	 

	stPanelInfoType	stPanelInfo;
};

//	Glass Delete Event & Judgement Event 공통 사용
struct	stGlassDeleteType
{
	short	shAckCode;
	long	nPortNo;
	long	nSlotNo;
	long	nModuleID;	 

	stPanelInfoType	stPanelInfo;
};
struct stElapseTimeDataType
{
	BOOL bCheckStart;
	BOOL bCheckEnd;
	
	BOOL bMeasureStart;
	BOOL bMeasureEnd;
	
	BOOL bReportFlag;
	
	float fStartSecond;
	float fElapsedSecond;
};
// @ brief RS-232C Serial Configuration
struct stCOMSettingType
{
	long	nComPortNo;
	long	nBaudRate;
	long	nParity;
	long	nDataBit;
	long	nStopBit;
	long	nAlarmHour;
	BOOL	bConnected;
	BOOL	bACKReceived;
	BOOL	bDataReceived;
};

struct stSEMAlarmDataType
{
	stElapseTimeDataType	stSEMAlarmElapseTime;
	
	long	nALSetCode;
	long	nALID;
	long	nALCD;
	
	char szALTX[MAX_ALARM_TEXT_LEN+1];
};

struct stSerialConfigType
{
	BOOL	bSEMTraceEnable;
	BOOL	bEndCommandSend;
	
	long	nControllerCount;
	stCOMSettingType	stCom[MAX_COMM_PORT_COUNT];	
	
	stSEMAlarmDataType  stSEMAlarmData;
};

struct stSEMSVDataType
{
	long	nIndex;
	long	nSVID;
	char	szSEMSV_Value[MAX_STATUS_VALUE_LEN+1];
	char	szSEMSV_Name[MAX_STATUS_VALUE_NAME_LEN+1];
};
struct stSEMControllerType
{
	long	nSVItemCount;
	stSEMSVDataType	stSVData[MAX_SEM_SVID_COUNT];
	
};
struct stSEM_SVItemType
{
	long	nControllerCount;
	stSEMControllerType		stSEMController[MAX_COMM_PORT_COUNT];
};

// 면취세정기 Tracking 관련 추가분
struct	stSMAUnitGlassIDType
{
	char	szPanelID[MAX_PANEL_ID_LEN+1];
};

struct stRPCDataType
{
	BOOL    bUsed;
	long    nModuleNo;
	long	nRPC_State;
	long	nSEQ_No;
	long	nPPIDType;

	char    szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	char    szH_PanelID[MAX_PANEL_ID_LEN+1];
	char    szRPC_PPID[MAX_PPID_LEN+1];
	char    szOriginalPPID[MAX_PPID_LEN+1];
	char	szSetTime[MAX_DATE_TIME_LEN+1];		
};

struct stRPCDataTableType
{
	long nRPCCount;
	
	BOOL bInsertEvent;
	BOOL bDeleteEvent;
	
	stRPCDataType stInsertRPCData;
	stRPCDataType stDeleteRPCData;
	
	stRPCDataType stRPCData[MAX_RPC_QUEUE_COUNT];
};

struct stRPCHistoryType
{
	long nRPCChangeByWho;
	long nRPCChangeMode;
	stRPCDataType stRPCData;
};

struct stSCH2GUIEventData
{	
	//RPC Data
	stRPCDataType stInsertRPCData;
	stRPCDataType stDeleteRPCData;
	stRPCDataType stStateChangeRPCData;
	stRPCHistoryType stRPCHistory;
};


struct stSystemRunInfoType
{
	stEventCtrlDataType	 stSystemEvtCtrl;

	long	nOnLineMode;			//	1 - OffLine, 2 - Local, 3 - OnLine Remote
	BOOL	bHsmsState;				//	0 : Disconnected, 1 : Connected
	long	nConnectionState;		//	0 : Not Selected, 1 : Not Connected, 2 : Selected, 3 : T3 Timeout

	long	nRecoveryMode;			//	0 - Normal, 1 - Auto, 2 - Clean Out,  3 - Mecha Running

	long	nEQState;				//	Current EQ State	: 1 - Normal, 2 - Fault, 3 - PM
	long	nProcState;				//	Current EQ Process State	: 1 - Init, 2 - Idle, 3 - Setup, 4 - Ready, 5 - Execute, 6 - Pause

	long	nProcessMode;			//	RW설비 진행방향 명시.
									//	: 0 - Normal(상공 투입 Stripper 배출)
									//	: 1 - Bypass Normal(상공 순방향)
									//	: 2 - Bypass Back  (상공 역방향)

	BOOL	bMasterState;			//	PLC I/F Alive 상태
	BOOL	bMasterEditBusy;		//	CIM Data 편집 상태
	BOOL	bJudgeMode;				//	Judgement 실행
	BOOL	bPMMode;				//  CIM 에서의 PM 명령 상태

	BOOL	bTimeSetReq;			// Date/Time 동기화 (0.5초 ON후 OFF)
	BOOL	bPMCodeSetReq;			// Code 등록 Request
	BOOL	bBrokenCodeSetReq;
	BOOL	bJudgeCodeSetReq;
	BOOL	bMNCellMode;

	stTowerLampControlType	stTowerControl;	//	Tower Lamp / Buzzer Control
	stTraceTableType		stTraceTbl;
	stTraceTableType		stTraceTbl_SEM;

	stPanelInfoType			stULGlassData;

	stWaitAlarmTableType	stWaitAlmTbl[MAX_LAYER1_MODULE_COUNT];

	//	온도 모니터링 Data
	stTempMonitoringDataTableType		stCurTemp;

	//	제어 Module별 Running Info
	stModuleRunInfoType			stModRunInfo[MAX_LAYER1_MODULE_COUNT];

	// Unitque ID Table 유지 - Unit내의 Run Glass Data 체크용
	stSMAUniqueIDTableType		stUniqueIDTable[MAX_UNIQID_COUNT];
	stGlassMakeType		stUnscrapData;
	stGlassDeleteType	stScrapData;

	// RS-232C Serial 정보
	stSerialConfigType	stSerialCfg;
	stSEM_SVItemType	stSEM_SVItem;

	//면취 세정기 Manual Cell Load HPanel ID
	char szEDGEManualCellID[MAX_PANEL_ID_LEN + 1];
	// for 면취후 세정기 Tracking
	stSMAUnitGlassIDType		stSMAUnitGlassID[MAX_LAYER1_MODULE_COUNT];

	stRPCDataTableType	stRPCTbl;

	stSCH2GUIEventData  stSCHEventData;
};

/*
///////////////////////////////////////////////////////////////////
// Indexer 관련 Structure
//	Indexer 자체 정보
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
	stContactPointIOType		stIOInConvStatus;    // stIOConvStatus -> stIOInConvStatus 수정됨
	stContactPointIOType		stIOOutConvStatus;   // 

	// Indexer Handshake I/O
	stGlassSendIOType			stIOGlassSend;	//입구 Loader
	stContactPointIOType		stIOSendPreView;
	stGlassRecvIOType			stIOGlassRecv;	//출구 Unloader
	stContactPointIOType		stIORecvPreView;

	// 설비(Process Module) Handshake I/O
	stGlassSendIOType			stIOEQGlassSend;	 // [입구] 
	stGlassRecvIOType			stIOEQGlassRecv;	 // [출구] 

	//	Indexer 자체 정보	
	stRobotWaitPosType			stIORobotWaitPos;
	stGlassStatusInArmIOType	stIOGlassStatusInArm;
	stRobotModeStatusIOType		stIORobotModeStatus;		// stIOMachineStatus;	
	stRobotStatusIOType			stIORobotRunStatus;		// Robot In Machine Status
	stPortStatusIOType			stIOPortStatus;
	stPortEventIOType			stIOPortEvent;
	stMachineReplyIOType		stIOMachineReply;
	stRobotReplyIOType			stIORobotReply;
	stGlassHandlingReplyIOType	stIOGlassHandleReply;
	stPortCommandReplyIOType	stIOPortCommandReply;
	stSpecialCommandReplyIOType	stIOSpecialCommandReply;
	stETCEventIOType			stIOETCEvent;

	//	Indexer Control Master 정보
	stMachineReqIOType			stIOMachineReq;
	stRobotReqIOType			stIORobotReq;
	stGlassHandlingReqIOType	stIOGlassHandleReq;
	stPortCommandReqIOType		stIOPortCommandReq;
	stSpecialCommandReqIOType	stIOSpecialCommandReq;
	stPortEventReplyIOType		stIOPortEventReply;
	stETCEventReplyIOType		stIOEtcReply;
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
	long	nPanelMapping[MAX_SLOT_COUNT_PER_PORT];
	char	szCassetteID[MAX_CASSETTE_ID_LEN+1];
	long	nBCRMode;

	//	Running Information Data
	char	szPortID[MAX_PORT_ID_LEN+1];
	long	nSortType;
	long	nCSTDemand;
	long	nHotDevice;				//	0 : Normal, 1 : Hot Device

	//	
	long	nPortEvent;				//	Port Event에 대한 Data

	stPanelInfoType	 stPanelInfo[MAX_SLOT_COUNT_PER_PORT];
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
	long	nState;		    //	Indexer EQ State
	long	nProcState;			//	Indexer Process State
	long	nPortCount;			//	Used Indexer Cassette Stage(Port) Count
	long	nRobotCount;		//	Used Robot Count

	long	nTimeOut3;		// Index <-> PC Timeout Check
	long	nTimeOut4;
	long	nTimeOutRetry;	// Time Out Retry Count Limit

	long	nCompany;
	long	nDockingType;
	long	nMelChannelNo;

	stIOInfoType	stIOInfo;
	stRobotInfoType	stRobotInfo;
	stPortInfoType	stPortInfo[MAX_PORT_COUNT];	//	Each Port Information
	
	stIdxReplyDataType	stReplyData;
	stIdxEventDataType	stEventData;
};
*/

struct	stSmaErrorType
{
	BOOL	bEvent;
	long	nErrorCode;
};

// CIM, ETCH, STRIP
struct stVersionInfo
{
	// CIM, PLC , Touch 공통 아이템
	char	szSoftRev[MAX_SOFTWARE_VERSION_LEN+1];   // SW 버전 or Touch 버전
	char	szReleaseTime[MAX_RELEASE_TIME_LEN+1];  // SW or Touch 업데이트 날짜
	char	szMDLN[MAX_MDLN_LEN+1];                  // 해당 SW or Touch  모듈ID ( CIM: EQPID, PLC: EQPID+ModuleID  )
	char	szSize[MAX_RELEASE_SIZE_LEN+1];          // 해당 SW or Touch 사이즈 ( 단위포함.) 10000 bytes or 10000 ladder
	char	szDesc[MAX_DESCRIPTION_LEN+1];           // 해당 SW or Touch 설명
};               

struct	stSystemVerionType
{
	// CIM Version
	stVersionInfo	stSoftRev[MAX_SOFT_VERSION_COUNT];
};

//	Alarm 발생시 사용
struct	stAlarmOccured
{
	long	nModuleID;	
	long	nAlarmCode;
	long	nAlarmID;
	long	nAlarmPause;
	char	szUnitID[MAX_UNIT_ID_LEN+1];
	char	szAlarmText[MAX_ALARM_TEXT_LEN+1];
};

//	Alarm 해제시 사용
struct	stAlarmTreated
{
	long	nModuleID; 
	long	nAlarmCode;
	long	nAlarmID;
	char	szUnitID[MAX_UNIT_ID_LEN+1];
	char	szAlarmText[MAX_ALARM_TEXT_LEN+1];
};

/*****************************************************************************************************
										T8-1 SMA Root
*****************************************************************************************************/
struct	stSmaRootType
{
	 /* System Layout Configure Information */
	stLayOutCfgType			stLayOutCfg;

	/* System Data Information */
	stSystemDataInfoType	stSysData;

	/* System Running Information */
	stSystemRunInfoType		stSysRunInfo;

	/* System Error Information */
	stSmaErrorType			stSmaError;

	stSystemVerionType		stVersionType;  // 2010-08-24 add

	stAlarmOccured			stAlarmOccuredEvent;		
	stAlarmTreated			stAlarmTreatedEvent;	

	stProcEndData_Inline_ET	stProcEnd_ET[MAX_SEMES_UNIQUE_COUNT];

	////////////////////////////////////////
	// Indexer 관련 All 정보
	////////////////////////////////////////
//	stIndexerInfoType		stIndexInfo;
};

#endif	//IPCSMA.H
