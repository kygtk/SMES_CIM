/*****************************************************************************************************
		T8-1 Constant Define Version Date : 2007-03-05
 *****************************************************************************************************/

#ifndef	__ConstDefine_h__
#define	__ConstDefine_h__
#include	<windows.h>

//***************************************
//*****		Define.h  			*****
//***************************************
//	1.	Configuration file Path & file Name Define
//	2.	IPC (SMA(Shared Memory & Queue(Name & Count)) Define
//	3.	System Configuration Define
//	4.	Host I/F Message Length Define
//	5.	
//	6.	Etc 
//	7.	Temp		
//
//////////////////////////////////////////////////////////////////////////////
//	1. Config File Path Name 및 file Name 변경함.
//	1)	Configuration File Path
#define	CONFIG_FILE_PATH			"D:\\FPDCIM\\DATA\\Config\\"

//	2)	Configuration File Name
#define	SYS_CONFIG_FILE_NAME		"SYSCONFIG.CFG"
//#define	TEMP_CONFIG_FILE_NAME		"TEMP.CFG"
#define SYS_RUN_INFO_FILE_NAME		"SYSRUN.CFG"

//	3)	Parameter 설정 File Name
#define	EOID_CONFIG_FILE_NAME		"EOID.CFG"
#define	ECID_CONFIG_FILE_NAME		"ECID.CFG"
#define	SVID_CONFIG_FILE_NAME		"SVID.CFG"
#define	DVID_CONFIG_FILE_NAME		"DVID.CFG"

#define	INDEX_CONFIG_FILE_NAME		"INDEX.CFG"	//	Indexer Module
#define	CLEAN_CONFIG_FILE_NAME		"CLEAN.CFG"	//	Cleaner Module
#define	DBMOD_CONFIG_FILE_NAME		"DBAKE.CFG"	//	Dehyde Bake Module
#define	CTMOD_CONFIG_FILE_NAME		"CTOAT.CFG"	//	Toray Coater Module
#define	VCMOD_CONFIG_FILE_NAME		"VCDOAT.CFG"	//	Toray Coater Module
#define	SBMOD_CONFIG_FILE_NAME		"SBAKE.CFG"	//	Soft Bake Module
#define	BFMOD_CONFIG_FILE_NAME		"BFMOD.CFG"	//	Buff Module
#define	IFMOD_CONFIG_FILE_NAME		"IFMOD.CFG"	//	Interface Module
#define	EXMOD_CONFIG_FILE_NAME		"EXMOD.CFG"	//	노광기 Module
#define	EGMOD_CONFIG_FILE_NAME		"EGMOD.CFG"	//	Edge Exp Module
#define	DVMOD_CONFIG_FILE_NAME		"DVMOD.CFG"	//	Developer Module
#define	INMOD_CONFIG_FILE_NAME		"INMOD.CFG"	//	Inspector Module
#define	IRMOD_CONFIG_FILE_NAME		"IRMOD.CFG"	//	IR Oven Module
#define	MCMOD_CONFIG_FILE_NAME		"MCMOD.CFG"	//	Macro Inspector Module
#define	PBMOD_CONFIG_FILE_NAME		"PBMOD.CFG"	//	PostExpouseBake Module
#define	HBMOD_CONFIG_FILE_NAME		"HBMOD.CFG"	//	HARDBake Module

///////////////////////////////////////////////////////////////////////
//	2.	IPC ( SMA(Shared Memory) & Queue(Internal Signal Queue) )
//	1) SMA Name
#define	SHARE_INFORMATION_NAME		"IPCSMA"

//	2) Task Queue Name
#define	MMI_QUEUE_NAME				"MMI_QUEUE"
#define	SCH_QUEUE_NAME				"SCH_QUEUE"
#define	HST_QUEUE_NAME				"HST_QUEUE"			
#define	IND_QUEUE_NAME				"IND_QUEUE"				
#define	PLC_QUEUE_NAME				"PLC_QUEUE"
#define	TMP_QUEUE_NAME				"TMP_QUEUE"

#define SIMULATER_QUEUE_NAME		"SIMULATOR_QUEUE"

//	2-1) Task Inner Queue Name
#define	TEMP_THREAD_QUEUE_NAME		"TMP_THREAD_QUEUE"

