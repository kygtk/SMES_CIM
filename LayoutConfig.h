#include	"InlineExtern.h"

#ifdef __cplusplus
	extern "C" {
#endif
	long WINAPI smGetLayer1ModuleCount();
	long WINAPI smGetLayer2ModuleCount(long nModule);
	long WINAPI smGetUnitConfig(long nModuleNum, long nUnitNum, stUnitCfgType* pDstUnitCfg);
	long WINAPI smGetEQType();
	long WINAPI smGetEQSubType();
	long WINAPI smGetEQNumber();
	long WINAPI	smGetSoftVersion(char* szVersion);

	long WINAPI smGetUnitName(long nModuleNum, long nUnitNum, stUnitCfgType* pDstUnitCfg);
	long WINAPI smSetUnitName(long nModuleNum, long nUnitNum, stUnitCfgType* pSrcUnitCfg);

	long WINAPI smGetUnitDesc(long nModule, long nUnit, char* szUnitDesc);

	long WINAPI smGetModuleName(long nModuleNum, char* szModuleName);
	long WINAPI smSetModuleName(long nModuleNum, char* szModuleName);

	long WINAPI smGetModuleDesc(long nModule, char* szModuleDesc);

	long WINAPI	smGetEQID(char* szEQID);
	void WINAPI smSetEQID(char* szEQID);
	
	long WINAPI smGetModelNumber(char* szMDLN);
	void WINAPI smSetModelNumber(char* szMDLN);

	long WINAPI smGetEQEvtCtrl(stEventCtrlDataType* pDstEvtCtrl);
	void WINAPI smSetEQEvtCtrl(stEventCtrlDataType* pSrcEvtCtrl);
		
	long WINAPI smGetModuleEvtCtrl(long nModule, stEventCtrlDataType* pDstEvtCtrl);
	long WINAPI smSetModuleEvtCtrl(long nModule, stEventCtrlDataType* pSrcEvtCtrl);

	long WINAPI smGetUnitEvtCtrl(long nModule, long nUnit, stEventCtrlDataType* pDstEvtCtrl);
	long WINAPI smSetUnitEvtCtrl(long nModule, long nUnit, stEventCtrlDataType* pSrcEvtCtrl);

	long	WINAPI	smGetEOIDCount();
	long	WINAPI	smGetEOMDCount(long nEOID);
	long	WINAPI	smGetEOIDParam(stEOIDTableType* pDstEOID);
	void	WINAPI	smSetEOIDParam(stEOIDTableType* pSrcEOID);
	long	WINAPI	smSetSVIDTable(stSVIDTableType* pSrcSVIDTable);
	long	WINAPI	smSetECIDTable(stECIDTableType* pSrcECIDTable);
	long	WINAPI	smSetDVIDTable(stDVIDTableType* pSrcDVIDTable);

	void WINAPI smSetModuleDesc(long nModuleNo, char* szModuleDesc);
	long WINAPI smGetPLCVersion(long nModule, char* szPLCVersion);
	void WINAPI smSetPLCVersion(long nModuleNo, char* szPLCVersion);
	long WINAPI smSetModuleNumber(long nModuleNo);
	long WINAPI smSetModelType(long nModuleNo, long nModelType);
	long WINAPI smSetMakerType(long nModuleNo, long nMakerType);
	long WINAPI smSetModuleCount(long nModuleNo, long nModuleCount);
	long WINAPI smSetRobotCount(long nModuleNo, long nRobotCount);
	long WINAPI smSetPortCount(long nModuleNo, long nPortCount);
	long WINAPI smSetUsedMelsecNetCount(long nModuleNo, long nCount);
	long WINAPI smSetModuleMelsecConfig(long nModuleNo, long nIndex, stMelsecConfigForModuleType pSrcConfig);
	long WINAPI smSetHandshakeCount(long nModuleNo, long nCount);
	void WINAPI smSetHandshakeConfig(long nModuleNo, long nHSIndex, stHandShakeConfigForModuleType pSrcHandshake);
//	void WINAPI smSetUsedSerial(long nModuleNo, long nUsedSerial);
//	void WINAPI smSetSerialConfig(long nModuleNo, stSerialCommCfgType pSrcSerialCfg);
	void WINAPI smSetTerminalID(long nModuleNo, long nTerminalID);
	void WINAPI smSetAlarmCount(long nModuleNo, long nCount);
	void WINAPI smSetRobotArmCount(long nModuleNo, long nCount);
	void WINAPI smSetUsedBuffer(long nModuleNo, long nUsedBuffer);
	void WINAPI smSetUsedBufferSlotCount(long nModuleNo, long nCount);

	BOOL IsRPCRange(long nItemNo);
#ifdef __cplusplus
}	
#endif
