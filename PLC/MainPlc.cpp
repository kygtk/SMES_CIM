// CMainPlc.cpp: implementation of the CCMainPlc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdio.h"
#include "MainPlc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMainPlc::CMainPlc()
{
	m_shCIMAlive		= 0;
	m_nTimesetDay		= 0;
}

CMainPlc::~CMainPlc()
{
	
}

// Thread (Timer) 관련.
BOOL CMainPlc::OnInit()
{

	return TRUE;
}

BOOL CMainPlc::OnTerminate()
{
	return TRUE;
}


void CMainPlc::CreateExternalQueue()
{
	stPlcQueueType	Que;

	m_cMyQueue.Create(PLC_QUEUE_NAME, MAX_PLC_QUEUE_COUNT, sizeof(stPlcQueueType));	
	m_cSendQue.Create(SCH_QUEUE_NAME, MAX_SCH_QUEUE_COUNT, sizeof(stQueueRootType));	
	m_cMMIQue.Create(MMI_QUEUE_NAME , MAX_MMI_QUEUE_COUNT, sizeof(stQueueRootType));

	while(m_cMyQueue.GetState() >0)
		m_cMyQueue.Read(&Que);		 
}

void CMainPlc::CreateInternalQueue()
{
	switch(m_stpSma->stLayOutCfg.nEQType)
	{
	case eEQType_ETCHSTRIP:
		if(m_stpSma->stLayOutCfg.nEQSubType == eEQETCHSTRIPSubType_CFITO)
		{
			m_cInternelQue[eModuleType_Etcher	].Create(MDL_ETCH_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));
			m_cInternelQue[eModuleType_Stripper	].Create(MDL_STRP_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));
		}
		else
		{
			m_cInternelQue[eModuleType_Etcher	].Create(MDL_ETCH_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));
			m_cInternelQue[eModuleType_Stripper	].Create(MDL_STRP_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));
			m_cInternelQue[eModuleType_Buffer	].Create(MDL_BUFF_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));
			m_cInternelQue[eModuleType_Bypass	].Create(MDL_BYPASS_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));
		}
		break;

	case eEQType_ETCH:
		m_cInternelQue[eModuleType_Etcher	].Create(MDL_ETCH_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));
		break;

	case eEQType_STRIP:
		m_cInternelQue[eModuleType_Stripper	].Create(MDL_STRP_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));
		break;
		
	case eEQType_PFC:
		m_cInternelQue[eModuleType_PFC	].Create(MDL_PFC_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));
		m_cInternelQue[eModuleType_EX	].Create(MDL_EX_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));
		break;

	case eEQType_EDGE:
		m_cInternelQue[eModuleType_Cleaner	].Create(MDL_EDGE_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));
		break;

	case eEQType_WRU:
		m_cInternelQue[eModuleType_WRU	].Create(MDL_WRU_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));
		break;

	case eEQType_LCPI:
		m_cInternelQue[eModuleType_LCPI	].Create(MDL_LCPI_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));
		break;

	case eEQType_LCODF:
		m_cInternelQue[eModuleType_LCODF	].Create(MDL_LCODF_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));
		break;

	case eEQType_LCRW:
		m_cInternelQue[eModuleType_LCRW	].Create(MDL_LCRW_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));
		break;
	}
}

void CMainPlc::SendSignalToSCH(stQueueRootType *sig)
{
	long nRet;

	nRet=m_cSendQue.Write(sig);
}
void CMainPlc::SendSignalToMMI(stQueueRootType *sig)
{
	long nRet;
	
	nRet = m_cMMIQue.Write(sig);
}

