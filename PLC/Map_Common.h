#ifndef	_FPDCIMDataMap_h__
#define	_FPDCIMDataMap_h__

/* Bit Area */
/*============================================= MASTER AREA ======================================================*/
// L01 Bit CIM
#define	B_L1_ALIVE_STATUS						0x0000 // MELSEC-NET STATUS (Alive On)
#define	B_L1_STATE_STATUS						0x0001 // NORMAL/ABNORMAL (Normal On)
#define	B_L1_DATA_EDIT_STATUS					0x0002 // L1 PROCESS DATA (GUI) EDITTING
#define	B_L1_JUDGE_MODE_STATUS					0x0003 // 1:JUDGEMENT 판정(SCH ONLINE)

// L02 Req & Event Bit CLN or ETCH
#define	B_L2_FROM_CIM_MACHINE_CMD_REQ			0x0020 
#define	B_L2_FROM_CIM_ALARM_CLR_REQ				0x0021 
#define	B_L2_TIMESET_SYNC_REQ					0x0022 // DATA/TIME 동기화 
#define	B_L2_FROM_CIM_TERMINAL_MSG_REQ			0x0023
#define	B_L2_FROM_CIM_BUZZER_STOP_REQ			0x0024 
#define	B_L2_TO_CIM_NR_HS_TIME_OVER_RLY			0x0025 // HANDSHAKE TIME OVER EVENT [FOR NORMAL EQ]
#define	B_L2_TO_CIM_NR_HS_VALID_RLY				0x0026 // HANDSHAKE VALID FAIL CHECK [EVENT FOR NORMAL EQ]

#define	B_L2_FROM_CIM_OPERATORCALL_MSG_REQ		0x0027

#define	B_L2_TO_CIM_ALARM_OCCUR_RLY				0x0028 
#define	B_L2_TO_CIM_ALARM_TREAT_RLY				0x0029 
#define	B_L2_TO_CIM_GLASS_SCRAP_RLY				0x002A 
#define	B_L2_TO_CIM_GLASS_UNSCRAP_RLY			0x002B 
#define	B_L2_TO_CIM_GLASS_JUDGE_RLY				0x002C 
#define	B_L2_TO_CIM_RECIPE_DOWN_RLY				0x002D 
#define	B_L2_TO_MANUAL_CELL_LOAD_RLY			0x002E // 면취용
#define	B_L2_TO_GLASS_SEND_FAIL_RLY				0x002E

//Not Used
#define	B_L2_FROM_CIM_ECID_CHANGE_REQ			0x0030 // L2, L9
#define	B_L2_FROM_CIM_HP_DATA_CHANGE_REQ		0x0030 // L3, L4, L5, L6, L7 ,L8, L10
#define	B_L2_FROM_CIM_TEMP_CHANGE_REQ			0x0031 // L2, L9
#define	B_L2_FROM_CIM_AP_DATA_CHANGE_REQ		0x0031 // L3, L4, L5, L6, L7 ,L8, L10
#define	B_L2_FROM_CIM_TANK_CHANGE_REQ			0x0032 // L2, L9
#define	B_L2_FROM_CIM_CP_DATA_CHANGE_REQ		0x0032 // L3, L4, L5, L6, L7 ,L8, L10
#define	B_L2_FROM_CIM_ECO_MODE_DATA_CHANGE_REQ	0x0032 
#define	B_L2_MNCELL_READ_REQ					0x0033
#define	B_L2_MN_VALID_CHK_RLY					0x0034
#define	B_L2_MN_CANCEL_REQ						0x0035
#define	B_L2_TO_CIM_VCR_READ_FAIL_RLY			0x0036 // VCR FAIL REPLY 면취 전용
#define	B_L2_MN_JUDGE_REQ						0x0037

#define	B_L2_TO_CIM_ECID_CHANGE_EVENT			0x0038
#define	B_L2_TO_CIM_TEMP_CHANGE_EVENT			0x0039
#define	B_L2_TO_CIM_TANK_CHANGE_EVENT			0x003A
#define	B_L2_TO_CIM_ECO_MODE_CHANGE_RLY			0x003A
#define	B_L2_TO_CIM_PPID_AVAIL_CHK				0x003B
#define	B_L2_TO_CIM_PROC_ENDDATA_EVENT			0x003C
#define	B_L2_TO_CIM_BC_HS_TIME_OVER_RLY			0x003D // HANDSHAKE TIME OVER EVENT [FOR BYPASS EQ ]
#define	B_L2_TO_CIM_BC_HS_VALID_RLY				0x003E // HANDSHAKE VALID FAIL CHECK EVENT [FOR BYPASS EQ]
#define	B_L2_TO_BACK_MODE_RLY					0x003F // PHOTO