//	2-2) PLC Inner Queue Define	
#define	MDL_CLN_QUEUE_NAME			"MDL_CLN"
#define	MDL_DBK_QUEUE_NAME			"MDL_DBK"
#define	MDL_COT_QUEUE_NAME			"MDL_COT"
#define	MDL_VCD_QUEUE_NAME			"MDL_VCD"
#define	MDL_SBK_QUEUE_NAME			"MDL_SBK"
#define	MDL_IFU_QUEUE_NAME			"MDL_IFU"
#define	MDL_PEB_QUEUE_NAME			"MDL_PEB"
#define	MDL_DEV_QUEUE_NAME			"MDL_DEV"
#define	MDL_PBK_QUEUE_NAME			"MDL_PBK"

#define MDL_ETCH_QUEUE_NAME			"MDL_ETCH"
#define MDL_STRP_QUEUE_NAME			"MDL_STRP"
#define MDL_BUFF_QUEUE_NAME			"MDL_BUFF"
#define MDL_BYPASS_QUEUE_NAME		"MDL_BYPASS"

// PSK 080225 SD01 호기 QUEUE NAME 추가
#define MDL_PFC_QUEUE_NAME			"MDL_PFC"
#define MDL_EX_QUEUE_NAME			"MDL_EX"

#define MDL_EDGE_QUEUE_NAME			"MDL_EDGE"
#define MDL_WRU_QUEUE_NAME			"MDL_WRU"
#define MDL_LCPI_QUEUE_NAME			"MDL_LCPI"
#define MDL_LCODF_QUEUE_NAME		"MDL_LCODF"
#define MDL_LCRW_QUEUE_NAME			"MDL_LCRW"

#define	MAX_MDL_QUEUE_COUNT				20

//	3) Task Queue Count
#define	MAX_MMI_QUEUE_COUNT				100
#define	MAX_SCH_QUEUE_COUNT				100
#define	MAX_PLC_QUEUE_COUNT				100
#define	MAX_HST_QUEUE_COUNT				100
#define	MAX_IND_QUEUE_COUNT				30
#define	MAX_PLC_QUEUE_COUNT				100
#define	MAX_TMP_QUEUE_COUNT				30

/////////////////////////////////////////////////////////////////////////
// Task ID
// 정규 Process Task ID
#define GUI_TASK_ID						1
#define PLC_TASK_ID						2
#define SEM_TASK_ID						3
#define	SCH_TASK_ID						4
#define HST_TASK_ID						5
#define LMP_TASK_ID						6

///////////////////////////////////////////////////////////////////////
//	3.	System Configuration Define
//	1) System Layout Count
#define	MAX_LAYER1_MODULE_COUNT			5	//16
#define MAX_LAYER1_SEMES_MODULE_COUNT	10	
#define	MAX_LAYER2_MODULE_COUNT			30
#define MAX_UNIT_COUNT					30
#define	MAX_BUFFER_SLOT_COUNT			30		//20	
#define	MAX_ROBOT_ARM_COUNT				2	
#define	MAX_BUFFER_COUNT				3
#define MAX_HANDSHAKE_COUNT				3		// V105
#define MAX_GECD_EVENT_COUNT			5

#define MAX_HS_VALID_ID_COUNT			3		// 1: Duplication, 2: Omission, 3: Availability

#define	MAX_INDEXER_COUNT				2
#define MAX_CONV_MODULE_COUNT			2

#define MAX_SERIAL_COMM_PORT_COUNT		8	//	Rocket Port : 8 Port
																			//	Temp 관련 통신 Port 사용 수량( 세정기/현상기 )

//	2) Control Item Count
#define MAX_SVID_1WORD_COUNT			610
#define MAX_SVID_2WORD_COUNT			100
#define	MAX_SVID_COUNT					710

#define	MAX_ECID_COUNT					1000	//300	