//	Photo Line Melsecnet Interface 초기화
BOOL CMainPlc::InitInstance()
{
//	long	i;
	stPlcQueueType sig;

	//	1. 공유 메모리 Addres 획득
	SysLib::SmaInitialize(PLC_TASK_ID);

	//  2. Soft Version SMA 등록
	//sprintf(m_stpSma->stLayOutCfg.stSoftTaskRev.szCIMRevPLC,"%s", PLC_VERSION);

	//	3. 내부 IPC용 Queue 생성 :
	CreateExternalQueue();	//	SCH, TMP Task용 IPC 통신
	CreateInternalQueue();	//	PLC Inner Thread IPC 
	
	//	4. Log File 경로 설정
	m_LogMaster.SetLogConfig(-1,eLog_DEFAULT_PLC,-1); 
	m_GUIlog.SetLogConfig(-1,eLog_DEFAULT_GUI,-1); 
	//m_MSCLog.SetLogConfig(-1,eLog_DEFAULT_MSC,-1); 

	m_LogMaster.AddLog("PLC Task Started!!");

	//	5. Melsecnet Board Open
 	for(int nMelOpen = 0 ; nMelOpen < 3 ; nMelOpen++) // Open될때까지 3회 Retry
	{
		if ( !m_MelLinkMemIF.m_nMelOpened)
		{
			m_MelLinkMemIF.SetConfig(151, 0, 255, 255);		// (S.J.W) nNetworkNo = 1
			Sleep(1000);
		}
		if ( !m_MelERMemIF.m_nMelOpened)
		{
			m_MelERMemIF.SetConfig(151, 0, 255, 255); // (S.J.W) nNetworkNo = 0, Channel = 151
			Sleep(1000);
		}

		if(m_MelLinkMemIF.m_nMelOpened || m_MelERMemIF.m_nMelOpened) break;
	}

	if ( !(m_MelLinkMemIF.m_nMelOpened | m_MelERMemIF.m_nMelOpened) )
	{
		m_LogMaster.AddLog("Isn't open the Link Memory Network!");
		return FALSE;
	}
	
	//	6. 각 Process Module I/F 기동
	SetLayOutCfg();

	//	First Time Synchronize with PLC
	sig.nSignal = sigTimeSetRequest;
	C2P_TimeSetSequence(sig);

	m_stpSma->stSysRunInfo.bMasterState = TRUE;

	return TRUE;
}


void CMainPlc::SetLayOutCfg()
{
	long	i,j			;
	long	nLocalID	=	0;
	long	nModuleCnt = 0;
	long	nEqType = m_stpSma->stLayOutCfg.nEQType;

	switch(nEqType)
	{
	case eEQType_ETCHSTRIP:
		nModuleCnt = MAX_ETCHSTRIP_MODULE_CNT;			
		break;
	case eEQType_ETCH:
		nModuleCnt = MAX_ETCH_MODULE_CNT;
		break;
	case eEQType_STRIP:
		nModuleCnt = MAX_STRIP_MODULE_CNT;
		break;
	case eEQType_EDGE:
		nModuleCnt = MAX_EDGE_MODULE_CNT;
		break;
	case eEQType_PFC:
		nModuleCnt = MAX_PFC_MODULE_CNT;			
		break;
	case eEQType_WRU:
		nModuleCnt = MAX_WRU_MODULE_CNT;
		break;

	case eEQType_LCPI:
		nModuleCnt = MAX_LCPI_MODULE_CNT;
		break;
	case eEQType_LCODF:
		nModuleCnt = MAX_LCODF_MODULE_CNT;
		break;
	case eEQType_LCRW:
		nModuleCnt = MAX_LCRW_MODULE_CNT;
		break;

	default:
		m_LogMaster.AddLog("[SetLayOutCfg] EQ Type is Wrong!! : EQ Type = %d", nEqType);
		break;
	}

	if (nEqType == eEQType_STRIP)		
	{
		nLocalID = m_stpSma->stLayOutCfg.stModCfg[eModuleType_Stripper].nModuleID ;
		
		if(m_stpSma->stLayOutCfg.stModCfg[eModuleType_Stripper].bModuleUsed == TRUE)
		{
			m_LocalUnit[nLocalID].SetConfig(this, &m_stpSma->stLayOutCfg.stModCfg[eModuleType_Stripper], &m_stpSma->stSysData.stMODDataInfo[eModuleType_Stripper], &m_stpSma->stSysRunInfo.stModRunInfo[eModuleType_Stripper]);
			
			m_Log[nLocalID].SetLogConfig(nEqType, 0, eLogPLC);
			
			for(j=0 ; j<MAX_LOG_HS_KIND ; j++)
				m_Log_HandShake[MAX_LOG_HS_KIND+j].SetLogConfig(nEqType, j, eLogHS);	
				//m_Log_HandShake[MAX_LOG_HS_KIND*nModuleCnt+j].SetLogConfig(nEqType, j, eLogHS);	
									
			m_LocalUnit[nLocalID].ThreadAllRun();
		}
	}	
	else
	{
		for( i = 0; i < nModuleCnt; i++ )
		{
			nLocalID = m_stpSma->stLayOutCfg.stModCfg[i+2].nModuleID ;
			
			if(m_stpSma->stLayOutCfg.stModCfg[i+2].bModuleUsed == TRUE)
			{
				m_LocalUnit[nLocalID].SetConfig(this, &m_stpSma->stLayOutCfg.stModCfg[i+2], &m_stpSma->stSysData.stMODDataInfo[i+2], &m_stpSma->stSysRunInfo.stModRunInfo[i+2]);
				
				m_Log[nLocalID].SetLogConfig(nEqType, nLocalID-2,eLogPLC);

			for(j=0 ; j<MAX_LOG_HS_KIND ; j++)			// 2 
				m_Log_HandShake[MAX_LOG_HS_KIND*i+j].SetLogConfig(nEqType, MAX_LOG_HS_KIND*i+j,eLogHS);		// eLogHS == 1 
				
			m_LocalUnit[nLocalID].ThreadAllRun();
			}
		}
	}
}