#define	B_CIM_EACH_LAYER_INTERVAL				0x0020
/*============================================= MASTER AREA ======================================================*/

/*=============================================   EQ   AREA ======================================================*/
// L02 ~ L10
// OPERATION STATUS
#define	B_L2_ALIVE_STATUS						0x0200 // MELSEC-NET STATUS (ALIVE On)
#define	B_L2_INLINE_STATUS						0x0201 // HOST Inline STATUS (INLINE = On)
#define	B_L2_EQ_MODE							0x0202 // MANUAL/AUTO (AUTO = On)
#define	B_L2_EQ_STANBY							0x0203 // READY ON/OFF (READY = On)
#define	B_L2_EQ_OP_START						0x0204 // EQ OPERATION START STATUS (START = On)
#define	B_L2_EQ_OP_STOP							0x0205 // EQ OPERATION STOP STATUS (STOP = On)
#define	B_L2_EQ_CYCLE_STOP						0x0206 // EQ CYCLE STOP STATUS (CYCLE STOP = On)
#define	B_L2_EQ_PM_STATE						0x0207 // EQ PM STATE (PM = On)
#define	B_L2_EQ_BUZZER							0x0208 // EQ BUZZER ON/OFF (BUZZER ON = On)
#define	B_L2_EQ_DATA_EDIT_STATE					0x0209 // EQ DATA EDIT 유무 (EDITTING = On)

#define	B_L2_SLEEP_MODE_READY					0x020E // On - Sleep Mode 진입 可, Off - Sleep Mode 진입 不可

#define	B_L2_WARNING_ALARM						0x0210 // WARNING ALARM 발생 유무 (OCCUR = On)
#define	B_L2_HEAVY_ALARM						0x0211 // HEAVY ALARM 발생 유무 (OCCUR = On)
#define	B_L2_CHEMICAL_CHNG_WARNING				0x0212 // CHEMICAL CHANGE WARNING (WARNING = On)
#define	B_L2_CHEMICAL_CHANGING					0x0213 // CHEMICAL CHANGING (CHANGING = On)

// GLASS SET
#define	B_L2_POS1_GLS_SET						0x0220 // POSITION 1 GLASS SET (POS1 ~ POS50)

// REQUEST & EVENT TO CIM 
#define	B_L2_MACHINE_CMD_RLY					0x0260 // MACHINE COMMNAD REPLY
#define	B_L2_ALARM_CLEAR_RLY					0x0261 // ALARM CLEAR REPLY
#define	B_L2_TIMESET_SYNC_RLY					0x0262 // TIME SET REPLY
#define	B_L2_TERMINAL_MSG_RLY					0x0263 // TERMINAL MESSAGE REPLY
#define	B_L2_BUZZER_STOP_RLY					0x0264 // BUZZER STOP REPLY

#define	B_L2_ALARM_OCCUR						0x0268 // ALARM OCCURRED EVENT
#define	B_L2_ALARM_TREAT						0x0269 // ALARM TREATED EVENT
#define	B_L2_GLASS_SCRAP						0x026A // GLASS SCRAP EVENT
#define	B_L2_GLASS_UNSCRAP						0x026B // GLASS UNSCRAP EVENT
#define	B_L2_GLASS_JUDGE						0x026C // GLASS JUDGEMENT EVENT
#define	B_L2_RECP_DOWN							0x026D // RECIPE DOWNLOAD EVENT
#define	B_L2_TO_MANUAL_CELL_LOAD				0x026E // 면취용
#define	B_L2_GLASS_SEND_FAIL					0x026E // GLASS SEND FAIL EVENT // 2010-08-24 add
#define	B_L2_TO_BACK_MODE						0x026F // PHOTO