#define	MAX_FLOW_COUNT					16	
#define	MAX_PRESS_COUNT					16
#define	MAX_FLOWPRESS_COUNT				500
#define	MAX_TEMP_CTRL_COUNT				16	
#define MAX_EPD_CTRL_COUNT				6
#define	MAX_TANK_KIND_COUNT				2
#define	MAX_MODULE_TANK_COUNT			2
#define	MAX_ETCH_TANK_COUNT				4	
#define	MAX_TANK_TOTAL_COUNT			5
#define	MAX_ECID_ITEM_COUNT				5
// shmagic 2011.06.20 접액시간추가.
#define MAX_CONTTACT_TIME_COUNT			8
#define MAX_CONTTACT_TIME_ETCH_COUNT	4 // 맵에 표기해놓은 ContactTime에 관련된 Etcher 수.
#define MAX_CONTTACT_TIME_RINSE_COUNT	3 // 맵에 표기해놓은 ContactTime에 관련된 Rinse 수.

#define MAX_PLATE_COUNT					3	//HP(1~3), CP(1~3)
#define MAX_PLATE_TYPE_COUNT			3	//AP,HP,CP
#define MAX_TEMP_CHANGE_COUNT			2	
#define	MAX_BAKE_MODULE_COUNT			4	//DBK / SBK / PBK / PEB
#define	MAX_BRUSH_COUNT					8
#define	MAX_BRUSH_USED_COUNT			6	
	
//#define	MAX_HNO3_TANK_COUNT			2
#define	MAX_STRIP_TANK_COUNT			2
#define	MAX_TERMINAL_COUNT				10
#define	MAX_TOWER_LAMP_COUNT			2
#define	MAX_BUZZER_COUNT				10

//	3) Data Item Count
#define MAX_DCOLL_MODULE_COUNT			8
#define	MAX_EQ_CONSTANT_COUNT			1000	//160
#define	MAX_DATA_COLLECT_ITEM_COUNT		370	// 160(FlowPress) + 10(Temp) + 100(Tank) + 100(Unit Tact)

#define MAX_TOTAL_EQ_CONSTANT_COUNT		1500  
#define MAX_ECID_MULTI_COUNT			300		// USL,UWL, LSL, LWL, DEF 값을 모두 사용하는 ECID Count
#define	MAX_ECID_SINGLE_COUNT			500		// DEF 값만 사용하는 ECID Count

//	4) Alarm
#define	MAX_ALARM_COUNT_IN_MODULE		4500	//3000
#define	MAX_WAIT_ALARM_COUNT			100

//	5) Recipe
#define	MAX_MODULE_RECIPE_COUNT			50			//Module Recipe
#define	MAX_PPID_COUNT					999			//Flow Recipe
#define MAX_RECIPE_PARAM_COUNT			200			//Sub Recipe Item Count     //cts


#define	MAX_ETCHING_STEP_COUNT			5		//Etching Step 1~4
#define	MAX_ETCHING_UNIT_COUNT			4		//Etching Unit
#define MAX_ETCHING_ZONE_ITEM_COUNT		7
#define	MAX_ETCH_PROCESS_PARAM_COUNT	20
#define	MAX_RINSING_UNIT_COUNT			3

#define	MAX_STRIPPING_UNIT_COUNT		4		//Stripping Unit
#define	MAX_STRIPPING_STEP_COUNT		10
#define	MAX_STRIP_PROCESS_PARAM_COUNT	20

///////////////////////////////////////////////////////////////////
//	6) Temp Control
#define	COMPONENT_TEMP_CONTROL_TYPE		1
#define	TEMP_CONTROL_TYPE_REX_F400		1	//	RKC	F400 Controller
#define	TEMP_CONTROL_TYPE_MA900			2	//	RKC	MA900 Controller

#define	MAX_MA900_MODULE_COUNT			5	//
#define	MAX_F400_MODULE_COUNT			5	//
#define	MAX_NODE_COUNT					10	//	Multi Drop 방식으로 최대 32개 까지 접속이 가능
												//	단, MA900의 경우 H/W Module 당 4개의 Node를 갖는다.

///////////////////////////////////////////////////////////////////////
//	4.	Host I/F Message Length Define
// EQ Modeling 관련 Define
#define MAX_EQ_MODULE_ID_LEN			27	// EQ(7)_Layer1(4)_Layer2(4)_Layer3(4)
#define	MAX_EQPID_LEN					17
#define	MAX_LAYER_MODULE_ID_LEN		    4
#define	MAX_UNIT_ID_LEN					4
#define	MAX_MODEL_NUMBER_LEN			6
#define	MAX_SOFT_REVISION_LEN			6
#define	MAX_ONLINE_MODE_LEN				6
#define MAX_UNIT_DESCRIPT_LEN           20
#define MAX_MODULE_DESCRIPT_LEN         20

