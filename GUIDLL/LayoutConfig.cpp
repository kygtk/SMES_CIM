#include	"LayoutConfig.h"
#include	"SmaHandle.h"
#include	<stdio.h>

//--------------------------------------------------------------------------------------
/*
 * Layer1의 Module 개수를 얻는다.
 */
long WINAPI smGetLayer1ModuleCount()
{
	long nModuleCount=0;
	
	stLayOutCfgType* pLayoutCfg=&pstSma->stLayOutCfg;
	
	nModuleCount=pLayoutCfg->nModuleCount;

	if (nModuleCount > 0 &&  nModuleCount <= MAX_LAYER1_MODULE_COUNT) 
		return nModuleCount;

	return dGUIERROR;
}

void WINAPI smSetLayer1ModuleCount(long nModuleCount)
{
	stLayOutCfgType* pLayoutCfg=&pstSma->stLayOutCfg;
	pLayoutCfg->nModuleCount=nModuleCount;
}

//--------------------------------------------------------------------------------------
/*
 * Layer2의 Module 개수를 얻는다.
 */
long WINAPI smGetLayer2ModuleCount(long nModule)
{
	long i = 0;
	long nUnitCount=0;

	stModuleCfgType	*pModCfg = GetModCfgInfo(nModule);
	if ( pModCfg == NULL )	return dGUIERROR;

	if ( nModule == pModCfg->nModuleID)
	{
		nUnitCount = pModCfg->nUnitCount;

		if (nUnitCount > 0 &&  nUnitCount <= MAX_LAYER2_MODULE_COUNT) 
			return nUnitCount;
	}

	return dGUIERROR;
}

long WINAPI smSetLayer2ModuleCount(long nModule, long nUnitCount)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModule);
	if ( pModCfg == NULL )	return dGUIERROR;

	if ( nModule == pModCfg->nModuleID)
	{
		pModCfg->nUnitCount=nUnitCount;
	}

	return dSUCCESS;
}

//--------------------------------------------------------------------------------------
// Operator ID
void WINAPI smSetOperatorID(char *szOperatorID)
{
	stLayOutCfgType *pLayoutcfg=&pstSma->stLayOutCfg;
	memcpy(pLayoutcfg->szOperatorID, szOperatorID, MAX_OPERATOR_ID_LEN);
}

void WINAPI smGetOperatorID(char *szOperatorID)
{
	stLayOutCfgType *pLayoutcfg=&pstSma->stLayOutCfg;
	memcpy(szOperatorID, pLayoutcfg->szOperatorID,MAX_OPERATOR_ID_LEN);
}

//--------------------------------------------------------------------------------------
// Melsec Config

void WINAPI smSetMelsecBoardCount(long nBoardCount)
{
	stLayOutCfgType* pLayoutCfg=&pstSma->stLayOutCfg;

	pLayoutCfg->nMelsecBoardCount = nBoardCount;
}

void WINAPI smSetMelsecConfig(long nBoardIndex, stMelsecBoardConfigType *pDstMelsecCfg)
{
	stLayOutCfgType* pLayoutCfg=&pstSma->stLayOutCfg;
	
	if(nBoardIndex > 0)
		memcpy(&pLayoutCfg->stMelsecConfig[nBoardIndex-1], pDstMelsecCfg, sizeof(stMelsecBoardConfigType));
}

//--------------------------------------------------------------------------------------
/*
	System Config
*/
/*
long WINAPI smSetSystemConfig(stLayOutCfgType* stSrcLayout)
{
	stLayOutCfgType* stLayoutCfg=&pstSma->stLayOutCfg;
	if(stLayoutCfg == NULL) return dGUIERROR;

	memcpy(stLayoutCfg,stSrcLayout, sizeof(stLayOutCfgType));
	return dSUCCESS;
}

long WINAPI smGetSystemConfig(stLayOutCfgType* stDstLayout)
{
	stLayOutCfgType* stLayoutCfg=&pstSma->stLayOutCfg;
	if(stLayoutCfg == NULL) return dGUIERROR;

	memcpy(stDstLayout,stLayoutCfg, sizeof(stLayOutCfgType));
	return dSUCCESS;
}
*/

//--------------------------------------------------------------------------------------



void WINAPI smSetModuleUsed(long nModuleNo, long nUsed)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	
	if ( pModCfg != NULL ) {
		//Module 번호가 같으면 Config 저장
		if(nModuleNo == pModCfg->nModuleID)
		{
			pModCfg->bModuleUsed=nUsed;
		}
	}
}

long WINAPI smGetModuleUsed(long nModuleNo)
{
	
	BOOL bModuleUsed = FALSE;

	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);

	if ( pModCfg == NULL )	return dGUIERROR;
	
	//Module 번호가 같으면 Config 저장
	if(nModuleNo == pModCfg->nModuleID)
	{
		bModuleUsed = pModCfg->bModuleUsed;
	}

	return bModuleUsed;
}


//------------------------------------------------
/*  
	Unit Config
	GUI에서 읽어들인 Layout 정보를 SMA로 저장
*/ 
long WINAPI smSetUnitConfig(long nModuleNo, long nUnitNo, stUnitCfgType *pSrcUnitCfg)
{
	stUnitCfgType *stUnitCfg = GetUnitCfgInfo(nModuleNo, nUnitNo);
	if(stUnitCfg == NULL )	return dGUIERROR;

	memcpy(stUnitCfg, pSrcUnitCfg, sizeof(stUnitCfgType));

	return dSUCCESS;
}

long WINAPI smGetUnitConfig(long nModuleNo, long nUnitNo, stUnitCfgType *pDstUnitCfg)
{
	stUnitCfgType *stUnitCfg = GetUnitCfgInfo(nModuleNo, nUnitNo);
	if(stUnitCfg == NULL )	return dGUIERROR;

	if(nModuleNo == stUnitCfg->nModuleID && nUnitNo == stUnitCfg->nUnitID)
	{
		memcpy(pDstUnitCfg, stUnitCfg, sizeof(stUnitCfgType));
	}

	return dSUCCESS;
}

//--------------------------------------------------------------------------------------
/*
 * 상하류 설비 ID 얻기
 */
long WINAPI	smGetOtherEQID(long nModuleNo, long nIndex, stOtherEQPIDType* stEQPID)
{	
	stOtherEQPIDType* stOtherEQPID=&pstSma->stLayOutCfg.stModCfg[nModuleNo].stOtherEQPID[nIndex-1];
	memcpy(stEQPID, stOtherEQPID, sizeof(stOtherEQPIDType));

	return dSUCCESS;
}

