#ifndef	_C7IndIOMap_H_
#define	_C7IndIOMap_H_

///////////////////////////////////////////////////
//	Indexer -> EQ	Bit Data
//	Indexer Glass Unloading Handshake & Arm Status Bit
//	
#define MAX_HANDSHAKE_BIT_COUNT			32
#define MAX_CONTACT_BIT_COUNT			16
#define	MAX_EQ_AND_PROC_STATE_BIT_COUNT	16
#define	MAX_WAIT_ROBOT_STATE_BIT_COUNT	4
#define MAX_ROBOT_OPMODE_BIT_COUNT		4
#define	MAX_ROBOT_RUN_STATUS_BIT_COUNT	5
#define	MAX_PORT_INFORMATION_BIT_COUNT	6
#define	MAX_OPCALL_BUSY_BIT_COUNT		1
#define	MAX_MACHINE_COMMAND_BIT_COUNT	4
#define	MAX_GLASS_MOVE_CMD_BIT_COUNT	1
#define	MAX_WAIT_POS_CHANGE_BIT_COUNT	1
#define	MAX_PORT_COMMAND_BIT_COUNT		16
#define	MAX_SPECIAL_COMMAND_BIT_COUNT	5
#define	MAX_ETC_EVENT_BIT_COUNT			1		//	T7-2 
#define	MAX_ROBOT_ARM_STATE_BIT_COUNT	8		//	T7-2 

#define	MAX_ALARM_DATA_WORD_LEN			22

//	Indexer Hand Shake tU1
#define	B_IND_RECV_MACHINE_RUNNING		0x0000	//	0x0000 ~ 0x001F : Indexer Recv Handshake PIO
#define	B_IND_RECV_ROBOT_CONTACT		0x0020	//	0x0020 ~ 0x002F	: Contact Point

//	Indexer Hand Shake tL1
#define	B_IND_SEND_MACHINE_RUNNING		0x0090	//	0x0090 ~ 0x00AF	: Indexer Send Handshake PIO
#define	B_IND_SEND_ROBOT_CONTACT		0x00B0	//	0x00B0 ~ 0x00BF	: Contact Point

//	Indexer Equipment State
#define	B_EQ_STATE_NORMAL				0x0120	//	0x0120 ~ 0x0122 : EQ. State
#define	B_EQ_STATE_FAULT				0x0121
#define	B_EQ_STATE_PM					0x0122

//	Indexer Process State
#define	B_EQ_PROCESS_INITIAL			0x0128	//	0x0128 ~ 0x002D : EQ. Process State
#define	B_EQ_PROCESS_IDLE				0x0129
#define	B_EQ_PROCESS_SETUP				0x012A
#define	B_EQ_PROCESS_READY				0x012B
#define	B_EQ_PROCESS_EXECUTING			0x012C
#define	B_EQ_PROCESS_PAUSE				0x012D

//	Indexer Robot Wait Position Mode
#define	B_ROBOT_GLASS_TAKE_MODE			0x0130

//	Indexer Robot Arm Glass Check
#define	B_ROBOT_ARM_GLASS_CONTAIN		0x0132
#define	B_ROBOT_ARM_GLASS_WITHOUT		0x0133

//	Indexer Machine Status Bits
#define	B_ROBOT_OPMODE_INITIALIZING		0x0140
#define	B_ROBOT_OPMODE_MANUAL_OP		0x0141
#define	B_ROBOT_OPMODE_EMERGENCY		0x0142
#define	B_ROBOT_OPMODE_ABNORMAL			0x0143

//	Indexer Machine Status Bits
#define	B_ROBOT_STATUS_IDLE				0x0150
#define	B_ROBOT_STATUS_WAIT				0x0151
#define	B_ROBOT_STATUS_BUSY				0x0152
#define	B_ROBOT_STATUS_COMPLETE			0x0153
#define	B_ROBOT_STATUS_ERROR			0x0154

//	Indexer Port Status
//	Indexer Port Mapping Status : 동작중 ON
#define	B_PORT1_MAPPING_STATUS			0x0160
#define	B_PORT2_MAPPING_STATUS			0x0161
#define	B_PORT3_MAPPING_STATUS			0x0162
#define	B_PORT4_MAPPING_STATUS			0x0163

//	Cassette Contain Status Bits
#define	B_PORT1_CASSETTE_CONTAINED		0x0170
#define	B_PORT2_CASSETTE_CONTAINED		0x0171
#define	B_PORT3_CASSETTE_CONTAINED		0x0172
#define	B_PORT4_CASSETTE_CONTAINED		0x0173