// ONLINE Parameter 관련 Define
#define	MAX_EOMD_LEN					40	// Online Parameter Mode Len ( U2 -> ASCII(4) 로 변경 T7-2 )
#define	MAX_ONLINE_PARAM_COUNT			5	// Online Parameter Count
#define	MAX_ONLINE_PARAM_MODE_COUNT		40	// Online Parameter Mode Count

#define	MAX_SPOOL_JOB_END_COUNT			2	//

//	Status Variable Define
#define	MAX_STATUS_VALUE_NAME_LEN		40
#define	MAX_STATUS_VALUE_LEN			80


//	Equipment Constant Define
#define	MAX_EQ_CONSTANT_NAME_LEN		40
#define	MAX_EQ_CONSTANT_VALUE_LEN		20

//	Remote Command
#define	MAX_EQ_CMD_RCODE_LEN			4	//	T7-2 Add

//	Alarm Variable Define
#define	MAX_ALARM_TEXT_LEN				80
#define	MAX_ALARM_TIME_LEN				14
#define MAX_ALARM_HISTORY_COUNT		    20
#define	MAX_ALARM_DATA_COUNT			1000

//	Data & Time
#define	MAX_DATE_TIME_LEN			    14
#define MAX_SAMPLE_COLLECTED_TIME       18 //yyyymmddhhmmss.mmm
#define	MAX_TIME_LEN					10 //hhmmss.mmm 	6	// hhmmss
#define	MAX_DATE_LEN					8	// yyyymmdd
#define MAX_LIMIT_TIME_LEN              6
//	Port
#define	MAX_PORT_COUNT					6	// Normal Port 5, NG Port 1
#define	MAX_PORT_ID_LEN					4
#define	MAX_PORT_MODE_LEN				2
#define	MAX_SLOT_COUNT_PER_PORT	        20
#define	MAX_SLOT_NUMBER_LEN			    4

//	Cassette
#define	MAX_CASSETTE_ID_LEN			    16
#define	MAX_SLOT_INFO_LEN				80
#define MAX_TRAY_ID_LEN					16

//	Glass
#define	MAX_PANEL_ID_LEN				12
#define	MAX_BATCH_ID_LEN				12
#define	MAX_STEP_ID_LEN					12
#define	MAX_PROCESS_ID_LEN			    20		//T8 추가
#define	MAX_PRODUCT_ID_LEN		    	20		//T8 추가
#define	MAX_PPID_LEN					16
#define	MAX_FLOW_ID_LEN					4
#define MAX_DBR_RECIPE_LEN              4       
#define	MAX_GLASS_SIZE_LEN				2
#define	MAX_GLASS_THICK_LEN				2
#define	MAX_ALL_PORT_GLASS_COUNT		90	//	15 * 6
#define	MAX_PRODUCT_TYPE_LEN			2
#define	MAX_PRODUCT_KIND_LEN			2
#define	MAX_INSPECT_FLAG_LEN			2

#define	MAX_GLASS_POSITION_LEN			2
#define	MAX_GLASS_COUNT_LEN				2
#define	MAX_PANEL_TYPE_LEN				2
#define	MAX_GLASS_COMMENT_LEN			16	
#define	MAX_JUDGEMENT_RESULT_LEN		4
#define	MAX_JUDGEMENT_CODE_LEN			4
#define	MAX_READING_FLAG_LEN			2
#define MAX_CELL_GRADE_LEN				160
#define	MAX_FLOW_HISTORY_LEN			40			//T8 추가
#define	MAX_MULTI_USE_LEN				20			//T8 추가
#define	MAX_FLAG_NAME_LEN				16			//T8 추가 - Bit Signal 용으로 사용
#define	MAX_FLOW_GROUP_LEN				20
#define	MAX_USABLE_CHAMBER_LEN			10

#define	MAX_RUN_LINE_LEN				20	
#define	MAX_UNIQUE_ID_LEN				4
#define	MAX_MATCH_GROUP_LEN				2

