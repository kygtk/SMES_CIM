// Melsec.cpp: implementation of the CMelsec class.
//
//////////////////////////////////////////////////////////////////////
#include	<stdio.h>
#include	"Melsec.h"
#include	"../include/mdfunc.h"
#include	<atlbase.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComAutoCriticalSection MelsecLock;

CMelsec::CMelsec()
{
	m_nChannel			=	0;	
//	m_nERMemTreatChNo	=	9;		// ERMem Ã³¸®¿ë °íÁ¤ Chanel 9, Mdopen¿ë Chanel 15
	m_nNetworkNo		=	0;		// (S.J.W) NetworkNo ÃÊ±âÈ­
	m_shSourceStNo		=	0;	
	m_shTargetStNo		=	0;
	m_shControl			=	0;
	m_shStationType		=	0;

	memset(m_shBoardLedInfo,0x00, MAX_STATION_COUNT);			//	Board Led Information
	m_shBoardMode	=	0;										//	Board Mode
	memset(m_shBoardSWInfo,	0x00, MAX_BOARD_SWITCH_SIZE);		//	Board Switch Information
	memset(m_shBoardVersionInfo,0x00, MAX_BOARD_VERSION_SIZE);	//	Board Version Information

	m_nMelOpened = FALSE;
	m_MelsecLog.SetLogConfig(-1,eLog_DEFAULT_MELSEC,-1);
}

CMelsec::~CMelsec()
{

}

void CMelsec::SetConfig(long nChannel, long nNetwork, short shSourceStNo, short shTargetStNo)
{
	m_nChannel		=	nChannel;
	m_nNetworkNo	=	nNetwork;		// (S.J.W)
	m_shSourceStNo	=	shSourceStNo;
	m_shTargetStNo	=	shTargetStNo;

	MelNetOpen();
	if(m_nChannel != 9)
	{
		MelNetBdLedRead();
		//BOOL bRval = m_shBoardLedInfo[0] & 0x0010;
		//if (bRval == FALSE)
			MelNetBdRst();
	}

}

//	»óÀ§¿¡ ÀÇÇØ »ç¿ëÇÒ MelsecNet BoardÀ» ¼³Á¤ÇÑ´Ù.
//	MelsecNet BoardÀÇ ChannelÀº 51 ~ 54±îÁö Max 4°³ÀÇ Board°¡ ¼³Ä¡ °¡´ÉÇÏ´Ù.
//	Return Value : if Success then 0
//				   else	Error Code
//	MelsecNet/H Interface¸¦ ÇÏ±â À§ÇÑ ÇÔ¼ö 
//	ÀÌ ÇÔ¼ö È£ÃâÀÌÈÄ¿¡ Melsecnet interface°¡ °¡´ÉÇÏ´Ù.
short CMelsec::MelNetOpen()
{
	short	error		=	0;
	short	shChannel	=	(short)m_nChannel;
//	shRval = mdOpen(shChannel, -1, &m_nChannel);
	error = mdOpen(shChannel, 255, &m_nChannel);
//	if ( shRval != 0 || shChannel != m_nChannel )
	if ( error != 0) // PCJ
	{
//		MessageBox(NULL, "MelNet Open Failed", "MelNet Error", MB_OK);
		m_nMelOpened = FALSE;
		return MelsecError("mdOpen()", error,NULL,NULL);
	}

	m_nMelOpened = TRUE;
	return error;
}

//	Closes the communication loop
//	Return Value : if Success then 0
//				   else	Error Code
short CMelsec::MelNetClose()
{
	short error = 0;
	error = mdClose(m_nChannel);

	return MelsecError("mdClose()", error,NULL,NULL);
}

//	This function is used to send data
//	Writes data to selected device.
//	Checks the address and size that are decided from checking arguments and the arguments that are within the device range of memory.
//	Responds maximum size in “size” when the read size is beyond the ranges of device.
short CMelsec::MelNetSend(short shDevType, short shDevNo, short shByteSize, void *vpData, bool bExchangeData)
{
	MelsecLock.Lock();

	short error = 0;
	error = mdSend(m_nChannel, m_shSourceStNo, shDevType, shDevNo, &shByteSize, vpData); 
//	error = mdSend(51, 0xff, shDevType, shDevNo, &shByteSize, vpData); 

	MelsecLock.Unlock();

	return MelsecError("mdSend()",error,shDevType,shDevNo);
}

//	This function is used to receive data.
short CMelsec::MelNetReceive(short shDevType, short shDevNo, short *shpByteSize, void *vpData )
{
	MelsecLock.Lock();

	short error = 0;
	error = mdReceive(m_nChannel, m_shTargetStNo, shDevType, shDevNo, shpByteSize, vpData); 
//	error = mdReceive(51, 0xff, shDevType, shDevNo, shpByteSize, vpData); 

	MelsecLock.Unlock();

	return MelsecError("mdReceive()",error,shDevType,shDevNo);
}