/*
 * 상하류설비 ID 설정하기
 */
void WINAPI smSetOtherEQID(long nModuleNo, long nIndex, stOtherEQPIDType* stEQPID)
{
	stOtherEQPIDType* stOtherEQPID=&pstSma->stLayOutCfg.stModCfg[nModuleNo].stOtherEQPID[nIndex-1];
	memcpy(stOtherEQPID, stEQPID,sizeof(stOtherEQPIDType));
}

//--------------------------------------------------------------------------------------
/*
 * 설비 ID 얻기
 */
long	WINAPI	smGetEQID(char* szEQID)
{
	stLayOutCfgType* pLayoutCfg=&pstSma->stLayOutCfg;
	memcpy(szEQID, pLayoutCfg->szEQPID, MAX_EQ_MODULE_ID_LEN);

	return dSUCCESS;
}

/*
 * 설비 ID 설정하기
 */
void WINAPI smSetEQID(char* szEQID)
{
	stLayOutCfgType* pLayoutCfg=&pstSma->stLayOutCfg;
	if(szEQID != NULL)
		memcpy(pLayoutCfg->szEQPID, szEQID, MAX_EQ_MODULE_ID_LEN);
}

//--------------------------------------------------------------------------------------
/*
 * 설비군 타입		1:PHOTO, 2:ETCH/STRIP INLINE ,3:ETCH, 4:STRIP, 5:PFC, 6:EDGE  
 */
long WINAPI smGetEQType()
{
	stLayOutCfgType* pLayoutCfg=&pstSma->stLayOutCfg;
	return pLayoutCfg->nEQType;
}

long WINAPI smGetEQSubType()
{
	stLayOutCfgType* pLayoutCfg=&pstSma->stLayOutCfg;
	return pLayoutCfg->nEQSubType;
}

long WINAPI smGetEQNumber()
{
	stLayOutCfgType* pLayoutCfg=&pstSma->stLayOutCfg;
	return pLayoutCfg->nEQNumber;
}

//설비 진행 방향을 얻는다.
long WINAPI smGetProcessDirection()
{
	stLayOutCfgType* pLayoutCfg=&pstSma->stLayOutCfg;
	return pLayoutCfg->nProcessDirection;
};

void WINAPI smSetProcessDirection(long nDirectoin)
{
	stLayOutCfgType* pLayoutCfg=&pstSma->stLayOutCfg;
	pLayoutCfg->nProcessDirection=nDirectoin ;
};


//--------------------------------------------------------------------------------------
/*
 * Soft Version 얻기
 */
long	WINAPI	smGetSoftVersion(char* szVersion)
{
	stLayOutCfgType* pLayoutCfg=&pstSma->stLayOutCfg;
	//memcpy(szVersion, pLayoutCfg->szSOFTREV , MAX_SOFT_REVISION_LEN);

	return dSUCCESS;
}

//--------------------------------------------------------------------------------------
/* 
 * 설비 Model Number 얻기
 */
long WINAPI smGetModelNumber(char* szModelName)
{
	stLayOutCfgType* pLayoutCfg=&pstSma->stLayOutCfg;

	memcpy(szModelName, pLayoutCfg->szModelName, MAX_MODEL_NUMBER_LEN);

	return dSUCCESS;
}

/* 
 * Model Number 설정하기
 */
void WINAPI smSetModelNumber(char* szModelName)
{
	stLayOutCfgType* pLayoutCfg=&pstSma->stLayOutCfg;

	memcpy(pLayoutCfg->szModelName, szModelName, MAX_MODEL_NUMBER_LEN);
}

//--------------------------------------------------------------------------------------
/*
 * Module Name 얻어오기
 */
long WINAPI smGetModuleName(long nModule, char* szModuleName)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModule);
	if ( pModCfg == NULL )	return dGUIERROR;

	memcpy(szModuleName, pModCfg->szModuleName,  MAX_LAYER_MODULE_ID_LEN);

	return dSUCCESS ;
}

/*
 * Module ID 설정하기
 */
long WINAPI smSetModuleName(long nModule, char* szModuleName)
{
	if(nModule > 0 && nModule <= MAX_LAYER1_MODULE_COUNT)
	{
		if(szModuleName != NULL)
		{
			stModuleCfgType *pModCfg = GetModCfgInfo(nModule);
			if(pModCfg == NULL )	return dGUIERROR;

			memcpy(pModCfg->szModuleName, szModuleName, MAX_LAYER_MODULE_ID_LEN);
		}
	}

	return dSUCCESS ;
}

/*
===========================================================
  Module 정보 설정하기
===========================================================
 */

/*
 * Module Dscription 얻어오기
 */
long WINAPI smGetModuleDesc(long nModuleNo, char* szModuleDesc)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	memcpy(szModuleDesc, pModCfg->szModuleDesc,  MAX_MODULE_DESCRIPT_LEN);
	return dSUCCESS ;
}

void WINAPI smSetModuleDesc(long nModuleNo, char* szModuleDesc)
{	
	if(nModuleNo > 0 && nModuleNo <= MAX_LAYER1_MODULE_COUNT)
	{
		stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
		memcpy(pModCfg->szModuleDesc, szModuleDesc, MAX_MODULE_DESCRIPT_LEN);
	}
}

long WINAPI smGetPLCVersion(long nModuleNo, char* szPLCVersion)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	memcpy(szPLCVersion, pModCfg->szPLCVersion,  MAX_PLC_VERSION_LEN);
	return dSUCCESS ;
}

void WINAPI smSetPLCVersion(long nModuleNo, char* szPLCVersion)
{	
	if(nModuleNo > 0 && nModuleNo <= MAX_LAYER1_MODULE_COUNT)
	{
		if(szPLCVersion != NULL)
		{
			stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
			memcpy(pModCfg->szPLCVersion, szPLCVersion,MAX_PLC_VERSION_LEN);
		}
	}
}

long WINAPI smGetModuleNumber(long nModuleNo)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	return pModCfg->nModuleID;
}

long WINAPI smSetModuleNumber(long nModuleNo)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	pModCfg->nModuleID=nModuleNo;

	stModuleRunInfoType *pModRunInfo=GetModRunInfo(nModuleNo);
	pModRunInfo->nModuleID=nModuleNo;

	stModuleDataInfoType *pModDataInfo=GetModDataInfo(nModuleNo);
	pModDataInfo->nModuleID=nModuleNo;

	return dSUCCESS;
}

