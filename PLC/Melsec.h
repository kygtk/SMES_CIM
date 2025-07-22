// Melsec.h: interface for the CMelsec class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MELSEC_H__0C4BC213_84E0_11D6_B2A9_0050DA8B0583__INCLUDED_)
#define AFX_MELSEC_H__0C4BC213_84E0_11D6_B2A9_0050DA8B0583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	"MyLog.h"

#define	MAX_STATION_COUNT		64
#define	MAX_BOARD_LED_SIZE		3
#define	MAX_BOARD_SWITCH_SIZE	6
#define	MAX_BOARD_VERSION_SIZE	32


class CMelsec  
{
public:
	CMelsec();
	virtual ~CMelsec();

	CMyLog	m_MelsecLog;

	void  SetConfig(long nChannel, long nNetwork, short shSourceStNo, short shTargetStNo);

	short MelNetOpen		();
	short MelNetClose		();
	short MelNetSend		(short shDevType, short shDevNo, short shpByteSize, void *vpData, bool bExchangeData = true);
	short MelNetReceive		(short shDevType, short shDevNo, short *shpByteSize, void *vpData);
	short MelNetDevSet		(short shDevType, short shDevNo );
	short MelNetDevRst		(short shDevType, short shDevNo );
	short MelNetRandW		(short *shpSelDev,short *shpBuff, short shBuffSize );
	short MelNetRandR		(short *shpSelDev,short *shpBuff, short shBuffSize );
	short MelNetControl		(short shBuff );
	short MelNetTypeRead	();
	short MelNetBdLedRead	();
	short MelNetBdModRead	();
	short MelNetBdModSet	( short shMode );
	short MelNetBdRst		();
	short MelNetBdSwRead	();
	short MelNetBdVerRead	();
	short MelNetInit		();
	
	// (S.J.W)
	long MelNetSendEx(long nDevType, long nDevNo, long* npByteSize, short *shpData);
	long MelNetReceiveEx(long nDevType, long nDevNo, long* npByteSize, short *shpData);
	long MelNetDevSetEx(long nDevType, long nDevNo );
	long MelNetDevRstEx(long nDevType, long nDevNo );

//==========================================================================================================================//
// ER(0~31) Memory Data 전용
// PCJ
//	short MelNetERSend		(long nDevType, long nDevNo, long* npByteSize, void *vpData, bool bExchangeData = true);
//	short MelNetERRecv		(long nDevType, long nDevNo, long* npByteSize, void *vpData);
//==========================================================================================================================//

	short MelsecError(char *Func, long nError, long nDevType, long nDevAdd);

	long	m_nMelOpened;									//	Melsec Net Open Status
	long	m_nChannel;										//	Used Channel No
//	long	m_nERMemTreatChNo;								//	Board ER(0~31) Memory 처리용 Channel No (CH 9 : PC Device ... (ER - Memory)
	long	m_nNetworkNo;									//  (S.J.W) Ex 함수를 위한 Network parameter
	short	m_shSourceStNo;									//	Self Station No ( 255 )
	short	m_shTargetStNo;									//	Other Station No
	short	m_shControl;									//	Remote Run/Stop/Pause
	short	m_shStationType;								//	Station Type Information
	short	m_shBoardLedInfo[MAX_BOARD_LED_SIZE];			//	Board Led Information
	short	m_shBoardMode;									//	Board Mode
	short	m_shBoardSWInfo[MAX_BOARD_SWITCH_SIZE];			//	Board Switch Information
	short	m_shBoardVersionInfo[MAX_BOARD_VERSION_SIZE];	//	Board Version Information
};

#endif // !defined(AFX_MELSEC_H__0C4BC213_84E0_11D6_B2A9_0050DA8B0583__INCLUDED_)