BOOL CMainPlc::ExitInstance()
{
	for(int i=0;i<MAX_LOCAL_COUNT;i++)
		m_LocalUnit[i].OnTerminate();

	KillThread();
	return TRUE;
}

/////////////////////////////////////////////////////////////////
//	Related Data Link 
//	CIM Data Update : Bit Area
void CMainPlc::OnSetCIM_BitStateToEQ()
{
	short	shCmdPacket[4]	=	{ 0x00, };
	short   shSetData[2]		=	{ 0x00, };
	long	nSetDataSize	=	sizeof(short) * 2;
	
	if ( m_stpSma->stSysRunInfo.bMasterState	) shSetData[0] |= 0x0003 ;		// Alive + Nomal/Abnormal
	if ( m_stpSma->stSysRunInfo.bMasterEditBusy	) shSetData[0] |= 0x0004 ;
	if ( m_stpSma->stSysRunInfo.bJudgeMode		) shSetData[0] |= 0x0008 ;

	if ( m_stpSma->stSysRunInfo.bTimeSetReq		) shSetData[1] |= 0x0001 ;
	if ( m_stpSma->stSysRunInfo.bPMCodeSetReq	) shSetData[1] |= 0x0002 ;
	if ( m_stpSma->stSysRunInfo.bBrokenCodeSetReq) shSetData[1] |= 0x0004 ;
	if ( m_stpSma->stSysRunInfo.bJudgeCodeSetReq) shSetData[1] |= 0x0008 ;

	shCmdPacket[0] = 1;
	shCmdPacket[1] = DevB;		//	Master Status 
	shCmdPacket[2] = B_L1_ALIVE_STATUS;
	shCmdPacket[3] = 32;		

	m_MelLinkMemIF.MelNetSendEx(DevB, B_L1_ALIVE_STATUS, &nSetDataSize, shSetData);
}

//	CIM Req&Rly Data Update : Bit Area
void CMainPlc::OnSetCIM_ReqRlyBitToEQ(long nModuleID)
{
	short   shSetData[2]		=	{ 0x00, };
	long	nSetDataSize	=	sizeof(short) * 2;	

	m_MelLinkMemIF.MelNetSendEx(DevB, B_L2_FROM_CIM_MACHINE_CMD_REQ, &nSetDataSize, shSetData);
}