#define	B_L2_ECID_CHNG_RLY						0x0270 // ECID CHANGE REPLY
#define	B_L2_TEMP_CHNG_RLY						0x0271 // TEMP CHANGE REPLY
#define	B_L2_TANK_CHNG_RLY						0x0272 // TANK CHANGE REPLY
#define	B_L2_ECO_MODE_CHNG_RLY					0x0272 // ECO MODE CHANGE REPLY

#define	B_L2_MNCELL_READ_RLY					0x0273
#define	B_L2_MN_VALID_CHK_REQ					0x0274
#define	B_L2_MN_CANCEL_RLY						0x0275
#define	B_L2_TO_VCR_READ_FAIL_REQ				0x0276 // 면취용
#define	B_L2_MN_JUDGE_RLY						0x0277

#define	B_L2_ECID_CHNG_EVENT					0x0278 // ECID CHANGE EVENT
#define	B_L2_TEMP_CHNG_EVENT					0x0279 // TEMP CHANGE EVENT
#define	B_L2_TANK_CHNG_EVENT					0x027A // TANK CHANGE EVENT
#define	B_L2_ECO_MODE_CHNG_EVENT				0x027A // ECO MODE CHANGE EVENT
#define	B_L2_PROCESS_AVAIL_CHK					0x027B // PROCESS AVAILIBILITY CHECK
#define	B_L2_ENDDATA_CHNG_EVENT					0x027C // PROCESS ENDDATA CHANGE EVENT
#define	B_L2_HS_NR_TIME_OVER_EVENT				0x027D // HANDSHAKE TIME OVER EVENT [FOR NORMAL EQ]
#define	B_L2_HS_BC_TIME_OVER_EVENT				0x027E // HANDSHAKE TIME OVER EVENT [FOR BYPASS EQ ]

#define	B_EQ_EACH_LAYER_INTERVAL				0x0200
/*=============================================   EQ   AREA ======================================================*/



/* Word Area */
/*============================================= MASTER AREA ======================================================*/
// L01 Word CIM
#define	W_L1_ONLINE_STATUS						0x0000 // Online Control Mode (1:Offline, 2:Local, 3:Remote)
#define	W_L1_ALIVE_STATE_DATA					0x0001 // CIM PC Alive State Change Value
#define	W_L1_TOWERLAMP_CONTROL_DATA				0x0002 // All EQ Tower Lamp Control(Binary)
#define	W_L1_TIMESET_SYNC_DATA					0x0003 // 년/월/일/시/분/초
#define	W_L1_CIM_ALIVE_STATE					0x000A // CIM Alive Check (Count No 1 ~ 255)
 
#define	W_L1_MACHINE_CMD_DATA					0x0010 // 1: Pause/Resume, 2: ChangeToPM/ChangeToNormal
#define	W_L1_CLR_ALARM_DATA						0x0011 // Alarm Clear Request ID
#define	W_L1_TERMINAL_MSG_DATA					0x0012 // EQ Terminal Message
#define	W_L1_VCR_MODE							0x003A
#define	W_L1_KEYIN_WAIT_TIME					0x003B

#define W_L1_EQ_CMD_PM_CODE						0x003C
#define W_L1_EQ_CMD_PAUSE_CODE					0x003E

#define	W_L1_ECO_MODE_EOMD_DATA					0x003C	//BoB Sub Mode 1:DI, 2:Chemical or 1:ETCH, 2:STRIP 기타 등등
#define	W_L1_ECO_MODE_EOV_DATA					0x003D	//BoB 0:Working, 1~n:StandbyMode #1 ~ n

#define	W_L1_UNSCRAP_RLY_DATA					0x0040 // Unscrap Glass Data => Each EQ is compare unique id

// L02 Event Reply Data CLN or ETCH
#define	W_L2_FROM_CIM_SCRAP_JUDGE_RLY_DATA		0x0100
#define	W_L2_FROM_CIM_UNSCRAP_RLY_DATA			0x0101
#define	W_L2_FROM_CIM_RCP_DOWN_RLY_DATA			0x0102
#define	W_L2_FROM_CIM_PPID_AVAIL_CHK_RLY_DATA	0x0103
#define	W_L2_FROM_CIM_ECID_CHANGE_RLY_DATA		0x0108
#define	W_L2_FROM_CIM_HP_CHANGE_RLY_DATA		0x0108
#define	W_L2_FROM_CIM_TEMP_CHANGE_RLY_DATA		0x0109
#define	W_L2_FROM_CIM_AP_CHANGE_RLY_DATA		0x0109
#define	W_L2_FROM_CIM_ECO_MODE_CHANGE_RLY_DATA	0x010A
#define	W_L2_FROM_CIM_CP_CHANGE_RLY_DATA		0x010A
#define	W_L2_MN_JUDGE_RLY_DATA					0x010B