//	Cassette Chucking Status Bits
#define	B_PORT1_CASSETTE_CHUCKED		0x0180
#define	B_PORT2_CASSETTE_CHUCKED		0x0181
#define	B_PORT3_CASSETTE_CHUCKED		0x0182
#define	B_PORT4_CASSETTE_CHUCKED		0x0183

//	STK/AGV COMMUNICATION Status Bits
#define	B_PORT1_STK_AGV_COMMUNICATION	0x0190
#define	B_PORT2_STK_AGV_COMMUNICATION	0x0191
#define	B_PORT3_STK_AGV_COMMUNICATION	0x0192
#define	B_PORT4_STK_AGV_COMMUNICATION	0x0193

//	Indexer Reply Bits
//	Machine Command Reply Bits
#define	B_MACHINE_COMMAND_RLY			0x01B0
#define	B_ALARM_CLEAR_RLY				0x01B1
#define	B_TERMINAL_MESSAGE_RLY			0x01B2
#define	B_OPERATOR_CALL_RLY				0x01B3

//	Indexer Robot Wait Position Reply Bits
#define	B_WAIT_POS_CHANGE_RLY			0x01B8

//	Indexer Glass Handling Reply Bits
//	Robot Arm Glass Handling
#define	B_GLASS_CYCLE_MOVE_RLY			0x01C0
#define	B_GLASS_CANCEL_RLY				0x01C4

//	Indexer Port Command Reply Bits
#define	B_PROCESS_RESERVED_RLY			0x01D0
#define	B_PROCESS_START_RLY				0x01D1
#define	B_PROCESS_COMPLETE_RLY			0x01D2
#define	B_PROCESS_CANCEL_RLY			0x01D3
#define	B_PROCESS_ABORT_RLY				0x01D4
#define	B_SPECIFY_PORT_DISABLE_RLY		0x01D5
#define	B_STK_AGV_ABORT_RLY				0x01D6
#define	B_PORT_PAUSE_RLY				0x01D7
#define	B_PORT_MODE_CHANGE_RLY			0x01D8
#define	B_PORT_REMAPPING_RLY			0x01D9
#define	B_PORT_GLASSSIZE_CHANGE_RLY		0x01DA
#define	B_PORT_GLASSTHICK_CHANGE_RLY	0x01DB
#define	B_PORT_CHUCK_RETRY_RLY			0x01DC
#define	B_PORT_UNCHUCK_RLY				0x01DD
#define	B_PORT_CASSETTE_ID_RLY			0x01DE
#define	B_PORT_BCR_MODE_CHANGE_RLY		0x01DF

//	Indexer Special Command Reply Bits
#define	B_SPECIAL_COMMAND1_RLY			0x01E0
#define	B_SPECIAL_COMMAND2_RLY			0x01E1
#define	B_SPECIAL_COMMAND3_RLY			0x01E2
#define	B_SPECIAL_COMMAND4_RLY			0x01E3
#define	B_SPECIAL_COMMAND5_RLY			0x01E4

//	Indexer Port Event Report Bits
#define	B_PORT1_EVENT_REPORT			0x01F0
#define	B_PORT2_EVENT_REPORT			0x01F1
#define	B_PORT3_EVENT_REPORT			0x01F2
#define	B_PORT4_EVENT_REPORT			0x01F3

//	Indexer Cassette ID Read Retry Bits
#define	B_ALARM_EVENT_REPORT			0x0200