long WINAPI smGetModelType(long nModuleNo)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	return pModCfg->nModelType;
}

long WINAPI smSetModelType(long nModuleNo, long nModelType)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	pModCfg->nModelType=nModelType;
	return dSUCCESS;
}

long WINAPI smGetMakerType(long nModuleNo)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	return pModCfg->nMakerType;
}

long WINAPI smSetMakerType(long nModuleNo, long nMakerType)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	pModCfg->nMakerType=nMakerType;
	return dSUCCESS;
}

long WINAPI smGetUnitCount(long nModuleNo)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	return pModCfg->nUnitCount;
}

long WINAPI smSetUnitCount(long nModuleNo, long nUnitCount)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	pModCfg->nUnitCount=nUnitCount;
	return dSUCCESS;
}

long WINAPI smGetRobotCount(long nModuleNo)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	return pModCfg->nRobotCount;
}

long WINAPI smSetRobotCount(long nModuleNo, long nRobotCount)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	pModCfg->nRobotCount=nRobotCount;
	return dSUCCESS;
}

long WINAPI smGetPortCount(long nModuleNo)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	return pModCfg->nPortCount;
}

long WINAPI smSetPortCount(long nModuleNo, long nPortCount)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	pModCfg->nPortCount=nPortCount;

	return dSUCCESS;
}

long WINAPI smGetUsedMelsecNetCount(long nModuleNo)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	return pModCfg->nUsedMelCount;
}

long WINAPI smSetUsedMelsecNetCount(long nModuleNo, long nCount)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	pModCfg->nUsedMelCount=nCount;

	return dSUCCESS;
}

long WINAPI smGetModuleMelsecConfig(long nModuleNo, long nIndex, stMelsecConfigForModuleType *pSrcConfig)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	
	memcpy(pSrcConfig,&pModCfg->stMelsecCfg[nIndex-1], sizeof(stMelsecConfigForModuleType));

	return dSUCCESS;
}

long WINAPI smSetModuleMelsecConfig(long nModuleNo, long nIndex, stMelsecConfigForModuleType *pSrcConfig)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	
	memcpy(&pModCfg->stMelsecCfg[nIndex-1], pSrcConfig, sizeof(stMelsecConfigForModuleType));

	return dSUCCESS;
}

long WINAPI smGetHandshakeCount(long nModuleNo)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	return pModCfg->nHandShakeCount;
}

long WINAPI smSetHandshakeCount(long nModuleNo, long nCount)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);

	if(pModCfg != NULL)
		pModCfg->nHandShakeCount = nCount;

	return dSUCCESS;
}

long  WINAPI smGetHandshakeConfig(long nModuleNo, long nHSIndex, stHandShakeConfigForModuleType *pSrcHandshake)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	
	memcpy(pSrcHandshake,&pModCfg->stHandShakeCfg[nHSIndex-1], sizeof(stHandShakeConfigForModuleType));

	return dSUCCESS;
}

void WINAPI smSetHandshakeConfig(long nModuleNo, long nHSIndex, stHandShakeConfigForModuleType *pSrcHandshake)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	
	memcpy(&pModCfg->stHandShakeCfg[nHSIndex-1], pSrcHandshake,sizeof(stHandShakeConfigForModuleType));
}

/*
long WINAPI smGetUsedSerial(long nModuleNo)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	return pModCfg->nUsedSerialComm;
}

void WINAPI smSetUsedSerial(long nModuleNo, long nUsedSerial)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	pModCfg->nUsedSerialComm=nUsedSerial;
}

long WINAPI smGetSerialConfig(long nModuleNo, stSerialCommCfgType* pSrcSerialCfg)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);

	memcpy(pSrcSerialCfg, &pModCfg->stSerialCfg,  sizeof(stSerialCommCfgType));

	return dSUCCESS;
}

void WINAPI smSetSerialConfig(long nModuleNo, stSerialCommCfgType* pSrcSerialCfg)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);

	memcpy(&pModCfg->stSerialCfg, &pSrcSerialCfg, sizeof(stSerialCommCfgType));
}
*/

long WINAPI smGetTerminalID(long nModuleNo)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	return pModCfg->nTerminalID;
}

void WINAPI smSetTerminalID(long nModuleNo, long nTerminalID)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	pModCfg->nTerminalID=nTerminalID;
}

long WINAPI smGetAlarmCount(long nModuleNo)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	return pModCfg->nAlarmCount;
}

void WINAPI smSetAlarmCount(long nModuleNo, long nCount)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	pModCfg->nAlarmCount=nCount;
}

long WINAPI smGetRobotArmCount(long nModuleNo)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	return pModCfg->nRobotArmCount;
}

void WINAPI smSetRobotArmCount(long nModuleNo, long nCount)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	pModCfg->nRobotArmCount =nCount;
}

long WINAPI smGetUsedBuffer(long nModuleNo)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	return pModCfg->nUsedBuff;
}

void WINAPI smSetUsedBuffer(long nModuleNo, long nUsedBuffer)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	pModCfg->nUsedBuff =nUsedBuffer;
}

long WINAPI smGetUsedBufferSlotCount(long nModuleNo)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	return pModCfg->nUsedBuffSlotCount;
}

void WINAPI smSetUsedBufferSlotCount(long nModuleNo, long nCount)
{
	stModuleCfgType	*pModCfg = GetModCfgInfo(nModuleNo);
	pModCfg->nUsedBuffSlotCount  =nCount;
}

//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
/* 
 * Unit의 이름을 얻는다.
 */
long WINAPI smGetUnitName(long nModule, long nUnit, char* szUnitName)
{
	if(nModule > 0 && nModule <= MAX_LAYER1_MODULE_COUNT)
	{
		stModuleCfgType	*pModCfg = GetModCfgInfo(nModule);
		if ( pModCfg == NULL )	return dGUIERROR;

		stUnitCfgType* stUnitCfg=&pModCfg->stUnitCfg[nUnit-1];

		memcpy(szUnitName, stUnitCfg->szUnitName, MAX_UNIT_ID_LEN);
	}
		
	return dSUCCESS ;
}

/* 
 * Unit의 이름을 저장.
 */