#define	MAX_WORKING_STATE_BATCH_COUNT	10

#define MAX_GLASS_SUB_DATA_COUNT		1

#define MAX_POST_ACT_DATA_LEN			16
#define MAX_BIT_SIGNAL_FLAG_NAME		16

#define MAX_TOTAL_GLASS_DATA_SIZE		180

//	Recipe
#define	MAX_COMMAND_CODE_COUNT			40	//30
#define	MAX_PROCESS_PARAM_NAME_LEN		40	//16
#define	MAX_PROCESS_PARAM_VALUE_LEN		80	//16
#define	MAX_RECIPE_NICK_NAME_LEN		14
#define	MAX_PROCESS_PARAM_COUNT			200		//100		//20
#define MAX_BRUSH_TYPE                  3

//	Etc
#define	MAX_MESSAGE_HEADER_LEN			10
#define	MAX_TEXT_MESSAGE_LEN			80
#define	MAX_TERMINAL_MESSAGE_LEN		MAX_TEXT_MESSAGE_LEN
#define	MAX_PLC_VERSION_LEN		        20
#define MAX_SETTING_TIME_LEN            6
#define MAX_TACT_UNIT_LEN			    16

#define	MAX_ATTR_DATA_COUNT				10
#define MAX_ATTR_ID_LEN					40
#define MAX_ATTR_DATA_LEN				40


#define MAX_MESSAGE_REASON_LEN			160	

#define MAX_EQ_SPECIFIC_ITEM_COUNT		5
#define MAX_EQ_SPECIFIC_ITEM_NAME		40
#define MAX_EQ_SPECIFIC_ITEM_VALUE		80

#define MAX_EQ_SPECIFIC_STEP_LEN		20

#define MAX_PROCESS_CTRL_DATA_QUEUE_COUNT	MAX_RPC_QUEUE_COUNT // 200
#define MAX_RANGE_LEN				10
#define MAX_SYMBOL_LEN				10

///////////////////////////////////////////////////////////////////
//	4.	Len 최대 정의 상수 
#define	MAX_LOT_TYPE_LEN				16
#define	MAX_LOT_KIND_LEN				8
#define	MAX_TACT_LEN					2
#define	MAX_STDCELL_LEN					2

#define	MAX_LOT_ID_NAME_LEN				16
#define	MAX_GLASS_ID_NAME_LEN			16
#define	MAX_PPID_NAME_LEN				16
#define	MAX_STEP_ID_NAME_LEN			16
#define	MAX_RECIPE_NAME_LEN				32
#define	MAX_ACTIVE_IP_LEN				16

//	Message 관련 Define 상수 정의
#define	MAX_PROCESS_ID_LEN				20
#define	MAX_PART_ID_LEN					20
#define	MAX_GLASS_TYPE_LEN				2
#define	MAX_LOT_ID_LEN					16
#define	MAX_LOT_ACTION_LEN				16

//  S1F6
#define MAX_EQ_CURRENT_INTERLOCK_COUNT		10
#define MAX_EQ_INTERLOCK_ITEM_NAME_LEN		40
#define MAX_EQ_INTERLOCK_ITEM_VALUE_LEN		40
#define MAX_EQ_INTERLOCK_RELATED_MODULEID	40

//	S6F11
#define	MAX_OK_NG_REPORT_LEN			2
#define	MAX_SPLIT_MODE_LEN				4
#define	MAX_PROCESS_RESULT_LEN			6

#define	MAX_MATERIAL_NAME_LEN			40
#define	MAX_MATERIAL_VALUE_LEN			40

#define MAX_CODE_LEN                    4 
#define MAX_CODE_DESCRIPTION_LEN        20    
#define MAX_CODE_COUNT                  20        

#define MAX_SLOT_ID_LEN                 4

//	S6F13
#define	MAX_S6F13_VARIABLE_NAME_LEN		10
#define	MAX_DATA_COLLECT_ITEM_NAME_LEN	40
#define MAX_BARCODE_LEN					20	

//	S9*

