// SECSecs.h : header file
//
#ifndef _SECSECS_H_
#define _SECSECS_H_

#pragma once
#include "excom.h"
#include "HListBox.h"
#include "C7Host.h"
#include "C7Define.h"
#include "C7Queue.h"

void Send_S1F1(stC7QueueType *pSig);
void RecvReply_S1F2(long lMsgId);
void Recv_S1F1(long lMsgId);
void SendReply_S1F2(stC7QueueType *pSig);
void Recv_S1F3(long lMsgId);
void SendReply_S1F4(stC7QueueType *pSig);
void Recv_S1F5(long lMsgId);
void SendReply_S1F6EqOnlineParam(long nEventId, stC7QueueType *pSig);
void SendReply_S1F6PortStates(long nEventId, stC7QueueType *pSig);
void SendReply_S1F6GlassTracking(long nEventId, stC7QueueType *pSig);
void SendReply_S1F6ModuleStates(long nEventId, stC7QueueType *pSig);
void Recv_S1F11(long lMsgId);
void SendReply_S1F12(stC7QueueType *pSig);
void Recv_S1F15(long lMsgId);
void SendReply_S1F16(stC7QueueType *pSig);
void Recv_S1F17(long lMsgId);
void SendReply_S1F18(stC7QueueType *pSig);
void Recv_S2F15(long lMsgId);
void SendReply_S2F16(stC7QueueType *pSig);
void Recv_S2F23(long lMsgId);
void SendReply_S2F24(stC7QueueType *pSig);
void Recv_S2F29(long lMsgId);
void SendReply_S2F30(stC7QueueType *pSig);
void Recv_S2F31(long lMsgId);
void SendReply_S2F32(stC7QueueType *pSig);
void Recv_S2F41ProcessCmd(long lMsgId, long lRcmd);
void Recv_S2F41PortCmd(long lMsgId, long lRcmd);
void Recv_S2F41EQCmd(long lMsgId, long lRcmd);
void SendReply_S2F42ProcessCmdAck(stC7QueueType *pSig);
void SendReply_S2F42PortCmdAck(stC7QueueType *pSig);
void SendReply_S2F42EQCmdAck(stC7QueueType *pSig);
void Recv_S2F101(long lMsgId);
void SendReply_S2F102(stC7QueueType *pSig);
void Recv_S2F103(long lMsgId);
void SendReply_S2F104(stC7QueueType *pSig);
void Recv_S3F101(long lMsgId);
void SendReply_S3F102(stC7QueueType *pSig);
void Send_S5F1AlarmReport(stC7QueueType *pSig);
void RecvReply_S5F2(long lMsgId);
void Recv_S5F101(long lMsgId);
void SendReply_S5F102(stC7QueueType *pSig);
void Send_S6F1(stC7QueueType *pSig);
void RecvReply_S6F2(long lMsgId);
void Send_S6F3(stC7QueueType *pSig);
void RecvReply_S6F4(long lMsgId);
void Send_S6F11ProcessEvent(long nEventId, stC7QueueType *pSig);
void Send_S6F11GlassEvent(long nEventId, stC7QueueType *pSig);
void Send_S6F11PortEvent(long nEventId, stC7QueueType *pSig);
void Send_S6F11EquipmentEvent(long nEventId, stC7QueueType *pSig);
void Send_S6F11EQParamEvent(long nEventId, stC7QueueType *pSig);
void Send_S6F11MaterialEvent(long nEventId, stC7QueueType *pSig);
void RecvReply_S6F12(long lMsgId);
void Send_S6F13DataCollection(stC7QueueType *pSig);
void RecvReply_S6F14(long lMsgId);
void Recv_S7F23(long lMsgId);
void SendReply_S7F24(stC7QueueType *pSig);
void Recv_S7F25(long lMsgId);
void SendReply_S7F26(stC7QueueType *pSig);
void Recv_S7F101(long lMsgId);
void SendReply_S7F102(stC7QueueType *pSig);
void Recv_S7F103(long lMsgId);
void SendReply_S7F104(stC7QueueType *pSig);
void Recv_S7F105(long lMsgId);
void SendReply_S7F106(stC7QueueType *pSig);
void Send_S7F107(stC7QueueType *pSig);
void RecvReply_S7F108(long lMsgId);
void Recv_S7F109(long lMsgId);
void SendReply_S7F110(stC7QueueType *pSig);
void Recv_S9F11(long lMsgId);
void Recv_S10F3(long lMsgId);
void SendReply_S10F4(stC7QueueType *pSig);
void Recv_S10F9(long lMsgId);
void SendReply_S10F10(stC7QueueType *pSig);
void Send_AbortStreamFunction(long nStrm, long lSysByte);

void FillSpace(CString *str, int Len);
void OverFlowToU1(short *n_pU1);
/*
//Common Data
#define	MAX_MDLN_LEN			6
#define	MAX_ONLINEMODE_LEN		6
#define	MAX_SOFTREV_LEN			6
#define	MAX_TOOLID_LEN			9
#define	MAX_UNITID_LEN			9
#define	MAX_SV_LEN				20
#define	MAX_SVNAME_LEN			40
#define	MAX_ECV_LEN				20
#define	MAX_DATETIME_LEN		14
#define	MAX_ECNAME_LEN			40
#define	MAX_ECMIN_LEN			20
#define	MAX_ECMAX_LEN			20
#define	MAX_ECDEF_LEN			20
#define	MAX_TEXT_LEN			80
#define	MAX_DSPER_LEN			6
#define	MAX_MNAME_LEN			40	// C6 Project 변경 ( 20 -> 40 )
#define	MAX_MVALUE_LEN			40	// C6 Project 변경 ( 20 -> 40 )
#define	MAX_DCOLLNAME_LEN		16
#define	MAX_DCOLLVALUE_LEN		16
#define	MAX_UNLOADTYPE_LEN		2
#define	MAX_SPLITMODE_LEN		4
#define	MAX_PORTMODE_LEN		3
#define	MAX_RESULT_LEN			6
#define	MAX_JUDGEMENT_LEN		6
#define	MAX_NGCODE_LEN			6
#define	MAX_CPNAME_LEN			10
#define	MAX_TABLENAME_LEN		16
#define	MAX_PARAMNAME_LEN		16
#define	MAX_PARAMVALUE_LEN		16

//Job Data
#define	MAX_PORTID_LEN			2
#define	MAX_CARID_LEN			16
#define	MAX_JOBID_LEN			20
#define	MAX_STIF_LEN			20
#define	MAX_TOTALGSTATE_LEN		20

//Glass Data
#define	MAX_SLOTNO_LEN			2
#define	MAX_PROCESSID_LEN		20
#define	MAX_PARTID_LEN			20
#define	MAX_STEPID_LEN			20
#define	MAX_GLASSTYPE_LEN		2
#define	MAX_LOTID_LEN			16
#define	MAX_GLASSID_LEN			20
#define	MAX_PPID_LEN			16
#define	MAX_CELLGRADE_LEN		20
#define	MAX_LOTACTION_LEN		16
*/
#endif // _SECSECS_H_