long WINAPI smSetUnitName(long nModule, long nUnit, char* szUnitName)
{
	if(nModule > 0 && nModule <= eModuleType_Macro)
	{
		stModuleCfgType	*pModCfg = GetModCfgInfo(nModule);
		if ( pModCfg == NULL )	return dGUIERROR;

		stUnitCfgType* stUnitCfg=&pModCfg->stUnitCfg[nUnit-1];

		memcpy(stUnitCfg->szUnitName, szUnitName, MAX_UNIT_ID_LEN);
	}
	return dSUCCESS;
}

//--------------------------------------------------------------------------------------
/* 
 * Unit의 Description을 얻는다.
 */
long WINAPI smGetUnitDesc(long nModule, long nUnit, char* szUnitDesc)
{
	if(nModule > 0 && nModule <= MAX_LAYER1_MODULE_COUNT)
	{
		stModuleCfgType	*pModCfg = GetModCfgInfo(nModule);
		if ( pModCfg == NULL )	return dGUIERROR;

		stUnitCfgType* stUnitCfg=&pModCfg->stUnitCfg[nUnit-1];

		memcpy(szUnitDesc, stUnitCfg->szUnitDesc, MAX_UNIT_DESCRIPT_LEN);
	}
		
	return dSUCCESS ;
}
/*--------------------------------------------------------------------------------------
						Equipment Online Paramter
----------------------------------------------------------------------------------------*/
long WINAPI	smGetEOIDParam(stEOIDTableType* pDstEOID)
{
	stEOIDTableType* stEOID=&pstSma->stLayOutCfg.stEOIDTable;
	memcpy(pDstEOID, stEOID, sizeof(stEOIDTableType));

	return dSUCCESS;
}

//
void WINAPI	smSetEOIDParamTable(stEOIDTableType* pSrcEOIDTable)
{
	stModuleCfgType	*pModCfg = NULL;
	stEOIDTableType* stEOIDTable=&pstSma->stLayOutCfg.stEOIDTable;
	memcpy(stEOIDTable, pSrcEOIDTable, sizeof(stEOIDTableType));

	long i = 0;
	long j = 0;
	long k = 0;
	long nIdx = 0;
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1] = {0x00, };
	memset(szModuleID, 0x20, MAX_EQ_MODULE_ID_LEN);

	GetEQPID(szModuleID);
	for( i=0; i<pstSma->stLayOutCfg.stEOIDTable.nEOIDCount; i++ )
	{
		switch(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOID)
		{
		case eEOID_New_GlassTrace_IN:
			for( j=0; j<pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOMDCount; j++ )
			{
				if( strncmp(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].szEOMD, szModuleID, MAX_EQ_MODULE_ID_LEN) == 0)
				{
					pstSma->stSysData.stEvtData.nSelect_GlassIN = pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].nEOV;
				}
			}
			break;
		case eEOID_New_GlassTrace_OUT:
			for( j=0; j<pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOMDCount; j++ )
			{
				if( strncmp(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].szEOMD, szModuleID, MAX_EQ_MODULE_ID_LEN) == 0)
				{
					pstSma->stSysData.stEvtData.nSelect_GlassOUT = pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].nEOV;
				}
			}
			break;
		case eEOID_New_EQStateTrace:
			for( j=0; j<pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOMDCount; j++ )
			{
				if( strncmp(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].szEOMD, szModuleID, MAX_EQ_MODULE_ID_LEN) == 0)
				{
					pstSma->stSysData.stEvtData.nSelectEQState = pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].nEOV;
				}
			}
			break;
		case eEOID_New_EQProcStateTrace:
			for( j=0; j<pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOMDCount; j++ )
			{
				if( strncmp(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].szEOMD, szModuleID, MAX_EQ_MODULE_ID_LEN) == 0)
				{
					pstSma->stSysData.stEvtData.nSelectProcState = pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].nEOV;
				}
			}
			break;
		}
	}

	if( ISInline())
	{
		for( k = 0; k < GetLayer1ModuleCount(); k++ )
		{
			pModCfg		= GetModuleConfig(k+2);
			if ( pModCfg == NULL )	continue;
			if ( pModCfg->bModuleUsed == FALSE)	continue;

			memset(szModuleID, 0x20, MAX_EQ_MODULE_ID_LEN);
			GetEQPID(szModuleID, k+2);

			for( i=0; i<pstSma->stLayOutCfg.stEOIDTable.nEOIDCount; i++ )
			{
				switch(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOID)
				{
				case eEOID_New_GlassTrace_IN:
					for( j=0; j<pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOMDCount; j++ )
					{
						if( strncmp(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].szEOMD, szModuleID, MAX_EQ_MODULE_ID_LEN) == 0)
						{
							pstSma->stSysData.stMODDataInfo[k+2].stEvtData.nSelect_GlassIN = pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].nEOV;
						}
					}
					break;
				case eEOID_New_GlassTrace_OUT:
					for( j=0; j<pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOMDCount; j++ )
					{
						if( strncmp(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].szEOMD, szModuleID, MAX_EQ_MODULE_ID_LEN) == 0)
						{
							pstSma->stSysData.stMODDataInfo[k+2].stEvtData.nSelect_GlassOUT = pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].nEOV;
						}
					}
					break;
				case eEOID_New_EQStateTrace:
					for( j=0; j<pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOMDCount; j++ )
					{
						if( strncmp(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].szEOMD, szModuleID, MAX_EQ_MODULE_ID_LEN) == 0)
						{
							pstSma->stSysData.stMODDataInfo[k+2].stEvtData.nSelectEQState = pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].nEOV;
						}
					}
					break;
				case eEOID_New_EQProcStateTrace:
					for( j=0; j<pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOMDCount; j++ )
					{
						if( strncmp(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].szEOMD, szModuleID, MAX_EQ_MODULE_ID_LEN) == 0)
						{
							pstSma->stSysData.stMODDataInfo[k+2].stEvtData.nSelectProcState = pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].nEOV;
						}
					}
					break;
				}
			}
		}
	}

	for( k = 0; k < GetLayer1ModuleCount(); k++ )
	{
		pModCfg		= GetModuleConfig(k+2);
		if ( pModCfg == NULL )	continue;
		if ( pModCfg->bModuleUsed == FALSE)	continue;
		
		for( nIdx = 0; nIdx < pModCfg->nUnitCount; nIdx++ )
		{		
			memset(szModuleID, 0x20, MAX_EQ_MODULE_ID_LEN);
			GetEQPID(szModuleID, k+2, nIdx+1);

			for( i=0; i<pstSma->stLayOutCfg.stEOIDTable.nEOIDCount; i++ )
			{
				switch(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOID)
				{
				case eEOID_New_GlassTrace_IN:
					for( j=0; j<pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOMDCount; j++ )
					{
						if( strncmp(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].szEOMD, szModuleID, MAX_EQ_MODULE_ID_LEN) == 0)
						{
							pstSma->stSysData.stMODDataInfo[k+2].stUnitData[nIdx].stEvtData.nSelect_GlassIN = pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].nEOV;
						}
					}
					break;
				case eEOID_New_GlassTrace_OUT:
					for( j=0; j<pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOMDCount; j++ )
					{
						if( strncmp(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].szEOMD, szModuleID, MAX_EQ_MODULE_ID_LEN) == 0)
						{
							pstSma->stSysData.stMODDataInfo[k+2].stUnitData[nIdx].stEvtData.nSelect_GlassOUT = pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].nEOV;
						}
					}
					break;
				case eEOID_New_EQStateTrace:
					for( j=0; j<pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOMDCount; j++ )
					{
						if( strncmp(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].szEOMD, szModuleID, MAX_EQ_MODULE_ID_LEN) == 0)
						{
							pstSma->stSysData.stMODDataInfo[k+2].stUnitData[nIdx].stEvtData.nSelectEQState = pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].nEOV;
						}
					}
					break;
				case eEOID_New_EQProcStateTrace:
					for( j=0; j<pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOMDCount; j++ )
					{
						if( strncmp(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].szEOMD, szModuleID, MAX_EQ_MODULE_ID_LEN) == 0)
						{
							pstSma->stSysData.stMODDataInfo[k+2].stUnitData[nIdx].stEvtData.nSelectProcState = pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].nEOV;
						}
					}
					break;
				}
			}
		}
	}

}