#define	W_CIM_EACH_LAYER_INTERVAL				0x0010

#define W_L2_FROM_CIM_ECID_MAPINDEX				0x0104
#define W_L3_FROM_CIM_ECID_MAPINDEX				0x0114


// PHOTO
#define	W_CLN_RECIPE_DATA						0x0200 // Cleaner
#define	W_DBK_RECIPE_DATA						0x0260 // DBK
#define	W_COT_RECIPE_DATA						0x02C0 // Coater
#define	W_VCD_RECIPE_DATA						0x0320 // VCD
#define	W_SBK_RECIPE_DATA						0x0380 // SBK
#define	W_INF_RECIPE_DATA						0x03E0 // INF
#define	W_PEB_RECIPE_DATA						0x0440 // PEB
#define	W_DEV_RECIPE_DATA						0x04A0 // DEV
#define	W_PBK_RECIPE_DATA						0x0500 // PBK

#define	W_PHOTO_EACH_LAYER_RCP_INTERVAL			0x0060

// WET
// MODULE1 : ETCH
#define	W_MD1_RECIPE_DATA						0x0200 // BATH1 START WORD
#define	W_MD1_RB_RECIPE_DATA					0x0380
#define	W_MD1_DB_RECIPE_DATA					0x0394
#define	W_MD1_EB_RECIPE_DATA					0x03A4
#define	W_MD1_R1_RECIPE_DATA					0x03C0
#define	W_MD1_R2_RECIPE_DATA					0x03C9
#define	W_MD1_R3_RECIPE_DATA					0x03D2
#define	W_MD1_DEFAULT_RECIPE_DATA				0x03DF
// MODULE2 : STRIP
#define	W_MD2_RECIPE_DATA						0x03F4 // BATH1 START WORD 
#define	W_MD2_RB_RECIPE_DATA					0x0570
#define	W_MD2_DB_RECIPE_DATA					0x0584
#define	W_MD2_EB_RECIPE_DATA					0x0594
#define	W_MD2_R1_RECIPE_DATA					0x05B0
#define	W_MD2_R2_RECIPE_DATA					0x05B9
#define	W_MD2_R3_RECIPE_DATA					0x05C2
#define	W_MD2_DEFAULT_RECIPE_DATA				0x05CF

#define	W_WET_EACH_BATH_RCP_INTERVAL			0x0040 // BATH1~BATH6
/*============================================= MASTER AREA ======================================================*/

/*=============================================   EQ   AREA ======================================================*/
// L02 ~ L10
// EVENT REPORT TO MASTER
#define	W_L2_EQ_STATE							0x07D0 // MODULE EQ STATE
#define	W_L2_EQP_STATE							0x07D1 // MODULE EQ PROCESS STATE
#define W_L2_PLC_ALIVE_STATE					0x07D6 // PLC Alive Check (Count No 1 ~ 255) 
#define W_L2_PROCESS_MODE						0x07D7 // 설비 진행 모드(RW 용)
//#define W_L2_CURRENT_USING_TANK_NO				0x07D8 // Current Using Tank No  2009-05-19 KWY
#define W_L2_CURRENT_USING_TANK_NO				0x07DA // Current Using Tank No 

#define	W_L2_GLS_TRANS_DATA_TO_MASTER			0x07E0 // JUDGEMENT & SCRAP GLASS DATA
#define	W_L2_UNSCRAP_UNIQID						0x08A8 // UNSCRAP GLASS UNIQUE ID
#define	W_L2_PROC_AVAIL_CHK_PPID				0x08AA // PROCESS AVAILIBILITY CHECK PPID
#define	W_L2_PPID_TRANS_DATA					0x08B2 // PPID FOR RECIPE DOWNLOAD

#define W_L2_PANELID_TRANS_DATA					0x090F // HPanelID For RPC