//	S64*	// T8-2 추가사양
#define MAX_GLASSHISTORYDATA_COUNT		60
//////////////////////////////////////////////////
//	Hsms Status
#define HSMS_NOT_CONNECTED				1
#define HSMS_NOT_SELECTED				2
#define HSMS_SELECTED					3

#define	MAX_ROBOT_COUNT					1
#define	MAX_ROBOT_ARM_COUNT	                        2

#define	MAX_TRACE_ID_COUNT				MAX_SVID_COUNT
#define	MAX_LOT_DATA_COUNT				80

#define	MAX_SPEC_CONTROL_EVENT_COUNT			10	//	T7-2 Add
#define	MAX_SPEC_CONTROL_DATA_NAME_LEN			40	//	T7-2 Add
#define	MAX_SPEC_CONTROL_DATA_VALUE_LEN			80	//	T7-2 Add
#define	MAX_SPEC_CONTROL_RELATED_MODULEID		40	//	T7-2 Add


// Material Information Data (S3FX)
#define	MAX_MATERIAL_ID_LEN				80
#define	MAX_MATERIAL_TYPE_LEN			8
#define MAX_MATERIAL_KIND_LEN			20
#define MAX_MATERIAL_LAYER_LEN			12
#define MAX_MATERIAL_CODE_LEN			16
#define MAX_MATERIAL_SLOTID_LEN			2
#define	MAX_MATERIAL_LOC_LEN			40
#define MAX_MATERIAL_TOTAL_QTY_LEN		10
#define MAX_MATERIAL_USED_CNT_LEN		10
#define MAX_MATERIAL_USED_QTY_LEN		10
#define MAX_MATERIAL_REMAINED_QTY_LEN	10
#define MAX_MATERIAL_REQ_QTY_LEN		10
#define MAX_MATERIAL_NG_QTY_LEN			10
#define MAX_MATERIAL_ASSEMBLED_QTY_LEN	10

#define MAX_MATERIAL_PROCESS_ID_LEN		20
#define	MAX_MATERIAL_PRODUCT_ID_LEN		20
#define MAX_MATERIAL_BATCH_ID_LEN		12
#define MAX_MATERIAL_STEP_ID_LEN		12
#define MAX_MATERIAL_PPID_LEN			16

#define MAX_MATERIAL_TRAY_ID_LEN		16
#define MAX_MATERIAL_PANEL_ID_LEN		12
#define MAX_MATERIAL_ASSEMBLED_LOC_LEN	40
#define MAX_MATERIAL_CANCEL_CODE_LEN	4
#define MAX_MATERIAL_DEFECT_CODE_LEN	4

#define	MAX_MATERIAL_COUNT				10
#define	MAX_MATERIAL_DEVICE_ID_LEN		18
#define MAX_MATERIAL_PRODUCTID_COUNT	10

#define MAX_LABEL_LEN                   8
#define	MAX_RCMD_LABEL_LEN				8
#define MAX_RCODE_LEN                   4
#define	MAX_REPORT_GROUP_SIZE			10
#define	MAX_OPERATOR_ID_LEN				16
#define	MAX_REASON_CODE_LEN				4

#define	MAX_LIBRARY_ID_LEN				2

#define	MAX_DATA_COLLECT_ITEM_VAULE_LEN	80

#define MAX_JUDGE_LABEL_LEN             9
#define MAX_GLASS_JUDGE_COUNT           100

//////////////////////////////////////////////////
//////////////////////////////////////////////////
//	Indexer Interface 관련 Define
#define	MAX_IDX_ALARM_TEXT_LEN		20

//	Indexer Port Status
#define	IND_PORT_STATUS_EMPTY		0x4D45	//	EM
#define	IND_PORT_STATUS_IDLE		0x4449	//	ID
#define	IND_PORT_STATUS_READY		0x4452	//	RD
#define	IND_PORT_STATUS_WAIT		0x5457	//	WT
#define	IND_PORT_STATUS_RESERVE		0x5352	//	RS
#define	IND_PORT_STATUS_BUSY		0x5342	//	BS
#define	IND_PORT_STATUS_COMPLETE	0x4D43	//	CM
#define	IND_PORT_STATUS_ABORT		0x4241	//	AB
#define	IND_PORT_STATUS_CANCEL		0x4E43	//	CN
#define	IND_PORT_STATUS_PAUSE		0x5350	//	PS
#define	IND_PORT_STATUS_DISABLE		0x554E	//	NU
#define	IND_PORT_STATUS_ERROR		0x5245	//	ER