///////////////////////////////////////////////////
//	EQ <- Indexer	Bit : EQ ENTRANCE 
#define	B_EQ_RECV_MACHINE_RUNNING		0x0300	// RESERVED_1
#define	B_EQ_RECV_MACHINE_PAUSE			0x0301
#define	B_EQ_RECV_MACHINE_DOWN			0x0302
#define	B_EQ_RECV_MACHINE_ALARM			0x0303
#define	B_EQ_RECV_RECEIVE_ABLE			0x0304
#define	B_EQ_RECV_RECEIVE_START			0x0305
#define	B_EQ_RECV_RECEIVE_COMPLETE		0x0306
#define	B_EQ_RECV_EXCHANGE_FLAG			0x0307
#define	B_EQ_RECV_RET_SEND_START		0x0308
#define	B_EQ_RECV_RET_SEND_COMPLETE		0x0309
#define	B_EQ_RECV_EMG_PAUSE_REQ			0x030A
#define	B_EQ_RECV_EMG_STOP_REQ			0x030B
#define	B_EQ_RECV_ABLE_REM_1			0x030C
#define	B_EQ_RECV_ABLE_REM_2			0x030D
#define	B_EQ_RECV_ABLE_REM_3			0x030E
#define	B_EQ_RECV_ABLE_REM_4			0x030F
#define	B_EQ_RECV_GLSID_READ_CMPL		0x0310
#define	B_EQ_RECV_LOADING_STOP			0x0311
#define	B_EQ_RECV_TRANSFER_STOP			0x0312
#define	B_EQ_RECV_RESERVED_2			0x0313
#define	B_EQ_RECV_RESERVED_3			0x0314
#define	B_EQ_RECV_RESERVED_4			0x0315
#define	B_EQ_RECV_RESERVED_5			0x0316
#define	B_EQ_RECV_RESERVED_6			0x0317
#define	B_EQ_RECV_HS_CANCEL_REQ_RLY		0x0318
#define	B_EQ_RECV_HS_ABORT_REQ_RLY		0x0319
#define	B_EQ_RECV_HS_RESUME_REQ_RLY		0x031A
#define	B_EQ_RECV_RECOVERY_ACK_RLY		0x031B
#define	B_EQ_RECV_RECOVERY_NAK_RLY		0x031C
#define	B_EQ_RECV_RESERVED_7			0x031D
#define	B_EQ_RECV_RESERVED_8			0x031E
#define	B_EQ_RECV_RESERVED_9			0x031F

//	In Conveyer Status Bit
#define	B_IN_CONV_ABNORMAL				0x0320
#define	B_IN_ARM_TYPE					0x0321	//	NORMAL OFF
#define	B_IN_CONV_TYPE					0x0322	//	NORMAL ON
#define	B_IN_CONV_EMPTY					0x0323
#define	B_IN_CONV_IDLE					0x0324
#define	B_IN_CONV_BUSY					0x0325
#define	B_IN_CONV_COMPLETE				0x0326
#define	B_IN_CONV_LIFT_UP				0x0327
#define	B_IN_CONV_LIFT_DOWN				0x0328
#define	B_IN_CONV_STOPPER_UP			0x0329
#define	B_IN_CONV_STOPPER_DOWN			0x032A
#define	B_IN_CONV_GLASS_CHK_SENSOR_ON	0x032B
#define	B_IN_CONV_MANUAL_OPERATION		0x032C
#define	B_IN_CONV_EMERGENCY				0x032D
#define	B_IN_CONV_BODY_MOVING			0x032E
#define	B_IN_CONV_RESERVED_1			0x032F

#define	B_EQ_RECV_MACHINE_RUNNING2		0x0330	// RESERVED_1

//	EQ -> Indexer	Bit : EQ EXIT
#define	B_EQ_SEND_MACHINE_RUNNING		0x03C0
#define	B_EQ_SEND_MACHINE_PAUSE			0x03C1
#define	B_EQ_SEND_MACHINE_DOWN			0x03C2
#define	B_EQ_SEND_MACHINE_ALARM			0x03C3
#define	B_EQ_SEND_SEND_ABLE				0x03C4
#define	B_EQ_SEND_SEND_START			0x03C5
#define	B_EQ_SEND_SEND_COMPLETE			0x03C6
#define	B_EQ_SEND_EXCHANGE_FLAG			0x03C7
#define	B_EQ_SEND_RET_RECEIVE_START		0x03C8
#define	B_EQ_SEND_RET_RECEIVE_COMPLETE	0x03C9
#define	B_EQ_SEND_EMG_PAUSE_REQ			0x03CA
#define	B_EQ_SEND_EMG_STOP_REQ			0x03CB
#define	B_EQ_SEND_SEND_ABLE_REM_1		0x03CC
#define	B_EQ_SEND_SEND_ABLE_REM_2		0x03CD
#define	B_EQ_SEND_SEND_ABLE_REM_3		0x03CE
#define	B_EQ_SEND_SEND_ABLE_REM_4		0x03CF
#define	B_EQ_SEND_WORK_START			0x03D0
#define	B_EQ_SEND_WORK_CANCEL			0x03D1
#define	B_EQ_SEND_WORK_SKIP				0x03D2
#define	B_EQ_SEND_JOB_START				0x03D3
#define	B_EQ_SEND_JOB_END				0x03D4
#define	B_EQ_SEND_HOT_FLOW				0x03D5
#define	B_EQ_SEND_RESERVED_2			0x03D6
#define	B_EQ_SEND_RESERVED_3			0x03D7
#define	B_EQ_SEND_HS_CANCEL_REQ_RLY		0x03D8
#define	B_EQ_SEND_HS_ABORT_REQ_RLY		0x03D9
#define	B_EQ_SEND_HS_RESUME_REQ_RLY		0x03DA
#define	B_EQ_SEND_HS_RECOVERY_ACK_RLY	0x03DB
#define	B_EQ_SEND_HS_RECOVERY_NAK_RLY	0x03DC
#define	B_EQ_SEND_RESERVED_4			0x03DD
#define	B_EQ_SEND_RESERVED_5			0x03DE
#define	B_EQ_SEND_RESERVED_6			0x03DF