#define	W_L2_OCCUR_ALID							0x08BA // OCCURRED ALARM ID
#define	W_L2_OCCUR_ALCD							0x08BB // OCCURRED ALARM CODE
#define W_L2_OCCUR_ALTEXT						0x08BC // OCCURRED ALARM TEXT
#define	W_L2_TREAT_ALID							0x08E4 // TREATED ALARM ID
#define	W_L2_TREAT_ALCD							0x08E5 // TREATED ALARM CODE
#define W_L2_TREAT_ALTEXT						0x08E6 // TREATED ALARM TEXT
//================================================================================================
/*
#define	W_L2_VCR_READ_FAIL_DATA					0x08BE // 면취 전용
#define	W_L2_PM_CODE							0x08C0 // PM CODE
#define	W_L2_PAUSE_CODE							0x08C2 // PAUSE CODE
//#define	W_L2_EPD_VALUE							0x08C4 // EPD Data
#define	W_L2_MANUAL_LOAD_N_VCR_HPANEL_ID		0x08CA // 면취 전용

#define W_L2_HS_VALID_DATA_UPPER				0x092F

#define	W_L2_HS_TIME_OVER_EVENT					0x08DC

#define	W_L2_ECO_MODE_EOMD_EVENT				0x08DD		//BoB Sub Mode 1:DI, 2:Chemical or 1:ETCH, 2:STRIP 기타 등등
#define	W_L2_ECO_MODE_EOV_EVENT					0x08DE		//BoB 0:Working, 1~n:StandbyMode #1 ~ n


// EPD Time Data ohanaya 2011.06
#define	W_L2_EPD_TIME_EVENT					    0x08F7 // ~ 0x08F9 
#define	W_L2_EPD_VALUE							0x08FA // EPD Data  주소변경 ohanaya 2011.06


#define W_L2_HS_VALID_FAIL_VALUE				0x08ED // 8 WORD

#define	W_L2_MACHINE_CMD_RLY					0x08E0 // ACK(0x41), NAK(0x4E)
#define	W_L2_CLR_ALARM_RLY						0x08E1 // ACK(0x41), NAK(0x4E)
#define	W_L2_MNCEL_VALID_RLY					0x08E2 // ACK(0x41), NAK(0x4E)
#define	W_L2_ECID_CHNG_RLY						0x08E3 // ACK(0x41), NAK(0x4E)
#define	W_L2_TEMP_CHNG_RLY						0x08E4 // ACK(0x41), NAK(0x4E)
#define	W_L2_TANK_CHNG_RLY						0x08E5 // ACK(0x41), NAK(0x4E)
#define	W_L2_ECO_MODE_CHNG_RLY					0x08E5 // ACK(0x41), NAK(0x4E)

#define	W_L2_GLASS_SEND_FAIL_CODE				0x08E6 // 2010-08-24 add
#define	W_L2_GLASS_SEND_FAIL_HPANELID			0x08E7 // 2010-08-24 add < 6WORD>

#define	W_EQ_EACH_LAYER_INTERVAL				0x0200
  */
//=================================================================================================
#define	W_L2_VCR_READ_FAIL_DATA					0x090E // 면취 전용
#define	W_L2_PM_CODE							0x0919 // PM CODE
#define	W_L2_PAUSE_CODE							0x091B // PAUSE CODE
//#define	W_L2_EPD_VALUE							0x08C4 // EPD Data
#define	W_L2_MANUAL_LOAD_N_VCR_HPANEL_ID		0x0923 // 면취 전용

#define W_L2_HS_VALID_DATA_UPPER				0x092F

#define W_L2_GECD1_CRACK_EVENT					0x091D
#define W_L2_GECD2_CRACK_EVENT					0x091F
#define W_L2_GECD3_CRACK_EVENT					0x0921
#define W_L2_GECD4_CRACK_EVENT					0x0923
#define W_L2_GECD5_CRACK_EVENT					0x0925

#define W_L2_MODULE_RUN_DATA					0x0958

#define	W_L2_HS_TIME_OVER_EVENT					0x08DC

#define	W_L2_ECO_MODE_EOMD_EVENT				0x08DD		//BoB Sub Mode 1:DI, 2:Chemical or 1:ETCH, 2:STRIP 기타 등등
#define	W_L2_ECO_MODE_EOV_EVENT					0x08DE		//BoB 0:Working, 1~n:StandbyMode #1 ~ n