//	CIM Data Update : Word Area
void CMainPlc::OnSetCIM_WordDataToEQ()
{
	long nIdx	= 0;
	short   shSetData[2]		=	{ 0x00, };
	long	nSetDataSize	=	sizeof(short) * 2;

	shSetData[nIdx]	= (short)m_stpSma->stSysRunInfo.nOnLineMode;	nIdx++;
	shSetData[nIdx]	= (short)(m_stpSma->stSysRunInfo.bMasterState ? 1 : 0);	nIdx++;	

	m_MelLinkMemIF.MelNetSendEx(DevW, W_L1_ONLINE_STATUS, &nSetDataSize, shSetData);
}

////////////////////////////////////////////////////////////////////
//	IPC Signal Function
//	Master PC <-> PLC Time Synchronization
void CMainPlc::C2P_TimeSetSequence(stPlcQueueType sig)
{
	long nLocalNo = 0;
	long nLocalID = 0;
	long nModuleCnt = 0;

	switch(m_stpSma->stLayOutCfg.nEQType)
	{
	case eEQType_ETCHSTRIP:
		if(m_stpSma->stLayOutCfg.nEQSubType == eEQETCHSTRIPSubType_CFITO)
			nModuleCnt = MAX_ETCHSTRIP_SUB_2_MODULE_CNT;
		else
			nModuleCnt = MAX_ETCHSTRIP_MODULE_CNT;
		break;
	case eEQType_ETCH:
		nModuleCnt = MAX_ETCH_MODULE_CNT;
		break;
	case eEQType_STRIP:
		nModuleCnt = MAX_STRIP_MODULE_CNT;
		break;
	case eEQType_EDGE:
		nModuleCnt = MAX_EDGE_MODULE_CNT;
		break;
	case eEQType_PFC:
		nModuleCnt = MAX_PFC_MODULE_CNT;
		break;
	case eEQType_WRU:
		nModuleCnt = MAX_WRU_MODULE_CNT;
		break;
	case eEQType_LCPI:
		nModuleCnt = MAX_LCPI_MODULE_CNT;
		break;
	case eEQType_LCODF:
		nModuleCnt = MAX_LCODF_MODULE_CNT;
		break;
	case eEQType_LCRW:
		nModuleCnt = MAX_LCRW_MODULE_CNT;
		break;
	}

	if (m_stpSma->stLayOutCfg.nEQType == eEQType_STRIP)		
	{
		m_cInternelQue[eModuleType_Stripper].Write(&sig);
	}
	else
	{
		for(nLocalNo=2; nLocalNo < (nModuleCnt + 2); nLocalNo++)
		{
			m_cInternelQue[nLocalNo].Write(&sig);
		}
	}
}

void CMainPlc::C2P_ECOModeChangeSequence(stPlcQueueType sig)
{
	long nLocalNo = 0;
	long nLocalID = 0;
	long nModuleCnt = 0;
	
	switch(m_stpSma->stLayOutCfg.nEQType)
	{
	case eEQType_ETCHSTRIP:
		if(m_stpSma->stLayOutCfg.nEQSubType == eEQETCHSTRIPSubType_CFITO)
			nModuleCnt = MAX_ETCHSTRIP_SUB_2_MODULE_CNT;
		else
			nModuleCnt = MAX_ETCHSTRIP_MODULE_CNT;
		break;
	case eEQType_ETCH:
		nModuleCnt = MAX_ETCH_MODULE_CNT;
		break;
	case eEQType_STRIP:
		nModuleCnt = MAX_STRIP_MODULE_CNT;
		break;
	case eEQType_EDGE:
		nModuleCnt = MAX_EDGE_MODULE_CNT;
		break;
	case eEQType_PFC:
		nModuleCnt = MAX_PFC_MODULE_CNT;
		break;
	case eEQType_WRU:
		nModuleCnt = MAX_WRU_MODULE_CNT;
		break;
	case eEQType_LCPI:
		nModuleCnt = MAX_LCPI_MODULE_CNT;
		break;
	case eEQType_LCODF:
		nModuleCnt = MAX_LCODF_MODULE_CNT;
		break;
	case eEQType_LCRW:
		nModuleCnt = MAX_LCRW_MODULE_CNT;
		break;
	}
	
	if (m_stpSma->stLayOutCfg.nEQType == eEQType_STRIP)		
	{
		m_cInternelQue[eModuleType_Stripper].Write(&sig);
	}
	else
	{
		for(nLocalNo=2; nLocalNo < (nModuleCnt + 2); nLocalNo++)
		{
			m_cInternelQue[nLocalNo].Write(&sig);
		}
	}
}