//	Out Conveyer Status Bit
#define	B_OUT_CONV_ABNORMAL				0x03E0
#define	B_OUT_ARM_TYPE					0x03E1	//	NORMAL OFF
#define	B_OUT_CONV_TYPE					0x03E2	//	NORMAL ON
#define	B_OUT_CONV_EMPTY				0x03E3
#define	B_OUT_CONV_IDLE					0x03E4
#define	B_OUT_CONV_BUSY					0x03E5
#define	B_OUT_CONV_COMPLETE				0x03E6
#define	B_OUT_CONV_LIFT_UP				0x03E7
#define	B_OUT_CONV_LIFT_DOWN			0x03E8
#define	B_OUT_CONV_STOPPER_UP			0x03E9
#define	B_OUT_CONV_STOPPER_DOWN			0x03EA
#define	B_OUT_CONV_GLASS_CHK_SENSOR_ON	0x03EB
#define	B_OUT_CONV_MANUAL_OPERATION		0x03EC
#define	B_OUT_CONV_EMERGENCY			0x03ED
#define	B_OUT_CONV_BODY_MOVING			0x03EE
#define	B_OUT_CONV_RESERVED_1			0x03EF

#define	B_EQ_SEND2_MACHINE_RUNNING		0x03F0

//	EQ Machine Command Bit
#define	B_MACHINE_COMMAND_REQ			0x0480
#define	B_ALARM_CLEAR_REQ				0x0481
#define	B_TERMINAL_MESSAGE_REQ			0x0482
#define	B_OPERATOR_CALL_REQ				0x0483

//	Robot Wait Position Change Command Bit
#define	B_WAIT_POSITION_CHANGE_REQ		0x0488
#define	B_ARM_DATA_CLEAR_REQ			0x048C

//	Glass Handle Command Bit
#define	B_GLASS_CYCLE_MOVE_REQ			0x0490
#define	B_GLASS_STEP_MOVE_REQ			0x0491
#define	B_GLASS_CANCEL_REQ				0x0498

//	Port Command Bit
#define	B_PROCESS_RESERVED_REQ			0x04A0
#define	B_PROCESS_START_REQ				0x04A1
#define	B_PROCESS_COMPLETE_REQ			0x04A2
#define	B_PROCESS_CANCEL_REQ			0x04A3
#define	B_PROCESS_ABORT_REQ				0x04A4
#define	B_SPECIFY_PORT_DISABLE_REQ		0x04A5
#define	B_STK_AGV_ABORT_REQ				0x04A6
#define	B_PORT_PAUSE_REQ				0x04A7
#define	B_PORT_MODE_CHANGE_REQ			0x04A8
#define	B_PORT_REMAPPING_REQ			0x04A9
#define	B_PORT_GLASSSIZE_CHANGE_REQ		0x04AA
#define	B_PORT_GLASSTHICK_CHANGE_REQ	0x04AB
#define	B_CHUCK_RETRY_REQ				0x04AC
#define	B_UNCHUCK_REQ					0x04AD
#define	B_PORT_CASSETTE_ID_REQ			0x04AE
#define	B_PORT_BCR_MODE_CHANGE_REQ		0x04AF

//	Special Command Bit
#define	B_SPECIAL_COMMAND1_REQ			0x04B0
#define	B_SPECIAL_COMMAND2_REQ			0x04B1
#define	B_SPECIAL_COMMAND3_REQ			0x04B2
#define	B_SPECIAL_COMMAND4_REQ			0x04B3
#define	B_SPECIAL_COMMAND5_REQ			0x04B4