void WINAPI	smSetEOIDParamData(long nIndex, stEOIDDataType* pSrcEOID)
{
	stEOIDDataType* stEOID=&pstSma->stLayOutCfg.stEOIDTable.stEOIDData[nIndex];
	memcpy(stEOID, pSrcEOID, sizeof(stEOIDDataType));
}


/*--------------------------------------------------------------------------------------
			State Variable ID
----------------------------------------------------------------------------------------*/
long WINAPI	smSetSVIDData(long nModuleNo, long nIndex, stSVIDConfigType* pSrcSVID)
{
	stSVIDConfigType* stSVID = &pstSma->stLayOutCfg.stModCfg[nModuleNo].stSVIDTable.stSVIDCfg[nIndex-1];
	memcpy(stSVID, pSrcSVID ,sizeof(stSVIDConfigType));

	return dSUCCESS;
}

void WINAPI	smGetSVIDData(long nModuleNo, long nIndex, stSVIDConfigType* pDstSVID)
{
	stSVIDConfigType* stSVID = &pstSma->stLayOutCfg.stModCfg[nModuleNo].stSVIDTable.stSVIDCfg[nIndex-1];
	memcpy(pDstSVID, stSVID,  sizeof(stSVIDConfigType));
}

void WINAPI	smSetSVIDCount(long nModuleNo, long nCount)
{
	stSVIDTableType* stTable=&pstSma->stLayOutCfg.stModCfg[nModuleNo].stSVIDTable;

	stTable->nSVIDCount=nCount; 
}

long WINAPI	smGetSVIDCount(long nModuleNo)
{
	stSVIDTableType* stTable=&pstSma->stLayOutCfg.stModCfg[nModuleNo].stSVIDTable;

	return stTable->nSVIDCount; 
}

/*--------------------------------------------------------------------------------------
			Equipment Constant ID
----------------------------------------------------------------------------------------*/
long WINAPI	smSetECIDData(long nModuleNo, long nIndex, stECIDConfigType* pSrcECID)
{
	stECIDConfigType* stECID=&pstSma->stLayOutCfg.stModCfg[nModuleNo].stECIDTable.stECIDCfg_Multi[nIndex-1];
	memcpy(stECID, pSrcECID, sizeof(stECIDConfigType));

	return dSUCCESS;
}
long WINAPI	smSetECIDData_Single(long nModuleNo,long nIndex, stECIDConfigType* pSrcECID)
{
	stECIDConfigType* stECID=&pstSma->stLayOutCfg.stModCfg[nModuleNo].stECIDTable.stECIDCfg_Single[nIndex-1];
	memcpy(stECID, pSrcECID, sizeof(stECIDConfigType));
	
	return dSUCCESS;
}
long WINAPI	smGetECIDData(long nModuleNo, long nIndex, stECIDConfigType* pDstECID)
{
	stECIDConfigType* stECID=&pstSma->stLayOutCfg.stModCfg[nModuleNo].stECIDTable.stECIDCfg_Multi[nIndex-1];
	memcpy(pDstECID, stECID, sizeof(stECIDConfigType));

	return dSUCCESS;
}
long WINAPI	smGetECIDData_Single(long nModuleNo, long nIndex, stECIDConfigType* pDstECID)
{
	stECIDConfigType* stECID=&pstSma->stLayOutCfg.stModCfg[nModuleNo].stECIDTable.stECIDCfg_Single[nIndex-1];
	memcpy(pDstECID, stECID, sizeof(stECIDConfigType));
	
	return dSUCCESS;
}

long WINAPI	smGetECIDData_SV_Mapping(long nModuleNo, long nSVECIndex, stECIDConfigType* pDstECID)
{
	long nIndex = 0;
	
	for (nIndex = 0; nIndex < MAX_ECID_MULTI_COUNT; nIndex++)
	{	
		stECIDConfigType* stECID= &pstSma->stLayOutCfg.stModCfg[nModuleNo].stECIDTable.stECIDCfg_Multi[nIndex-1];
		
		if (nSVECIndex == stECID->stParamCfg[0].nSVECIndex)
		{
			memcpy(pDstECID, stECID, sizeof(stECIDConfigType));
			break;
		}
	}
	
	return dSUCCESS;
}

void WINAPI	smSetECIDCount(long nModuleNo, long nCount)
{
	stECIDTableType* stTable=&pstSma->stLayOutCfg.stModCfg[nModuleNo].stECIDTable;

	stTable->nECIDCount=nCount; 
}
void WINAPI	smSetECIDCount_Single(long nModuleNo, long nCount)
{
	stECIDTableType* stTable=&pstSma->stLayOutCfg.stModCfg[nModuleNo].stECIDTable;
	
	stTable->nECIDCount_Single= nCount; 
}

