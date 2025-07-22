  // InIineSma.cpp: implementation of the InIineSma class.
//
//////////////////////////////////////////////////////////////////////

#include "SysLib.h"
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SysLib::SysLib()
{

}   

SysLib::~SysLib()
{

}

BOOL SysLib::SmaInitialize(long nTaskID)
{
	long nState;
	m_nTaskID = nTaskID;
	//-------------------------------------------------------------------------
	// Common Shared Memory Assign
	//
	long nAddress;

	// Get Config Shared Memory Address
	nState = m_shMem.Create(SHARE_INFORMATION_NAME, sizeof( stSmaRootType ), &nAddress );
	if( dERROR == nState )
	{
		MessageBox(NULL,"PMC-E-[CreateSharedMemory] System Information","SystemInformation", MB_OK );
		::ExitProcess( __LINE__ );
	}
	m_stpSma = (stSmaRootType*)nAddress;

	return (nState == -1) ? FALSE : TRUE;
}

//	System Layout Configuration Data Load...
void SysLib::GetSystemConfig(stLayOutCfgType* pCfg)
{
	try{
		long	i	=	0;
		long	j	=	0;
		long	nUnitPos = 1;

		char strSN[64] = { NULL, };
		char strFileName[256] = { NULL, };

		sprintf(strFileName,"%s%s", m_szFilePath, SYS_CONFIG_FILE_NAME);
		SetScriptFileName(strFileName);

		GetInt("SYSTEM_CONFIG", "EquipmentType"	, pCfg->nEQType);
		GetInt("SYSTEM_CONFIG", "EquipmentSubType" , pCfg->nEQSubType);
		GetInt("SYSTEM_CONFIG", "EquipmentNumber" , pCfg->nEQNumber);
		//GetInt("ProcessDirection" , pCfg->nProcessDirection);

		GetInt("MELSEC_CONFIG", "MelsecBoardCount", pCfg->nMelsecBoardCount);
		GetInt("MELSEC_CONFIG", "Melsec01_ChannelNo", pCfg->stMelsecConfig[0].nChannelNo);
		GetInt("MELSEC_CONFIG", "Melsec01_InterfaceType", pCfg->stMelsecConfig[0].nInterfaceType);

	}catch(...)
	{
		printf("%s", "Config Read Error");
	}
}

// System Data Info Data Load...

BOOL SysLib::LoadSystemData()
{								
	//SetXMLFileName(CONFIG_FILE_PATH, SYS_CONFIG_FILE_NAME);
	InitConfigFilePath();
	GetSystemConfig(&GetSMAPointer()->stLayOutCfg);	//	System Config Data Load
	return TRUE;
}

/*==========================================================================
 * SMA Pointer 
 *==========================================================================*/
stSmaRootType* SysLib::GetSMAPointer()
{
	return m_stpSma;
}

/*==========================================================================
 *	Get System Configure Information Pointer
 *==========================================================================*/
stModuleCfgType* SysLib::GetModCfgPointer(long nModule)
{
	stLayOutCfgType* pSysCfg = &GetSMAPointer()->stLayOutCfg; 

	for( long i = 0; i < MAX_LAYER1_MODULE_COUNT; i++ )
	{
		if (nModule == pSysCfg->stModCfg[i].nModuleID)
		{
			return &pSysCfg->stModCfg[i];
		}
	}

	return NULL;
}

/*==========================================================================
 *	Get Module Run Information Pointer
 *==========================================================================*/
stModuleRunInfoType* SysLib::GetModRunPointer(long nModule)
{
	stSystemRunInfoType* pSysRunInfo = &GetSMAPointer()->stSysRunInfo;

	for( long i = 0; i < MAX_LAYER1_MODULE_COUNT; i++ )
	{
		if(nModule == pSysRunInfo->stModRunInfo[i].nModuleID)
		{
			return &pSysRunInfo->stModRunInfo[i];
		}
	}
	
	return NULL;
}

/*==========================================================================
 *	Get Module Data Information Pointer
 *==========================================================================*/
stModuleDataInfoType* SysLib::GetModDataPointer(long nModule)
{
	stSystemDataInfoType* pSysDataInfo = &GetSMAPointer()->stSysData;

	for( long i = 0; i < MAX_LAYER1_MODULE_COUNT; i++ )
	{
		if (nModule == pSysDataInfo->stMODDataInfo[i].nModuleID)
		{
			return &pSysDataInfo->stMODDataInfo[i];
		}
	}

	return NULL;
}

void SysLib::InitConfigFilePath()
{
	strcpy(m_szFilePath, CONFIG_FILE_PATH);
}	