// EPD Time Data ohanaya 2011.06
#define	W_L2_EPD_TIME_EVENT					    0x094F // ~ 0x0951 
#define	W_L2_EPD_VALUE							0x0952 // EPD Data


#define W_L2_HS_VALID_FAIL_VALUE				0x08ED // 8 WORD

#define	W_L2_MACHINE_CMD_RLY					0x0939 // ACK(0x41), NAK(0x4E)
#define	W_L2_CLR_ALARM_RLY						0x093A // ACK(0x41), NAK(0x4E)
#define	W_L2_MNCEL_VALID_RLY					0x093B // ACK(0x41), NAK(0x4E)
#define	W_L2_ECID_CHNG_RLY						0x093C // ACK(0x41), NAK(0x4E)
#define	W_L2_TEMP_CHNG_RLY						0x093D // ACK(0x41), NAK(0x4E)
#define	W_L2_TANK_CHNG_RLY						0x093E // ACK(0x41), NAK(0x4E)
#define	W_L2_ECO_MODE_CHNG_RLY					0x08E5 // ACK(0x41), NAK(0x4E)

#define	W_L2_GLASS_SEND_FAIL_CODE				0x093F // 2010-08-24 add
#define	W_L2_GLASS_SEND_FAIL_HPANELID			0x0940 // 2010-08-24 add < 6WORD>

#define	W_EQ_EACH_LAYER_INTERVAL				0x0200


// (S.J.W) 08.01.30
/*============================================= ER Area Format -> W Area Formet ===================================================*/
// 1. Block 0 ~ 31 : 각 Block은 각각의 Module에 해당   -> W device 영역으로 변경
// 2. 각 Block별 Address : 0 ~ 32767				   -> 각 설비별 시작주소 지정
// 3. Address 단위 : Decimal						   -> HEX
/*============================================= ER Area Format -> W Area Formet ===================================================*/


/*============================================= MASTER AREA ======================================================*/
// Code Change Data : PM Code Data, Broken Code Data, Judgement Code Data
// ER0 : CIM MAster Area 고정
//#define	ER0_PM_DATA_CODE						0x3370
//#define	ER0_PM_DATA_DESCRIPT					0x3372
//#define	ER0_BROKEN_DATA_CODE					0x3820
//#define	ER0_BROKEN_DATA_DESCRIPT				0x3822
//#define	ER0_JUDGE_DATA_CODE						0x3CD0
//#define	ER0_JUDGE_DATA_DESCRIPT					0x3CD2
//#define	ER0_CIM_EACH_CODE_DATA_INTERVAL			12		// ????????

// DATA CHANGE SET : ECID IN POSION
#define	W_POS1_ECID1_DEF_SET_DATA				0x4180			// ECID_1 SETTING DATA
#define	W_POS1_ECID1_SLL_SET_DATA				0x4181			// ECID_1 STOP LOWER LIMIT
#define	W_POS1_ECID1_SUL_SET_DATA				0x4182			// ECID_1 STOP UPPER LIMIT
#define	W_POS1_ECID1_WLL_SET_DATA				0x4183			// ECID_1 WARNING LOWER LIMIT
#define	W_POS1_ECID1_WUL_SET_DATA				0x4184			// ECID_1 WARNING UPPER LIMIT

// DATA CHANGE SET : TEMP OF TANK
#define	W_TANK1_ECID_TEMP_DEF_SET_DATA			0x4B44			// Tank1 온도 설정값
#define	W_TANK1_ECID_TEMP_SLL_SET_DATA			0x4B45			// Tank1 온도 STOP 상한알람 설정값
#define	W_TANK1_ECID_TEMP_SUL_SET_DATA			0x4B46			// Tank1 온도 STOP 하한알람 설정값
#define	W_TANK1_ECID_TEMP_WLL_SET_DATA			0x4B47			// Tank1 온도 WARNING 상한알람 설정값
#define	W_TANK1_ECID_TEMP_WUL_SET_DATA			0x4B48			// Tank1 온도 WARNING 하한알람 설정값
/*============================================= MASTER AREA ======================================================*/