long WINAPI	smGetECIDCount(long nModuleNo)
{
	stECIDTableType* stTable=&pstSma->stLayOutCfg.stModCfg[nModuleNo].stECIDTable;

	return stTable->nECIDCount; 
}
long WINAPI	smGetECIDCount_Single(long nModuleNo)
{
	stECIDTableType* stTable=&pstSma->stLayOutCfg.stModCfg[nModuleNo].stECIDTable;
	
	return stTable->nECIDCount_Single; 
}

/*--------------------------------------------------------------------------------------
			Data Collection Value ID
----------------------------------------------------------------------------------------*/
long WINAPI	smSetDVIDData(long nModuleNo, long nIndex, stDVIDConfigType* pSrcDVID)
{
	stDVIDConfigType* stDVID =&pstSma->stLayOutCfg.stModCfg[nModuleNo].stDVIDTable.stDVIDCfg[nIndex-1];
	memcpy(stDVID , pSrcDVID , sizeof(stDVIDConfigType));

	return dSUCCESS;
}

long WINAPI	smGetDVIDData(long nModuleNo, long nIndex, stDVIDConfigType* pDstDVID)
{
	stDVIDConfigType* stDVID =&pstSma->stLayOutCfg.stModCfg[nModuleNo].stDVIDTable.stDVIDCfg[nIndex-1];
	memcpy(pDstDVID , stDVID , sizeof(stDVIDConfigType));

	return dSUCCESS;
}

void WINAPI	smSetDVIDCount(long nModuleNo, long nCount)
{
	stDVIDTableType* stTable=&pstSma->stLayOutCfg.stModCfg[nModuleNo].stDVIDTable;
	stTable->nDVIDCount=nCount; 
}

long WINAPI	smGetDVIDCount(long nModuleNo)
{
	stDVIDTableType* stTable=&pstSma->stLayOutCfg.stModCfg[nModuleNo].stDVIDTable;
	return stTable->nDVIDCount; 
}

/*--------------------------------------------------------------------------------------
			EQ Interlock List
----------------------------------------------------------------------------------------*/
void WINAPI smGetEQInterlock(long nModuleNo, long nItemNo, stEQInterlockListType* pDstEQInterlock)
{	
	stModuleRunInfoType* stModRunInfo=GetModRunInfo(nModuleNo);
	stEQInterlockListType* stEQInterlock=NULL;

	if(nItemNo > 0)
	{
		stEQInterlock=&stModRunInfo->stEQInterlockTable.stEQInterlock[0][nItemNo-1];		
		memcpy(pDstEQInterlock, stEQInterlock, sizeof(stEQInterlockListType));
	}


	
};

void WINAPI smSetEQInterlock(long nModuleNo, long nItemNo, stEQInterlockListType* pSrcEQInterlock)
{	
	stModuleRunInfoType* stModRunInfo=GetModRunInfo(nModuleNo);
	stEQInterlockListType* stEQInterlock=NULL;

	if(nItemNo>0)
	{
		stEQInterlock=&stModRunInfo->stEQInterlockTable.stEQInterlock[0][nItemNo-1];		
		memcpy(stEQInterlock, pSrcEQInterlock, sizeof(stEQInterlockListType));
	}

};

long WINAPI smGetEQInterlockCount(long nModuleNo)
{
	stModuleRunInfoType* stModRunInfo=GetModRunInfo(nModuleNo);
	stEQInterlockTableType* stEQInterlockTable=NULL;
	stEQInterlockTable=&stModRunInfo->stEQInterlockTable;

	return stEQInterlockTable->nInterlockCount;
}

void WINAPI smSetEQInterlockCount(long nModuleNo, long nCount)
{
	stModuleRunInfoType* stModRunInfo=GetModRunInfo(nModuleNo);
	stEQInterlockTableType* stEQInterlockTable=NULL;
	stEQInterlockTable=&stModRunInfo->stEQInterlockTable;
	
	stEQInterlockTable->nModuleNo=nModuleNo;
	stEQInterlockTable->nInterlockCount=nCount;
}

/*--------------------------------------------------------------------------------------
			Event Control
----------------------------------------------------------------------------------------*/
// Equipment Event Control을 얻는다.
long WINAPI smGetEQEvtCtrl(stEventCtrlDataType* pDstEvtCtrl)
{
	stEventCtrlDataType* stEventCtrl=NULL;
	stEventCtrl=&pstSma->stSysData.stEvtData;
	memcpy(pDstEvtCtrl, stEventCtrl, sizeof(stEventCtrlDataType));
	
	return dSUCCESS;
}

void WINAPI smSetEQEvtCtrl(stEventCtrlDataType* pSrcEvtCtrl)
{
	stEventCtrlDataType* stEventCtrl = &pstSma->stSysData.stEvtData;

	memcpy(stEventCtrl, pSrcEvtCtrl,  sizeof(stEventCtrlDataType));
	
	long i = 0;
	long j = 0;
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1] = {0x00, };
	memset(szModuleID, 0x20, MAX_EQ_MODULE_ID_LEN);
	
	GetEQPID(szModuleID);
	
	for( i=0; i<pstSma->stLayOutCfg.stEOIDTable.nEOIDCount; i++ )
	{
		switch(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOID)
		{
		case eEOID_New_GlassTrace_IN:
			for( j=0; j<pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOMDCount; j++ )
			{
				if( strncmp(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].szEOMD, szModuleID, MAX_EQ_MODULE_ID_LEN) == 0)
				{
					pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].nEOV = stEventCtrl->nSelect_GlassIN;
				}
			}
			break;
		case eEOID_New_GlassTrace_OUT:
			for( j=0; j<pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOMDCount; j++ )
			{
				if( strncmp(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].szEOMD, szModuleID, MAX_EQ_MODULE_ID_LEN) == 0)
				{
					pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].nEOV = stEventCtrl->nSelect_GlassOUT;
				}
			}
			break;
		case eEOID_New_EQStateTrace:
			for( j=0; j<pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOMDCount; j++ )
			{
				if( strncmp(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].szEOMD, szModuleID, MAX_EQ_MODULE_ID_LEN) == 0)
				{
					pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].nEOV = stEventCtrl->nSelectEQState;
				}
			}
			break;
		case eEOID_New_EQProcStateTrace:
			for( j=0; j<pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOMDCount; j++ )
			{
				if( strncmp(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].szEOMD, szModuleID, MAX_EQ_MODULE_ID_LEN) == 0)
				{
					pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].nEOV = stEventCtrl->nSelectProcState;
				}
			}
			break;
		}
	}
}

