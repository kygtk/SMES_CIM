  // ControlModule.cpp: implementation of the ControlModule class.
//
//////////////////////////////////////////////////////////////////////

#include "BaseModule.h"
#include "Thread.h"
#include "MainPlc.h"
#include "../include/ConstDefine.h"
#include "../include/NotifyECode.h"
#include "stdio.h"
///////////////////////////////////////////////////////////////////
// Test ohanaya 2011.06.01 -delete it
void TracePrint(char *pFormat, ...)
{
    char szBuf[1024];
    va_list vlMarker;

    va_start( vlMarker, pFormat );
    vsprintf(szBuf, pFormat, vlMarker);
    OutputDebugString(szBuf);
}
///////////////////////////////////////////////////////////////////

CShareCommonData::CShareCommonData()
{
	m_nPPID			= 0;
	m_nCmdData		= 0;
	m_nReqID		= 0;
	m_shBitAddr		= 0;	
	m_shDataAddr	= 0;
	m_shSetData		= 0;
	m_shTempAddr	= 0;
	m_bWaitTime		= FALSE;
	m_nElaspedTime	= 0;
	
	memset(m_nUniquID,0x00,sizeof(m_nUniquID));
	m_nAlarmID		=	0;
	m_nAlarmCode	=	0;
	m_nModuleID		=	0;
	
	memset(m_shCmdPacket,0x00,sizeof(m_shCmdPacket));

	// Add 2011.10.07 
	memset(m_szHPanelID,0x00,sizeof(m_szHPanelID));
	memset(m_szRCode,0x00,sizeof(m_szRCode));
	m_nOwnGlassNo = 0;
	
	memset(&m_pData, 0x00, sizeof(stECIDChangeType));
	memset(&m_stpProcEndData, 0x00, sizeof(stProcessEndDataType));

	memset(&m_stECOData, 0x00, sizeof(stECOChangeType));

}

CShareCommonData::~CShareCommonData()
{
	
}

//////////////////////////////////////////////////////////////////////
// Operation Thread
//////////////////////////////////////////////////////////////////////
UINT ThreadUpdateMelSecArea(LPVOID pParam)
{
	CThread	*pThread	 = (CThread*)*((UINT*)pParam);
	CBaseModule	*pCBaseModule = (CBaseModule*)*((UINT*)pParam+1);
	
	if (pThread == NULL)
		return	-1;
	
	if (pCBaseModule == NULL) 
		return -1;
	
	pThread->m_bThreadRun = TRUE;
	
	pCBaseModule->TrsUpdateMelSecArea();
	Sleep(100);
	return 0;
}

UINT ThreadSequenceProcess(LPVOID pParam)
{
	CThread	*pThread	 = (CThread*)*((UINT*)pParam);
	CBaseModule	*pCBaseModule = (CBaseModule*)*((UINT*)pParam+1);
	
	if (pThread == NULL)
		return	-1;
	
	if (pCBaseModule == NULL) 
		return -1;
	
	pThread->m_bThreadRun = TRUE;
	
	pCBaseModule->TrsSequenceProcessing();
	Sleep(100);
	return 0;
}

UINT ThreadSequenceProcessEnd(LPVOID pParam)
{
	CThread	*pThread	 = (CThread*)*((UINT*)pParam);
	CBaseModule	*pCBaseModule = (CBaseModule*)*((UINT*)pParam+1);
	
	if (pThread == NULL)
		return	-1;
	
	if (pCBaseModule == NULL) 
		return -1;
	
	pThread->m_bThreadRun = TRUE;
	
	pCBaseModule->TrsSequenceProcessEnd();
	Sleep(100);
	return 0;
}

UINT ThreadQueueProcess(LPVOID pParam)
{
	CThread	*pThread	 = (CThread*)*((UINT*)pParam);
	CBaseModule	*pCBaseModule = (CBaseModule*)*((UINT*)pParam+1);
	
	if (pThread == NULL)
		return	-1;
	
	if (pCBaseModule == NULL) 
		return -1;
	
	pThread->m_bThreadRun = TRUE;
	
	pCBaseModule->TrsQueueCheckProcessing();
	Sleep(100);
	return 0;
}

UINT ThreadUpdateTimer(LPVOID pParam)
{
	CThread	*pThread	 = (CThread*)*((UINT*)pParam);
	CBaseModule	*pCBaseModule = (CBaseModule*)*((UINT*)pParam+1);
	
	if (pThread == NULL)
		return	-1;
	
	if (pCBaseModule == NULL) 
		return -1;
	
	pThread->m_bThreadRun = TRUE;
	
	pCBaseModule->TrsBaseTimer();
	Sleep(100);
	return 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBaseModule::CBaseModule()
{
	for(int i=0;i<MAX_SEQUENCE_EVENT;i++)
	{
		m_ShCommData_Master[i] = NULL;
		m_ShCommData_Master[i] = new CShareCommonData;
		
		m_ShCommData_Local[i] = NULL;
		m_ShCommData_Local[i] = new CShareCommonData;
	}
	
	m_bBaseThread		=	FALSE;
	
	m_bSetRecoveryCmd	= FALSE;
	
	m_shPLCMAlive			=	0;
	m_nVCRMode				=	0;
	m_nKeyInWaitTime		=	0;
	memset(m_bHSLogUpperState,0x00,sizeof(m_bHSLogUpperState));
	memset(m_bHSLogLowerState,0x00,sizeof(m_bHSLogLowerState));
	
	memset(m_bHSLogUpper1,0x00,sizeof(m_bHSLogUpper1));
	memset(m_bHSLogUpper2,0x00,sizeof(m_bHSLogUpper2));
	memset(m_bHSLogUpperOld1,0x00,sizeof(m_bHSLogUpperOld1));
	memset(m_bHSLogUpperOld2,0x00,sizeof(m_bHSLogUpperOld2));
	
	memset(m_bHSLogLower1,0x00,sizeof(m_bHSLogLower1));
	memset(m_bHSLogLower2,0x00,sizeof(m_bHSLogLower2));
	memset(m_bHSLogLowerOld1,0x00,sizeof(m_bHSLogLowerOld1));
	memset(m_bHSLogLowerOld2,0x00,sizeof(m_bHSLogLowerOld2));
	
	for(i=0;i<3;i++)
	{
		m_bProgramFirstStart1[i] = TRUE; // HansShake Log용 : Program 최초 기동관련 최초 Event변화 Skip
		m_bProgramFirstStart2[i] = TRUE; // HansShake Log용 : Program 최초 기동관련 최초 Event변화 Skip
	}
	
	memset(m_szMSCHPanelID,0x20,sizeof(m_szMSCHPanelID));
	
	memset(m_bMDStartforFlowPress,0x00,sizeof(m_bMDStartforFlowPress));
	memset(m_bMDStartforTemp,0x00,sizeof(m_bMDStartforTemp));

	memset(m_bHSValid_Upper1, 0x00, sizeof(m_bHSValid_Upper1));
	memset(m_bHSValid_Upper2, 0x00, sizeof(m_bHSValid_Upper2));
	memset(m_bHSValid_Upper3, 0x00, sizeof(m_bHSValid_Upper3));

	memset(m_nHSValid_SemesUniqID, 0x00, sizeof(m_nHSValid_SemesUniqID));

	memset(m_bEQNetworkState, 0x00, sizeof(m_bEQNetworkState));

	memset(m_nGECDEventID, 0x00, sizeof(m_nGECDEventID));

	
	memset(m_bOldActionState, 0x00, sizeof(m_bOldActionState)); // shseo 2010_1007 - add
	// kmi
	m_nMaxCount		= 1024;
	m_nOffset		= 0;
	m_nAlarmCount	= 0;
	m_pbAlarmHappen	=	NULL;
	
	m_pbAlarmHappen	=	new bool[m_nMaxCount];
	memset(m_pbAlarmHappen, 0x00, sizeof(bool) * m_nMaxCount);
	
	OnAlarmOffset(1);  // Alarm Start Address
	
	SysLib::SmaInitialize(PLC_TASK_ID);
}

CBaseModule::~CBaseModule()
{
	for(int i=0;i<MAX_SEQUENCE_EVENT;i++)
	{
		if(m_ShCommData_Master[i] != NULL)
		{
			delete m_ShCommData_Master[i];
			m_ShCommData_Master[i] = NULL;
		}
		
		if(m_ShCommData_Local[i] != NULL)
		{
			delete m_ShCommData_Local[i];
			m_ShCommData_Local[i] = NULL;
		}
		
	}
	
	if(m_pbAlarmHappen != NULL)
	{
		delete[] m_pbAlarmHappen;
		m_pbAlarmHappen = NULL;
	}
	
}

void CBaseModule::TrsBaseTimer()
{
	while(m_bBaseThread)
	{
		for(long nIdx=1; nIdx<MAX_SEQUENCE_EVENT; nIdx++)
		{
			if( m_ShCommData_Master[nIdx]->m_TimeCheck.IsStartedTimer() == TRUE )
			{
				if( m_ShCommData_Master[nIdx]->m_TimeCheck.GetTimerAfterStart() > m_ShCommData_Master[nIdx]->m_TimeCheck.GetLimitedTime()) // 60 sec
					m_ShCommData_Master[nIdx]->m_TimeCheck.CountResest();
				else
					m_ShCommData_Master[nIdx]->m_TimeCheck.Count();
			}

			if( m_ShCommData_Local[nIdx]->m_TimeCheck.IsStartedTimer() == TRUE )
			{
				if( m_ShCommData_Local[nIdx]->m_TimeCheck.GetTimerAfterStart() > m_ShCommData_Master[nIdx]->m_TimeCheck.GetLimitedTime()) // 60 sec
					m_ShCommData_Local[nIdx]->m_TimeCheck.CountResest();
				else
					m_ShCommData_Local[nIdx]->m_TimeCheck.Count();
			}
		}

		Sleep(MAX_RUN_TIME_SET);
	}
}

void CBaseModule::TrsUpdateMelSecArea()
{
	while(m_bBaseThread)
	{
		/*-----------------------------------*/
		// 2007.07.15 PCJ
		// Melsec Update 순서 재 정의 
		// 1) ER Area
		// 2) Word Area
		// 3) Bit Area
		/*-----------------------------------*/
		
		OnUpdateERArea();
		::Sleep(10);
		
		OnUpdateWordArea();
		::Sleep(10);
		
		OnUpdateBitArea();		
		::Sleep(10);
	}
}

void CBaseModule::OnUpdateERArea()
{
	GetDataLink_GlassTrackingData();
	::Sleep(1);

	GetDataLink_CurProcessingData();
	::Sleep(1);
	
	OnECIDDataUpdateforStart();
	::Sleep(1);

	//@ FIC Not Used
	//GetDataLink_ActionLogData();
	//::Sleep(1);
}

void CBaseModule::OnUpdateWordArea()
{
	CheckAliveState();
	::Sleep(1);

	GetEquipmentData();						// To Upper, To Lower, EQ State, Glass Summary
	::Sleep(1);
	
	GetStatusData();
	::Sleep(1);

	//GetProcessMode();
	::Sleep(1);

	GetUsingTankNo();
	::Sleep(1);
	
	GetECOModeState();
	::Sleep(1);

	CheckHandShakeValidData();
	::Sleep(1);

	CheckEQNetworkStateCheck();
	::Sleep(1);

	CheckGECDCrackEvent();
	::Sleep(1);

	switch (m_stpSma->stLayOutCfg.nEQType)
	{
	case eEQType_ETCHSTRIP:
		switch (m_stpSma->stLayOutCfg.nEQSubType)		
		{
		case eEQETCHSTRIPSubType_RW:
		case eEQETCHSTRIPSubType_PIXELCLN:					
			break;
			
		case eEQETCHSTRIPSubType_GATE:
		case eEQETCHSTRIPSubType_PIXEL:		
			GetEpdData();	// EPD가 있는 Etcher 설비만
			GetEpdTime();   // EPD Event Time
			break;
		}
		break;
		
		case eEQType_ETCH:
			GetEpdData();	// EPD가 있는 Etcher 설비만
			GetEpdTime();   // EPD Event Time  
			break;
	}

}

void CBaseModule::GetEpdData()
{
	short   shReadData[MAX_EPD_CTRL_COUNT]	= { 0x00, };
	long	nReadDataSize	= sizeof(short) * MAX_EPD_CTRL_COUNT;
	long	nDevType		= 0;
	short	shAddr = 0;	
	
	//	Melsec Read
	m_pParent->m_MelERMemIF.MelNetReceiveEx(DevW, W_L2_EPD_VALUE, &nReadDataSize, shReadData);

	for(int nBathCount = 0; nBathCount < MAX_EPD_CTRL_COUNT; nBathCount++)
	{
		m_stpModRunInfo->stCurProcData.stCurEpdData[nBathCount].nCurEpdValue = shReadData[nBathCount];		
		//1616
		//Test Start
		//m_stpModRunInfo->stCurProcData.stCurEpdData[nBathCount].nCurEpdValue = rand()%100;
		//Test End			 
	}
}

// EPD Event Time   ohanaya 2011.05.22
void CBaseModule::GetEpdTime()
{
	short   shReadData[3]	= { 0x00, };
	short	shReadDataSize	= sizeof(short) * 3;    
	long	nReadDataSize	= sizeof(short) * 3;            
	short	shTemp			= 0;
	char	sTemp1[16 + 1]	= { '0', };
	char	sTemp2[16 + 1]	= { '0', };
	char	sTemp3[2];
	int		iCount			= 16;
	int		iIdx[4]			= {0,};
	short	shAddr			= 0;
	
	memset(sTemp3,		0x00,sizeof(sTemp3));
	
	//	Melsec Read
	
    m_pParent->m_MelERMemIF.MelNetReceiveEx(DevW, W_L2_EPD_TIME_EVENT, &nReadDataSize, shReadData);
    //m_pParent->m_MelLinkMemIF.MelNetReceive(DevW, W_L2_EPD_TIME_EVENT, &shReadDataSize, shReadData);
	
	// Test ---- start
	//shReadData[0] = 513;
	//shReadData[1] = 2052;
	//shReadData[2] = 513;
	// Test ---- End
	
    
	for(int nBathCount = 0; nBathCount < (MAX_EPD_CTRL_COUNT / 2); nBathCount++)
	{
		
		shTemp = shReadData[nBathCount];
		
		for(iIdx[0] = iCount ; iIdx[0] >= 0 ; --iIdx[0])
		{
			sTemp1[iIdx[0]] = '0' + (char) (shTemp & 1);
			shTemp		    = shTemp >> 1;
		}
		
		for (iIdx[0] = iCount, iIdx[1] = 0 ; iIdx[0] >= 0, iIdx[1] < iCount ; --iIdx[0], iIdx[1]++)
		{
			sTemp2[iIdx[1]] = sTemp1[iIdx[0]];
		}
		
        // Test ohanaya - delete it
        //TracePrint("Bit 16: Bath %d : %s\n",nBathCount,sTemp2);
        // Test
		
        for(iIdx[0] = 0 ; iIdx[0] < iCount ; iIdx[0]++)
        {
            if (iIdx[0] >= 0 && iIdx[0] < 4)	// 0 ~ 3 bit 사용
			{
				
				sprintf(sTemp3,"%c",sTemp2[iIdx[0]]);
				
				if (nBathCount == 0)        iIdx[3] = 0;
				else if (nBathCount == 1)   iIdx[3] = 2;
				else iIdx[3] = 4;
				
				m_stpModRunInfo->stCurProcData.stCurEpdData[iIdx[3]].nCurEpdTime[iIdx[0]] = atoi(sTemp3);
				
				
			}
			
			if (iIdx[0] >= 8 && iIdx[0] < 12)	// 8 ~ 11 bit 사용
			{
				iIdx[2] = iIdx[0] % 4;
				
				sprintf(sTemp3,"%c",sTemp2[iIdx[0]]);
				
				if (nBathCount == 0)        iIdx[3] = 1;
				else if (nBathCount == 1)   iIdx[3] = 3;
				else iIdx[3] = 5;
				
				m_stpModRunInfo->stCurProcData.stCurEpdData[iIdx[3]].nCurEpdTime[iIdx[2]] = atoi(sTemp3);  
				
			}
			
			
            memset(sTemp3,		0x00,sizeof(sTemp3));
        }
		// Test ohanaya - delete it
		//TracePrint("ID: Bath %d : %s\n",nBathCount,
		//	m_stpModRunInfo->stCurProcData.stCurEpdData[iIdx[3]].szPanelID);
		// Test
	}
	

}

void CBaseModule::OnUpdateBitArea()
{
	GetHandShakeIO();
	::Sleep(1);

	GetDataLink_BitState();
	::Sleep(1);

	m_pParent->OnSetCIM_BitStateToEQ();
	::Sleep(1);
}

void CBaseModule::TrsSequenceProcessing()
{
	long nStationNo = 0;

	while(m_bBaseThread)
	{
		//	Processing Event 처리
		// 2010-08-24 add
		if ( m_ShCommData_Master[eP2C_GlassSendFail]->m_bWaitTime == FALSE && m_stpModRunInfo->stEQCmdRlyEvtIO.bGlassSendFail == TRUE)
			P2C_GlassSendFailSequence();
		
		if ( m_ShCommData_Master[eP2C_GlassScrap]->m_bWaitTime == FALSE && m_stpModRunInfo->stEQCmdRlyEvtIO.bGlassScrap == TRUE )
			P2C_GlassScrapSequence();
		
		if ( m_ShCommData_Master[eP2C_GlassUnscrap]->m_bWaitTime == FALSE && m_stpModRunInfo->stEQCmdRlyEvtIO.bGlassUnscrap == TRUE )
			P2C_GlassUnscrapSequence();
		
		if ( m_ShCommData_Master[eP2C_GlassJudgement]->m_bWaitTime == FALSE && m_stpModRunInfo->stEQCmdRlyEvtIO.bGlassJudgement == TRUE )
			P2C_GlassJudgementSequence();
		
		if ( m_ShCommData_Master[eP2C_RecipeDown]->m_bWaitTime == FALSE && m_stpModRunInfo->stEQCmdRlyEvtIO.bRecipeDownReq == TRUE )
			P2C_RecipeDownloadSequence();
		
		if ( m_ShCommData_Master[eP2C_ECIDChange]->m_bWaitTime == FALSE && m_stpModRunInfo->stDataChangeRlyEvtIO.bECIDEvtReq == TRUE )
			P2C_ECIDChangeSequence();
		
		if ( m_ShCommData_Master[eP2C_TEMPChange]->m_bWaitTime == FALSE && m_stpModRunInfo->stDataChangeRlyEvtIO.bTemperatureEvtReq == TRUE )
			P2C_TEMPChangeSequence();
		
		if ( m_ShCommData_Master[eP2C_PPIDCheck]->m_bWaitTime == FALSE && m_stpModRunInfo->stDataChangeRlyEvtIO.bPPIDValidationReq == TRUE )
			P2C_PPIDValidationSequence();
		
		if ( m_ShCommData_Master[eP2C_ProcessEnd]->m_bWaitTime == FALSE && m_stpModRunInfo->stDataChangeRlyEvtIO.bProcessEndEvtReq == TRUE )
			P2C_ProcessEndChangeSequence();
		
		if ( m_ShCommData_Master[eP2C_HS_NRTimeOver]->m_bWaitTime == FALSE && m_stpModRunInfo->stEQCmdRlyEvtIO.bHS_NR_TimeOver == TRUE )
			P2C_HSTimeOverSequence();
		
		if ( m_ShCommData_Master[eP2C_HS_BCTimeOver]->m_bWaitTime == FALSE && m_stpModRunInfo->stEQCmdRlyEvtIO.bHS_BC_TimeOver == TRUE )
			P2C_HSTimeOverSequence();
				
		if ( m_stpSma->stSysRunInfo.nRecoveryMode == 1 || m_stpSma->stSysRunInfo.nRecoveryMode == 2	)
		{
			if ( m_bSetRecoveryCmd == FALSE )
			{
				C2P_RecoveryModeSequence(); //
				m_bSetRecoveryCmd = TRUE;
			}
		}
		else
			m_bSetRecoveryCmd = FALSE;
		
		//	Alarm Event 처리
		if ( m_ShCommData_Master[eP2C_AlarmOccured]->m_bWaitTime == FALSE && m_stpModRunInfo->stEQCmdRlyEvtIO.bAlarmOccured == TRUE )
			P2C_AlarmOccuredSequence();
		
		if ( m_ShCommData_Master[eP2C_AlarmTreated]->m_bWaitTime == FALSE && m_stpModRunInfo->stEQCmdRlyEvtIO.bAlarmTreated == TRUE )
			P2C_AlarmTreatedSequence();
	
		Sleep(10);
	}
}

void CBaseModule::TrsSequenceProcessEnd()
{
	while(m_bBaseThread)
	{
		//	Processing Event Wait Time 처리

		// 2010-08-24 add
		if ( m_ShCommData_Master[eP2C_GlassSendFail]->m_bWaitTime == TRUE)
			P2C_GlassSendFailSequenceEnd();
		
		if (m_ShCommData_Master[eP2C_GlassScrap]->m_bWaitTime == TRUE)
			P2C_GlassScrapSequenceEnd();
		
		if (m_ShCommData_Master[eP2C_GlassUnscrap]->m_bWaitTime == TRUE)
			P2C_GlassUnscrapSequenceEnd();
		
		if (m_ShCommData_Master[eP2C_GlassJudgement]->m_bWaitTime == TRUE)
			P2C_GlassJudgementSequenceEnd();
		
		if (m_ShCommData_Master[eP2C_PPIDCheck]->m_bWaitTime == TRUE)
			P2C_PPIDValidationSequenceEnd();
		
		if (m_ShCommData_Master[eP2C_ECIDChange]->m_bWaitTime == TRUE)
			P2C_ECIDChangeSequenceEnd();
		
		if (m_ShCommData_Master[eP2C_TEMPChange]->m_bWaitTime == TRUE)
			P2C_TEMPChangeSequenceEnd();
		
		if (m_ShCommData_Master[eP2C_RecipeDown]->m_bWaitTime == TRUE)
			P2C_RecipeDownloadSequenceEnd();
		
		if (m_ShCommData_Master[eP2C_ProcessEnd]->m_bWaitTime == TRUE)
		{
			P2C_ProcessEndChangeSequenceEnd();
		}

		if (m_ShCommData_Master[eP2C_ManualCellRoad]->m_bWaitTime == TRUE)
			P2C_ManualCellLoadSequenceEnd();
		
		if ( m_ShCommData_Master[eP2C_HS_NRTimeOver]->m_bWaitTime == TRUE 
			|| m_ShCommData_Master[eP2C_HS_BCTimeOver]->m_bWaitTime == TRUE)
			P2C_HSTimeOverSequenceEnd();
		
		//--------- Queue Check Time 대기 완료 ----------------------//
		
		if (m_ShCommData_Master[eC2P_AlarmTreated]->m_bWaitTime == TRUE)
			C2P_AlarmTreatSequenceEnd();
		
		if (m_ShCommData_Master[eC2P_AlarmResetEvent]->m_bWaitTime == TRUE)
			C2P_AlarmResetSequenceEnd();
		
		if (m_ShCommData_Master[eC2P_MachineCommand]->m_bWaitTime == TRUE)
			C2P_MachineCmdSequenceEnd();
		
		if (m_ShCommData_Master[eC2P_TimeSet]->m_bWaitTime == TRUE)
			C2P_TimeSetSequenceEnd();
		
		if (m_ShCommData_Master[eC2P_OperatorCall]->m_bWaitTime == TRUE)
			C2P_OperatorCallSequenceEnd();
		
		if (m_ShCommData_Master[eC2P_TerminalMsg]->m_bWaitTime == TRUE)
			C2P_TerminalMsgSequenceEnd();

		if (m_ShCommData_Master[eC2P_ECIDChange]->m_bWaitTime == TRUE)
			C2P_ECIDChangeSequenceEnd();

		if (m_ShCommData_Master[eC2P_ECOModeChange]->m_bWaitTime == TRUE)
			C2P_ECOModeChangeSequenceEnd();

		

		//--------- Event Check Time 대기 완료 ----------------------//
		
		if (m_ShCommData_Master[eP2C_AlarmOccured]->m_bWaitTime == TRUE)
			P2C_AlarmOccuredSequenceEnd();
		
		if (m_ShCommData_Master[eP2C_AlarmTreated]->m_bWaitTime == TRUE)
			P2C_AlarmTreatedSequenceEnd();
		
		if (m_ShCommData_Master[eC2P_RecoveryMode]->m_bWaitTime == TRUE)
			C2P_RecoveryModeSequenceEnd();
		
		if (m_ShCommData_Master[eC2P_BuzzerStop]->m_bWaitTime == TRUE)
			C2P_BuzzerStopSequenceEnd();
		
		if (m_ShCommData_Master[eMNCellRead]->m_bWaitTime == TRUE)
			C2P_MNCellReadSequenceEnd();
		
		if (m_ShCommData_Master[eMNCellValid]->m_bWaitTime == TRUE)
			P2C_MNCellValidCheckSequenceEnd();
		
		if(m_ShCommData_Master[eMNCellCancel]->m_bWaitTime == TRUE)
			P2C_MNCellCancelSequenceEnd();
		
		if(m_ShCommData_Master[eMNCellJudge]->m_bWaitTime == TRUE)
			C2P_MNCellJudgeFromHSTSequenceEnd();

		if(m_ShCommData_Master[eP2C_VCRReadingNG]->m_bWaitTime == TRUE)
			P2C_VCRReadingFailSequenceEnd();

		Sleep(10);
	}
}

void CBaseModule::TrsQueueCheckProcessing()
{
	while(m_bBaseThread)
	{
		OnQueueCheck();
		Sleep(10);
	}
}
void CBaseModule::OnQueueCheck()
{
	stPlcQueueType	sig;
	memset(&sig, 0x00, sizeof(stPlcQueueType));
	
	if ( m_cMyQue.GetState() > 0 )
	{
		m_cMyQue.Read(&sig);
		switch(sig.nSignal)
		{
		case sigAlarmTreated:
			C2P_AlarmTreatSequence(sig.unionData.stAlarmClearData);//
			//AlarmHappenDataInit();
			break;
		case sigAlarmResetEvent:
			m_ShCommData_Master[eC2P_AlarmResetEvent]->m_bWaitTime = TRUE;
			m_ShCommData_Master[eC2P_AlarmResetEvent]->m_TimeCheck.StartTimer();
			break;
		case sigTerminalRequest:
			C2P_TerminalMsgSequence(sig.unionData.stTerminalMsg);
			break;
		case sigBuzzerStop:
			C2P_BuzzerStopSequence(sig.unionData.stBuzzerCtrl);
			break;
		case sigOperatorCall:
			C2P_OperatorCallSequence(sig.unionData.stTerminalMsg);
			break;
		case sigMachineCommand:
			C2P_MachineCmdSequence(sig.unionData.stMachineCmd);
			break;
		case sigTimeSetRequest:
			C2P_TimeSetSequence();
			break;
		case sigEcoModeRequest:
			C2P_ECOModeChangeSequence(sig.unionData.stECOChange);
			break;			
		case sigEQConstantChange:
			//C2P_DataChangeSequence(sig.unionData.stECIDChange);//
			C2P_ECIDChangeSequence(sig.unionData.stECIDChange);
			break;
		case sigManualCellLoad:
			C2P_MNCellReadSequence();
			break;
		case sigMNGlassRunStart:
			C2P_MNCellJudgeFromHSTSequence(sig.unionData.stMachineCmd.nCmdData); // Manual Cell Start or Cancel
			break;
		}
	}
}

void CBaseModule::CheckAliveState()
{
	short	shAddr		= 0;
	short	shData		= 0;
	long	nSize		= sizeof(short);
	// PLC Alive Check
	GetLocalDevBWAddr(eCheckAliveState, m_stpModRunInfo->nModuleID, 0, W_L2_PLC_ALIVE_STATE);	
	shAddr = m_ShCommData_Local[eCheckAliveState]->m_shDataAddr;
	
	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, shAddr, &nSize, &shData);
	
	if(m_ShCommData_Master[eCheckAliveState]->m_bWaitTime == TRUE
		&& m_ShCommData_Master[eCheckAliveState]->m_TimeCheck.MoreThan(3000)
		&& shData == m_shPLCMAlive)
	{
		m_stpModRunInfo->stOPModeIO.bAlive = FALSE;
		m_ShCommData_Master[eCheckAliveState]->m_bWaitTime = FALSE;
	}
	
	if(shData == m_shPLCMAlive )
	{
		m_ShCommData_Master[eCheckAliveState]->m_bWaitTime = TRUE;
		m_ShCommData_Master[eCheckAliveState]->m_TimeCheck.StartTimer();
	}
	else if(shData != m_shPLCMAlive)
	{
		m_ShCommData_Master[eCheckAliveState]->m_bWaitTime = FALSE;
		m_stpModRunInfo->stOPModeIO.bAlive = TRUE;
		m_shPLCMAlive = shData;
	}
}

void CBaseModule::GetProcessMode()
{
	short	shAddr		= 0;
	short	shData		= 0;
	short	shOldValue	= 0;	// Change Check
	long	nSize		= sizeof(short);

	shOldValue = static_cast<short> (m_stpSma->stSysRunInfo.nProcessMode);

	GetLocalDevBWAddr(eProcessMode, m_stpModRunInfo->nModuleID, 0, W_L2_PROCESS_MODE);	
	shAddr = m_ShCommData_Local[eProcessMode]->m_shDataAddr;
	
	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, shAddr, &nSize, &shData);
	
    if(shData != shOldValue)
   	    m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Process Mode Changed!! ([%02d -> %02d])",
			m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, shOldValue, shData);
	
	// Update Prcess Mode
	m_stpSma->stSysRunInfo.nProcessMode = shData;
}

void CBaseModule :: GetUsingTankNo()
{
	short	shAddr		= 0;
	short	shData		= 0;
	short	shOldValue	= 0;
	long	nSize		= sizeof(short);
	
	GetLocalDevBWAddr(eUsingTankNo, m_stpModRunInfo->nModuleID , 0 , W_L2_CURRENT_USING_TANK_NO);

	shAddr = m_ShCommData_Local[eUsingTankNo]->m_shDataAddr;
	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, shAddr, &nSize, &shData);
	m_stpSma->stSysRunInfo.stModRunInfo[m_stpModRunInfo->nModuleID].nUsingTankNo = shData;
}

//@ [FIC] Not Used
void CBaseModule :: GetECOModeState()
{
/*
	long	nDataSize		=	sizeof(short);

	long	nModuleID		=	0;
	long	nEOMD			=	0;
	long	nEOV			=	0;
	long	nIndex			=	0;

	long	nEoid_EcoModeIdx=	0;

	short	shEOMDDataAddr	=   0;
	short	shEOVDataAddr	=   0;
	short	shECO_EOMD		=	0;
	short	shECO_EOV		=	0;

	GetLocalDevBWAddr(eP2C_ECOModeChange, m_stpModCfg->nModuleID, 0, W_L2_ECO_MODE_EOMD_EVENT);	
	shEOMDDataAddr = m_ShCommData_Local[eP2C_ECOModeChange]->m_shDataAddr;
	
	GetLocalDevBWAddr(eP2C_ECOModeChange, m_stpModCfg->nModuleID, 0, W_L2_ECO_MODE_EOV_EVENT);	
	shEOVDataAddr = m_ShCommData_Local[eP2C_ECOModeChange]->m_shDataAddr;
	
	// Event Data Read
	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, shEOMDDataAddr, &nDataSize, &shECO_EOMD);
	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, shEOVDataAddr,	&nDataSize, &shECO_EOV);

	for( long i = 0; i < m_stpSma->stLayOutCfg.stEOIDTable.nEOIDCount; i++ )
	{
		if ( m_stpSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOID == eEOID_ECOMode )
			nEoid_EcoModeIdx = i;
	}

	for (nIndex = 0; nIndex < m_stpSma->stLayOutCfg.stEOIDTable.stEOIDData[nEoid_EcoModeIdx].nEOMDCount; nIndex++)
	{
		if (atoi(m_stpSma->stLayOutCfg.stEOIDTable.stEOIDData[nEoid_EcoModeIdx].stEOMDData[nIndex].szEOMD) == shECO_EOMD)
		{
			m_stpSma->stLayOutCfg.stEOIDTable.stEOIDData[nEoid_EcoModeIdx].stEOMDData[nIndex].nEOV = shECO_EOV;
			break;
		}
	}
*/
}


// Alarm Start Address
void CBaseModule::OnAlarmOffset(long nOffset)
{
	m_nOffset = nOffset;
}

// Alarm Refresh [10/3/2003]
void CBaseModule::AlarmHappenDataInit()
{
	memset(m_pbAlarmHappen, 0x00, sizeof(bool) * m_nMaxCount);
	m_nAlarmCount = 0;
}

void CBaseModule::SendAlarmEvent(long nAlarmID, long nSig, long nModuleID)
{
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	sig.nFrom	= PLC_TASK_ID;
	sig.nTo		= GUI_TASK_ID;
	sig.nSignal = nSig;
	
	if(nAlarmID>1) nAlarmID=nAlarmID-1;
	
	switch(nSig)
	{
	case sigAlarmOccured:
		sig.unionData.stAlarmOccuredData.nAlarmID = nAlarmID;
		sig.unionData.stAlarmOccuredData.nModuleID = nModuleID;
		m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Alarm ID(%04d) Occured Event OK", nModuleID,m_stpModCfg->szModuleName, nAlarmID);
		break;
	case sigAlarmTreated:
		sig.unionData.stAlarmTreatedData.nAlarmID = nAlarmID;
		sig.unionData.stAlarmTreatedData.nModuleID = nModuleID;
		m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Alarm ID(%04d) Treated Event OK", nModuleID,m_stpModCfg->szModuleName, nAlarmID);
		break;
	}
	m_pParent->SendSignalToSCH(&sig);
}

void CBaseModule::OnEventAnalysis(long *pVal, long nModuleID)
{
	if(pVal == NULL || m_nMaxCount == 0)	return ;
	
	short sArray = m_nMaxCount / 16;
	
	for(long Bit = 0; Bit < 16; Bit++)
	{
		for (long ArrayCnt = 0; ArrayCnt < sArray; ArrayCnt++)
		{
			IsAlarmEvent(ArrayCnt * 16 + Bit,
				(*(pVal+ArrayCnt) & (0x0001 << Bit) ) == 0 ? false : true, nModuleID);
			
			if(m_nMaxCount <= (ArrayCnt* 16 + Bit))	return;
		}
	}
}

long CBaseModule::IsAlarmEvent( long nIndex, bool bVal, long nModuleID )
{
	if((bVal))
	{
		if(!*(m_pbAlarmHappen+nIndex))			// Alarm Happen Event
		{
			*(m_pbAlarmHappen+nIndex) = true;
			SendAlarmEvent(nIndex + m_nOffset, sigAlarmOccured, nModuleID);
			m_nAlarmCount+=1;
			return 1;
		}
		else
			return 0;							// ~ing
	}
	else
	{
		if(!*(m_pbAlarmHappen+nIndex))
		{
			return	0;							// ~ing
		}
		else
		{
			*(m_pbAlarmHappen+nIndex) = false;	// Alarm Clear Event
			SendAlarmEvent(nIndex + m_nOffset, sigAlarmTreated, nModuleID);
			m_nAlarmCount-=1;
			return -1;	
		}
	}
}

void CBaseModule::OnAlarmEventHandle()
{
	long	i = 0;
	long	j = 0;
	long	nCurAlmSts = 0;
	long	nEvtAlmSts = 0;
	long	nEvtAlmFlag= 0;
	
	bool	bCurAlmSet = false;
	bool	bEvtAlmSet = false;
	
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	stAlarmOccuredDataType		*pOccured = NULL;
	stAlarmTreatedDataType		*pTreated = NULL;
	
	sig.nFrom	= PLC_TASK_ID;
	sig.nTo		= GUI_TASK_ID;
	
	OnEventAnalysis(m_stpModRunInfo->nCurAlarmState, m_stpModRunInfo->nModuleID);
}

// Update [PLC Link] -> [SMA]
void CBaseModule::ConvertTransferDataToPanelData(stPanelInfoType *pPanelInfo, short *shData)
{
	long i		= 0;
	long nIdx	= 0;
	
	memcpy(pPanelInfo->szHPanelID	, (shData+nIdx), MAX_PANEL_ID_LEN		);		nIdx += 6;
	memcpy(pPanelInfo->szEPanelID	, (shData+nIdx), MAX_PANEL_ID_LEN		);		nIdx += 6;
	memcpy(pPanelInfo->szProcessID	, (shData+nIdx), MAX_PROCESS_ID_LEN		);		nIdx +=10;
	memcpy(pPanelInfo->szProductID	, (shData+nIdx), MAX_PRODUCT_ID_LEN		);		nIdx +=10;
	
	memcpy(pPanelInfo->szStepID		, (shData+nIdx), MAX_STEP_ID_LEN		);		nIdx += 6;
	memcpy(pPanelInfo->szBatchID	, (shData+nIdx), MAX_BATCH_ID_LEN		);		nIdx += 6;
	memcpy(pPanelInfo->szProdType	, (shData+nIdx), MAX_PRODUCT_TYPE_LEN	);		nIdx++;
	memcpy(pPanelInfo->szProdKind	, (shData+nIdx), MAX_PRODUCT_KIND_LEN	);		nIdx++;
	memcpy(pPanelInfo->szPPID		, (shData+nIdx), MAX_PPID_LEN			);		nIdx += 8;
	memcpy(pPanelInfo->szFlowID		, (shData+nIdx), MAX_FLOW_ID_LEN		);		nIdx += 2;
	memcpy(pPanelInfo->shFlowGroup, (shData+nIdx), MAX_FLOW_GROUP_LEN);				nIdx +=10;	//	Flow Group
	memcpy(pPanelInfo->shUsableChamber, (shData+nIdx), MAX_USABLE_CHAMBER_LEN);		nIdx +=5;	//	Usable Chamber

	pPanelInfo->nPanelSize[0]		= *(shData+nIdx);								nIdx++;
	pPanelInfo->nPanelSize[1]		= *(shData+nIdx);								nIdx++;
	pPanelInfo->nThickness			= *(shData+nIdx);								nIdx++;
	pPanelInfo->nCompCount			= *(shData+nIdx);								nIdx++;
	
	// Cell Grade : PIO Spec Bit 처리이고 HSMS는 Char 처리가 필요함. $GRADE A[160]
	for( i = 0; i < MAX_CELL_GRADE_LEN; i++ )
	{
		( ( *(shData+nIdx) >> i ) & 0x0001 ) ? pPanelInfo->szGrade[i] = 0x31 : pPanelInfo->szGrade[i] = 0x30;
		switch(i)
		{
		case 15:	nIdx++;	break;
		case 31:	nIdx++;	break;
		case 47:	nIdx++;	break;
		case 63:	nIdx++;	break;
		case 79:	nIdx++; break;
		case 95:	nIdx++; break;
		case 111:	nIdx++; break;
		case 127:	nIdx++; break;
		case 143:	nIdx++; break;
		case 159:	nIdx++; break;
		}
	}
	
	memcpy(pPanelInfo->szJudgement	, (shData+nIdx), MAX_JUDGEMENT_RESULT_LEN);		nIdx += 2;
	memcpy(pPanelInfo->szCode		, (shData+nIdx), MAX_JUDGEMENT_CODE_LEN);		nIdx += 2;
	memcpy(pPanelInfo->szCount1		, (shData+nIdx), MAX_GLASS_COUNT_LEN	);		nIdx++;	
	memcpy(pPanelInfo->szCount2		, (shData+nIdx), MAX_GLASS_COUNT_LEN	);		nIdx++;	
	memcpy(pPanelInfo->szPanelPosition, (shData+nIdx), MAX_GLASS_POSITION_LEN);		nIdx++;	
	
	for(i=0; i<MAX_FLOW_HISTORY_LEN/2; i++)
	{
		pPanelInfo->nFlowHistory[i*2]   =  *(shData+nIdx) & 0x00ff; 
		pPanelInfo->nFlowHistory[i*2+1] = (*(shData+nIdx) >> 8 ) & 0x00ff;
		nIdx++; 
	}
	
	pPanelInfo->nUniqueID[0]		= *(shData+nIdx) & 0x00ff; 
	pPanelInfo->nUniqueID[1]		= ( *(shData+nIdx) >> 8 ) & 0x00ff;				nIdx++; 
	pPanelInfo->nUniqueID[2]		= *(shData+nIdx) & 0x00ff;
	pPanelInfo->nUniqueID[3]		= ( *(shData+nIdx) >> 8 ) & 0x00ff;				nIdx++;
	
	memcpy(pPanelInfo->szReadingFlag, (shData+nIdx), MAX_READING_FLAG_LEN	);		nIdx++;
	memcpy(pPanelInfo->szMultiUse	, (shData+nIdx), MAX_MULTI_USE_LEN		);		nIdx +=10;
	
	pPanelInfo->stBitSignal.nJobStartBit		= *(shData+nIdx) & 0x0001;
	pPanelInfo->stBitSignal.nJobEndBit			= *(shData+nIdx) & 0x0002;
	
	nIdx ++;
	
	memcpy(pPanelInfo->stPairPanelInfo.szPairHPanelID, (shData+nIdx), MAX_PANEL_ID_LEN);	nIdx += 6;	//	Pair H GLass ID
	memcpy(pPanelInfo->stPairPanelInfo.szPairEPanelID, (shData+nIdx), MAX_PANEL_ID_LEN);	nIdx += 6;	//	Pair E GLass ID
	memcpy(pPanelInfo->stPairPanelInfo.szPairProductID, (shData+nIdx), MAX_PRODUCT_ID_LEN);	nIdx +=10;	//	Pair Product ID
	
	// Cell Grade : PIO Spec Bit 처리이고 HSMS는 Char 처리가 필요함. $GRADE A[160]
	for( i = 0; i < MAX_CELL_GRADE_LEN; i++ )
	{
		( ( *(shData+nIdx) >> i ) & 0x0001 ) ? pPanelInfo->stPairPanelInfo.szPairGrade[i] = 0x31 : pPanelInfo->stPairPanelInfo.szPairGrade[i] = 0x30;
		switch(i)
		{
		case 15:	nIdx++;	break;
		case 31:	nIdx++;	break;
		case 47:	nIdx++;	break;
		case 63:	nIdx++;	break;
		case 79:	nIdx++; break;
		case 95:	nIdx++; break;
		case 111:	nIdx++; break;
		case 127:	nIdx++; break;
		case 143:	nIdx++; break;
		case 159:	nIdx++; break;
		}
	}
	memcpy(pPanelInfo->shReferData, (shData+nIdx), 4);								nIdx += 2;	//	Refer Data	
	
	nIdx += 15;
	
	pPanelInfo->nPanelState			= eGlass_Processing;	//*(shData+nIdx);							nIdx++;		//@ Panel State 사용 여부 확인
	pPanelInfo->nOwnGlassNo			=	*(shData+nIdx);								nIdx++;
	
	memcpy(m_szMSCHPanelID,pPanelInfo->szHPanelID,sizeof(m_szMSCHPanelID));
}

void CBaseModule::ConvertPanelDataToTransferData(stPanelInfoType *pPanelInfo, short *shData)
{
	long i		= 0;
	long j		= 0;
	long nIdx	= 0;
	
	if(pPanelInfo == NULL) return;
	
	memcpy((shData+nIdx),	pPanelInfo->szHPanelID, MAX_PANEL_ID_LEN		);		nIdx += 6;
	memcpy((shData+nIdx),	pPanelInfo->szEPanelID, MAX_PANEL_ID_LEN		);		nIdx += 6;
	memcpy((shData+nIdx),	pPanelInfo->szProcessID, MAX_PROCESS_ID_LEN		);		nIdx +=10;
	memcpy((shData+nIdx),	pPanelInfo->szProductID, MAX_PRODUCT_ID_LEN		);		nIdx +=10;
	memcpy((shData+nIdx),	pPanelInfo->szStepID, MAX_STEP_ID_LEN		);			nIdx += 6;
	memcpy((shData+nIdx),	pPanelInfo->szBatchID, MAX_BATCH_ID_LEN		);			nIdx += 6;
	memcpy((shData+nIdx),	pPanelInfo->szProdType, MAX_PRODUCT_TYPE_LEN	);		nIdx++;
	memcpy((shData+nIdx),	pPanelInfo->szProdKind, MAX_PRODUCT_KIND_LEN	);		nIdx++;
	memcpy((shData+nIdx),	pPanelInfo->szPPID, MAX_PPID_LEN			);			nIdx += 8;
	memcpy((shData+nIdx),	pPanelInfo->szFlowID, MAX_FLOW_ID_LEN		);			nIdx += 2;
	memcpy((shData+nIdx),	pPanelInfo->shFlowGroup, MAX_FLOW_GROUP_LEN		);		nIdx += 10;
	memcpy((shData+nIdx),	pPanelInfo->shUsableChamber, MAX_USABLE_CHAMBER_LEN	);	nIdx += 5;

	*(shData+nIdx)	=	(short) pPanelInfo->nPanelSize[0];							nIdx++;
	*(shData+nIdx)	=	(short) pPanelInfo->nPanelSize[1];							nIdx++;
	*(shData+nIdx)	=	(short) pPanelInfo->nThickness;								nIdx++;
	*(shData+nIdx)	=	(short) pPanelInfo->nCompCount;								nIdx++;

	j = 0;
	for( i = 0; i < MAX_CELL_GRADE_LEN; i++ )
	{
		if ( pPanelInfo->szGrade[i] == 0x31 )
			shData[nIdx] = ( shData[nIdx] >> ( i - 16*j ) ) | 0x0001;
		
		switch(i)
		{
		case 15:	j++; nIdx++;	break;
		case 31:	j++; nIdx++;	break;
		case 47:	j++; nIdx++;	break;
		case 63:	j++; nIdx++;	break;
		case 79:	j++; nIdx++;	break;
		case 95:	j++; nIdx++;	break;
		case 111:	j++; nIdx++;	break;
		case 127:	j++; nIdx++;	break;
		case 143:	j++; nIdx++;	break;
		case 159:	j++; nIdx++;	break;
		}
	}

	memcpy((shData+nIdx),	pPanelInfo->szJudgement, MAX_JUDGEMENT_RESULT_LEN);		nIdx += 2;
	memcpy((shData+nIdx),	pPanelInfo->szCode, MAX_JUDGEMENT_CODE_LEN);			nIdx += 2;
	memcpy((shData+nIdx),	pPanelInfo->szCount1,	MAX_GLASS_COUNT_LEN	);			nIdx++;	
	memcpy((shData+nIdx),	pPanelInfo->szCount2,	MAX_GLASS_COUNT_LEN	);			nIdx++;	
	memcpy((shData+nIdx),	pPanelInfo->szPanelPosition, MAX_GLASS_POSITION_LEN);	nIdx++;

	for( i = 0 ; i < MAX_FLOW_HISTORY_LEN/4; i++)
	{
		*(shData+nIdx) = (short)(pPanelInfo->nFlowHistory[i*4] | pPanelInfo->nFlowHistory[i*4+1] << 8 );
		nIdx += 1;
		
		*(shData+nIdx) = (short)(pPanelInfo->nFlowHistory[i*4+2] | pPanelInfo->nFlowHistory[i*4+3] << 8 );
		nIdx += 1;
	}

	shData[nIdx] = (short)pPanelInfo->nUniqueID[1];
	shData[nIdx] = ( ( shData[nIdx] << 8 ) | (short)pPanelInfo->nUniqueID[0] );	nIdx++;
	shData[nIdx] = (short)pPanelInfo->nUniqueID[3];
	shData[nIdx] = ( ( shData[nIdx] << 8 ) | (short)pPanelInfo->nUniqueID[2] );	nIdx++;

	memcpy((shData+nIdx),	pPanelInfo->szReadingFlag, MAX_READING_FLAG_LEN	);		nIdx++;
	memcpy((shData+nIdx),	pPanelInfo->szMultiUse, MAX_MULTI_USE_LEN		);		nIdx +=10;

	//@ Bit Signal (Job Start / Job End)
	if(pPanelInfo->stBitSignal.nJobStartBit ==  TRUE)
	{
		*(shData+nIdx) |= 0x0001;
	}
	else
	{
		*(shData+nIdx) &= 0xFFFE;
	}
	if(pPanelInfo->stBitSignal.nJobEndBit ==  TRUE)
	{
		*(shData+nIdx) |= 0x0002;
	}
	nIdx ++;

	memcpy((shData+nIdx), pPanelInfo->stPairPanelInfo.szPairHPanelID, MAX_PANEL_ID_LEN);	nIdx += 6;	//	Pair H GLass ID
	memcpy((shData+nIdx), pPanelInfo->stPairPanelInfo.szPairEPanelID, MAX_PANEL_ID_LEN);	nIdx += 6;	//	Pair E GLass ID
	memcpy((shData+nIdx), pPanelInfo->stPairPanelInfo.szPairProductID, MAX_PRODUCT_ID_LEN);	nIdx +=10;	//	Pair Product ID

	j = 0;
	for( i = 0; i < MAX_CELL_GRADE_LEN; i++ )
	{
		if ( pPanelInfo->szGrade[i] == 0x31 )
			shData[nIdx] = ( shData[nIdx] >> ( i - 16*j ) ) | 0x0001;
		
		switch(i)
		{
		case 15:	j++; nIdx++;	break;
		case 31:	j++; nIdx++;	break;
		case 47:	j++; nIdx++;	break;
		case 63:	j++; nIdx++;	break;
		case 79:	j++; nIdx++;	break;
		case 95:	j++; nIdx++;	break;
		case 111:	j++; nIdx++;	break;
		case 127:	j++; nIdx++;	break;
		case 143:	j++; nIdx++;	break;
		case 159:	j++; nIdx++;	break;
		}
	}
	
	memcpy((shData+nIdx), pPanelInfo->shReferData, 4);								nIdx += 2;	//	Refer Data						

	nIdx += 15;

	//*(shData+nIdx)	=	(short) pPanelInfo->nPanelState;							nIdx++;
	*(shData+nIdx)	=	(short) pPanelInfo->nOwnGlassNo;							nIdx++;

}

void CBaseModule::SetInterlockRelatedID(long nModuleNo, long nHSNo, long nInterlockNo, long nUpLow)
{
	stOtherEQPIDType		*pstOtherEQPID = NULL;

	pstOtherEQPID = &m_stpSma->stLayOutCfg.stModCfg[nModuleNo].stOtherEQPID[nHSNo];

	switch(nUpLow)
	{
	case eInterlock_Upper:		// Upper
		if(strlen(pstOtherEQPID->szUpperEQPID) > 0)
			CopyMemory(m_stpSma->stSysRunInfo.stModRunInfo[nModuleNo].stEQInterlockTable.stEQInterlock[nHSNo][nInterlockNo].szRelatedModuleID,
				pstOtherEQPID->szUpperEQPID, MAX_EQ_MODULE_ID_LEN);
		break;
	case eInterlock_SEMES:		// SEMES
		if(strlen(pstOtherEQPID->szUpperEQPID) > 0)
			CopyMemory(m_stpSma->stSysRunInfo.stModRunInfo[nModuleNo].stEQInterlockTable.stEQInterlock[nHSNo][nInterlockNo].szRelatedModuleID,
				m_stpSma->stLayOutCfg.szEQPID, MAX_EQ_MODULE_ID_LEN);
		break;
	case eInterlock_Lower:		// Lower
		if(strlen(pstOtherEQPID->szLowerEQPID) > 0)
			CopyMemory(m_stpSma->stSysRunInfo.stModRunInfo[nModuleNo].stEQInterlockTable.stEQInterlock[nHSNo][nInterlockNo].szRelatedModuleID,
				pstOtherEQPID->szLowerEQPID, MAX_EQ_MODULE_ID_LEN);
		break;
	}
}


void CBaseModule::SetCleanerProcessData(long nPPID)		
{
 	long	nRcpNo = 0;
	long	nCnt = 0;
 	short   shSetData[MAX_RECIPE_ITEM]	=	{ 0x00, };
	
 	long	nSetDataSize	=	sizeof(short) * MAX_RECIPE_ITEM;
 	long	nStAddr	= W_MD1_RECIPE_DATA;
	long	nMapIdx = 0;
 	
 	stMainRecipeTableType	*pMainRcp = NULL;
 	stCleanerRecipeDataType	*pRcpCln = NULL;

	stRecipeParamTableType  *pRcpParamTable = NULL;
 	
 	pMainRcp = &m_stpSma->stSysData.stMainRecipeTbl;
	pRcpParamTable = &m_stpSma->stSysData.stRecipeParamTbl[eModuleType_Cleaner];
 	
	switch(m_stpSma->stLayOutCfg.nEQType)
	{
	case eEQType_PFC:
		if ( pMainRcp == NULL || pMainRcp->stRcp[nPPID-1].stPFCRcp.stRecipeHead.bUsed == FALSE ) return;
		
		//	Process Data No
		nRcpNo	= pMainRcp->stRcp[nPPID-1].stPFCRcp.stMainRecipe.nCLNRecipeNo;
 		if(nRcpNo < 1 || nRcpNo > 50) return;

		pRcpCln	= &m_stpSma->stSysData.stRecipeTblCLN.stRecipeData[nRcpNo-1]; 	
 		if ( pRcpCln == NULL || pRcpCln->stRecipeHead.bUsed == FALSE ) return;

		// Recipe No Write
		shSetData[eCLN_RECIPE_NO] = static_cast<short>(nRcpNo);

		for (nCnt = 0; nCnt < pRcpParamTable->nParamCount; nCnt++)
		{
			if (pRcpParamTable->stRecipeParam[nCnt].bUsed == TRUE)
			{
				nMapIdx = pRcpParamTable->stRecipeParam[nCnt].nMapIndex-1;
				shSetData[nMapIdx] = static_cast<short>(pRcpCln->stPFCProcData.nRecipeValue[nCnt]);

			}
		}

		break;
	case eEQType_LCPI:
		if ( pMainRcp == NULL || pMainRcp->stRcp[nPPID-1].stLCRcp.stRecipeHead.bUsed == FALSE ) return;
		
		//	Process Data No
		nRcpNo	= pMainRcp->stRcp[nPPID-1].stLCRcp.stMainRecipe.nCLNRecipeNo;
		if(nRcpNo < 1 || nRcpNo > 50) return;
		
		pRcpCln	= &m_stpSma->stSysData.stRecipeTblCLN.stRecipeData[nRcpNo-1]; 	
		if ( pRcpCln == NULL || pRcpCln->stRecipeHead.bUsed == FALSE ) return;
		
		// Recipe No Write
		shSetData[eCLN_RECIPE_NO] = static_cast<short>(nRcpNo);
		
		for (nCnt = 0; nCnt < pRcpParamTable->nParamCount; nCnt++)
		{
			if (pRcpParamTable->stRecipeParam[nCnt].bUsed == TRUE)
			{
				nMapIdx = pRcpParamTable->stRecipeParam[nCnt].nMapIndex-1;
				shSetData[nMapIdx] = static_cast<short>(pRcpCln->stLCProcData.nRecipeValue[nCnt]);
				
			}
		}
		break;
	}
	
	//	Data Write
 	m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, nStAddr, &nSetDataSize, shSetData);
}

void CBaseModule::SetEtchProcessData(long nPPID)
{
	long	nRcpNo = 0;
	long	nCnt = 0;
	short   shSetData[MAX_RECIPE_ITEM]	=	{ 0x00, };
	
	long	nSetDataSize	=	sizeof(short) * MAX_RECIPE_ITEM;
	long	nStAddr	= W_MD1_RECIPE_DATA;
	long	nMapIdx = 0;
	
	stMainRecipeEtchStripDataType	*pMainRcp = NULL;
	stModuleRecipeEtchType	*pRcpEtch = NULL;
	
	stRecipeParamTableType  *pRcpParamTable = NULL;
	
	pRcpParamTable = &m_stpSma->stSysData.stRecipeParamTbl[eModuleType_Etcher];
	
	pMainRcp = &m_stpSma->stSysData.stMainRecipeTbl.stRcp[nPPID-1].stWetRcp;
	if ( pMainRcp == NULL || pMainRcp->stRecipeHead.bUsed == FALSE ) return;
	
	// Process Data No	
	nRcpNo	= pMainRcp->stMainRecipe.nEtchRecipeNo;
	if(nRcpNo < 1 || nRcpNo > 50) return;
	
	pRcpEtch = &m_stpSma->stSysData.stRecipeTblEtch.stRecipeData[nRcpNo-1].stProcData; 	
	if ( pRcpEtch == NULL || m_stpSma->stSysData.stRecipeTblEtch.stRecipeData[nRcpNo-1].stRecipeHead.bUsed  == FALSE ) return;
	
	for (nCnt = 0; nCnt < pRcpParamTable->nParamCount; nCnt++)
	{
		if (pRcpParamTable->stRecipeParam[nCnt].bUsed == TRUE)
		{
			nMapIdx = pRcpParamTable->stRecipeParam[nCnt].nMapIndex-1;
			shSetData[nMapIdx] = static_cast<short>(pRcpEtch->nRecipeValue[nCnt]);
		}
	}
	
	// Recipe No Write
	shSetData[eCLN_RECIPE_NO] = static_cast<short>(nRcpNo);
	shSetData[eCLN_LINE_TACT] = static_cast<short> (pMainRcp->stMainRecipe.nLineTact);
	
	//	Data Write
 	m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, nStAddr, &nSetDataSize, shSetData);
}

void CBaseModule::SetStripProcessData(long nPPID)
{

	long	nRcpNo = 0;
	long	nCnt = 0;
	short   shSetData[MAX_RECIPE_ITEM]	=	{ 0x00, };
	
	long	nSetDataSize	=	sizeof(short) * MAX_RECIPE_ITEM;
	long	nStAddr	= W_MD2_RECIPE_DATA;
	long	nMapIdx = 0;
	
	stMainRecipeEtchStripDataType	*pMainRcp = NULL;
	stModuleRecipeStripType	*pRcpStrip = NULL;
	
	stRecipeParamTableType  *pRcpParamTable = NULL;
	
	pRcpParamTable = &m_stpSma->stSysData.stRecipeParamTbl[eModuleType_Stripper];

	pMainRcp = &m_stpSma->stSysData.stMainRecipeTbl.stRcp[nPPID-1].stWetRcp;
	if ( pMainRcp == NULL || pMainRcp->stRecipeHead.bUsed == FALSE ) return;

	// Process Data No	
	nRcpNo	= pMainRcp->stMainRecipe.nStripRecipeNo;
	if(nRcpNo < 1 || nRcpNo > 50) return;
	
	pRcpStrip = &m_stpSma->stSysData.stRecipeTblStrip.stRecipeData[nRcpNo-1].stProcData; 	
	if ( pRcpStrip == NULL || m_stpSma->stSysData.stRecipeTblStrip.stRecipeData[nRcpNo-1].stRecipeHead.bUsed  == FALSE ) return;

	for (nCnt = 0; nCnt < pRcpParamTable->nParamCount; nCnt++)
	{
		if (pRcpParamTable->stRecipeParam[nCnt].bUsed == TRUE)
		{
			nMapIdx = pRcpParamTable->stRecipeParam[nCnt].nMapIndex-1;
			shSetData[nMapIdx] = static_cast<short>(pRcpStrip->nRecipeValue[nCnt]);
		}
	}

	// Recipe No Write
	shSetData[eCLN_RECIPE_NO] = static_cast<short>(nRcpNo);
	shSetData[eCLN_LINE_TACT] = static_cast<short> (pMainRcp->stMainRecipe.nLineTact);


	//	Data Write
 	m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, nStAddr, &nSetDataSize, shSetData);
}

BOOL CBaseModule::ThreadAllRun()
{
	m_bBaseThread	=	TRUE;

	m_ThreadUpdateMelSecArea.StartThread(&ThreadUpdateMelSecArea, this);
	if (!m_ThreadUpdateMelSecArea.IsRunning())
		return FALSE;
	
	m_ThreadSequenceProcess.StartThread(&ThreadSequenceProcess, this);
	if (!m_ThreadSequenceProcess.IsRunning())
		return FALSE;
	
	m_ThreadSequenceProcessEnd.StartThread(&ThreadSequenceProcessEnd, this);
	if (!m_ThreadSequenceProcessEnd.IsRunning())
		return FALSE;
	
	m_ThreadQueueProcess.StartThread(&ThreadQueueProcess, this);
	if (!m_ThreadQueueProcess.IsRunning())
		return FALSE;

	m_ThreadUpdateTimer.StartThread(&ThreadUpdateTimer, this);
	if (!m_ThreadUpdateTimer.IsRunning())
		return FALSE;

	return TRUE;
}

BOOL CBaseModule::OnTerminate()
{
	m_bBaseThread	=	FALSE;
	
	m_ThreadUpdateMelSecArea.EndThread();

	m_ThreadSequenceProcess.EndThread();
	m_ThreadSequenceProcessEnd.EndThread();
	m_ThreadQueueProcess.EndThread();
	m_ThreadUpdateTimer.EndThread();
	
	Sleep(100);
	
	if( m_ThreadUpdateMelSecArea.IsRunning() )
		m_ThreadUpdateMelSecArea.KillThread();

	if ( m_ThreadSequenceProcess.IsRunning() )
		m_ThreadSequenceProcess.KillThread();
	
	if ( m_ThreadSequenceProcessEnd.IsRunning() )
		m_ThreadSequenceProcessEnd.KillThread();
	
	if ( m_ThreadQueueProcess.IsRunning() )
		m_ThreadQueueProcess.KillThread();

	if ( m_ThreadUpdateTimer.IsRunning() )
		m_ThreadUpdateTimer.KillThread();

	return TRUE;
}

void CBaseModule::C2P_MachineCmdSequence(stMachineCmdType stCmd)
{
	long	nStep			=	0;
	long	nStepState		=	STEP_START;
	short   shSetData		=	0;
	long	nSetDataSize	=	sizeof(short);
	long	nCmdBitAddr		=	0;

	short	shEQCmd[2] = {0x00,};
	long	nEQCmdSize = sizeof(short) * 2;
	
	GetMasterDevBWAddr(eC2P_MachineCommand, stCmd.nModuleNo, B_L2_FROM_CIM_MACHINE_CMD_REQ, 0);	
	
	nCmdBitAddr = m_ShCommData_Master[eC2P_MachineCommand]->m_shBitAddr;
	
	m_ShCommData_Master[eC2P_MachineCommand]->m_nCmdData = stCmd.nCmdData;
	
	// Event 진행 전 Request Bit Off. (Request Bit Off 안된 경우 대비)
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eC2P_MachineCommand]->m_shBitAddr);
	
	while(nStep != -1) 
	{
		switch(nStep++)
		{
		case 0:	//	동일 Sequence가 진행중이면 Request Bit Off
			if ( m_stpModRunInfo->stEQCmdRlyEvtIO.bMachineCmdReply == TRUE )
			{
				m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nCmdBitAddr);
				nStep = -1;
				nStepState = STEP_NG;
			}
			break;
		case 1:	//	Data 유효성 Check
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Machine Command(%d) Started!", m_stpModCfg->nModuleID,m_stpModCfg->szModuleName, stCmd.nCmdData);
			switch(stCmd.nCmdData)
			{
			case eMachine_PM:
			case eMachine_Normal:
			case eMachine_Pause:
			case eMachine_Resume:
			case eMachine_CycleStop:
				break;
			default:
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)]Unknown Machine Command(%d)", m_stpModCfg->nModuleID,m_stpModCfg->szModuleName, stCmd.nCmdData);
				nStep = -1;
				nStepState = STEP_NG;
				break;
			}
			break;
			
			case 2:	//	해당 Machine Command에 대해 Melnet에 설정할 값을 선택하고, Melnet에 Write한다.
				switch(stCmd.nCmdData)
				{
				case eMachine_PM:		
					shSetData = 0x4D50;	// PM
					m_ShCommData_Master[eC2P_MachineCommand]->m_nCmdData = 53;

					memcpy(shEQCmd, stCmd.szReasonCode, sizeof(shEQCmd));
					m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, W_L1_EQ_CMD_PM_CODE, &nEQCmdSize, shEQCmd);
					break;
				case eMachine_Normal:	
					shSetData = 0x4F4E;	// NO
					m_ShCommData_Master[eC2P_MachineCommand]->m_nCmdData = 54;
					break;
				case eMachine_Pause:
					shSetData = 0x2050; // p
					m_ShCommData_Master[eC2P_MachineCommand]->m_nCmdData = 51;

					memcpy(shEQCmd, stCmd.szReasonCode, sizeof(shEQCmd));
					m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, W_L1_EQ_CMD_PAUSE_CODE, &nEQCmdSize, shEQCmd);
					break;
				case eMachine_Resume:
					shSetData = 0x2052; // R
					m_ShCommData_Master[eC2P_MachineCommand]->m_nCmdData = 52;
					break;
				case eMachine_CycleStop:  // CS
					shSetData = 0x5343;
					m_ShCommData_Master[eC2P_MachineCommand]->m_nCmdData = 57;

					memcpy(shEQCmd, stCmd.szReasonCode, sizeof(shEQCmd));
					m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, W_L1_EQ_CMD_PAUSE_CODE, &nEQCmdSize, shEQCmd);
					break;		
					
				}
				memcpy(m_ShCommData_Master[eC2P_MachineCommand]->m_szRCode, stCmd.szReasonCode, MAX_REASON_CODE_LEN);
				
				m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, W_L1_MACHINE_CMD_DATA, &nSetDataSize, &shSetData);
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Machine Command(%d) Data Write To PLC(0x%4x)", m_stpModCfg->nModuleID,m_stpModCfg->szModuleName,stCmd.nCmdData, shSetData);
				break;
				
				case 3:	//	Request Bit를 On하여 Maching Command를 수행하도록 한다.
					m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, nCmdBitAddr);	
					m_ShCommData_Master[eC2P_MachineCommand]->m_bWaitTime = TRUE;
					m_ShCommData_Master[eC2P_MachineCommand]->m_TimeCheck.StartTimer();
					nStep = -1;
					if(nStepState != STEP_NG)
						nStepState = STEP_OK;
					break;
		}
	}
	
	if(nStepState == STEP_NG) 
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Machine Command",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
}

void CBaseModule::C2P_MachineCmdSequenceEnd()
{
	ULONG	nElaspedTime	= 0;
	long	nCmdBitAddr		= 0;
	long	nCmdData		= 0;
	short	shRpyAddr		= 0;
	short	shRpyData		= 0;
	long	nRpySize		= sizeof(short);
	
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	char szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	memset(szModuleID, 0x00, MAX_EQ_MODULE_ID_LEN);
	long len = 0;
	
	GetLocalDevBWAddr(eC2P_MachineCommand, m_stpModRunInfo->nModuleID, 0, W_L2_MACHINE_CMD_RLY);	
	shRpyAddr = m_ShCommData_Local[eC2P_MachineCommand]->m_shDataAddr;
	
	nCmdBitAddr = m_ShCommData_Master[eC2P_MachineCommand]->m_shBitAddr;
	nCmdData	= m_ShCommData_Master[eC2P_MachineCommand]->m_nCmdData;
	
	//	Machine Command에 대한 응답 Check를 하고, Timeout이 발행하면 Alarm을 보고한다.
	if(m_stpModRunInfo->stEQCmdRlyEvtIO.bMachineCmdReply == FALSE)
	{
		if(m_ShCommData_Master[eC2P_MachineCommand]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			nElaspedTime = m_ShCommData_Master[eC2P_MachineCommand]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eC2P_MachineCommand]->m_nElaspedTime = nElaspedTime;
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)]Machine Command(%d) Timeout[Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nCmdData, nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG]Machine Command(%d) Timeout[Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nCmdData, nElaspedTime);
			
			m_ShCommData_Master[eC2P_MachineCommand]->m_bWaitTime = FALSE;
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nCmdBitAddr);
			
			sig.unionData.s2f42ToHost.nTMACK = eTMAck_HWConditionError;
			//memcpy(sig.unionData.s2f42EQCmdToHost.stEQCmd[0].szRCode, m_ShCommData_Master[eC2P_MachineCommand]->m_szRCode, MAX_REASON_CODE_LEN);
			
			// Host Report Send
			sig.nSignal = sigMachineCommand;
			sig.nFrom	= PLC_TASK_ID;
			sig.nTo		= SCH_TASK_ID;
			sig.nParam  = m_stpModRunInfo->nModuleID;

			sig.unionData.s2f42ToHost.nRCMD = m_ShCommData_Master[eC2P_MachineCommand]->m_nCmdData;

			sig.unionData.s2f42ToHost.MsgHead.nSessionID = 1;
				
			m_pParent->SendSignalToSCH(&sig);
		}
		return;
	}
	
	//	다음 명령을 수행할 수 있도록 응답 받은 Code를 상위에 보고한다.
	nElaspedTime = m_ShCommData_Master[eC2P_MachineCommand]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[eC2P_MachineCommand]->m_nElaspedTime = nElaspedTime;
	
	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, shRpyAddr, &nRpySize, &shRpyData);
	
	//	if ( m_stpModRunInfo->stMasterReplyData.nReplyData == 0x41 ) // Ack
	if ( shRpyData == 0x41 ) // Ack
	{
		m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)]Machine Command(%d) AcK[0x:%4x] Reply Complete[Elasped Time : %dms]!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nCmdData, shRpyData, nElaspedTime);
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK]Machine Command(%d) AcK[0x:%4x] Reply Complete[Elasped Time : %dms]!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nCmdData,shRpyData, nElaspedTime);
		
		sig.unionData.s2f42ToHost.nTMACK = eTMAck_Acknowledge;
	}
	else	//	Nak
	{
		m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)]Machine Command(%d) NAK[0x:%4x] Reply Complete[Elasped Time : %dms]!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nCmdData, shRpyData,nElaspedTime);
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK]Machine Command(%d) NAK[0x:%4x] Reply Complete[Elasped Time : %dms]!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nCmdData, shRpyData,nElaspedTime);

		sig.unionData.s2f42ToHost.nTMACK = eTMAck_HWConditionError;
		memcpy(sig.unionData.s2f42ToHost.szReason, m_ShCommData_Master[eC2P_MachineCommand]->m_szRCode, MAX_REASON_CODE_LEN);
	}
	
	// Host Report Send
	sig.nSignal = sigMachineCommand;
	sig.unionData.s2f42ToHost.nRCMD = m_ShCommData_Master[eC2P_MachineCommand]->m_nCmdData;
	sig.nParam = m_ShCommData_Master[eC2P_MachineCommand]->m_nCmdData;
	sig.nFrom	= PLC_TASK_ID;
	sig.nTo		= SCH_TASK_ID;	
	sig.unionData.s2f42ToHost.MsgHead.nSessionID = 1;

	len = sprintf(szModuleID, m_stpSma->stLayOutCfg.szEQPID);
	szModuleID[len] = '_';
	memcpy(&szModuleID[len+1], m_stpModCfg->szModuleName, MAX_LAYER_MODULE_ID_LEN);
	memcpy(sig.unionData.s2f42ToHost.szModuleID, szModuleID, MAX_EQ_MODULE_ID_LEN);
	
	m_pParent->SendSignalToSCH(&sig);
	
	//	명령 처리 수행후에는 반드시 Melnet Data를 Clear시킨다. 오동작의 원인이 될 수 있다.
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nCmdBitAddr);
	
	m_ShCommData_Master[eC2P_MachineCommand]->m_bWaitTime = FALSE;
}

void CBaseModule::C2P_AlarmTreatSequence(stAlarmClearReqType stClearAlarmData)
{
	long	nStep			=	0;
	long	nStepState		=	STEP_START;
	short   shSetData		=	0;
	long	nSetDataSize	=	sizeof(short);
	long	nCmdBitAddr	=   0;
	
	GetMasterDevBWAddr(eC2P_AlarmTreated, stClearAlarmData.nModuleID, B_L2_FROM_CIM_ALARM_CLR_REQ, 0);	
	GetLocalDevBWAddr(eC2P_AlarmTreated, stClearAlarmData.nModuleID, 0, W_L2_CLR_ALARM_RLY);	
	
	nCmdBitAddr = m_ShCommData_Master[eC2P_AlarmTreated]->m_shBitAddr;
	
	// Event 진행 전 Request Bit Off. (Request Bit Off 안된 경우 대비)
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eC2P_AlarmTreated]->m_shBitAddr);

	while(nStep != -1) 
	{
		switch(nStep++)
		{
		case 0:	//	동일 Sequence가 진행중이면 Request Bit Off
			if ( m_stpModRunInfo->stEQCmdRlyEvtIO.bAlarmClearReply == TRUE)
			{
				m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nCmdBitAddr);
				nStep = -1;
				nStepState = STEP_NG;
			}
			break;
		case 1:	//	해당 Machine Command에 대해 Melnet에 설정할 값을 선택하고, Melnet에 Write한다.
			shSetData = (short)stClearAlarmData.nAlarmID;
			
			m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, W_L1_CLR_ALARM_DATA, &nSetDataSize, &shSetData);
			m_ShCommData_Master[eC2P_AlarmTreated]->m_shSetData = shSetData;
			break;
			
		case 2:	//	Request Bit를 On하여 Command를 수행하도록 한다.
			m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, nCmdBitAddr);
			m_ShCommData_Master[eC2P_AlarmTreated]->m_bWaitTime = TRUE;
			m_ShCommData_Master[eC2P_AlarmTreated]->m_TimeCheck.StartTimer();
			nStep = -1;
			if(nStepState != STEP_NG)
				nStepState = STEP_OK;
			break;
		}
	}
	
	if(nStepState == STEP_NG) 
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Alarm Clear Request",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
}

void CBaseModule::C2P_AlarmTreatSequenceEnd()
{
	ULONG	nElaspedTime	= 0;
	long	nCmdBitAddr		= 0;
	short	shSetData		= 0;
	short	shRpyAddr		= 0;
	short	shRpyData		= 0;
	long	nRpySize		= sizeof(short);
	
	nCmdBitAddr = m_ShCommData_Master[eC2P_AlarmTreated]->m_shBitAddr;
	shSetData	= m_ShCommData_Master[eC2P_AlarmTreated]->m_shSetData;
	shRpyAddr = m_ShCommData_Local[eC2P_AlarmTreated]->m_shDataAddr;
	
	//	Command에 대한 응답 Check를 하고, Timeout이 발행하면 Alarm을 보고한다.
	if(m_stpModRunInfo->stEQCmdRlyEvtIO.bAlarmClearReply == FALSE)
	{
		if(m_ShCommData_Master[eC2P_AlarmTreated]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			nElaspedTime = m_ShCommData_Master[eC2P_AlarmTreated]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eC2P_AlarmTreated]->m_nElaspedTime = nElaspedTime;
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)]Alarm Clear(%04d) Request Timeout[Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, shSetData, nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG]Alarm Clear(%04d) Request Timeout[Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, shSetData, nElaspedTime);
			
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nCmdBitAddr);
			m_ShCommData_Master[eC2P_AlarmTreated]->m_bWaitTime = FALSE;
		}
		return;
	}
	
	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, shRpyAddr, &nRpySize, &shRpyData);
	nElaspedTime = m_ShCommData_Master[eC2P_AlarmTreated]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[eC2P_AlarmTreated]->m_nElaspedTime = nElaspedTime; 
	
	if(shRpyData == 0x41)
	{
		m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Alarm(%04d) Clear Request[Ack:0x%x] Complete!![Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, shSetData, shRpyData, nElaspedTime);
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] Alarm(%04d) Clear Request[Ack:0x%x] Complete!![Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, shSetData,shRpyData, nElaspedTime);
	}
	else if(shRpyData == 0x4E)
	{
		m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Alarm(%04d) Clear Request[Nak:0x%x] Complete!![Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, shSetData, shRpyData, nElaspedTime);
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] Alarm(%04d) Clear Request[Nak:0x%x] Complete!![Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, shSetData,shRpyData, nElaspedTime);
	}
	
	//	명령 처리 수행후에는 반드시 Melnet Data를 Clear시킨다. 오동작의 원인이 될 수 있다.
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nCmdBitAddr);
	m_ShCommData_Master[eC2P_AlarmTreated]->m_bWaitTime = FALSE;
}

void CBaseModule::C2P_AlarmResetSequenceEnd()
{
	short	shSetData = 0;
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	shSetData	= m_ShCommData_Master[eC2P_AlarmResetEvent]->m_shSetData;
	
	if(m_ShCommData_Master[eC2P_AlarmResetEvent]->m_bWaitTime == TRUE)
	{
		if(m_ShCommData_Master[eC2P_AlarmResetEvent]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			m_ShCommData_Master[eC2P_AlarmResetEvent]->m_bWaitTime = FALSE;
			
			// Host Report Send
			sig.nSignal = sigAlarmResetEvent;
			sig.nFrom	= PLC_TASK_ID;
			sig.nTo		= GUI_TASK_ID;
			m_pParent->SendSignalToSCH(&sig);
			
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] Alarm(%04d) Reset Request from HOST Complete!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, shSetData);
		}
		return;
	}
}

void CBaseModule::C2P_TimeSetSequence()
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	
	WORD	wYear	=	systime.wYear;
	WORD	wMonth	=	systime.wMonth;
	WORD	wDay	=	systime.wDay;
	WORD	wHour	=	systime.wHour;
	WORD	wMin	=	systime.wMinute;
	WORD	wSec	=	systime.wSecond;
	WORD	wDayofWeek	=	systime.wDayOfWeek;
	
	long	nIdx = 0;
	short   shSetData[7]	=	{ 0x00, };
	long	nSetDataSize	=	sizeof(short) * 7;
	
	GetMasterDevBWAddr(eC2P_TimeSet, m_stpModCfg->nModuleID, B_L2_TIMESET_SYNC_REQ, 0);	

	// Event 진행 전 Request Bit Off. (Request Bit Off 안된 경우 대비)
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eC2P_TimeSet]->m_shBitAddr);
	
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Time Set Event Started", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
	//	Date/Time Data
	shSetData[nIdx]	= wYear;	nIdx++;
	shSetData[nIdx]	= wMonth;	nIdx++;
	shSetData[nIdx]	= wDay;		nIdx++;
	shSetData[nIdx]	= wHour;	nIdx++;
	shSetData[nIdx]	= wMin;		nIdx++;
	shSetData[nIdx]	= wSec;		nIdx++;	
	shSetData[nIdx]	= wDayofWeek;		nIdx++;	
	
	m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, W_L1_TIMESET_SYNC_DATA, &nSetDataSize, shSetData);
	m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, m_ShCommData_Master[eC2P_TimeSet]->m_shBitAddr);
	
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Time Set Data Write to PLC", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
	
	m_ShCommData_Master[eC2P_TimeSet]->m_bWaitTime = TRUE;
	m_ShCommData_Master[eC2P_TimeSet]->m_TimeCheck.StartTimer();
}

void CBaseModule::C2P_TimeSetSequenceEnd()
{
	ULONG	nElaspedTime	= 0;
	long	nCmdBitAddr	= 0;
	
	nCmdBitAddr = m_ShCommData_Master[eC2P_TimeSet]->m_shBitAddr;
	
	//	Command에 대한 응답 Check를 하고, Timeout이 발행하면 Alarm을 보고한다.
	if(m_stpModRunInfo->stEQCmdRlyEvtIO.bTimeSetReply == FALSE)
	{
		if(m_ShCommData_Master[eC2P_TimeSet]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			nElaspedTime = m_ShCommData_Master[eC2P_TimeSet]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eC2P_TimeSet]->m_nElaspedTime = nElaspedTime;
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Time Set Event Request Timeout[Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Time Set Event Request Timeout[Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nCmdBitAddr);
			m_ShCommData_Master[eC2P_TimeSet]->m_bWaitTime = FALSE;
		}
		return;
	}
	
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nCmdBitAddr);
	nElaspedTime = m_ShCommData_Master[eC2P_TimeSet]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[eC2P_TimeSet]->m_nElaspedTime = nElaspedTime; 
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Time Set Event Complete!![Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] Time Set Event Complete!![Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	m_ShCommData_Master[eC2P_TimeSet]->m_bWaitTime = FALSE;
}


void CBaseModule::C2P_ECOModeChangeSequence(stECOChangeType stECO)
{
	long	nStep				=	0;
	long	nStepState			=	STEP_START;
	short	nSetDataEOMD		=	sizeof(short);
	short	nSetDataEOV			=	sizeof(short);
	long	nCmdBitAddr			=	0;
	long	nCmdEOMD_WordAddr	=	0;
	long	nCmdEOV_WordAddr	=	0;
	long	nSetDataSize		=	sizeof(short);
		
	GetMasterDevBWAddr(eC2P_ECOModeChange, stECO.nModuleID, B_L2_FROM_CIM_ECO_MODE_DATA_CHANGE_REQ, W_L1_ECO_MODE_EOMD_DATA);	
	nCmdEOMD_WordAddr = m_ShCommData_Master[eC2P_ECOModeChange]->m_shDataAddr;
	nCmdBitAddr = m_ShCommData_Master[eC2P_ECOModeChange]->m_shBitAddr;

	GetMasterDevBWAddr(eC2P_ECOModeChange, stECO.nModuleID, B_L2_FROM_CIM_ECO_MODE_DATA_CHANGE_REQ, W_L1_ECO_MODE_EOV_DATA);		
	nCmdEOV_WordAddr = m_ShCommData_Master[eC2P_ECOModeChange]->m_shDataAddr;

	// Event 진행 전 Request Bit Off. (Request Bit Off 안된 경우 대비)
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eC2P_ECOModeChange]->m_shBitAddr);

	while(nStep != -1) 
	{
		switch(nStep++)
		{
		case 0:	//	동일 Sequence가 진행중이면 Request Bit Off
			if ( m_stpModRunInfo->stDataChangeRlyEvtIO.bECORly == TRUE )
			{
				m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nCmdBitAddr);
				nStep = -1;
				nStepState = STEP_NG;
			}
			break;
		case 1:	//	Data 유효성 Check
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECO Mode Change (EOMD=%d, EOV=%d) Started!", m_stpModCfg->nModuleID,m_stpModCfg->szModuleName, stECO.nEOMD, stECO.nEOV);

			switch(short(stECO.nEOMD))
			{
			case eEOMD_All_Unit:
				break;
			default:
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)]Unknown ECO Mode EOMD(%d)", m_stpModCfg->nModuleID,m_stpModCfg->szModuleName, stECO.nEOMD);
				nStep = -1;
				nStepState = STEP_NG;
				break;
			}
			break;

			switch(short(stECO.nEOV))
			{
			case eEOV_WorkingMode:
			case eEOV_StandyByMode_1:
				break;
			default:
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)]Unknown ECO Mode EOV(%d)", m_stpModCfg->nModuleID,m_stpModCfg->szModuleName, stECO.nEOV);
				nStep = -1;
				nStepState = STEP_NG;
				break;
			}
			break;
		
		case 2:	//	해당 Machine Command에 대해 Melnet에 설정할 값을 선택하고, Melnet에 Write한다.

			// Write EOMD
			nSetDataEOMD = short(stECO.nEOMD);
			m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, nCmdEOMD_WordAddr, &nSetDataSize, &nSetDataEOMD);

			//Write EOV
			nSetDataEOV = short(stECO.nEOV);		
			m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, nCmdEOV_WordAddr, &nSetDataSize, &nSetDataEOV);
			
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECO Mode Change Data Write To PLC EOMD(%d), EOV(%d)", m_stpModCfg->nModuleID,m_stpModCfg->szModuleName, stECO.nEOMD, stECO.nEOV);
			break;
			
		case 3:	//	Request Bit를 On하여 ECO Mode Change를 수행하도록 한다.
			m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, nCmdBitAddr);	
			m_ShCommData_Master[eC2P_ECOModeChange]->m_bWaitTime = TRUE;
			m_ShCommData_Master[eC2P_ECOModeChange]->m_TimeCheck.StartTimer();
			nStep = -1;
			if(nStepState != STEP_NG)
				nStepState = STEP_OK;
			break;
		}
	}
	
	if(nStepState == STEP_NG) 
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] ECO Mode Change",	m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
}

void CBaseModule::C2P_ECOModeChangeSequenceEnd()
{
	ULONG	nElaspedTime	= 0;
	long	nCmdBitAddr		= 0;
	short	shRpyAddr		= 0;
	short	shRpyData		= 0;
	long	nRpySize		= sizeof(short);
	
	char szModuleID[MAX_EQ_MODULE_ID_LEN+1];
	memset(szModuleID, 0x00, MAX_EQ_MODULE_ID_LEN);
	
	GetLocalDevBWAddr(eC2P_ECOModeChange, m_stpModRunInfo->nModuleID, 0, W_L2_ECO_MODE_CHNG_RLY);	
	shRpyAddr = m_ShCommData_Local[eC2P_ECOModeChange]->m_shDataAddr;
	
	nCmdBitAddr = m_ShCommData_Master[eC2P_ECOModeChange]->m_shBitAddr;	
	
	//	ECO Mode Change 에 대한 응답 Check를 하고, Timeout이 발행하면 Alarm을 보고한다.
	if(m_stpModRunInfo->stDataChangeRlyEvtIO.bECORly == FALSE)
	{
		if(m_ShCommData_Master[eC2P_ECOModeChange]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			nElaspedTime = m_ShCommData_Master[eC2P_ECOModeChange]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eC2P_ECOModeChange]->m_nElaspedTime = nElaspedTime;
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECO Mode Change Timeout[Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			
			m_ShCommData_Master[eC2P_ECOModeChange]->m_bWaitTime = FALSE;
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nCmdBitAddr);
		}
		return;
	}
	
	//	다음 명령을 수행할 수 있도록 응답 받은 Code를 상위에 보고한다.
	nElaspedTime = m_ShCommData_Master[eC2P_ECOModeChange]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[eC2P_ECOModeChange]->m_nElaspedTime = nElaspedTime;
	
	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, shRpyAddr, &nRpySize, &shRpyData);
	
	if ( shRpyData == 0x41 ) // Ack
	{
		m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECO Mode Change AcK[0x:%4x] Reply Complete[Elasped Time : %dms]!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, shRpyData, nElaspedTime);	
	}
	else	//	Nak
	{
		m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECO MODE Change NAK[0x:%4x] Reply Complete[Elasped Time : %dms]!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, shRpyData,nElaspedTime);
	}

	//	명령 처리 수행후에는 반드시 Melnet Data를 Clear시킨다. 오동작의 원인이 될 수 있다.
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nCmdBitAddr);
	
	m_ShCommData_Master[eC2P_ECOModeChange]->m_bWaitTime = FALSE;
}

void CBaseModule::C2P_OperatorCallSequence(stTerminalMsgType stTerminalMsg)
{
	long	nStep			=	0;
	long	nStepState		=	STEP_START;
	//	short	shCmdPacket[4]	=	{ 0x00, };
	short   shSetData[40]	=	{ 0x00, };
	long	nSetDataSize	=	sizeof(short) * 40;
	long	nCmdBitAddr		=	B_L2_FROM_CIM_OPERATORCALL_MSG_REQ;
	BOOL	bNextStep		=	FALSE;
	
	GetMasterDevBWAddr(eC2P_OperatorCall, stTerminalMsg.nModuleNo, B_L2_FROM_CIM_OPERATORCALL_MSG_REQ, 0);
	
	nCmdBitAddr = m_ShCommData_Master[eC2P_OperatorCall]->m_shBitAddr;
	
	// Event 진행 전 Request Bit Off. (Request Bit Off 안된 경우 대비)
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eC2P_OperatorCall]->m_shBitAddr);

	while(nStep != -1) 
	{
		switch(nStep++)
		{
		case 0:	
			if ( m_stpModRunInfo->stEQCmdRlyEvtIO.bOperatorCallReply == TRUE)
			{
				m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nCmdBitAddr);
				nStep = -1;
				nStepState = STEP_NG;
			}
			break;
		case 1:	
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] OperatorCall MSG Request Started!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
			memcpy(shSetData, stTerminalMsg.szMsg, nSetDataSize);
			m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, W_L1_TERMINAL_MSG_DATA, &nSetDataSize, shSetData);
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] OperatorCall MSG Request Data[Msg:%s] Writed!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,shSetData);
			
		case 2:
			m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, nCmdBitAddr);
			m_ShCommData_Master[eC2P_OperatorCall]->m_bWaitTime = TRUE;
			m_ShCommData_Master[eC2P_OperatorCall]->m_TimeCheck.StartTimer();
			nStep = -1;
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] OperatorCall MSG Request Bit On!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
			if(nStepState != STEP_NG)
				nStepState = STEP_OK;
			break;
		}
	}
	
	if(nStepState == STEP_NG) 
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] OperatorCall Message Request ",m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
	
}

void CBaseModule::C2P_TerminalMsgSequence(stTerminalMsgType stTerminalMsg)
{
	long	nStep			=	0;
	long	nStepState		=	STEP_START;
	//	short	shCmdPacket[4]	=	{ 0x00, };
	short   shSetData[40]	=	{ 0x00, };
	long	nSetDataSize	=	sizeof(short) * 40;
	long	nCmdBitAddr		=	B_L2_FROM_CIM_TERMINAL_MSG_REQ;
	BOOL	bNextStep		=	FALSE;
	
	GetMasterDevBWAddr(eC2P_TerminalMsg, stTerminalMsg.nModuleNo, B_L2_FROM_CIM_TERMINAL_MSG_REQ, 0);
	
	nCmdBitAddr = m_ShCommData_Master[eC2P_TerminalMsg]->m_shBitAddr;
	
	// Event 진행 전 Request Bit Off. (Request Bit Off 안된 경우 대비)
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eC2P_TerminalMsg]->m_shBitAddr);

	while(nStep != -1) 
	{
		switch(nStep++)
		{
		case 0:	
			if ( m_stpModRunInfo->stEQCmdRlyEvtIO.bTerminalMsgReply == TRUE)
			{
				m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nCmdBitAddr);
				nStep = -1;
				nStepState = STEP_NG;
			}
			break;
		case 1:	
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Terminal MSG Request Started!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
			memcpy(shSetData, stTerminalMsg.szMsg, nSetDataSize);
			m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, W_L1_TERMINAL_MSG_DATA, &nSetDataSize, shSetData);
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Terminal MSG Request Data[Msg:%s] Writed!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,shSetData);
			
		case 2:
			m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, nCmdBitAddr);
			m_ShCommData_Master[eC2P_TerminalMsg]->m_bWaitTime = TRUE;
			m_ShCommData_Master[eC2P_TerminalMsg]->m_TimeCheck.StartTimer();
			nStep = -1;
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Terminal MSG Request Bit On!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
			if(nStepState != STEP_NG)
				nStepState = STEP_OK;
			break;
		}
	}
	
	if(nStepState == STEP_NG) 
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Terminal Message Request ",m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
	
}

void CBaseModule::C2P_OperatorCallSequenceEnd()
{
	ULONG	nElaspedTime	= 0;
	long	nCmdBitAddr	= 0;
	
	nCmdBitAddr = m_ShCommData_Master[eC2P_OperatorCall]->m_shBitAddr;
	
	//	Command에 대한 응답 Check를 하고, Timeout이 발행하면 Alarm을 보고한다.
	if(m_stpModRunInfo->stEQCmdRlyEvtIO.bOperatorCallReply == FALSE)
	{
		if(m_ShCommData_Master[eC2P_OperatorCall]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			nElaspedTime = m_ShCommData_Master[eC2P_OperatorCall]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eC2P_OperatorCall]->m_nElaspedTime = nElaspedTime;
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Operatorcall MSG Request Timeout[Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Operatorcall MSG Request Timeout[Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nCmdBitAddr);
			m_ShCommData_Master[eC2P_OperatorCall]->m_bWaitTime = FALSE;
		}
		return;
	}
	
	nElaspedTime = m_ShCommData_Master[eC2P_OperatorCall]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[eC2P_OperatorCall]->m_nElaspedTime = nElaspedTime; 
	
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Operatorcall MSG Request Complete!![Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] Operatorcall MSG Request Complete!![Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	
	//	명령 처리 수행후에는 반드시 Melnet Data를 Clear시킨다. 오동작의 원인이 될 수 있다.
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nCmdBitAddr);
	m_ShCommData_Master[eC2P_OperatorCall]->m_bWaitTime = FALSE;
}

void CBaseModule::C2P_TerminalMsgSequenceEnd()
{
	ULONG	nElaspedTime	= 0;
	long	nCmdBitAddr	= 0;
	
	nCmdBitAddr = m_ShCommData_Master[eC2P_TerminalMsg]->m_shBitAddr;
	
	//	Command에 대한 응답 Check를 하고, Timeout이 발행하면 Alarm을 보고한다.
	if(m_stpModRunInfo->stEQCmdRlyEvtIO.bTerminalMsgReply == FALSE)
	{
		if(m_ShCommData_Master[eC2P_TerminalMsg]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			nElaspedTime = m_ShCommData_Master[eC2P_TerminalMsg]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eC2P_TerminalMsg]->m_nElaspedTime = nElaspedTime;
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Terminal MSG Request Timeout[Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Terminal MSG Request Timeout[Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nCmdBitAddr);
			m_ShCommData_Master[eC2P_TerminalMsg]->m_bWaitTime = FALSE;
		}
		return;
	}
	
	nElaspedTime = m_ShCommData_Master[eC2P_TerminalMsg]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[eC2P_TerminalMsg]->m_nElaspedTime = nElaspedTime; 
	
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Terminal MSG Request Complete!![Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] Terminal MSG Request Complete!![Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	
	//	명령 처리 수행후에는 반드시 Melnet Data를 Clear시킨다. 오동작의 원인이 될 수 있다.
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nCmdBitAddr);
	m_ShCommData_Master[eC2P_TerminalMsg]->m_bWaitTime = FALSE;
}

// Operator Call 대용
void CBaseModule::C2P_BuzzerStopSequence(stBuzzerCtrlType stBuzzStop)
{
	long	nCmdBitAddr	= 0;
	
	GetMasterDevBWAddr(eC2P_BuzzerStop,stBuzzStop.nModuleNo,B_L2_FROM_CIM_BUZZER_STOP_REQ,0);
	nCmdBitAddr = m_ShCommData_Master[eC2P_BuzzerStop]->m_shBitAddr;
	
	m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, nCmdBitAddr);
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Operator Call Event : Request ON", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
	
	if(m_ShCommData_Master[eC2P_BuzzerStop]->m_bWaitTime == FALSE)
	{
		m_ShCommData_Master[eC2P_BuzzerStop]->m_TimeCheck.StartTimer();
		m_ShCommData_Master[eC2P_BuzzerStop]->m_bWaitTime = TRUE;
	}
}

void CBaseModule::C2P_BuzzerStopSequenceEnd()
{
	ULONG	nElaspedTime	= 0;
	long	nCmdBitAddr	= 0;
	
	nCmdBitAddr = m_ShCommData_Master[eC2P_BuzzerStop]->m_shBitAddr;
	
	//	Command에 대한 응답 Check를 하고, Timeout이 발행하면 Alarm을 보고한다.
	if(m_stpModRunInfo->stEQCmdRlyEvtIO.bBuzzerStopReply == FALSE)
	{
		if(m_ShCommData_Master[eC2P_BuzzerStop]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			nElaspedTime = m_ShCommData_Master[eC2P_BuzzerStop]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eC2P_BuzzerStop]->m_nElaspedTime = nElaspedTime;
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Buzzer Stop Event Request Timeout[Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Buzzer Stop Event Request Timeout[Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nCmdBitAddr);
			m_ShCommData_Master[eC2P_BuzzerStop]->m_bWaitTime = FALSE;
		}
		return;
	}
	
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nCmdBitAddr);
	nElaspedTime = m_ShCommData_Master[eC2P_BuzzerStop]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[eC2P_BuzzerStop]->m_nElaspedTime = nElaspedTime; 
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Buzzer Stop Event Complete!![Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] Buzzer Stop Event Complete!![Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	m_ShCommData_Master[eC2P_BuzzerStop]->m_bWaitTime = FALSE;
}

void CBaseModule::C2P_MNCellJudgeFromHSTSequence(long nAckCode)
{
	long	nCmdBitAddr	= 0;
	short	shAckCode		= 0;
	long	nDataSize		= sizeof(short);
	
	GetMasterDevBWAddr(eMNCellJudge, eModuleType_Edge ,B_L2_MN_JUDGE_REQ,0);
	nCmdBitAddr = m_ShCommData_Master[eMNCellJudge]->m_shBitAddr;
	
	shAckCode = (short)nAckCode;
	m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, W_L2_MN_JUDGE_RLY_DATA, &nDataSize, &shAckCode);
	m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, B_L2_MN_JUDGE_REQ);
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Manual Cell Load Judge from Host : Request ON", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
	
	if(m_ShCommData_Master[eMNCellJudge]->m_bWaitTime == FALSE)
	{
		m_ShCommData_Master[eMNCellJudge]->m_TimeCheck.StartTimer();
		m_ShCommData_Master[eMNCellJudge]->m_bWaitTime = TRUE;
	}
}

void CBaseModule::C2P_MNCellJudgeFromHSTSequenceEnd()
{
	ULONG	nElaspedTime	= 0;
	short	shCmdBitAddr	= 0;
	
	shCmdBitAddr = m_ShCommData_Master[eMNCellJudge]->m_shBitAddr;
	
	//	Command에 대한 응답 Check를 하고, Timeout이 발행하면 Alarm을 보고한다.
	if(m_stpModRunInfo->stDataChangeRlyEvtIO.bMNCellJudgeRly == FALSE)
	{
		if(m_ShCommData_Master[eMNCellJudge]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			nElaspedTime = m_ShCommData_Master[eMNCellRead]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eMNCellJudge]->m_nElaspedTime = nElaspedTime;
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Manual Cell Load Judge from Host : Timeout[Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Manual Cell Load Judge from Host Request Timeout[Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, B_L2_MN_JUDGE_REQ);
			m_ShCommData_Master[eMNCellJudge]->m_bWaitTime = FALSE;
		}
		return;
	}
	
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, B_L2_MN_JUDGE_REQ);
	nElaspedTime = m_ShCommData_Master[eMNCellJudge]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[eMNCellJudge]->m_nElaspedTime = nElaspedTime; 
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Manual Cell Load Judge from Host Complete!![Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] Manual Cell Load Judge from Host Complete!![Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	m_ShCommData_Master[eMNCellJudge]->m_bWaitTime = FALSE;
}

void CBaseModule::C2P_MNCellReadSequence()
{
	long	nCmdBitAddr	= 0;
	
	GetMasterDevBWAddr(eMNCellRead, eModuleType_Edge ,B_L2_MNCELL_READ_REQ,0);
	nCmdBitAddr = m_ShCommData_Master[eMNCellRead]->m_shBitAddr;
	
	m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, B_L2_MNCELL_READ_REQ);
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Manual Cell Load Glass Data Read : Request ON", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
	
	if(m_ShCommData_Master[eMNCellRead]->m_bWaitTime == FALSE)
	{
		m_ShCommData_Master[eMNCellRead]->m_TimeCheck.StartTimer();
		m_ShCommData_Master[eMNCellRead]->m_bWaitTime = TRUE;
	}
}

void CBaseModule::C2P_MNCellReadSequenceEnd()
{
	ULONG	nElaspedTime	= 0;
	short	shCmdBitAddr	= 0;
	
	shCmdBitAddr = m_ShCommData_Master[eMNCellRead]->m_shBitAddr;
	
	//	Command에 대한 응답 Check를 하고, Timeout이 발행하면 Alarm을 보고한다.
	if(m_stpModRunInfo->stDataChangeRlyEvtIO.bMNCellReadRly == FALSE)
	{
		if(m_ShCommData_Master[eMNCellRead]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			nElaspedTime = m_ShCommData_Master[eMNCellRead]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eMNCellRead]->m_nElaspedTime = nElaspedTime;
			
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Manual Cell Load Glass Data Read : Timeout[Elasped Time : %dms]", 
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Manual Cell Load Glass Data Read Request Timeout[Elasped Time : %dms]", 
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, B_L2_MNCELL_READ_REQ);
			m_ShCommData_Master[eMNCellRead]->m_bWaitTime = FALSE;
		}
		return;
	}
	
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, B_L2_MNCELL_READ_REQ);
	nElaspedTime = m_ShCommData_Master[eMNCellRead]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[eMNCellRead]->m_nElaspedTime = nElaspedTime; 
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Manual Cell Load Glass Data Read Event Complete!![Elasped Time : %dms]", 
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] Manual Cell Load Glass Data Read Event Complete!![Elasped Time : %dms]", 
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	m_ShCommData_Master[eMNCellRead]->m_bWaitTime = FALSE;
}

void CBaseModule::P2C_AlarmOccuredSequence()
{
	long	nStep			=	0;
	long	nStepState		=	STEP_START;
	long	nDataSize		=	sizeof(short) * 42;
	short	shEvent[42]		=	{ 0x00, };
	long	nCmdBitAddr	=   0;
	short	shCmdDataAddr	=   0;
	long	nTimeout = 0;

	GetMasterDevBWAddr(eP2C_AlarmOccured, m_stpModCfg->nModuleID, B_L2_TO_CIM_ALARM_OCCUR_RLY, 0);	
	nCmdBitAddr = m_ShCommData_Master[eP2C_AlarmOccured]->m_shBitAddr;
	GetLocalDevBWAddr(eP2C_AlarmOccured, m_stpModCfg->nModuleID, 0, W_L2_OCCUR_ALID);	
	shCmdDataAddr = m_ShCommData_Local[eP2C_AlarmOccured]->m_shDataAddr;

	// Event 진행 전 Reply Bit Off. (Reply Bit Off 안된 경우 대비)
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_AlarmOccured]->m_shBitAddr);
	
	while(nStep != -1)
	{
		switch(nStep++)
		{
		case 0:
			if(m_stpModRunInfo->stEQCmdRlyEvtIO.bAlarmOccured == FALSE)
			{
				m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nCmdBitAddr);
				nStep = -1;
				nStepState = STEP_NG;
			}				
			break;
			
		case 1:	// Event Data Read
			m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, shCmdDataAddr, &nDataSize, shEvent);
			m_ShCommData_Master[eP2C_AlarmOccured]->m_nAlarmID = shEvent[eAlarmID];
			m_ShCommData_Master[eP2C_AlarmOccured]->m_nAlarmCode = shEvent[eAlarmCode];
			CopyMemory(m_ShCommData_Master[eP2C_AlarmOccured]->m_szAlarmText, &shEvent[eAlarmText], MAX_ALARM_TEXT_LEN);
			break;
			
		case 2: // Reply
			m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, nCmdBitAddr);
			m_ShCommData_Master[eP2C_AlarmOccured]->m_bWaitTime = TRUE;
			m_ShCommData_Master[eP2C_AlarmOccured]->m_TimeCheck.StartTimer();
			nStep = -1;
			if(nStepState != STEP_NG)
				nStepState = STEP_OK;
			break;
		}
	}

	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Alarm ID(%04d) Alarm Code(%04d) Alarm Text(%s)Occured Event Start!!",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, m_ShCommData_Master[eP2C_AlarmOccured]->m_nAlarmID, 
		m_ShCommData_Master[eP2C_AlarmOccured]->m_nAlarmCode, m_ShCommData_Master[eP2C_AlarmOccured]->m_szAlarmText);


	if(nStepState == STEP_NG)
	{
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Alarm Occured Event", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
	}
}

void CBaseModule::P2C_AlarmOccuredSequenceEnd()
{
	ULONG	nElaspedTime	= 0;
	long	shEvent			= m_nLastAlmOccurID;
	long	nReplyBitAddr  = m_ShCommData_Master[eP2C_AlarmOccured]->m_shBitAddr;	
	
	// Reply에 대한 응답 Check를 하고, Timeout이 발행하면 Alarm을 보고한다.
	if(m_stpModRunInfo->stEQCmdRlyEvtIO.bAlarmOccured ==TRUE )
	{
		if(m_ShCommData_Master[eP2C_AlarmOccured]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			nElaspedTime = m_ShCommData_Master[eP2C_AlarmOccured]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eP2C_AlarmOccured]->m_nElaspedTime = nElaspedTime;
			
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Alarm ID(%04d) Occured Event Bit not Off[Elasped Time : %dms]", 
																m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, shEvent, nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Alarm ID(%04d) Occured Event Bit not Off[Elasped Time : %dms]", 
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, shEvent, nElaspedTime);
			
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nReplyBitAddr);
			m_ShCommData_Master[eP2C_AlarmOccured]->m_bWaitTime = FALSE;
		}
		return;
	}

	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	sig.nSignal = sigAlarmOccured;
	sig.nFrom	= PLC_TASK_ID;
	sig.nTo		= GUI_TASK_ID;
	
	sig.unionData.stAlarmOccuredData.nAlarmID = m_ShCommData_Master[eP2C_AlarmOccured]->m_nAlarmID;
	sig.unionData.stAlarmOccuredData.nAlarmCode = m_ShCommData_Master[eP2C_AlarmOccured]->m_nAlarmCode;
	CopyMemory(sig.unionData.stAlarmOccuredData.szAlarmText, m_ShCommData_Master[eP2C_AlarmOccured]->m_szAlarmText, MAX_ALARM_TEXT_LEN);
	sig.unionData.stAlarmOccuredData.nModuleID = m_stpModCfg->nModuleID;
	
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nReplyBitAddr);
	m_pParent->SendSignalToSCH(&sig);
	nElaspedTime = m_ShCommData_Master[eP2C_AlarmOccured]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[eP2C_AlarmOccured]->m_nElaspedTime = nElaspedTime;
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Alarm ID(%04d) Alarm Code(%04d) Occured Event Complete!![Elasped Time : %dms]", 
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, sig.unionData.stAlarmOccuredData.nAlarmID, 
		sig.unionData.stAlarmOccuredData.nAlarmCode, nElaspedTime);
	m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] Alarm ID(%04d) Alarm Code(%04d) Occured Event Complete!![Elasped Time : %dms]", 
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, sig.unionData.stAlarmOccuredData.nAlarmID, 
		sig.unionData.stAlarmOccuredData.nAlarmCode, nElaspedTime);
	
	m_ShCommData_Master[eP2C_AlarmOccured]->m_bWaitTime = FALSE;
	
}

void CBaseModule::P2C_AlarmTreatedSequence()
{
	long	nStep			=	0;
	long	nStepState		=	STEP_START;	
	long	nDataSize		=	sizeof(short)*42;
	short	shEvent[42]		=	{ 0x00, };
	long	nCmdBitAddr		=   0;
	short	shCmdDataAddr	=   0;
	long    nTimeout = 0;

	GetMasterDevBWAddr(eP2C_AlarmTreated, m_stpModCfg->nModuleID, B_L2_TO_CIM_ALARM_TREAT_RLY, 0);	
	nCmdBitAddr = m_ShCommData_Master[eP2C_AlarmTreated]->m_shBitAddr;
	GetLocalDevBWAddr(eP2C_AlarmTreated, m_stpModCfg->nModuleID, 0, W_L2_TREAT_ALID);	
	shCmdDataAddr = m_ShCommData_Local[eP2C_AlarmTreated]->m_shDataAddr;

	// Event 진행 전 Reply Bit Off. (Reply Bit Off 안된 경우 대비)
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_AlarmTreated]->m_shBitAddr);
	
	while(nStep != -1) 
	{
		switch(nStep++)
		{
		case 0:
			if(m_stpModRunInfo->stEQCmdRlyEvtIO.bAlarmTreated == FALSE)
			{
				m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nCmdBitAddr);
				nStep = -1;
				nStepState = STEP_NG;
			}				
			break;
			
		case 1:	// Event Data Read
			m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, shCmdDataAddr, &nDataSize, shEvent);
			m_ShCommData_Master[eP2C_AlarmTreated]->m_nAlarmID = shEvent[eAlarmID];
			m_ShCommData_Master[eP2C_AlarmTreated]->m_nAlarmCode = shEvent[eAlarmCode];
			CopyMemory(m_ShCommData_Master[eP2C_AlarmTreated]->m_szAlarmText, &shEvent[eAlarmText], MAX_ALARM_TEXT_LEN);
			
		case 2: // Reply
			m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, nCmdBitAddr);
			m_ShCommData_Master[eP2C_AlarmTreated]->m_bWaitTime = TRUE;
			m_ShCommData_Master[eP2C_AlarmTreated]->m_TimeCheck.StartTimer();
			nStep = -1;
			if(nStepState != STEP_NG)
				nStepState = STEP_OK;
			break;
		}
	}
	
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Alarm ID(%04d) Alarm Code(%04d) Treated Event Start!!",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, m_stpSma->stAlarmTreatedEvent.nAlarmID, m_stpSma->stAlarmTreatedEvent.nAlarmCode);

	if(nStepState == STEP_NG) 
	{
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Alarm Treated Event", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
	}
}

void CBaseModule::P2C_AlarmTreatedSequenceEnd()
{
	ULONG	nElaspedTime	= 0;
	long	shEvent			= m_nLastAlmTreatID;
	long	nReplyBitAddr	= m_ShCommData_Master[eP2C_AlarmTreated]->m_shBitAddr;
	
	// Reply에 대한 응답 Check를 하고, Timeout이 발행하면 Alarm을 보고한다.
	if(m_stpModRunInfo->stEQCmdRlyEvtIO.bAlarmTreated == TRUE)
	{
		if(m_ShCommData_Master[eP2C_AlarmTreated]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			nElaspedTime = m_ShCommData_Master[eP2C_AlarmTreated]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eP2C_AlarmTreated]->m_nElaspedTime = nElaspedTime;
			
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Alarm ID(%04d) Treated Event Bit not Off[Elasped Time : %dms]", 
																m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, shEvent, nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Alarm ID(%04d) Treated Event Bit not Off[Elasped Time : %dms]", 
																m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, shEvent, nElaspedTime);
			
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nReplyBitAddr);
			m_ShCommData_Master[eP2C_AlarmTreated]->m_bWaitTime = FALSE;
		}
		return;
	}
	
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	sig.nSignal = sigAlarmTreated;
	sig.nFrom	= PLC_TASK_ID;
	sig.nTo		= GUI_TASK_ID;
	
	sig.unionData.stAlarmTreatedData.nAlarmID = m_ShCommData_Master[eP2C_AlarmTreated]->m_nAlarmID;
	sig.unionData.stAlarmTreatedData.nAlarmCode = m_ShCommData_Master[eP2C_AlarmTreated]->m_nAlarmCode;
	CopyMemory(sig.unionData.stAlarmTreatedData.szAlarmText, m_ShCommData_Master[eP2C_AlarmTreated]->m_szAlarmText, MAX_ALARM_TEXT_LEN);
	sig.unionData.stAlarmTreatedData.nModuleID = m_stpModCfg->nModuleID;
	
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nReplyBitAddr);
	m_pParent->SendSignalToSCH(&sig);
	nElaspedTime = m_ShCommData_Master[eP2C_AlarmTreated]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[eP2C_AlarmTreated]->m_nElaspedTime = nElaspedTime;
	
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Alarm ID(%04d) Treated Event Complete!![Elasped Time : %dms]",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, sig.unionData.stAlarmTreatedData.nAlarmID, nElaspedTime);
	m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] Alarm ID(%04d) Treated Event Complete!![Elasped Time : %dms]", 
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, sig.unionData.stAlarmTreatedData.nAlarmID, nElaspedTime);
	
	m_ShCommData_Master[eP2C_AlarmTreated]->m_bWaitTime = FALSE;
}

//void CBaseModule::P2C_ECOModeChangeSequence()
//{
//	long	nStep			=	0;
//	long	nStepState		=	STEP_START;
//	long	nCmdBitAddr		=   0;
//	long	nTimeout		=	0;
//	long	nDataSize		=	sizeof(short);
//	
//	short	shRlyData		=	0x41;
//	short	shEOMDDataAddr	=   0;
//	short	shEOVDataAddr	=   0;
//	short	shECO_EOMD		=	0;
//	short	shECO_EOV		=	0;
//
//	GetMasterDevBWAddr(eP2C_ECOModeChange, m_stpModCfg->nModuleID, B_L2_TO_CIM_ECO_MODE_CHANGE_RLY, W_L2_FROM_CIM_ECO_MODE_CHANGE_RLY_DATA);	
//	nCmdBitAddr = m_ShCommData_Master[eP2C_ECOModeChange]->m_shBitAddr;
//
//	GetLocalDevBWAddr(eP2C_ECOModeChange, m_stpModCfg->nModuleID, 0, W_L2_ECO_MODE_EOMD_EVENT);	
//	shEOMDDataAddr = m_ShCommData_Local[eP2C_ECOModeChange]->m_shDataAddr;
//
//	GetLocalDevBWAddr(eP2C_ECOModeChange, m_stpModCfg->nModuleID, 0, W_L2_ECO_MODE_EOV_EVENT);	
//	shEOVDataAddr = m_ShCommData_Local[eP2C_ECOModeChange]->m_shDataAddr;
//	
//	while(nStep != -1)
//	{
//		switch(nStep++)
//		{
//		case 0:
//			if(m_stpModRunInfo->stDataChangeRlyEvtIO.bECOEvtReq == FALSE)
//			{
//				m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nCmdBitAddr);
//				nStep = -1;
//				nStepState = STEP_NG;
//			}				
//			break;
//			
//		case 1:	// Event Data Read
//			m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, shEOMDDataAddr, &nDataSize, &shECO_EOMD);
//			m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, shEOVDataAddr,	&nDataSize, &shECO_EOV);
//
//			m_ShCommData_Master[eP2C_ECOModeChange]->m_stECOData.nEOMD = shECO_EOMD;
//			m_ShCommData_Master[eP2C_ECOModeChange]->m_stECOData.nEOV = shECO_EOV;
//			
//			//////////////////////////////////////////////////////////////////////////	
//			//PLC에서 사용하지 않는 EOMD or EOV Write시에 NAK
//			if (shECO_EOMD <= 0 || shECO_EOMD > MAX_ECO_EOMD_COUNT )
//			{
//				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECO Mode Change Event by EOMD Value Error!! shECO_EOMD[%d]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, shECO_EOMD);
//				shRlyData = 0x4E;
//				nStepState = STEP_NG;
//			}
//
//			if (shECO_EOV < 0)
//			{
//				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECO Mode Change Event by EOMD Value Error!! shECO_EOV[%d]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, shECO_EOV);
//				shRlyData = 0x4E;
//				nStepState = STEP_NG;
//			}
//			
//			//////////////////////////////////////////////////////////////////////////
//
//			break;
//
//		case 2: // Reply
//			m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, m_ShCommData_Master[eP2C_ECOModeChange]->m_shDataAddr, &nDataSize, &shRlyData);
//			m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, nCmdBitAddr);
//			m_ShCommData_Master[eP2C_ECOModeChange]->m_bWaitTime = TRUE;
//			m_ShCommData_Master[eP2C_ECOModeChange]->m_TimeCheck.StartTimer();
//			nStep = -1;
//			if(nStepState != STEP_NG)
//				nStepState = STEP_OK;
//			break;
//		}
//	}
//
// 	m_stpSma->stSysRunInfo.stModRunInfo[m_stpModCfg->nModuleID].stECOMode.stECO_EOMD[shECO_EOMD].nECO_EOV = shECO_EOV;
//
//	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECO EOMD(%d) ECO EOV(%d) Event Start!!", 
//		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, shECO_EOMD, shECO_EOV);
//
//	if(nStepState == STEP_NG)
//	{
//		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] ECO Event", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
//	}
//}
//
//void CBaseModule::P2C_ECOModeChangeSequenceEnd()
//{
//	ULONG	nElaspedTime	= 0;	
//	long	nReplyBitAddr  = m_ShCommData_Master[eP2C_ECOModeChange]->m_shBitAddr;	
//
//	stQueueRootType sig;
//	memset(&sig, 0x00, sizeof(stQueueRootType));
//
//	if(m_stpModRunInfo->stDataChangeRlyEvtIO.bECOEvtReq ==TRUE )
//	{
//		if(m_ShCommData_Master[eP2C_ECOModeChange]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
//		{
//			nElaspedTime = m_ShCommData_Master[eP2C_ECOModeChange]->m_TimeCheck.GetTimerAfterStart();
//			m_ShCommData_Master[eP2C_ECOModeChange]->m_nElaspedTime = nElaspedTime;
//			
//			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECO EOMD(%d) ECO EOV(%d) Event Bit not Off[Elasped Time : %dms]", 
//																m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, m_ShCommData_Master[eP2C_ECOModeChange]->m_stECOData.nEOMD,
//																m_ShCommData_Master[eP2C_ECOModeChange]->m_stECOData.nEOV, nElaspedTime);
//			
//			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nReplyBitAddr);
//			m_ShCommData_Master[eP2C_ECOModeChange]->m_bWaitTime = FALSE;
//		}
//		return;
//	}
//	
//	//////////////////////////////////////////////////////////////////////////
//	// PLC에서 ECO Mode Change 후 CIM에 보고
//	sig.nSignal = eSigECOChange;
//	sig.nFrom	= PLC_TASK_ID;
//	sig.nTo		= SCH_TASK_ID;
//	sig.unionData.stECOChange.nModuleID	=	m_stpModCfg->nModuleID;
//	sig.unionData.stECOChange.nEOMD		=	m_ShCommData_Master[eP2C_ECOModeChange]->m_stECOData.nEOMD;
//	sig.unionData.stECOChange.nEOV		=	m_ShCommData_Master[eP2C_ECOModeChange]->m_stECOData.nEOV;
//
//	m_pParent->SendSignalToSCH(&sig);
//	//////////////////////////////////////////////////////////////////////////
//
//
//	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nReplyBitAddr);
//
//	nElaspedTime = m_ShCommData_Master[eP2C_ECOModeChange]->m_TimeCheck.GetTimerAfterStart();
//	m_ShCommData_Master[eP2C_ECOModeChange]->m_nElaspedTime = nElaspedTime;
//	
//	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECO EOMD(%d) ECO EOV(%d) Event Complete!![Elasped Time : %dms]", 
//		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, m_ShCommData_Master[eP2C_ECOModeChange]->m_stECOData.nEOMD,	m_ShCommData_Master[eP2C_ECOModeChange]->m_stECOData.nEOV, nElaspedTime);
//		
//	m_ShCommData_Master[eP2C_ECOModeChange]->m_bWaitTime = FALSE;
//	
//}


/////////////////////////////////////////

void CBaseModule::GetMasterDevBWAddr(short eContent,long nModuleSel, short shBitAddr, short shDataAddr)
{
	
	if(m_stpSma->stLayOutCfg.nEQType == eEQType_ETCHSTRIP)
	{
		switch(nModuleSel)
		{
		case eModuleType_Etcher:
			shBitAddr  += B_CIM_EACH_LAYER_INTERVAL * 0;	
			shDataAddr += W_CIM_EACH_LAYER_INTERVAL * 0;
			break;
		case eModuleType_Stripper:
			shBitAddr  += B_CIM_EACH_LAYER_INTERVAL * 1;
			shDataAddr += W_CIM_EACH_LAYER_INTERVAL * 1;
			break;
		}
	}
	else if(m_stpSma->stLayOutCfg.nEQType == eEQType_ETCH)
	{
		switch(nModuleSel)
		{
		case eModuleType_Etcher:
			shBitAddr  += B_CIM_EACH_LAYER_INTERVAL * 0;	
			shDataAddr += W_CIM_EACH_LAYER_INTERVAL * 0;
			break;
		}
	}
	else if(m_stpSma->stLayOutCfg.nEQType == eEQType_STRIP)
	{
		switch(nModuleSel)
		{
		case eModuleType_Stripper:
			shBitAddr  += B_CIM_EACH_LAYER_INTERVAL * 1;	
			shDataAddr += W_CIM_EACH_LAYER_INTERVAL * 1;
			break;
		}
	}
	else if(m_stpSma->stLayOutCfg.nEQType == eEQType_PFC)
	{
		switch(nModuleSel)
		{
		case eModuleType_PFC:
			shBitAddr  += B_CIM_EACH_LAYER_INTERVAL * 0;	
			shDataAddr += W_CIM_EACH_LAYER_INTERVAL * 0;
			break;
			
			// SD03호기는 입구가 두개 PSK 080205
		case eModuleType_EX:	//3
			shBitAddr  += B_CIM_EACH_LAYER_INTERVAL * 1;	
			shDataAddr += W_CIM_EACH_LAYER_INTERVAL * 1;
			break;
		}
	}
	else if(m_stpSma->stLayOutCfg.nEQType == eEQType_EDGE)
	{
		switch(nModuleSel)
		{
		case eModuleType_Edge:
			shBitAddr  += B_CIM_EACH_LAYER_INTERVAL * 0;	
			shDataAddr += W_CIM_EACH_LAYER_INTERVAL * 0;
			break;
		}
	}
	
	else if(m_stpSma->stLayOutCfg.nEQType == eEQType_WRU)
	{
		switch(nModuleSel)
		{
		case eModuleType_WRU:
			shBitAddr  += B_CIM_EACH_LAYER_INTERVAL * 0;	
			shDataAddr += W_CIM_EACH_LAYER_INTERVAL * 0;
			break;
		}
	}	

	else if(m_stpSma->stLayOutCfg.nEQType == eEQType_LCPI)
	{
		switch(nModuleSel)
		{
		case eModuleType_LCPI:
			shBitAddr  += B_CIM_EACH_LAYER_INTERVAL * 0;	
			shDataAddr += W_CIM_EACH_LAYER_INTERVAL * 0;
			break;
			
		}
	}

	else if(m_stpSma->stLayOutCfg.nEQType == eEQType_LCODF)
	{
		switch(nModuleSel)
		{
		case eModuleType_LCODF:
			shBitAddr  += B_CIM_EACH_LAYER_INTERVAL * 0;	
			shDataAddr += W_CIM_EACH_LAYER_INTERVAL * 0;
			break;
			
		}
	}

	else if(m_stpSma->stLayOutCfg.nEQType == eEQType_LCRW )
	{
		switch(nModuleSel)
		{
		case eModuleType_LCRW:
			shBitAddr  += B_CIM_EACH_LAYER_INTERVAL * 0;	
			shDataAddr += W_CIM_EACH_LAYER_INTERVAL * 0;
			break;
			
		}
	}
	m_ShCommData_Master[eContent]->m_shBitAddr  = shBitAddr;
	m_ShCommData_Master[eContent]->m_shDataAddr = shDataAddr;
}

void CBaseModule::GetLocalDevBWAddr(short eContent,long nModuleSel, short shBitAddr, short shDataAddr)
{
	
	if(m_stpSma->stLayOutCfg.nEQType == eEQType_ETCHSTRIP)
	{
		switch(nModuleSel)
		{
		case eModuleType_Etcher:
			shBitAddr  += B_EQ_EACH_LAYER_INTERVAL * 0;	
			shDataAddr += W_EQ_EACH_LAYER_INTERVAL * 0;
			break;
		case eModuleType_Stripper:
			shBitAddr  += B_EQ_EACH_LAYER_INTERVAL * 1;
			shDataAddr += W_EQ_EACH_LAYER_INTERVAL * 1;
			break;
		}
	}
	else if(m_stpSma->stLayOutCfg.nEQType == eEQType_ETCH)
	{
		switch(nModuleSel)
		{
		case eModuleType_Etcher:
			shBitAddr  += B_EQ_EACH_LAYER_INTERVAL * 0;	
			shDataAddr += W_EQ_EACH_LAYER_INTERVAL * 0;
			break;
		}
	}
	else if(m_stpSma->stLayOutCfg.nEQType == eEQType_STRIP)
	{
		switch(nModuleSel)
		{
		case eModuleType_Stripper:
			shBitAddr  += B_EQ_EACH_LAYER_INTERVAL * 1;	
			shDataAddr += W_EQ_EACH_LAYER_INTERVAL * 1;
			break;
		}
	}
	else if(m_stpSma->stLayOutCfg.nEQType == eEQType_PFC)
	{
		switch(nModuleSel)
		{
		case eModuleType_PFC:
			shBitAddr  += B_EQ_EACH_LAYER_INTERVAL * 0;
			shDataAddr += W_EQ_EACH_LAYER_INTERVAL * 0;
			break;
			
			// SD03호기는 입구가 두개 PSK 080205
		case eModuleType_EX:	//3
			shBitAddr  += B_EQ_EACH_LAYER_INTERVAL * 1;
			shDataAddr += W_EQ_EACH_LAYER_INTERVAL * 1;
			break;
		}
	}
	else if(m_stpSma->stLayOutCfg.nEQType == eEQType_EDGE)
	{
		switch(nModuleSel)
		{
		case eModuleType_Edge:
			shBitAddr  += B_EQ_EACH_LAYER_INTERVAL * 0;	
			shDataAddr += W_EQ_EACH_LAYER_INTERVAL * 0;
			break;
		}
	}
	else if(m_stpSma->stLayOutCfg.nEQType == eEQType_WRU)
	{
		switch(nModuleSel)
		{
		case eModuleType_WRU:
			shBitAddr  += B_EQ_EACH_LAYER_INTERVAL * 0;	
			shDataAddr += W_EQ_EACH_LAYER_INTERVAL * 0;
			break;
		}
	}

	else if(m_stpSma->stLayOutCfg.nEQType == eEQType_LCPI)
	{
		switch(nModuleSel)
		{
		case eModuleType_LCPI:
			shBitAddr  += B_EQ_EACH_LAYER_INTERVAL * 0;
			shDataAddr += W_EQ_EACH_LAYER_INTERVAL * 0;
			break;

		}
	}

	else if(m_stpSma->stLayOutCfg.nEQType == eEQType_LCODF)
	{
		switch(nModuleSel)
		{
		case eModuleType_LCODF:
			shBitAddr  += B_EQ_EACH_LAYER_INTERVAL * 0;
			shDataAddr += W_EQ_EACH_LAYER_INTERVAL * 0;
			break;
			
		}
	}

	else if(m_stpSma->stLayOutCfg.nEQType == eEQType_LCRW)
	{
		switch(nModuleSel)
		{
		case eModuleType_LCRW:
			shBitAddr  += B_EQ_EACH_LAYER_INTERVAL * 0;
			shDataAddr += W_EQ_EACH_LAYER_INTERVAL * 0;
			break;
			
		}
	}

	m_ShCommData_Local[eContent]->m_shBitAddr  = shBitAddr;
	m_ShCommData_Local[eContent]->m_shDataAddr = shDataAddr;
}

long CBaseModule::GetPosStartAddr(long nModuleSel)
{
	long nStartAddr = 0;
	
	switch(nModuleSel)
	{
	case eModuleType_Cleaner:		// 공용	(Etch/PFC/EDGE/WRU)			
		nStartAddr = ADDR_OFFSET_CLN; 
		break;
	case eModuleType_DBK:			// 공용 (Strip)			
		nStartAddr = ADDR_OFFSET_DBK; 
		break;
	case eModuleType_Coater:					
		nStartAddr = ADDR_OFFSET_COT; 
		break;
	case eModuleType_VCD:						
		nStartAddr = ADDR_OFFSET_VCD; 
		break;
	case eModuleType_SBK:	
		nStartAddr = ADDR_OFFSET_SBK; 
		break;
	case eModuleType_Interface:		
		nStartAddr = ADDR_OFFSET_INF;
		break;
	case eModuleType_PEB:				
		nStartAddr = ADDR_OFFSET_PEB;
		break;
	case eModuleType_Developer:			
		nStartAddr = ADDR_OFFSET_DEV;
		break;
	case eModuleType_PBK:	
		nStartAddr = ADDR_OFFSET_PBK;
		break;
	}
	
	return nStartAddr;
}


void CBaseModule::P2C_GlassSendFailSequence()
{
	long	nStep			=	0;
	long	nStepState		=	STEP_START;	
	long	nTimeout		=	0;	
	long	nDataNo			=	0;
	long	nPPID			=	0;
	long	nDataSize_1		=	sizeof(short) * 1;
	long	nDataSize_6		=	sizeof(short) * 6;
	long	nRlyDataSize	=	sizeof(short);
	short	shAckCode		=	0;
	short   shLocalDataAddr	=   0;
	short	sFailCode[2]		=	{ 0x00, };
	short	sHPanelID[6]		=	{ 0x00, };

	char	szHPANELID[MAX_PANEL_ID_LEN] =	{ NULL, };
	
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	GetMasterDevBWAddr(eP2C_GlassSendFail, m_stpModCfg->nModuleID, B_L2_TO_GLASS_SEND_FAIL_RLY, 0);
	
	long	i	 = 0;
	long	nCnt = 0;
	
	// Event 진행 전 Reply Bit Off. (Reply Bit Off 안된 경우 대비)
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_GlassSendFail]->m_shBitAddr);

	while(nStep != -1) 
	{
		switch(nStep++)
		{
		case 0:
			if(m_stpModRunInfo->stEQCmdRlyEvtIO.bGlassSendFail == FALSE)
			{
				m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_GlassSendFail]->m_shBitAddr);
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Glass Send Fail Event is Canceled!!", 
					m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
				nStep = -1;
				nStepState = STEP_NG;
			}
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Glass Send Fail Event Start!! : Request On", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
			break;

		case 1: // Event Data Read
			GetLocalDevBWAddr(eP2C_GlassSendFail, m_stpModCfg->nModuleID, 0, W_L2_GLASS_SEND_FAIL_CODE);
			shLocalDataAddr = m_ShCommData_Local[eP2C_GlassSendFail]->m_shDataAddr;
			
			m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, shLocalDataAddr, &nDataSize_1, sFailCode);
			m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, W_L2_GLASS_SEND_FAIL_HPANELID, &nDataSize_6, sHPanelID);
			
			m_stpModRunInfo->stGlassSendFail.nGlassSendFailCode = (long)sFailCode[0];
			memcpy(m_stpModRunInfo->stGlassSendFail.szH_PanelID, sHPanelID, MAX_PANEL_ID_LEN);

			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Glass Send Fail Event : FailCode[%d] H_PANELID[%s] Readed!!",
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, m_stpModRunInfo->stGlassSendFail.nGlassSendFailCode, m_stpModRunInfo->stGlassSendFail.szH_PanelID);
			break;		
			
		case 2: // Ack/Nak Write & Reply Bit On
		
			m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, B_L2_TO_GLASS_SEND_FAIL_RLY);
			
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Glass Send Fail Reply Bit == TRUE", 
																m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
			m_ShCommData_Master[eP2C_GlassSendFail]->m_shSetData = 0x41;
			break;
			
		case 4:
			m_ShCommData_Master[eP2C_GlassSendFail]->m_bWaitTime = TRUE;
			m_ShCommData_Master[eP2C_GlassSendFail]->m_TimeCheck.StartTimer();
			nStep = -1;
			if(nStepState != STEP_NG) 
				nStepState = STEP_OK;
			break;
		}
	}
}

void CBaseModule::P2C_GlassSendFailSequenceEnd()
{
	long	nDataSize		=	sizeof(short);
	short 	shAckCode		=	0;
	long    nItemVal		=	0;
	ULONG	nElaspedTime	=	0;
	
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	if(m_stpModRunInfo->stEQCmdRlyEvtIO.bGlassSendFail == TRUE)
	{
		if(m_ShCommData_Master[eP2C_GlassSendFail]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			nElaspedTime = m_ShCommData_Master[eP2C_GlassSendFail]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eP2C_GlassSendFail]->m_nElaspedTime = nElaspedTime;
			
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_GlassSendFail]->m_shBitAddr);
			
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Glass (P%02d) Send Fail Event Time Over!![Elasped Time : %dms]", 
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, m_ShCommData_Master[eP2C_GlassSendFail]->m_nCmdData, nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Glass (P%02d) Send Fail Event Time Over!![Elasped Time : %dms]", 
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, m_ShCommData_Master[eP2C_GlassSendFail]->m_nCmdData, nElaspedTime);
			
			m_ShCommData_Master[eP2C_GlassSendFail]->m_bWaitTime = FALSE;
		}
		return;
	}
	
	// Host Report Send
	if(m_ShCommData_Master[eP2C_GlassSendFail]->m_shSetData == 0x41)
	{
		nItemVal = m_stpModRunInfo->stGlassSendFail.nGlassSendFailCode;

		sig.nSignal = sigEQSpecCtrlEvent;
		sig.nFrom	= PLC_TASK_ID;
		sig.nTo		= SCH_TASK_ID;
		sig.unionData.stEQSpecCtrlEventData.nEventId	= eSpecGlassSendFail;
		sig.unionData.stEQSpecCtrlEventData.nItemId	    = 0;
		sig.unionData.stEQSpecCtrlEventData.nItemVal	= nItemVal; // <1. Duplication, 2: Ommission, 3: Availability>
		sig.unionData.stEQSpecCtrlEventData.nModuleNo	= m_stpModCfg->nModuleID;

		switch(nItemVal)
		{
		case eFailCode_Duplication:  
			memcpy(sig.unionData.stEQSpecCtrlEventData.szItemValue, 
				m_stpModRunInfo->stGlassSendFail.szH_PanelID, MAX_SPEC_CONTROL_DATA_VALUE_LEN);
			break;
		case eFailCode_Omission:
			memcpy(sig.unionData.stEQSpecCtrlEventData.szItemValue, 
				m_stpModRunInfo->stGlassSendFail.szH_PanelID, MAX_SPEC_CONTROL_DATA_VALUE_LEN);
			break;
		case eFailCode_Availability: 
			memcpy(sig.unionData.stEQSpecCtrlEventData.szItemValue,
				m_stpModRunInfo->stGlassSendFail.szH_PanelID, MAX_SPEC_CONTROL_DATA_VALUE_LEN);
			break;
		}

		m_pParent->SendSignalToSCH(&sig);
	}
	
	// Reply Bit Off
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_GlassSendFail]->m_shBitAddr);
	
	nElaspedTime = m_ShCommData_Master[eP2C_GlassSendFail]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[eP2C_GlassSendFail]->m_nElaspedTime = nElaspedTime;
	
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Glass ( P%02d) Send Fail Event Complete!![Elasped Time : %dms]",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, m_ShCommData_Master[eP2C_GlassSendFail]->m_nCmdData, nElaspedTime);

	m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] Glass ( P%02d) Send Fail Event Complete!![Elasped Time : %dms]",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, m_ShCommData_Master[eP2C_GlassSendFail]->m_nCmdData, nElaspedTime);

	m_ShCommData_Master[eP2C_GlassSendFail]->m_bWaitTime = FALSE;
}

void CBaseModule::P2C_GlassScrapSequence()
{
	short   shData[MAX_TOTAL_GLASS_DATA_SIZE]	=	{ 0x00, };
	long	nReadDataSize  = sizeof(short) * MAX_TOTAL_GLASS_DATA_SIZE;
	long	nCmdDataSize	= sizeof(short);
	long	nMasterBitAddr	= 0;
	short	shMasterDataAddr= 0;
	short	shLocalDataAddr	= 0;
	short	shAckCode		= 0x4E;
    long	nStep			= 0;
	long	nStepState		= STEP_START;	
	long	i				= 0;
	long	j				= 0;
	long	k				= 0;
	long	nStationNo		= 0;
	long	nLen			= 0;
	
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	nStationNo = m_stpModCfg->nModuleID;
	
	GetMasterDevBWAddr(eP2C_GlassScrap, nStationNo, B_L2_TO_CIM_GLASS_SCRAP_RLY, W_L2_FROM_CIM_SCRAP_JUDGE_RLY_DATA);	

	// Event 진행 전 Reply Bit Off. (Reply Bit Off 안된 경우 대비)
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_GlassScrap]->m_shBitAddr);
	
	while(nStep != -1) 
	{
		switch(nStep++)
		{
		case 0:
			if(m_stpModRunInfo->stEQCmdRlyEvtIO.bGlassScrap == FALSE)
			{
				m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_GlassScrap]->m_shBitAddr);
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Glass Scrap Request Bit be Canceled", 
					m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
				nStep = -1;
				nStepState = STEP_NG;
			}
			break;			
			
		case 1: // Event Data Read
			GetLocalDevBWAddr(eP2C_GlassScrap, nStationNo, 0, W_L2_GLS_TRANS_DATA_TO_MASTER);	
			shLocalDataAddr = m_ShCommData_Local[eP2C_GlassScrap]->m_shDataAddr;
			
			m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, shLocalDataAddr, &nReadDataSize, shData);
			
			ConvertTransferDataToPanelData(&m_stpModRunInfo->stGlassJudgeScrapData, &shData[0]);
			
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Glass Scrap Event :  ScrapData[Glass ID: %s] Readed!!",
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, m_stpModRunInfo->stGlassJudgeScrapData.szHPanelID);
			break;
			
		case 2: // Ack/Nak Check
			m_ShCommData_Master[eP2C_GlassScrap]->m_nModuleID = m_stpModCfg->nModuleID;
			if(shAckCode != 0x41)
			{
				for(k=0; k < MAX_LAYER1_MODULE_COUNT-2; k++)
				{
					for(j=0; j<MAX_LAYER2_MODULE_COUNT; j++)
					{
						if(m_stpModRunInfo->stGlassJudgeScrapData.nOwnGlassNo < 1 || m_stpModRunInfo->stGlassJudgeScrapData.nOwnGlassNo > 255)
							continue;
						
						if(m_stpModRunInfo->stGlassJudgeScrapData.nOwnGlassNo == m_stpSma->stSysRunInfo.stModRunInfo[k+2].stUnitInfo[j].nOwnGlassNo)
						{
							shAckCode = 0x41;		//Ack
							m_pParent->m_Log[m_stpSma->stLayOutCfg.stModCfg[k+2].nModuleID].AddLog("ModuleID[%02d(%s)] Glass Scrap Event : Own Glass No [%d], Unique ID [%d, %d, %d, %d]", 
								k+2,  
								m_stpSma->stLayOutCfg.stModCfg[k+2].szModuleName,
								m_stpModRunInfo->stGlassJudgeScrapData.nOwnGlassNo,
								m_stpModRunInfo->stGlassJudgeScrapData.nUniqueID[UNIQUEID],    // 0
								m_stpModRunInfo->stGlassJudgeScrapData.nUniqueID[JOBORDER],    // 1
								m_stpModRunInfo->stGlassJudgeScrapData.nUniqueID[SLOTNO  ],    // 2
								m_stpModRunInfo->stGlassJudgeScrapData.nUniqueID[PORTNO  ] );  // 3
							
							m_ShCommData_Master[eP2C_GlassScrap]->m_nCmdData = j;
							m_ShCommData_Master[eP2C_GlassScrap]->m_nModuleID = k+2;
							break;
						}
					}
				}
			}
			
			// Scrap시 Space 체크 
			if(!strncmp(m_stpModRunInfo->stGlassJudgeScrapData.szCode, " ", 1) || m_stpModRunInfo->stGlassJudgeScrapData.szCode[0] == 0x00)
				shAckCode = 0x4E;
			break;
			
		case 3: // Ack/Nak Write & Reply Bit On
			nMasterBitAddr = m_ShCommData_Master[eP2C_GlassScrap]->m_shBitAddr;
			shMasterDataAddr = m_ShCommData_Master[eP2C_GlassScrap]->m_shDataAddr;
			
			if(shAckCode == 0x41)
			{
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Glass Scrap Reply : [W %d] Ack, Scrap Code[%s]", 
					m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, shMasterDataAddr, m_stpModRunInfo->stGlassJudgeScrapData.szCode);
			}
			else
			{
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Glass Scrap Reply : [W %d] Nak, Scrap Code[%s]",
					m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, shMasterDataAddr, m_stpModRunInfo->stGlassJudgeScrapData.szCode);
			}
			
			m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, shMasterDataAddr, &nCmdDataSize, &shAckCode);
			m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, nMasterBitAddr);
			
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Glass Scrap Reply Bit == TRUE", 
																m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
			m_ShCommData_Master[eP2C_GlassScrap]->m_shSetData = shAckCode;
			break;
			
		case 4:
			m_ShCommData_Master[eP2C_GlassScrap]->m_bWaitTime = TRUE;
			m_ShCommData_Master[eP2C_GlassScrap]->m_TimeCheck.StartTimer();
			nStep = -1;
			if(nStepState != STEP_NG) 
				nStepState = STEP_OK;
			break;
		}
	}
	
	if(nStepState == STEP_NG) 
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Glass Scrap Event",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
}

void CBaseModule::P2C_GlassScrapSequenceEnd()
{
	long	nDataSize		=	sizeof(short);
	short 	shAckCode		=	0;
	ULONG	nElaspedTime	=	0;
	
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	if(m_stpModRunInfo->stEQCmdRlyEvtIO.bGlassScrap == TRUE)
	{
		if(m_ShCommData_Master[eP2C_GlassScrap]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			nElaspedTime = m_ShCommData_Master[eP2C_GlassScrap]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eP2C_GlassScrap]->m_nElaspedTime = nElaspedTime;
			
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_GlassScrap]->m_shBitAddr);
			m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, m_ShCommData_Master[eP2C_GlassScrap]->m_shDataAddr, &nDataSize, &shAckCode);
			
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Glass (P%02d) Scrap Event Time Over!![Elasped Time : %dms]", 
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, m_ShCommData_Master[eP2C_GlassScrap]->m_nCmdData, nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Glass (P%02d) Scrap Event Time Over!![Elasped Time : %dms]", 
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, m_ShCommData_Master[eP2C_GlassScrap]->m_nCmdData, nElaspedTime);
			
			m_ShCommData_Master[eP2C_GlassScrap]->m_bWaitTime = FALSE;
		}
		return;
	}
	
	// Host Report Send
	if(m_ShCommData_Master[eP2C_GlassScrap]->m_shSetData == 0x41)
	{
		sig.nSignal = sigGlassDelete;
		sig.nFrom	= PLC_TASK_ID;
		sig.nTo		= GUI_TASK_ID;
		memcpy(&sig.unionData.stGlassScrapData.stPanelInfo, &m_stpModRunInfo->stGlassJudgeScrapData, sizeof(stPanelInfoType));
		sig.unionData.stGlassScrapData.nUniqueID[UNIQUEID] = m_stpModRunInfo->stGlassJudgeScrapData.nUniqueID[UNIQUEID];  // 0	
		sig.unionData.stGlassScrapData.nUniqueID[JOBORDER] = m_stpModRunInfo->stGlassJudgeScrapData.nUniqueID[JOBORDER];  // 1
		sig.unionData.stGlassScrapData.nUniqueID[SLOTNO  ] = m_stpModRunInfo->stGlassJudgeScrapData.nUniqueID[SLOTNO  ];  // 2
		sig.unionData.stGlassScrapData.nUniqueID[PORTNO  ] = m_stpModRunInfo->stGlassJudgeScrapData.nUniqueID[PORTNO  ];  // 3
		sig.unionData.stGlassScrapData.nModuleID = m_stpModCfg->nModuleID;	//m_ShCommData_Master[eP2C_GlassScrap]->m_nModuleID;
		m_pParent->SendSignalToSCH(&sig);
	}
	
	// Reply Bit Off
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_GlassScrap]->m_shBitAddr);
	
	m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, m_ShCommData_Master[eP2C_GlassScrap]->m_shDataAddr, &nDataSize, &shAckCode);
	m_pParent->m_stpSma->stSysRunInfo.stScrapData.shAckCode = 0;
	
	nElaspedTime = m_ShCommData_Master[eP2C_GlassScrap]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[eP2C_GlassScrap]->m_nElaspedTime = nElaspedTime;
	
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Glass ( P%02d) Scrap Event Complete!![Elasped Time : %dms]",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, m_ShCommData_Master[eP2C_GlassScrap]->m_nCmdData, nElaspedTime);
	m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] Glass ( P%02d) Scrap Event Complete!![Elasped Time : %dms]",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, m_ShCommData_Master[eP2C_GlassScrap]->m_nCmdData, nElaspedTime);
	m_ShCommData_Master[eP2C_GlassScrap]->m_bWaitTime = FALSE;
}

void CBaseModule::P2C_GlassUnscrapSequence()
{
	short	sTransData[MAX_TOTAL_GLASS_DATA_SIZE]	=	{ 0x00, };
	short	shUniqID[2]		=	{ 0x00, };
	short   shPanelData[6]	=	{ 0x00, };
	long	nGlsDataSize	=	sizeof(short) * MAX_TOTAL_GLASS_DATA_SIZE;
	long	nDataSize		=	sizeof(short) * 2;
	long	nCmdDataSize	=	sizeof(short);
	short   shPanelDataSize	=	sizeof(short) * 6;
	short	shAckCode		=	0x4E;
	long	nStep			=	0;
	long	nStepState		=	STEP_START;
	long	i				=	0;
	long	nStationNo		=	0;

	nStationNo = m_stpModCfg->nModuleID;
	
	GetMasterDevBWAddr(eP2C_GlassUnscrap, nStationNo, B_L2_TO_CIM_GLASS_UNSCRAP_RLY, W_L2_FROM_CIM_UNSCRAP_RLY_DATA);
	GetLocalDevBWAddr(eP2C_GlassUnscrap, nStationNo, 0, W_L2_UNSCRAP_UNIQID);
	
	// Event 진행 전 Reply Bit Off. (Reply Bit Off 안된 경우 대비)
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_GlassUnscrap]->m_shBitAddr);

	while(nStep != -1)
	{
		switch(nStep++)
		{
		case 0:
			if(m_stpModRunInfo->stEQCmdRlyEvtIO.bGlassUnscrap == FALSE)
			{
				m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_GlassUnscrap]->m_shBitAddr);
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Glass Unscrap Request Bit be Canceled", 
					m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
				nStep = -1;
				nStepState = STEP_NG;
			}				
			break;
			
		case 1:	// Event Data Read
			// Read Unique ID
			m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, m_ShCommData_Local[eP2C_GlassUnscrap]->m_shDataAddr, &nDataSize, shUniqID);
			m_ShCommData_Master[eP2C_GlassUnscrap]->m_nOwnGlassNo = (long) shUniqID[0];
			
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Glass Unscrap Event : [W %d] Own Glass No [%d] Readed!!", 
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, 
				m_ShCommData_Local[eP2C_GlassUnscrap]->m_shDataAddr, 
				m_ShCommData_Master[eP2C_GlassUnscrap]->m_nOwnGlassNo);
			break;
			
		case 2: // Glass Data Detect & Set
			if(m_ShCommData_Master[eP2C_GlassUnscrap]->m_nOwnGlassNo > 0)
			{
				for(i=0; i<MAX_UNIQID_COUNT; i++)
				{
					// RUNDATA 파일에서 Glass Data 저장.
					GetGlassDataFromRUNDATA(&m_stpSma->stSysRunInfo.stUniqueIDTable[i], m_ShCommData_Master[eP2C_GlassUnscrap]->m_nOwnGlassNo);
					
					if(m_ShCommData_Master[eP2C_GlassUnscrap]->m_nOwnGlassNo == m_stpSma->stSysRunInfo.stUniqueIDTable[i].nOwnGlassNo)
					{						
						if(&m_stpSma->stSysRunInfo.stUniqueIDTable[i].stUniqueIDPanelInfo == NULL)
						{
							m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Glass Unscrap Event : UnscrapData is NULL!!", 
								m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
							break;
						}
						
						ConvertPanelDataToTransferData(&m_stpSma->stSysRunInfo.stUniqueIDTable[i].stUniqueIDPanelInfo, sTransData);
						m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, W_L1_UNSCRAP_RLY_DATA, &nGlsDataSize, sTransData);
						
						shAckCode = 0x41;
						m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Glass Unscrap Event : Start Word [W %d] UnscrapData[Glass ID: %s] Writed!!", 
							m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, W_L1_UNSCRAP_RLY_DATA,
							m_stpSma->stSysRunInfo.stUniqueIDTable[i].stUniqueIDPanelInfo.szHPanelID);
						break;
					}
				}
			}
			
			if(shAckCode == 0x41)
			{
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Glass Unscrap Reply : [W %d] Ack", 
					m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, m_ShCommData_Master[eP2C_GlassUnscrap]->m_shDataAddr );
			}
			else
			{
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Glass Unscrap Reply : [W %d] Nak", 
					m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, m_ShCommData_Master[eP2C_GlassUnscrap]->m_shDataAddr);
			}
			
			m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, m_ShCommData_Master[eP2C_GlassUnscrap]->m_shDataAddr, &nCmdDataSize, &shAckCode);
			m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, m_ShCommData_Master[eP2C_GlassUnscrap]->m_shBitAddr);
			
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Glass Unscrap Reply Bit == TRUE", 
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
			
			m_ShCommData_Master[eP2C_GlassUnscrap]->m_shSetData = shAckCode;
			break;
			
		case 3:
			m_ShCommData_Master[eP2C_GlassUnscrap]->m_bWaitTime = TRUE;
			m_ShCommData_Master[eP2C_GlassUnscrap]->m_TimeCheck.StartTimer();
			nStep = -1;
			if(nStepState != STEP_NG)
				nStepState = STEP_OK;
			break;
		}
	}
	
	if(nStepState == STEP_NG) 
	{
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Glass Unscrap Event", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
	}
}

void CBaseModule::P2C_GlassUnscrapSequenceEnd()
{
	long i = 0;
	long j = 0;
	long nNewEtchCnt		= 0;
	long nNewBuffCnt		= 0;
	long nNewBypassCnt		= 0;
	long	nDataSize		=	sizeof(short);
	short 	shAckCode		= 0;
	ULONG	nElaspedTime	= 0;	
	
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	if(m_stpModRunInfo->stEQCmdRlyEvtIO.bGlassUnscrap == TRUE)
	{
		if(m_ShCommData_Master[eP2C_GlassUnscrap]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			nElaspedTime = m_ShCommData_Master[eP2C_GlassUnscrap]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eP2C_GlassUnscrap]->m_nElaspedTime = nElaspedTime;
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_GlassUnscrap]->m_shBitAddr);
			
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Glass Unscrap Event Time Over!![Elasped Time : %dms]",
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Glass Unscrap Event Time Over!![Elasped Time : %dms]",
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			
			m_ShCommData_Master[eP2C_GlassUnscrap]->m_bWaitTime = FALSE;
		}
		return;
	}
	
	// Host Report Send
	if(m_ShCommData_Master[eP2C_GlassUnscrap]->m_shSetData == 0x41)
	{
		sig.nSignal = sigGlassMake;
		sig.nFrom	= PLC_TASK_ID;
		sig.nTo		= GUI_TASK_ID;
		sig.unionData.stGlassUnscrapData.nModuleID = m_stpModCfg->nModuleID;
		sig.unionData.stGlassUnscrapData.nOwnGlassNo = m_ShCommData_Master[eP2C_GlassUnscrap]->m_nOwnGlassNo;
		//sig.unionData.stGlassUnscrapData.nUniqueID[0] = m_ShCommData_Master[eP2C_GlassUnscrap]->m_nOwnGlassNo; // m_nUniquID[0];
		//sig.unionData.stGlassUnscrapData.nUniqueID[1] = m_ShCommData_Master[eP2C_GlassUnscrap]->m_nUniquID[1];
		//sig.unionData.stGlassUnscrapData.nUniqueID[2] = m_ShCommData_Master[eP2C_GlassUnscrap]->m_nUniquID[2];
		//sig.unionData.stGlassUnscrapData.nUniqueID[3] = m_ShCommData_Master[eP2C_GlassUnscrap]->m_nUniquID[3];
		m_pParent->SendSignalToSCH(&sig);
	}
	
	// Reply Bit Off
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_GlassUnscrap]->m_shBitAddr);
	
	m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, m_ShCommData_Master[eP2C_GlassUnscrap]->m_shDataAddr, &nDataSize, &shAckCode);
	m_pParent->m_stpSma->stSysRunInfo.stUnscrapData.shAckCode = 0;
	
	nElaspedTime = m_ShCommData_Master[eP2C_GlassUnscrap]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[eP2C_GlassUnscrap]->m_nElaspedTime = nElaspedTime;
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Glass Unscrap Event Complete!![Elasped Time : %dms]",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] Glass Unscrap Event Complete!![Elasped Time : %dms]",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	
	m_ShCommData_Master[eP2C_GlassUnscrap]->m_bWaitTime = FALSE;
}

void CBaseModule::P2C_GlassJudgementSequence()
{
	short   shData[MAX_TOTAL_GLASS_DATA_SIZE]		=	{ 0x00, };
	long	nReadDataSize  = sizeof(short) * MAX_TOTAL_GLASS_DATA_SIZE;
	long	nStep			= 0;
	long	nStepState		= STEP_START;	
	short	shAckCode		= 0x4E;
	long	nDataSize		= sizeof(short);
	long    nGlassNo		= 0;
	long	nJobOrder		= 0;
	long	nPortNo			= 0;
	long	nSlotNo			= 0;
	
	GetMasterDevBWAddr(eP2C_GlassJudgement, m_stpModCfg->nModuleID, B_L2_TO_CIM_GLASS_JUDGE_RLY, W_L2_FROM_CIM_SCRAP_JUDGE_RLY_DATA);	
	GetLocalDevBWAddr(eP2C_GlassJudgement, m_stpModCfg->nModuleID, 0, W_L2_GLS_TRANS_DATA_TO_MASTER);	
	
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	sig.nSignal = sigJudgementEvent;
	sig.nFrom	= PLC_TASK_ID;
	sig.nTo		= GUI_TASK_ID;

	// Event 진행 전 Reply Bit Off. (Reply Bit Off 안된 경우 대비)
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_GlassJudgement]->m_shBitAddr);
	
	while(nStep != -1) 
	{
		switch(nStep++)
		{
		case 0:
			if(m_stpModRunInfo->stEQCmdRlyEvtIO.bGlassJudgement == FALSE)
			{
				m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_GlassJudgement]->m_shBitAddr);
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Glass Judgement Request Bit be Canceled", 
					m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
				nStep=-1;
				nStepState = STEP_NG;
				break;
			}				
			break;
			
		case 1:	// Event Data Read
			m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, m_ShCommData_Local[eP2C_GlassJudgement]->m_shDataAddr, &nReadDataSize, shData);
			ConvertTransferDataToPanelData(&m_stpModRunInfo->stGlassJudgeScrapData, &shData[0]);
			
			if(&m_stpModRunInfo->stGlassJudgeScrapData == NULL) 
			{
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("JudgeMent Data Not Exist");
				shAckCode = 0x4E;
				m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, m_ShCommData_Master[eP2C_GlassJudgement]->m_shDataAddr, &nDataSize, &shAckCode);
				m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, m_ShCommData_Master[eP2C_GlassJudgement]->m_shBitAddr);
				break;
			}
			
			nGlassNo = m_stpModRunInfo->stGlassJudgeScrapData.nUniqueID[eUniqueID_GlassNo];
			nJobOrder = m_stpModRunInfo->stGlassJudgeScrapData.nUniqueID[eUniqueID_JobOrder];
			nPortNo = m_stpModRunInfo->stGlassJudgeScrapData.nUniqueID[eUniqueID_PortNo];
			nSlotNo = m_stpModRunInfo->stGlassJudgeScrapData.nUniqueID[eUniqueID_SlotNo];
			
			if((nPortNo > 0 && nPortNo <= MAX_PORT_COUNT) 
				&& (nSlotNo > 0 && nSlotNo <= MAX_SLOT_COUNT_PER_PORT ))
			{
				memcpy(&sig.unionData.stGlassScrapData.stPanelInfo, &m_stpModRunInfo->stGlassJudgeScrapData, sizeof(stPanelInfoType));
				
				sig.unionData.stGlassScrapData.nModuleID	= m_stpModCfg->nModuleID;
				sig.unionData.stGlassScrapData.nUniqueID[eUniqueID_GlassNo]     = nGlassNo;
				sig.unionData.stGlassScrapData.nUniqueID[eUniqueID_JobOrder]    = nJobOrder;
				sig.unionData.stGlassScrapData.nUniqueID[eUniqueID_SlotNo]		= nSlotNo;
				sig.unionData.stGlassScrapData.nUniqueID[eUniqueID_PortNo]		= nPortNo;
				m_pParent->SendSignalToSCH(&sig);
				shAckCode = 0x41;
			}
			m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, m_ShCommData_Master[eP2C_GlassJudgement]->m_shDataAddr, &nDataSize, &shAckCode);
			m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, m_ShCommData_Master[eP2C_GlassJudgement]->m_shBitAddr);
			break;
			
		case 3:
			m_ShCommData_Master[eP2C_GlassJudgement]->m_bWaitTime = TRUE;
			m_ShCommData_Master[eP2C_GlassJudgement]->m_TimeCheck.StartTimer();
			nStep = -1;
			
			if(nStepState != STEP_NG) 
				nStepState = STEP_OK;
			break;
		}
	}
	
	if(nStepState == STEP_NG) 
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Judgement Event",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
}

void CBaseModule::P2C_GlassJudgementSequenceEnd()
{
	long	nDataSize		=	sizeof(short);
	short 	shAckCode		= 0;
	ULONG	nElaspedTime	= 0;
	
	if(m_stpModRunInfo->stEQCmdRlyEvtIO.bGlassJudgement == TRUE)
	{
		if(m_ShCommData_Master[eP2C_GlassJudgement]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			nElaspedTime = m_ShCommData_Master[eP2C_GlassJudgement]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eP2C_GlassJudgement]->m_nElaspedTime = nElaspedTime;
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_GlassJudgement]->m_shBitAddr);
			
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Judgement Event Event Bit not off[Elasped Time : %dms]",
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Judgement Event Event Bit not off[Elasped Time : %dms]",
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			
			m_ShCommData_Master[eP2C_GlassJudgement]->m_bWaitTime = FALSE;
		}
		return;
	}
	
	// Reply Bit Off
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_GlassJudgement]->m_shBitAddr);
	shAckCode = 0;
	
	m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, m_ShCommData_Master[eP2C_GlassJudgement]->m_shDataAddr, &nDataSize, &shAckCode);
	m_pParent->m_stpSma->stSysRunInfo.stScrapData.shAckCode = 0;
	
	nElaspedTime = m_ShCommData_Master[eP2C_GlassJudgement]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[eP2C_GlassJudgement]->m_nElaspedTime = nElaspedTime;
	
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Judgement Event Complete!![Elasped Time : %dms]", 
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,nElaspedTime);
	m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] Judgement Event Complete!![Elasped Time : %dms]", 
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,nElaspedTime);
	
	m_ShCommData_Master[eP2C_GlassJudgement]->m_bWaitTime = FALSE;
}

// PPID Validation Check
// Recipe Download Event 전 PPID 유효성 Check
void CBaseModule::P2C_PPIDValidationSequence()
{
	long	nStep			=	0;
	long	nStepState		=	STEP_START;	
	long	nTimeout		=	0;	
	long	nDataNo			=	0;
	long	nPPID			=	0;
	long	nDataSize		=	sizeof(short) * 8;
	long	nRlyDataSize	=	sizeof(short);
	short	shAckCode		=	0;
	short	sPPID[8]		=	{ 0x00, };
	char	szPPID[MAX_PPID_LEN] =	{ NULL, };
	
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	GetMasterDevBWAddr(eP2C_PPIDCheck, m_stpModCfg->nModuleID, B_L2_TO_CIM_PPID_AVAIL_CHK, W_L2_FROM_CIM_PPID_AVAIL_CHK_RLY_DATA);
	GetLocalDevBWAddr(eP2C_PPIDCheck, m_stpModCfg->nModuleID, 0, W_L2_PROC_AVAIL_CHK_PPID);
					GetMasterDevBWAddr(eP2C_PPIDCheck, m_stpModCfg->nModuleID, B_L2_TO_CIM_PPID_AVAIL_CHK, W_L2_FROM_CIM_PPID_AVAIL_CHK_RLY_DATA);
				m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_PPIDCheck]->m_shBitAddr);
	long	i	 = 0;
	long	nCnt = 0;
	
	// Event 진행 전 Reply Bit Off. (Reply Bit Off 안된 경우 대비)
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_PPIDCheck]->m_shBitAddr);

	while(nStep != -1) 
	{
		switch(nStep++)
		{
		case 0:
			if(m_stpModRunInfo->stDataChangeRlyEvtIO.bPPIDValidationReq == FALSE)
			{
				m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_PPIDCheck]->m_shBitAddr);
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] PPID Validation Check Event is Canceled!!", 
					m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
				nStep = -1;
				nStepState = STEP_NG;
			}
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] PPID Validation Check Event Start!! : Request On", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
			break;
			
		case 1:	// Event Data Read
			m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, m_ShCommData_Local[eP2C_PPIDCheck]->m_shDataAddr, &nDataSize, sPPID);
			memcpy(szPPID, sPPID, MAX_PPID_LEN);
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] PPID Validation Check Event : Recieved PPID = %s", 
																m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, szPPID);
			sprintf(sig.unionData.s10f3FromHost.szText, "ModuleID[%02d(%s)] PPID Validation Check Event : Recieved PPID = %s", 
																m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, szPPID);
			break;
			
		case 2: // PPID Validation Check
			for(i = 0; i < MAX_PPID_COUNT; i++)
			{
				switch(m_stpSma->stLayOutCfg.nEQType)
				{
				case eEQType_ETCHSTRIP:
				case eEQType_ETCH:
				case eEQType_STRIP:
					if(strncmp(m_stpSma->stSysData.stMainRecipeTbl.stRcp[i].stWetRcp.stMainRecipe.szPPID, szPPID, MAX_PPID_LEN) == 0)
					{
						switch(m_stpModCfg->nModuleID)
						{
						case eModuleType_Etcher:
							nPPID = i + 1;
							shAckCode = (short)m_stpSma->stSysData.stMainRecipeTbl.stRcp[i].stWetRcp.stMainRecipe.nEtchRecipeNo;	
							break;
						case eModuleType_Stripper:
							nPPID = i + 1;
							shAckCode = (short)m_stpSma->stSysData.stMainRecipeTbl.stRcp[i].stWetRcp.stMainRecipe.nStripRecipeNo;
							break;
						}
					}
					break;

				case eEQType_PFC:
				case eEQType_WRU: // WRU - 2500 공통
					if(strncmp(m_stpSma->stSysData.stMainRecipeTbl.stRcp[i].stPFCRcp.stMainRecipe.szPPID, szPPID, MAX_PPID_LEN) == 0)
					{
						nPPID = i + 1;
						shAckCode = (short)m_stpSma->stSysData.stMainRecipeTbl.stRcp[i].stPFCRcp.stMainRecipe.nCLNRecipeNo;
					}
					break;
					
				case eEQType_EDGE:
					if(strncmp(m_stpSma->stSysData.stMainRecipeTbl.stRcp[i].stEdgeRcp.stMainRecipe.szPPID, szPPID, MAX_PPID_LEN) == 0)
					{
						nPPID = i + 1;
						shAckCode = (short)m_stpSma->stSysData.stMainRecipeTbl.stRcp[i].stEdgeRcp.stMainRecipe.nCLNRecipeNo;
					}
					break;

				case eEQType_LCPI:
				case eEQType_LCODF:
				case eEQType_LCRW:
					if(strncmp(m_stpSma->stSysData.stMainRecipeTbl.stRcp[i].stLCRcp.stMainRecipe.szPPID, szPPID, MAX_PPID_LEN) == 0)
					{
						nPPID = i + 1;
						shAckCode = (short)m_stpSma->stSysData.stMainRecipeTbl.stRcp[i].stLCRcp.stMainRecipe.nCLNRecipeNo;
					}
					break;
				
				default:
					m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] PPID Transfer Event: Undefined EQ Type [%d]", 
						m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, m_stpSma->stLayOutCfg.nEQType);
					break;
				}
			}
			
		case 3:	//	Reply Data Set
			if(shAckCode == 0)
			{	
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] PPID Validation Check : Nak", 
					m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
				
				sig.nFrom = PLC_TASK_ID;
				sig.nTo = GUI_TASK_ID;
				sig.nSignal = sigRecipeDownloadReq;
				m_pParent->SendSignalToSCH(&sig);
			}
			else
			{
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] PPID Validation Check : Ack [Receipe No:%d]", 
					m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,shAckCode);
			}
			
			m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, m_ShCommData_Master[eP2C_PPIDCheck]->m_shDataAddr, &nRlyDataSize, &shAckCode);
			m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, m_ShCommData_Master[eP2C_PPIDCheck]->m_shBitAddr);
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] PPID Validation Event:  [B %X] Reply bit On.", 
																m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, 
																m_ShCommData_Master[eP2C_PPIDCheck]->m_shBitAddr );
			
			m_ShCommData_Master[eP2C_PPIDCheck]->m_nPPID = nPPID;
			m_ShCommData_Master[eP2C_PPIDCheck]->m_bWaitTime = TRUE;
			m_ShCommData_Master[eP2C_PPIDCheck]->m_TimeCheck.StartTimer();
			nStep = -1;
			if(nStepState != STEP_NG)
				nStepState = STEP_OK;
			break;
		}
	}
	
	if(nStepState == STEP_NG) 
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] PPID Transfer Event",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
}

void CBaseModule::P2C_PPIDValidationSequenceEnd()
{
	ULONG	nElaspedTime	= 0;
	long	nPPID			= m_ShCommData_Master[eP2C_PPIDCheck]->m_nPPID;
	
	// Event Off Check
	if(m_stpModRunInfo->stDataChangeRlyEvtIO.bPPIDValidationReq == TRUE)
	{
		if(m_ShCommData_Master[eP2C_PPIDCheck]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			nElaspedTime = m_ShCommData_Master[eP2C_PPIDCheck]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eP2C_PPIDCheck]->m_nElaspedTime = nElaspedTime;
			
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_PPIDCheck]->m_shBitAddr);
			
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] PPID Validation Event Bit not off[Elasped Time : %dms]" , 
																m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] PPID Validation Event Bit not off[Elasped Time : %dms]" , 
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			
			m_ShCommData_Master[eP2C_PPIDCheck]->m_bWaitTime = FALSE;
		}
		return;
	}
	
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	sig.nSignal = sigPPIDEvent;
	sig.nFrom	= PLC_TASK_ID;
	sig.nTo		= GUI_TASK_ID;
	
	sig.unionData.stEvtPPIDData.nModuleType = m_stpModCfg->nModuleID;
	memcpy(sig.unionData.stEvtPPIDData.szPPID, m_stpModRunInfo->szPPIDTransferData, MAX_PPID_LEN);
	
	// Reply Bit Off
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_PPIDCheck]->m_shBitAddr);
	m_pParent->SendSignalToSCH(&sig);
	
	nElaspedTime = m_ShCommData_Master[eP2C_PPIDCheck]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[eP2C_PPIDCheck]->m_nElaspedTime = nElaspedTime;
	
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] PPID Validation Event Complete!![Elasped Time : %dms]" , 
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] PPID Validation Event Complete!![Elasped Time : %dms]" , 
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	
	m_ShCommData_Master[eP2C_PPIDCheck]->m_bWaitTime = FALSE;
}

/*
@ PLC -> CIM ECID Change Event는 사용하지 않는다.
  PLC에서 올려주는 ECID를 Real-Time Read하여 기존 Data와 변경점 있으면
  ECID Change 보고 진행. (FLOW / PRESS / TEMP 포함)
   
*/
//	EQ Constance Change Report
void CBaseModule::P2C_ECIDChangeSequence()
{
/*
	short   shReadData[2500]	= { 0x00, };
	long	nReadDataSize	= sizeof(short) * 2500;
	long	nAddr			= W_POS1_ECID1_DEF_DATA;
	long	nStartAddr		= 0;
	short	shCmdPacket[4]	= { 0x00, };
	short	shRlyData		= 0x41;
	long	nDataSize		= sizeof(short);
	long	nStep			= 0;
	long	nStepState		= STEP_START;	
	long	i				= 0;
	long	nLeft			= 0;
	
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	stECIDChangeType	*pData	= &sig.unionData.stECIDChange;
	stECIDConfigType	*pECCfg	= NULL;	
	
	GetMasterDevBWAddr(eP2C_ECIDChange, m_stpModCfg->nModuleID, B_L2_TO_CIM_ECID_CHANGE_EVENT, W_L2_FROM_CIM_ECID_CHANGE_RLY_DATA);	
	
	while(nStep != -1)
	{
		switch(nStep++)
		{
		case 0:
			if (m_stpModRunInfo->stDataChangeRlyEvtIO.bECIDEvtReq == FALSE)
			{
				m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_ECIDChange]->m_shBitAddr);
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECID Change Event by EQ is Canceled!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
				nStep = -1;
				nStepState = STEP_NG;
			}
			break;
			
		case 1:	//	변경 요구 Data를 읽는다.
			nStartAddr = GetPosStartAddr(m_stpModCfg->nModuleID);
			nStartAddr += nAddr;
			m_pParent->m_MelERMemIF.MelNetReceiveEx(DevW, nStartAddr, &nReadDataSize, shReadData);
			
				for ( i = 0; i < m_stpModCfg->stECIDTable.nECIDCount; i++ )
				{
					pECCfg = &m_stpModCfg->stECIDTable.stECIDCfg[i];
					if ( pECCfg == NULL )
					{
						m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECID Change Event by EQ has ECID CFG Error(NULL)!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  pECCfg->stParamCfg[0].nMapIndex);
						shRlyData = 0x4E;
						nStepState = STEP_NG;
						break;
					}
					if (pECCfg->stParamCfg[0].nMapIndex < 1)
					{
						m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECID Change Event by EQ has ECID[%d] MapIndex[%d] Error!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, i,  pECCfg->stParamCfg[0].nMapIndex);
						shRlyData = 0x4E;
						nStepState = STEP_NG;
						break;
					}
				}
			
			memcpy(&m_ShCommData_Master[eP2C_ECIDChange]->m_pData, pData, sizeof(stECIDChangeType));
			break;
			
		case 2:	//	Request Bit를 On하여 Machine Command를 수행하도록 한다.
			m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, m_ShCommData_Master[eP2C_ECIDChange]->m_shDataAddr, &nDataSize, &shRlyData);
			m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, m_ShCommData_Master[eP2C_ECIDChange]->m_shBitAddr);
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECID Change Event by EQ Start: Reply bit ON!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
			m_ShCommData_Master[eP2C_ECIDChange]->m_bWaitTime = TRUE;
			m_ShCommData_Master[eP2C_ECIDChange]->m_TimeCheck.StartTimer();
			nStep = -1;
			if(nStepState != STEP_NG)
				nStepState = STEP_OK;
			break;
		}
	}
	
	if(nStepState == STEP_NG) 
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] ECID Data Change Event",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
*/
}

void CBaseModule::P2C_ECIDChangeSequenceEnd()
{
/*
	ULONG	nElaspedTime	= 0;
	stECDataType	*pECData = NULL;
	long	i				= 0;
	long	j				= 0;
	long	nIdx			= 0;
	long	nMapIdx			= 0;
	long	nPlateIdx		= 0;
	short	shRlyData		= 0;
	long	nDataSize		= sizeof(short);
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	//	Machine Command에 대한 응답 Check를 하고, Timeout이 발행하면 Alarm을 보고한다.
	if(m_stpModRunInfo->stDataChangeRlyEvtIO.bECIDEvtReq == TRUE)
	{
		if(m_ShCommData_Master[eP2C_ECIDChange]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			nElaspedTime = m_ShCommData_Master[eP2C_ECIDChange]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eP2C_ECIDChange]->m_nElaspedTime = nElaspedTime;
			
			shRlyData = 0x4E;
			m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, m_ShCommData_Local[eP2C_ECIDChange]->m_shDataAddr, &nDataSize, &shRlyData);
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_ECIDChange]->m_shBitAddr);
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECID Data Change by EQ Set Bit not Off[Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] ECID Data Change by EQ Set Bit not Off[Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			
			m_ShCommData_Master[eP2C_ECIDChange]->m_bWaitTime = FALSE;
		}
		return;
	}
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECID Data Change Request = False", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
	
	// SMA Update
				
		for(i=0; i<m_ShCommData_Master[eP2C_ECIDChange]->m_pData.nECCount; i++)
		{
			if(m_ShCommData_Master[eP2C_ECIDChange]->m_pData.stECData[i].nECID < 1)	
			{
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECID Data Change by EQ has ECID Error : stECData[%d].nECID = %d",
					m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  i, m_ShCommData_Master[eP2C_ECIDChange]->m_pData.stECData[i].nECID);
				continue;
			}
			
			pECData = &m_ShCommData_Master[eP2C_ECIDChange]->m_pData.stECData[i];
			for(j=0; j < m_stpModCfg->stECIDTable.nECIDCount; j++)
			{
				if(m_stpModCfg->stECIDTable.stECIDCfg[j].nECID	== pECData->nECID)
				{
					nIdx = m_stpModCfg->stECIDTable.stECIDCfg[j].stParamCfg[0].nIndex;
					break;
				}
			}
			if(nIdx < 1)
			{
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECID Data Change by EQ has ECID Index Error: ECID = %d", pECData->nECID);
				break;
			}
		}
	
	sig.nSignal = sigEQConstantChange;
	sig.nFrom	= PLC_TASK_ID;
	sig.nTo		= HST_TASK_ID;
	
	memcpy(&sig.unionData.stECIDChange, &m_ShCommData_Master[eP2C_ECIDChange]->m_pData, sizeof(stECIDChangeType));
	
	//	명령 처리 수행후에는 반드시 Melnet Data를 Clear시킨다. 오동작의 원인이 될 수 있다.
	m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, m_ShCommData_Master[eP2C_ECIDChange]->m_shDataAddr, &nDataSize, &shRlyData);
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_ECIDChange]->m_shBitAddr);
	m_pParent->SendSignalToSCH(&sig);
	
	nElaspedTime = m_ShCommData_Master[eP2C_ECIDChange]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[eP2C_ECIDChange]->m_nElaspedTime = nElaspedTime;
	
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECID Data Change by EQ Set Complete!![Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] ECID Data Change by EQ Set Complete!![Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	
	//m_stpSma->stLayOutCfg.nEQType;
	stInfomationLogType info; 
	memset(&info, 0, sizeof(stInfomationLogType)); 
	
//	sprintf(info.szModuleName, m_stpModCfg->szModuleName); 
//	if ( m_stpModCfg->nModuleID < 1 || m_stpModCfg->nModuleID > MAX_LAYER1_MODULE_COUNT )
//		sprintf(info.szUnitName, ""); 
//	else
//		sprintf(info.szUnitName, m_stpSma->stLayOutCfg.stModCfg[m_stpModCfg->nModuleID].stUnitCfg[0].szUnitName); // stUnitCfg[0] 주의 
	sprintf(info.szUnitName, m_stpModCfg->szModuleName); 
	sprintf(info.szLogType, "para"); 
	sprintf(info.szStepID, "       -"); 
	//sprintf(info.szFromPosUnitName, info.szUnitName); 
	//sprintf(info.szToPosUnitName, info.szUnitName); 
	sprintf(info.szPPID, "               -");		//               -,           -
	sprintf(info.szHPanelID, "           -");		//
	sprintf(info.szInformation, "ModuleID[%02d(%s)][OK] ECID Data Change by EQ Set Complete!![Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	//	sprintf(szInformation, "EVENT : [RECIPE DOWNLOAD]  PPID : %s  RECIPE NO : %2d", stEventLogRecipeData->szPPID, stEventLogRecipeData->nRecipeNo);
	m_pParent->m_InfoLog.AddInfo(&info);

	m_ShCommData_Master[eP2C_ECIDChange]->m_bWaitTime = FALSE;
*/
}

void CBaseModule::P2C_TEMPChangeSequence()
{
/*
	short   shReadData[50]	= { 0x00, };
	long	nReadDataSize	= sizeof(short) * 50;
	long	nDevType		= 0;
	long	nAddr			= W_TANK1_ECID_TEMP_DEF_DATA;
	long	nStartAddr		= 0;
	short	shRlyData		= 0x41;
	long	nDataSize		= sizeof(short);
	long	nStep			= 0;
	long	nStepState		= STEP_START;
	long	i				= 0;
	long	nLeft			= 0;
	long	nCnt			= 0;
	
	
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	stECIDChangeType	*pData	= &sig.unionData.stECIDChange;
	stECIDConfigType	*pECCfg	= NULL;	
	
	GetMasterDevBWAddr(eP2C_TEMPChange, m_stpModCfg->nModuleID, B_L2_TO_CIM_TEMP_CHANGE_EVENT, W_L2_FROM_CIM_TEMP_CHANGE_RLY_DATA);	
	
	while(nStep != -1)
	{
		switch(nStep++)
		{
		case 0:
			if ( m_stpModRunInfo->stDataChangeRlyEvtIO.bTemperatureEvtReq == FALSE )
			{
				m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_TEMPChange]->m_shBitAddr);
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Temperature Change Event by EQ is Canceled!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
				nStep = -1;
				nStepState = STEP_NG;
			}
			break;
			
		case 1:	//	변경 요구 Data를 읽는다.
			nStartAddr = GetPosStartAddr(m_stpModCfg->nModuleID);
			nStartAddr += nAddr;
			m_pParent->m_MelERMemIF.MelNetReceiveEx(DevW, nStartAddr, &nReadDataSize, shReadData);
			
			for ( i = 0; i < m_stpModCfg->stECIDTable.nECIDCount; i++ )
			{
				pECCfg = &m_stpModCfg->stECIDTable.stECIDCfg[i];
				if ( pECCfg == NULL )
				{
					m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Temperature Change Event by EQ has ECID CFG Error(NULL)!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  pECCfg->stParamCfg[0].nMapIndex);
					shRlyData = 0x4E;
					nStepState = STEP_NG;
					break;
				}
				if (pECCfg->stParamCfg[0].nMapIndex < 1)
				{
					m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Temperature Change Event by EQ has MapIndex[%d] Error!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  pECCfg->stParamCfg[0].nMapIndex);
					shRlyData = 0x4E;
					nStepState = STEP_NG;
					break;
				}
				
				if ( pECCfg->stParamCfg[0].nModuleID != m_stpModCfg->nModuleID || 
					pECCfg->stParamCfg[0].nConstantType != eConstant_Temperature ) continue;
			}
			memcpy(&m_ShCommData_Master[eP2C_TEMPChange]->m_pData, pData, sizeof(stECIDChangeType));
			break;
			
			// 변경될 Data를 Melnet에 Write한다.
		case 2:	// Request Bit를 On하여 Machine Command를 수행하도록 한다.
			m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, m_ShCommData_Master[eP2C_TEMPChange]->m_shDataAddr, &nDataSize, &shRlyData);
			m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, m_ShCommData_Master[eP2C_TEMPChange]->m_shBitAddr);
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Temperature Change Event by EQ Start: Reply bit ON!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
			m_ShCommData_Master[eP2C_TEMPChange]->m_bWaitTime = TRUE;
			m_ShCommData_Master[eP2C_TEMPChange]->m_TimeCheck.StartTimer();
			nStep = -1;
			if(nStepState != STEP_NG)
				nStepState = STEP_OK;
			break;
		}
	}
	
	if(nStepState == STEP_NG) 
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Temperature data Change Event",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
*/
}

void CBaseModule::P2C_TEMPChangeSequenceEnd()
{
/*
	ULONG	nElaspedTime	= 0;
	stECDataType	*pECData = NULL;
	long	i				= 0;
	long	j				= 0;
	long	nIndex			= 0;
	short	shRlyData		= 0;
	long	nDataSize		= sizeof(short);
	
	//	Machine Command에 대한 응답 Check를 하고, Timeout이 발행하면 Alarm을 보고한다.
	if(m_stpModRunInfo->stDataChangeRlyEvtIO.bTemperatureEvtReq == TRUE)
	{
		if(m_ShCommData_Master[eP2C_TEMPChange]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			nElaspedTime = m_ShCommData_Master[eP2C_TEMPChange]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eP2C_TEMPChange]->m_nElaspedTime = nElaspedTime;
			shRlyData = 0x4E;
			m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, m_ShCommData_Master[eP2C_TEMPChange]->m_shDataAddr, &nDataSize, &shRlyData);
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_TEMPChange]->m_shBitAddr);
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Temperature data Change by EQ Set Timeout[Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Temperature data Change by EQ Set Timeout[Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  nElaspedTime);
			
		}
		return;
	}
	
	// SMA Update	
	for(i=0; i<m_ShCommData_Master[eP2C_TEMPChange]->m_pData.nECCount; i++)
	{
		if(m_ShCommData_Master[eP2C_TEMPChange]->m_pData.stECData[i].nECID < 1)
		{
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Temperature Data Change by EQ has ECID Error : stECData[%d].nECID = %d",
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  i, m_ShCommData_Master[eP2C_TEMPChange]->m_pData.stECData[i].nECID);
			continue;
		}
		
		pECData = &m_ShCommData_Master[eP2C_TEMPChange]->m_pData.stECData[i];
		for(j=0; j < m_stpModCfg->stECIDTable.nECIDCount; j++)
		{
			if(m_stpModCfg->stECIDTable.stECIDCfg[j].nECID	== pECData->nECID)
			{
				nIndex = m_stpModCfg->stECIDTable.stECIDCfg[j].stParamCfg[0].nIndex;
				break;
			}
		}
		if(nIndex < 0)
		{
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Temperature Data Change by EQ has ECID Map Index Error: ECID = %d, nIndex = %d", pECData->nECID,nIndex);
			break;
		}	
	}
	
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	sig.nSignal = sigEQConstantChange;
	sig.nFrom	= PLC_TASK_ID;
	sig.nTo		= GUI_TASK_ID;
	
	memcpy(&sig.unionData.stECIDChange, &m_ShCommData_Master[eP2C_TEMPChange]->m_pData, sizeof(stECIDChangeType));
	
	//	명령 처리 수행후에는 반드시 Melnet Data를 Clear시킨다. 오동작의 원인이 될 수 있다.
	m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, m_ShCommData_Master[eP2C_TEMPChange]->m_shDataAddr, &nDataSize, &shRlyData);
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_TEMPChange]->m_shBitAddr);
	m_pParent->SendSignalToSCH(&sig);
	
	nElaspedTime = m_ShCommData_Master[eP2C_TEMPChange]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[eP2C_TEMPChange]->m_nElaspedTime = nElaspedTime;
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Temperature data Change by EQ Sequence Complete!![Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  nElaspedTime);
	m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] Temperature data Change by EQ Sequence Complete!![Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  nElaspedTime);
	
	m_ShCommData_Master[eP2C_TEMPChange]->m_bWaitTime = FALSE;
*/
}

void CBaseModule::P2C_RecipeDownloadSequence()
{
	long	nStep			= 0;
	long	nStepState		= STEP_START;
	long	nDataNo			= 0;
	long	nUniqueID		= 0;
	long	i				= 0;
	long	nCnt			= 0;
	long	nPPID			= 0;
	long	nDataSize		= sizeof(short) * 8;
	long	nPanelSize		= sizeof(short) * 6;
	long	nCmdDataSize	= sizeof(short);
	short	shAckCode		= 0;		// 0=: Nak 0<: Ack(Recipe No)
	short	sPPID[8]		= {0, };
	short   sPanelID[6]		= {0x00,};

	long	nRPCMode	= 0;

	BOOL    bIsRPCData      = FALSE;

	char	szPPID[MAX_PPID_LEN+1];
	memset(szPPID, 0x00, MAX_PPID_LEN+1);

	char    szPanelID[MAX_PANEL_ID_LEN+1];
	memset(szPanelID, 0x00, MAX_PANEL_ID_LEN+1);

	
	GetMasterDevBWAddr(eP2C_RecipeDown, m_stpModCfg->nModuleID, B_L2_TO_CIM_RECIPE_DOWN_RLY, W_L2_FROM_CIM_RCP_DOWN_RLY_DATA);	
	GetLocalDevBWAddr(eP2C_RecipeDown, m_stpModCfg->nModuleID, 0, W_L2_PPID_TRANS_DATA);

	GetLocalDevBWAddr(eP2C_RPC_GLASSID, m_stpModCfg->nModuleID, 0, W_L2_PANELID_TRANS_DATA);
	
	stUnitRunInfoType		*pUnitInfo	= NULL;
	
	stRPCDataType            stRPCData;
	memset(&stRPCData, 0x00, sizeof(stRPCDataType));

	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	// Event 진행 전 Reply Bit Off. (Reply Bit Off 안된 경우 대비)
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_RecipeDown]->m_shBitAddr);

	while(nStep != -1) 
	{
		switch(nStep++)
		{
		case 0:
			if(m_stpModRunInfo->stEQCmdRlyEvtIO.bRecipeDownReq == FALSE)
			{
				m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_RecipeDown]->m_shBitAddr);
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Recipe Download Event is Canceled!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
				nStep = -1;
				nStepState = STEP_NG;
			}
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Recipe Download Event Start!! : Request On", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
			break;
			
		case 1:	// Event Data Read
			m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, m_ShCommData_Local[eP2C_RecipeDown]->m_shDataAddr, &nDataSize, sPPID);
			memcpy(szPPID, sPPID, MAX_PPID_LEN);
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Recipe Download Event : Received PPID = %s", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  szPPID);

			m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, m_ShCommData_Local[eP2C_RPC_GLASSID]->m_shDataAddr, &nPanelSize, sPanelID);
			memcpy(szPanelID, sPanelID, MAX_PANEL_ID_LEN);
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Recipe Download Event : Received PanelID = %s", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  sPanelID);
			break;
			
		case 2: // PPID Validation, Recipe Check
			nRPCMode = m_stpSma->stLayOutCfg.stEOIDTable.stEOIDData[4].stEOMDData[0].nEOV;	// 0: Host Data 1:Inline Data
			
			if (nRPCMode == eRPC_Host )
			{
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("EIN EOID Mode = Host Data ON ");

				bIsRPCData = OnCheckRPCData(m_stpModCfg->nModuleID, szPanelID, &stRPCData);

				if (bIsRPCData == TRUE)
				{
					m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] RPC Process Start : GlassID = [%s] PPID = [%s -> %s]", 
						m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, szPanelID, szPPID, stRPCData.szRPC_PPID);
					
					memcpy(szPPID, stRPCData.szRPC_PPID, MAX_PPID_LEN);
					SetRPCState(m_stpModCfg->nModuleID, szPanelID);
				}
			}

			for(i = 0; i < MAX_PPID_COUNT; i++)
			{
				switch(m_stpSma->stLayOutCfg.nEQType)
				{
				case eEQType_ETCHSTRIP:
				case eEQType_ETCH:
				case eEQType_STRIP:
					if(strncmp(m_stpSma->stSysData.stMainRecipeTbl.stRcp[i].stWetRcp.stMainRecipe.szPPID, szPPID, MAX_PPID_LEN) == 0)
					{
						if(m_stpSma->stSysData.stMainRecipeTbl.stRcp[i].stWetRcp.stRecipeHead.bUsed == FALSE)
						{
							shAckCode = 0;	// Nak
							m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Recipe Download Event : PPID is Not Used = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
							sprintf(sig.unionData.s10f3FromHost.szText, "ModuleID[%02d(%s)] Recipe Download Event : PPID is Not Used = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
							break;
						}
						
						switch(m_stpModCfg->nModuleID)
						{
						case eModuleType_Etcher:		// RW
							shAckCode = (short)m_stpSma->stSysData.stMainRecipeTbl.stRcp[i].stWetRcp.stMainRecipe.nEtchRecipeNo;
							nPPID = i+1;
							if(shAckCode > 0 && shAckCode != 99)
							{
								if(m_stpSma->stSysData.stRecipeTblEtch.stRecipeData[shAckCode-1].stRecipeHead.bUsed == FALSE)
								{
									shAckCode = 0;	// Nak
									m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Recipe Download Event : Process Data is Not Used = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
									sprintf(sig.unionData.s10f3FromHost.szText, "ModuleID[%02d(%s)] Recipe Download Event : Process Data is Not Used = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
								}
								if(&m_stpSma->stSysData.stRecipeTblEtch.stRecipeData[shAckCode-1] == NULL)
								{
									shAckCode = 0;	// Nak
									m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Recipe Download Event : Process Data is NULL = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
									sprintf(sig.unionData.s10f3FromHost.szText, "ModuleID[%02d(%s)] Recipe Download Event : Process Data is NULL = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
								}
							}
							break;
						case eModuleType_Stripper:		
							shAckCode = (short)m_stpSma->stSysData.stMainRecipeTbl.stRcp[i].stWetRcp.stMainRecipe.nStripRecipeNo;
							nPPID = i+1;
							if(shAckCode > 0)
							{
								if(m_stpSma->stSysData.stRecipeTblStrip.stRecipeData[shAckCode-1].stRecipeHead.bUsed == FALSE)
								{
									shAckCode = 0;	// Nak
									m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Recipe Download Event : Process Data is Not Used = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
									sprintf(sig.unionData.s10f3FromHost.szText, "ModuleID[%02d(%s)] Recipe Download Event : Process Data is Not Used = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
								}
								if(&m_stpSma->stSysData.stRecipeTblStrip.stRecipeData[shAckCode-1] == NULL)
								{
									shAckCode = 0;	// Nak
									m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Recipe Download Event : Process Data is NULL = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
									sprintf(sig.unionData.s10f3FromHost.szText, "ModuleID[%02d(%s)] Recipe Download Event : Process Data is NULL = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
								}
							}
							break;
						}
					}
					break;
					
				case eEQType_PFC:					
				case eEQType_WRU: // WRU-2500 공통 사용
					if(strncmp(m_stpSma->stSysData.stMainRecipeTbl.stRcp[i].stPFCRcp.stMainRecipe.szPPID, szPPID, MAX_PPID_LEN) == 0)
					{
						if(m_stpSma->stSysData.stMainRecipeTbl.stRcp[i].stPFCRcp.stRecipeHead.bUsed == FALSE)
						{
							shAckCode = 0;	// Nak
							m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Recipe Download Event : PPID is Not Used = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
							sprintf(sig.unionData.s10f3FromHost.szText, "ModuleID[%02d(%s)] Recipe Download Event : PPID is Not Used = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
							break;
						}
						
						shAckCode = (short)m_stpSma->stSysData.stMainRecipeTbl.stRcp[i].stPFCRcp.stMainRecipe.nCLNRecipeNo;
						nPPID = i+1;
						if(shAckCode > 0)
						{
							if(m_stpSma->stSysData.stRecipeTblCLN.stRecipeData[shAckCode-1].stRecipeHead.bUsed == FALSE)
							{
								shAckCode = 0;	// Nak
								m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Recipe Download Event : Process Data is Not Used = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
								sprintf(sig.unionData.s10f3FromHost.szText, "ModuleID[%02d(%s)] Recipe Download Event : Process Data is Not Used = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
							}
							if(&m_stpSma->stSysData.stRecipeTblCLN.stRecipeData[shAckCode-1].stPFCProcData ==NULL)
							{
								shAckCode = 0;	// Nak
								m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Recipe Download Event : Process Data is NULL = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
								sprintf(sig.unionData.s10f3FromHost.szText, "ModuleID[%02d(%s)] Recipe Download Event : Process Data is NULL = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
							}
						}
						break;
					}
					break;
					
				case eEQType_EDGE:
					if(strncmp(m_stpSma->stSysData.stMainRecipeTbl.stRcp[i].stEdgeRcp.stMainRecipe.szPPID, szPPID, MAX_PPID_LEN) == 0)
					{
						if(m_stpSma->stSysData.stMainRecipeTbl.stRcp[i].stEdgeRcp.stRecipeHead.bUsed == FALSE)
						{
							shAckCode = 0;	// Nak
							m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Recipe Download Event : PPID is Not Used = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
							sprintf(sig.unionData.s10f3FromHost.szText, "ModuleID[%02d(%s)] Recipe Download Event : PPID is Not Used = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
							break;
						}
						
						shAckCode = (short)m_stpSma->stSysData.stMainRecipeTbl.stRcp[i].stEdgeRcp.stMainRecipe.nCLNRecipeNo;
						nPPID = i+1;
						if(shAckCode > 0)
						{
							if(m_stpSma->stSysData.stRecipeTblCLN.stRecipeData[shAckCode-1].stRecipeHead.bUsed == FALSE)
							{
								shAckCode = 0;	// Nak
								m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Recipe Download Event : Process Data is Not Used = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
								sprintf(sig.unionData.s10f3FromHost.szText, "ModuleID[%02d(%s)] Recipe Download Event : Process Data is Not Used = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
							}
							if(&m_stpSma->stSysData.stRecipeTblCLN.stRecipeData[shAckCode-1].stEdgProcData ==NULL)
							{
								shAckCode = 0;	// Nak
								m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Recipe Download Event : Process Data is NULL = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
								sprintf(sig.unionData.s10f3FromHost.szText, "ModuleID[%02d(%s)] Recipe Download Event : Process Data is NULL = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
							}
						}
						break;
					}
					break;
				
				case eEQType_LCPI:
				case eEQType_LCODF:
				case eEQType_LCRW:
					if(strncmp(m_stpSma->stSysData.stMainRecipeTbl.stRcp[i].stLCRcp.stMainRecipe.szPPID, szPPID, MAX_PPID_LEN) == 0)
					{
						if(m_stpSma->stSysData.stMainRecipeTbl.stRcp[i].stLCRcp.stRecipeHead.bUsed == FALSE)
						{
							shAckCode = 0;	// Nak
							m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Recipe Download Event : PPID is Not Used = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
							sprintf(sig.unionData.s10f3FromHost.szText, "ModuleID[%02d(%s)] Recipe Download Event : PPID is Not Used = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
							break;
						}
						
						shAckCode = (short)m_stpSma->stSysData.stMainRecipeTbl.stRcp[i].stLCRcp.stMainRecipe.nCLNRecipeNo;
						nPPID = i+1;
						if(shAckCode > 0)
						{
							if(m_stpSma->stSysData.stRecipeTblCLN.stRecipeData[shAckCode-1].stRecipeHead.bUsed == FALSE)
							{
								shAckCode = 0;	// Nak
								m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Recipe Download Event : Process Data is Not Used = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
								sprintf(sig.unionData.s10f3FromHost.szText, "ModuleID[%02d(%s)] Recipe Download Event : Process Data is Not Used = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
							}
							if(&m_stpSma->stSysData.stRecipeTblCLN.stRecipeData[shAckCode-1].stLCProcData ==NULL)
							{
								shAckCode = 0;	// Nak
								m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Recipe Download Event : Process Data is NULL = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
								sprintf(sig.unionData.s10f3FromHost.szText, "ModuleID[%02d(%s)] Recipe Download Event : Process Data is NULL = Nak ", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
							}
						}
						break;
					}
					break;
					

				default:
					m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("Recipe Download Event : Undefined EQ Type [EQ Type: %d]", m_stpSma->stLayOutCfg.nEQType);
					break;
				}
			}
			break;
			
		case 3:	//	해당 Module의 Recipe Data Update 
			if ( shAckCode > 0 )
			{
				switch(m_stpSma->stLayOutCfg.nEQType)
				{
				case eEQType_ETCHSTRIP:
					switch(m_stpModCfg->nModuleID)
					{
					case eModuleType_Etcher:	// RW
						SetEtchProcessData(nPPID);
						break;
					case eModuleType_Stripper:	
						SetStripProcessData(nPPID);
						break;
					}
					break;
					
				case eEQType_ETCH:
					SetEtchProcessData(nPPID);
					break;
					
				case eEQType_STRIP:
					SetStripProcessData(nPPID);
					break;
					
				case eEQType_PFC:
					SetCleanerProcessData(nPPID);
					break;
					
				case eEQType_EDGE:
					SetCleanerProcessData(nPPID);
					break;
					
				case eEQType_WRU:
					SetCleanerProcessData(nPPID);
					break;

				case eEQType_LCPI:
					SetCleanerProcessData(nPPID);
					break;

				case eEQType_LCODF:					
					SetCleanerProcessData(nPPID);
					break;

				case eEQType_LCRW:
					SetCleanerProcessData(nPPID);
					break;
				}
			}
			break;
			
		case 4:	//	Reply Data Set
			m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, m_ShCommData_Master[eP2C_RecipeDown]->m_shDataAddr, &nCmdDataSize, &shAckCode);
			if(shAckCode == 0)
			{	
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Recipe Download Event : PPID Compare = NAK, [W %X] AckCode = %d", 
					m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  
					m_ShCommData_Master[eP2C_RecipeDown]->m_shDataAddr, shAckCode);
				
				SetNotifyEvent(eCode_PPID_NAK);
			}
			else
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Recipe Download Event : PPID Compare = ACK, [W %X] AckCode = %d", 
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  
				m_ShCommData_Master[eP2C_RecipeDown]->m_shDataAddr, shAckCode);
			
			m_ShCommData_Master[eP2C_RecipeDown]->m_nPPID = nPPID;
			m_ShCommData_Master[eP2C_RecipeDown]->m_bWaitTime = TRUE;
			m_ShCommData_Master[eP2C_RecipeDown]->m_TimeCheck.StartTimer();
			nStep = -1;
			
			if(nStepState != STEP_NG) nStepState = STEP_OK;
			
			m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, m_ShCommData_Master[eP2C_RecipeDown]->m_shBitAddr);
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Recipe Download Event : [B %X] Reply bit On", 
																m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, m_ShCommData_Master[eP2C_RecipeDown]->m_shBitAddr);
			break;
		}
	}
	
	if(nStepState == STEP_NG) 
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Recipe Download Event",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);	
}

void CBaseModule::P2C_RecipeDownloadSequenceEnd()
{
	ULONG	nElaspedTime	= 0;
	long	nPPID			= m_ShCommData_Master[eP2C_RecipeDown]->m_nPPID;
	long	nCmdDataSize	= sizeof(short);
	short	shAckCode		= 0;
	
	// Event Off Check
	if(m_stpModRunInfo->stEQCmdRlyEvtIO.bRecipeDownReq == TRUE)
	{
		if(m_ShCommData_Master[eP2C_RecipeDown]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, m_ShCommData_Master[eP2C_RecipeDown]->m_shDataAddr, &nCmdDataSize, &shAckCode);
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_RecipeDown]->m_shBitAddr);
			
			nElaspedTime = m_ShCommData_Master[eP2C_RecipeDown]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eP2C_RecipeDown]->m_nElaspedTime = nElaspedTime;
			
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] [W %X] Ackcode = %d, [B %X] Recipe Download Event Bit not off[Elasped Time : %dms]" ,
																m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, 
																m_ShCommData_Master[eP2C_RecipeDown]->m_shDataAddr, shAckCode,
																m_ShCommData_Master[eP2C_RecipeDown]->m_shBitAddr, nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] [W %X] Ackcode = %d, [B %X] Recipe Download Event Bit not off[Elasped Time : %dms]" ,
																m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, 
																m_ShCommData_Master[eP2C_RecipeDown]->m_shDataAddr, shAckCode,
																m_ShCommData_Master[eP2C_RecipeDown]->m_shBitAddr, nElaspedTime);
			
			m_ShCommData_Master[eP2C_RecipeDown]->m_bWaitTime = FALSE;
		}
	}
	else
	{
		m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Recipe Download Event Request bit = False", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
		
		// Reply Bit Off
		m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, m_ShCommData_Master[eP2C_RecipeDown]->m_shDataAddr, &nCmdDataSize, &shAckCode);
		m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_RecipeDown]->m_shBitAddr);
		
		nElaspedTime = m_ShCommData_Master[eP2C_RecipeDown]->m_TimeCheck.GetTimerAfterStart();
		m_ShCommData_Master[eP2C_RecipeDown]->m_nElaspedTime = nElaspedTime;
		m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] [W %X] Ackcode = %d, [B %X] Bit off, Recipe Download Event Complete!![Elasped Time : %dms]" , 
			m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  
			m_ShCommData_Master[eP2C_RecipeDown]->m_shDataAddr, shAckCode,
			m_ShCommData_Master[eP2C_RecipeDown]->m_shBitAddr,nElaspedTime);
		
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] Recipe Download Event Complete!![Elasped Time : %dms]" , m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  nElaspedTime);
		
		m_ShCommData_Master[eP2C_RecipeDown]->m_bWaitTime = FALSE;
	}
}

//	Process End Event
void CBaseModule::P2C_ProcessEndChangeSequence()
{
	long	nStep			=	0;
	long	nStepState		=	STEP_START;
	long	i				=	0;
	short	shCmdPacket[4]	=	{ 0x00, };
	
	GetMasterDevBWAddr(eP2C_ProcessEnd, m_stpModCfg->nModuleID, B_L2_TO_CIM_PROC_ENDDATA_EVENT, 0);
	
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	// Event 진행 전 Reply Bit Off. (Reply Bit Off 안된 경우 대비)
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_ProcessEnd]->m_shBitAddr);

	while(nStep != -1)
	{
		switch(nStep++)
		{
		case 0:	//	이전 진행중인 Sequence가 있는 경우, Request Bit = False 처리 ;
			if ( m_stpModRunInfo->stDataChangeRlyEvtIO.bProcessEndEvtReq == FALSE)
			{
				m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_ProcessEnd]->m_shBitAddr);
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Process End Event is Canceled!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
				nStep = -1;
				nStepState = STEP_NG;
			}
			break;
			
		case 1:	//	변경 Data를 읽는다.
			GetDataLink_ProcessEndData();
			memcpy(&m_ShCommData_Master[eP2C_ProcessEnd]->m_stpProcEndData, &m_stpModRunInfo->stProcEndData, sizeof(stProcessEndDataType));
			m_ShCommData_Master[eP2C_ProcessEnd]->m_nModuleID = m_stpModCfg->nModuleID;

			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Process End Event : Data Read Complete!! Unique ID[%d, %d, %d, %d]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, 
				m_ShCommData_Master[eP2C_ProcessEnd]->m_stpProcEndData.nUniqueID[0], 
				m_ShCommData_Master[eP2C_ProcessEnd]->m_stpProcEndData.nUniqueID[1],
				m_ShCommData_Master[eP2C_ProcessEnd]->m_stpProcEndData.nUniqueID[2], 
				m_ShCommData_Master[eP2C_ProcessEnd]->m_stpProcEndData.nUniqueID[3] );
			break;
			
		case 2:	//	Request Bit를 On하여 Maching Command를 수행하도록 한다.
			m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, m_ShCommData_Master[eP2C_ProcessEnd]->m_shBitAddr);
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Process End Event : Reply bit On", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
			m_ShCommData_Master[eP2C_ProcessEnd]->m_bWaitTime = TRUE;
			m_ShCommData_Master[eP2C_ProcessEnd]->m_TimeCheck.StartTimer();
			nStep = -1;
			if(nStepState != STEP_NG)
				nStepState = STEP_OK;
			break;
		}
	}
	
	if(nStepState == STEP_NG) 
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Process End Event",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
}

void CBaseModule::P2C_ProcessEndChangeSequenceEnd()
{
	ULONG	nElaspedTime	= 0;
	
	//	Machine Command에 대한 응답 Check를 하고, Timeout이 발행하면 Alarm을 보고한다.
	if(m_stpModRunInfo->stDataChangeRlyEvtIO.bProcessEndEvtReq == TRUE)
	{
		if(m_ShCommData_Master[eP2C_ProcessEnd]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_ProcessEnd]->m_shBitAddr);
			
			nElaspedTime = m_ShCommData_Master[eP2C_ProcessEnd]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eP2C_ProcessEnd]->m_nElaspedTime = nElaspedTime;
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Process End Event Bit not Off[Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Process End Event Bit not Off[Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  nElaspedTime);
			
			m_ShCommData_Master[eP2C_ProcessEnd]->m_bWaitTime = FALSE;
		}
		return;
	}
	
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_ProcessEnd]->m_shBitAddr); // 0x3C OFF
		
	nElaspedTime = m_ShCommData_Master[eP2C_ProcessEnd]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[eP2C_ProcessEnd]->m_nElaspedTime = nElaspedTime;
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Process End Event Complete!![%03d][Elasped Time : %dms]", 
		m_ShCommData_Master[eP2C_ProcessEnd]->m_nModuleID, m_stpModCfg->szModuleName,
		m_ShCommData_Master[eP2C_ProcessEnd]->m_stpProcEndData.nUniqueID[0], nElaspedTime);
	m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] Process End Event Complete!![%03d][Elasped Time : %dms]", 
		m_ShCommData_Master[eP2C_ProcessEnd]->m_nModuleID, m_stpModCfg->szModuleName,
		m_ShCommData_Master[eP2C_ProcessEnd]->m_stpProcEndData.nUniqueID[0], nElaspedTime);
	
	m_ShCommData_Master[eP2C_ProcessEnd]->m_bWaitTime = FALSE;
}

//	Host(CIM) -> EQ Constance Change Request
void CBaseModule::C2P_ECIDChangeSequence(stECIDChangeType stECIDChange)
{
	short   shSetData[2750]	=	{ 0x00, };
	long	nSetDataSize	=	sizeof(short) * 2750;
	long	nStartAddr		=	0;
	short	shAddr			=	W_POS1_ECID1_DEF_SET_DATA;
	short	shCmdBitAddr	=	0;
	long	nStep			=	0;
	long	nStepState		=	STEP_START;	
	long	nIdx			=	0;
	long	i				=	0;
	long	nLeft			=	0;

	short	shMapIndex		= 0;
	long	nDataSize		= sizeof(short);

	
	stECIDConfigType	*pECCfg = NULL;
	
	GetMasterDevBWAddr(eC2P_ECIDChange, m_stpModCfg->nModuleID, B_L2_FROM_CIM_ECID_CHANGE_REQ, 0);
	
	// Event 진행 전 Request Bit Off. (Request Bit Off 안된 경우 대비)
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eC2P_ECIDChange]->m_shBitAddr);

	while(nStep != -1)
	{
		switch(nStep++)
		{
		case 0:	//	이전 진행중인 Sequence가 있는 경우 Check
			//	if ( m_stpModRunInfo->stDataChangeRlyEvtIO.bECIDEvtReq == FALSE )
			if(m_stpModRunInfo->stDataChangeRlyEvtIO.bECIDRly == TRUE)
			{
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECID Change Event is Running!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
				nStep = -1;
				nStepState = STEP_NG;
			}
			break;
			
		case 1:	//	Melsec Network 상의 현재 설정 Data를 읽는다.
			//nStartAddr = GetPosStartAddr(m_stpModCfg->nModuleID);
			//nStartAddr += shAddr;
			//m_pParent->m_MelERMemIF.MelNetReceiveEx(DevW, nStartAddr, &nSetDataSize, shSetData);
			break;
			
		case 2:	//	변경될 Data를 Melnet에 Write한다.
			for( i = 0; i < stECIDChange.nECCount; i++ )
			{
				pECCfg = GetECIDConfig(stECIDChange.stECData[i].nECID);
				if ( pECCfg == NULL )
				{
					m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECID Change Event has ECID CFG Error(NULL)!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
					nStepState = STEP_NG;
					break;
				}
				if (pECCfg->stParamCfg[0].nMapIndex < 1)
				{
					m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECID Change Event has MapIndex[%d] Error!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  pECCfg->stParamCfg[0].nMapIndex);
					nStepState = STEP_NG;
					break;
				}
				
				shSetData[pECCfg->stParamCfg[0].nMapIndex*5 - 5] = (short)atol(stECIDChange.stECData[i].szECDefault);
				shSetData[pECCfg->stParamCfg[0].nMapIndex*5 - 4] = (short)atol(stECIDChange.stECData[i].szECStopLowLimit);
				shSetData[pECCfg->stParamCfg[0].nMapIndex*5 - 3] = (short)atol(stECIDChange.stECData[i].szECStopUpLimit);
				shSetData[pECCfg->stParamCfg[0].nMapIndex*5 - 2] = (short)atol(stECIDChange.stECData[i].szECWarnLowLimit);
				shSetData[pECCfg->stParamCfg[0].nMapIndex*5 - 1] = (short)atol(stECIDChange.stECData[i].szECWarnUpLimit);
			}
			
			// Write Data On Melsec-Net 
			shMapIndex = (short)pECCfg->stParamCfg[0].nMapIndex;
			if (m_stpModCfg->nModuleID == eModuleType_Etcher)
				m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, W_L2_FROM_CIM_ECID_MAPINDEX, &nDataSize, &shMapIndex);
			else if(m_stpModCfg->nModuleID == eModuleType_Stripper)
				m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, W_L3_FROM_CIM_ECID_MAPINDEX, &nDataSize, &shMapIndex);

			m_pParent->m_MelLinkMemIF.MelNetSendEx(DevW, shAddr, &nSetDataSize, shSetData);
			memcpy(&m_ShCommData_Master[eC2P_ECIDChange]->m_pData, &stECIDChange, sizeof(stECIDChangeType));
			break;
			
		case 3:	//	Request Bit를 On하여 Maching Command를 수행하도록 한다.
			m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, m_ShCommData_Master[eC2P_ECIDChange]->m_shBitAddr);	
			m_ShCommData_Master[eC2P_ECIDChange]->m_bWaitTime = TRUE;
			m_ShCommData_Master[eC2P_ECIDChange]->m_TimeCheck.StartTimer();
			nStep = -1;
			if(nStepState != STEP_NG) 
				nStepState = STEP_OK;
			break;
		}
	}
	
	if(nStepState == STEP_NG) 
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] FlowPress Change Event",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
}

void CBaseModule::C2P_ECIDChangeSequenceEnd()
{
	ULONG	nElaspedTime	= 0;
	stECDataType	*pECData = NULL;
	long	i				= 0;
	long	j				= 0;
	long	nIdx			= 0;
	long	nMapIdx			= 0;
	long	nPlateIdx		= 0;
	
	// 응답 Check를 하고, Timeout이 발행하면 Alarm을 보고한다.
	if(m_stpModRunInfo->stDataChangeRlyEvtIO.bECIDRly == FALSE)
	{
		if(m_ShCommData_Master[eC2P_ECIDChange]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eC2P_ECIDChange]->m_shBitAddr);
			nElaspedTime = m_ShCommData_Master[eC2P_ECIDChange]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eC2P_ECIDChange]->m_nElaspedTime = nElaspedTime;
			
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECID Set Timeout[Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] ECID Chagne Set Timeout[Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  nElaspedTime);
			
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eC2P_ECIDChange]->m_shBitAddr);
			m_ShCommData_Master[eC2P_ECIDChange]->m_bWaitTime = FALSE;
		}
		return;
	}
	
	/*
	// SMA Update
	for(i=0; i<m_ShCommData_Master[eC2P_ECIDChange]->m_pData.nECCount; i++)
	{
		if(m_ShCommData_Master[eC2P_ECIDChange]->m_pData.stECData[i].nECID < 1)
		{
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECID Data Change has ECID Error : stECData[%d].nECID = %d",
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  i, m_ShCommData_Master[eC2P_ECIDChange]->m_pData.stECData[i].nECID);
			continue;
		}
		
		pECData = &m_ShCommData_Master[eC2P_ECIDChange]->m_pData.stECData[i];
		for(j=0; j < m_stpModCfg->stECIDTable.nECIDCount; j++)
		{
			if(m_stpModCfg->stECIDTable.stECIDCfg[j].nECID	== pECData->nECID)
			{
				nIdx = m_stpModCfg->stECIDTable.stECIDCfg[j].stParamCfg[0].nIndex;
				break;
			}
		}
		if(nIdx < 1)
		{
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECID Data Change has ECID Index Error: ECID = %d", pECData->nECID);
			break;
		}
		
		m_stpModData->stProcData.stECIDData[nIdx - 1].nECDefault = (long) atoi(pECData->szECDefault);
		m_stpModData->stProcData.stECIDData[nIdx - 1].nECStopLowLimit = (long) atoi(pECData->szECStopLowLimit);
		m_stpModData->stProcData.stECIDData[nIdx - 1].nECStopUpLimit = (long) atoi(pECData->szECStopUpLimit);
		m_stpModData->stProcData.stECIDData[nIdx - 1].nECWarnLowLimit = (long) atoi(pECData->szECWarnLowLimit);
		m_stpModData->stProcData.stECIDData[nIdx - 1].nECWarnUpLimit = (long) atoi(pECData->szECWarnUpLimit);
	}
	*/

	//	명령 처리 수행후에는 반드시 Melnet Data를 Clear시킨다. 오동작의 원인이 될 수 있다.
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eC2P_ECIDChange]->m_shBitAddr);
	nElaspedTime = m_ShCommData_Master[eC2P_ECIDChange]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[eC2P_ECIDChange]->m_nElaspedTime = nElaspedTime;
	
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECID Set Complete!![Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  nElaspedTime);
	m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] ECID Set Complete!![Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  nElaspedTime);
	
	m_ShCommData_Master[eC2P_ECIDChange]->m_bWaitTime = FALSE;
}

//@ Not Used
void CBaseModule::C2P_TEMPChangeSequence(stECIDChangeType stTemp)
{
/*
	short   shSetData[50]	=	{ 0x00, };
	long	nSetDataSize	=	sizeof(short) * 50;
	long	nDevType		=	0;
	long	nAddr			=	W_TANK1_ECID_TEMP_DEF_DATA;
	long	nCmdBitAddr		=	0;
	long	nStep			=	0;
	long	nStepState		=	STEP_START;
	long	i				=	0;
	long	nLeft			=	0;
	long	nStartAddr		=	0;
	
	stECIDConfigType	*pECCfg = NULL;
	
	GetMasterDevBWAddr(eC2P_TEMPChange, m_stpModCfg->nModuleID, B_L2_TO_CIM_TEMP_CHANGE_EVENT, 0);	
	nCmdBitAddr = m_ShCommData_Master[eC2P_TEMPChange]->m_shBitAddr;
	
	while(nStep != -1)
	{
		switch(nStep++)
		{
		case 0:	//	이전 진행중인 Sequence가 있는 경우, Request Bit = False 처리 ;
			if ( m_stpModRunInfo->stDataChangeRlyEvtIO.bTemperatureRly == FALSE)
			{
				m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nCmdBitAddr);
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Temperature Change Event is Canceled!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
				nStep = -1;
				nStepState = STEP_NG;
			}
			break;
			
		case 1:	//	Melsec Network 상의 현재 설정 Data를 읽는다.
			nStartAddr = GetPosStartAddr(m_stpModCfg->nModuleID);
			nStartAddr += nAddr;
			m_pParent->m_MelERMemIF.MelNetReceiveEx(DevW, nStartAddr, &nSetDataSize, shSetData);
			break;
			
		case 2:	//	변경될 Data를 Melnet에 Write한다.
			for( i = 0; i < stTemp.nECCount; i++ )
			{
				pECCfg = GetECIDConfig(stTemp.stECData[i].nECID);
				if ( pECCfg == NULL )
				{
					m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Temperature Change Event has ECID CFG Error(NULL)!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
					nStepState = STEP_NG;
					break;
				}
				if (pECCfg->stParamCfg[0].nMapIndex < 1)
				{
					m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Temperature Change Event has MapIndex[%d] Error!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  pECCfg->stParamCfg[0].nMapIndex);
					nStepState = STEP_NG;
					break;
				}
				
				shSetData[pECCfg->stParamCfg[0].nMapIndex - 1] = (short)stTemp.stECData[i].szECDefault,"%06.0f";
				shSetData[pECCfg->stParamCfg[0].nMapIndex] = (short)stTemp.stECData[i].szECStopLowLimit,"%06.0f";
				shSetData[pECCfg->stParamCfg[0].nMapIndex + 1] = (short)stTemp.stECData[i].szECStopUpLimit,"%06.0f";
				shSetData[pECCfg->stParamCfg[0].nMapIndex + 2] = (short)stTemp.stECData[i].szECWarnLowLimit,"%06.0f";
				shSetData[pECCfg->stParamCfg[0].nMapIndex + 3] = (short)stTemp.stECData[i].szECWarnUpLimit,"%06.0f";
			}
			
			// Write Data On Melsec-Net 
			m_pParent->m_MelERMemIF.MelNetSendEx(nDevType, nAddr, &nSetDataSize, shSetData);
			break;
			
		case 3:	//	Request Bit를 On하여 Maching Command를 수행하도록 한다.
			m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, nCmdBitAddr);
			m_ShCommData_Master[eC2P_TEMPChange]->m_bWaitTime = TRUE;
			m_ShCommData_Master[eC2P_TEMPChange]->m_TimeCheck.StartTimer();
			nStep = -1;
			if(nStepState != STEP_NG) 
				nStepState = STEP_OK;
			break;
		}
	}
	
	if(nStepState == STEP_NG) 
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Temperature Change Set",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
*/
}

//@ Not Used
void CBaseModule::C2P_TEMPChangeSequenceEnd()
{
/*
	ULONG	nElaspedTime	= 0;
	stECDataType	*pECData = NULL;
	long	i				=0;
	long	j				= 0;
	long	nIdx			= 0;
	
	//	Machine Command에 대한 응답 Check를 하고, Timeout이 발행하면 Alarm을 보고한다.
	if(m_stpModRunInfo->stDataChangeRlyEvtIO.bTemperatureRly == TRUE)
	{
		if(m_ShCommData_Master[eC2P_TEMPChange]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eC2P_TEMPChange]->m_shBitAddr);
			nElaspedTime = m_ShCommData_Master[eC2P_TEMPChange]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eC2P_TEMPChange]->m_nElaspedTime = nElaspedTime;
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Temperature Change Set Timeout[Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Temperature Change Set Timeout[Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  nElaspedTime);
			m_ShCommData_Master[eC2P_TEMPChange]->m_bWaitTime = FALSE;
			
		}
		return;
	}
	
	// SMA Update	
	for(i=0; i<m_ShCommData_Master[eC2P_TEMPChange]->m_pData.nECCount; i++)
	{
		if(m_ShCommData_Master[eC2P_TEMPChange]->m_pData.stECData[i].nECID < 1)	
		{
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Temperature Change has ECID Error : stECData[%d].nECID = %d",
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  i, m_ShCommData_Master[eC2P_TEMPChange]->m_pData.stECData[i].nECID);
			continue;
		}
		
		pECData = &m_ShCommData_Master[eC2P_TEMPChange]->m_pData.stECData[i];
		for(j=0; j < m_stpModCfg->stECIDTable.nECIDCount; j++)
		{
			if(m_stpModCfg->stECIDTable.stECIDCfg[j].nECID	== pECData->nECID)
			{
				nIdx = m_stpModCfg->stECIDTable.stECIDCfg[j].stParamCfg[0].nIndex;
				break;
			}
		}
		if(nIdx < 1)
		{
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Temperature Data Change has ECID Index Error: ECID = %d", pECData->nECID);
			break;
		}
		
		m_stpModData->stProcData.stTempData[nIdx - 1].fTempDefault = (float) atoi(pECData->szECDefault);
		m_stpModData->stProcData.stTempData[nIdx - 1].fTempStopLowLimit = (float) atoi(pECData->szECStopLowLimit);
		m_stpModData->stProcData.stTempData[nIdx - 1].fTempStopUpLimit = (float) atoi(pECData->szECStopUpLimit);
		m_stpModData->stProcData.stTempData[nIdx - 1].fTempWarnLowLimit = (float) atoi(pECData->szECWarnLowLimit);
		m_stpModData->stProcData.stTempData[nIdx - 1].fTempWarnUpLimit = (float) atoi(pECData->szECWarnUpLimit);
	}
	
	//	명령 처리 수행후에는 반드시 Melnet Data를 Clear시킨다. 오동작의 원인이 될 수 있다.
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eC2P_TEMPChange]->m_shBitAddr);
	nElaspedTime = m_ShCommData_Master[eC2P_TEMPChange]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[eC2P_TEMPChange]->m_nElaspedTime = nElaspedTime;
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Temperature Change Set Complete!![Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  nElaspedTime);
	m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] Temperature Change Set Complete!![Elasped Time : %dms]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  nElaspedTime);
	m_ShCommData_Master[eC2P_TEMPChange]->m_bWaitTime = FALSE;
*/
}

void CBaseModule::C2P_DataChangeSequence(stECIDChangeType stData)
{
	long	i = 0;
	stECIDChangeType	stFlowPress;
	stECIDChangeType	stTemp;
	stECIDChangeType	stCurTankData;
	stECIDChangeType	*pChgData = NULL;
	
	BOOL	bChgFlowPress	= FALSE;
	BOOL	bChgTemp		= FALSE;
	BOOL	bChgTank		= FALSE;
	
	memset(&stFlowPress	, 0x00, sizeof(stECIDChangeType));
	memset(&stTemp	, 0x00, sizeof(stECIDChangeType));
	memset(&stCurTankData	, 0x00, sizeof(stECIDChangeType));
	
	stECIDConfigType	*pECCfg = NULL;

	for( i = 0; i < stData.nECCount; i++ )
	{
		pECCfg = GetECIDConfig(stData.stECData[i].nECID);
		switch(pECCfg->stParamCfg[0].nConstantType)
		{
		case eConstant_Flow:
		case eConstant_Press:
			pChgData = &stFlowPress;		bChgFlowPress	= TRUE;	break;
		case eConstant_Temperature:
			pChgData = &stTemp;				bChgTemp	= TRUE;	break;
		case eConstant_TankInfo:
			pChgData = &stCurTankData;		bChgTank	= TRUE;	break;
		}
		
		pChgData->stECData[pChgData->nECCount].nECID	= stData.stECData[i].nECID;
		sprintf(pChgData->stECData[pChgData->nECCount].szECDefault,"%06.0f", stData.stECData[i].szECDefault);
		sprintf(pChgData->stECData[pChgData->nECCount].szECStopLowLimit,"%06.0f", stData.stECData[i].szECStopLowLimit);
		sprintf(pChgData->stECData[pChgData->nECCount].szECStopUpLimit,"%06.0f", stData.stECData[i].szECStopUpLimit);
		sprintf(pChgData->stECData[pChgData->nECCount].szECWarnLowLimit,"%06.0f", stData.stECData[i].szECWarnLowLimit);
		sprintf(pChgData->stECData[pChgData->nECCount].szECWarnUpLimit,"%06.0f", stData.stECData[i].szECWarnUpLimit);
		pChgData->nECCount++;
	}
	
	if ( bChgFlowPress	)
		C2P_ECIDChangeSequence(stFlowPress);
	if ( bChgTemp	)
		C2P_TEMPChangeSequence(stTemp);
}

void CBaseModule::SetConfig(void *pParent, stModuleCfgType *stpModCfg, stModuleDataInfoType *stpModDataInfo, stModuleRunInfoType *stpModRunInfo )
{
	m_pParent		=	(CMainPlc *)pParent;
	m_stpModCfg		=	stpModCfg;
	m_stpModRunInfo	=	stpModRunInfo;
	m_stpModData	=	stpModDataInfo;
	
	if(m_stpSma->stLayOutCfg.nEQType == eEQType_ETCHSTRIP)
	{
		switch(m_stpModCfg->nModuleID)
		{
		case eModuleType_Etcher:	
			m_cMyQue.Create(MDL_ETCH_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));	break;
		case eModuleType_Stripper:		
			m_cMyQue.Create(MDL_STRP_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));	break;
		case eModuleType_Buffer:		
			m_cMyQue.Create(MDL_BUFF_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));	break;
		case eModuleType_Bypass:		
			m_cMyQue.Create(MDL_BYPASS_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));	break;
		}
	}
	else if(m_stpSma->stLayOutCfg.nEQType == eEQType_ETCH)
	{
		m_cMyQue.Create(MDL_ETCH_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));
	}
	else if(m_stpSma->stLayOutCfg.nEQType == eEQType_STRIP)
	{
		m_cMyQue.Create(MDL_STRP_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));
	}
	else if(m_stpSma->stLayOutCfg.nEQType == eEQType_PFC)
	{
		switch(m_stpModCfg->nModuleID)
		{
		case eModuleType_PFC:
			m_cMyQue.Create(MDL_PFC_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));	break;
		case eModuleType_EX:
			m_cMyQue.Create(MDL_EX_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));	break;
		}
	}
	else if(m_stpSma->stLayOutCfg.nEQType == eEQType_EDGE)
	{
		m_cMyQue.Create(MDL_EDGE_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));
	}
	else if(m_stpSma->stLayOutCfg.nEQType == eEQType_WRU)
	{
		m_cMyQue.Create(MDL_WRU_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));
	}
	else if(m_stpSma->stLayOutCfg.nEQType == eEQType_LCPI)
	{
		m_cMyQue.Create(MDL_LCPI_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));
	}
	else if(m_stpSma->stLayOutCfg.nEQType == eEQType_LCODF)
	{
		m_cMyQue.Create(MDL_LCODF_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));
	}
	else if(m_stpSma->stLayOutCfg.nEQType == eEQType_LCRW)
	{
		m_cMyQue.Create(MDL_LCRW_QUEUE_NAME, MAX_MDL_QUEUE_COUNT, sizeof(stPlcQueueType));
	}
}

void CBaseModule::UpdateHandShakeInfoToUpper(short *pshData, short shAddress)
{
	long i		= 0;
	long j		= 0;
	long nLen	= 0;
	long nValue[2] = { 0x00, };	
	bool bBitState = 0;
	short shHSAddr = shAddress;
	long nHSNo	= 0;
	
	stGlassSendIOType		*pSendIO			= NULL;
	stContactPointIOType	*pSendContactPoint	= NULL;
	stGlassRecvIOType		*pRecvIO			= NULL;
	stContactPointIOType	*pRecvContactPoint	= NULL;
	stPanelInfoType			*pTransferData		= NULL;
	
	for( i = 0; i < m_stpModCfg->nHandShakeCount; i++ )
	{
		pSendIO = &m_stpModRunInfo->stToUpperIO[i].stSendIO;
		pSendContactPoint= &m_stpModRunInfo->stToUpperIO[i].stSendContactPoint;
		
		pRecvIO = &m_stpModRunInfo->stToUpperIO[i].stRecvIO;
		pRecvContactPoint= &m_stpModRunInfo->stToUpperIO[i].stRecvContactPoint;
		
		if ( m_stpModRunInfo == NULL || pRecvIO == NULL || pRecvContactPoint == NULL 
			|| pSendIO == NULL || pSendContactPoint == NULL )
			continue;
		
		memcpy(&nValue, &pshData[i*8], sizeof(long)*2);
		for( j = 0 ; j < 64; j++ )
		{
			if(j < 32)
				bBitState = ( nValue[0] >> j) & 0x0001;
			else
				bBitState = ( nValue[1] >> j) & 0x0001;
			
			switch(j)
			{
 			case 0:		pSendIO->bHeartBeat					= bBitState;	break;
 			case 1:		pSendIO->bPause						= bBitState;	break; 
 			case 2:		pSendIO->bAbnormal					= bBitState;	break; 	
 			case 3:		pSendIO->bReserved3					= bBitState;	break; 	
 			case 4:		pSendIO->bSendAble					= bBitState;	break; 	
 			case 5:		pSendIO->bSendStart					= bBitState;	break; 	
 			case 6:		pSendIO->bSendComplete				= bBitState;	break; 	
 			case 7:		pSendIO->bImmPauseReq				= bBitState;	break; 
 			case 8:		pSendIO->bReturnRecvStart			= bBitState;	break; 	
 			case 9:		pSendIO->bReturnRecvComplete		= bBitState;	break; 	
 			case 10:	pSendIO->bExchangeFlag				= bBitState;	break; 	
 			case 11:	pSendIO->bMultiCarryFlag			= bBitState;	break; 	
 			case 12:	pSendIO->bReserved12				= bBitState;	break; 	
 			case 13:	pSendIO->bReserved13				= bBitState;	break; 	
 			case 14:	pSendIO->bReserved14				= bBitState;	break; 	
 			case 15:	pSendIO->bReserved15				= bBitState;	break; 	
 			case 16:	pSendIO->bPreAction1				= bBitState;	break; 	
 			case 17:	pSendIO->bPreAction2				= bBitState;	break; 	
 			case 18:	pSendIO->bMidAction1				= bBitState;	break; 
 			case 19:	pSendIO->bMidAction2				= bBitState;	break; 	
 			case 20:	pSendIO->bPostAction1				= bBitState;	break; 
 			case 21:	pSendIO->bReserved21				= bBitState;	break; 	
 			case 22:	pSendIO->bReserved22				= bBitState;	break; 	
 			case 23:	pSendIO->bReserved23				= bBitState;	break; 	
 			case 24:	pSendIO->bReserved24				= bBitState;	break; 	
 			case 25:	pSendIO->bReserved25				= bBitState;	break; 	
 			case 26:	pSendIO->bHSResumeReq				= bBitState;	break; 	
 			case 27:	pSendIO->bHSRecoveryAck				= bBitState;	break; 	
 			case 28:	pSendIO->bHSRecoveryNak				= bBitState;	break; 	
 			case 29:	pSendIO->bReserved29				= bBitState;	break; 	
 			case 30:	pSendIO->bHSInitial					= bBitState;	break; 	
 			case 31:	pSendIO->bHSError					= bBitState;	break; 	

 			case 32:	pSendContactPoint->bAbnormal		= bBitState;	break; 	
 			case 33:	pSendContactPoint->bTypeofArm		= bBitState;	break; 	
 			case 34:	pSendContactPoint->bTypeofStage		= bBitState;	break; 	
 			case 35:	pSendContactPoint->bManualOp		= bBitState;	break; 	
 			case 36:	pSendContactPoint->bSafety			= bBitState;	break; 	
 			case 37:	pSendContactPoint->bEmpty			= bBitState;	break; 
 			case 38:	pSendContactPoint->bWait			= bBitState;	break; 	
 			case 39:	pSendContactPoint->bBusy			= bBitState;	break; 	
 			case 40:	pSendContactPoint->bPause			= bBitState;	break; 
 			case 41:	pSendContactPoint->bReserved9		= bBitState;	break; 	
 			case 42:	pSendContactPoint->bArm1Violate		= bBitState;	break; 	
 			case 43:	pSendContactPoint->bArm2Violate		= bBitState;	break; 	
 			case 44:	pSendContactPoint->bArm1FoldComplete= bBitState;	break; 	
 			case 45:	pSendContactPoint->bArm2FoldComplete= bBitState;	break; 	
 			case 46:	pSendContactPoint->bArm1GlassCheck	= bBitState;	break; 	
 			case 47:	pSendContactPoint->bArm2GlassCheck	= bBitState;	break; 	
 			case 48:	pSendContactPoint->bRobotDirection	= bBitState;	break; 	
 			case 49:	pSendContactPoint->bReserved17		= bBitState;	break; 
 			case 50:	pSendContactPoint->bReserved18		= bBitState;	break; 	
 			case 51:	pSendContactPoint->bReserved19		= bBitState;	break; 	
 			case 52:	pSendContactPoint->bLiftUp			= bBitState;	break; 	
 			case 53:	pSendContactPoint->bLiftDown		= bBitState;	break; 	
 			case 54:	pSendContactPoint->bStopperUp		= bBitState;	break; 
 			case 55:	pSendContactPoint->bStopperDown		= bBitState;	break; 	
 			case 56:	pSendContactPoint->bDoorOpen		= bBitState;	break; 	
 			case 57:	pSendContactPoint->bDoorClose		= bBitState;	break; 	
 			case 58:	pSendContactPoint->bGlassDetect		= bBitState;	break; 	
 			case 59:	pSendContactPoint->bBodyMoving		= bBitState;	break; 	
 			case 60:	pSendContactPoint->bBodyOP			= bBitState;	break; 	
 			case 61:	pSendContactPoint->bReserved29		= bBitState;	break; 	
 			case 62:	pSendContactPoint->bReserved30		= bBitState;	break; 	
 			case 63:	pSendContactPoint->bReserved31		= bBitState;	break; 
			}
			
			m_bHSLogUpper1[i][j] = bBitState;
			if(m_bHSLogUpper1[i][j] != m_bHSLogUpperOld1[i][j])
			{
				if(j != 0)
				{
					pTransferData = &m_stpModRunInfo->stInGlassData[i];
					m_bHSLogUpperState[i] = TRUE; // Parity Check Skip
					
					AnalizeHSLog(shHSAddr);
				}
				m_bHSLogUpperOld1[i][j] = m_bHSLogUpper1[i][j];
			}
		}
		
		memcpy(&nValue, &pshData[i*8 + 24], sizeof(long)*2);
		for( j = 0 ; j < 64; j++ )
		{
			if(j < 32)
				bBitState = ( nValue[0] >> j) & 0x0001;
			else
				bBitState = ( nValue[1] >> j) & 0x0001;
			
			switch(j)
			{
 			case 0:		pRecvIO->bHeartBeat					= bBitState;	break;
 			case 1:		pRecvIO->bPause						= bBitState;	break;	
 			case 2:		pRecvIO->bAbnormal					= bBitState;	break;	
 			case 3:		pRecvIO->bReserved3					= bBitState;	break;	
 			case 4:		pRecvIO->bRecvAble					= bBitState;	break;	
 			case 5:		pRecvIO->bRecvStart					= bBitState;	break;						
 			case 6:		pRecvIO->bRecvComplete				= bBitState;	break;						
 			case 7:		pRecvIO->bImmPauseReq				= bBitState;	break;					
 			case 8:		pRecvIO->bReturnSendStart			= bBitState;	break;						
 			case 9:		pRecvIO->bReturnSendComplete		= bBitState;	break;					
 			case 10:	pRecvIO->bExchangeFlag				= bBitState;	break;	
 			case 11:	pRecvIO->bMultiCarryFlag			= bBitState;	break;
 			case 12:	pRecvIO->bReserved12				= bBitState;	break;	
 			case 13:	pRecvIO->bReserved13				= bBitState;	break;	
			case 14:	pRecvIO->bLoadingStop				= bBitState;	break;
 			case 15:	pRecvIO->bTransferStop				= bBitState;	break;	
 			case 16:	pRecvIO->bPreAction1				= bBitState;	break;
 			case 17:	pRecvIO->bPreAction2				= bBitState;	break;
 			case 18:	pRecvIO->bMidAction1				= bBitState;	break;
 			case 19:	pRecvIO->bMidAction2				= bBitState;	break;
 			case 20:	pRecvIO->bPostAction1				= bBitState;	break;
 			case 21:	pRecvIO->bReserved21				= bBitState;	break;	
 			case 22:	pRecvIO->bReceiveRefuse				= bBitState;	break;	
 			case 23:	pRecvIO->bReserved23				= bBitState;	break;	
 			case 24:	pRecvIO->bReserved24				= bBitState;	break;	
 			case 25:	pRecvIO->bReserved25				= bBitState;	break;
 			case 26:	pRecvIO->bHSResumeReq				= bBitState;	break;	
 			case 27:	pRecvIO->bHSRecoveryAck				= bBitState;	break;	
 			case 28:	pRecvIO->bHSRecoveryNak				= bBitState;	break;	
 			case 29:	pRecvIO->bReserved29				= bBitState;	break;	
 			case 30:	pRecvIO->bHSInitial					= bBitState;	break;	
 			case 31:	pRecvIO->bHSError					= bBitState;	break;	

 			case 32:	pRecvContactPoint->bAbnormal		= bBitState;	break;
 			case 33:	pRecvContactPoint->bTypeofArm		= bBitState;	break;	
 			case 34:	pRecvContactPoint->bTypeofStage		= bBitState;	break;	
 			case 35:	pRecvContactPoint->bManualOp		= bBitState;	break;
 			case 36:	pRecvContactPoint->bSafety			= bBitState;	break;	
 			case 37:	pRecvContactPoint->bEmpty			= bBitState;	break;	
 			case 38:	pRecvContactPoint->bWait			= bBitState;	break;
 			case 39:	pRecvContactPoint->bBusy			= bBitState;	break;
 			case 40:	pRecvContactPoint->bPause			= bBitState;	break;	
 			case 41:	pRecvContactPoint->bReserved9		= bBitState;	break;	
 			case 42:	pRecvContactPoint->bArm1Violate		= bBitState;	break;	
 			case 43:	pRecvContactPoint->bArm2Violate		= bBitState;	break;	
 			case 44:	pRecvContactPoint->bArm1FoldComplete= bBitState;	break;
 			case 45:	pRecvContactPoint->bArm2FoldComplete= bBitState;	break;
 			case 46:	pRecvContactPoint->bArm1GlassCheck	= bBitState;	break;	
 			case 47:	pRecvContactPoint->bArm2GlassCheck	= bBitState;	break;	
 			case 48:	pRecvContactPoint->bRobotDirection	= bBitState;	break;	
 			case 49:	pRecvContactPoint->bReserved17		= bBitState;	break;
 			case 50:	pRecvContactPoint->bReserved18		= bBitState;	break;	
 			case 51:	pRecvContactPoint->bReserved19		= bBitState;	break;	
 			case 52:	pRecvContactPoint->bLiftUp			= bBitState;	break;	
 			case 53:	pRecvContactPoint->bLiftDown		= bBitState;	break;
 			case 54:	pRecvContactPoint->bStopperUp		= bBitState;	break;	
 			case 55:	pRecvContactPoint->bStopperDown		= bBitState;	break;	
 			case 56:	pRecvContactPoint->bDoorOpen		= bBitState;	break;
 			case 57:	pRecvContactPoint->bDoorClose		= bBitState;	break;	
 			case 58:	pRecvContactPoint->bGlassDetect		= bBitState;	break;	
			case 59:	pRecvContactPoint->bBodyMoving		= bBitState;	break;	
 			case 60:	pRecvContactPoint->bBodyOP			= bBitState;	break;	
 			case 61:	pRecvContactPoint->bReserved29		= bBitState;	break;	
 			case 62:	pRecvContactPoint->bReserved30		= bBitState;	break;
 			case 63:	pRecvContactPoint->bReserved31		= bBitState;	break;	
 			}
			
			m_bHSLogUpper2[i][j] = bBitState;
			
			if(m_bHSLogUpper2[i][j] != m_bHSLogUpperOld2[i][j])
			{
				if(j != 0)
				{
					pTransferData = &m_stpModRunInfo->stInGlassData[i];
					m_bHSLogUpperState[i] = TRUE; // Parity Check Skip
					
					AnalizeHSLog(shHSAddr);
				}
				m_bHSLogUpperOld2[i][j] = m_bHSLogUpper2[i][j];
			}	
		}
	}

}


void CBaseModule::UpdateHandShakeInfoToLower(short *pshData, short shAddress)
{
	long i		   = 0;
	long j		   = 0;
	long nValue[2] = { 0x00, };
	bool bBitState = 0;
	short shHSAddr = shAddress;
	
	stGlassSendIOType		*pSendIO			= NULL;
	stContactPointIOType	*pSendContactPoint	= NULL;
	stGlassRecvIOType		*pRecvIO			= NULL;
	stContactPointIOType	*pRecvContactPoint	= NULL;
	stPanelInfoType			*pTransferData		= NULL;
	
	for( i = 0; i < m_stpModCfg->nHandShakeCount; i++ )
	{
		pSendIO = &m_stpModRunInfo->stToLowerIO[i].stSendIO;
		pSendContactPoint= &m_stpModRunInfo->stToLowerIO[i].stSendContactPoint;
		
		pRecvIO = &m_stpModRunInfo->stToLowerIO[i].stRecvIO;
		pRecvContactPoint= &m_stpModRunInfo->stToLowerIO[i].stRecvContactPoint;
		
		if ( m_stpModRunInfo == NULL || pRecvIO == NULL || pRecvContactPoint == NULL 
			|| pSendIO == NULL || pSendContactPoint == NULL )
			continue;
		
		memcpy(&nValue, &pshData[i*8], sizeof(long)*2);
		for( j = 0 ; j < 64; j++ )
		{
			if(j < 32)
				bBitState = ( nValue[0] >> j) & 0x0001;
			else
				bBitState = ( nValue[1] >> j) & 0x0001;
			
			switch(j)
			{
 			case 0:		pSendIO->bHeartBeat					= bBitState;	break;
 			case 1:		pSendIO->bPause						= bBitState;	break;
 			case 2:		pSendIO->bAbnormal					= bBitState;	break;
 			case 3:		pSendIO->bReserved3					= bBitState;	break;
 			case 4:		pSendIO->bSendAble					= bBitState;	break;
 			case 5:		pSendIO->bSendStart					= bBitState;	break;
 			case 6:		pSendIO->bSendComplete				= bBitState;	break;
 			case 7:		pSendIO->bImmPauseReq				= bBitState;	break;
 			case 8:		pSendIO->bReturnRecvStart			= bBitState;	break;
 			case 9:		pSendIO->bReturnRecvComplete		= bBitState;	break;
 			case 10:	pSendIO->bExchangeFlag				= bBitState;	break;
			case 11:	pSendIO->bMultiCarryFlag			= bBitState;	break;
			case 12:	pSendIO->bReserved12				= bBitState;	break;
			case 13:	pSendIO->bReserved13				= bBitState;	break;
			case 14:	pSendIO->bReserved14				= bBitState;	break;
			case 15:	pSendIO->bReserved15				= bBitState;	break;
			case 16:	pSendIO->bPreAction1				= bBitState;	break;
			case 17:	pSendIO->bPreAction2				= bBitState;	break;
			case 18:	pSendIO->bMidAction1				= bBitState;	break;
			case 19:	pSendIO->bMidAction2				= bBitState;	break;
			case 20:	pSendIO->bPostAction1				= bBitState;	break;
			case 21:	pSendIO->bReserved21				= bBitState;	break;
			case 22:	pSendIO->bReserved22				= bBitState;	break;
			case 23:	pSendIO->bReserved23				= bBitState;	break;
			case 24:	pSendIO->bReserved24				= bBitState;	break;
			case 25:	pSendIO->bReserved25				= bBitState;	break;
			case 26:	pSendIO->bHSResumeReq				= bBitState;	break;
			case 27:	pSendIO->bHSRecoveryAck				= bBitState;	break;
			case 28:	pSendIO->bHSRecoveryNak				= bBitState;	break;
			case 29:	pSendIO->bReserved29				= bBitState;	break;
			case 30:	pSendIO->bHSInitial					= bBitState;	break;
			case 31:	pSendIO->bHSError					= bBitState;	break;

			case 32:	pSendContactPoint->bAbnormal		= bBitState;	break;
			case 33:	pSendContactPoint->bTypeofArm		= bBitState;	break;
			case 34:	pSendContactPoint->bTypeofStage		= bBitState;	break;
			case 35:	pSendContactPoint->bManualOp		= bBitState;	break;
			case 36:	pSendContactPoint->bSafety			= bBitState;	break;
			case 37:	pSendContactPoint->bEmpty			= bBitState;	break;
			case 38:	pSendContactPoint->bWait			= bBitState;	break;
			case 39:	pSendContactPoint->bBusy			= bBitState;	break;
			case 40:	pSendContactPoint->bPause			= bBitState;	break;
			case 41:	pSendContactPoint->bReserved9		= bBitState;	break;
			case 42:	pSendContactPoint->bArm1Violate		= bBitState;	break;
			case 43:	pSendContactPoint->bArm2Violate		= bBitState;	break;
			case 44:	pSendContactPoint->bArm1FoldComplete= bBitState;	break;
			case 45:	pSendContactPoint->bArm2FoldComplete= bBitState;	break;
			case 46:	pSendContactPoint->bArm1GlassCheck	= bBitState;	break;
			case 47:	pSendContactPoint->bArm2GlassCheck	= bBitState;	break;
			case 48:	pSendContactPoint->bRobotDirection	= bBitState;	break;
			case 49:	pSendContactPoint->bReserved17		= bBitState;	break;
			case 50:	pSendContactPoint->bReserved18		= bBitState;	break;
			case 51:	pSendContactPoint->bReserved19		= bBitState;	break;
			case 52:	pSendContactPoint->bLiftUp			= bBitState;	break;
			case 53:	pSendContactPoint->bLiftDown		= bBitState;	break;
			case 54:	pSendContactPoint->bStopperUp		= bBitState;	break;
			case 55:	pSendContactPoint->bStopperDown		= bBitState;	break;
			case 56:	pSendContactPoint->bDoorOpen		= bBitState;	break;
			case 57:	pSendContactPoint->bDoorClose		= bBitState;	break;
			case 58:	pSendContactPoint->bGlassDetect		= bBitState;	break;
			case 59:	pSendContactPoint->bBodyMoving		= bBitState;	break;
			case 60:	pSendContactPoint->bBodyOP			= bBitState;	break;
			case 61:	pSendContactPoint->bReserved29		= bBitState;	break;
			case 62:	pSendContactPoint->bReserved30		= bBitState;	break;
			case 63:	pSendContactPoint->bReserved31		= bBitState;	break;
			}
			m_bHSLogLower1[i][j] = bBitState;
			
			if(m_bHSLogLower1[i][j] != m_bHSLogLowerOld1[i][j])
			{
				if(j != 0)
				{
					pTransferData = &m_stpModRunInfo->stOutGlassData[i];
					
					m_bHSLogLowerState[i] = TRUE; // Parity Check Skip
					
					AnalizeHSLog(shHSAddr);
				}
				m_bHSLogLowerOld1[i][j] = m_bHSLogLower1[i][j];
			}
		}
		
		memcpy(&nValue, &pshData[i*8 + 24], sizeof(long)*2);
		for( j = 0 ; j < 64; j++ )
		{
			if(j < 32)
				bBitState = ( nValue[0] >> j) & 0x0001;
			else
				bBitState = ( nValue[1] >> j) & 0x0001;
			
			switch(j)
			{
 			case 0:		pRecvIO->bHeartBeat					= bBitState;	break;
 			case 1:		pRecvIO->bPause						= bBitState;	break;
 			case 2:		pRecvIO->bAbnormal					= bBitState;	break;
 			case 3:		pRecvIO->bReserved3					= bBitState;	break;
 			case 4:		pRecvIO->bRecvAble					= bBitState;	break;
 			case 5:		pRecvIO->bRecvStart					= bBitState;	break;
 			case 6:		pRecvIO->bRecvComplete				= bBitState;	break;
			case 7:		pRecvIO->bImmPauseReq				= bBitState;	break;
			case 8:		pRecvIO->bReturnSendStart			= bBitState;	break;
			case 9:		pRecvIO->bReturnSendComplete		= bBitState;	break;
			case 10:	pRecvIO->bExchangeFlag				= bBitState;	break;
			case 11:	pRecvIO->bMultiCarryFlag			= bBitState;	break;
			case 12:	pRecvIO->bReserved12				= bBitState;	break;
			case 13:	pRecvIO->bReserved13				= bBitState;	break;
			case 14:	pRecvIO->bLoadingStop				= bBitState;	break;
			case 15:	pRecvIO->bTransferStop				= bBitState;	break;
			case 16:	pRecvIO->bPreAction1				= bBitState;	break;
			case 17:	pRecvIO->bPreAction2				= bBitState;	break;
			case 18:	pRecvIO->bMidAction1				= bBitState;	break;
			case 19:	pRecvIO->bMidAction2				= bBitState;	break;
			case 20:	pRecvIO->bPostAction1				= bBitState;	break;
			case 21:	pRecvIO->bReserved21				= bBitState;	break;
			case 22:	pRecvIO->bReceiveRefuse				= bBitState;	break;
			case 23:	pRecvIO->bReserved23				= bBitState;	break;
			case 24:	pRecvIO->bReserved24				= bBitState;	break;
			case 25:	pRecvIO->bReserved25				= bBitState;	break;
			case 26:	pRecvIO->bHSResumeReq				= bBitState;	break;
			case 27:	pRecvIO->bHSRecoveryAck				= bBitState;	break;
			case 28:	pRecvIO->bHSRecoveryNak				= bBitState;	break;
			case 29:	pRecvIO->bReserved29				= bBitState;	break;
			case 30:	pRecvIO->bHSInitial					= bBitState;	break;
			case 31:	pRecvIO->bHSError					= bBitState;	break;

			case 32:	pRecvContactPoint->bAbnormal		= bBitState;	break;
			case 33:	pRecvContactPoint->bTypeofArm		= bBitState;	break;
			case 34:	pRecvContactPoint->bTypeofStage		= bBitState;	break;
			case 35:	pRecvContactPoint->bManualOp		= bBitState;	break;
			case 36:	pRecvContactPoint->bSafety			= bBitState;	break;
			case 37:	pRecvContactPoint->bEmpty			= bBitState;	break;
			case 38:	pRecvContactPoint->bWait			= bBitState;	break;
			case 39:	pRecvContactPoint->bBusy			= bBitState;	break;
			case 40:	pRecvContactPoint->bPause			= bBitState;	break;
			case 41:	pRecvContactPoint->bReserved9		= bBitState;	break;
			case 42:	pRecvContactPoint->bArm1Violate		= bBitState;	break;
			case 43:	pRecvContactPoint->bArm2Violate		= bBitState;	break;
			case 44:	pRecvContactPoint->bArm1FoldComplete= bBitState;	break;
			case 45:	pRecvContactPoint->bArm2FoldComplete= bBitState;	break;
			case 46:	pRecvContactPoint->bArm1GlassCheck	= bBitState;	break;
			case 47:	pRecvContactPoint->bArm2GlassCheck	= bBitState;	break;
			case 48:	pRecvContactPoint->bRobotDirection	= bBitState;	break;
			case 49:	pRecvContactPoint->bReserved17		= bBitState;	break;
			case 50:	pRecvContactPoint->bReserved18		= bBitState;	break;
			case 51:	pRecvContactPoint->bReserved19		= bBitState;	break;
			case 52:	pRecvContactPoint->bLiftUp			= bBitState;	break;
			case 53:	pRecvContactPoint->bLiftDown		= bBitState;	break;
			case 54:	pRecvContactPoint->bStopperUp		= bBitState;	break;
			case 55:	pRecvContactPoint->bStopperDown		= bBitState;	break;
			case 56:	pRecvContactPoint->bDoorOpen		= bBitState;	break;
			case 57:	pRecvContactPoint->bDoorClose		= bBitState;	break;
			case 58:	pRecvContactPoint->bGlassDetect		= bBitState;	break;
			case 59:	pRecvContactPoint->bBodyMoving		= bBitState;	break;
			case 60:	pRecvContactPoint->bBodyOP			= bBitState;	break;
			case 61:	pRecvContactPoint->bReserved29		= bBitState;	break;
			case 62:	pRecvContactPoint->bReserved30		= bBitState;	break;
			case 63:	pRecvContactPoint->bReserved31		= bBitState;	break;
			}
			m_bHSLogLower2[i][j] = bBitState;
			
			if(m_bHSLogLower2[i][j] != m_bHSLogLowerOld2[i][j])
			{
				if(j != 0) 
				{
					pTransferData = &m_stpModRunInfo->stOutGlassData[i];
					
					m_bHSLogLowerState[i] = TRUE; // Parity Check Skip
					AnalizeHSLog(shHSAddr);
				}
				m_bHSLogLowerOld2[i][j] = m_bHSLogLower2[i][j];
			}
		}
	}
}

void CBaseModule::AnalizeHSLog(short shAddr)
{
	int i=0;
	int j=0;
	int nSide = 0;
	long nLen;
	short shHSStartAddr = 0;
	char chHEXAddr[2] ={0,};
	
	char chHead[128] = {NULL,};
	char chHSSide[10] = {NULL,};
	char chHSData[2048] = {NULL,};
	char chHSUpper1[256] = {NULL,};
	char chHSUpper2[256] = {NULL,};
	char chHPanelID[MAX_PANEL_ID_LEN] = {NULL,};
	
	BOOL bHSValue1[3][100] = {FALSE,};
	BOOL bHSValue2[3][100] = {FALSE,};

	SYSTEMTIME systime;
	GetLocalTime(&systime);

	WORD	wYear	=	systime.wYear;
	WORD	wMonth	=	systime.wMonth;
	WORD	wDay	=	systime.wDay;
	WORD	wHour	=	systime.wHour;
	WORD	wMin	=	systime.wMinute;
	WORD	wSec	=	systime.wSecond;
	WORD	wMMSec	=	systime.wMilliseconds;

	char	buff[128] = { NULL, };
	sprintf(buff, "[%02d:%02d:%02d.%03d]", wHour, wMin, wSec, wMMSec);

	//for(i=0;i<3;i++)
	for(i=0;i<2;i++)
	{
		if(m_bProgramFirstStart1[i] && m_bHSLogUpperState[i]) 
		{
			m_bProgramFirstStart1[i] = FALSE;
			m_bHSLogUpperState[i] = FALSE;
			return;
		}
		else if(m_bProgramFirstStart2[i] && m_bHSLogLowerState[i]) 
		{
			m_bProgramFirstStart2[i] = FALSE;
			m_bHSLogLowerState[i] = FALSE;
			return;
		}
		else if(m_bHSLogUpperState[i])
		{
			nSide = i;
			shHSStartAddr = shAddr + B_EACH_HS_INTERVAL*i;
			strcpy(chHSSide,"UPPER");
		}
		else if(m_bHSLogLowerState[i])
		{
			nSide = i;
			shHSStartAddr = shAddr + B_EACH_HS_INTERVAL*i + B_EACH_UP_TO_LOWER_INTERVAL;
			strcpy(chHSSide,"LOWER");
		}
	}
	
	stPanelInfoType			*pTransferData	= NULL;
	
	for(i=0;i<2;i++)
	{
		if(m_bHSLogUpperState[i])
		{
			pTransferData = &m_stpModRunInfo->stInGlassData[i];
		}
		else if(m_bHSLogLowerState[i])
		{
			pTransferData = &m_stpModRunInfo->stOutGlassData[i];
		}
		if(pTransferData != NULL)
		{
			strcpy(chHPanelID, pTransferData->szHPanelID);
			strcpy(m_szMSCHPanelID,pTransferData->szHPanelID);
		}
	}
	
//	sprintf(chHSData,"\r\n[MODULE ID:%d(%s)][HPanelID:%s][Started Address:0x%x]",
//		m_stpModCfg->nModuleID,m_stpModCfg->szModuleName,chHPanelID,shHSStartAddr);

	sprintf(chHead,"[MODULE ID:%d(%s)][HPanelID:%s]",
		m_stpModCfg->nModuleID,m_stpModCfg->szModuleName,chHPanelID);


/*	
	for(i=0;i<64;i++)
	{
		itoa(i,chHEXAddr,16);
		if(i==0) sprintf(chHSUpper1,"\r\n%s%d][ADDR]%s,%2s",chHSSide,nSide+1,buff,chHEXAddr);
		else if(i == 63) sprintf(chHSUpper1,",%2s\r\n",chHEXAddr);
		else sprintf(chHSUpper1,",%2s",chHEXAddr);
		strcat(chHSData,chHSUpper1);
	}
*/	
	for(j=0;j<64;j++)
	{
		if(m_bHSLogUpperState[nSide] == TRUE)
			bHSValue1[nSide][j] = m_bHSLogUpper1[nSide][j];
		else if(m_bHSLogLowerState[nSide] == TRUE)
			bHSValue1[nSide][j] = m_bHSLogLower1[nSide][j];
		
		if(j == 0)
			sprintf(chHSUpper1,"%s[%s%d][SEND]%s,%2d",chHead,chHSSide,nSide+1,buff,bHSValue1[nSide][j]);
		else if(j == 63)
			sprintf(chHSUpper1,",%2d\r\n",bHSValue1[nSide][j]);
		else
			sprintf(chHSUpper1,",%2d",bHSValue1[nSide][j]);
		strcat(chHSData,chHSUpper1);
		
	}
	nLen = strlen(chHSData);
	for(j=0;j<64;j++)
	{
		if(m_bHSLogUpperState[nSide] == TRUE)
			bHSValue2[nSide][j] = m_bHSLogUpper2[nSide][j];
		else if(m_bHSLogLowerState[nSide] == TRUE)
			bHSValue2[nSide][j] = m_bHSLogLower2[nSide][j];
		
		if(j == 0)
			sprintf(chHSUpper2,"%s[%s%d][RECV]%s,%2d",chHead,chHSSide,nSide+1,buff,bHSValue2[nSide][j]);
		else if(j == 63)
			sprintf(chHSUpper2,",%2d\r\n",bHSValue2[nSide][j]);
		else 
			sprintf(chHSUpper2,",%2d",bHSValue2[nSide][j]);
		strcat(chHSData,chHSUpper2);
	}
	
	nLen = strlen(chHSData);
	
	if(m_bHSLogUpperState[nSide])
	{
		m_pParent->m_Log_HandShake[(m_stpModCfg->nModuleID - 2) * MAX_LOG_HS_KIND].AddLog(chHSData);
		m_bHSLogUpperState[nSide] = FALSE;
	}
	if(m_bHSLogLowerState[nSide])
	{
		m_pParent->m_Log_HandShake[(m_stpModCfg->nModuleID - 2) * MAX_LOG_HS_KIND + 1].AddLog(chHSData);
		m_bHSLogLowerState[nSide] = FALSE;
	}
}

void CBaseModule::WriteActionLog(long nModuleID, long nUnitID, BOOL bHS, BOOL bBitState, long nBitNo, long nFromPos, long nToPos, long nSemesUniqID)
{

	char szActionData[256] = {NULL,};					
	char szDate[MAX_DATE_LEN+1] = {NULL,};				// wYear, wMonth, wDay //8 
	char szTime[MAX_OCCUR_TIME_LEN+1] = {NULL,};		// wHour, wMin, wSec, wMMSec //12
	char szModuleID[MAX_MODULE_LEN+1] = {NULL,};		//8
	char szLogType[MAX_LOG_TYPE_LEN	+1] = {NULL,};		//4
	char szActionID[MAX_ACTION_ID_LEN+1] = {NULL,};		//8
	char szFromPosition[MAX_FROM_POSITION_LEN+1] = {NULL,};//8
	char szToPosition[MAX_TO_POSITION_LEN+1] = {NULL,};//8
	char szPPID[MAX_PPID_LEN+1] = {NULL,};//16
	char szHPanelID[MAX_PANEL_ID_LEN+1] = {NULL,};//12
	char szUnitName[MAX_UNIT_ID_LEN+1] = {NULL,};		//4
	char szFromPosUnitName[MAX_UNIT_ID_LEN+1] = {NULL,};//4
	char szToPosUnitName[MAX_UNIT_ID_LEN+1] = {NULL,};//4
	long nUnitCount = 0;

	stSystemRunInfoType *pSysRunInfo = GetSysRunInfo();

	SYSTEMTIME	systime;
	GetLocalTime(&systime);

	WORD	wYear	= systime.wYear;
	WORD	wMonth	= systime.wMonth;
	WORD	wDay	= systime.wDay;
	WORD	wHour	= systime.wHour;
	WORD	wMin	= systime.wMinute;
	WORD	wSec	= systime.wSecond;
	WORD	wMMSec	= systime.wMilliseconds;
	
	//발생날짜
	sprintf(szDate, "%04d%02d%02d", wYear, wMonth, wDay);
	strcpy(szActionData, szDate);

	//발생시간
	sprintf(szTime, ",%02d:%02d:%02d.%03d",  wHour, wMin, wSec, wMMSec);
	strcat(szActionData, szTime);

	//ModuleID (ex.DV01EN01)
	memcpy(szUnitName, m_stpModCfg->stUnitCfg[nUnitID].szUnitName, MAX_UNIT_ID_LEN);
	sprintf(szModuleID, ",%s%s",m_stpModCfg->szModuleName, szUnitName);
	strcat(szActionData, szModuleID);

	//Log Type
	if( !bHS )
		sprintf(szLogType, ",%s", bBitState ? "ACTS": "ACTE");
	else 
		sprintf(szLogType, ",%s", bBitState ? "CMDS": "CMDE");
	strcat(szActionData, szLogType);
	
	/*
	switch(m_stpModCfg->stUnitCfg[nUnitID].nUnitType)
	{
	case eUnitType_Robot:
		sprintf(szActionID, ",%s", GetActionID(eActionLog_Robot, nBitNo));
		break;

	case eUnitType_HP:
	case eUnitType_CP:
	case eUnitType_COATER:
	case eUnitType_VAC_DRYER:
		sprintf(szActionID, ",%s", GetActionID(eActionLog_Stage, nBitNo));
		break;

	default:
		sprintf(szActionID, ",%s", GetActionID(eActionLog_CV, nBitNo));
		break;
	}
	*/
	sprintf(szActionID, ",%s", GetActionID(eActionLog_CV, nBitNo));
	strcat(szActionData, szActionID);

	//From Position
	if (51 == nFromPos)		// nFromPos가 "51"이면 From Position이 상류 설비 
	{
		/*
		nUnitCount = m_stpSma->stLayOutCfg.stModCfg[nModuleID-1].nUnitCount;
		memcpy(szFromPosUnitName, m_stpSma->stLayOutCfg.stModCfg[nModuleID-1].stUnitCfg[nUnitCount-1].szUnitName, MAX_UNIT_ID_LEN);
		//if (strncmp(szFromPosUnitName , "" ,MAX_UNIT_ID_LEN) == 0)
		//	sprintf(szFromPosUnitName , "");//sprintf(szFromPosUnitName , "TM01");

		sprintf(szFromPosition, ",%s%s", m_stpSma->stLayOutCfg.stModCfg[nModuleID-1].szModuleName, szFromPosUnitName);
		strcat(szActionData, szFromPosition);
		*/

		

		sprintf(szFromPosition, ",UPPER   ");
		strcat(szActionData, szFromPosition);
	}
	else
	{
		//sprintf(szFromPosition, ",%02d", nFromPos);
		sprintf(szFromPosition, ",%s", m_stpSma->stLayOutCfg.stModCfg[nModuleID].stUnitCfg[nFromPos].szUnitName);
		szFromPosition[4] = ' '; 
		szFromPosition[5] = ' '; 
		szFromPosition[6] = ' '; 
		szFromPosition[7] = ' '; 
		szFromPosition[8] = NULL; 
		strcat(szActionData, szFromPosition);
		/*
		sprintf(szFromPosition, ",%s", m_stpModCfg->szModuleName);
		strcat(szActionData, szFromPosition);
		strcat(szActionData, GetDirectionUnitName(nFromPos));
		*/
	}

	//To Position
	if (61 == nToPos)		// nToPos가 "61"이면 To Position이 하류 설비 
	{
		/*
		nUnitCount = m_stpSma->stLayOutCfg.stModCfg[nModuleID+1].nUnitCount;
		memcpy(szToPosUnitName, m_stpSma->stLayOutCfg.stModCfg[nModuleID+1].stUnitCfg[0].szUnitName, MAX_UNIT_ID_LEN);
		//if (strncmp(szToPosUnitName , "" ,MAX_UNIT_ID_LEN) == 0)
		//	sprintf(szToPosUnitName , ""); // "TM01");

		sprintf(szToPosition, ",%s%s", m_stpSma->stLayOutCfg.stModCfg[nModuleID+1].szModuleName, szToPosUnitName);
		strcat(szActionData, szToPosition);
		*/
		sprintf(szToPosition, ",LOWER   ");
		strcat(szActionData, szToPosition);
	}
	else
	{
		/*
		sprintf(szToPosition, ",%s", m_stpModCfg->szModuleName);
		strcat(szActionData, szToPosition);
		strcat(szActionData, GetDirectionUnitName(nToPos));
		*/
		sprintf(szToPosition, ",%s", m_stpSma->stLayOutCfg.stModCfg[nModuleID].stUnitCfg[nToPos].szUnitName);
		szToPosition[4] = ' '; 
		szToPosition[5] = ' '; 
		szToPosition[6] = ' '; 
		szToPosition[7] = ' '; 
		szToPosition[8] = NULL; 
		strcat(szActionData, szToPosition);
	}

	if (nSemesUniqID < 1 || nSemesUniqID > 255)
	{
		sprintf(szPPID, ",%16s", "-");
		sprintf(szHPanelID, ",%12s\r\n", "-");
	}
	else
	{	
		sprintf(szPPID, ",%16s", pSysRunInfo->stUniqueIDTable[nSemesUniqID-1].stUniqueIDPanelInfo.szPPID);
		sprintf(szHPanelID, ",%12s\r\n", pSysRunInfo->stUniqueIDTable[nSemesUniqID-1].stUniqueIDPanelInfo.szHPanelID);
	} 
	
	strcat(szActionData, szPPID);
	strcat(szActionData, szHPanelID);
	
	m_pParent->m_ActionLog[nModuleID].SetMSCPrefixName(m_stpSma->stLayOutCfg.szEQPID, m_stpModCfg->szModuleName);
	m_pParent->m_ActionLog[nModuleID].AddLog(szActionData);
}

char* CBaseModule::GetActionID(long nUnitType, long nActionID)
{
	char* szActionID[3][48] =
	{
		{
			//CV
			"GLASS_I ", "GLASS_O ", "LIFT_U  ",	"LIFT_D  ",	"PIN_U   ",	"PIN_D   ",	"SHUT_O  ",	"SHUT_C  ",
			"RUN     ",	"ALIGN_O ",	"ALIGN_C ",	"SPARE_1 ",	"SPARE_2 ",	"SPARE_3 ",	"SPARE_4 ",	"SPARE_5 ",
			"RECV_A1 ", "RECV_S1 ", "RECV_C1 ", "SEND_A1 ", "SEND_S1 ", "SEND_C1 ", "RECV_A2 ", "RECV_S2 ",
			"RECV_C2 ", "SEND_A2 ", "SEND_S2 ", "SEND_C2 ", "SPARE_1 ", "SPARE_2 ", "SPARE_3 ", "SPARE_4 ",
			"RECV_A3 ", "RECV_S3 ", "RECV_C3 ", "SEND_A3 ", "SEND_S3 ", "SEND_C3 ", "SPARE_1 ", "SPARE_2 ",
			"SPARE_3 ", "SPARE_4 ", "SPARE_5 ", "SPARE_6 ", "SPARE_7 ", "SPARE_8 ", "SPARE_9 ", "SPARE_10"
		},
		{
			//ROBOT
			"GET     ", "PUT     ",	"MOVE_G  ",	"MOVE_P  ", "EXCHANGE", "SPARE_1 ",	"SPARE_2 ",	"SPARE_3 ",
			"STOP    ",	"SPARE_4 ",	"SPARE_5 ",	"SPARE_6 ",	"SPARE_7 ",	"SPARE_8 ",	"SPARE_9 ",	"SPARE_10",
			"RECV_A1 ", "RECV_S1 ", "RECV_C1 ", "SEND_A1 ", "SEND_S1 ", "SEND_C1 ", "RECV_A2 ", "RECV_S2 ",
			"RECV_C2 ", "SEND_A2 ", "SEND_S2 ", "SEND_C2 ", "SPARE_1 ", "SPARE_2 ", "SPARE_3 ", "SPARE_4 ",
			"RECV_A3 ", "RECV_S3 ", "RECV_C3 ", "SEND_A3 ", "SEND_S3 ", "SEND_C3 ", "SPARE_1 ", "SPARE_2 ",
			"SPARE_3 ", "SPARE_4 ", "SPARE_5 ", "SPARE_6 ", "SPARE_7 ", "SPARE_8 ", "SPARE_9 ", "SPARE_10"
				
		},
		{
			//Stage: HP, CP, T/T, Coater, VCD
			"GLASS_I ",	"GLASS_O ",	"LIFT_U  ", "LIFT_D  ",	"PIN_U   ",	"PIN_D   ", "SHUT_O  ",	"SHUT_C  ",
			"RUN     ",	"ALIGN_O ",	"ALIGN_C ",	"ROTATE_1",	"ROTATE_2",	"ROTATE_3",	"ROTATE_4",	"SPARE_5 ",
			"RECV_A1 ", "RECV_S1 ", "RECV_C1 ", "SEND_A1 ", "SEND_S1 ", "SEND_C1 ", "RECV_A2 ", "RECV_S2 ",
			"RECV_C2 ", "SEND_A2 ", "SEND_S2 ", "SEND_C2 ", "SPARE_1 ", "SPARE_2 ", "SPARE_3 ", "SPARE_4 ",
			"RECV_A3 ", "RECV_S3 ", "RECV_C3 ", "SEND_A3 ", "SEND_S3 ", "SEND_C3 ", "SPARE_1 ", "SPARE_2 ",
			"SPARE_3 ", "SPARE_4 ", "SPARE_5 ", "SPARE_6 ", "SPARE_7 ", "SPARE_8 ", "SPARE_9 ", "SPARE_10"
		}
	};
	
	return szActionID[nUnitType][nActionID];
}

//From Position or To Position을 이용하여 UnitName 얻는다.
char* CBaseModule::GetDirectionUnitName(long nUnitPos)
{
	long nUnitNo = 0;
	char szUnitName[MAX_UNIT_ID_LEN+1] = {NULL,};
	
	for ( nUnitNo = 0; m_stpModCfg->nUnitCount ; nUnitNo++)
	{
		if (nUnitPos == m_stpModCfg->stUnitCfg[nUnitNo].nPosiNo)
			return  strncpy(szUnitName, m_stpModCfg->stUnitCfg[nUnitNo].szUnitName, MAX_UNIT_ID_LEN);
	}
	
	return NULL;
}



void CBaseModule::GetHandShakeIO()
{
	short   shReadData[96]	= { 0x00, };
	long	nReadDataSize	= sizeof(short) * 24;
	short	shAddr			= 0;
	short	shHSAddr		= 0; // HandShake Log용
		
	long	nStationNo = m_stpModCfg->stMelsecCfg[0].nMelStationNo;

	if ( nStationNo > 1 )
		shAddr = B_L2_FROM_UPPER1_HS + B_EACH_LAYER_INLINE_INTERVAL * (nStationNo - 2);
	else
		return;
	
	shHSAddr = shAddr; // HandShake Log용
	
	//	From Upper Handshake Packet
	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevB, shAddr, &nReadDataSize, &shReadData[0]);
	shAddr += B_EACH_STATE_INTERVAL;
	
	//	To Upper Handshake Packet
	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevB, shAddr, &nReadDataSize, &shReadData[24]);
	shAddr += B_EACH_STATE_INTERVAL;
	
	//	To Lower Handshake Packet
	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevB, shAddr, &nReadDataSize, &shReadData[48]);
	shAddr += B_EACH_STATE_INTERVAL;
	
	//	From Lower Handshake Packet
	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevB, shAddr, &nReadDataSize, &shReadData[72]);
	
	// 각 Module 별 Handshake IO 정보 SMA Update
	UpdateHandShakeInfoToUpper(&shReadData[0],shHSAddr);
	UpdateHandShakeInfoToLower(&shReadData[48],shHSAddr);
}

//	각 Local의 Status를 SMA에 Update : Word Data
void CBaseModule::UpdateEquipmentDataToSMA(short *pshData)
{
	long i		= 0;
	long nIdx	= 0;
	long nLen	= 0;
	BOOL bBitState = FALSE;
	char szNullRefuseCode[3];
	memset(szNullRefuseCode, 0x00, 3);
	
	stPanelInfoType					*pTransferData	= NULL;
	stPreViewRunDataType			*pEQState		= NULL;
	stPreViewRunGlassSummaryType	*pSummaryData	= NULL;
	stPreViewRunGlassSummaryDataType *pBatchData	= NULL; 
	
	//	From Upper Glass Data Packet
	for( i = 0; i < MAX_SUMMARY_DATA_COUNT; i++ )
	{
		pTransferData = &m_stpModRunInfo->stInGlassData[i];
		ConvertTransferDataToPanelData(pTransferData, &pshData[nIdx]);	
		nIdx += 180;
	}

	nIdx = 180 * MAX_SUMMARY_DATA_COUNT;
	
	//	To Upper Data Packet
	for( i = 0; i < m_stpModCfg->nHandShakeCount; i++ )
	{
		memcpy(m_stpModRunInfo->stToUpperData[i].szPostActData, (pshData+nIdx), MAX_POST_ACT_DATA_LEN );
		nIdx += 8;
		
		memcpy(m_stpModRunInfo->stToUpperData[i].szRefuseCode, (pshData+nIdx), MAX_REFUSE_CODE_LEN );	
		nIdx ++;
		
		nIdx += 3;		//Reserved
		
		m_stpModRunInfo->stToUpperData[i].nCSIF = *(pshData+nIdx);
		nIdx += 4;
	}

	nIdx = (180 * MAX_SUMMARY_DATA_COUNT) + (16 * MAX_SUMMARY_DATA_COUNT);
	
	//	To Lower Glass Data Packet
	for( i = 0; i < MAX_SUMMARY_DATA_COUNT; i++ )
	{
		pTransferData = &m_stpModRunInfo->stOutGlassData[i];
		ConvertTransferDataToPanelData(pTransferData, &pshData[nIdx]);	
		nIdx += 180;
	}
	nIdx = (180 * MAX_SUMMARY_DATA_COUNT * 2) + (16 * MAX_SUMMARY_DATA_COUNT);
	
	//	Equipment state Data Packet
	pEQState = &m_stpModRunInfo->stPreViewRunData;
	
	memcpy(pEQState->szPPID,	(pshData+nIdx), MAX_PPID_LEN);	nIdx += 8;
	pEQState->nGlassSize[0]		= *(pshData+nIdx);				nIdx++;
	pEQState->nGlassSize[1]		= *(pshData+nIdx);				nIdx++;
	pEQState->nGlassThickness	= *(pshData+nIdx);				nIdx += 2;
	pEQState->nSetTactTime		= *(pshData+nIdx);				nIdx++;
	pEQState->nCurTactTime		= *(pshData+nIdx);				nIdx++;
	pEQState->nReqGlassType		= *(pshData+nIdx);				nIdx++;
	pEQState->nReqGlassCount	= *(pshData+nIdx);				nIdx++;
	memcpy(pEQState->szEOMode,	(pshData+nIdx), sizeof(short));	nIdx++;
	memcpy(pEQState->szOPMode,	(pshData+nIdx), sizeof(short));	nIdx++;
	
	//Eqp Status's Bits Signal
	for( i = 0 ; i < 32; i++ )
	{
		bBitState = ( *(pshData+nIdx) >> i) & 0x0001;
		switch(i)
		{
		case 0:		pEQState->stEQState.nEQState_Normal			= bBitState;	break;
		case 1:		pEQState->stEQState.nEQState_Fault			= bBitState;	break;
		case 2:		pEQState->stEQState.nEQState_PM				= bBitState;	break;
		case 3:		pEQState->stEQState.nEQState_Reserved[0]	= bBitState;	break;
		case 4:		pEQState->stEQState.nEQState_Reserved[1]	= bBitState;	break;
		case 5:		pEQState->stEQState.nEQState_Reserved[2]	= bBitState;	break;
		case 6:		pEQState->stEQState.nEQState_Reserved[3]	= bBitState;	break;
		case 7:		pEQState->stEQState.nEQState_Reserved[4]	= bBitState;	break;
		case 8:		pEQState->stEQState.nProcState_Init			= bBitState;	break;
		case 9:		pEQState->stEQState.nProcState_Idle			= bBitState;	break;
		case 10:	pEQState->stEQState.nProcState_Setup		= bBitState;	break;
		case 11:	pEQState->stEQState.nProcState_Ready		= bBitState;	break;
		case 12:	pEQState->stEQState.nProcState_Execute		= bBitState;	break;
		case 13:	pEQState->stEQState.nProcState_Pause		= bBitState;	break;
		case 14:	pEQState->stEQState.nProcState_Reserved[0]	= bBitState;	break;
		case 15:	pEQState->stEQState.nProcState_Reserved[1]	= bBitState;	break;
		case 16:	pEQState->stEQState.nOther_Reserved[0]		= bBitState;	break;
		case 17:	pEQState->stEQState.nOther_Reserved[1]		= bBitState;	break;
		case 18:	pEQState->stEQState.nOther_Reserved[2]		= bBitState;	break;
		case 19:	pEQState->stEQState.nOther_Reserved[3]		= bBitState;	break;
		case 20:	pEQState->stEQState.nOther_Reserved[4]		= bBitState;	break;
		case 21:	pEQState->stEQState.nOther_Reserved[5]		= bBitState;	break;
		case 22:	pEQState->stEQState.nOther_Reserved[6]		= bBitState;	break;
		case 23:	pEQState->stEQState.nOther_Reserved[7]		= bBitState;	break;
		case 24:	pEQState->stEQState.nOther_Reserved[8]		= bBitState;	break;
		case 25:	pEQState->stEQState.nOther_Reserved[9]		= bBitState;	break;
		case 26:	pEQState->stEQState.nOther_Reserved[10]		= bBitState;	break;
		case 27:	pEQState->stEQState.nOther_Reserved[11]		= bBitState;	break;
		case 28:	pEQState->stEQState.nOther_Reserved[12]		= bBitState;	break;
		case 29:	pEQState->stEQState.nOther_Reserved[13]		= bBitState;	break;
		case 30:	pEQState->stEQState.nOther_Reserved[14]		= bBitState;	break;
		case 31:	pEQState->stEQState.nOther_Reserved[15]		= bBitState;	break;
		}
	}
}

void CBaseModule::GetStatusData()
{
	short   shReadData[2]	= { 0x00, };
	long	nReadDataSize	= sizeof(short)*2;
	short	shAddr			= 0;

	short   shReadCodeData[4] =	{ 0x00, };
	long	nReadCodeDataSize = sizeof(short)*4;
	short	shCodeAddr		  = 0;
	
	GetLocalDevBWAddr(eGetStatus, m_stpModCfg->nModuleID, 0, W_L2_EQ_STATE);
	shAddr = m_ShCommData_Local[eGetStatus]->m_shDataAddr;
	
	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, shAddr, &nReadDataSize, shReadData);
	
	if(m_stpSma->stLayOutCfg.nEQType == eEQType_ETCHSTRIP)
	{
		if(m_stpSma->stLayOutCfg.nModuleCount > 2)
		{
			switch(m_stpModCfg->nModuleID)
			{
			case eModuleType_Bypass:
				m_stpModRunInfo->nModuleID = m_stpModCfg->nModuleID;
				m_stpModRunInfo->nEQState = (long) shReadData[2];	
				m_stpModRunInfo->nProcState = (long) shReadData[3];	
				break;
			case eModuleType_Buffer:
				m_stpModRunInfo->nModuleID = m_stpModCfg->nModuleID;
				m_stpModRunInfo->nEQState = (long) shReadData[4];	
				m_stpModRunInfo->nProcState = (long) shReadData[5];	
				break;
			default:
				m_stpModRunInfo->nModuleID = m_stpModCfg->nModuleID;
				m_stpModRunInfo->nEQState = (long) shReadData[0];	
				m_stpModRunInfo->nProcState = (long) shReadData[1];	
				break;
			}
		}
	}
	else
	{
		m_stpModRunInfo->nModuleID = m_stpModCfg->nModuleID;
		m_stpModRunInfo->nEQState = (long) shReadData[0];
		m_stpModRunInfo->nProcState = (long) shReadData[1];
		m_stpModRunInfo->nBypassEQState = (long) shReadData[2];
		m_stpModRunInfo->nBypassProcState = (long) shReadData[3];
		m_stpModRunInfo->nBufferEQState = (long) shReadData[4];
		m_stpModRunInfo->nBufferProcState = (long) shReadData[5];
	}

	// Pause/PM Code
	GetLocalDevBWAddr(eGetStatusCode, m_stpModCfg->nModuleID, 0, W_L2_PM_CODE);
	shCodeAddr = m_ShCommData_Local[eGetStatusCode]->m_shDataAddr;
	
	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, shCodeAddr, &nReadCodeDataSize, shReadCodeData);
	
	memcpy(m_stpModRunInfo->szPMCode, &shReadCodeData[0], MAX_PM_CODE_LEN);
	memcpy(m_stpModRunInfo->szPauseCode, &shReadCodeData[2], MAX_PAUSE_CODE_LEN);
	
	
	//--------------------------------------------------------------------
	// PM Mode 전환 및 해제시 Log 남김
	if( bPMCheck[m_stpModCfg->nModuleID] == true && bPMLog_Set[m_stpModCfg->nModuleID] == true && m_stpModRunInfo->nEQState != eEQState_PM )
	{
		m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] PM Treated : PM Code [%s]" , 
			m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, m_stpModRunInfo->szPMCode);
	}
	
	if( m_stpModRunInfo->nEQState == eEQState_PM ) 
		bPMCheck[m_stpModCfg->nModuleID] = true;
	else
	{
		bPMCheck[m_stpModCfg->nModuleID] = false;
		bPMLog_Set[m_stpModCfg->nModuleID] = false;
	}
	
	if( bPMCheck[m_stpModCfg->nModuleID] == true && bPMLog_Set[m_stpModCfg->nModuleID] == false )
	{
		bPMLog_Set[m_stpModCfg->nModuleID] = true;
		m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] PM Occured : PM Code [%s]" , 
			m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, m_stpModRunInfo->szPMCode);
	}
	//--------------------------------------------------------------------
}

//	Read each station state from Melsec Network
void CBaseModule::GetEquipmentData()
{
	short   shReadData[1500]	= { 0x00, };
	long	nReadDataSize	= 0;
	long	shAddr			= 0;
	long	shGapAddr		= 0;	
	long	nStationNo		= m_stpModCfg->stMelsecCfg[0].nMelStationNo;

	if ( nStationNo > 1 )
		shGapAddr = W_EACH_LAYER_INLINE_INTERVAL * (nStationNo - 2);
	else
		return;
	
	//	To Lower Data Packet : Transfer Glass Data
	shAddr = W_L2_FROM_UPPER1_GLASS_DATA + shGapAddr;
	nReadDataSize = sizeof(short)*180 * MAX_SUMMARY_DATA_COUNT;			// From Upper Data : Max 3개
	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, shAddr, &nReadDataSize, &shReadData[0]);
	
	//	To Upper Data Packet
	shAddr = W_L2_TO_UPPER1 + shGapAddr;
	nReadDataSize = sizeof(short)*16 * MAX_SUMMARY_DATA_COUNT;			// To Upper Data : Max 3개
	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, shAddr, &nReadDataSize, &shReadData[180 * MAX_SUMMARY_DATA_COUNT]);
	
	//	To Lower Data Packet : Transfer Glass Data
	shAddr = W_L2_TO_LOWER1_GLASS_DATA + shGapAddr;
	nReadDataSize = sizeof(short)*180 * MAX_SUMMARY_DATA_COUNT;			// To Lower Data : Max 3개
	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, shAddr, &nReadDataSize, &shReadData[180 * MAX_SUMMARY_DATA_COUNT + 16 * MAX_SUMMARY_DATA_COUNT]);
	
	//	Equipment state Data Packet
	shAddr = W_L2_OWN_EQ_STATE + shGapAddr;
	nReadDataSize = sizeof(short)*32;
	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, shAddr, &nReadDataSize, &shReadData[180 * MAX_SUMMARY_DATA_COUNT * 2 + 16 * MAX_SUMMARY_DATA_COUNT]);
	
	//	Panel Data Upate To SMA
	UpdateEquipmentDataToSMA(shReadData);
}

////////////////////////////////////////////////////
//	Data Net Related
void CBaseModule::UpdateDataLink_OpModeIOToSMA(short shData)
{
	BOOL	bState = FALSE;
	for( long i = 0; i < 16; i++ )
	{
		bState = (shData >> i) & 0x0001;
		switch(i)
		{
		case 0:
			//			m_stpModRunInfo->stOPModeIO.bAlive		= bState ? TRUE: FALSE;	
			break;
		case 1:
			m_stpModRunInfo->stOPModeIO.bInLine		= bState ? TRUE: FALSE;	break;
		case 2:
			m_stpModRunInfo->stOPModeIO.bManual		= bState ? TRUE: FALSE;	break;
		case 3:
			m_stpModRunInfo->stOPModeIO.bStandBy	= bState ? TRUE: FALSE;	break;
		case 4:
			m_stpModRunInfo->stOPModeIO.bStart		= bState ? TRUE: FALSE;	break;
		case 5:
			m_stpModRunInfo->stOPModeIO.bStop		= bState ? TRUE: FALSE;	break;
		case 6:
			m_stpModRunInfo->stOPModeIO.bCycleStop	= bState ? TRUE: FALSE;	break;
		case 7:
			m_stpModRunInfo->stOPModeIO.bPM			= bState ? TRUE: FALSE;	break;
		case 8:
			m_stpModRunInfo->stOPModeIO.bBuzzer		= bState ? TRUE: FALSE;	break;
		case 9:
			m_stpModRunInfo->stOPModeIO.bDataEditing= bState ? TRUE: FALSE;	break;
		case 10:
			m_stpModRunInfo->stOPModeIO.bNetworkError[0]= bState ? TRUE: FALSE; break;	// Upper1 
		case 11:
			m_stpModRunInfo->stOPModeIO.bNetworkError[1]= bState ? TRUE: FALSE; break;	// Lower1
		case 12:
			m_stpModRunInfo->stOPModeIO.bNetworkError[2]= bState ? TRUE: FALSE; break;	// Upper2
		case 13:
			m_stpModRunInfo->stOPModeIO.bNetworkError[3]= bState ? TRUE: FALSE; break;	// Lower2
		case 14:
			m_stpModRunInfo->stOPModeIO.bNetworkError[4]= bState ? TRUE: FALSE; break;	// Upper3
		case 15:
			m_stpModRunInfo->stOPModeIO.bNetworkError[5]= bState ? TRUE: FALSE; break;	// Lower3
		}
	}
}

void CBaseModule::UpdateDataLink_OpStatusIOToSMA(short shData)
{
	BOOL	bState = FALSE;
	for( long i = 0; i < 16; i++ )
	{
		bState = (shData >> i) & 0x0001;
		switch(i)
		{
		case 0:
			m_stpModRunInfo->stOPStateIO.bWarnAlarm			=	bState ? TRUE: FALSE;	break;
		case 1:
			m_stpModRunInfo->stOPStateIO.bHeavyAlarm		=	bState ? TRUE: FALSE;	break;
		case 2:
			m_stpModRunInfo->stOPStateIO.bChemChgWarning	=	bState ? TRUE: FALSE;	break;
		case 3:
			m_stpModRunInfo->stOPStateIO.bChemChanging		=	bState ? TRUE: FALSE;	break;

		case 5:		// PLC EPD 사용일 경우에만 Log 기록
			m_stpSma->stLayOutCfg.stEPDCfg.bEPDUsed			=	bState ? TRUE: FALSE;	break;	

//if(!m_stpSma->stLayOutCfg.stEPDCfg.bEPDUsed)

/**	\brief 090714 PSK 유저 요청 사항. Bit 추가.
	＊ Etcher =====================
	Bit 214~217 :GECD 사용
	  0 : A Tank 액 보충 중.
	  1 : B Tank 액 보충 중.

	＊ Stripper ===================
	Bit 414~417 :GECD 사용
	  0 : HP01 액 보충 중.
	  1 : HP02 Tank 액 보충 중.
	  2 : SS02 Tank 액 보충 중.
	  3 : 조합조 Tank 액 보충 중.
*/		
		case 8:
			m_stpModRunInfo->stOPStateIO.bChemicalSupply[0]	= bState ? TRUE : FALSE; break;
		case 9:
			m_stpModRunInfo->stOPStateIO.bChemicalSupply[1]	= bState ? TRUE : FALSE; break;
		case 10:
			m_stpModRunInfo->stOPStateIO.bChemicalSupply[2]	= bState ? TRUE : FALSE; break;
		case 11:
			m_stpModRunInfo->stOPStateIO.bChemicalSupply[3]	= bState ? TRUE : FALSE; break;
		}
	}
}

void CBaseModule::UpdateDataLink_GlassTrackingIOToSMA(short *shData)
{
	BOOL	bState	= FALSE;
	BOOL	bGlsChk	= FALSE;
	long	i		= 0;
	long	j		= 0;
	long	k		= 0;
	long	nCnt	= 0;
	long	nGCnt   = 0;
	long	nMapCnt = 0;
	
	stUnitRunInfoType	*pArmInfo	= NULL;
	stUnitRunInfoType	*pUnitInfo	= NULL;
	stUnitRunInfoType	*pBuffInfo	= NULL;
	stUnitCfgType		*pUnitCfg	= NULL;
	
	for( i = 0; i < MAX_LAYER2_MODULE_COUNT; i++ )
	{
		pUnitCfg	= &m_stpModCfg->stUnitCfg[i];
		pUnitInfo	= &m_stpModRunInfo->stUnitInfo[i];
		
		if ( pUnitCfg == NULL || pUnitInfo == NULL )	continue;
		
		switch( pUnitCfg->nUnitType )
		{
		case eUnitType_Robot:
			// Arm Glass Set Info
			nCnt = pUnitCfg->nPosiNo;
			if(nCnt < 1) break;
			k = (nCnt-1)/16;
			
			bState = ( shData[k] >> (nCnt-k*16-1) ) & 0x0001;
			pArmInfo = &m_stpModRunInfo->stRobotArmInfo[0];
			pArmInfo->bGlassSet = bState;

			// Unit Glass Set Info
			if (m_stpModRunInfo->stRobotArmInfo[0].bGlassSet == TRUE || m_stpModRunInfo->stRobotArmInfo[1].bGlassSet == TRUE)
			{
				pUnitInfo->bGlassSet = TRUE;
			}
			else
				pUnitInfo->bGlassSet = FALSE;
			break;
			
		case eUnitType_Buffer:
			nCnt = pUnitCfg->nPosiNo;
			if(nCnt < 1) break;
			k = (nCnt-1)/16;
			
			nMapCnt = nCnt-k*16-1;
			// Buffer Slot Glass Set Info
			for(j=0; j<m_stpModCfg->nUsedBuffSlotCount; j++)
			{
				if(nMapCnt+j < 16 )
				{
					bState = ( shData[k] >> (nMapCnt+j) ) & 0x0001;
					m_stpModRunInfo->stBuffInfo[j].bGlassSet = bState;	
				}
				else
				{
					bState = ( shData[k+1] >> (nMapCnt+j-16) ) & 0x0001;
				}
				m_stpModRunInfo->stBuffInfo[j].bGlassSet = bState;	
			}
			
			// Unit Glass Set Info
			for(j=0; j<m_stpModCfg->nUsedBuffSlotCount; j++)
			{
				if (m_stpModRunInfo->stBuffInfo[j].bGlassSet == TRUE)
				{
					pUnitInfo->bGlassSet = TRUE;
					nGCnt++;
					break;
				}
			}
			
			if(nGCnt == 0)
				pUnitInfo->bGlassSet = FALSE;
			break;
			
		case eUnitType_UpDown:
			// Unit Map Index
			nCnt = pUnitCfg->nPosiNo;
			
			if(nCnt < 1) break;
			k = (nCnt-1)/16;
			
			bState = ( shData[k] >> (nCnt-k*16-1) ) & 0x0001;
			pUnitInfo->bGlassSet = bState;
			
			///////////////////////////////////////////////////////////////////////
			// Up/Down Map Index
			nCnt = pUnitCfg->nLiftPosiNo;
			
			if(nCnt < 1) break;
			k = (nCnt-1)/16;
			
			for(j=0;j<3;j++)
			{
				bState = ( shData[k] >> (nCnt-k*16-1+j) ) & 0x0001;
				switch(j)
				{
				case 0: pUnitInfo->stUDLiftData.bUpPosition	 = bState; break;
				case 1: pUnitInfo->stUDLiftData.bMidPosition	 = bState; break;
				case 2: pUnitInfo->stUDLiftData.bDownPosition = bState; break;
				}
			}
			//////////////////////////////////////////////////////////////////////
			break;
			
		default:
			// Unit Glass Set Info
			nCnt = pUnitCfg->nPosiNo;
			if(nCnt < 1) break;
			k = (nCnt-1)/16;
			
			bState = ( shData[k] >> (nCnt-k*16-1) ) & 0x0001;
			pUnitInfo->bGlassSet = bState;
			
			break;
		}
	}
}

void CBaseModule::UpdateDataLink_ReplyAndEventIOToSMA(short shData)
{
	BOOL	bState = FALSE;
	
	// Except Etcher Bypass/Buffer Module for Module No
	if(m_stpSma->stLayOutCfg.nEQType == eEQType_ETCHSTRIP)
	{
		if(m_stpModCfg->nModuleID == eModuleType_Bypass || m_stpModCfg->nModuleID == eModuleType_Buffer)	return;
	}
	
	for( long i = 0; i < 16; i++ )
	{
		bState = (shData >> i) & 0x0001;
		switch(i)
		{
		case 0:
			m_stpModRunInfo->stEQCmdRlyEvtIO.bMachineCmdReply	= bState ? TRUE: FALSE;	break;
		case 1:
			m_stpModRunInfo->stEQCmdRlyEvtIO.bAlarmClearReply	= bState ? TRUE: FALSE;	break;
		case 2:
			m_stpModRunInfo->stEQCmdRlyEvtIO.bTimeSetReply		= bState ? TRUE: FALSE;	break;
		case 3:
			m_stpModRunInfo->stEQCmdRlyEvtIO.bOperatorCallReply	= bState ? TRUE: FALSE;	break;
		case 4:
			m_stpModRunInfo->stEQCmdRlyEvtIO.bBuzzerStopReply	= bState ? TRUE: FALSE;	break;
		case 7:
			m_stpModRunInfo->stEQCmdRlyEvtIO.bTerminalMsgReply	= bState ? TRUE: FALSE;	break;
		case 8:
			m_stpModRunInfo->stEQCmdRlyEvtIO.bAlarmOccured		= bState ? TRUE: FALSE;	break;
		case 9:
			m_stpModRunInfo->stEQCmdRlyEvtIO.bAlarmTreated		= bState ? TRUE: FALSE;	break;
		case 10:
			m_stpModRunInfo->stEQCmdRlyEvtIO.bGlassScrap		= bState ? TRUE: FALSE;	break;
		case 11:
			m_stpModRunInfo->stEQCmdRlyEvtIO.bGlassUnscrap		= bState ? TRUE: FALSE;	break;
		case 12:
			m_stpModRunInfo->stEQCmdRlyEvtIO.bGlassJudgement	= bState ? TRUE: FALSE;	break;
		case 13:
			m_stpModRunInfo->stEQCmdRlyEvtIO.bRecipeDownReq		= bState ? TRUE: FALSE;	break;
		case 14:
			m_stpModRunInfo->stEQCmdRlyEvtIO.bManualCellLoad	= bState ? TRUE: FALSE;	
			m_stpModRunInfo->stEQCmdRlyEvtIO.bGlassSendFail		= bState ? TRUE: FALSE;	break;  // 2010-08-24 add
		case 15:
			m_stpModRunInfo->stEQCmdRlyEvtIO.bBackModeStart		= bState ? TRUE: FALSE;	break;
		}
	}
	
}

void CBaseModule::UpdateDataLink_DataChangeReplyAndEventIOToSMA(short shData)
{
	BOOL	bState = FALSE;
	
	// Except Etcher Bypass/Buffer Module for Module No
	if(m_stpSma->stLayOutCfg.nEQType == eEQType_ETCHSTRIP)
	{
		if(m_stpModCfg->nModuleID == eModuleType_Bypass || m_stpModCfg->nModuleID == eModuleType_Buffer)	return;
	}
	
	for( long i = 0; i < 16; i++ )
	{
		bState = (shData >> i) & 0x0001;
		switch(i)
		{
		case 0:
			m_stpModRunInfo->stDataChangeRlyEvtIO.bECIDRly			= bState ? TRUE: FALSE;	break;
		case 1:
			m_stpModRunInfo->stDataChangeRlyEvtIO.bTemperatureRly	= bState ? TRUE: FALSE;	break;
		case 2:
			m_stpModRunInfo->stDataChangeRlyEvtIO.bECORly			= bState ? TRUE: FALSE;	break;
		case 3:
			m_stpModRunInfo->stDataChangeRlyEvtIO.bMNCellReadRly	= bState ? TRUE: FALSE;	break;
		case 4:
			m_stpModRunInfo->stDataChangeRlyEvtIO.bMNValidChkReq	= bState ? TRUE: FALSE;	break;
		case 5:
			m_stpModRunInfo->stDataChangeRlyEvtIO.bMNCancelReq		= bState ? TRUE: FALSE;	break;
		case 6:
			m_stpModRunInfo->stDataChangeRlyEvtIO.bVCRReadFail		= bState ? TRUE: FALSE;	break;
		case 7:
			m_stpModRunInfo->stDataChangeRlyEvtIO.bMNCellJudgeRly	= bState ? TRUE: FALSE;	break;
		case 8:
			m_stpModRunInfo->stDataChangeRlyEvtIO.bECIDEvtReq		= bState ? TRUE: FALSE;	break;
		case 9:
			m_stpModRunInfo->stDataChangeRlyEvtIO.bTemperatureEvtReq = bState ? TRUE: FALSE;	break;
		case 10:
			m_stpModRunInfo->stDataChangeRlyEvtIO.bECOEvtReq	= bState ? TRUE: FALSE;	break;
		case 11:
			m_stpModRunInfo->stDataChangeRlyEvtIO.bPPIDValidationReq = bState ? TRUE: FALSE;	break;
		case 12:
			m_stpModRunInfo->stDataChangeRlyEvtIO.bProcessEndEvtReq	= bState ? TRUE: FALSE;	break;
		case 13:
			//			m_stpModRunInfo->stDataChangeRlyEvtIO.bParam6EvtReq		= bState ? TRUE: FALSE;	break;
			m_stpModRunInfo->stEQCmdRlyEvtIO.bHS_NR_TimeOver		= bState ? TRUE: FALSE;	break;
		case 14:
			//			m_stpModRunInfo->stDataChangeRlyEvtIO.bParam7EvtReq		= bState ? TRUE: FALSE;	break;
			m_stpModRunInfo->stEQCmdRlyEvtIO.bHS_BC_TimeOver		= bState ? TRUE: FALSE;	break;
		case 15:
			m_stpModRunInfo->stDataChangeRlyEvtIO.bParam8EvtReq		= bState ? TRUE: FALSE;	break;
		}
	}
}

void CBaseModule::GetDataLink_BitState()
{
	short   shReadData[10]	=	{ 0x00, };
	long	nReadDataSize	= sizeof(short) * 10;
	short	shAddr = 0;
	long	nStationNo = 0;
	
	if(m_stpSma->stLayOutCfg.nEQType == eEQType_ETCHSTRIP && m_stpModCfg->nModuleID > eModuleType_Stripper)
	{
		nStationNo = eModuleType_Etcher;
	}
	else
	{
		nStationNo = m_stpModCfg->stMelsecCfg[0].nMelStationNo;
	}
	
	if ( nStationNo <= 1 ) return ;
	
	if ( nStationNo > 1 )
		shAddr = B_L2_ALIVE_STATUS + B_EQ_EACH_LAYER_INTERVAL * (nStationNo - 2); //0x200 + 
	else
		shAddr = B_L2_ALIVE_STATUS;
	
	m_stpModRunInfo->nModuleID = m_stpModCfg->nModuleID;
	
	//	To Get EQ Bit State Packet
	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevB, shAddr, &nReadDataSize, shReadData);
	
	//	Equipment IO Info Update To SMA
	
	if(m_stpSma->stLayOutCfg.nEQType == eEQType_ETCHSTRIP &&
		(m_stpModCfg->nModuleID == eModuleType_Bypass || m_stpModCfg->nModuleID == eModuleType_Buffer))
	{
		UpdateDataLink_OpModeIOToSMA_FOR_BYP_BUF(shReadData[8]); // Etcher/Strip Type의 Bypass & Buffer 전용
		UpdateDataLink_OpStatusIOToSMA_FOR_BYP_BUF(shReadData[9]); // Etcher/Strip Type의 Bypass & Buffer 전용
	}
	else
	{
		UpdateDataLink_OpModeIOToSMA(shReadData[0]);
		UpdateDataLink_OpStatusIOToSMA(shReadData[1]);
	}
	
	UpdateDataLink_GlassTrackingIOToSMA(&shReadData[2]);
	UpdateDataLink_ReplyAndEventIOToSMA(shReadData[6]);
	UpdateDataLink_DataChangeReplyAndEventIOToSMA(shReadData[7]);
	
}

void CBaseModule::UpdateDataLink_OpModeIOToSMA_FOR_BYP_BUF(short shData)
{
	BOOL	bState = FALSE;
	for( long i = 0; i < 16; i++ )
	{
		bState = (shData >> i) & 0x0001;
		switch(i)
		{
		case 0:
			m_stpModRunInfo->stOPModeIO.bAlive		= bState ? TRUE: FALSE;	break;
		case 1:
			m_stpModRunInfo->stOPModeIO.bInLine		= bState ? TRUE: FALSE;	break;
		case 2:
			m_stpModRunInfo->stOPModeIO.bManual		= bState ? TRUE: FALSE;	break;
		case 3:
			m_stpModRunInfo->stOPModeIO.bStandBy	= bState ? TRUE: FALSE;	break;
		case 4:
			m_stpModRunInfo->stOPModeIO.bStart	  	= bState ? TRUE: FALSE;	break;
		case 5:
			m_stpModRunInfo->stOPModeIO.bStop	  	= bState ? TRUE: FALSE;	break;
		case 6:
			m_stpModRunInfo->stOPModeIO.bCycleStop	  = bState ? TRUE: FALSE;	break;
		case 7:
			m_stpModRunInfo->stOPModeIO.bPM			  = bState ? TRUE: FALSE;	break;
		case 8:
			m_stpModRunInfo->stOPModeIO.bBuzzer		  = bState ? TRUE: FALSE;	break;
		case 9:
			m_stpModRunInfo->stOPModeIO.bDataEditing  = bState ? TRUE: FALSE;	break;
		case 10:
			m_stpModRunInfo->stOPModeIO.bNetworkError[0] = bState ? TRUE: FALSE;	break;	// 상류
		case 11:
			m_stpModRunInfo->stOPModeIO.bNetworkError[1] = bState ? TRUE: FALSE;	break;	// SEMES
		case 12:
			m_stpModRunInfo->stOPModeIO.bNetworkError[2] = bState ? TRUE: FALSE;	break;	// 하류
		}
	}
}

void CBaseModule::UpdateDataLink_OpStatusIOToSMA_FOR_BYP_BUF(short shData)
{
	BOOL	bState = FALSE;
	for( long i = 0; i < 16; i++ )
	{
		bState = (shData >> i) & 0x0001;
		switch(i)
		{
		case 0:
			m_stpModRunInfo->stOPStateIO.bWarnAlarm		= bState ? TRUE: FALSE;	break;
		case 1:
			m_stpModRunInfo->stOPStateIO.bHeavyAlarm		= bState ? TRUE: FALSE;	break;
		case 2:
			m_stpModRunInfo->stOPStateIO.bChemChgWarning	= bState ? TRUE: FALSE;	break;
		case 3:
			m_stpModRunInfo->stOPStateIO.bChemChanging		= bState ? TRUE: FALSE;	break;


/**	\brief 090714 PSK 유저 요청 사항. Bit 추가.
	＊ Etcher =====================
	Bit 214~217 :GECD 사용
	  0 : A Tank 액 보충 중.
	  1 : B Tank 액 보충 중.

	＊ Stripper ===================
	Bit 414~417 :GECD 사용
	  0 : HP01 액 보충 중.
	  1 : HP02 Tank 액 보충 중.
	  2 : SS02 Tank 액 보충 중.
	  3 : 조합조 Tank 액 보충 중.
*/
		case 8:
			m_stpModRunInfo->stOPStateIO.bChemicalSupply[0]	= bState ? TRUE: FALSE;	break;
		case 9:
			m_stpModRunInfo->stOPStateIO.bChemicalSupply[1]	= bState ? TRUE: FALSE;	break;
		case 10:
			m_stpModRunInfo->stOPStateIO.bChemicalSupply[2]	= bState ? TRUE: FALSE;	break;
		case 11:
			m_stpModRunInfo->stOPStateIO.bChemicalSupply[3]	= bState ? TRUE: FALSE;	break;
		}
	}
}

//	Each Module Word Data Related
//	Glass Tracking Data Read
void CBaseModule::UpdateDataLink_GlassTrackingDataToSMA(short *shData)
{
	long i		= 0;
	long j		= 0;
	long k		= 0;
	long nIdx	= 0;
	long nUniqID = 0;
	long nGCnt = 0;
	long nBuffEQstate = 0;
	long nBuffEQPstate = 0;
	
	stUnitRunInfoType	*pUnitInfo	= NULL;
	stSystemRunInfoType *pSysRunInfo = GetSysRunInfo();
	stUnitRunInfoType	*pArmInfo	= NULL;
	stUnitRunInfoType	*pBuffInfo	= NULL;
	
	for( i = 0; i < m_stpModCfg->nUnitCount ; i++ )
	{
		pUnitInfo = &m_stpModRunInfo->stUnitInfo[i];
		if(m_stpModCfg->stUnitCfg[i].nUnitID < 1 )
		{
			//m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("Unit CFG Position Data Error: Module No = %d, Position = %d, nUnitID = %d", m_stpModCfg->nModuleID, i, m_stpModCfg->stUnitCfg[i].nUnitID);
			return;
		}
		if(m_stpModCfg->stUnitCfg[i].nPosiNo < 1)	continue;
		
		nIdx = (m_stpModCfg->stUnitCfg[i].nPosiNo - 1) * 10;				// 10 (Item/Position)
		
		switch(m_stpModCfg->stUnitCfg[i].nUnitType)
		{
		case eUnitType_Robot:
			for( j = 0; j < m_stpModCfg->nRobotArmCount; j++ )
			{
				pArmInfo = &m_stpModRunInfo->stRobotArmInfo[j];
				if ( pArmInfo == NULL ) continue;
				
				pArmInfo->nEQState		= shData[nIdx];	nIdx++;
				pArmInfo->nProcState	= shData[nIdx];	nIdx++;
				
				pArmInfo->nUniqueID[0]	= shData[nIdx] & 0x00ff;			//	Glass id
				pArmInfo->nUniqueID[1]	= ( shData[nIdx] >> 8 ) & 0x00ff;	//	Job Order
				nIdx++;
				pArmInfo->nUniqueID[2]	= shData[nIdx] & 0x00ff;			//	Port No
				pArmInfo->nUniqueID[3]	= ( shData[nIdx] >> 8 ) & 0x00ff;	//	Slot No
				nIdx++;
				memcpy(pArmInfo->szJudgement, &shData[nIdx], MAX_JUDGEMENT_RESULT_LEN); nIdx += 2;
				pArmInfo->nGlassState	= shData[nIdx];	nIdx++;
				pArmInfo->nRecipeNo	= shData[nIdx];	nIdx++;
				pArmInfo->nOwnGlassNo	= shData[nIdx];	nIdx += 2;
			}
			
			if ( !m_stpModRunInfo->stRobotArmInfo[0].bGlassSet && m_stpModRunInfo->stRobotArmInfo[1].bGlassSet )
				pArmInfo = &m_stpModRunInfo->stRobotArmInfo[1];
			else
				pArmInfo = &m_stpModRunInfo->stRobotArmInfo[0];
			
			nUniqID = pArmInfo->nOwnGlassNo;	// nUniqueID[0]; 
			if ( nUniqID == NULL || nUniqID == 0 ) 
			{
				memset(&pArmInfo->stPanelData, 0x00, sizeof(stPanelInfoType));
			}
			else
			{
				memcpy(&pArmInfo->stPanelData , &pSysRunInfo->stUniqueIDTable[nUniqID-1].stUniqueIDPanelInfo, sizeof(stPanelInfoType));
			}
			
			if ( pUnitInfo == NULL ) continue;
			pUnitInfo->nEQState		= pArmInfo->nEQState;
			pUnitInfo->nProcState	= pArmInfo->nProcState;
			pUnitInfo->nUniqueID[0]	= pArmInfo->nUniqueID[0];
			pUnitInfo->nUniqueID[1]	= pArmInfo->nUniqueID[1];
			pUnitInfo->nUniqueID[2]	= pArmInfo->nUniqueID[2];
			pUnitInfo->nUniqueID[3]	= pArmInfo->nUniqueID[3];
			memcpy(pUnitInfo->szJudgement, pArmInfo->szJudgement, MAX_JUDGEMENT_RESULT_LEN);
			pUnitInfo->nGlassState	= pArmInfo->nGlassState;
			pUnitInfo->nRecipeNo	= pArmInfo->nRecipeNo;
			pUnitInfo->nOwnGlassNo	= pArmInfo->nOwnGlassNo;
			
			nUniqID = pUnitInfo->nOwnGlassNo;		//nUniqueID[0];
			if ( nUniqID == NULL || nUniqID == 0 ) 
			{
				memset(&pUnitInfo->stPanelData, 0x00, sizeof(stPanelInfoType));
			}
			else
			{
				memcpy(&pUnitInfo->stPanelData , &pSysRunInfo->stUniqueIDTable[nUniqID-1].stUniqueIDPanelInfo, sizeof(stPanelInfoType));
			}
			break;
			
		case eUnitType_Buffer:
			for( j = 0; j < m_stpModCfg->nUsedBuffSlotCount; j++ )
			{
				pBuffInfo = &m_stpModRunInfo->stBuffInfo[j];
				if ( pBuffInfo == NULL ) continue;
				
				pBuffInfo->nEQState		= shData[nIdx];	nIdx++;
				pBuffInfo->nProcState	= shData[nIdx];	nIdx++;
				
				pBuffInfo->nUniqueID[0]	= shData[nIdx] & 0x00ff;			//	Glass id
				pBuffInfo->nUniqueID[1]	= ( shData[nIdx] >> 8 ) & 0x00ff;	//	Job Order
				nIdx++;
				pBuffInfo->nUniqueID[2]	= shData[nIdx] & 0x00ff;			//	Port No
				pBuffInfo->nUniqueID[3]	= ( shData[nIdx] >> 8 ) & 0x00ff;	//	Slot No	
				nIdx++;
				memcpy(pBuffInfo->szJudgement, &shData[nIdx], MAX_JUDGEMENT_RESULT_LEN); nIdx += 2;
				pBuffInfo->nGlassState	= shData[nIdx];	nIdx ++;
				pBuffInfo->nRecipeNo	= shData[nIdx];	nIdx ++;
				pBuffInfo->nOwnGlassNo	= shData[nIdx];	nIdx += 2;
				
				nUniqID = pBuffInfo->nOwnGlassNo;		//nUniqueID[0];
				if ( nUniqID == NULL || nUniqID == 0 ) 
				{
					memset(&pBuffInfo->stPanelData, 0x00, sizeof(stPanelInfoType));
				}
				else
				{
					memcpy(&pBuffInfo->stPanelData , &pSysRunInfo->stUniqueIDTable[nUniqID-1].stUniqueIDPanelInfo, sizeof(stPanelInfoType));
				}
			}
			
			nBuffEQstate = 0;
			nBuffEQPstate = 0;
			
			for( j = 0; j < m_stpModCfg->nUsedBuffSlotCount; j++ )
			{
				pBuffInfo = &m_stpModRunInfo->stBuffInfo[j];
				if ( pBuffInfo == NULL) continue;
				
				if(nBuffEQstate < pBuffInfo->nEQState)
					nBuffEQstate	= pBuffInfo->nEQState;
				if(nBuffEQPstate  < pBuffInfo->nProcState)
					nBuffEQPstate = pBuffInfo->nProcState;
			}
			
			if(nBuffEQstate >0)
				pUnitInfo->nEQState = nBuffEQstate;
			if(nBuffEQPstate >0)
				pUnitInfo->nProcState = nBuffEQPstate;
			
			for( j = 0; j < m_stpModCfg->nUsedBuffSlotCount; j++ )
			{
				pBuffInfo = &m_stpModRunInfo->stBuffInfo[j];
				if ( pBuffInfo == NULL) continue;
				
				if(pBuffInfo->bGlassSet == TRUE)
				{
					pUnitInfo->nUniqueID[0]	= pBuffInfo->nUniqueID[0];
					pUnitInfo->nUniqueID[1]	= pBuffInfo->nUniqueID[1];
					pUnitInfo->nUniqueID[2]	= pBuffInfo->nUniqueID[2];
					pUnitInfo->nUniqueID[3]	= pBuffInfo->nUniqueID[3];
					memcpy(pUnitInfo->szJudgement, pBuffInfo->szJudgement, MAX_JUDGEMENT_RESULT_LEN);
					pUnitInfo->nGlassState	= pBuffInfo->nGlassState;
					pUnitInfo->nRecipeNo	= pBuffInfo->nRecipeNo;
					pUnitInfo->nOwnGlassNo	= pBuffInfo->nOwnGlassNo;
					
					nUniqID = pUnitInfo->nOwnGlassNo;		//nUniqueID[0];
					if(nUniqID > 0)
					{
						memcpy(&pUnitInfo->stPanelData , &pSysRunInfo->stUniqueIDTable[nUniqID-1].stUniqueIDPanelInfo, sizeof(stPanelInfoType));
					}
					nGCnt++;
					break;
				}
			}
			
			if(nGCnt == 0)
				memset(&pUnitInfo->stPanelData, 0x00, sizeof(stPanelInfoType));
			break;
		default:
			//	일반 Unit Type Run Info
			pUnitInfo->nEQState = shData[nIdx];	nIdx++;
			pUnitInfo->nProcState = shData[nIdx]; nIdx++;
			pUnitInfo->nUniqueID[0]	= shData[nIdx] & 0x00ff;			//	Unique id
			pUnitInfo->nUniqueID[1]	= ( shData[nIdx] >> 8 ) & 0x00ff;	//	Job Order
			nIdx++;
			pUnitInfo->nUniqueID[2]	= shData[nIdx] & 0x00ff;			//	Slot No
			pUnitInfo->nUniqueID[3]	= ( shData[nIdx] >> 8 ) & 0x00ff;	//	Port No
			nIdx++;
			
			memcpy(pUnitInfo->szJudgement, &shData[nIdx], MAX_JUDGEMENT_RESULT_LEN); nIdx += 2;
			pUnitInfo->nGlassState	= shData[nIdx];	nIdx++;
			pUnitInfo->nRecipeNo	= shData[nIdx];	nIdx++;
			pUnitInfo->nOwnGlassNo	= shData[nIdx];	nIdx += 2;
			
			nUniqID = pUnitInfo->nOwnGlassNo;		//nUniqueID[0];
			if ( nUniqID < 1 || nUniqID > 255 )
			{
				if( pUnitInfo->bGlassSet == FALSE )
					memset(&pUnitInfo->stPanelData, 0x00, sizeof(stPanelInfoType));
			}
			else
			{
				memcpy(&pUnitInfo->stPanelData , &pSysRunInfo->stUniqueIDTable[nUniqID-1].stUniqueIDPanelInfo, sizeof(stPanelInfoType));
			}
			break;
		}
	}
}

void CBaseModule::GetDataLink_GlassTrackingData()
{
	short   shReadData[500]	= { 0x00, };
	long	nReadDataSize	= sizeof(short) * 500;
	long	nDevType		= 0;
	long	nAddr			= W_POS1_EQ_STATE;
	long	nStationNo		= 0;
	long	nStartAddr		=	0;
	
	//	Melsec Read
	if(m_stpSma->stLayOutCfg.nEQType == eEQType_ETCHSTRIP && m_stpModCfg->nModuleID > 3)
	{
		nStationNo = eModuleType_Etcher;
	}
	else
	{
		nStationNo = m_stpModCfg->nModuleID;
	}
	
	nStartAddr = GetPosStartAddr(nStationNo);
	nStartAddr += nAddr;
	m_pParent->m_MelERMemIF.MelNetReceiveEx(DevW, nStartAddr, &nReadDataSize, shReadData);
	
	//	SMA Update
	UpdateDataLink_GlassTrackingDataToSMA(shReadData);
}

void CBaseModule::UpdateDataLink_CurrentProcessDataToSMA(short *shData)
{
	long	i = 0;
	long	j = 0;
	long	nIdx = 0;
	long	nMapIdx = 0;
	long	nType = 0;
	long	nDataType = 0;
	long	nPoint = 0;
	long	nViewPoint = 0;
	long	nEPDCount = 0;
	long	nWordType = 0;
	
	// SVID
	for( i = 0; i < m_stpModCfg->stSVIDTable.nSVIDCount; i++ )
	{
		nType = m_stpModCfg->stSVIDTable.stSVIDCfg[i].stParamCfg.nConstantType;
		nDataType = m_stpModCfg->stSVIDTable.stSVIDCfg[i].stParamCfg.nDataType;
	
		if(m_stpModCfg->stSVIDTable.stSVIDCfg[i].stParamCfg.nMapIndex < 1)	continue;
			
		nIdx = m_stpModCfg->stSVIDTable.stSVIDCfg[i].stParamCfg.nIndex - 1;
		nWordType = m_stpModCfg->stSVIDTable.stSVIDCfg[i].stParamCfg.nWordType;

		//@ 2Word 사용 SVID
		if (nWordType == eWordSize_2Word)
		{
			nMapIdx = MAX_SVID_1WORD_COUNT + (m_stpModCfg->stSVIDTable.stSVIDCfg[i].stParamCfg.nMapIndex*2 - 2);

			memcpy(&m_stpModRunInfo->stCurProcData.nCurSVIDData[nIdx], &shData[nMapIdx], sizeof(short) * 2);
		}
		else	//@ 1Word 사용 SVID
		{
			nMapIdx = m_stpModCfg->stSVIDTable.stSVIDCfg[i].stParamCfg.nMapIndex - 1;
			m_stpModRunInfo->stCurProcData.nCurSVIDData[nIdx] = shData[nMapIdx];
		}
	}
}

void CBaseModule::GetDataLink_CurProcessingData()
{
	short   shReadData[MAX_SVID_COUNT]	= { 0x00, };
	long	nReadDataSize	= sizeof(short) * MAX_SVID_COUNT;
	long	nDevType		= 0;
	long	nStartAddr		= 0;
	long	nAddr			= W_POS1_SVID1;
	
	//	Melsec Read
	nStartAddr = GetPosStartAddr(m_stpModCfg->nModuleID);
	nStartAddr += nAddr;
	m_pParent->m_MelERMemIF.MelNetReceiveEx(DevW, nStartAddr, &nReadDataSize, shReadData);
	
	UpdateDataLink_CurrentProcessDataToSMA(shReadData);
}

// 
void CBaseModule::GetDataLink_ActionLogData()
{
/*
	short	shReadData[500] = { 0x00, };
	long	nReadDataSize	= sizeof(short) * 500;
	long	nDevType		= 0;
	long	nStartAddr		= 0;
	long	nAddr			= W_POS1_ACTION_LOG_DATA;//0x1290		, 0x1270		// gap: 0x1D00
	
	nStartAddr = GetPosStartAddr(m_stpModCfg->nModuleID);//0x50E6
	nStartAddr += nAddr; //0x6356 = 0x5070 + 0x1290
	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, nStartAddr, &nReadDataSize, shReadData);
	
	OnActionStatusCheck(shReadData);
*/
}

void CBaseModule::OnActionStatusCheck(short *shData)
{
	long	nModuleID	= 0;
	long	nUnitNo		= 0;
	long	nIdx		= 0;	
	long	nBitNo		= 0;
	long	nFromPos	= 0;
	long	nToPos		= 0;
	BOOL	bBitState	= FALSE;
	long	nSemesUniqID	= 0;
	BOOL    bHS = FALSE; 
	
	stUnitRunInfoType	*pUnitInfo = NULL;
	
	nModuleID = m_stpModCfg->nModuleID;
	
	for ( nUnitNo = 0; m_stpModCfg->nUnitCount ; nUnitNo++)
	{
		pUnitInfo = &m_stpModRunInfo->stUnitInfo[nUnitNo];
		
		if(m_stpModCfg->stUnitCfg[nUnitNo].nUnitID < 1 )	return;
		
		if(m_stpModCfg->stUnitCfg[nUnitNo].nPosiNo < 1 )	continue;
		
		nSemesUniqID = pUnitInfo->nOwnGlassNo;
		nIdx = (m_stpModCfg->stUnitCfg[nUnitNo].nPosiNo - 1) * 10;
		
		nFromPos= shData[nIdx+ eActionLogData_FromPos];	//3 
		nToPos = shData[nIdx + eActionLogData_ToPos];	//5 
		
		for (nBitNo = 0; nBitNo < MAX_ACTION_ID_COUNT; nBitNo++)	//48 
		{
			if ( nBitNo < 16 )
			{
				//각 Unit 동작 상태 Check
				bBitState = (shData[nIdx + eActionLogData1] >> nBitNo) & 0x0001;		//0

			
				// test - s
				static bool b = true; 
				if( b )
				{
					bBitState = 1; 
					b = false; 
				}
				// test - e 


				bHS = FALSE; 
			}
			else if (16 <= nBitNo && nBitNo < 32 )
			{
				//HandShake No.1 & HandShake No.2 현재 상태 Check
				bBitState = (shData[nIdx + eActionLogData2] >> (nBitNo - 16)) & 0x0001;//2	
				bHS = TRUE; 
			}
			else // if (32 <= nBitNo )
			{
				//HandShake No.3 현재 상태 Check
				bBitState = (shData[nIdx + eActionLogData3] >> (nBitNo - 32)) & 0x0001;	//1
				bHS = TRUE; 
			}
			/*
			if (nBitNo >= 16)
			{
				//HandShake No.1 & HandShake No.2 현재 상태 Check
				bBitState = (shData[nIdx + eActionLogData2] >> (nBitNo - 16)) & 0x0001;//2	
			}
			else if (nBitNo >= 32)
			{
				//HandShake No.3 현재 상태 Check
				bBitState = (shData[nIdx + eActionLogData3] >> (nBitNo - 32)) & 0x0001;	//1
			}
			else
			{
				//각 Unit 동작 상태 Check
				bBitState = (shData[nIdx + eActionLogData1] >> nBitNo) & 0x0001;		//0
			}
			*/

			if (m_bOldActionState[nUnitNo][nBitNo] != bBitState)
			{	
				//MSC 관련 Log를 기록한다.
				WriteActionLog(nModuleID, nUnitNo, bHS, bBitState, nBitNo, nFromPos, nToPos, nSemesUniqID);
				//void CBaseModule::WriteActionLog(long nModuleID, long nUnitID, BOOL bHS, BOOL bBitState, long nBitNo, long nFromPos, long nToPos, long nSemesUniqID)
				m_bOldActionState[nUnitNo][nBitNo] = bBitState;
			}
		}
	}
}

void CBaseModule::UpdateDataLink_ProcessEndDataToSMA_Inline_ETCH(short *shCommonData, short *shData, char *szHPanelID)
{
/*
	long i = 0;
	long j = 0;
	long nIdx = 0;
	long nMapIdx = 0;
	long nType = 0;
	long nDIdx = 0;
	long nSVIDCnt = 0;
	long nDCIdx = 0;

	long nSEMES_UniqueNo = 0;		// nDcollIndex = 진입시 SEMES_UNIQUE_NO

	
	// ETCH End Data를 STRIP에서 보고시에 사용
	nSEMES_UniqueNo = *(shCommonData+eDcoll_SEMES_UNIQUE_NO);
	m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].nOwnGlassNo = nSEMES_UniqueNo;
	memcpy(m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].szHPanelID, szHPanelID, MAX_PANEL_ID_LEN);
	
	if (nSEMES_UniqueNo < 0 || nSEMES_UniqueNo > MAX_SEMES_UNIQUE_COUNT)
	{
		m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Process End Event : SEMES Unique No Fail!! nSEMES_UniqueNo[%d], HPanelID[%s]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nSEMES_UniqueNo, m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].szHPanelID);
		nSEMES_UniqueNo = 0;		
	}
	else
		{
		m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Process End Event : Data Read Complete!! SEMES Unique[%d], HPanelID[%s]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nSEMES_UniqueNo, m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].szHPanelID);
	}
	

			
			//	Common Data Update
	m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.nUniqueID[0]= *(shCommonData+nIdx) & 0x00ff; // unique id
	m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.nUniqueID[1]= ( *(shCommonData+nIdx) >> 8 ) & 0x00ff; // Job Order
			nIdx++;
	m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.nUniqueID[2]= *(shCommonData+nIdx) & 0x00ff; // SLOT NO
	m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.nUniqueID[3]= ( *(shCommonData+nIdx) >> 8 ) & 0x00ff; // PORT No
			nIdx++;
	m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.nTotalProcTime	= *(shCommonData+nIdx);	nIdx++;
			for(i=0; i<MAX_ETCHING_UNIT_COUNT; i++)
			{
		m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.nProcessTime[i]	= *(shCommonData+nIdx);
				nIdx++;
			}
	m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.nProcGlassCount	= *(shCommonData+nIdx);	nIdx++;
	m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.nProcRecipeNo	= *(shCommonData+nIdx);	nIdx++;
	m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.nUsedTankNo		= *(shCommonData+nIdx);	nIdx++;
	m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.nTankUsedTime	= *(shCommonData+nIdx);	nIdx++;
			for(i=0; i<6; i++)
			{
		m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.nUVUsedTime[i]	= *(shCommonData+nIdx);	nIdx++;
			}
			
			// Data Collection
			for( j = 0; j < m_stpModCfg->stDVIDTable.nDVIDCount; j++ )
			{
				nType = m_stpModCfg->stDVIDTable.stDVIDCfg[j].stParamCfg.nConstantType;
				
		if(nType == eConstant_Flow || nType == eConstant_Press || nType == eConstant_TankInfo || nType == eConstant_Common)
				{
					nMapIdx = m_stpModCfg->stDVIDTable.stDVIDCfg[j].stParamCfg.nMapIndex - 1;
					nDCIdx = m_stpModCfg->stDVIDTable.stDVIDCfg[j].stParamCfg.nIndex;

					if(nIdx > 0)
					{


// 				if (nType == eConstant_2Word)
// 					CopyMemory(&m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.stConvProcEndData.nSVIDMaxValue[nDCIdx-1], &shData[nMapIdx], sizeof(short) * 2);
// 				else
// 				m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.stConvProcEndData.nSVIDMaxValue[nDCIdx-1] = shData[nMapIdx];

				
						//2010-11-24 add voltage 1 word -> 2 words
						switch( m_stpSma->stLayOutCfg.nEQType ) 
						{
						case eEQType_LCPI: 	
							if( nMapIdx == (78-1) || nMapIdx == (80-1)) // 78 gps voltage, 80 ups voltage map index
							{
						m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.stConvProcEndData.nSVIDMaxValue[nDCIdx-1] += shData[nMapIdx +1] << 16;
							}

							break; 
						case eEQType_LCODF: 
							if( nMapIdx == (44-1) || nMapIdx == (46-1)) // 121 gps voltage, 122 ups voltage  map index
							{
						m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.stConvProcEndData.nSVIDMaxValue[nDCIdx-1] += shData[nMapIdx +1] << 16;
							}			
							break; 
						case eEQType_LCRW: 
							if( nMapIdx == (59-1) || nMapIdx == (61-1)) // 59 gps voltage, 61 ups voltage  map index
							{
						m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.stConvProcEndData.nSVIDMaxValue[nDCIdx-1] += shData[nMapIdx +1] << 16;
							}
							break; 

						// ohanaya 2011.10.05 접액
						case eEQType_ETCHSTRIP:
							switch (m_stpSma->stLayOutCfg.nEQSubType)		
							{
							case eEQETCHSTRIPSubType_RW:
							case eEQETCHSTRIPSubType_PIXELCLN:					
								break;
								
							case eEQETCHSTRIPSubType_GATE:
							case eEQETCHSTRIPSubType_PIXEL:		
									if(nMapIdx == 7)
								m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.stConvProcEndData.nContactTime[0] = shData[nMapIdx];
									else if(nMapIdx == 17)
								m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.stConvProcEndData.nContactTime[1] = shData[nMapIdx];
									else if(nMapIdx == 27)
								m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.stConvProcEndData.nContactTime[2] = shData[nMapIdx];
									else if(nMapIdx == 37)
								m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.stConvProcEndData.nContactTime[3] = shData[nMapIdx];
									else if(nMapIdx == 74)
								m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.stConvProcEndData.nContactTime[4] = shData[nMapIdx];
									else if(nMapIdx == 111)
								m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.stConvProcEndData.nContactTime[5] = shData[nMapIdx];
									else if(nMapIdx == 131)
								m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.stConvProcEndData.nContactTime[6] = shData[nMapIdx];
									else if(nMapIdx == 159)
								m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.stConvProcEndData.nContactTime[7] = shData[nMapIdx];
								break;
							}
							break;
						// ohanaya 2011.10.05 접액					
						case eEQType_ETCH:
									if(nMapIdx == 7)
								m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.stConvProcEndData.nContactTime[0] = shData[nMapIdx];
									else if(nMapIdx == 17)
								m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.stConvProcEndData.nContactTime[1] = shData[nMapIdx];
									else if(nMapIdx == 27)
								m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.stConvProcEndData.nContactTime[2] = shData[nMapIdx];
									else if(nMapIdx == 37)
								m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.stConvProcEndData.nContactTime[3] = shData[nMapIdx];
									else if(nMapIdx == 74)
								m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.stConvProcEndData.nContactTime[4] = shData[nMapIdx];
									else if(nMapIdx == 111)
								m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.stConvProcEndData.nContactTime[5] = shData[nMapIdx];
									else if(nMapIdx == 131)
								m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.stConvProcEndData.nContactTime[6] = shData[nMapIdx];
									else if(nMapIdx == 159)
								m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.stConvProcEndData.nContactTime[7] = shData[nMapIdx];
							break;

						default: 			
							break; 
						}
					}
				}
				else if(nType == eConstant_Temperature)
				{
					nMapIdx = m_stpModCfg->stDVIDTable.stDVIDCfg[j].stParamCfg.nMapIndex - 1;
					nDCIdx = m_stpModCfg->stDVIDTable.stDVIDCfg[j].stParamCfg.nMapIndex;
					if(nDCIdx > 500)
				m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.stConvProcEndData.nTempValue[nDCIdx-501] = shData[nMapIdx];
				}
				else if(nType == eConstant_TactTime)
				{
					nMapIdx = m_stpModCfg->stDVIDTable.stDVIDCfg[j].stParamCfg.nMapIndex - 1;
					nDCIdx = m_stpModCfg->stDVIDTable.stDVIDCfg[j].stParamCfg.nMapIndex;
					if(nDCIdx > 600)
					{
						if(m_stpSma->stLayOutCfg.nEQType == eEQType_PFC)		// SonJaeWon 080621
					m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.stConvProcEndData.stUnitTactData[nDCIdx-661].nMesTactTime = shData[nMapIdx];
						else if(m_stpSma->stLayOutCfg.nEQType == eEQType_WRU)		// SonJaeWon 080621
					m_stpSma->stProcEnd_ET[nSEMES_UniqueNo].stProcEndData.stConvProcEndData.stUnitTactData[nDCIdx-611].nMesTactTime = shData[nMapIdx];
				}
			}			
	}
*/
}


void CBaseModule::UpdateDataLink_ProcessEndDataToSMA(short *shCommonData, short *shData)
{
	long i		= 0;
	long j		= 0;
	long nIdx	= 0;
	long nMapIdx = 0;
	long nType	= 0;
	long nDCIdx = 0;
	long nWordType = 0;

	//	Common Data Update
	m_stpModRunInfo->stProcEndData.nOwnGlassNo			= *(shCommonData+nIdx);	nIdx+=2;
	m_stpModRunInfo->stProcEndData.nTotalProcTime		= *(shCommonData+nIdx);	nIdx++;
	//for(i=0; i<MAX_ETCHING_UNIT_COUNT; i++)
	for(i=0; i< 6; i++)
	{
		m_stpModRunInfo->stProcEndData.nProcessTime[i]	= *(shCommonData+nIdx);
		nIdx++;
	}
	m_stpModRunInfo->stProcEndData.nProcGlassCount		= *(shCommonData+nIdx);	nIdx++;
	m_stpModRunInfo->stProcEndData.nProcRecipeNo		= *(shCommonData+nIdx);	nIdx++;
	m_stpModRunInfo->stProcEndData.nUsedTankNo			= *(shCommonData+nIdx);	nIdx++;
	m_stpModRunInfo->stProcEndData.nTankUsedTime		= *(shCommonData+nIdx);	nIdx++;

	for(i=0; i<4; i++)
	{
		m_stpModRunInfo->stProcEndData.nUVUsedTime[i]	= *(shCommonData+nIdx);	nIdx++;
	}
	
	// Data Collection
	for( j = 0; j < m_stpModCfg->stDVIDTable.nDVIDCount; j++ )
	{
		nType = m_stpModCfg->stDVIDTable.stDVIDCfg[j].stParamCfg.nConstantType;

		if(nType == eConstant_Common) continue;
		
		nMapIdx = m_stpModCfg->stDVIDTable.stDVIDCfg[j].stParamCfg.nMapIndex-1;
		nDCIdx  = m_stpModCfg->stDVIDTable.stDVIDCfg[j].stParamCfg.nIndex;
		nWordType = m_stpModCfg->stDVIDTable.stDVIDCfg[j].stParamCfg.nWordType;

		if(nDCIdx > 0)
		{
			//@ 2Word 사용 SVID
			if (nWordType == eWordSize_2Word)
			{
				nMapIdx = MAX_SVID_1WORD_COUNT + (m_stpModCfg->stDVIDTable.stDVIDCfg[j].stParamCfg.nMapIndex*2 - 2);
				memcpy(&m_stpModRunInfo->stProcEndData.stConvProcEndData.nDVIDMaxValue[nDCIdx-1], &shData[nMapIdx], sizeof(short) * 2);
			}
			else	//@ 1Word 사용 DVID
			{
				m_stpModRunInfo->stProcEndData.stConvProcEndData.nDVIDMaxValue[nDCIdx-1] = shData[nMapIdx];
			}
		}
	}
}

// process end data 모두 읽음 
void CBaseModule::GetDataLink_ProcessEndData()
{
	short   shCommonData[50]	= { 0x00, };
	short   shReadData[710]		= { 0x00, };
	long	nCommonDataSize		= sizeof(short) * 50;
	long	nReadDataSize		= sizeof(short) * 710;
	long	nDevType			= 0;
	
	long	nCommonAddr			= W_ENDD_GLS_UNIQID;	// 0x0F78
	long	nReadAddr			= W_ENDD_POS1_DATA;		// 0x0FAA
	long	nStartAddr1			= 0;
	long	nStartAddr2			= 0;
	
//	Melsec Read
	nStartAddr1 = GetPosStartAddr(m_stpModCfg->nModuleID);
	nStartAddr1 += nCommonAddr;
	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, nStartAddr1, &nCommonDataSize, shCommonData);
	nStartAddr2 = GetPosStartAddr(m_stpModCfg->nModuleID);
	nStartAddr2 += nReadAddr;
	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, nStartAddr2, &nReadDataSize, shReadData);
	
	UpdateDataLink_ProcessEndDataToSMA(shCommonData, shReadData);
	
}

//Get Pointer
stSystemRunInfoType* CBaseModule::GetSysRunInfo()
{
	if(&m_stpSma->stSysRunInfo != NULL)
		return &m_stpSma->stSysRunInfo;
	
	return NULL;
}

stECIDConfigType *CBaseModule::GetECIDConfig(long nECID)		
{
	long	i = 0;
	long	j = 0;
	stECIDConfigType	*pECIDCfg = NULL;
	
	// Multi ECID
	for( i = 0; i < m_stpSma->stLayOutCfg.nModuleCount; i++ )
	{
		for (j = 0; j < m_stpSma->stLayOutCfg.stModCfg[i+2].stECIDTable.nECIDCount; j++)
		{
			if (nECID == m_stpSma->stLayOutCfg.stModCfg[i+2].stECIDTable.stECIDCfg_Multi[j].nECID)
			{
				pECIDCfg = &m_stpSma->stLayOutCfg.stModCfg[i+2].stECIDTable.stECIDCfg_Multi[j];
				return pECIDCfg;
			}
		}
	}
	
	//Single ECID
	for( i = 0; i < m_stpSma->stLayOutCfg.nModuleCount; i++ )
	{
		for (j = 0; j < m_stpSma->stLayOutCfg.stModCfg[i+2].stECIDTable.nECIDCount_Single; j++)
		{
			if (nECID == m_stpSma->stLayOutCfg.stModCfg[i+2].stECIDTable.stECIDCfg_Single[j].nECID)
			{
				pECIDCfg = &m_stpSma->stLayOutCfg.stModCfg[i+2].stECIDTable.stECIDCfg_Single[j];
				return pECIDCfg;
			}
		}
	}

	return NULL;
}

void CBaseModule::P2C_ManualCellLoadSequence()
{
	if(m_stpSma->stLayOutCfg.nEQType != eEQType_EDGE) return; // 면취 전용
	
	long	nStep			=	0;
	long	nStepState		=	STEP_START;		
	long	i				=	0;
	short	shCmdPacket[4]	=	{ 0x00, };
	short	shLocalDataAddr =	0;
	short   shData[200]		=	{ 0x00, };
	long	nReadDataSize  = sizeof(short) * 6;
	
	memset(m_ShCommData_Master[eP2C_ManualCellRoad]->m_szHPanelID,0x20,MAX_PANEL_ID_LEN);
	
	GetMasterDevBWAddr(eP2C_ManualCellRoad, m_stpModCfg->nModuleID, B_L2_TO_MANUAL_CELL_LOAD_RLY, 0);	
	
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	// Event 진행 전 Reply Bit Off. (Reply Bit Off 안된 경우 대비)
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_ManualCellRoad]->m_shBitAddr);

	while(nStep != -1)
	{
		switch(nStep++)
		{
		case 0:	//	이전 진행중인 Sequence가 있는 경우, Request Bit = False 처리 ;
			if ( m_stpModRunInfo->stEQCmdRlyEvtIO.bManualCellLoad == FALSE)
			{
				m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_ManualCellRoad]->m_shBitAddr);
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Manual Cell Load Event is Canceled!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
				nStep = -1;
				nStepState = STEP_NG;
			}
			break;
			
		case 1:	
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Manual Cell Load Event Started!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName); 
			GetLocalDevBWAddr(eP2C_ManualCellRoad, m_stpModCfg->nModuleID, 0, W_L2_MANUAL_LOAD_N_VCR_HPANEL_ID);	
			shLocalDataAddr = m_ShCommData_Local[eP2C_ManualCellRoad]->m_shDataAddr;
			
			m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, shLocalDataAddr, &nReadDataSize, shData);
			
			memcpy(m_stpSma->stSysRunInfo.szEDGEManualCellID, shData, MAX_PANEL_ID_LEN);
			memcpy(m_ShCommData_Master[eP2C_ManualCellRoad]->m_szHPanelID, shData, MAX_PANEL_ID_LEN);
			m_ShCommData_Master[eP2C_ManualCellRoad]->m_nModuleID = m_stpModCfg->nModuleID;
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Manual Cell Load Event : Data Read Complete!! HPanelID[%s]", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, m_stpSma->stSysRunInfo.szEDGEManualCellID); 
			break; 
			
		case 2:	//	Bit를 On
			m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, m_ShCommData_Master[eP2C_ManualCellRoad]->m_shBitAddr);
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Manual Cell Load Event : Reply bit On", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
			m_ShCommData_Master[eP2C_ManualCellRoad]->m_bWaitTime = TRUE;
			m_ShCommData_Master[eP2C_ManualCellRoad]->m_TimeCheck.StartTimer();
			nStep = -1;
			if(nStepState != STEP_NG) 
				nStepState = STEP_OK;
			break;
		}
	}
	
	if(nStepState == STEP_NG) 
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Manual Cell Load Event",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
	
}

void CBaseModule::P2C_ManualCellLoadSequenceEnd()
{
	short	shDataSize		=	sizeof(short);
	short 	shAckCode		=	0;
	ULONG	nElaspedTime	=	0;
	
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	if(m_stpModRunInfo->stEQCmdRlyEvtIO.bManualCellLoad == TRUE)
	{
		if(m_ShCommData_Master[eP2C_ManualCellRoad]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			nElaspedTime = m_ShCommData_Master[eP2C_ManualCellRoad]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eP2C_ManualCellRoad]->m_nElaspedTime = nElaspedTime;
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_ManualCellRoad]->m_shBitAddr);
			
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Manual Cell Load Event Time Over!![Elasped Time : %dms]", 
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Manual Cell Load Event Time Over!![Elasped Time : %dms]", 
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			
			m_ShCommData_Master[eP2C_ManualCellRoad]->m_bWaitTime = FALSE;
		}
		return;
	}
	
	// Host Report Send
	sig.nSignal = sigManualCellLoad;
	sig.nFrom	= PLC_TASK_ID;
	sig.nTo		= SCH_TASK_ID;
	memcpy(sig.unionData.stGlassUnscrapData.stPanelInfo.szHPanelID, m_stpSma->stSysRunInfo.szEDGEManualCellID, MAX_PANEL_ID_LEN);
	m_pParent->SendSignalToSCH(&sig);
	
	// Reply Bit Off
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_ManualCellRoad]->m_shBitAddr);
	
	nElaspedTime = m_ShCommData_Master[eP2C_ManualCellRoad]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[eP2C_ManualCellRoad]->m_nElaspedTime = nElaspedTime;
	
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Manual Cell Load Event Complete!![Elasped Time : %dms]",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] Manual Cell Load Event Complete!![Elasped Time : %dms]",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	m_ShCommData_Master[eP2C_ManualCellRoad]->m_bWaitTime = FALSE;
}

void CBaseModule::P2C_VCRReadingFailSequence()
{
	if(m_stpSma->stLayOutCfg.nEQType != eEQType_EDGE) return; // 면취 전용
	
	long	nStep			=	0;
	long	nStepState		=	STEP_START;		
	long	i				=	0;
	short   shFailData		=	0;
	short   shHPanel[200]		=	{ 0x00, };
	long	nReadDataSize  = sizeof(short) * 6;
	
	memset(m_ShCommData_Master[eP2C_VCRReadingNG]->m_szHPanelID, 0x20, MAX_PANEL_ID_LEN);
	
	while(nStep != -1)
		//	while((nStepState == STEP_START)) 
	{
		switch(nStep++)
		{
		case 0:	//	이전 진행중인 Sequence가 있는 경우, Request Bit = False 처리 ;
			if ( m_stpModRunInfo->stDataChangeRlyEvtIO.bVCRReadFail == FALSE)
			{
				m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_VCRReadingNG]->m_shBitAddr);
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] VCR Read Fail Event is Canceled!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
				nStep = -1;
				nStepState = STEP_NG;
			}
			break;
			
		case 1:	
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] VCR Read Fail Event Started!!", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName); 
			
			m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, W_L2_VCR_READ_FAIL_DATA, &nReadDataSize, &shFailData);
			m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, W_L2_MANUAL_LOAD_N_VCR_HPANEL_ID, &nReadDataSize, shHPanel);
			
			memcpy(&m_ShCommData_Master[eP2C_VCRReadingNG]->m_shSetData	, &shFailData, sizeof(short));
			memcpy(m_ShCommData_Master[eP2C_VCRReadingNG]->m_szHPanelID, shHPanel, MAX_PANEL_ID_LEN);
			m_ShCommData_Master[eP2C_VCRReadingNG]->m_nModuleID = m_stpModCfg->nModuleID;
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] VCR Read Fail Event : Data Read Complete!! VCR PanelID[%s] FailData[%d]",
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, m_ShCommData_Master[eP2C_VCRReadingNG]->m_szHPanelID, m_ShCommData_Master[eP2C_VCRReadingNG]->m_shSetData); 
			break; 
			
		case 2:	//	Bit를 On
			m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, B_L2_TO_CIM_VCR_READ_FAIL_RLY);
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] VCR Read Fail Event: Reply bit On", m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
			m_ShCommData_Master[eP2C_VCRReadingNG]->m_bWaitTime = TRUE;
			m_ShCommData_Master[eP2C_VCRReadingNG]->m_TimeCheck.StartTimer();
			nStep = -1;
			if(nStepState != STEP_NG) 
				nStepState = STEP_OK;
			break;
		}
	}
	
	if(nStepState == STEP_NG) 
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] VCR Read Fail Event",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
	
}

void CBaseModule::P2C_VCRReadingFailSequenceEnd()
{
	short	shDataSize		=	sizeof(short);
	short 	shAckCode		=	0;
	ULONG	nElaspedTime	=	0;
	
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	if(m_stpModRunInfo->stDataChangeRlyEvtIO.bVCRReadFail == TRUE)
	{
		if(m_ShCommData_Master[eP2C_VCRReadingNG]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			nElaspedTime = m_ShCommData_Master[eP2C_VCRReadingNG]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eP2C_VCRReadingNG]->m_nElaspedTime = nElaspedTime;
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, B_L2_TO_CIM_VCR_READ_FAIL_RLY);
			
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] VCR Read Fail Event Time Over!![Elasped Time : %dms]", 
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] VCR Read Fail Event Time Over!![Elasped Time : %dms]", 
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			
			m_ShCommData_Master[eP2C_VCRReadingNG]->m_bWaitTime = FALSE;
		}
		return;
	}
	
	// Host Report Send
	sig.nSignal = sigVCRReadFail;
	sig.nFrom	= PLC_TASK_ID;
	sig.nTo		= SCH_TASK_ID;
	sig.nFlag   = (long)m_ShCommData_Master[eP2C_VCRReadingNG]->m_shSetData;
	// 111 : Panel Read Fail
	// 113 : Mismatch
	// 114 : Key-In Time Out
	memcpy(sig.unionData.stGlassUnscrapData.stPanelInfo.szEPanelID, m_ShCommData_Master[eP2C_VCRReadingNG]->m_szHPanelID, MAX_PANEL_ID_LEN);
	m_pParent->SendSignalToSCH(&sig);
	
	// Reply Bit Off
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, B_L2_TO_CIM_VCR_READ_FAIL_RLY);
	
	nElaspedTime = m_ShCommData_Master[eP2C_VCRReadingNG]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[eP2C_VCRReadingNG]->m_nElaspedTime = nElaspedTime;
	
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)]VCR Read Fail Event Complete!![Elasped Time : %dms]",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] VCR Read Fail Event Complete!![Elasped Time : %dms]",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	m_ShCommData_Master[eP2C_VCRReadingNG]->m_bWaitTime = FALSE;
}

void CBaseModule::P2C_HSTimeOverSequence()
{
	long	nStep			=	0;
	long	nStepState		=	STEP_START;	
	long	nSequence		=	0;
	long	nBitAddr		=	0;
	BOOL	bState			=	FALSE;
	short	shData			=	0;
	long	nSize			=	sizeof(short);
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	if(m_stpModRunInfo->stEQCmdRlyEvtIO.bHS_NR_TimeOver == TRUE)
	{
		GetMasterDevBWAddr(eP2C_HS_NRTimeOver, m_stpModCfg->nModuleID, B_L2_TO_CIM_NR_HS_TIME_OVER_RLY, 0);	
		nBitAddr = m_ShCommData_Master[eP2C_HS_NRTimeOver]->m_shBitAddr;
		bState = m_stpModRunInfo->stEQCmdRlyEvtIO.bHS_NR_TimeOver;
		nSequence = eP2C_HS_NRTimeOver;
	}
	else if(m_stpModRunInfo->stEQCmdRlyEvtIO.bHS_BC_TimeOver == TRUE)
	{
		nBitAddr = B_L2_TO_CIM_BC_HS_TIME_OVER_RLY;
		bState = m_stpModRunInfo->stEQCmdRlyEvtIO.bHS_BC_TimeOver;
		nSequence = eP2C_HS_BCTimeOver;
	}
	else return;
	
	// Event 진행 전 Reply Bit Off. (Reply Bit Off 안된 경우 대비)
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eP2C_HS_NRTimeOver]->m_shBitAddr);

	while(nStep != -1)
	{
		switch(nStep++)
		{
		case 0:	//	이전 진행중인 Sequence가 있는 경우, Request Bit = False 처리 ;
			if ( bState == FALSE)
			{
				m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nBitAddr);
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] HandShake Time Over Event is Canceled!!", 
					m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
				nStep = -1;
				nStepState = STEP_NG;
			}
			else
			{	// (Son Jae Won) HS Time Over 발생 위치(Upper/Lower 판단) 6개 Bit 사용(XXXXXXXX XXOOOOOO)
				m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, W_L2_HS_TIME_OVER_EVENT, &nSize, &shData);
								
				// Host Report Send
				sig.nSignal = sigEQSpecCtrlEvent;
				sig.nFrom	= PLC_TASK_ID;
				sig.nTo		= SCH_TASK_ID;
				sig.unionData.stEQSpecCtrlEventData.nModuleNo = m_stpModCfg->nModuleID;
				sig.unionData.stEQSpecCtrlEventData.nEventId = eSpecGlassHSTimeout;
				sig.unionData.stEQSpecCtrlEventData.nItemVal = shData;
				
				m_pParent->SendSignalToSCH(&sig);
				
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] HandShake Time Over Event : Started!!", 
					m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
				
				m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, nBitAddr);
				m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] HandShake Time Over Event : Reply bit On", 
					m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
				
				m_ShCommData_Master[nSequence]->m_bWaitTime = TRUE;
				m_ShCommData_Master[nSequence]->m_TimeCheck.StartTimer();
				nStep = -1;
				if(nStepState != STEP_NG) 
					nStepState = STEP_OK;
			}
			break;
			
		}
	}
	
	if(nStepState == STEP_NG) 
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] HandShake Time Over Event",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
}

void CBaseModule::P2C_HSTimeOverSequenceEnd()
{
	ULONG	nElaspedTime	=	0;
	long	nSequence		=	0;
	BOOL	bState			=	FALSE;
	long	nRpyBitAddr	=	0;
	
	if(m_ShCommData_Master[eP2C_HS_NRTimeOver]->m_bWaitTime == TRUE)
	{
		nSequence = eP2C_HS_NRTimeOver;
		bState	  = m_stpModRunInfo->stEQCmdRlyEvtIO.bHS_NR_TimeOver;
		nRpyBitAddr = m_ShCommData_Master[eP2C_HS_NRTimeOver]->m_shBitAddr;
	}
	else if(m_ShCommData_Master[eP2C_HS_BCTimeOver]->m_bWaitTime == TRUE)
	{
		nSequence = eP2C_HS_BCTimeOver;
		bState	  = m_stpModRunInfo->stEQCmdRlyEvtIO.bHS_BC_TimeOver;
		nRpyBitAddr = B_L2_TO_CIM_BC_HS_TIME_OVER_RLY;
	}
	else return;
	
	
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	if(bState == TRUE)
	{
		if(m_ShCommData_Master[nSequence]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			nElaspedTime = m_ShCommData_Master[nSequence]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[nSequence]->m_nElaspedTime = nElaspedTime;
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nRpyBitAddr);
			
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] HandShake Time Over Event Time Over!![Elasped Time : %dms]", 
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] HandShake Time Over Event Time Over!![Elasped Time : %dms]", 
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			
			m_ShCommData_Master[nSequence]->m_bWaitTime = FALSE;
		}
		return;
	}
	
	// Host Report Send
/*	sig.nSignal = sigEQSpecCtrlEvent;
	sig.nFrom	= PLC_TASK_ID;
	sig.nTo		= GUI_TASK_ID;
	sig.unionData.stEQSpecCtrlEventData.nModuleNo = m_stpModCfg->nModuleID;
	sig.unionData.stEQSpecCtrlEventData.nEventId = eSpecGlassHSTimeout;
	
	m_pParent->SendSignalToSCH(&sig);
*/	
	// Reply Bit Off
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, nRpyBitAddr);
	
	nElaspedTime = m_ShCommData_Master[nSequence]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[nSequence]->m_nElaspedTime = nElaspedTime;
	
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] HandShake Time Over Event Complete!![Elasped Time : %dms]",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] HandShake Time Over Event Complete!![Elasped Time : %dms]",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	m_ShCommData_Master[nSequence]->m_bWaitTime = FALSE;
}

void CBaseModule::P2C_MNCellValidCheckSequence()
{
	long	nStep			=	0;
	long	nStepState		=	STEP_START;	
	short	shBitAddr		=	0;
	short	shData			=	0;
	long	nSize			=	sizeof(short);
	
	// Event 진행 전 Reply Bit Off. (Reply Bit Off 안된 경우 대비)
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, m_ShCommData_Master[eMNCellValid]->m_shBitAddr);

	while(nStep != -1)
	{
		switch(nStep++)
		{
		case 0:	//	이전 진행중인 Sequence가 있는 경우, Request Bit = False 처리 ;
			if ( m_stpModRunInfo->stDataChangeRlyEvtIO.bMNValidChkReq == FALSE)
			{
				m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, B_L2_MN_VALID_CHK_RLY);
				m_pParent->m_Log[eModuleType_Edge].AddLog("ModuleID[%02d(%s)] Manual Cell Load Data Valid Check Event is Canceled!!", 
					m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
				nStep = -1;
				nStepState = STEP_NG;
			}
			else
			{
				m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, W_L2_MNCEL_VALID_RLY, &nSize, &shData);
				m_ShCommData_Master[eMNCellValid]->m_nCmdData = (long)shData;
				
				m_pParent->m_Log[eModuleType_Edge].AddLog("ModuleID[%02d(%s)] Manual Cell Load Data Valid Check Readed Reply : %d", 
					m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, m_ShCommData_Master[eMNCellValid]->m_nCmdData);
				m_pParent->m_Log[eModuleType_Edge].AddLog("ModuleID[%02d(%s)] Manual Cell Load Data Valid Check Event : Started!!", 
					m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
				
				m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, B_L2_MN_VALID_CHK_RLY);
				m_pParent->m_Log[eModuleType_Edge].AddLog("ModuleID[%02d(%s)] Manual Cell Load Data Valid Check Event : Reply bit On", 
					m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
				
				m_ShCommData_Master[eMNCellValid]->m_bWaitTime = TRUE;
				m_ShCommData_Master[eMNCellValid]->m_TimeCheck.StartTimer();
				nStep = -1;
				if(nStepState != STEP_NG) 
					nStepState = STEP_OK;
			}
			break;
			
		}
	}
	
	if(nStepState == STEP_NG) 
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Manual Cell Load Data Valid Check Event",
		eModuleType_Edge, m_stpModCfg->szModuleName);
}

void CBaseModule::P2C_MNCellValidCheckSequenceEnd()
{
	ULONG	nElaspedTime	=	0;
	short	shRpyBitAddr	=	0;
	
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	if(m_stpModRunInfo->stDataChangeRlyEvtIO.bMNValidChkReq == TRUE)
	{
		if(m_ShCommData_Master[eMNCellValid]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			nElaspedTime = m_ShCommData_Master[eMNCellValid]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eMNCellValid]->m_nElaspedTime = nElaspedTime;
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, B_L2_MN_VALID_CHK_RLY);
			
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Manual Cell Load Data Valid Check Event Time Over!![Elasped Time : %dms]", 
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Manual Cell Load Data Valid Check Event Time Over!![Elasped Time : %dms]", 
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			
			m_ShCommData_Master[eMNCellValid]->m_bWaitTime = FALSE;
		}
		return;
	}
	
	// Host Report Send
	sig.nSignal = sigHostPanelValid;
	sig.nFrom	= PLC_TASK_ID;
	sig.nTo		= GUI_TASK_ID;
	sig.nParam	= m_ShCommData_Master[eMNCellValid]->m_nCmdData;
	
	m_pParent->SendSignalToSCH(&sig);
	
	// Reply Bit Off
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, B_L2_MN_VALID_CHK_RLY);
	
	nElaspedTime = m_ShCommData_Master[eMNCellValid]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[eMNCellValid]->m_nElaspedTime = nElaspedTime;
	
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Manual Cell Load Data Valid Check Event Complete!![Elasped Time : %dms]",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] Manual Cell Load Data Valid Check Event Complete!![Elasped Time : %dms]",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	m_ShCommData_Master[eMNCellValid]->m_bWaitTime = FALSE;
}

//@ [FIC] Not Used
void CBaseModule::P2C_MNCellCancelSequence()
{
/*
	long	nStep			=	0;
	long	nStepState		=	STEP_START;	
	short	shBitAddr		=	0;
	short	shData			=	0;
	short	shSize			=	sizeof(short);
	
	while(nStep != -1)
	{
		switch(nStep++)
		{
		case 0:	//	이전 진행중인 Sequence가 있는 경우, Request Bit = False 처리 ;
			if ( m_stpModRunInfo->stDataChangeRlyEvtIO.bMNCancelReq == FALSE)
			{
				m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, B_L2_MN_CANCEL_REQ);
				m_pParent->m_Log[eModuleType_Edge].AddLog("ModuleID[%02d(%s)] Manual Cell Load Cancel Event is Canceled!!", 
					m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
				nStep = -1;
				nStepState = STEP_NG;
			}
			else
			{
				m_pParent->GetGlassTransferData(&m_stpSma->stSysRunInfo.stScrapData.stPanelInfo);
				
				m_pParent->m_Log[eModuleType_Edge].AddLog("ModuleID[%02d(%s)] Manual Cell Load Cancel Event : Started!!", 
					m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
				
				m_pParent->m_MelLinkMemIF.MelNetDevSetEx(DevB, B_L2_MN_CANCEL_REQ);
				m_pParent->m_Log[eModuleType_Edge].AddLog("ModuleID[%02d(%s)] Manual Cell Load Cancel Event : Reply bit On", 
					m_stpModCfg->nModuleID, m_stpModCfg->szModuleName);
				
				m_ShCommData_Master[eMNCellCancel]->m_bWaitTime = TRUE;
				m_ShCommData_Master[eMNCellCancel]->m_TimeCheck.StartTimer();
				nStep = -1;
				if(nStepState != STEP_NG) 
					nStepState = STEP_OK;
			}
			break;
			
		}
	}
	
	if(nStepState == STEP_NG) 
		m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Manual Cell Load Cancel Event",
		eModuleType_Edge, m_stpModCfg->szModuleName);
*/
}
//@ [FIC] Not Used
void CBaseModule::P2C_MNCellCancelSequenceEnd()
{
/*
	ULONG	nElaspedTime	=	0;
	short	shRpyBitAddr	=	0;
	
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	if(m_stpModRunInfo->stDataChangeRlyEvtIO.bMNCancelReq == TRUE)
	{
		if(m_ShCommData_Master[eMNCellCancel]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
			nElaspedTime = m_ShCommData_Master[eMNCellCancel]->m_TimeCheck.GetTimerAfterStart();
			m_ShCommData_Master[eMNCellCancel]->m_nElaspedTime = nElaspedTime;
			m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, B_L2_MN_CANCEL_REQ);
			
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Manual Cell Load Cancel Event Time Over!![Elasped Time : %dms]", 
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][NG] Manual Cell Load Cancel Event Time Over!![Elasped Time : %dms]", 
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
			
			m_ShCommData_Master[eMNCellCancel]->m_bWaitTime = FALSE;
		}
		return;
	}
	
	// Host Report Send
	sig.nSignal = sigMNCellCancel;
	sig.nFrom	= PLC_TASK_ID;
	sig.nTo		= GUI_TASK_ID;
	
	memcpy(&sig.unionData.stGlassScrapData.stPanelInfo, &m_stpSma->stSysRunInfo.stScrapData.stPanelInfo, sizeof(stPanelInfoType));
	m_pParent->SendSignalToSCH(&sig);
	
	// Reply Bit Off
	m_pParent->m_MelLinkMemIF.MelNetDevRstEx(DevB, B_L2_MN_CANCEL_REQ);
	
	nElaspedTime = m_ShCommData_Master[eMNCellCancel]->m_TimeCheck.GetTimerAfterStart();
	m_ShCommData_Master[eMNCellCancel]->m_nElaspedTime = nElaspedTime;
	
	m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] Manual Cell Load Cancel Event Complete!![Elasped Time : %dms]",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	m_pParent->m_LogMaster.AddLog("ModuleID[%02d(%s)][OK] Manual Cell Load Cancel Event Complete!![Elasped Time : %dms]",
		m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, nElaspedTime);
	
	m_ShCommData_Master[eMNCellCancel]->m_bWaitTime = FALSE;
*/
}

void CBaseModule::OnECIDDataUpdateforStart()
{
	short   shReadData1[1500]	= { 0x00, };
	short	shReadData2[1250]   = { 0x00, };
 	long	nReadDataSize1	= sizeof(short) * 1500;		// Multi ECID
	long	nReadDataSize2	= sizeof(short) * 1250;		// Single ECID

	long	nAddr1			= W_POS1_ECID1_DEF_DATA;
	long	nAddr2			= W_POS1_ECID_SINGLE_DATA;

	long	nDevType		= 0;
	long	i				= 0;
	long	nLeft			= 0;
	long	nIdx			= 0;
	long	nStartAddr		= 0;
	long	nConstantType	= 0;
	long	nNozzleNo		= 0;
	long	nMapIdx			= 0;

	stECIDConfigType	*pECCfg	= NULL;	
	
	// Get Multi Use ECID (USL/UWL/LSL/LWL/DEF)
	nStartAddr = GetPosStartAddr(m_stpModCfg->nModuleID);
	nStartAddr += nAddr1;
	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, nStartAddr, &nReadDataSize1, shReadData1);

	// Get Single Use ECID (DEF)
	nStartAddr = GetPosStartAddr(m_stpModCfg->nModuleID);
	nStartAddr += nAddr2;
	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, nStartAddr, &nReadDataSize2, shReadData2);

	// Update Multi ECID Value 
	for ( i = 0 ; i < m_stpModCfg->stECIDTable.nECIDCount; i++)
	{
		pECCfg = &m_stpModCfg->stECIDTable.stECIDCfg_Multi[i];

		if ( pECCfg == NULL )
		{
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECID(Multi) For Start Event by EQ has ECID CFG Error(NULL)!!", 
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  pECCfg->stParamCfg[0].nMapIndex);
			break;
		}

		if (pECCfg->stParamCfg[0].nMapIndex < 1 || pECCfg->stParamCfg[0].nIndex < 1)
		{
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECID(Multi) For Start Event by EQ has ECID[%d] MapIndex[%d] Error!!", 
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, i,  pECCfg->stParamCfg[0].nMapIndex);
			break;
		}

		if ( pECCfg->stParamCfg[0].nModuleID != m_stpModCfg->nModuleID || pECCfg->stParamCfg[0].nMapIndex > MAX_ECID_MULTI_COUNT ) continue;
		
		// SMA Update
		nIdx = pECCfg->stParamCfg[0].nIndex - 1;
		nMapIdx = pECCfg->stParamCfg[0].nMapIndex -1;

		m_stpModData->stProcData.stECIDData[nMapIdx].nECDefault		= (long)shReadData1[pECCfg->stParamCfg[0].nMapIndex*5 - 5];
		m_stpModData->stProcData.stECIDData[nMapIdx].nECStopLowLimit	= (long)shReadData1[pECCfg->stParamCfg[0].nMapIndex*5 - 4];
		m_stpModData->stProcData.stECIDData[nMapIdx].nECStopUpLimit	= (long)shReadData1[pECCfg->stParamCfg[0].nMapIndex*5 - 3];
		m_stpModData->stProcData.stECIDData[nMapIdx].nECWarnLowLimit	= (long)shReadData1[pECCfg->stParamCfg[0].nMapIndex*5 - 2];
		m_stpModData->stProcData.stECIDData[nMapIdx].nECWarnUpLimit	= (long)shReadData1[pECCfg->stParamCfg[0].nMapIndex*5 - 1];
	}	
	
	// Update Single ECID Value 
	for ( i = 0 ; i < m_stpModCfg->stECIDTable.nECIDCount_Single; i++)
	{
		pECCfg = &m_stpModCfg->stECIDTable.stECIDCfg_Single[i];
		
		if ( pECCfg == NULL )
		{
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECID(Single) For Start Event by EQ has ECID CFG Error(NULL)!!", 
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName,  pECCfg->stParamCfg[0].nMapIndex);
			break;
		}
		
		if (pECCfg->stParamCfg[0].nMapIndex < 1 || pECCfg->stParamCfg[0].nIndex < 1)
		{
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%02d(%s)] ECID(Single) For Start Event by EQ has ECID[%d] MapIndex[%d] Error!!", 
				m_stpModCfg->nModuleID, m_stpModCfg->szModuleName, i,  pECCfg->stParamCfg[0].nMapIndex);
			break;
		}
		
		if ( pECCfg->stParamCfg[0].nModuleID != m_stpModCfg->nModuleID || pECCfg->stParamCfg[0].nMapIndex > MAX_ECID_SINGLE_COUNT ) continue;
		
		nConstantType = pECCfg->stParamCfg[0].nConstantType;
		nMapIdx = pECCfg->stParamCfg[0].nMapIndex -1;
		// SMA Update
		nIdx = pECCfg->stParamCfg[0].nIndex - 1;
		
		m_stpModData->stProcData.stECIDData_Single[nMapIdx].nECDefault		= (long)shReadData2[nMapIdx];
	}	
}

char* CBaseModule::GetHandShakeItem(long nSide, long nItem)
{
	char* Temp[2][64] = 
	{   // SEND
		{
				"IO->OddParity              ",		
				"IO->Pause                  ",
				"IO->Down                   ",
				"IO->Alarm                  ",
				"IO->SendAble               ",
				"IO->SendStart              ",
				"IO->SendComplete           ",
				"IO->ImmPauseReq            ",
				"IO->ReturnRecvStart        ",
				"IO->ReturnRecvComplete     ",
				"IO->ExchangeFlag           ",
				"IO->MultiCarryFlag         ",
				"IO->Reserved1              ",
				"IO->Reserved2              ",
				"IO->Reserved3              ",
				"IO->Reserved4              ",
				"IO->PreAction1             ",
				"IO->PreAction2             ",
				"IO->MidAction1             ",
				"IO->MidAction2             ",
				"IO->PostAction1            ",
				"IO->Reserved5              ",
				"IO->WorkStart              ",
				"IO->WorkCancel             ",
				"IO->HSCancelReq            ",
				"IO->HSAbortReq             ",
				"IO->HSResumeReq            ",
				"IO->HSRecoveryAck          ",
				"IO->HSRecoveryNak          ",
				"IO->Reserved6              ",
				"IO->Reserved7              ",
				"IO->Reserved8              ",
				"CP->Abnormal               ",
				"CP->TypeofArm              ",
				"CP->TypeofStage            ",
				"CP->ManualOp               ",
				"CP->Safety                 ",
				"CP->Empty                  ",
				"CP->Wait                   ",
				"CP->Busy                   ",
				"CP->Pause                  ",
				"CP->Reserved1              ",
				"CP->Arm1Violate            ",
				"CP->Arm2Violate            ",
				"CP->Arm1FoldComplete       ",
				"CP->Arm2FoldComplete       ",
				"CP->Arm1GlassCheck         ",
				"CP->Arm2GlassCheck         ",
				"CP->RobotDirection         ",
				"CP->Reserved2              ",
				"CP->Reserved3              ",
				"CP->Reserved4              ",
				"CP->LiftUp                 ",
				"CP->LiftDown               ",
				"CP->StopperUp              ",
				"CP->StopperDown            ",
				"CP->DoorOpen               ",
				"CP->DoorClose              ",
				"CP->GlassDetect            ",
				"CP->BodyMoving             ",
				"CP->BodyOP                 ",
				"CP->Reserved5              ",
				"CP->Reserved6              ",
				"CP->Reserved7              "				
		},
		{ // RECEIVE
				"IO->EvenParity             ",
				"IO->Pause                  ",
				"IO->Down                   ",
				"IO->Alarm                  ",
				"IO->RecvAble               ",
				"IO->RecvStart              ",
				"IO->RecvComplete           ",
				"IO->ImmPauseReq            ",
				"IO->ReturnSendStart        ",
				"IO->ReturnSendComplete     ",
				"IO->ExchangeFlag           ",
				"IO->MultiCarryFlag         ",
				"IO->Reserved1              ",
				"IO->Reserved2              ",
				"IO->LoadingStop            ",
				"IO->TransferStop           ",
				"IO->PreAction1             ",
				"IO->PreAction2             ",
				"IO->MidAction1             ",
				"IO->MidAction2             ",
				"IO->PostAction1            ",
				"IO->Reserved3              ",
				"IO->ReceiveRefuse          ",
				"IO->GlassIDReadComplete    ",
				"IO->HSCancelReq            ",
				"IO->HSAbortReq             ",
				"IO->HSResumeReq            ",
				"IO->HSRecoveryAck          ",
				"IO->HSRecoveryNak          ",
				"IO->Reserved4              ",
				"IO->Reserved5              ",
				"IO->Reserved6              ",
				"CP->Abnormal               ",
				"CP->TypeofArm              ",
				"CP->TypeofStage            ",
				"CP->ManualOp               ",
				"CP->Safety                 ",
				"CP->Empty                  ",
				"CP->Wait                   ",
				"CP->Busy                   ",
				"CP->Pause                  ",
				"CP->Reserved1              ",
				"CP->Arm1Violate            ",
				"CP->Arm2Violate            ",
				"CP->Arm1FoldComplete       ",
				"CP->Arm2FoldComplete       ",
				"CP->Arm1GlassCheck         ",
				"CP->Arm2GlassCheck         ",
				"CP->RobotDirection         ",
				"CP->Reserved2              ",
				"CP->Reserved3              ",
				"CP->Reserved4              ",
				"CP->LiftUp                 ",
				"CP->LiftDown               ",
				"CP->StopperUp              ",
				"CP->StopperDown            ",
				"CP->DoorOpen               ",
				"CP->DoorClose              ",
				"CP->GlassDetect            ",
				"CP->BodyMoving             ",
				"CP->BodyOP                 ",
				"CP->Reserved5              ",
				"CP->Reserved6              ",
				"CP->Reserved7              "	
			}
	};
	
	return Temp[nSide][nItem];
}

void CBaseModule::CheckEQNetworkStateCheck()
{
	long	i	= 0;
	long	nStationNo	= 0;
	short	shData[3]	= {0x00,};
	long	nSize		= sizeof(short) * 3;
	short	shAddr		= 0;
	long	nHSNo		= 0;
	long	nPosition	= 0;

	BOOL	bEQNetworkState[6] = {0x00,};

	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));

	for (i = 0; i < 6; i++)
	{
		bEQNetworkState[i] = m_stpModRunInfo->stOPModeIO.bNetworkError[i];

		if (m_bEQNetworkState[i] != bEQNetworkState[i])
		{
			switch(i)
			{
			case 0:	//@ Upper1 
				nPosition = 1;	
				nHSNo = 1;		
				break;
			case 1: //@ Lower1 
				nPosition = 2;	
				nHSNo = 1;		
				break;
			case 2: //@ Upper2 
				nPosition = 1;	
				nHSNo = 2;	
				break;
			case 3: //@ Lower2 
				nPosition = 2;  
				nHSNo = 2;		
				break;
			case 4: //@ Upper3 
				nPosition = 1;	
				nHSNo = 3;		
				break;
			case 5: //@ Lower3 
				nPosition = 2;	
				nHSNo = 3;		
				break;
			}

			// Host Report Send
			sig.nSignal = sigEQSpecCtrlEvent;
			sig.nFrom	= PLC_TASK_ID;
			sig.nTo		= SCH_TASK_ID;
			sig.unionData.stEQSpecCtrlEventData.nEventId	= eSpecEQNetworkError;
			sig.unionData.stEQSpecCtrlEventData.nItemId	    = nPosition;
			sig.unionData.stEQSpecCtrlEventData.nItemVal	= bEQNetworkState[i];
			sig.unionData.stEQSpecCtrlEventData.nHSNo	    = nHSNo;
			sig.unionData.stEQSpecCtrlEventData.nModuleNo	= m_stpModCfg->nModuleID;
			
			m_pParent->SendSignalToSCH(&sig);
			
			m_bEQNetworkState[i] = bEQNetworkState[i];
		}

	}
}

void CBaseModule::CheckGECDCrackEvent()
{
	long	i = 0;
	long	nStationNo	= 0;
	short	shData[MAX_GECD_EVENT_COUNT*2]	= {0x00,};		// GECD1 ~ GECD5 (각 2word)
	long	nSize		= sizeof(short) * MAX_GECD_EVENT_COUNT*2;
	short	shAddr		= 0;
	long	nGECDEvent[MAX_GECD_EVENT_COUNT] = {0x00, };
	long	nSemesUniqID[MAX_GECD_EVENT_COUNT] = {0x00,};

	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));

	GetLocalDevBWAddr(eGECDCrackEvent, m_stpModRunInfo->nModuleID, 0, W_L2_GECD1_CRACK_EVENT);
	shAddr = m_ShCommData_Local[eGECDCrackEvent]->m_shDataAddr;

	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, shAddr, &nSize, shData);

	//@ 1st word : Semes UniqueID / 2nd Word : GECD EventID (1: Crack Detect, 2: Crack Relese, 3: Crack Confirm)
	nSemesUniqID[0] = (long)shData[0]; 
	nGECDEvent[0]	= (long)shData[1];
	
	nSemesUniqID[1] = (long)shData[2];
	nGECDEvent[1]	= (long)shData[3];

	nSemesUniqID[2] = (long)shData[3];
	nGECDEvent[2]	= (long)shData[5];

	nSemesUniqID[3] = (long)shData[4];
	nGECDEvent[3]	= (long)shData[7];

	nSemesUniqID[4] = (long)shData[5];
	nGECDEvent[4]	= (long)shData[9];

	for (i = 0; i < MAX_GECD_EVENT_COUNT; i++)
	{
		if (m_nGECDEventID[i] != nGECDEvent[i])
		{
			sig.nSignal = sigGECDCrackEvent;
			sig.nFrom	= PLC_TASK_ID;
			sig.nTo		= SCH_TASK_ID;
			sig.unionData.stGECDCrackEvent.nModuleNo	= m_stpModRunInfo->nModuleID;
			sig.unionData.stGECDCrackEvent.nGECDEventID	= nGECDEvent[i];
			sig.unionData.stGECDCrackEvent.nSemesUniqID	= nSemesUniqID[i];
		
			m_pParent->SendSignalToSCH(&sig);

			m_nGECDEventID[i] = nGECDEvent[i];
		}
	}

}


void CBaseModule::CheckHandShakeValidData()
{	
	long	nStationNo	= 0;
	short	shData[9]	= {0x00,};
	long	nSize		= sizeof(short) * 9;
	short	shAddr		= 0;
	long	nHSID		= 0;
	long	nBitNo		= 0;
	long	nValue		= 0;

	BOOL	bHSValid_Upper1[MAX_HS_VALID_ID_COUNT][16] = {0x00,};
	BOOL	bHSValid_Upper2[MAX_HS_VALID_ID_COUNT][16] = {0x00,};
	BOOL	bHSValid_Upper3[MAX_HS_VALID_ID_COUNT][16] = {0x00,};
	long	nHSValid_SemesUniqID[MAX_HANDSHAKE_COUNT] = {0x00,};

	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));

	GetLocalDevBWAddr(eHandShakeValid, m_stpModRunInfo->nModuleID, 0, W_L2_HS_VALID_DATA_UPPER);
	shAddr = m_ShCommData_Local[eHandShakeValid]->m_shDataAddr;

	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, shAddr, &nSize, shData);

	// Upper1 Check
	nHSValid_SemesUniqID[0] = long(shData[0]);	//@ SEMES UniqueID
	nHSID = long(shData[1]);						//@ HandShake ID
	memcpy(&nValue, &shData[2], sizeof(short));	//@ HandShake Detailed Item

	if (nHSValid_SemesUniqID[0] < 1 || nHSValid_SemesUniqID[0] > MAX_UNIQID_COUNT || nHSID < 0 || nHSID > MAX_HANDSHAKE_COUNT)
		return;

	if(nHSValid_SemesUniqID[0] == 0 || nHSID == 0)	return;

	for (nBitNo = 0; nBitNo < 16; nBitNo++)
	{
		bHSValid_Upper1[nHSID-1][nBitNo] = (nValue >> nBitNo) & 0x0001;

		if (m_bHSValid_Upper1[nHSID-1][nBitNo] != bHSValid_Upper1[nHSID-1][nBitNo])
		{
			if (m_nHSValid_SemesUniqID[0] != nHSValid_SemesUniqID[0] )
			{
				// Host Report Send
				sig.nSignal = sigEQSpecCtrlEvent;
				sig.nFrom	= PLC_TASK_ID;
				sig.nTo		= SCH_TASK_ID;
				sig.unionData.stEQSpecCtrlEventData.nEventId	= eSpecHSCheckFail;
				sig.unionData.stEQSpecCtrlEventData.nItemId	    = nHSID;
				sig.unionData.stEQSpecCtrlEventData.nItemVal	= nBitNo;
				sig.unionData.stEQSpecCtrlEventData.nHSNo		= 1;
				sig.unionData.stEQSpecCtrlEventData.nModuleNo	= m_stpModCfg->nModuleID;
				
				m_pParent->SendSignalToSCH(&sig);
				
				m_bHSValid_Upper1[nHSID][nBitNo]  = bHSValid_Upper1[nHSID][nBitNo];
				m_nHSValid_SemesUniqID[0] = nHSValid_SemesUniqID[0];
			}
		}
	}

	// Upper2 Check
	nHSValid_SemesUniqID[1] = long(shData[3]);	//@ Semes UniqueID
	nHSID = long(shData[4]);						//@ HandShake ID
	memcpy(&nValue, &shData[5], sizeof(short));

	if (nHSValid_SemesUniqID[1] < 1 || nHSValid_SemesUniqID[1] > MAX_UNIQID_COUNT || nHSID < 0 || nHSID > MAX_HANDSHAKE_COUNT)
		return;
	
	if(nHSValid_SemesUniqID[1] == 0 || nHSID == 0)	return;

	for (nBitNo = 0; nBitNo < 16; nBitNo++)
	{
		bHSValid_Upper2[nHSID-1][nBitNo] = (nValue >> nBitNo) & 0x0001;
		
		if (m_bHSValid_Upper2[nHSID-1][nBitNo] != bHSValid_Upper2[nHSID-1][nBitNo])
		{
			if (m_nHSValid_SemesUniqID[1] != nHSValid_SemesUniqID[1] )
			{
				// Host Report Send
				sig.nSignal = sigEQSpecCtrlEvent;
				sig.nFrom	= PLC_TASK_ID;
				sig.nTo		= SCH_TASK_ID;
				sig.unionData.stEQSpecCtrlEventData.nEventId	= eSpecHSCheckFail;
				sig.unionData.stEQSpecCtrlEventData.nItemId	    = nHSID;
				sig.unionData.stEQSpecCtrlEventData.nItemVal	= nBitNo;
				sig.unionData.stEQSpecCtrlEventData.nHSNo		= 2;
				sig.unionData.stEQSpecCtrlEventData.nModuleNo	= m_stpModCfg->nModuleID;
				
				m_pParent->SendSignalToSCH(&sig);
				
				m_bHSValid_Upper2[nHSID][nBitNo]  = bHSValid_Upper2[nHSID][nBitNo];
				m_nHSValid_SemesUniqID[1] = nHSValid_SemesUniqID[1];
			}
		}
	}

	// Upper3 Check
	nHSValid_SemesUniqID[2] = long(shData[6]);	//@ Semes UniqueID
	nHSID = long(shData[7]);						//@ HandShake ID
	memcpy(&nValue, &shData[8], sizeof(short));

	if (nHSValid_SemesUniqID[2] < 1 || nHSValid_SemesUniqID[2] > MAX_UNIQID_COUNT || nHSID < 0 || nHSID > MAX_HANDSHAKE_COUNT)
		return;
	
	if(nHSValid_SemesUniqID[2] == 0 || nHSID == 0)	return;

	for (nBitNo = 0; nBitNo < 16; nBitNo++)
	{
		bHSValid_Upper3[nHSID-1][nBitNo] = (nValue >> nBitNo) & 0x0001;
		
		if (m_bHSValid_Upper3[nHSID-1][nBitNo] != bHSValid_Upper3[nHSID-1][nBitNo])
		{
			if (m_nHSValid_SemesUniqID[2] != nHSValid_SemesUniqID[2] )
			{
				// Host Report Send
				sig.nSignal = sigEQSpecCtrlEvent;
				sig.nFrom	= PLC_TASK_ID;
				sig.nTo		= SCH_TASK_ID;
				sig.unionData.stEQSpecCtrlEventData.nEventId	= eSpecHSCheckFail;
				sig.unionData.stEQSpecCtrlEventData.nItemId	    = nHSID;
				sig.unionData.stEQSpecCtrlEventData.nItemVal	= nBitNo;
				sig.unionData.stEQSpecCtrlEventData.nHSNo		= 3;
				sig.unionData.stEQSpecCtrlEventData.nModuleNo	= m_stpModCfg->nModuleID;
				
				m_pParent->SendSignalToSCH(&sig);
				
				m_bHSValid_Upper3[nHSID][nBitNo]  = bHSValid_Upper3[nHSID][nBitNo];
				m_nHSValid_SemesUniqID[2] = nHSValid_SemesUniqID[2];
			}
		}
	}
}

void CBaseModule::GetGlassDataFromRUNDATA(stSMAUniqueIDTableType *pUniqueIDData, long OwnGlassNo)
{
	long nVal = 0;

	if(pUniqueIDData->nOwnGlassNo == OwnGlassNo)
	{
		CScript Script;
		Script.SetScriptFileName("D:/FPDCIM/BIN/RUNDATA.INI");
		
		char szSectionName[32];
		memset(szSectionName, 0x00, 32);
		
		sprintf(szSectionName, "UNIQUE_ID_DATA_%03d", OwnGlassNo);
		
		//stPanelInfo
		Script.GetString(szSectionName, "szHPanelID", pUniqueIDData->stUniqueIDPanelInfo.szHPanelID);
		FillSpace(pUniqueIDData->stUniqueIDPanelInfo.szHPanelID, MAX_PANEL_ID_LEN);

		Script.GetString(szSectionName, "szEPanelID", pUniqueIDData->stUniqueIDPanelInfo.szEPanelID);
		FillSpace(pUniqueIDData->stUniqueIDPanelInfo.szEPanelID, MAX_PANEL_ID_LEN);
		
		Script.GetString(szSectionName, "szSlotNo", pUniqueIDData->stUniqueIDPanelInfo.szSlotNo);
		FillSpace(pUniqueIDData->stUniqueIDPanelInfo.szSlotNo, MAX_SLOT_ID_LEN);
		
		Script.GetString(szSectionName, "szProcessID", pUniqueIDData->stUniqueIDPanelInfo.szProcessID);
		FillSpace(pUniqueIDData->stUniqueIDPanelInfo.szProcessID, MAX_PROCESS_ID_LEN);
		
		Script.GetString(szSectionName, "szProductID", pUniqueIDData->stUniqueIDPanelInfo.szProductID);
		FillSpace(pUniqueIDData->stUniqueIDPanelInfo.szProductID, MAX_PRODUCT_ID_LEN);
		
		Script.GetString(szSectionName, "szStepID", pUniqueIDData->stUniqueIDPanelInfo.szStepID);
		FillSpace(pUniqueIDData->stUniqueIDPanelInfo.szStepID, MAX_STEP_ID_LEN);
		
		Script.GetString(szSectionName, "szBatchID", pUniqueIDData->stUniqueIDPanelInfo.szBatchID);
		FillSpace(pUniqueIDData->stUniqueIDPanelInfo.szBatchID, MAX_BATCH_ID_LEN);
		
		Script.GetString(szSectionName, "szProdType", pUniqueIDData->stUniqueIDPanelInfo.szProdType);
		FillSpace(pUniqueIDData->stUniqueIDPanelInfo.szProdType, MAX_PRODUCT_TYPE_LEN);
		
		Script.GetString(szSectionName, "szProdKind", pUniqueIDData->stUniqueIDPanelInfo.szProdKind);
		FillSpace(pUniqueIDData->stUniqueIDPanelInfo.szProdKind, MAX_PRODUCT_KIND_LEN);
		
		Script.GetString(szSectionName, "szPPID", pUniqueIDData->stUniqueIDPanelInfo.szPPID);
		FillSpace(pUniqueIDData->stUniqueIDPanelInfo.szPPID, MAX_PPID_LEN);
		
		Script.GetString(szSectionName, "szFlowID", pUniqueIDData->stUniqueIDPanelInfo.szFlowID);
		FillSpace(pUniqueIDData->stUniqueIDPanelInfo.szFlowID, MAX_FLOW_ID_LEN);
		
		Script.GetInt(szSectionName, "shFlowGroup_0", nVal);
		pUniqueIDData->stUniqueIDPanelInfo.shFlowGroup[0] = (short)nVal;
		Script.GetInt(szSectionName, "shFlowGroup_1", nVal);
		pUniqueIDData->stUniqueIDPanelInfo.shFlowGroup[1] = (short)nVal;
		Script.GetInt(szSectionName, "shFlowGroup_2", nVal);
		pUniqueIDData->stUniqueIDPanelInfo.shFlowGroup[2] = (short)nVal;
		Script.GetInt(szSectionName, "shFlowGroup_3", nVal);
		pUniqueIDData->stUniqueIDPanelInfo.shFlowGroup[3] = (short)nVal;
		Script.GetInt(szSectionName, "shFlowGroup_4", nVal);
		pUniqueIDData->stUniqueIDPanelInfo.shFlowGroup[4] = (short)nVal;
		Script.GetInt(szSectionName, "shFlowGroup_5", nVal);
		pUniqueIDData->stUniqueIDPanelInfo.shFlowGroup[5] = (short)nVal;
		Script.GetInt(szSectionName, "shFlowGroup_6", nVal);
		pUniqueIDData->stUniqueIDPanelInfo.shFlowGroup[6] = (short)nVal;
		Script.GetInt(szSectionName, "shFlowGroup_7", nVal);
		pUniqueIDData->stUniqueIDPanelInfo.shFlowGroup[7] = (short)nVal;
		Script.GetInt(szSectionName, "shFlowGroup_8", nVal);
		pUniqueIDData->stUniqueIDPanelInfo.shFlowGroup[8] = (short)nVal;
		Script.GetInt(szSectionName, "shFlowGroup_9", nVal);
		pUniqueIDData->stUniqueIDPanelInfo.shFlowGroup[9] = (short)nVal;

		Script.GetInt(szSectionName, "shUsableChamber_0", nVal);
		pUniqueIDData->stUniqueIDPanelInfo.shUsableChamber[0] = (short)nVal;
		Script.GetInt(szSectionName, "shUsableChamber_1", nVal);
		pUniqueIDData->stUniqueIDPanelInfo.shUsableChamber[1] = (short)nVal;
		Script.GetInt(szSectionName, "shUsableChamber_2", nVal);
		pUniqueIDData->stUniqueIDPanelInfo.shUsableChamber[2] = (short)nVal;
		Script.GetInt(szSectionName, "shUsableChamber_3", nVal);
		pUniqueIDData->stUniqueIDPanelInfo.shUsableChamber[3] = (short)nVal;
		Script.GetInt(szSectionName, "shUsableChamber_4", nVal);
		pUniqueIDData->stUniqueIDPanelInfo.shUsableChamber[4] = (short)nVal;

		Script.GetInt	(szSectionName, "nPanelSize_0", pUniqueIDData->stUniqueIDPanelInfo.nPanelSize[0]);
		Script.GetInt	(szSectionName, "nPanelSize_1", pUniqueIDData->stUniqueIDPanelInfo.nPanelSize[1]);
		Script.GetInt	(szSectionName, "nThickness", pUniqueIDData->stUniqueIDPanelInfo.nThickness);
		Script.GetInt	(szSectionName, "nCompCount", pUniqueIDData->stUniqueIDPanelInfo.nCompCount);

		Script.GetString(szSectionName, "szGrade", pUniqueIDData->stUniqueIDPanelInfo.szGrade );
		FillSpace(pUniqueIDData->stUniqueIDPanelInfo.szGrade, MAX_CELL_GRADE_LEN);

		Script.GetString(szSectionName, "szJudgement", pUniqueIDData->stUniqueIDPanelInfo.szJudgement);
		FillSpace(pUniqueIDData->stUniqueIDPanelInfo.szJudgement, MAX_JUDGEMENT_RESULT_LEN);

		Script.GetString(szSectionName, "szCode", pUniqueIDData->stUniqueIDPanelInfo.szCode);
		FillSpace(pUniqueIDData->stUniqueIDPanelInfo.szCode, MAX_JUDGEMENT_CODE_LEN);

		Script.GetString(szSectionName, "szCount1", pUniqueIDData->stUniqueIDPanelInfo.szCount1);
		FillSpace(pUniqueIDData->stUniqueIDPanelInfo.szCount1, MAX_FLOW_ID_LEN);
		
		Script.GetString(szSectionName, "szCount2", pUniqueIDData->stUniqueIDPanelInfo.szCount2);
		FillSpace(pUniqueIDData->stUniqueIDPanelInfo.szCount2, MAX_GLASS_COUNT_LEN);
		
		Script.GetString(szSectionName, "szPanelPosition", pUniqueIDData->stUniqueIDPanelInfo.szPanelPosition);
		FillSpace(pUniqueIDData->stUniqueIDPanelInfo.szPanelPosition, MAX_GLASS_POSITION_LEN);

		Script.GetInt	(szSectionName, "nOwnGlassNo", pUniqueIDData->stUniqueIDPanelInfo.nOwnGlassNo);
		Script.GetInt	(szSectionName, "nUniqueID_0", pUniqueIDData->stUniqueIDPanelInfo.nUniqueID[0]);
		Script.GetInt	(szSectionName, "nUniqueID_1", pUniqueIDData->stUniqueIDPanelInfo.nUniqueID[1]);
		Script.GetInt	(szSectionName, "nUniqueID_2", pUniqueIDData->stUniqueIDPanelInfo.nUniqueID[2]);
		Script.GetInt	(szSectionName, "nUniqueID_3", pUniqueIDData->stUniqueIDPanelInfo.nUniqueID[3]);

		Script.GetString(szSectionName, "szReadingFlag", pUniqueIDData->stUniqueIDPanelInfo.szReadingFlag);
		FillSpace(pUniqueIDData->stUniqueIDPanelInfo.szReadingFlag, MAX_READING_FLAG_LEN);

		Script.GetString(szSectionName, "szMultiUse", pUniqueIDData->stUniqueIDPanelInfo.szMultiUse);
		FillSpace(pUniqueIDData->stUniqueIDPanelInfo.szMultiUse, MAX_MULTI_USE_LEN);

		Script.GetString(szSectionName, "szPairHPanel", pUniqueIDData->stUniqueIDPanelInfo.stPairPanelInfo.szPairHPanelID);
		FillSpace(pUniqueIDData->stUniqueIDPanelInfo.stPairPanelInfo.szPairHPanelID, MAX_PANEL_ID_LEN);
		
		Script.GetString(szSectionName, "szPairEPanel", pUniqueIDData->stUniqueIDPanelInfo.stPairPanelInfo.szPairEPanelID);
		FillSpace(pUniqueIDData->stUniqueIDPanelInfo.stPairPanelInfo.szPairEPanelID, MAX_PANEL_ID_LEN);
		
		Script.GetString(szSectionName, "szPairGrade", pUniqueIDData->stUniqueIDPanelInfo.stPairPanelInfo.szPairGrade);
		FillSpace(pUniqueIDData->stUniqueIDPanelInfo.stPairPanelInfo.szPairGrade, MAX_CELL_GRADE_LEN);
		
		Script.GetString(szSectionName, "szPairProcID", pUniqueIDData->stUniqueIDPanelInfo.stPairPanelInfo.szPairProductID);
		FillSpace(pUniqueIDData->stUniqueIDPanelInfo.stPairPanelInfo.szPairProductID, MAX_PRODUCT_ID_LEN);

		Script.GetInt(szSectionName, "shReferData_0", nVal);
		pUniqueIDData->stUniqueIDPanelInfo.shReferData[0] = (short)nVal;
		Script.GetInt(szSectionName, "shReferData_1", nVal);
		pUniqueIDData->stUniqueIDPanelInfo.shReferData[1] = (short)nVal;

		Script.GetInt(szSectionName, "nPanelState", pUniqueIDData->stUniqueIDPanelInfo.nPanelState);

		Script.GetInt(szSectionName, "JobStart", pUniqueIDData->stUniqueIDPanelInfo.stBitSignal.nJobStartBit);
		Script.GetInt(szSectionName, "JobEnd", pUniqueIDData->stUniqueIDPanelInfo.stBitSignal.nJobEndBit);

	}
}

void CBaseModule::FillSpace(char *pGlassDataItem, size_t nSize)
{
	long nlen = 0;
	char *szBuffer;
	szBuffer = (char *)malloc((nSize +1) * sizeof(char));
	
	nlen = sprintf(szBuffer, pGlassDataItem);
	memset(pGlassDataItem, 0x20, nSize);
	memcpy(pGlassDataItem, szBuffer, nlen);
	
	free(szBuffer);
}

void CBaseModule::SetNotifyEvent(long nECode)
{
	m_stpSma->stSmaError.nErrorCode = nECode;
	m_stpSma->stSmaError.bEvent = TRUE;
}

///////////////////// 미사용 //////////////////////////////////////////////////////////////////////////////////
//	20060206 : Recovery Mode Sequence
void CBaseModule::C2P_RecoveryModeSequence()
{
/*
long	nStep			= 0;
short	shAddr			= 0;

  short	shCmdPacket[4]	= { 0x00, };
  long	nTimeout		= 0;
  long	i				= 0;
  
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));
	
	  long	nStationNo = m_stpModCfg->nModuleID;
	  if ( nStationNo > 1 )
	  shAddr = B_L1_SECOND_START_ADDRESS + B_L1_EACH_REQ_RLY_SIZE * (nStationNo - 2);
	  else
	  shAddr = B_L1_SECOND_START_ADDRESS;
	  
		shAddr += 2;
		
		  m_ShCommData_Master[eC2P_RecoveryMode]->m_shBitAddr = shAddr;
		  
			while(nStep != -1) 
			{
			switch(nStep++)
			{
			case 0:	//	Reply가 응답진행중인 Sequence가 있는 경우, Request Bit = False 처리 ;
			if ( m_stpModRunInfo->stSecondReply.bCollectMode )
			{
			m_pParent->m_MelLinkMemIF.MelNetDevRst(DevB, shAddr);
			//				Sleep(100);
			m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%s] CollectMode Reply[%d] Bit On", m_stpModCfg->nModuleID,m_stpModRunInfo->stSecondReply.bProcAvailCheck);
			nStep = -1;
			
			  }
			  break;
			  case 1:	//	Job 예약 Data는 이미 Update되어 있으므로 각 제어기별로 Request만 한다.
			  //			Sleep(1000);
			  m_pParent->m_MelLinkMemIF.MelNetDevSet(DevB, shAddr);
			  m_pParent->m_MelLinkMemIF.MelNetDevSet(DevB, 3042);	//Coater Req
			  m_pParent->m_MelLinkMemIF.MelNetDevSet(DevB, 3062);	//VCD Req
			  
				
				  //JJS TEST 2006-08-09
				  m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("<TEST>ModuleID[%s] CollectMode Request[%d] Bit On", m_stpModCfg->nModuleID,shAddr);
				  //			Sleep(300);
				  m_ShCommData_Master[eC2P_RecoveryMode]->m_bWaitTime = TRUE;
				  m_ShCommData_Master[eC2P_RecoveryMode]->m_TimeCheck.StartTimer();
				  nStep = -1;
				  break;
				  }
				  }
	*/
}

BOOL CBaseModule::SetRPCState(long nModuleNo, char *szH_PanelID, char *szJobID)
{
	long nRPCCount = 0;
	
	stRPCDataTableType *pstSmaRPC = NULL;
	pstSmaRPC = &m_stpSma->stSysRunInfo.stRPCTbl;
	
	char szSMAHPanelID[MAX_PANEL_ID_LEN + 1] = {0,};
	
	_Trim(szH_PanelID);
	
	for (nRPCCount = 0; nRPCCount < pstSmaRPC->nRPCCount; nRPCCount++)
	{
		if (nModuleNo == pstSmaRPC->stRPCData[nRPCCount].nModuleNo)
		{
			memcpy(szSMAHPanelID, pstSmaRPC->stRPCData[nRPCCount].szH_PanelID, MAX_PANEL_ID_LEN);
	
			_Trim(szSMAHPanelID);
			
			if (strncmp(szH_PanelID, szSMAHPanelID, MAX_PANEL_ID_LEN) == 0)
			{
				pstSmaRPC->stRPCData[nRPCCount].bUsed = TRUE;
				return TRUE;
			}
		}
	}
	
	return FALSE;
}

BOOL CBaseModule::OnCheckRPCData(long nModuleNo, char *szH_PanelID, stRPCDataType *stRPCData)
{
	long nRPCCount = 0;
	
	stRPCDataTableType *pstSmaRPC = NULL;
	pstSmaRPC = &m_stpSma->stSysRunInfo.stRPCTbl;
	
	char szSMAHPanelID[MAX_PANEL_ID_LEN + 1] = {0,};
	
	_Trim(szH_PanelID);

	for (nRPCCount = 0; nRPCCount < pstSmaRPC->nRPCCount; nRPCCount++)
	{
		if (nModuleNo == pstSmaRPC->stRPCData[nRPCCount].nModuleNo)
		{
			memcpy(szSMAHPanelID, pstSmaRPC->stRPCData[nRPCCount].szH_PanelID, MAX_PANEL_ID_LEN);
			
			_Trim(szSMAHPanelID);
			
			if (strncmp(szH_PanelID, szSMAHPanelID, MAX_PANEL_ID_LEN) == 0)
			{
				memcpy(stRPCData->szRPC_PPID, pstSmaRPC->stRPCData[nRPCCount].szRPC_PPID, MAX_PPID_LEN);
				return TRUE;
			}
		}
	}
	
	return FALSE;
}

void CBaseModule::C2P_RecoveryModeSequenceEnd()
{
/*
ULONG	nElaspedTime	= 0;
short	shAddr			= m_ShCommData_Master[eC2P_RecoveryMode]->m_shBitAddr;

		//	각제어기의 Reply 유무를 Check한다.
		if(!m_stpModRunInfo->stSecondReply.bCollectMode || m_stpModRunInfo->stSecondReply.bCollectMode )
		{
		if(m_ShCommData_Master[eC2P_RecoveryMode]->m_TimeCheck.MoreThan(DEF_TIME_OVER)) 
		{
		m_pParent->m_MelLinkMemIF.MelNetDevRst(DevB, 3042);	//Coater Req
		m_pParent->m_MelLinkMemIF.MelNetDevRst(DevB, 3062); //VCD Req
		m_pParent->m_MelLinkMemIF.MelNetDevRst(DevB, shAddr);
		
		  nElaspedTime = m_ShCommData_Master[eC2P_RecoveryMode]->m_TimeCheck.GetTimerAfterStart();
		  m_ShCommData_Master[eC2P_RecoveryMode]->m_nElaspedTime = nElaspedTime;
		  m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%s] Recovery Mode Timeout", m_stpModCfg->nModuleID, nElaspedTime);
		  
			m_ShCommData_Master[eC2P_RecoveryMode]->m_bWaitTime = FALSE;
			}
			return;
			}
			
			  nElaspedTime = m_ShCommData_Master[eC2P_RecoveryMode]->m_TimeCheck.GetTimerAfterStart();
			  m_ShCommData_Master[eC2P_RecoveryMode]->m_nElaspedTime = nElaspedTime;
			  if ( m_stpModRunInfo->stSecondReply.nCollectMode == 1 || 
			  m_stpModRunInfo->stSecondReply.nCollectMode == 2 || 
			  m_stpModRunInfo->stSecondReply.nCollectMode == 0x41 )
			  m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%s] Recovery Mode[%d] (Time %d)Ack Receive Complete!!", m_stpModCfg->nModuleID,m_stpModRunInfo->stSecondReply.nCollectMode, nElaspedTime);
			  else
			  m_pParent->m_Log[m_stpModCfg->nModuleID].AddLog("ModuleID[%s] Recovery Mode[%d] (Time %d)Nak Receive Complete!!", m_stpModCfg->nModuleID,m_stpModRunInfo->stSecondReply.nCollectMode, nElaspedTime);
			  
				//	Request Bit를 On하여 Maching Command를 수행하도록 한다.
				//		Sleep(100);
				m_pParent->m_MelLinkMemIF.MelNetDevRst(DevB, shAddr);
				m_pParent->m_MelLinkMemIF.MelNetDevRst(DevB, 3042);	//Coater Req
				m_pParent->m_MelLinkMemIF.MelNetDevRst(DevB, 3062);	//VCD Req
				
				  m_ShCommData_Master[eC2P_RecoveryMode]->m_bWaitTime = FALSE;
	*/
}

//@ Module별 설비 가동 Data Read..
void CBaseModule::GetEQModuleRunData()
{
	long	nStationNo	= 0;
	short	shData[MAX_MODULE_RUN_DATA_COUNT]	= {0x00,};
	long	nSize		= sizeof(short) * MAX_MODULE_RUN_DATA_COUNT;
	short	shAddr		= 0;
	
	stQueueRootType sig;
	memset(&sig, 0x00, sizeof(stQueueRootType));

	nStationNo = m_stpModCfg->nModuleID;

	GetLocalDevBWAddr(eModuleRunData, nStationNo, 0, W_L2_MODULE_RUN_DATA);
	shAddr = m_ShCommData_Local[eModuleRunData]->m_shDataAddr;

	m_pParent->m_MelLinkMemIF.MelNetReceiveEx(DevW, shAddr, &nSize, shData);

	m_stpModRunInfo->stModuleRunData.nRuningTime = (long)shData[eRunData_RunningTime];
	m_stpModRunInfo->stModuleRunData.nErrorCount = (long)shData[eRunData_ErrorCount];
	m_stpModRunInfo->stModuleRunData.nErrorTime  = (long)shData[eRunData_ErrorTime];
	m_stpModRunInfo->stModuleRunData.nMTBF		 = (long)shData[eRunData_MTBF];
	m_stpModRunInfo->stModuleRunData.nMTTR		 = (long)shData[eRunData_MTTR];
	m_stpModRunInfo->stModuleRunData.nRunningRate= (long)shData[eRunData_RunningRate];

	m_stpModRunInfo->stModuleRunData.nTotalProductCnt =(long)shData[eRunData_TotalGlassCnt];
	m_stpModRunInfo->stModuleRunData.nRWGlassCnt = (long)shData[eRunData_RWGlassCnt];


	//@ MMI로 RunData 로그 기록을 위하여 전송.
	sig.nSignal = sigRunDataLog;
	sig.nFrom   = PLC_TASK_ID;
	sig.nTo		= GUI_TASK_ID;
	sig.nParam  = nStationNo;

	m_pParent->SendSignalToMMI(&sig);
}
/*!
@brief	공백 제거 함수
*/
void CBaseModule::_Trim(char* szString)
{
	if(szString != NULL)
	{
		char *sp, *head, *tail;
		
		sp = szString;
		head = tail = NULL;
		
		while(*sp!='\0') 
		{
			if(*sp != ' ') 
			{
				tail = sp;
				if(head == '\0') head = sp;
			}
			sp++;
		}
		
		if(tail) 
		{
			*(tail + 1) = '\0';
			while(*head != '\0')
			{
				*szString++ = *head++;
			}
		}
		
		*szString = '\0';
	}
}