//	Sets bit device.
short CMelsec::MelNetDevSet(short shDevType, short shDevNo )
{
	MelsecLock.Lock();

	short error = 0;
	error = mdDevSet(m_nChannel, m_shSourceStNo, shDevType, shDevNo);
//	error = mdDevSet(51, 0xff, shDevType, shDevNo);

	MelsecLock.Unlock();

	return MelsecError("mdDevSet()",error,shDevType,shDevNo);
}

//	Resets bit device.
short CMelsec::MelNetDevRst(short shDevType, short shDevNo )
{
	MelsecLock.Lock();

	short error = 0;
	error = mdDevRst(m_nChannel, m_shSourceStNo, shDevType, shDevNo);
//	error = mdDevRst(51, 0xff, shDevType, shDevNo);

	MelsecLock.Unlock();

	return MelsecError("mdDevRst()",error,shDevType,shDevNo);
}

//	Write device random.
short CMelsec::MelNetRandW(short *shpSelDev,short *shpBuff, short shBuffSize )
{
	short error = 0;
	error = mdRandW(m_nChannel, m_shSourceStNo, shpSelDev, shpBuff, shBuffSize);

	return MelsecError("mdRandW()",error,shpSelDev[1],shpSelDev[2]);
}

//	Reads device random
short CMelsec::MelNetRandR(short *shpSelDev,short *shpBuff, short shBuffSize )
{
	short error = 0;
	error = mdRandR(m_nChannel, m_shTargetStNo, shpSelDev, shpBuff, shBuffSize);

	return MelsecError("mdRandR()",error,shpSelDev[1],shpSelDev[2]);
}

#if FALSE
// ========================================================================================================================//
// (S.J.W) ER¿µ¿ª Á¢±ÙÀÌ ¾ø¾îÁö¸é¼­ ¾ø¾Öµµ µÇ´Â ºÎºÐ
// ========================================================================================================================//
short CMelsec::MelNetERSend(long nDevType, long nDevNo, long* npByteSize, void *vpData, bool bExchangeData)
{
	MelsecLock.Lock();

	long error = 0;
	error = mdSendEx(m_nChannel, m_nNetworkNo, m_shSourceStNo, nDevType, nDevNo, npByteSize, vpData); 
	MelsecLock.Unlock();

	return MelsecError("mdSend()",error,nDevType,nDevNo);
}

//	This function is used to receive data.
short CMelsec::MelNetERRecv(long nDevType, long nDevNo, long *npByteSize, void *vpData )
{
	MelsecLock.Lock();

	long error = 0;
	error = mdReceiveEx(m_nChannel, m_nNetworkNo, m_shSourceStNo, nDevType, nDevNo, npByteSize, vpData); 
	MelsecLock.Unlock();
	return MelsecError("mdReceive()",error,nDevType,nDevNo);
}
// ========================================================================================================================//
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (S.J.W) Ex Function
long CMelsec::MelNetSendEx(long nDevType, long nDevNo, long* npByteSize, short *shpData)
{
	MelsecLock.Lock();
	
	long error;
	error = mdSendEx(m_nChannel, m_nNetworkNo, m_shSourceStNo, nDevType, nDevNo, npByteSize, shpData); 

	MelsecLock.Unlock();

	return MelsecError("mdSendEx()",error,nDevType,nDevNo);
}

long CMelsec::MelNetReceiveEx(long nDevType, long nDevNo, long* npByteSize, short *shpData)
{
	MelsecLock.Lock();

	long error;
	error = mdReceiveEx(m_nChannel, m_nNetworkNo, m_shSourceStNo, nDevType, nDevNo, npByteSize, shpData); 

	MelsecLock.Unlock();

	return MelsecError("mdReceiveEx()",error,nDevType,nDevNo);
}

long CMelsec::MelNetDevSetEx(long nDevType, long nDevNo )
{
	MelsecLock.Lock();

	long error = 0;
	error = mdDevSetEx(m_nChannel, m_nNetworkNo, m_shSourceStNo, nDevType, nDevNo);

	MelsecLock.Unlock();

	return MelsecError("mdDevSetEx()",error,nDevType,nDevNo);
}