//	Machine Command Module 분리
void CMainPlc::C2P_MachineCmdSequence(stPlcQueueType sig)
{
	long	nLocalNo = 0;

	nLocalNo = sig.unionData.stMachineCmd.nModuleNo;

	m_cInternelQue[nLocalNo].Write(&sig);
}

//	Alarm Treat Request Module 분리
void CMainPlc::C2P_AlarmTreatSequence(stPlcQueueType sig)
{
	long	nLocal = 0;

	nLocal = sig.unionData.stAlarmClearData.nModuleID;

	m_cInternelQue[nLocal].Write(&sig);
}

void CMainPlc::C2P_OperatorCallSequence(stPlcQueueType sig)
{
	long nLocalNo = 0;
	long nLocalID = 0;
	long nModuleCnt = 0;

	if(sig.unionData.stTerminalMsg.nModuleNo == 99)
	{
		switch(m_stpSma->stLayOutCfg.nEQType)
		{
		case eEQType_ETCHSTRIP:
			if(m_stpSma->stLayOutCfg.nEQSubType == eEQETCHSTRIPSubType_CFITO)
				nModuleCnt = MAX_ETCHSTRIP_SUB_2_MODULE_CNT;
			else
				nModuleCnt = MAX_ETCHSTRIP_MODULE_CNT;
			break;
		case eEQType_ETCH:
			nModuleCnt = MAX_ETCH_MODULE_CNT;
			break;
		case eEQType_STRIP:
			nModuleCnt = MAX_STRIP_MODULE_CNT;
			break;
		case eEQType_EDGE:
			nModuleCnt = MAX_EDGE_MODULE_CNT;
			break;
		case eEQType_PFC:
			nModuleCnt = MAX_PFC_MODULE_CNT;
			break;
		case eEQType_WRU:
			nModuleCnt = MAX_WRU_MODULE_CNT;
			break;
		case eEQType_LCPI:
			nModuleCnt = MAX_LCPI_MODULE_CNT;
			break;
		case eEQType_LCODF:
			nModuleCnt = MAX_LCODF_MODULE_CNT;
			break;
		case eEQType_LCRW:
			nModuleCnt = MAX_LCRW_MODULE_CNT;
			break;
		}

		if (m_stpSma->stLayOutCfg.nEQType == eEQType_STRIP)			
		{
			sig.unionData.stTerminalMsg.nModuleNo = eModuleType_Stripper;
			m_cInternelQue[eModuleType_Stripper].Write(&sig);
		}
		else
		{
			for(nLocalNo=2; nLocalNo < (nModuleCnt + 2); nLocalNo++)
			{
				sig.unionData.stTerminalMsg.nModuleNo = nLocalNo;
				m_cInternelQue[nLocalNo].Write(&sig);
			}
		}
	}
	else
	{
		if(sig.unionData.stTerminalMsg.nModuleNo > eModuleType_Indexer)
		{
			nLocalNo = sig.unionData.stTerminalMsg.nModuleNo;
			m_cInternelQue[nLocalNo].Write(&sig);
		}
	}
}