//	Port Event Reply Bit
#define	B_PORT1_EVENT_RLY				0x04C0
#define	B_PORT2_EVENT_RLY				0x04C1
#define	B_PORT3_EVENT_RLY				0x04C2
#define	B_PORT4_EVENT_RLY				0x04C3

//	Indexer Alarm Event Reply
#define	B_ALARM_EVENT_RLY				0x04D0


///////////////////////////////////////////////////
//	Indexer -> EQ	Word Data
//	Indexer Wait Position Data
#define	W_ROBOT_WAIT_POSITION			0x0240

//	Indexer Port Status Data
#define	W_PORT1_STATUS					0x0242
#define	W_PORT2_STATUS					0x0243
#define	W_PORT3_STATUS					0x0244
#define	W_PORT4_STATUS					0x0245

//	Indexer Port Mode Data
#define	W_PORT1_MODE					0x0252
#define	W_PORT2_MODE					0x0253
#define	W_PORT3_MODE					0x0254
#define	W_PORT4_MODE					0x0255

//	Indexer Port Glass Size Data
#define	W_PORT1_GLASS_SIZE				0x0262
#define	W_PORT2_GLASS_SIZE				0x0264
#define	W_PORT3_GLASS_SIZE				0x0266
#define	W_PORT4_GLASS_SIZE				0x0268

//	Indexer Port Glass Thickness 
#define	W_PORT1_CASSETTE_TYPE			0x0282
#define	W_PORT2_CASSETTE_TYPE			0x0283
#define	W_PORT3_CASSETTE_TYPE			0x0284
#define	W_PORT4_CASSETTE_TYPE			0x0285

//	Indexer Port Mapping Data
#define	W_PORT1_SLOT_INFO				0x0292
#define	W_PORT2_SLOT_INFO				0x0297
#define	W_PORT3_SLOT_INFO				0x029C
#define	W_PORT4_SLOT_INFO				0x02A1

//	Indexer Cassette ID Data
#define	W_PORT1_CASSETTE_ID				0x02E2
#define	W_PORT2_CASSETTE_ID				0x02E8
#define	W_PORT3_CASSETTE_ID				0x02EE
#define	W_PORT4_CASSETTE_ID				0x02F4

//	Indexer Port BCR Mode Data
#define	W_PORT1_BCR_MODE				0x0342
#define	W_PORT2_BCR_MODE				0x0343
#define	W_PORT3_BCR_MODE				0x0344
#define	W_PORT4_BCR_MODE				0x0345

//	EQP STATE
#define	W_RCODE							0x0352
#define	W_R_ALARM_ID					0x0354
#define	W_R_ALARM_CODE					0x0355

#define	W_ROBOT_CURRENT_POSITION		0x0356
#define	W_ROBOT_ARM_CURRENT_STATE		0x0358


//	Indexer Machine Reply Data
#define	W_MACHINE_COMMAND_RESULT		0x0360
#define	W_ALARM_CLEAR_RESULT			0x0361
#define	W_TERMINAL_MESSAGE_RESULT		0x0362

//	Indexer Wait Position Reply Data
#define	W_ROBOT_WAIT_POS_RESULT			0x0363
#define W_ARM_DATA_CLEAR_RESULT			0x0365

//	Indexer Glass Handling Reply Data
#define	W_GLASS_HANDLING_CMD_RESULT		0x0366

//	Port Command Reply Data
#define	W_PROCESS_RESERVED_RESULT		0x036E
#define	W_PROCESS_START_RESULT			0x036F
#define	W_PROCESS_COMPLETE_RESULT		0x0370
#define	W_PROCESS_CANCEL_RESULT			0x0371
#define	W_PROCESS_ABORT_RESULT			0x0372
#define	W_SPECIFY_PORT_DISABLE_RESULT	0x0373
#define	W_STK_AGV_ABORT_RESULT			0x0374
#define	W_PORT_PAUSE_RESULT				0x0375
#define	W_PORT_MODE_SET_RESULT			0x0376
#define	W_PORT_MAPPING_RESULT			0x0377
#define	W_PORT_GLASSSIZE_CHANGE_RESULT	0x0378
#define	W_PORT_STATUS_WAIT_RESULT		0x0379
#define	W_PORT_CHUCK_RESULT				0x037A
#define	W_PORT_UNCHUCK_RESULT			0x037B
#define	W_PORT_CASSETTE_ID_RESULT		0x037C
#define	W_PORT_BCR_MODE_CHANGE_RESULT	0x037D