long CMelsec::MelNetDevRstEx(long nDevType, long nDevNo )
{
	MelsecLock.Lock();
	
	long error = 0;
	error = mdDevRstEx(m_nChannel, m_nNetworkNo, m_shSourceStNo, nDevType, nDevNo);

	MelsecLock.Unlock();

	return MelsecError("mdDevRstEx()",error,nDevType,nDevNo);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

//	Remotes RUN/STOP/PAUSE.
//	Return Value : if Success then 0
//				   else	Error Code
short CMelsec::MelNetControl(short shControlMode )
{
	short error = 0;
	error = mdControl(m_nChannel, m_shSourceStNo, m_shControl);

	return MelsecError("mdControl()",error,NULL,NULL);
}

//	Reads the format name of sequencer
//	Return Value : if Success then 0
//				   else	Error Code
short CMelsec::MelNetTypeRead()
{
	short error = 0;
	error = mdTypeRead(m_nChannel, m_shSourceStNo, &m_shStationType);

	return MelsecError("mdTypeRead()",error,NULL,NULL);
}

/*
	Melsec Net/H BoardÀÇ LedÀÇ »óÅÂ¸¦ ÀÐ¾î¿Â´Ù.
	Data Size : short shLedStatus[2];
	Return Value : if Success then 0
				   else	Error Code
*/
short CMelsec::MelNetBdLedRead()
{
	short error = 0;

	error = mdBdLedRead(m_nChannel, m_shBoardLedInfo);

	return MelsecError("mdBdLedRead()",error,NULL,NULL);
}

/*
	Melsec Net/H BoardÀÇ µ¿ÀÛ Mode¸¦ ÀÐ¾î¿Â´Ù.
	Data Size : short *shpMode;
	Return Value : if Success then 0
				   else	Error Code
*/
short CMelsec::MelNetBdModRead()
{
	short error = 0;

	error = mdBdModRead(m_nChannel, &m_shBoardMode);

	return MelsecError("mdBdModRead()",error,NULL,NULL);
}

/*
	Melsec Net/H BoardÀÇ µ¿ÀÛ Mode¸¦ ¼³Á¤ÇÑ´Ù.
	Data Size : short shMode;
	Return Value : if Success then 0
				   else	Error Code
*/
short CMelsec::MelNetBdModSet( short shMode )
{
	short error = 0;

	error = mdBdModSet(m_nChannel, m_shBoardMode);

	if(error != 0) return MelsecError("mdBdModSet()",error,NULL,NULL);
	else if(error == 0) m_shBoardMode	=	shMode;

	return error;
}

/*
	Melsec Net/H Board¸¦ ResetÇÑ´Ù.
	Return Value : if Success then 0
				   else	Error Code
*/
short CMelsec::MelNetBdRst()
{
	short error = 0;

	error = mdBdRst(m_nChannel);

	return MelsecError("mdBdRst()",error,NULL,NULL);
}

/*
	Melsec Net/H BoardÀÇ Switch¸¦ ReadÇÑ´Ù.
	Return Value : if Success then 0
				   else	Error Code
*/
short CMelsec::MelNetBdSwRead()
{
	short error = 0;

	error = mdBdSwRead(m_nChannel, m_shBoardSWInfo);

	return MelsecError("mdBdSwRead()",error,NULL,NULL);
}

//	Reads the version information of MelsecNet/H Card
//	Return Value : if Success then 0
//				   else	Error Code
short CMelsec::MelNetBdVerRead()
{
	short error = 0;

	error = mdBdVerRead(m_nChannel, m_shBoardVersionInfo);

	return MelsecError("mdBdVerRead()",error,NULL,NULL);
}

//	Refreshes PLC device address table
//	Return Value : if Success then 0
//				   else	Error Code
short CMelsec::MelNetInit()
{
	short error = 0;

	error = mdInit(m_nChannel);

	return MelsecError("mdInit()",error,NULL,NULL);
}

short CMelsec::MelsecError(char *Func, long nError, long nDevType, long nDevAdd)
{
	if(nError == 0 || !m_nMelOpened) return 0;

	char chTemp[255];
	char chDevType[10];

	memset(chTemp,0x00,sizeof(chTemp));
	memset(chDevType,0x00,sizeof(chDevType));

	if(nDevType == 23) sprintf(chDevType,"DevB");
	else if(nDevType == 24) sprintf(chDevType,"DevW");
	else if((nDevType >= 22000) && (nDevType <= 22256)) // DevER0(22000) ~ DevER256(22256)
	{
		nDevType = nDevType - 22000 ;
		sprintf(chDevType,"DevER%d",nDevType);
	}


	if((nError >=16384) && (nError <= 120479))
	{
		if((nError == 16386) || (nError == 16400) || (nError == 16432) || (nError == 16433)
		|| (nError == 16488) || (nError == 16449) || (nError == 16450) || (nError == 16451)
		|| (nError == 16523) || (nError == 19200) || (nError == 19203))	;
		else nError = 16384;
	}

	if((nError >=28672) && (nError <= 32767)) nError = 28672;

	if((nError >= -4096) && (nError <= -257))
	{
		if(nError == -2174);
		else nError = 28672;
	}

	if((nError >= -16384) && (nError <= -12289)) nError = -16384;

	if((nError >= -20480) && (nError <= -16385)) nError = -20480;

	switch(nError)
	{
	case 0:	sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Nomal end",Func,nError,chDevType,nDevAdd); break;
	case 1:	sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Driver is not started up. Interrupt number and I/O address are overlapped with other card",Func,nError,chDevType,nDevAdd); break;
	case 2:	sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Card response error. Timeout during waiting for the response of process",Func,nError,chDevType,nDevAdd); break;
	//(41)
	case 65: sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Channel error. Channel number that is not registered is selected.",Func,nError,chDevType,nDevAdd); break;
	//(42)
	case 66: sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Already opened. Selected channel has been already opened.",Func,nError,chDevType,nDevAdd); break;
	//(43)
	case 67: sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Already closed. Selected channel has been already closed.",Func,nError,chDevType,nDevAdd); break;
	//(44)
	case 68: sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] PATH error. PATH is selected that cannot be opened.",Func,nError,chDevType,nDevAdd); break;
	//(45)
	case 69: sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Operate code error. Operated code that is not supported is edited.",Func,nError,chDevType,nDevAdd); break;
	//(46)
	case 70: sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] station number setting error. own station number was set in the station number.",Func,nError,chDevType,nDevAdd); break;
	//(47)
	case 71: sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Non receiving data(during demand RECV). Data has not been received.",Func,nError,chDevType,nDevAdd); break;
	//(4D)
	case 77: sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Memory secure error. The memory cannot be secured.",Func,nError,chDevType,nDevAdd); break;
	//(4E)
	case 78: sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Time out error on the mode set. Mode setting is operated, but mode setting can not be done because of time out.",Func,nError,chDevType,nDevAdd); break;
	//(55)
	case 85: sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Channel number error(during demand RECV). Channel number error",Func,nError,chDevType,nDevAdd); break;
	//(64)
	case 100: sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Accessing to own station card. During own card accessing, it request to access own card.",Func,nError,chDevType,nDevAdd); break;
	//(65)
	case 101: sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Invalid routing parameter. Routing parameter is not set.",Func,nError,chDevType,nDevAdd); break;
	//(66)
	case 102:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Data sending error. It is failed to send data",Func,nError,chDevType,nDevAdd); break;
	//(67)
	case 103:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Receiving data error. It is failed to receive data",Func,nError,chDevType,nDevAdd); break;
	//(81)
	case 129:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Device type error. Selected device type is invalid",Func,nError,chDevType,nDevAdd); break;
	//(82)
	case 130:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Device number error. Device number is beyond the range.At the time of selecting bit device, the device number is not in multiples of 8.",Func,nError,chDevType,nDevAdd); break;
	//(83)
	case 131:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Device points error. The points beyond the range of device is set.At the time of selecting bit device, the device number is not in multiples of 8.",Func,nError,chDevType,nDevAdd); break;
	//(84)
	case 132:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Written count error. The value beyond the range in written bit count is set.",Func,nError,chDevType,nDevAdd); break;
	//(85)
	case 133:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Link parameter error. Link parameter is not perfect. All slave station counts of link parameter are 0.",Func,nError,chDevType,nDevAdd); break;
	//(D7)
	case 215:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Received data length error. Received data is too long, or bit is beyond the range of the length. ",Func,nError,chDevType,nDevAdd); break;
	//(E4)
	case 228:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Link selecting error. Operation code that cannot be operated by request target station is set.(request target link unit checks)",Func,nError,chDevType,nDevAdd); break;
	//(500)
	case 1280:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Memory access error of own station.",Func,nError,chDevType,nDevAdd); break;
	//(501)
	case 1281:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] I/O port cannot access.",Func,nError,chDevType,nDevAdd); break;
	//(100E)
	case 4110:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] DLL non-load error.",Func,nError,chDevType,nDevAdd); break;
	//(200C)
	case 8204:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Request cancel.",Func,nError,chDevType,nDevAdd); break;
	//(200D)
	case 8205:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Drive name error.",Func,nError,chDevType,nDevAdd); break;
	//(200E)
	case 8206:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] First step error.",Func,nError,chDevType,nDevAdd); break;
	//(200F)
	case 8207:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Parameter type error.",Func,nError,chDevType,nDevAdd); break;
	//(2010)
	case 8208:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] File name error.",Func,nError,chDevType,nDevAdd); break;
	//(2011)
	case 8209:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Registering/release/set status error.",Func,nError,chDevType,nDevAdd); break;
	//(2012)
	case 8210:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Detailed condition division error.",Func,nError,chDevType,nDevAdd); break;
	//(2013)
	case 8211:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Step condition error.",Func,nError,chDevType,nDevAdd); break;
	//(2014)
	case 8212:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Bit device condition error.",Func,nError,chDevType,nDevAdd); break;
	//(2015)
	case 8213:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Parameter setting error.",Func,nError,chDevType,nDevAdd); break;
	//(2017)
	case 8215:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Keyword error.",Func,nError,chDevType,nDevAdd); break;
	//(2018)
	case 8216:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Read/write flag error.",Func,nError,chDevType,nDevAdd); break;
	//(2019)
	case 8217:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Refresh method error.",Func,nError,chDevType,nDevAdd); break;
	//(201A)
	case 8218:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Buffer access method error.",Func,nError,chDevType,nDevAdd); break;
	//(201B)
	case 8219:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Start mode/stop mode error.",Func,nError,chDevType,nDevAdd); break;
	//(201C)
	case 8220:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Written clock data error.",Func,nError,chDevType,nDevAdd); break;
	//(201D)
	case 8221:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Online data write error.",Func,nError,chDevType,nDevAdd); break;
	//(201F)
	case 8223:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Trace time error.",Func,nError,chDevType,nDevAdd); break;
	//(2020)
	case 8224:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] First I/O number error.",Func,nError,chDevType,nDevAdd); break;
	//(2021)
	case 8225:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] First address error.",Func,nError,chDevType,nDevAdd); break;
	//(2022)
	case 8226:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Pattern error.",Func,nError,chDevType,nDevAdd); break;
	//(2023)
	case 8227:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] SFC block number error.",Func,nError,chDevType,nDevAdd); break;
	//(2024)
	case 8228:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] SFC step number error.",Func,nError,chDevType,nDevAdd); break;
	//(2025)
	case 8229:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Step number error.",Func,nError,chDevType,nDevAdd); break;
	//(2026)
	case 8230:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Data error.",Func,nError,chDevType,nDevAdd); break;
	//(2027)
	case 8231:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] System data error.",Func,nError,chDevType,nDevAdd); break;
	//(2028)
	case 8232:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] TC set value number error.",Func,nError,chDevType,nDevAdd); break;
	//(2029)
	case 8233:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Clear mode error.",Func,nError,chDevType,nDevAdd); break;
	//(202A)
	case 8234:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Signal flow error.",Func,nError,chDevType,nDevAdd); break;
	//(202B)
	case 8235:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Version administration error",Func,nError,chDevType,nDevAdd); break;
	//(202C)
	case 8236:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Module has been registered.",Func,nError,chDevType,nDevAdd); break;
	//(202D)
	case 8237:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] PI type error.",Func,nError,chDevType,nDevAdd); break;
	//(202E)
	case 8238:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] PI No error.",Func,nError,chDevType,nDevAdd); break;
	//(202F)
	case 8239:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] PI number error.",Func,nError,chDevType,nDevAdd); break;
	//(2030)
	case 8240:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Shift error.",Func,nError,chDevType,nDevAdd); break;
	//(2031)
	case 8241:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] File type error.",Func,nError,chDevType,nDevAdd); break;
	//(2032)
	case 8242:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Specified module error.",Func,nError,chDevType,nDevAdd); break;
	//(2033)
	case 8243:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Error check flag error.",Func,nError,chDevType,nDevAdd); break;
	//(2034)
	case 8244:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Step RUN-operation error.",Func,nError,chDevType,nDevAdd); break;
	//(2035)
	case 8245:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Step RUN data error.",Func,nError,chDevType,nDevAdd); break;
	//(2036)
	case 8246:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Step RUN-time error.",Func,nError,chDevType,nDevAdd); break;
	//(2037)
	case 8247:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Program RUN inside writing error to E2ROM.",Func,nError,chDevType,nDevAdd); break;
	//(2038)
	case 8248:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Clock data read/write error.",Func,nError,chDevType,nDevAdd); break;
	//(2039)
	case 8249:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Trace non-completion.",Func,nError,chDevType,nDevAdd); break;
	//(203A)
	case 8250:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Registration clearness flag error.",Func,nError,chDevType,nDevAdd); break;
	//(203B)
	case 8251:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Operation error.",Func,nError,chDevType,nDevAdd); break;
	//(203C)
	case 8252:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] The number of station error.",Func,nError,chDevType,nDevAdd); break;
	//(203D)
	case 8253:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] The number of repeat error.",Func,nError,chDevType,nDevAdd); break;
	//(203E)
	case 8254:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] The acquisition data selection error.",Func,nError,chDevType,nDevAdd); break;
	//(203F)
	case 8255:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] The number of SFC cycle error.",Func,nError,chDevType,nDevAdd); break;
	//(2042)
	case 8258:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] The scheduled time setting error.",Func,nError,chDevType,nDevAdd); break;
	//(2043)
	case 8259:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Function count error.",Func,nError,chDevType,nDevAdd); break;
	//(2044)
	case 8260:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] System information error.",Func,nError,chDevType,nDevAdd); break;
	//(2046)
	case 8262:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Function number error.",Func,nError,chDevType,nDevAdd); break;
	//(2047)
	case 8263:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] RAM operation error.",Func,nError,chDevType,nDevAdd); break;
	//(2048)
	case 8264:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Boot former ROM forwarding failure.",Func,nError,chDevType,nDevAdd); break;
	//(2049)
	case 8265:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Boot former transfer mode specification error.",Func,nError,chDevType,nDevAdd); break;
	//(204A)
	case 8266:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Not enough memory.",Func,nError,chDevType,nDevAdd); break;
	//(204B)
	case 8267:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Backup drive (former boot drive) ROM error.",Func,nError,chDevType,nDevAdd); break;
	//(204C)
	case 8268:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Block size error.",Func,nError,chDevType,nDevAdd); break;
	//(204D)
	case 8269:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] RUN-time detaching error.",Func,nError,chDevType,nDevAdd); break;
	//(204E)
	case 8270:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Module has already registered.",Func,nError,chDevType,nDevAdd); break;
	//(204F)
	case 8271:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Password registration data full error.",Func,nError,chDevType,nDevAdd); break;
	//(2050)
	case 8272:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Password unregistration error.",Func,nError,chDevType,nDevAdd); break;
	//(2051)
	case 8273:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Remote password error.",Func,nError,chDevType,nDevAdd); break;
	//(2052)
	case 8274:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] IP address error.",Func,nError,chDevType,nDevAdd); break;
	//(2053)
	case 8275:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Error (argument when requesting) outside time-out value range.",Func,nError,chDevType,nDevAdd); break;
	//(2054)
	case 8276:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Instruction cast undetection.",Func,nError,chDevType,nDevAdd); break;
	//(2055)
	case 8277:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Trace execution type error.",Func,nError,chDevType,nDevAdd); break;
	//(2056)
	case 8278:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] 	Version error.",Func,nError,chDevType,nDevAdd); break;
	//(4000)
	case 16384:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Errors detected by the access target CPU",Func,nError,chDevType,nDevAdd); break;
	//(4002)
	case 16386:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] The demand which cannot be processed was received.",Func,nError,chDevType,nDevAdd); break;
	//(4010)
	case 16400:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] A RUN state is impossible.",Func,nError,chDevType,nDevAdd); break;
	//(4030)
	case 16432:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] The specified device type does not exist.",Func,nError,chDevType,nDevAdd); break;
	//(4031)
	case 16433:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Specified device No. is outside the range.Block No. of the specified device is invalid.",Func,nError,chDevType,nDevAdd); break;
	//(4040)
	case 16488:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] A unit does not exist.",Func,nError,chDevType,nDevAdd); break;
	//(4041)
	case 16449:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Device mark are outside the range.",Func,nError,chDevType,nDevAdd); break;
	//(4042)
	case 16450:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] The abnormalities in an applicable unit",Func,nError,chDevType,nDevAdd); break;
	//(4043)
	case 16451:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] A unit does not exist in the specified position.",Func,nError,chDevType,nDevAdd); break;
	//(408B)
	case 16523:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Remote demand execution is impossible.",Func,nError,chDevType,nDevAdd); break;
	//(4B00)
	case 19200:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] The target abnormalities were detected.",Func,nError,chDevType,nDevAdd); break;
	//(4B03)
	case 19203:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Course [ in which it does not support ] error. A demand cannot be performed for the specified course and object.",Func,nError,chDevType,nDevAdd); break;
	//(7000)
	case 28672:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Errors detected by intelligent function modules such as the serial communication module",Func,nError,chDevType,nDevAdd); break;
	//(9E81)
	case 40577:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Device type error. The device type specified to the demand place office is invalid. (A demand place link unit checks.)",Func,nError,chDevType,nDevAdd); break;
	//(9E82)
	case 40578:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Device No. error. The mark besides the device range were set up to the demand place office. Device No. is not the multiple of 8 at the time of bit device specification. (A demand place link unit checks.)",Func,nError,chDevType,nDevAdd); break;
	//(9E83)
	case 40579:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] The mark besides the device range were set up to the demand place office. Device No. is not the multiple of 8 at the time of bit device specification. (A demand place link unit checks.)",Func,nError,chDevType,nDevAdd); break;
	//(FFFF)
	case -1:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Path error. Set path is invalid.",Func,nError,chDevType,nDevAdd); break;
	//(FFFE)
	case -2:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Device number error. The set device number is beyond the range.At the time of selecting bit device, the device number is not in multiples of 8.",Func,nError,chDevType,nDevAdd); break;
	//(FFFD)
	case -3:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Device type error. The set bit device type is invalid.",Func,nError,chDevType,nDevAdd); break;
	//(FFFB)
	case -5:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Size error. The device number and the size are beyond the range of device.The odd bit number is accessed.",Func,nError,chDevType,nDevAdd); break;
	//(FFFA)
	case -6:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Block count error. The read device random or block count that is set by written dev[0] is beyond the range.",Func,nError,chDevType,nDevAdd); break;
	//(FFF8)
	case -8:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Channel number error. The set channel number is invalid in mdOpenfunction.",Func,nError,chDevType,nDevAdd); break;
	//(FFF5)
	case -11:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Buffer area is insufficient. Read-out data storing area size reads, and it is smaller than data size.",Func,nError,chDevType,nDevAdd); break;
	//(FFF4)
	case -12:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Block error. The set block number of extension file is invalid.",Func,nError,chDevType,nDevAdd); break;
	//(FFF3)
	case -13:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Wright protected error. The block number of the set extension file resister is overlapped with protect area of memory cassette.",Func,nError,chDevType,nDevAdd); break;
	//(FFF2)
	case -14:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Memory cassette error. Memory cassette is not inserted in the accessed PLC or it is inserted inadequate memory cassette.",Func,nError,chDevType,nDevAdd); break;
	//(FFF1)
	case -15:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Reading area length error. The read area size (read data store) is small.",Func,nError,chDevType,nDevAdd); break;
	//(FFF0)
	case -16:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Station number,network number error. Station number and network number are beyond the range.",Func,nError,chDevType,nDevAdd); break;
	//(FFEF)
	case -17:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] All stations, group number setting error. All stations and group number are set in not supported function.",Func,nError,chDevType,nDevAdd); break;
	//(FFEE)
	case -18:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Remote select error. Selected code is not selected.",Func,nError,chDevType,nDevAdd); break;
	//(FFED)
	case -19:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] SEND/RECVchannel number error. The channel number that is select SEND/RECV function is out of range.",Func,nError,chDevType,nDevAdd); break;
	//(FFEB)
	case -21:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Error in gethostbyname(). Error in function gethostbyname()",Func,nError,chDevType,nDevAdd); break;
	//(FFE8)
	case -24:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Time out error in select(). Time out error in function select()",Func,nError,chDevType,nDevAdd); break;
	//(FFE7)
	case -25:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Error in sendto(). Error in function sendto()",Func,nError,chDevType,nDevAdd); break;
	//(FFE6)
	case -26:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Error in recvfrom(). Error in function recvfrom()",Func,nError,chDevType,nDevAdd); break;
	//(FFE4)
	case -28:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Receiving error. Received error response",Func,nError,chDevType,nDevAdd); break;
	//(FFE3)
	case -29:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Received data length over. received too many data",Func,nError,chDevType,nDevAdd); break;
	//(FFE2)
	case -30:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] PLC number error. Received PLC number is invalid",Func,nError,chDevType,nDevAdd); break;
	//(FFE1)
	case -31:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] DLL load error. It is failed to load necessary DLL to execute the function",Func,nError,chDevType,nDevAdd); break;
	//(FFE0)
	case -32:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Resource timeout error",Func,nError,chDevType,nDevAdd); break;
	//(FFDF)
	case -33:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Access target invalid error. The setting of the target is invalid.",Func,nError,chDevType,nDevAdd); break;
	//(FFDE)
	case -34:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Registry open error. It is failed to open the registry",Func,nError,chDevType,nDevAdd); break;
	//(FFDD)
	case -35:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Registry reading error. It is failed to read the registry",Func,nError,chDevType,nDevAdd); break;
	//(FFDC)
	case -36:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Registry writing error. It is failed to write to the registry",Func,nError,chDevType,nDevAdd); break;
	//(FFDB)
	case -37:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Communication initial setting error. It is failed to initialize communication setting.",Func,nError,chDevType,nDevAdd); break;
	//(FFDA)
	case -38:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Ethernet communication setting error. It is failed to set the Ethernet communication",Func,nError,chDevType,nDevAdd); break;
	//(FFD9)
	case -39:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] COM communication setting error. It is failed the set the communication of COM.",Func,nError,chDevType,nDevAdd); break;
	//(FFD7)
	case -41:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] COM control error. Control cannot be operated rightly in COM communication.",Func,nError,chDevType,nDevAdd); break;
	//(FFD6)
	case -42:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Closed error. Communication cannot close.",Func,nError,chDevType,nDevAdd); break;
	//(FFD5)
	case -43:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] ROM operation error. TC setting value was written in PLC under ROM operation.",Func,nError,chDevType,nDevAdd); break;
	//(FFD4)
	case -44:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] LLT communication setting error. A setup for performing LLT communication went wrong.",Func,nError,chDevType,nDevAdd); break;
	//(FFD3)
	case -45:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Ethernet control error. Control in the time of Ethernet communication cannot be performed correctly.",Func,nError,chDevType,nDevAdd); break;
	//(FFD2)
	case -46:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] USB open error. Initialization of a USB port and opening went wrong.",Func,nError,chDevType,nDevAdd); break;
	//(FFD1)
	case -47:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Random read-out condition abortive error. Since it is random read-out condition failure, it random-reads and cannot do.",Func,nError,chDevType,nDevAdd); break;
	//(FFD0)
	case -48:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] TEL error.",Func,nError,chDevType,nDevAdd); break;
	//(FFCE)
	case -50:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Open path maximum over. Opened path is beyond the maximum (32).",Func,nError,chDevType,nDevAdd); break;
	//(FFCD)
	case -51:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Exclusive control error. An error occurs in exclusive control.",Func,nError,chDevType,nDevAdd); break;
	//(F000)
	case -4096:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Errors detected in the MELSECNET/H,  MELSECNET/10 network system",Func,nError,chDevType,nDevAdd); break;
	//(F782)
	case -2174:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] station number setting error. own station number was set in the station number.",Func,nError,chDevType,nDevAdd); break;
	//(C000)
	case -16384:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Errors detected by intelligent function modules such as the Ethernet interface module",Func,nError,chDevType,nDevAdd); break;
	//(B000)
	case -20480:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] QJ61BT11/ QJ61BT11N Control & Errors detected in the CC-Link system",Func,nError,chDevType,nDevAdd); break;
	//(B782)
	case -18558:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] station number setting error. own station number was set in the station number.",Func,nError,chDevType,nDevAdd); break;
	//(B774)
	case -18572:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Transient a non-supported error. The transient demand was transmitted to the station which is not an intelligent device station.",Func,nError,chDevType,nDevAdd); break;
	//(9E81)
	case -24959:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Device type error. The set bit device type selected to the request station is invalid.(The request link unit checks.)",Func,nError,chDevType,nDevAdd); break;
	//(9E82)
	case -24958:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Device number error. The set device number to request station is beyond the range.At the time of selecting bit device, the device number is not in multiples of 8.(The request link unit checks.)",Func,nError,chDevType,nDevAdd); break;
	//(9E83)
	case -24957:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Device size error. The device number and the size to request station are beyond the range of device.At the time of selecting bit device, the device number is not in multiples of 8.(The request link unit checks.)",Func,nError,chDevType,nDevAdd); break;
	//(9920)
	case -26336:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Demand error to other loop. Routing demand to other loop",Func,nError,chDevType,nDevAdd); break;
	//(9922)
	case -26334:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Card reset error. During the access to other station, other process that is using same channel card reset.",Func,nError,chDevType,nDevAdd); break;
	//(9923)
	case -26333:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Card not support the QCPU error. The ROM version of the I/F card does not support to QCPU(Q mode).",Func,nError,chDevType,nDevAdd); break;
	//(9202)
	case -28158:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Watchdog Timer error",Func,nError,chDevType,nDevAdd); break;
	//(9204)
	case -28156:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Two Port Memory handshake error",Func,nError,chDevType,nDevAdd); break;
	//(920A)
	case -28150:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Data link error",Func,nError,chDevType,nDevAdd); break;
	//(9209)
	case -28151:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] APS number error. Received invalid response data",Func,nError,chDevType,nDevAdd); break;
	//(9E20)
	case -25056:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Operation code error. Set the operation code that demanded station cannot operate.(the request link unit checks)",Func,nError,chDevType,nDevAdd); break;
	//(902D)
	case -28627:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Transient a non-supported error. A transient demand cannot be performed for the specified course and object.(By CC-Link communication, when the station numbers of a own-station were 64 stations, other stations were specified.)",Func,nError,chDevType,nDevAdd); break;
	//(902C)
	case -28628:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Pointer address specification error. The inaccurate address was specified by specification of an argument pointer.?The address of a short type pointer is not the multiple of 2.?The address of a long type pointer is not the multiple of 4.",Func,nError,chDevType,nDevAdd); break;
	//(9026)
	case -28634:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Intelligent functional unit down error. An intelligent functional unit is unusual.",Func,nError,chDevType,nDevAdd); break;
	//(9025)
	case -28635:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Intelligent functional unit down error. The place which does not have an intelligent functional unit was accessed.",Func,nError,chDevType,nDevAdd); break;
	//(9024)
	case -28636:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Control bus error. The abnormalities in a control bus with an intelligent functional unit",Func,nError,chDevType,nDevAdd); break;
	//(9003)
	case -28669:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Area number error. The area number besides the specification range, an offset address, and the mode were set up.",Func,nError,chDevType,nDevAdd); break;
	//(9006)
	case -28666:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Data length error",Func,nError,chDevType,nDevAdd); break;
	//(9007)
	case -28665:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] A registration-data-less error",Func,nError,chDevType,nDevAdd); break;
	//(900A)
	case -28662:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Target CPU error. The set target CPU is beyond the range.",Func,nError,chDevType,nDevAdd); break;
	//(900B)
	case -28661:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Access improper error(Device number error). The set device number is beyond the access range.",Func,nError,chDevType,nDevAdd); break;
	//(900C)
	case -28660:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Access improper error(Size error). The device number and the size are beyond the access range.",Func,nError,chDevType,nDevAdd); break;
	//(9001)
	case -28671:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Unit discernment error",Func,nError,chDevType,nDevAdd); break;
	//(9000)
	case -28672:sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] Processing code error",Func,nError,chDevType,nDevAdd); break;
	default : sprintf(chTemp,"[%s][ErrNo: %d][%s][Hexa:%x] There is no data in Error List",Func,nError,chDevType,nDevAdd); break;
	}

	m_MelsecLog.AddLog(chTemp);

	return (short)nError;
}