void CMainPlc::C2P_TerminalMsgSequence(stPlcQueueType sig)
{
	long nLocalNo = 0;
	long nLocalID = 0;
	long nModuleCnt = 0;

	if(sig.unionData.stTerminalMsg.nModuleNo == 99)
	{
		switch(m_stpSma->stLayOutCfg.nEQType)
		{
		case eEQType_ETCHSTRIP:
			if(m_stpSma->stLayOutCfg.nEQSubType == eEQETCHSTRIPSubType_CFITO)
				nModuleCnt = MAX_ETCHSTRIP_SUB_2_MODULE_CNT;
			else
				nModuleCnt = MAX_ETCHSTRIP_MODULE_CNT;
			break;
		case eEQType_ETCH:
			nModuleCnt = MAX_ETCH_MODULE_CNT;
			break;
		case eEQType_STRIP:
			nModuleCnt = MAX_STRIP_MODULE_CNT;
			break;
		case eEQType_EDGE:
			nModuleCnt = MAX_EDGE_MODULE_CNT;
			break;
		case eEQType_PFC:
			nModuleCnt = MAX_PFC_MODULE_CNT;
			break;
		case eEQType_WRU:
			nModuleCnt = MAX_WRU_MODULE_CNT;
			break;
		case eEQType_LCPI:
			nModuleCnt = MAX_LCPI_MODULE_CNT;
			break;
		case eEQType_LCODF:
			nModuleCnt = MAX_LCODF_MODULE_CNT;
			break;
		case eEQType_LCRW:
			nModuleCnt = MAX_LCRW_MODULE_CNT;
			break;
		}

		if (m_stpSma->stLayOutCfg.nEQType == eEQType_STRIP)		
		{
			sig.unionData.stTerminalMsg.nModuleNo = eModuleType_Stripper;
			m_cInternelQue[eModuleType_Stripper].Write(&sig);
		}
		else
		{
			for(nLocalNo=2; nLocalNo < (nModuleCnt + 2); nLocalNo++)
			{
				sig.unionData.stTerminalMsg.nModuleNo = nLocalNo;
				m_cInternelQue[nLocalNo].Write(&sig);
			}
		}
	}
	else
	{
		if(sig.unionData.stTerminalMsg.nModuleNo > eModuleType_Indexer)
		{
			nLocalNo = sig.unionData.stTerminalMsg.nModuleNo;
			m_cInternelQue[nLocalNo].Write(&sig);
		}
	}
}

/*
void CMainPlc::OnOperatorCallSequence(stPlcQueueType sig)
{
	long	nLocal = 0;

	nLocal = sig.unionData.stBuzzerCtrl.nModuleNo;

	m_cInternelQue[nLocal].Write(&sig);
}
*/

void CMainPlc::C2P_BuzzerStopSequence(stPlcQueueType sig)
{
	long	nLocal = 0;

	nLocal = sig.unionData.stBuzzerCtrl.nModuleNo;

	m_cInternelQue[nLocal].Write(&sig);
}

//////////////////////////////////////////////////////
//	Data Link Function
void CMainPlc::C2P_DataChangeSequence(stPlcQueueType sig)
{
	long	i = 0;
	long	nLocalNo = 0;
	long	nModuleCnt = 0;
	BOOL	bFlag[MAX_LAYER1_MODULE_COUNT + 1] = { FALSE, };
	stECIDConfigType	*pECIDCfg = NULL;

	switch(m_stpSma->stLayOutCfg.nEQType)
	{
	case eEQType_ETCHSTRIP:
		if(m_stpSma->stLayOutCfg.nEQSubType == eEQETCHSTRIPSubType_CFITO)
			nModuleCnt = MAX_ETCHSTRIP_SUB_2_MODULE_CNT;
		else
			nModuleCnt = MAX_ETCHSTRIP_MODULE_CNT;
		break;
	case eEQType_ETCH:
		nModuleCnt = MAX_ETCH_MODULE_CNT;
		break;
	case eEQType_STRIP:
		nModuleCnt = MAX_STRIP_MODULE_CNT;
		break;
	case eEQType_EDGE:
		nModuleCnt = MAX_EDGE_MODULE_CNT;
		break;
	case eEQType_PFC:
		nModuleCnt = MAX_PFC_MODULE_CNT;
		break;
	case eEQType_WRU:
		nModuleCnt = MAX_WRU_MODULE_CNT;
		break;
	case eEQType_LCPI:
		nModuleCnt = MAX_LCPI_MODULE_CNT;
		break;
	case eEQType_LCODF:
		nModuleCnt = MAX_LCODF_MODULE_CNT;
		break;
	case eEQType_LCRW:
		nModuleCnt = MAX_LCRW_MODULE_CNT;
		break;
	}

	for( i = 0; i < sig.unionData.stECIDChange.nECCount; i++ )
	{
		pECIDCfg = m_BaseModule.GetECIDConfig(sig.unionData.stECIDChange.stECData[i].nECID);
		if ( pECIDCfg == NULL )	continue;

		bFlag[pECIDCfg->stParamCfg[0].nModuleID] = TRUE;
	}

	for( nLocalNo = 2; nLocalNo < (nModuleCnt + 2) ; nLocalNo++ )
	{
		if ( bFlag[nLocalNo] == TRUE ) 
			m_cInternelQue[nLocalNo].Write(&sig);
	}
}