// Module Level Event Control을 얻는다.
long WINAPI smGetModuleEvtCtrl(long nModule, stEventCtrlDataType* pDstEvtCtrl)
{
	stModuleDataInfoType *pModData=NULL;
	stEventCtrlDataType* stEventCtrl=NULL;

	if(nModule > 0) //Module Data
	{
		pModData=GetModDataInfo(nModule);

		if(pModData == NULL )	return dGUIERROR;

		stEventCtrl=&pModData->stEvtData; 
	}

	memcpy(pDstEvtCtrl, stEventCtrl, sizeof(stEventCtrlDataType));
	
	return dSUCCESS;
}

// Module별 Event Control을 SMA에 저장
long WINAPI smSetModuleEvtCtrl(long nModule, stEventCtrlDataType* pSrcEvtCtrl)
{
	stModuleDataInfoType *pModData=NULL;
	stEventCtrlDataType* stEventCtrl=NULL;
	
	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1] = {0x00, };
	memset(szModuleID, 0x20, MAX_EQ_MODULE_ID_LEN);
	long i = 0;
	long j = 0;
	
	if(nModule > 0) //Module Data
	{
		pModData=GetModDataInfo(nModule);

		if(pModData == NULL )	return dGUIERROR;

		stEventCtrl=&pModData->stEvtData; 
	}
	
	memcpy(stEventCtrl, pSrcEvtCtrl, sizeof(stEventCtrlDataType));
	
	GetEQPID(szModuleID, nModule);
	
	for( i=0; i<pstSma->stLayOutCfg.stEOIDTable.nEOIDCount; i++ )
	{
		switch(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOID)
		{
		case eEOID_New_GlassTrace_IN:
			for( j=0; j<pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOMDCount; j++ )
			{
				if( strncmp(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].szEOMD, szModuleID, MAX_EQ_MODULE_ID_LEN) == 0)
				{
					pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].nEOV = stEventCtrl->nSelect_GlassIN;
				}
			}
			break;
		case eEOID_New_GlassTrace_OUT:
			for( j=0; j<pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOMDCount; j++ )
			{
				if( strncmp(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].szEOMD, szModuleID, MAX_EQ_MODULE_ID_LEN) == 0)
				{
					pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].nEOV = stEventCtrl->nSelect_GlassOUT;
				}
			}
			break;
		case eEOID_New_EQStateTrace:
			for( j=0; j<pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOMDCount; j++ )
			{
				if( strncmp(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].szEOMD, szModuleID, MAX_EQ_MODULE_ID_LEN) == 0)
				{
					pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].nEOV = stEventCtrl->nSelectEQState;
				}
			}
			break;
		case eEOID_New_EQProcStateTrace:
			for( j=0; j<pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOMDCount; j++ )
			{
				if( strncmp(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].szEOMD, szModuleID, MAX_EQ_MODULE_ID_LEN) == 0)
				{
					pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].nEOV = stEventCtrl->nSelectProcState;
				}
			}
			break;
		}
	}
	
	return dSUCCESS;
}


// Unit별 Event Control을 얻는다.
long WINAPI smGetUnitEvtCtrl(long nModule, long nUnit, stEventCtrlDataType* pDstEvtCtrl)
{
	if(nModule > 0) //Module Data
	{
		stModuleDataInfoType *pModData=GetModDataInfo(nModule);
		if(pModData == NULL )	return dGUIERROR;

		stEventCtrlDataType* stEventCtrl=&pModData->stUnitData[nUnit-1].stEvtData; 
		memcpy(pDstEvtCtrl, stEventCtrl, sizeof(stEventCtrlDataType));
	}
	
	return dSUCCESS;
}

// Unit별 Event Control을 SMA에 저장
long WINAPI smSetUnitEvtCtrl(long nModule, long nUnit, stEventCtrlDataType* pSrcEvtCtrl)
{

	stEventCtrlDataType* stEventCtrl = NULL;

	char	szModuleID[MAX_EQ_MODULE_ID_LEN+1] = {0x00, };
	memset(szModuleID, 0x20, MAX_EQ_MODULE_ID_LEN);
	long i = 0;
	long j = 0;

	if(nModule > 0) //Module Data
	{
		stModuleDataInfoType *pModData=GetModDataInfo(nModule);
		if(pModData == NULL )	return dGUIERROR;
		
		stEventCtrl = &pModData->stUnitData[nUnit-1].stEvtData; 
		memcpy(stEventCtrl, pSrcEvtCtrl, sizeof(stEventCtrlDataType));
	}
	
	GetEQPID(szModuleID, nModule, nUnit);
	
	for( i=0; i<pstSma->stLayOutCfg.stEOIDTable.nEOIDCount; i++ )
	{
		switch(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOID)
		{
		case eEOID_New_GlassTrace_IN:
			for( j=0; j<pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOMDCount; j++ )
			{
				if( strncmp(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].szEOMD, szModuleID, MAX_EQ_MODULE_ID_LEN) == 0)
				{
					pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].nEOV = stEventCtrl->nSelect_GlassIN;
				}
			}
			break;
		case eEOID_New_GlassTrace_OUT:
			for( j=0; j<pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOMDCount; j++ )
			{
				if( strncmp(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].szEOMD, szModuleID, MAX_EQ_MODULE_ID_LEN) == 0)
				{
					pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].nEOV = stEventCtrl->nSelect_GlassOUT;
				}
			}
			break;
		case eEOID_New_EQStateTrace:
			for( j=0; j<pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOMDCount; j++ )
			{
				if( strncmp(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].szEOMD, szModuleID, MAX_EQ_MODULE_ID_LEN) == 0)
				{
					pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].nEOV = stEventCtrl->nSelectEQState;
				}
			}
			break;
		case eEOID_New_EQProcStateTrace:
			for( j=0; j<pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].nEOMDCount; j++ )
			{
				if( strncmp(pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].szEOMD, szModuleID, MAX_EQ_MODULE_ID_LEN) == 0)
				{
					pstSma->stLayOutCfg.stEOIDTable.stEOIDData[i].stEOMDData[j].nEOV = stEventCtrl->nSelectProcState;
				}
			}
			break;
		}
	}
	
	return dSUCCESS;
}