//	Indexer Special Command Reply Data
#define	W_SPECIAL_COMMAND1_RESULT		0x037E
#define	W_SPECIAL_COMMAND2_RESULT		0x037F
#define	W_SPECIAL_COMMAND3_RESULT		0x0380
#define	W_SPECIAL_COMMAND4_RESULT		0x0381
#define	W_SPECIAL_COMMAND5_RESULT		0x0382

//	Indexer Port Event Data
#define	W_PORT1_EVENT_DATA				0x0390
#define	W_PORT2_EVENT_DATA				0x0391
#define	W_PORT3_EVENT_DATA				0x0392
#define	W_PORT4_EVENT_DATA				0x0393

//	Indexer ALARM Data
#define	W_ALARM_ID						0x03A0
#define	W_ALARM_CODE					0x03A1
#define	W_ALARM_MODULE					0x03A2
#define	W_ALARM_TEXT					0x03AC


//	EQ -> Indexer	Word Data
//	Machine Command Data
#define	W_MACHINE_COMMAND_DATA			0x0680

//	Clear Alarm Data
#define	W_CLEAR_ALARM_ID_DATA			0x0681

//	Terminal Display Data
#define	W_TERMINAL_DISPLAY_DATA			0x0685	//	40 Words(80 Bytes)

//	Tower Lamp / Buzzer Control
#define	W_TOWER_BUZZER_CONTROL_DATA		0x06AD

//	Robot Wait Position Data
#define	W_WAIT_POSITION_DATA			0x06B0
#define	W_CLEAR_ARM_DATA				0x06B2

//	Robot Arm Glass Handling Data
#define	W_GLASS_HANDLING_DATA			0x06B3	//	4 Words(8 Bytes)

//	Port Command Data
#define	W_PROCESS_RESERVED_DATA			0x06C3
#define	W_PROCESS_START_DATA			0x06C4
#define	W_PROCESS_COMPLETE_DATA			0x06C5
#define	W_PROCESS_CANCEL_DATA			0x06C6
#define	W_PROCESS_ABORT_DATA			0x06C7
#define	W_SPECIFY_DISABLE_DATA			0x06C8	//	2 Words(4 Bytes)
#define	W_STK_AGV_ABORT_DATA			0x06CA
#define	W_PORT_PAUSE_DATA				0x06CB
#define	W_PORT_MODE_CHANGE_DATA			0x06CC	//	2 Words(4 Bytes)
#define	W_PORT_MAPPING_DATA				0x06CE
#define	W_PORT_GLASSSIZE_CHANGE_DATA	0x06CF	//	2 Words(4 Bytes)
#define	W_PORT_GLASSTHICK_CHANGE_DATA	0x06D1	//	2 Words(4 Bytes)
#define	W_PORT_CHUCK_DATA				0x06D3
#define	W_PORT_UNCHUCK_DATA				0x06D4
#define	W_PORT_CASSETTE_ID_DATA			0x06D5
#define	W_PORT_BCR_MODE_CHANGE_DATA		0x06D6	//	2 Words(4 Bytes)
#define	W_ARM_GLASS_THICKNESS_DATA		0x06D8	//	2 Words(4 Bytes)

//	Special Command Data
#define	W_SPECIAL_COMMAND1_DATA			0x06DC	// Arm No (Only One)

#define W_ARM_GLASS_SIZE_DATA			0x06DE

//	Port Event Result Data
#define	W_PORT1_EVENT_RESULT_DATA		0x06F0
#define	W_PORT2_EVENT_RESULT_DATA		0x06F1
#define	W_PORT3_EVENT_RESULT_DATA		0x06F2
#define	W_PORT4_EVENT_RESULT_DATA		0x06F3


#define	MAX_DATA_SIZE_1					2		//	단위 Word (2 Bytes)
#define	MAX_DATA_SIZE_2					4
#define	MAX_DATA_SIZE_3					6
#define	MAX_DATA_SIZE_4					8
#define	MAX_DATA_SIZE_5					10
#define	MAX_DATA_SIZE_6					12
#define	MAX_DATA_SIZE_10				20
#define	MAX_DATA_SIZE_20				40
#define	MAX_DATA_SIZE_40				80
#endif	// _IndIOMap_H_