//	CIM Data Update : Word Area
BOOL CMainPlc::Run()
{
	BOOL				bAlive = TRUE;
	stPlcQueueType		sig;

	while(bAlive)
	{
		memset(&sig,	0x00, sizeof(stPlcQueueType));	
		if ( (m_cMyQueue.GetState()) > 0 )
		{
			m_cMyQueue.Read(&sig);
			switch(sig.nSignal)
			{
			case sigAlarmTreated:
			case sigAlarmResetEvent:	
				C2P_AlarmTreatSequence(sig);
				break;
			case sigTerminalRequest: // Buzzer 없는 Terminal Message
				C2P_TerminalMsgSequence(sig);
				break;
			case sigBuzzerStop:
				C2P_BuzzerStopSequence(sig);
				break;
			case sigOperatorCall:	// Buzzer 있는 Terminal Message
				C2P_OperatorCallSequence(sig);
				break;
			case sigMachineCommand:
				C2P_MachineCmdSequence(sig);
				break;
			case sigTimeSetRequest:
				C2P_TimeSetSequence(sig);
				break;
			case sigEQConstantChange:
				C2P_DataChangeSequence(sig);
				break;
			case sigEcoModeRequest:
				C2P_ECOModeChangeSequence(sig);
				break;
			case sigShutDown:
				Sleep(1000);
				bAlive = FALSE;
				break;

			// Edge Cleaner
			case sigManualCellLoad:
				SetGlassTransferData(&sig);
				m_cInternelQue[eModuleType_Edge].Write(&sig);
				break;
			case sigMNGlassRunStart:
				m_cInternelQue[eModuleType_Edge].Write(&sig); // Manual Cell Start or Cancel
				break;
			}
		}

		SetCIMAlive();

		CheckSyncTimeset();

		Sleep(10);
	}
	return TRUE;
}

//	Plc Main
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	
	long	uId = -1;
	char* pszUnitName = (char *)lpCmdLine;
	if (pszUnitName != NULL) sscanf(pszUnitName, "%d", &uId);

	//...... check for previous instance of this application
	HANDLE hMutexInstance = CreateMutex(NULL, FALSE, "___Plc_Control___"); 
	DWORD rval = WaitForSingleObject(hMutexInstance, 0L);

	if (rval==WAIT_FAILED || rval==WAIT_TIMEOUT)
	{
		MessageBox(0, "InlinePlcIf is Already running !!", "InlinePlc", MB_OK);
		return FALSE;
	}

	CMainPlc	cPlcIf;
	if(!cPlcIf.InitInstance())
	{
//		MessageBox(NULL,"PLC Task Starting Error!!\r\n\r\nError창 확인버튼을 누른후 바탕화면의 [FPDCIM] ICON을 다시 실행시켜주세요!!","PLC Initial Error",MB_OK);
		return 0;
	}
	cPlcIf.Run();
	cPlcIf.ExitInstance();

	return 0;
}


void CMainPlc::UpdateUnloadGlassData(stPanelInfoType *pGlsInfo)
{
	memcpy(&m_stpSma->stSysRunInfo.stULGlassData, pGlsInfo, sizeof(stPanelInfoType));
}