#define	WAIT_POS_STAGE1				0x4D57	//	WM
#define	WAIT_POS_STAGE2				0x4E57	//	WN

#define	COURCE_BUFFER	0x42	//	Not Used
#define	COURCE_NOT_USED	0x4E	//	Not Used
#define	COURCE_PORT		0x50
#define	COURCE_STAGE	0x53
#define	COURCE_TEST		0x54	//	Not Used
#define	COURCE_WAIT_POS	0x57	//	Not Used	

#define	STEP_GET		0x47
#define	STEP_LAY		0x4C
#define	STEP_MOVE		0x4D
#define	STEP_ROTATE		0x52

#define	PORT_ENABLE		0x4E45	//	EN
#define	PORT_DISABLE	0x4944	//	DI

#define	ACK_OK			0x01

////////////////////////////////////////////////////////
// T7-2 Photo 전용  Define
//	EQ to EQ Interface Common
#define	MAX_SUMMARY_DATA_COUNT	3	//H/S MAX COUNT

//	CIM PC 최대 사용 가능한 MELSEC Board 수량
#define	MAX_MELSEC_BOARD_COUNT	4

//	20050816
#define	MAX_REFUSE_CODE_LEN		3

// Add Item
#define MAX_UNIQID_COUNT		256

#define MAX_MODULE_RUN_DATA_COUNT	8
//Panel Size
#define PANEL_SIZE_WIDTH		2500
#define PANEL_SIZE_HEIGHT		2200

// Upper Glass Data
#define MAX_CSIF_LEN				4

// PM/Pause Code
#define MAX_PAUSE_CODE_LEN			4
#define MAX_PM_CODE_LEN				4

#define TANK_TEMP_SVID_OFFSET		35		//Etcher_Tank_NO Start SVID		2009-05-19 KWY

//T8Y
#define MAX_DESCRIPTION_LEN			40
#define MAX_SYMBOL_LEN				10

#define	MAX_ER_REGULATOR_COUNT		10

// 2010-08-24 ADD
// For S1F5 Soft Version Request
#define	MAX_MDLN_LEN				40
#define	MAX_SOFTWARE_VERSION_LEN	40
#define	MAX_RELEASE_TIME_LEN		40
#define	MAX_RELEASE_SIZE_LEN		40
#define MAX_SOFT_VERSION_COUNT		10

////////////MSC LOG 관련 Define//////////////
//Action Log
#define MAX_DATE_LEN				8
#define MAX_OCCUR_TIME_LEN	    	12
#define MAX_MODULE_LEN				8
#define MAX_SUB_UNITID_LEN			8
#define MAX_INFO_SUB_UNITID_LEN		4
#define MAX_LOG_TYPE_LEN			4
#define MAX_FROM_POSITION_LEN		8
#define MAX_TO_POSITION_LEN			8
#define MAX_ACTION_ID_LEN			8
#define MAX_ACTION_ID_COUNT			48
//Event Log
#define MAX_LOG_TYPE_LEN			4
#define MAX_LOG_STEP_ID_LEN			8

#define TANK_TEMP_SVID_MAPOFFSET	501		// Tank_No_Start SVID

#define MAX_HS_COUNT				3

#define MAX_EQ_STATE_COUNT			4
#define MAX_PROCESS_STATE_COUNT		7


// EPD Event Time
#define MAX_EPD_TIME_COUNT          4


#define MAX_SEMES_UNIQUE_COUNT		256		//	0-255

// BoB ECO EOMD, EOV
#define MAX_ECO_EOMD_COUNT         99

#define MAX_PPC_QUEUE_COUNT    200
#define MAX_APC_QUEUE_COUNT    200
#define MAX_RPC_QUEUE_COUNT    200

////////////Serial 통신(SEM) 관련 Define//////////////
#define MAX_COMM_PORT_COUNT	7
#define MAX_SEM_SVID_COUNT	1000
#define	SEM_SVID_MIN		60000
#define SEM_SVID_MAX		65535

#endif	//	__ConstDefine_h__
