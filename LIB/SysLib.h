// InIineSma.h: interface for the InIineSma class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SYSLIB_H__
#define _SYSLIB_H__

#pragma once

#include	"../lib//SW3FALib.h"
#include	"../lib/Script.h"
#include	"../include/IPCSma.h"
#include	"../include/IPCQueue.h"

class SysLib : public CScript
{
public:
	void InitConfigFilePath();
	stSmaRootType* GetSMAPointer();
	stModuleCfgType* SysLib::GetModCfgPointer(long nModule);
	stModuleDataInfoType* SysLib::GetModDataPointer(long nModule);
	stModuleRunInfoType* SysLib::GetModRunPointer(long nModule);

	SysLib();
	virtual ~SysLib();
	
	BOOL	m_bSmaLoaded;
	stSmaRootType*	m_stpSma;
	
	BOOL SmaInitialize(long nTaskID);
	BOOL LoadSystemData();
	void GetSystemConfig(stLayOutCfgType* pCfg);


protected:
	long m_nTaskID;

	CShMem m_shMem;

	long m_nEQType;	
	char m_szFilePath[256];	//디렉토리 경로
};

#endif // _SYSLIB_H__