#define ADDR_OFFSET_CLN							0x05188		
#define ADDR_OFFSET_DBK							0x06E88
#define ADDR_OFFSET_COT							0x08B88
#define ADDR_OFFSET_VCD							0x0A888
#define ADDR_OFFSET_SBK							0x0C588
#define ADDR_OFFSET_INF							0x0E288
#define ADDR_OFFSET_PEB							0x0FF88
#define ADDR_OFFSET_DEV							0x11C88
#define ADDR_OFFSET_PBK							0x13988

///////////////////////////////////////////////////////////////////////////////////

// EQ POSITION DATA
#define	W_POS1_EQ_STATE							0x0000				
#define	W_POS1_EQP_STATE						0x0001				
#define	W_POS1_GLS_UNIQID						0x0002				
#define	W_POS1_GLS_JUDGE						0x0004				
#define	W_POS1_GLS_STATE						0x0006				
#define	W_POS1_RECP_NO							0x0007

// DATA CHANGE SET : ECID IN POSION
#define	W_POS1_ECID1_DEF_DATA					0x04BA			// (552A) ECID_1 SETTING DATA
#define	W_POS1_ECID1_SLL_DATA					0x04BB			// (552B) ECID_1 STOP LOWER LIMIT
#define	W_POS1_ECID1_SUL_DATA					0x04BC			// (552C) ECID_1 STOP UPPER LIMIT
#define	W_POS1_ECID1_WLL_DATA					0x04BD			// (552D) ECID_1 WARNING LOWER LIMIT
#define	W_POS1_ECID1_WUL_DATA					0x04BE			// (552E) ECID_1 WARNING UPPER LIMIT

#define W_POS1_ECID_SINGLE_DATA					0x0A96

// DATA CHANGE SET : TEMP OF TANK
#define	W_TANK1_ECID_TEMP_DEF_DATA				0x0E7E			// Tank1 온도 설정값
#define	W_TANK1_ECID_TEMP_SLL_DATA				0x0E7F			// Tank1 온도 STOP 상한알람 설정값
#define	W_TANK1_ECID_TEMP_SUL_DATA				0x0E80			// Tank1 온도 STOP 하한알람 설정값
#define	W_TANK1_ECID_TEMP_WLL_DATA				0x0E81			// Tank1 온도 WARNING 상한알람 설정값
#define	W_TANK1_ECID_TEMP_WUL_DATA				0x0E82			// Tank1 온도 WARNING 하한알람 설정값

// CURRENT DATA : SVID IN POSION
#define	W_POS1_SVID1							0x01F4			// 33268 // POSITION 1~16, SVID 1~10

// PROCESS ENDDATA
#define	W_ENDD_GLS_UNIQID						0x0F78			
#define	W_ENDD_TOTAL_PROC_TIME					0x0F7A			
#define	W_ENDD_PROC_TIME						0x0F7B			
#define	W_ENDD_PROC_GLS_COUNT					0x0F81			
#define	W_ENDD_PROC_RECP_NO						0x0F82				
#define	W_ENDD_USED_TANK_NO						0x0F83				
#define	W_ENDD_TANK_USED_TIME					0x0F84				
#define	W_ENDD_UV_USED_TIME						0x0F85				

#define	W_ENDD_POS1_DATA						0x0FAA			
#define	W_ENDD_MSC_DATA							0x104A			
#define	W_ENDD_TANK1_TEMP_DATA					0x119E			

#define	W_ENDD_TANK1_CHANGE_LIFE_COUNT			0x11A8			
#define	W_ENDD_TANK1_SUPPLY_LIFE_TIME			0x11A9			
#define	W_ENDD_TANK1_SUPPLY_LIFE_COUNT			0x11AA			

#define	W_ENDD_TANK1_ACTUAL_GLASS_COUNT			0x11AC			
#define	W_ENDD_TANK1_POSI_LIFE_CNT_OFFSET		0x11AD			
#define	W_ENDD_TANK1_NEGA_LIFE_CNT_OFFSET		0x11AE			
#define	W_ENDD_TANK1_POSI_LIFE_TIME_OFFSET		0x11AF			
#define	W_ENDD_TANK1_NEGA_LIFE_TIME_OFFSET		0x11B0	

#define W_POS1_ACTION_LOG_DATA					0x1290		// shseo 2010_1007 - ADD 

#define UNIQUEID    0
#define JOBORDER    1
#define SLOTNO      2
#define PORTNO      3

#endif