void CMainPlc::OnTowerLampControl()
{
	short	shTower = 0;
	long	nSetDataSize	=	sizeof(short);

	if ( m_stpSma->stSysRunInfo.stTowerControl.bControlMode )
		shTower |= 0x0001;
	else
		shTower &= 0xFFFE;

	if ( m_stpSma->stSysRunInfo.stTowerControl.bRed )
		shTower |= 0x0002;
	else
		shTower &= 0xFFFD;

	if ( m_stpSma->stSysRunInfo.stTowerControl.bYellow )
		shTower |= 0x0004;
	else
		shTower &= 0xFFFB;

	if ( m_stpSma->stSysRunInfo.stTowerControl.bGreen )
		shTower |= 0x0008;
	else
		shTower &= 0xFFF7;

	if ( m_stpSma->stSysRunInfo.stTowerControl.bBuzzer )
		shTower |= 0x0040;
	else
		shTower &= 0xFFBF;

	m_MelLinkMemIF.MelNetSendEx(DevW, W_L1_TOWERLAMP_CONTROL_DATA, &nSetDataSize, &shTower );
}

void CMainPlc::SetCIMAlive()
{
	long nSize = sizeof(short);

	// CIM Alive State Write ( ~ 32767 = 7FFF)
	if(m_shCIMAlive > 32000) m_shCIMAlive = 0;
	else m_shCIMAlive++;
	
	m_MelLinkMemIF.MelNetSendEx(DevW, W_L1_CIM_ALIVE_STATE, &nSize, &m_shCIMAlive);
}


//@ PLC, CIM간 CPU 클럭 상이로 인하여 매일 0시0분0초에 Timeset Event 발생한다.
void CMainPlc::CheckSyncTimeset()
{
	stPlcQueueType sig;
	memset(&sig, 0x00, sizeof(stPlcQueueType));
	
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	
	WORD	wYear	=	systime.wYear;
	WORD	wMonth	=	systime.wMonth;
	WORD	wDay	=	systime.wDay;
	WORD	wHour	=	systime.wHour;
	WORD	wMin	=	systime.wMinute;
	WORD	wSec	=	systime.wSecond;
	WORD	wMMSec	=	systime.wMilliseconds;
	
	long	nLocalNo = 0;
	
	//@ 매일 23시 59분 30초에 Time Sync Event 발생.
	if(wHour == 23 && wMin == 59 && wSec == 30)
	{
		if (m_nTimesetDay != wDay )
		{
			sig.nSignal = sigTimeSetRequest;
			C2P_TimeSetSequence(sig);
			
			//@ 23시 59분 30초에 Module별 Running Data Read..
			for (nLocalNo = 2; nLocalNo < MAX_LAYER1_MODULE_COUNT; nLocalNo++)
			{
				if(m_stpSma->stLayOutCfg.stModCfg[nLocalNo].bModuleUsed == TRUE)
					m_LocalUnit[nLocalNo].GetEQModuleRunData();
			}
		}
		
		m_nTimesetDay = wDay;
	}
}

void CMainPlc::SetGlassTransferData(stPlcQueueType* sig)
{
	short sGlsTransData[154];
	long nSize					= sizeof(short)*154;
	stPanelInfoType	*pGlsInfo	= NULL;

	pGlsInfo = &sig->unionData.stGlassUnscrapData.stPanelInfo;

	m_BaseModule.ConvertPanelDataToTransferData(pGlsInfo, sGlsTransData);
	m_MelLinkMemIF.MelNetSendEx(DevW, W_L1_UNSCRAP_RLY_DATA, &nSize, sGlsTransData);	// Unscrap Data 영역과 동일적용
}

void CMainPlc::GetGlassTransferData(stPanelInfoType* pGlsInfo)
{
	short sGlsTransData[154];
	long nSize					= sizeof(short)*154;

	m_MelLinkMemIF.MelNetReceiveEx(DevW, W_L2_GLS_TRANS_DATA_TO_MASTER, &nSize, sGlsTransData);
	m_BaseModule.ConvertTransferDataToPanelData(pGlsInfo, sGlsTransData);
}