//---------------------- Software 버젼 얻기 ----------------------------
void WINAPI smGetSoftwareVesrion(stSystemVerionType *pDstSoftRev)
{
	stSystemVerionType *stSoftRev = &pstSma->stVersionType;
	memcpy(pDstSoftRev, stSoftRev, sizeof(stSystemVerionType));
}

void WINAPI smSetSoftwareVersion(stSystemVerionType *pSrcSoftRev)
{
	stSystemVerionType *stSoftRev = &pstSma->stVersionType;
	memcpy(stSoftRev, pSrcSoftRev, sizeof(stSystemVerionType));
}

void WINAPI smSetGUIVersion(char *szVersion)
{
	stSoftTaskRevType *stSoftRev=&pstSma->stLayOutCfg.stSoftTaskRev;
	strcpy(stSoftRev->szCIMRevCIM, szVersion);
}
/*==========================================================================
 *	Get Common Pointer
 *==========================================================================*/
stLayOutCfgType *GetSysCfgInfo()
{
	return &pstSma->stLayOutCfg;;
}

stModuleCfgType *GetModCfgInfo(long nModuleNo)
{
	stLayOutCfgType* pSysCfg = &pstSma->stLayOutCfg;

	if(nModuleNo>0)
		return &pSysCfg->stModCfg[nModuleNo];	// 1 Base
		//return &pSysCfg->stModCfg[nModuleNo-1];

	return NULL;
}

stUnitCfgType *GetUnitCfgInfo(long nModuleNo, long nUnit)
{
	stLayOutCfgType* pSysCfg = &pstSma->stLayOutCfg;

	if(nModuleNo > 0 && nUnit > 0 )
		return &pSysCfg->stModCfg[nModuleNo].stUnitCfg[nUnit-1];

	return NULL;
}


//Tracking Module Info T8Y 추가 
void WINAPI smGetTrackingInfo(long nModuleNo, stTrackingInfoType *pDstTrackingInfo)
{
	stTrackingInfoType *stTrackingInfo=&pstSma->stLayOutCfg.stModCfg[nModuleNo].stTrackingInfo;
	memcpy(pDstTrackingInfo, stTrackingInfo, sizeof(stTrackingInfoType));
}

void WINAPI smSetTrackingInfo(long nModuleNo, stTrackingInfoType *pSrcTrackingInfo)
{
	stTrackingInfoType *stTrackingInfo=&pstSma->stLayOutCfg.stModCfg[nModuleNo].stTrackingInfo;
	memcpy(stTrackingInfo, pSrcTrackingInfo, sizeof(stTrackingInfoType));
}

void WINAPI smSetWaterjetUse(BOOL bWaterjetUse)
{
	pstSma->stLayOutCfg.bWaterjetUse = bWaterjetUse;
}


//====================================================================================================================
// RPC
//====================================================================================================================

/*!
@brief RPC Table Data를 지운다.
*/

BOOL IsRPCRange(long nItemNo)
{
	if ( nItemNo > 0 && nItemNo <= MAX_RPC_QUEUE_COUNT )	
		return TRUE;
	
	return FALSE;
}

void WINAPI smClearRPCData(long nModuleNo, long nRPCNo)
{
	if (IsRPCRange(nRPCNo))
	{
		stRPCDataType* pstRPCData = &pstSma->stSysRunInfo.stRPCTbl.stRPCData[nRPCNo-1];
		memset(pstRPCData, 0x00, sizeof(stRPCDataType));
	}
}

/*!
@brief	RPC Data의 정보 설정하고 얻는다.
*/
void WINAPI smRPCData(long nGetSet, long nModuleNo, long nRPCIndex, stRPCDataType* pRPC)
{
	if (IsRPCRange(nRPCIndex))
	{
		stRPCDataType* pSrc = &pstSma->stSysRunInfo.stRPCTbl.stRPCData[nRPCIndex-1];
		
		switch(nGetSet)
		{
		case DLL_GET:	CopyMemory(pRPC, pSrc, sizeof(stRPCDataType));	break;
		case DLL_SET:	CopyMemory(pSrc, pRPC, sizeof(stRPCDataType));	break;
		}
	}
}

// @brief	Insert RPC Data 정보 설정하고 얻는다.

void WINAPI smInsertRPCData(long nGetSet, stRPCDataType* pRPC)
{
	stRPCDataType* pSrc = &pstSma->stSysRunInfo.stSCHEventData.stInsertRPCData;
	
	switch(nGetSet)
	{
	case DLL_GET:	CopyMemory(pRPC, pSrc, sizeof(stRPCDataType));	break;
	case DLL_SET:	CopyMemory(pSrc, pRPC, sizeof(stRPCDataType));	break;
	}
}

/*!
@brief	Delete RPC Data 정보 설정하고 얻는다.
*/
void WINAPI smDeleteRPCData(long nGetSet, stRPCDataType	*pRPC)
{
	stRPCDataType* pSrc = &pstSma->stSysRunInfo.stSCHEventData.stDeleteRPCData;
	
	switch(nGetSet)
	{
	case DLL_GET:	CopyMemory(pRPC, pSrc, sizeof(stRPCDataType));	break;
	case DLL_SET:	CopyMemory(pSrc, pRPC, sizeof(stRPCDataType));	break;
	}
}

/*!
@brief	StateChange RPC Data 정보 설정하고 얻는다.
*/
void WINAPI smStateChangeRPCData(long nGetSet, stRPCDataType* pRPC)
{
	stRPCDataType* pSrc = &pstSma->stSysRunInfo.stSCHEventData.stStateChangeRPCData;
	
	switch(nGetSet)
	{
	case DLL_GET:	CopyMemory(pRPC, pSrc, sizeof(stRPCDataType));	break;
	case DLL_SET:	CopyMemory(pSrc, pRPC, sizeof(stRPCDataType));	break;
	}
}

long WINAPI smEditRPCCount(long nGetSet, long nModuleNo, long *nCount)
{
	stRPCDataTableType*	pSrc = &pstSma->stSysRunInfo.stRPCTbl;
	
	switch(nGetSet)
	{
	case DLL_GET: CopyMemory(nCount, &pSrc->nRPCCount, sizeof(long));	break;
	case DLL_SET: CopyMemory(&pSrc->nRPCCount, nCount, sizeof(long));	break;
	}
	
	return dSUCCESS;
}